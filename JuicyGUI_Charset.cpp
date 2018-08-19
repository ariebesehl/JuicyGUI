
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Charset.h"


JuicyGUI_Charset::JuicyGUI_Charset(JuicyGUI* iHostUI, JD_INDEX assignedID, const char* iFontPath, JD_I iFontSize, JD_COLOR iFontColor) {
	JD_INDEX totalSize = JUICYGUI_CHARSET_NUM_STYLES * JUICYGUI_CHARSET_SIZE;
    element.setCredentials(iHostUI, this, new JPM(), new JSPR(iHostUI->GetEngine(), totalSize), assignedID, JUICYGUI_TYPE_ID_CHARSET);
    initDefault(iFontPath, iFontSize, iFontColor);
	setStyleOffset();
	charSizes = NULL;
	charSizes = new JD_Point*[totalSize];
	JDM_NullList((void**)charSizes, &totalSize);
    createTextures();
}

void JuicyGUI_Charset::initDefault(const char* iFontpath, JD_I iFontSize, JD_COLOR iFontColor) {
    JuicyGUI_Charset_Properties* polledProps = NULL;
    JD_FLAG eventType = JUICYGUI_EVENT_NONE;
    while (element.properties->PollProperties((void**)&polledProps, &eventType)) {
        switch (eventType) {
            case JUICYGUI_EVENT_NONE:
                if (polledProps == NULL) {
                    polledProps = new JuicyGUI_Charset_Properties;
                    element.properties->SetProperties(eventType, (void*)polledProps);
                }
                polledProps->fontPath = iFontpath;
                polledProps->color = iFontColor;
                polledProps->fillColor = -iFontColor | 0xff;
                polledProps->fontSize = iFontSize;
                polledProps->style = JUICYDEFAULT_CHARSET_PROPERTIES_STYLE;
                break;
            default:
                break;
        }
    }
    defaultProperties = (JuicyGUI_Charset_Properties*)element.properties->GetProperties(JUICYGUI_EVENT_NONE);
}

JuicyGUI_Charset::~JuicyGUI_Charset() {
    if (element.properties != NULL) {
        JuicyGUI_Charset_Properties* ptrProps = NULL;
        while (element.properties->PollProperties((void**)&ptrProps, NULL)) {
            if (ptrProps != NULL) {
                delete ptrProps;
            }
        }
    }
	if (charSizes != NULL) {
        for (JD_INDEX i = 0; i < JUICYGUI_CHARSET_NUM_STYLES * JUICYGUI_CHARSET_SIZE; i++) {
            if (charSizes[i] != NULL) {
                delete charSizes[i];
                charSizes[i] = NULL;
            }
        }
        delete[] charSizes;
        charSizes = NULL;
	}
}

void JuicyGUI_Charset::PrintTXT(const char* text, JD_Point* iPosition) {
    SetCursor(iPosition);
    printer(text);
}

void JuicyGUI_Charset::PrintlnTXT(const char* text, JD_Point* iPosition) {
    SetCursor(iPosition);
    printer(text);
    carrierReturn();
}

void JuicyGUI_Charset::PrintINT(JD_I iInteger, JD_Point* iPosition) {
    SetCursor(iPosition);
    printerINT(iInteger);
}

void JuicyGUI_Charset::PrintlnINT(JD_I iInteger, JD_Point* iPosition) {
    SetCursor(iPosition);
    printerINT(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::PrintHEX(JD_I iInteger, JD_Point* iPosition) {
    SetCursor(iPosition);
    printerHEX(iInteger);
}

void JuicyGUI_Charset::PrintlnHEX(JD_I iInteger, JD_Point* iPosition) {
    SetCursor(iPosition);
    printerHEX(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::GetSizeTXT(const char* text, JD_Point* target) {
    sizer(text, target);
}

void JuicyGUI_Charset::GetSizeINT(JD_I integer, JD_Point* target) {
    sizerINT(integer, target);
}

void JuicyGUI_Charset::printer(const char* text) {
    if (text != NULL) {
        JD_INDEX i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                JD_INDEX numChar = (text[i] & JUICYGUI_CHARSET_MASK);
                switch (text[i]) {
                    default:
                        if (charSizes[numChar]->x) {
                            printerChar(numChar, 0);
                        }
                        break;
                }
            } else {
                return;
            }
            i++;
        }
    }
}

void JuicyGUI_Charset::printerINT(JD_I iInteger) {
    if (iInteger < 0) {
        printerChar(JUICYGUI_CHARSET_CHARNUM_HYPHEN, 0);
        iInteger = -iInteger;
    }
    JD_U numDigits = 1;
    JD_I exp = 10;
    while (iInteger / exp) {
        numDigits++;
        exp *= 10;
    }
    JD_U digit = 0;
    exp /= 10;
    for (JD_INDEX i = 0; i < numDigits; i++) {
        digit = (iInteger / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        printerChar(digit, 0);
        iInteger %= exp;
        exp /= 10;
    }
}

void JuicyGUI_Charset::printerHEX(JD_I iInteger) {
    if (iInteger < 0) {
        printerChar(JUICYGUI_CHARSET_CHARNUM_HYPHEN, 0);
        iInteger = -iInteger;
    }
    printerChar(JUICYGUI_CHARSET_CHARNUM_DIGIT_0, 0);
    printerChar(JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X, 0);
    JD_U numDigits = 1;
    JD_I exp = 16;
    while (iInteger / exp) {
        numDigits++;
        exp *= 16;
    }
    JD_U digit = 0;
    exp /= 16;
    for (JD_INDEX i = 0; i < numDigits; i++) {
        digit = (iInteger / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        printerChar(digit, 0);
        iInteger %= exp;
        exp /= 16;
    }
}

void JuicyGUI_Charset::newLine(bool iEnable) {
    static JD_I posX;
    if (iEnable) {
        posX = element.getPosX();
    } else {
        element.setPosX(posX);
    }
}

void JuicyGUI_Charset::carrierReturn(void) {
    newLine(false);
    element.setPosY(element.getPosY() + charSizes[JUICYGUI_CHARSET_REFERENCE_HEIGHT]->y + defaultProperties->lineMargin);
}

void JuicyGUI_Charset::printerChar(JD_INDEX iCharNum, JD_INDEX options) {
    if (element.GetSprites() != NULL) {
        iCharNum &= JUICYGUI_CHARSET_MASK;
        iCharNum += styleOffset;
        JD_Rect cursorRect = *(element.getRect());
        JDM_SetRectSize(&cursorRect, charSizes[iCharNum]);
        element.GetSprites()->AddInstruction(iCharNum, &cursorRect);
        element.setPosX(element.getPosX() + charSizes[iCharNum]->x);
    }
}

void JuicyGUI_Charset::sizer(const char* text, JD_Point* target) {
    if (text != NULL) {
        target->x = 0;
        target->y = 0;
        JD_INDEX i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                JD_INDEX numChar = text[i] & JUICYGUI_CHARSET_MASK;
                switch (text[i]) {
                    default:
                        if (charSizes[numChar + styleOffset]->x) {
                            target->x += charSizes[numChar + styleOffset]->x;
                            if (charSizes[numChar + styleOffset]->y > target->y) target->y = charSizes[numChar + styleOffset]->y;
                        }
                        break;
                }
            } else {
                return;
            }
            i++;
        }
    }
}

void JuicyGUI_Charset::sizerINT(JD_I integer, JD_Point* target) {
    target->x = 0;
    target->y = 0;
    if (integer < 0) {
        target->x += charSizes[JUICYGUI_CHARSET_CHARNUM_HYPHEN + styleOffset]->x;
        target->y = charSizes[JUICYGUI_CHARSET_CHARNUM_HYPHEN + styleOffset]->y;
        integer = -integer;
    }
    JD_U numDigits = 1;
    JD_I exp = 10;
    while (integer / exp) {
        numDigits++;
        exp *= 10;
    }
    JD_U digit = 0;
    exp /= 10;
    for (JD_INDEX i = 0; i < numDigits; i++) {
        digit = (integer / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        target->x += charSizes[digit + styleOffset]->x;
        if (charSizes[digit + styleOffset]->y > target->y) target->y = charSizes[digit + styleOffset]->y;
        integer %= exp;
        exp /= 10;
    }
}

void JuicyGUI_Charset::sizerHEX(JD_I integer, JD_Point* target) {
    target->x = 0;
    target->y = 0;
    if (integer < 0) {
        target->x += charSizes[JUICYGUI_CHARSET_CHARNUM_HYPHEN + styleOffset]->x;
        target->y = charSizes[JUICYGUI_CHARSET_CHARNUM_HYPHEN + styleOffset]->y;
        integer = -integer;
    }
    target->x += charSizes[JUICYGUI_CHARSET_CHARNUM_DIGIT_0 + styleOffset]->x;
    if (charSizes[JUICYGUI_CHARSET_CHARNUM_DIGIT_0 + styleOffset]->y > target->y) target->y = charSizes[JUICYGUI_CHARSET_CHARNUM_DIGIT_0 + styleOffset]->y;
    target->x += charSizes[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X + styleOffset]->x;
    if (charSizes[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X + styleOffset]->y > target->y) target->y = charSizes[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X + styleOffset]->y;
    JD_U numDigits = 1;
    JD_I exp = 16;
    while (integer / exp) {
        numDigits++;
        exp *= 16;
    }
    JD_U digit = 0;
    exp /= 16;
    for (JD_INDEX i = 0; i < numDigits; i++) {
        digit = (integer / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        target->x += charSizes[digit + styleOffset]->x;
        if (charSizes[digit + styleOffset]->y > target->y) target->y = charSizes[digit + styleOffset]->y;
        integer %= exp;
        exp /= 10;
    }
}


void JuicyGUI_Charset::SetFontSize(JD_I iFontSize) {
    if (defaultProperties->fontSize != iFontSize) {
        defaultProperties->fontSize = iFontSize;
        resetTextures();
    }
}

void JuicyGUI_Charset::SetCursor(JD_Point* iPosition) {
    if (iPosition != NULL) {
        element.setPos(iPosition);
        newLine(true);
    }
}

void JuicyGUI_Charset::GetCursor(JD_Point* oPosition) {
    if (oPosition != NULL) {
        element.getPos(oPosition);
    }
}

bool JuicyGUI_Charset::createTextures() {
	if (element.GetSprites() != NULL && element.getEngine() != NULL && charSizes != NULL) {
		if (defaultProperties->fontPath != NULL) {
            for (JD_INDEX style = 0; style < JUICYGUI_CHARSET_NUM_STYLES; style++) {
                JD_FLAG fontStyle = style ? (JUICYGUI_CHARSET_STYLE_BOLD << (style - 1)) : JUICYGUI_CHARSET_STYLE_NORMAL;
                char charEnum[] = {0, '\0'};
                char* ptrChar = &charEnum[0];
                for (JD_INDEX i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
                    JD_INDEX currentIndex = i + (style * JUICYGUI_CHARSET_SIZE);
                    element.GetSprites()->AddSpecificSprite(currentIndex, element.getEngine()->CreateSpriteText(ptrChar, defaultProperties->fontPath, defaultProperties->fontSize, defaultProperties->color, fontStyle));
                    if (charSizes[currentIndex] == NULL) {charSizes[currentIndex] = new JD_Point;}
                    if (element.GetSprites()->GetSprite(currentIndex) != NULL) {
                        *charSizes[currentIndex] = element.GetSprites()->GetSprite(currentIndex)->dimensions;
                    } else {
                        JDM_EmptyPoint(charSizes[currentIndex]);
                    }
                    (*ptrChar)++;
                }
            }
            defaultProperties->lineMargin = charSizes[JUICYGUI_CHARSET_REFERENCE_HEIGHT]->y * JUICYGUI_CHARSET_LINE_MARGIN_PERCENT / 100;
            return true;
		}
	}
	return false;
}

void JuicyGUI_Charset::resetTextures() {
	if (element.GetSprites() != NULL) {
        element.GetSprites()->Reset();
	}
    createTextures();
}

void JuicyGUI_Charset::setStyleOffset() {
    styleOffset = JDM_GetFlagIndex(defaultProperties->style) * JUICYGUI_CHARSET_SIZE;
}

void JuicyGUI_Charset::SetStyle(JD_FLAG iStyle){
    if (JDM_GetFlagIndex(iStyle) <= JUICYGUI_CHARSET_NUM_STYLES) {
        defaultProperties->style = iStyle;
        setStyleOffset();
    }
}

