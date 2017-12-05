
#include "Juicy_SDL.h"

#include "JuicyGUI_Element.h"

#include "JuicyGUI_Button.h"
#include "JuicyGUI_Definitions.h"


int DEF_UI_Button_Frame_Size[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE};
const char* DEF_UI_Button_Font_Type[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE};
JuicyGUI_Color DEF_UI_Button_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_COLOR, GAME_UI_BUTTON_COLOR_HOVER, GAME_UI_BUTTON_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Frame_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FRAME_COLOR, GAME_UI_BUTTON_FRAME_COLOR_HOVER, GAME_UI_BUTTON_FRAME_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Font_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FONT_COLOR, GAME_UI_BUTTON_FONT_COLOR_HOVER, GAME_UI_BUTTON_FONT_COLOR_PRESSED};

JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* iHostUI, JuicyGUI_ID iID, uint32_t iSubType, const char* iLabel, const SDL_Rect* iRect) {
    element.setCredentials(iHostUI, this, iID, JUICYGUI_TYPE_ID_BUTTON);
    element.setRect(iRect);
    _subType = iSubType;
    initTextures();
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        properties[i].label = iLabel;
        properties[i].frameSize = DEF_UI_Button_Frame_Size[i];
        properties[i].color = DEF_UI_Button_Color[i];
        properties[i].frameColor = DEF_UI_Button_Frame_Color[i];
        properties[i].fontColor = DEF_UI_Button_Font_Color[i];
        properties[i].fontPath = DEF_UI_Button_Font_Type[i];
    }
    initTextures();
    createTextures();
}

JuicyGUI_Button::~JuicyGUI_Button() {
    destroyTextures();
}

void JuicyGUI_Button::draw() {
    if (element.getFlag() & JUICYGUI_ELEMENTFLAG_SHOW) {
        if (_texture[getTextureID()] != NULL)  {
            element.getHost()->RenderTexture(_texture[getTextureID()], element.getRect());
        }
    }
}

uint32_t JuicyGUI_Button::getTextureID() {
    return getTextureID(element.getAction());
}

uint32_t JuicyGUI_Button::getTextureID(uint32_t iActionID) {
    uint32_t textureID = JUICYGUI_BUTTON_TEXTURE_ID_NONE;
    switch (iActionID) {
        default:
        case JUICYGUI_ACTION_NONE:
            break;
        case JUICYGUI_ACTION_HOVER:
            textureID = JUICYGUI_BUTTON_TEXTURE_ID_HOVER;
            break;
        case JUICYGUI_ACTION_PRESSED:
            textureID = JUICYGUI_BUTTON_TEXTURE_ID_PRESSED;
            break;
        case JUICYGUI_ACTION_RELEASED:
            textureID = JUICYGUI_BUTTON_TEXTURE_ID_RELEASED;
            break;
    }
    return textureID;
}

void JuicyGUI_Button::initTextures() {
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        _texture[i] = NULL;
    }
}

void JuicyGUI_Button::createTextures() {
    if (!JSDL_IsRectEmpty(element.getRect())) {
        for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
            SDL_Point buttonSize;
            element.getSize(&buttonSize);
            SDL_Surface* cacheDraw = element.getHost()->CreateSurfaceFill(&buttonSize, properties[i].frameColor);
            SDL_Surface* cacheContent = NULL;
            SDL_Rect cacheContentRect;
            cacheContentRect = *(element.getRect());
            cacheContentRect.x = properties[i].frameSize;
            cacheContentRect.y = properties[i].frameSize;
            cacheContentRect.w -= (properties[i].frameSize << 1);
            cacheContentRect.h -= (properties[i].frameSize << 1);
            switch (_subType) {
                case JUICYGUI_BUTTON_SUBTYPE_IMAGE: {
                        if (properties[i].label != NULL) {
                            cacheContent = element.getHost()->CreateSurfacePNG(properties[i].label);
                            SDL_BlitScaled(cacheContent, NULL, cacheDraw, &cacheContentRect);
                        }
                    }
                    break;
                case JUICYGUI_BUTTON_SUBTYPE_BASIC: {
                        SDL_FillRect(cacheDraw, &cacheContentRect, properties[i].color);
                        SDL_Point txtSize;
                        txtSize.x = 0;
                        txtSize.y = 0;
                        if (properties[i].label != NULL) {
                            TTF_Font* loadedFont = TTF_OpenFont(properties[i].fontPath, (buttonSize.y >> 1));
                            cacheContent = element.getHost()->CreateSurfaceTXT(properties[i].label, &txtSize, loadedFont, properties[i].fontColor);
                            TTF_CloseFont(loadedFont);
                        }
                        cacheContentRect.w = txtSize.x;
                        cacheContentRect.h = txtSize.y;
                        cacheContentRect.x = ((buttonSize.x - txtSize.x) >> 1);
                        cacheContentRect.y = ((buttonSize.y - txtSize.y) >> 1);
                        SDL_BlitScaled(cacheContent, NULL, cacheDraw, &cacheContentRect);
                    }
                    break;
                default:
                    break;
            }
            _texture[i] = element.getHost()->CreateTextureSurface(cacheDraw);
            SDL_FreeSurface(cacheDraw);
            if (cacheContent != NULL) SDL_FreeSurface(cacheContent);
        }
    }
}

void JuicyGUI_Button::resetTextures() {
    destroyTextures();
    initTextures();
    createTextures();
}

void JuicyGUI_Button::destroyTextures() {
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        if (_texture[i] != NULL) SDL_DestroyTexture(_texture[i]);
    }
}
