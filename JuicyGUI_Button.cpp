#include "JuicyGUI_Button.h"
#include "JuicyGUI_Definitions.h"


int DEF_UI_Button_Frame_Size[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE};
const char* DEF_UI_Button_Font_Type[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE};
int DEF_UI_Button_Font_Size[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_FONT_SIZE, GAME_UI_FONT_SIZE, GAME_UI_FONT_SIZE};
JuicyGUI_Color DEF_UI_Button_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_COLOR, GAME_UI_BUTTON_COLOR_HOVER, GAME_UI_BUTTON_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Frame_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FRAME_COLOR, GAME_UI_BUTTON_FRAME_COLOR_HOVER, GAME_UI_BUTTON_FRAME_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Font_Color[JUICYGUI_BUTTON_NUM_STATES] = {GAME_UI_BUTTON_FONT_COLOR, GAME_UI_BUTTON_FONT_COLOR_HOVER, GAME_UI_BUTTON_FONT_COLOR_PRESSED};


JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* hostInterface, JuicyGUI_ID id, uint32_t subType, JuicyGUI_Button_Struct* style, const char* label) {
    _id = id;
    _type = JUICYGUI_TYPE_ID_BUTTON;
    _action = JUICYGUI_ACTION_NONE;
    _host = hostInterface;
    _subType = subType;
    _elementFlag = JUICYGUI_ELEMENTFLAG_SHOW;
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        _texture[i] = NULL;
    }
    if (style != NULL) {
        buttonProperties = *style;
        createTextures();
    } else {
        for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
            buttonProperties._label[i] = label;
            buttonProperties._frameSize[i] = DEF_UI_Button_Frame_Size[i];
            buttonProperties._color[i] = DEF_UI_Button_Color[i];
            buttonProperties._frameColor[i] = DEF_UI_Button_Frame_Color[i];
            buttonProperties._fontColor[i] = DEF_UI_Button_Font_Color[i];
            buttonProperties._font[i] = DEF_UI_Button_Font_Type[i];
            buttonProperties._fontSize[i] = DEF_UI_Button_Font_Size[i];
        }
        buttonProperties._rect.x = 0;
        buttonProperties._rect.y = 0;
        buttonProperties._rect.w = 0;
        buttonProperties._rect.h = 0;
    }
    _host->RegisterElement(this, _type);
}

JuicyGUI_Button::~JuicyGUI_Button() {
    clearTextures();
}

void JuicyGUI_Button::enable() {
    _elementFlag &= ~JUICYGUI_ELEMENTFLAG_DISABLED;
}

void JuicyGUI_Button::disable() {
    _elementFlag |= JUICYGUI_ELEMENTFLAG_DISABLED;
}

void JuicyGUI_Button::setDraw(bool show) {
    if (show) {
        _elementFlag |= JUICYGUI_ELEMENTFLAG_SHOW;
    } else {
        _elementFlag &= ~JUICYGUI_ELEMENTFLAG_SHOW;
    }
}


void JuicyGUI_Button::setRect(SDL_Rect* rect) {
    if ((buttonProperties._rect.w != rect->w) || (buttonProperties._rect.h != rect->h)){
        buttonProperties._rect = *rect;
        createTextures();
    } else {
        buttonProperties._rect = *rect;
    }
}

void JuicyGUI_Button::draw() {
    if (_texture[_action] != NULL)  {
        SDL_RenderCopy(_host->_Renderer, _texture[_action], NULL, &buttonProperties._rect);
    }
}

void JuicyGUI_Button::clearTextures() {
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        if (_texture[i] != NULL) {
            SDL_DestroyTexture(_texture[i]);
            _texture[i] = NULL;
        }
    }
}

void JuicyGUI_Button::createTextures() {
    clearTextures();
    for (int i = 0; i < JUICYGUI_BUTTON_NUM_STATES; i++) {
        SDL_Surface* cacheDraw = SDL_CreateRGBSurface(0, buttonProperties._rect.w, buttonProperties._rect.h, JUICYGUI_LOWLVL_DEPTH, JUICYGUI_LOWLVL_MASKR, JUICYGUI_LOWLVL_MASKG, JUICYGUI_LOWLVL_MASKB, JUICYGUI_LOWLVL_MASKA);
        SDL_Surface* cacheContent = NULL;
        SDL_Rect cacheContentRect;
        cacheContentRect.x = buttonProperties._frameSize[i];
        cacheContentRect.y = buttonProperties._frameSize[i];
        cacheContentRect.w = buttonProperties._rect.w - (buttonProperties._frameSize[i] << 1);
        cacheContentRect.h = buttonProperties._rect.h - (buttonProperties._frameSize[i] << 1);
        SDL_FillRect(cacheDraw, NULL, buttonProperties._frameColor[i]);
        switch (_subType) {
            case JUICYGUI_BUTTON_SUBTYPE_IMAGE: {
                    if (buttonProperties._label[i] != NULL) {
                        cacheContent = _host->CreateSurfacePNG(buttonProperties._label[i]);
                        SDL_BlitScaled(cacheContent, NULL, cacheDraw, &cacheContentRect);
                    }
                }
                break;
            case JUICYGUI_BUTTON_SUBTYPE_BASIC: {
                    SDL_FillRect(cacheDraw, &cacheContentRect, buttonProperties._color[i]);
                    SDL_Point txtSize;
                    txtSize.x = 0;
                    txtSize.y = 0;
                    if (buttonProperties._label[i] != NULL) {
                        TTF_Font* font = TTF_OpenFont(buttonProperties._font[i], (buttonProperties._rect.h >> 1));
                        cacheContent = _host->CreateSurfaceTXT(buttonProperties._label[i], &txtSize, font, buttonProperties._fontColor[i]);
                        TTF_CloseFont(font);
                    }
                    cacheContentRect.h = txtSize.y;
                    cacheContentRect.w = txtSize.x;
                    cacheContentRect.y = ((buttonProperties._rect.h - txtSize.y) >> 1);
                    cacheContentRect.x = ((buttonProperties._rect.w - txtSize.x) >> 1);
                    SDL_BlitScaled(cacheContent, NULL, cacheDraw, &cacheContentRect);
                }
                break;
            default:
                break;
        }
        _texture[i] = SDL_CreateTextureFromSurface(_host->_Renderer, cacheDraw);
        SDL_FreeSurface(cacheDraw);
        if (cacheContent != NULL) SDL_FreeSurface(cacheContent);
    }
}

