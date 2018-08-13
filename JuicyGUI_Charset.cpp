
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Charset.h"


JuicyGUI_Charset::JuicyGUI_Charset(JuicyGUI* iHostUI, JD_INDEX assignedID, const char* filePath, JD_I fontSize, JD_COLOR fontColor) {
    element.setCredentials(iHostUI, this, assignedID, JUICYGUI_TYPE_ID_CHARSET);
    properties.fontPath = filePath;
    properties.fontSize = fontSize;
    properties.color = fontColor;
    properties.fillColor = ~fontColor | 0xff;
	properties.style = JUICYGUI_CHARSET_STYLE_NORMAL;
	setStyleOffset();
	JD_INDEX totalSize = JUICYGUI_CHARSET_NUM_STYLES * JUICYGUI_CHARSET_SIZE;
	textureEngine = NULL;
	textureEngine = new JSPR(element.getEngine(), totalSize);
	element.SetTextureEngine(textureEngine);
	charSizes = NULL;
	charSizes = new JD_Point*[totalSize];
	JDM_NullList((void**)charSizes, &totalSize);
    createTextures();
}

JuicyGUI_Charset::~JuicyGUI_Charset() {
	if (textureEngine != NULL) {
        delete textureEngine;
        textureEngine = NULL;
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

void JuicyGUI_Charset::printTXT(const char* text, JD_Point* iPosition) {
    setCursor(iPosition);
    printer(text);
}

void JuicyGUI_Charset::printlnTXT(const char* text, JD_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printer(text);
    carrierReturn();
}

void JuicyGUI_Charset::printINT(JD_I iInteger, JD_Point* iPosition) {
    setCursor(iPosition);
    printerINT(iInteger);
}

void JuicyGUI_Charset::printlnINT(JD_I iInteger, JD_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printerINT(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::printHEX(JD_I iInteger, JD_Point* iPosition) {
    setCursor(iPosition);
    printerHEX(iInteger);
}

void JuicyGUI_Charset::printlnHEX(JD_I iInteger, JD_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printerHEX(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::getSizeTXT(const char* text, JD_Point* target) {
    sizer(text, target);
}

void JuicyGUI_Charset::getSizeINT(JD_I integer, JD_Point* target) {
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
    static JD_Point cache;
    if (iEnable) {
        element.getPos(&cache);
    } else {
        element.setPos(&cache);
    }
}

void JuicyGUI_Charset::carrierReturn(void) {
    newLine(false);
    element.setPosY(element.getPosY() + charSizes[JUICYGUI_CHARSET_REFERENCE_HEIGHT]->x + properties.lineMargin);
}

void JuicyGUI_Charset::printerChar(JD_INDEX iCharNum, JD_INDEX options) {
    if (textureEngine != NULL) {
        iCharNum &= JUICYGUI_CHARSET_MASK;
        iCharNum += styleOffset;
        JD_Rect cursorRect = *(element.getRect());
        JDM_SetRectSize(&cursorRect, charSizes[iCharNum]);
        textureEngine->AddInstruction(iCharNum, &cursorRect);
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


void JuicyGUI_Charset::setFontSize(JD_I iFontSize) {
    if (properties.fontSize != iFontSize) {
        properties.fontSize = iFontSize;
        resetTextures();
    }
}

void JuicyGUI_Charset::setCursor(JD_Point* iPosition) {
    if (iPosition != NULL) {
        element.setPos(iPosition);
    }
}

void JuicyGUI_Charset::getCursor(JD_Point* oPosition) {
    if (oPosition != NULL) {
        element.getPos(oPosition);
    }
}

bool JuicyGUI_Charset::createTextures() {
	if (textureEngine != NULL && charSizes != NULL) {
		if (properties.fontPath != NULL) {
            JD_Point charSize;
            for (JD_INDEX style = 0; style < JUICYGUI_CHARSET_NUM_STYLES; style++) {
                JD_FLAG fontStyle = style ? (JUICYGUI_CHARSET_STYLE_BOLD << (style - 1)) : JUICYGUI_CHARSET_STYLE_NORMAL;
                char charEnum[] = {0, '\0'};
                char* ptrChar = &charEnum[0];
                for (JD_INDEX i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
                    JD_INDEX currentIndex = i + (style * JUICYGUI_CHARSET_SIZE);
                    JD_COLOR* charPixels = element.getEngine()->GetPixelsFromText(ptrChar, &charSize, properties.fontPath, properties.fontSize, properties.color, fontStyle);
                    textureEngine->AddSpecificSprite(currentIndex, element.getEngine()->CreateSprite(&charSize, charPixels));
                    if (charPixels != NULL) {delete[] charPixels;}
                    if (charSizes[currentIndex] == NULL) {charSizes[currentIndex] = new JD_Point;}
                    *charSizes[currentIndex] = charSize;
                    (*ptrChar)++;
                }
            }
            properties.lineMargin = charSizes[JUICYGUI_CHARSET_REFERENCE_HEIGHT]->y / JUICYGUI_CHARSET_LINE_MARGIN_DIVISOR;
            return true;
		}
	}
	return false;
}

void JuicyGUI_Charset::resetTextures() {
	textureEngine->Reset();
    createTextures();
}

void JuicyGUI_Charset::setStyleOffset() {
    styleOffset = JDM_GetFlagIndex(properties.style) * JUICYGUI_CHARSET_SIZE;
}

void JuicyGUI_Charset::setStyle(JD_FLAG iStyle){
    if (JDM_GetFlagIndex(iStyle) <= JUICYGUI_CHARSET_NUM_STYLES) {
        properties.style = iStyle;
        setStyleOffset();
    }
}

