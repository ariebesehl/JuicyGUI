#include "JuicyGUI.h"
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Button.h"
#include <iostream>

JuicyGUI::JuicyGUI(SDL_Window* Window, SDL_Renderer* Renderer, SDL_Event* Event) {
    // init GUI variables
    _Window = Window;
    _Renderer = Renderer;
    _Event = Event;
    _CommandCache = 0;
    _CommandQueue = 0;
    _ButtonList = NULL;
    _ButtonListSize = 0;
    // init PNG handling systems
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    UpdateState(NULL, NULL);
}

JuicyGUI::~JuicyGUI() {
    delete _ButtonList;
    IMG_Quit();
    TTF_Quit();
}

bool JuicyGUI::RegisterElement(void* ptrElement, JuicyGUI_Type elementType) {
    switch (elementType) {
        case JUICYGUI_TYPE_ID_BUTTON:
            JuicyGUI_Button* ptrButton;
            ptrButton = NULL;
            ptrButton = static_cast<JuicyGUI_Button*>(ptrElement);
            if (ptrButton != NULL) {
                JuicyGUI_Button** ptrCache = new JuicyGUI_Button*[_ButtonListSize + 1];
                for (uint32_t i = 0; i < _ButtonListSize; i++) {
                    ptrCache[i] = *(_ButtonList + i);
                }
                ptrCache[_ButtonListSize] = ptrButton;
                delete _ButtonList;
                _ButtonList = ptrCache;
                _ButtonListSize++;
            }
            return true;
        default:
            return false;
    }
}


JuicyGUI_Action JuicyGUI::UpdateState(JuicyGUI_ID* elementID, JuicyGUI_Action* elementAction) {
    JuicyGUI_Action hostAction = JUICYGUI_ACTION_NONE;
    static SDL_Point lastScreenSize;
    SDL_GetWindowSize(_Window, &_ScreenSize.x, &_ScreenSize.y);
    if (lastScreenSize.x != _ScreenSize.x || lastScreenSize.y != _ScreenSize.y) {
        hostAction |= JUICYGUI_ACTION_RESIZE;
    }
    lastScreenSize = _ScreenSize;
    _MouseState = 0xff & SDL_GetMouseState(&_MousePos.x, &_MousePos.y);
    uint32_t cacheID = 0;
    JuicyGUI_Action cacheAction = JUICYGUI_ACTION_NONE;
    for (uint32_t i = 0; i < _ButtonListSize; i++) {
        if (_ButtonList[i]->_hostFlag & JUICYGUI_HOSTFLAG_SHOW) {
            _ButtonList[i]->_action = EvaluateState(_ButtonList[i]->_id, _ButtonList[i]->_type, &(_ButtonList[i]->buttonProperties._rect));
            if (_ButtonList[i]->_action != JUICYGUI_ACTION_NONE) {
                cacheID = _ButtonList[i]->_id;
                cacheAction = _ButtonList[i]->_action;
            }
        }
    }
    if (elementID != NULL) *elementID = cacheID;
    if (elementAction != NULL) *elementAction = cacheAction;
    _CommandQueue = 0;
    return hostAction;
}

SDL_Texture* JuicyGUI::CreateTexturePNG(const char* path) {
    SDL_Texture* texture = NULL;
    SDL_Surface* cache = IMG_Load(path);
    if (cache == NULL) return texture;
    texture = SDL_CreateTextureFromSurface(_Renderer, cache);
    SDL_FreeSurface(cache);
    return texture;
}

SDL_Texture* JuicyGUI::CreateTextureTXT(const char* text, SDL_Point* dimensions, TTF_Font* font, JuicyGUI_Color color) {
    SDL_Texture* texture = NULL;
    SDL_Color cacheColor;
    cacheColor.r = color >> 24;
    cacheColor.g = color >> 16;
    cacheColor.b = color >> 8;
    cacheColor.a = color;
    TTF_SizeText(font, text, &(dimensions->x), &(dimensions->y));
    SDL_Surface* cache = TTF_RenderText_Blended(font, text, cacheColor);
    if (cache == NULL) return texture;
    texture = SDL_CreateTextureFromSurface(_Renderer, cache);
    SDL_FreeSurface(cache);
    return texture;
}

void JuicyGUI::DrawBackground(JuicyGUI_Color color) {
    SDL_Rect bgRect;
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = _ScreenSize.x;
    bgRect.h = _ScreenSize.y;
    SDL_SetRenderDrawColor(_Renderer, color >> 24, color >> 16, color >> 8, color);
    SDL_RenderFillRect(_Renderer, &bgRect);
}

void JuicyGUI::DrawElements() {
    for (uint32_t i = 0; i < _ButtonListSize; i++) {
        if (_ButtonList[i]->_hostFlag & JUICYGUI_HOSTFLAG_SHOW) _ButtonList[i]->draw();
    }
}

bool JuicyGUI::EvaluateMouseOver(SDL_Rect* rect) {
    if (_MousePos.y >= rect->y) {
        if (_MousePos.y <= rect->y + rect->h) {
            if (_MousePos.x >= rect->x) {
                if (_MousePos.x <= rect->x + rect->w) {
                    return true;
                }
            }
        }
    }
    return false;
}

JuicyGUI_Action JuicyGUI::EvaluateState(JuicyGUI_ID id_command, JuicyGUI_Type, SDL_Rect* rect) {
    JuicyGUI_Action state = JUICYGUI_ACTION_NONE;
    if (EvaluateMouseOver(rect)) { // Mouse over?
        if (!_CommandCache)  {
            if (_MouseState & 0x01) {
                _CommandCache = id_command;
                state |= JUICYGUI_ACTION_PRESSED;
            } else {
                state |= JUICYGUI_ACTION_HOVER;
            }
        } else {
            if (_CommandCache == id_command) {
                if (_MouseState & 0x01) {
                    state |= JUICYGUI_ACTION_PRESSED;
                } else {
                    _CommandQueue = _CommandCache;
                    _CommandCache = 0;
                    state |= JUICYGUI_ACTION_HOVER;
                }
            }
        }
    } else { // no mouse over
        if (_CommandCache == id_command){
            if (!(_MouseState & 0x01)) {
                _CommandCache = 0;
            }
        }
    }
    return state;
}

