
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
    if (!(*(element.getRect()) == 0) && textureEngine != NULL) {
        for (JD_INDEX i = 0; i < JUICYGUI_NUM_ACTION; i++) {
            if (states[i] != NULL) {
                JD_Point buttonSize;
                element.getSize(&buttonSize);
                SDL_Surface* cacheDraw = element.getEngine()->CreateSurfaceFill(&buttonSize, states[i]->frameColor);
                JD_Rect cacheContentRect;
                cacheContentRect.x = states[i]->frameSize;
                cacheContentRect.y = states[i]->frameSize;
                cacheContentRect.w = element.getWidth() - (states[i]->frameSize << 1);
                cacheContentRect.h = element.getHeight() - (states[i]->frameSize << 1);
                buttonSize.x = cacheContentRect.w;
                buttonSize.y = cacheContentRect.h;
                SDL_Surface* cacheContent = element.getEngine()->CreateSurfaceFill(&buttonSize, states[i]->color);
                element.getEngine()->BlitSurfaces(cacheContent, cacheDraw, NULL, &cacheContentRect);
                switch (flag & JUICYGUI_BUTTON_FLAG_MASK_TYPE) {
                    case JUICYGUI_BUTTON_FLAG_IMAGE: {
                            if (states[i]->label != NULL) {
                                cacheContent = element.getEngine()->CreateSurfaceIMG(states[i]->label);
                            }
                        }
                        break;
                    case JUICYGUI_BUTTON_FLAG_NORMAL: {
                            JD_Point txtSize;
                            txtSize.x = 0;
                            txtSize.y = 0;
                            if (states[i]->label != NULL) {
                                TTF_Font* loadedFont = TTF_OpenFont(states[i]->fontPath, (buttonSize.y >> 1));
                                cacheContent = element.getEngine()->CreateSurfaceTXT(states[i]->label, &txtSize, loadedFont, states[i]->fontColor);
                                TTF_CloseFont(loadedFont);
                            }
                            cacheContentRect.w = txtSize.x;
                            cacheContentRect.h = txtSize.y;
                            cacheContentRect.x = ((buttonSize.x - txtSize.x) >> 1);
                            cacheContentRect.y = ((buttonSize.y - txtSize.y) >> 1);
                        }
                        break;
                    default:
                        break;
                }
                element.getEngine()->BlitSurfaces(cacheContent, cacheDraw, NULL, &cacheContentRect);
				textureEngine->AddTexture(element.getEngine()->CreateTexture(cacheDraw));
                if (cacheDraw != NULL) SDL_FreeSurface(cacheDraw);
                if (cacheContent != NULL) SDL_FreeSurface(cacheContent);
            } else {
				textureEngine->AddTexture(NULL);
			}
        }
        element.SetTextureEngine(textureEngine);
    }
}

void JuicyGUI_Button::resetTextures() {
	textureEngine->Reset();
	createTextures();
}


