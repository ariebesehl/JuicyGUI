#include "JuicyGUI_Button.h"
#include "JuicyGUI_Definitions.h"


int DEF_UI_Button_Frame_Size[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE, GAME_UI_BUTTON_FRAME_SIZE};
const char* DEF_UI_Button_Font_Type[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE, GAME_UI_FONT_TYPE};
int DEF_UI_Button_Font_Size[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_FONT_SIZE, GAME_UI_FONT_SIZE, GAME_UI_FONT_SIZE};
JuicyGUI_Color DEF_UI_Button_Color[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_BUTTON_COLOR, GAME_UI_BUTTON_COLOR_HOVER, GAME_UI_BUTTON_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Frame_Color[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_BUTTON_FRAME_COLOR, GAME_UI_BUTTON_FRAME_COLOR_HOVER, GAME_UI_BUTTON_FRAME_COLOR_PRESSED};
JuicyGUI_Color DEF_UI_Button_Font_Color[NUM_STATES_JUICYGUI_BUTTON] = {GAME_UI_BUTTON_FONT_COLOR, GAME_UI_BUTTON_FONT_COLOR_HOVER, GAME_UI_BUTTON_FONT_COLOR_PRESSED};


JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* hostInterface, JuicyGUI_ID id, uint32_t options, JuicyGUI_Button_Struct* style, const char* label) {
    _id = id;
    _type = JUICYGUI_TYPE_ID_BUTTON;
    _action = JUICYGUI_ACTION_NONE;
    _host = hostInterface;
    _flag = options;
    _hostFlag = JUICYGUI_HOSTFLAG_SHOW;
    for (int i = 0; i < NUM_STATES_JUICYGUI_BUTTON; i++) {
        _textureLabel[i] = NULL;
    }
    if (style != NULL) {
        buttonProperties = *style;
        createTextures();
    } else {
        for (int i = 0; i < NUM_STATES_JUICYGUI_BUTTON; i++) {
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
    _flag &= ~JUICYGUI_BUTTON_DISABLED;
}

void JuicyGUI_Button::disable() {
    _flag |= JUICYGUI_BUTTON_DISABLED;
}

void JuicyGUI_Button::setDraw(bool show) {
    if (show) {
        _hostFlag |= JUICYGUI_HOSTFLAG_SHOW;
    } else {
        _hostFlag &= ~JUICYGUI_HOSTFLAG_SHOW;
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
    //JuicyGUI_Action state = (_host->EvaluateState(_id, _type, &buttonProperties._rect) & 0x03);
    SDL_Rect cacheLabelRect = buttonProperties._rect;
    for (int i = 0; i < (buttonProperties._frameSize[_action] & 0x7fff); i++) {
        SDL_SetRenderDrawColor(_host->_Renderer, buttonProperties._frameColor[_action] >> 24, buttonProperties._frameColor[_action] >> 16, buttonProperties._frameColor[_action] >> 8, buttonProperties._frameColor[_action]);
        SDL_RenderDrawRect(_host->_Renderer, &cacheLabelRect);
        cacheLabelRect.x += 1;
        cacheLabelRect.y += 1;
        cacheLabelRect.w -= 2;
        cacheLabelRect.h -= 2;
    }
    if (_textureLabel[_action] != NULL)  {
        if (!(_flag & JUICYGUI_BUTTON_IMAGE)) {
            SDL_SetRenderDrawColor(_host->_Renderer, buttonProperties._color[_action] >> 24, buttonProperties._color[_action] >> 16, buttonProperties._color[_action] >> 8, buttonProperties._color[_action]);
            SDL_RenderFillRect(_host->_Renderer, &cacheLabelRect);
            cacheLabelRect.h = _sizeTextureLabel[_action].y;
            cacheLabelRect.w = _sizeTextureLabel[_action].x;
            cacheLabelRect.y = buttonProperties._rect.y + ((buttonProperties._rect.h - _sizeTextureLabel[_action].y) >> 1);
            cacheLabelRect.x = buttonProperties._rect.x + ((buttonProperties._rect.w - _sizeTextureLabel[_action].x) >> 1);
        }
        SDL_RenderCopy(_host->_Renderer, _textureLabel[_action], NULL, &cacheLabelRect);
    }
}

void JuicyGUI_Button::clearTextures() {
    for (int i = 0; i < NUM_STATES_JUICYGUI_BUTTON; i++) {
        if (_textureLabel[i] != NULL) {
            SDL_DestroyTexture(_textureLabel[i]);
            _textureLabel[i] = NULL;
        }
    }
}

void JuicyGUI_Button::createTextures() {
    clearTextures();
    for (int i = 0; i < NUM_STATES_JUICYGUI_BUTTON; i++) {
        if (_flag & JUICYGUI_BUTTON_IMAGE) {
            if (buttonProperties._label[i] != NULL) _textureLabel[i] = _host->CreateTexturePNG(buttonProperties._label[i]);
        } else {
            TTF_Font* font = TTF_OpenFont(buttonProperties._font[i], (buttonProperties._rect.h * buttonProperties._fontSize[i] * 10 / 1400));
            if (buttonProperties._label[i] != NULL) _textureLabel[i] = _host->CreateTextureTXT(buttonProperties._label[i], &_sizeTextureLabel[i], font, buttonProperties._fontColor[i]);
            TTF_CloseFont(font);
        }
    }
}

