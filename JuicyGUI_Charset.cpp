
#include "Juicy_SDL.h"

#include "JuicyGUI_Element.h"
#include "JuicyGUI_Charset.h"
#include "JuicyGUI_Definitions.h"


JuicyGUI_Charset::JuicyGUI_Charset(JuicyGUI* iHostUI, JuicyGUI_ID assignedID, const char* filePath, int fontSize, JuicyGUI_Color fontColor) {
    element.setCredentials(iHostUI, this, assignedID, JUICYGUI_TYPE_ID_CHARSET);
    properties.fontPath = filePath;
    properties.fontSize = fontSize;
    properties.color = fontColor;
    properties.fillColor = ~fontColor | 0xff;
    _ctrPrintjobs = 0;
    initTextures();
    createTextures();
}

JuicyGUI_Charset::~JuicyGUI_Charset() {
    destroyTextures();
}

void JuicyGUI_Charset::printTXT(const char* text, SDL_Point* iPosition) {
    setCursor(iPosition);
    printer(text);
}

void JuicyGUI_Charset::printlnTXT(const char* text, SDL_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printer(text);
    carrierReturn();
}

void JuicyGUI_Charset::printINT(int iInteger, SDL_Point* iPosition) {
    setCursor(iPosition);
    printerINT(iInteger);
}

void JuicyGUI_Charset::printlnINT(int iInteger, SDL_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printerINT(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::printHEX(int iInteger, SDL_Point* iPosition) {
    setCursor(iPosition);
    printerHEX(iInteger);
}

void JuicyGUI_Charset::printlnHEX(int iInteger, SDL_Point* iPosition) {
    setCursor(iPosition);
    newLine(true);
    printerHEX(iInteger);
    carrierReturn();
}

void JuicyGUI_Charset::getSizeTXT(const char* text, SDL_Point* target) {
    sizer(text, target);
}

void JuicyGUI_Charset::getSizeINT(int integer, SDL_Point* target) {
    sizerINT(integer, target);
}

void JuicyGUI_Charset::printer(const char* text) {
    if (text != NULL) {
        uint32_t i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                uint32_t numChar = text[i] & JUICYGUI_CHARSET_MASK;
                switch (text[i]) {
                    default:
                        if (_textureWidth[numChar]) {
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

void JuicyGUI_Charset::printerINT(int iInteger) {
    if (iInteger < 0) {
        printerChar(JUICYGUI_CHARSET_CHARNUM_HYPHEN, 0);
        iInteger = -iInteger;
    }
    uint32_t numDigits = 1;
    int exp = 10;
    while (iInteger / exp) {
        numDigits++;
        exp *= 10;
    }
    uint32_t digit = 0;
    exp /= 10;
    for (uint32_t i = 0; i < numDigits; i++) {
        digit = (iInteger / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        printerChar(digit, 0);
        iInteger %= exp;
        exp /= 10;
    }
}

void JuicyGUI_Charset::printerHEX(int iInteger) {
    if (iInteger < 0) {
        printerChar(JUICYGUI_CHARSET_CHARNUM_HYPHEN, 0);
        iInteger = -iInteger;
    }
    printerChar(JUICYGUI_CHARSET_CHARNUM_DIGIT_0, 0);
    printerChar(JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X, 0);
    uint32_t numDigits = 1;
    int exp = 16;
    while (iInteger / exp) {
        numDigits++;
        exp *= 16;
    }
    uint32_t digit = 0;
    exp /= 16;
    for (uint32_t i = 0; i < numDigits; i++) {
        digit = (iInteger / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        printerChar(digit, 0);
        iInteger %= exp;
        exp /= 16;
    }
}

void JuicyGUI_Charset::newLine(bool iEnable) {
    static SDL_Point cache;
    if (iEnable) {
        element.getPos(&cache);
    } else {
        element.setPos(&cache);
    }
}

void JuicyGUI_Charset::carrierReturn(void) {
    newLine(false);
    element.setPosY(element.getPosY() + _textureHeight[JUICYGUI_CHARSET_REFERENCE_HEIGHT] + properties.lineMargin);
}

void JuicyGUI_Charset::printerChar(uint32_t iCharNum, uint32_t options) {
    if (_ctrPrintjobs < JUICYGUI_CHARSET_MAX_PRINTJOBS) {
        iCharNum &= JUICYGUI_CHARSET_MASK;
        element.getPos(&(_printjobs[_ctrPrintjobs].position));
        _printjobs[_ctrPrintjobs].charNum = iCharNum;
        _ctrPrintjobs++;
        element.setPosX(element.getPosX() + _textureWidth[iCharNum]);
    }
}

void JuicyGUI_Charset::sizer(const char* text, SDL_Point* target) {
    if (text != NULL) {
        target->x = 0;
        target->y = 0;
        uint32_t i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                uint32_t numChar = text[i] & JUICYGUI_CHARSET_MASK;
                switch (text[i]) {
                    default:
                        if (_textureWidth[numChar]) {
                            target->x += _textureWidth[numChar];
                            if (_textureHeight[numChar] > target->y) target->y = _textureHeight[numChar];
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

void JuicyGUI_Charset::sizerINT(int integer, SDL_Point* target) {
    target->x = 0;
    target->y = 0;
    if (integer < 0) {
        target->x += _textureWidth[JUICYGUI_CHARSET_CHARNUM_HYPHEN];
        target->y = _textureHeight[JUICYGUI_CHARSET_CHARNUM_HYPHEN];
        integer = -integer;
    }
    uint32_t numDigits = 1;
    int exp = 10;
    while (integer / exp) {
        numDigits++;
        exp *= 10;
    }
    uint32_t digit = 0;
    exp /= 10;
    for (uint32_t i = 0; i < numDigits; i++) {
        digit = (integer / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        target->x += _textureWidth[digit];
        if (_textureHeight[digit] > target->y) target->y = _textureHeight[digit];
        integer %= exp;
        exp /= 10;
    }
}

void JuicyGUI_Charset::sizerHEX(int integer, SDL_Point* target) {
    target->x = 0;
    target->y = 0;
    if (integer < 0) {
        target->x += _textureWidth[JUICYGUI_CHARSET_CHARNUM_HYPHEN];
        target->y = _textureHeight[JUICYGUI_CHARSET_CHARNUM_HYPHEN];
        integer = -integer;
    }
    target->x += _textureWidth[JUICYGUI_CHARSET_CHARNUM_DIGIT_0];
    if (_textureHeight[JUICYGUI_CHARSET_CHARNUM_DIGIT_0] > target->y) target->y = _textureHeight[JUICYGUI_CHARSET_CHARNUM_DIGIT_0];
    target->x += _textureWidth[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X];
    if (_textureHeight[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X] > target->y) target->y = _textureHeight[JUICYGUI_CHARSET_CHARNUM_LOWERCASE_X];
    uint32_t numDigits = 1;
    int exp = 16;
    while (integer / exp) {
        numDigits++;
        exp *= 16;
    }
    uint32_t digit = 0;
    exp /= 16;
    for (uint32_t i = 0; i < numDigits; i++) {
        digit = (integer / exp);
        digit += (digit > 9) ? JUICYGUI_CHARSET_CHARNUM_HEX_OFFSET : JUICYGUI_CHARSET_CHARNUM_INT_OFFSET;
        target->x += _textureWidth[digit];
        if (_textureHeight[digit] > target->y) target->y = _textureHeight[digit];
        integer %= exp;
        exp /= 10;
    }
}


void JuicyGUI_Charset::setFontSize(int fontSize) {
    if (properties.fontSize != fontSize) {
        properties.fontSize = fontSize;
        resetTextures();
    }
}

void JuicyGUI_Charset::setCursor(SDL_Point* iPosition) {
    if (iPosition != NULL) {
        element.setPos(iPosition);
    }
}

void JuicyGUI_Charset::getCursor(SDL_Point* oPosition) {
    if (oPosition != NULL) {
        element.getPos(oPosition);
    }
}

void JuicyGUI_Charset::draw() {
    SDL_Rect drawRect;
    for (uint32_t i = 0; i < _ctrPrintjobs; i++) {
        JSDL_SetRectPos(&drawRect, &(_printjobs[i].position));
        drawRect.w = _textureWidth[_printjobs[i].charNum];
        drawRect.h = _textureHeight[_printjobs[i].charNum];
        element.getHost()->RenderTexture(_texture[_printjobs[i].charNum], &drawRect);
    }
    _ctrPrintjobs = 0;
}

void JuicyGUI_Charset::initTextures() {
    for (uint32_t i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
        _textureWidth[i] = 0;
        _textureHeight[i] = 0;
        _texture[i] = NULL;
        _textureFill[i] = NULL;
    }
    properties.lineMargin = 0;
}


bool JuicyGUI_Charset::createTextures() {
    if (properties.fontPath != NULL) {
        TTF_Font* _Font = TTF_OpenFont(properties.fontPath, properties.fontSize);
        if (_Font != NULL) {
            char charEnum[] = {0, '\0'};
            char* ptrChar = &charEnum[0];
            SDL_Point fillDim;
            for (uint32_t i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
                TTF_SizeText(_Font, ptrChar, &_textureWidth[i], &_textureHeight[i]);
                _texture[i] = element.getHost()->CreateTextureTXT(ptrChar, NULL, _Font, properties.color);
                fillDim.x = _textureWidth[i];
                fillDim.y = _textureHeight[i];
                _textureFill[i] = element.getHost()->CreateTextureFill(&fillDim, properties.fillColor);
                (*ptrChar)++;
            }
            properties.lineMargin = _textureHeight[JUICYGUI_CHARSET_REFERENCE_HEIGHT] / JUICYGUI_CHARSET_LINE_MARGIN_DIVISOR;
            TTF_CloseFont(_Font);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void JuicyGUI_Charset::resetTextures() {
    destroyTextures();
    initTextures();
    createTextures();
}

void JuicyGUI_Charset::destroyTextures() {
    for (uint32_t i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
        if (_texture[i] != NULL) {
            SDL_DestroyTexture(_texture[i]);
        }
        if (_textureFill[i] != NULL) {
            SDL_DestroyTexture(_textureFill[i]);
        }
    }
}

