
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Button.h"

JuicyGUI_Button_Properties _defStateNone;
JuicyGUI_Button_Properties _defStatePressed;
JuicyGUI_Button_Properties _defStateHover;


JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iID, JD_FLAG iFlag, const char* iLabel, const JD_Rect* iRect) {
    element.setCredentials(iHostUI, this, iID, JUICYGUI_TYPE_ID_BUTTON);
    element.setRect(iRect);
    flag = iFlag;
    initStates();
	textureEngine = new JSPR(element.getEngine());
    initDefault(iLabel);
    createTextures();
}

JuicyGUI_Button::~JuicyGUI_Button() {
	if (textureEngine != NULL) delete textureEngine;
    destroyStates();
}

void JuicyGUI_Button::initDefault(const char* iLabel) {
    _defStateNone.label = (iLabel != NULL) ? iLabel : NULL;
    _defStateNone.fontPath = GAME_UI_FONT_TYPE;
    _defStateNone.frameSize = GAME_UI_BUTTON_FRAME_SIZE;
    _defStateNone.color = GAME_UI_BUTTON_COLOR;
    _defStateNone.frameColor = GAME_UI_BUTTON_FRAME_COLOR;
    _defStateNone.fontColor = GAME_UI_BUTTON_FONT_COLOR;
    _defStateHover = _defStateNone;
    _defStateHover.color = GAME_UI_BUTTON_COLOR_HOVER;
    _defStateHover.frameColor = GAME_UI_BUTTON_FRAME_COLOR_HOVER;
    _defStateHover.fontColor = GAME_UI_BUTTON_FONT_COLOR_HOVER;
    _defStatePressed = _defStateNone;
    _defStatePressed.color = GAME_UI_BUTTON_COLOR_PRESSED;
    _defStatePressed.frameColor = GAME_UI_BUTTON_FRAME_COLOR_PRESSED;
    _defStatePressed.fontColor = GAME_UI_BUTTON_FONT_COLOR_PRESSED;
    SetProperties(JUICYGUI_ACTION_NONE, &_defStateNone);
    SetProperties(JUICYGUI_ACTION_PRESSED, &_defStatePressed);
    SetProperties(JUICYGUI_ACTION_HOVER, &_defStateHover);
}

bool JuicyGUI_Button::GetProperties(JuicyGUI_Button_Properties* oTemplate) {
    return GetProperties(JUICYGUI_ACTION_NONE, oTemplate);
}
bool JuicyGUI_Button::GetProperties(JD_FLAG iActionID, JuicyGUI_Button_Properties* oTemplate) {
    JD_INDEX index = element.getActionIndex(iActionID);
    bool actionOK = (oTemplate != NULL);
    actionOK &= (iActionID && index) || (!bool(iActionID) && !bool(index));
    if (actionOK) {
        *oTemplate = *(states[index]);
    }
    return actionOK;
}
bool JuicyGUI_Button::SetProperties(JuicyGUI_Button_Properties* iTemplate) {
    bool settingOK = true;
    for (JD_INDEX i = 0; i < JUICYGUI_NUM_ACTION; i++) {
        settingOK &= SetProperties(i, iTemplate);
    }
    return settingOK;
}
bool JuicyGUI_Button::SetProperties(JD_FLAG iActionID, JuicyGUI_Button_Properties* iProperties) {
    JD_INDEX index = element.getActionIndex(iActionID);
    if ((iActionID && index) || (!bool(iActionID) && !bool(index))) {
        if (states[index] != NULL) {
            *(states[index]) = *iProperties;
        } else {
            states[index] = new JuicyGUI_Button_Properties;
            *(states[index]) = *iProperties;
        }
		resetTextures();
		return true;
    }
    return false;
}

void JuicyGUI_Button::initStates() {
    states = new JuicyGUI_Button_Properties*[JUICYGUI_NUM_ACTION];
    JDM_NullList((void**)states, JUICYGUI_NUM_ACTION);
}


void JuicyGUI_Button::destroyStates() {
    if (states != NULL) {
        for (JD_INDEX i = 0; i < JUICYGUI_NUM_ACTION; i++) {
            if (states[i] != NULL) {
                delete states[i];
            }
        }
        JDM_DeleteList((void***)&states, NULL);
    }
}

void JuicyGUI_Button::resetStates() {
    destroyStates();
    initStates();
}

void JuicyGUI_Button::createTextures() {
    if (textureEngine != NULL) {
        JD_Point buttonSize;
        element.getSize(&buttonSize);
        if (JDM_IsNotEmptyPoint(&buttonSize)) {
            for (JD_INDEX i = 0; i < JUICYGUI_NUM_ACTION; i++) {
                if (states[i] != NULL) {
                    JD_COLOR* pixelData = new JD_COLOR[buttonSize.x * buttonSize.y];
                    for (JD_I y = 0; y < buttonSize.y; y++) {
                        for (JD_I x = 0; x < buttonSize.x; x++) {
                            if ((x >= states[i]->frameSize) && (x < (buttonSize.x - states[i]->frameSize)) && (y >= states[i]->frameSize) && (y < (buttonSize.y - states[i]->frameSize))) {
                                pixelData[x + y * buttonSize.x] = states[i]->color;
                            } else {
                                pixelData[x + y * buttonSize.x] = states[i]->frameColor;
                            }
                        }
                    }
                    JuicySprite* buttonSprite = element.getEngine()->CreateSprite(&buttonSize, pixelData);
                    delete[] pixelData;
                    switch (flag & JUICYGUI_BUTTON_FLAG_MASK_TYPE) {
                        case JUICYGUI_BUTTON_FLAG_IMAGE: {
                                /*JD_Point imgSize;
                                JD_COLOR* imgPixels = element.getEngine()->GetPixelsFromImage(states[i]->label, &imgSize);
                                if (imgPixels != NULL) {
                                    JD_Point blendedSize;
                                    JD_COLOR* blendedPixels = NULL;
                                    blendedPixels = element.getEngine()->BlendPixels(pixelData, &buttonSize, imgPixels, &imgSize, &blendedSize);
                                    if (blendedPixels != NULL) {
                                        delete[] pixelData;
                                        buttonSize = blendedSize;
                                        pixelData = blendedPixels;
                                    }
                                    delete[] imgPixels;
                                }*/
                            }
                            break;
                        case JUICYGUI_BUTTON_FLAG_NORMAL: {
                                JuicySprite* textSprite = element.getEngine()->CreateSpriteText(states[i]->label, states[i]->fontPath, element.getHeight() / 2, states[i]->fontColor, JUICYGUI_CHARSET_STYLE_NORMAL);
                                if (textSprite != NULL) {
                                    element.getEngine()->BlendSprites(textSprite, buttonSprite);
                                    element.getEngine()->FreeSprite(textSprite);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    textureEngine->AddSprite(buttonSprite);
                } else {
                    textureEngine->AddSprite(NULL);
                }
            }
            element.SetTextureEngine(textureEngine);
        }
    }
}

void JuicyGUI_Button::resetTextures() {
	textureEngine->Reset();
	createTextures();
}


