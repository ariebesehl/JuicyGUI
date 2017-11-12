#include "JuicyGUI.h"
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Button.h"



JuicyGUI::JuicyGUI(SDL_Window* Window, SDL_Renderer* Renderer, SDL_Event* Event) {
    // init GUI variables
    _Window = Window;
    _Renderer = Renderer;
    _Event = Event;
    _ButtonList = NULL;
    _ButtonListSize = 0;
    _TmilNow = SDL_GetTicks();
    _Font = NULL;
    for (int i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
        _Charset[i] = NULL;
    }
    // init PNG handling systems
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    LoadCharset();
    UpdateState(NULL, NULL);
}

JuicyGUI::~JuicyGUI() {
    for (uint32_t i = 0; i < _ButtonListSize; i++) {
        delete _ButtonList[i];
    }
    delete _ButtonList;
    DestroyCharset();
    TTF_Quit();
    IMG_Quit();
}

bool JuicyGUI::LoadCharset() {
    DestroyCharset();
    _CharsetCursor.x = 0;
    _CharsetCursor.y = 0;
    _Font = TTF_OpenFont(GAME_UI_FONT_TYPE, GAME_UI_FONT_SIZE);
    if (_Font != NULL) {
        char charEnum[] = {0, '\0'};
        char* ptrChar = &charEnum[0];
        for (int i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
            TTF_SizeText(_Font, ptrChar, &_CharsetWidth[i], (i != JUICYGUI_CHARSET_REFERENCE_HEIGHT) ? NULL : &_CharsetHeight);
            _Charset[i] = CreateTextureTXT(ptrChar, NULL, _Font, GAME_UI_FONT_COLOR);
            (*ptrChar)++;
        }
        _CharsetLineMargin = _CharsetHeight >> JUICYGUI_CHARSET_LINE_MARGIN_BIT_SHIFT;
        return true;
    } else {
        DestroyCharset();
        return false;
    }
}

void JuicyGUI::DestroyCharset() {
    _CharsetLineMargin = 0;
    _CharsetHeight = 0;
    for (int i = 0; i < JUICYGUI_CHARSET_SIZE; i++) {
        _CharsetWidth[i] = 0;
        if (_Charset[i] != NULL) {
            SDL_DestroyTexture(_Charset[i]);
            _Charset[i] = NULL;
        }
    }
    if (_Font != NULL) TTF_CloseFont(_Font);
}


void JuicyGUI::PrintTXT(const char* text, SDL_Point* textPosition) {
    if (text != NULL) {
        SDL_Rect drawRect;
        drawRect.h = _CharsetHeight;
        SetPrintCursor(textPosition);
        int i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                switch (text[i]) {
                    default:
                        if (_CharsetWidth[(uint8_t)*(text + i)]) {
                            drawRect.x = _CharsetCursor.x;
                            drawRect.y = _CharsetCursor.y;
                            drawRect.w = _CharsetWidth[(uint8_t)*(text + i)];
                            SDL_RenderCopy(_Renderer, _Charset[(uint8_t)*(text + i)], NULL, &drawRect);
                            _CharsetCursor.x += drawRect.w;
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

void JuicyGUI::PrintLnTXT(const char* text, SDL_Point* textPosition) {
    if (text != NULL) {
        SDL_Rect drawRect;
        drawRect.h = _CharsetHeight;
        SetPrintCursor(textPosition);
        int cacheCursorNL = _CharsetCursor.x;
        int i = 0;
        while (true) {
            if (text[i] != '\0' && i < JUICYGUI_CHARSET_MAX_LENGTH) {
                switch (text[i]) {
                    default:
                        if (_CharsetWidth[(uint8_t)*(text + i)]) {
                            drawRect.x = _CharsetCursor.x;
                            drawRect.y = _CharsetCursor.y;
                            drawRect.w = _CharsetWidth[(uint8_t)*(text + i)];
                            SDL_RenderCopy(_Renderer, _Charset[(uint8_t)*(text + i)], NULL, &drawRect);
                            _CharsetCursor.x += drawRect.w;
                        }
                        break;
                }
            } else {
                _CharsetCursor.x = cacheCursorNL;
                _CharsetCursor.y += _CharsetHeight + _CharsetLineMargin;
                return;
            }
            i++;
        }
    }
}


void JuicyGUI::SetPrintCursor(SDL_Point* cursorPos) {
    if (cursorPos != NULL) {
        _CharsetCursor.x = cursorPos->x;
        _CharsetCursor.y = cursorPos->y;
    }
}

SDL_Point* JuicyGUI::GetPrintCursor() {
    return &_CharsetCursor;
}

int JuicyGUI::GetCharsetHeight() {
    return _CharsetHeight;
}



bool JuicyGUI::RegisterElement(void* ptrElement, JuicyGUI_Type elementType) {
    switch (elementType) {
        case JUICYGUI_TYPE_ID_BUTTON: {
                JuicyGUI_Button* ptrButton = NULL;
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
            }
            return true;
        default:
            return false;
    }
}


JuicyGUI_Action JuicyGUI::UpdateState(JuicyGUI_ID* elementID, JuicyGUI_Action* elementAction) {
    _TmilLast = _TmilNow;
    _TmilNow = SDL_GetTicks();
    _TmilDelta = _TmilNow - _TmilLast;
    JuicyGUI_Action hostAction = JUICYGUI_ACTION_NONE;
    static SDL_Point lastScreenSize;
    SDL_GetWindowSize(_Window, &_ScreenSize.x, &_ScreenSize.y);
    if (lastScreenSize.x != _ScreenSize.x || lastScreenSize.y != _ScreenSize.y) {
        hostAction |= JUICYGUI_ACTION_RESIZE;
    }
    lastScreenSize = _ScreenSize;
    _MouseState = SDL_GetMouseState(&_MousePos.x, &_MousePos.y);
    uint32_t cacheID = 0;
    JuicyGUI_Action cacheAction = JUICYGUI_ACTION_NONE;
    for (uint32_t i = 0; i < _ButtonListSize; i++) {
        if (_ButtonList[i]->_elementFlag & JUICYGUI_ELEMENTFLAG_SHOW) {
            EvaluateState(_ButtonList[i], _ButtonList[i]->_type);
            if (_ButtonList[i]->_action != JUICYGUI_ACTION_NONE) {
                cacheID = _ButtonList[i]->_id;
                cacheAction = _ButtonList[i]->_action;
            }
        }
    }
    if (elementID != NULL) *elementID = cacheID;
    if (elementAction != NULL) *elementAction = cacheAction;
    return hostAction;
}

void JuicyGUI::DrawBackground(JuicyGUI_Color color) {
    ElevateRenderer(true);
    SDL_Rect bgRect;
    bgRect.x = 0;
    bgRect.y = 0;
    bgRect.w = _ScreenSize.x;
    bgRect.h = _ScreenSize.y;
    SDL_SetRenderDrawColor(_Renderer, color >> 24, color >> 16, color >> 8, color);
    SDL_RenderFillRect(_Renderer, &bgRect);
    ElevateRenderer(false);
}

void JuicyGUI::DrawElements() {
    for (uint32_t i = 0; i < _ButtonListSize; i++) {
        if (_ButtonList[i]->_elementFlag & JUICYGUI_ELEMENTFLAG_SHOW) _ButtonList[i]->draw();
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

void JuicyGUI::EvaluateState(void* ptrElement, JuicyGUI_Type elementType) {
    JuicyGUI_Action state = JUICYGUI_ACTION_NONE;
    JuicyGUI_ID elementID = 0;
    static JuicyGUI_ID lockedElementID = 0;
    switch (elementType) {
        case JUICYGUI_TYPE_ID_BUTTON: {
                JuicyGUI_Button* ptrButton = NULL;
                ptrButton = static_cast<JuicyGUI_Button*>(ptrElement);
                elementID = ptrButton->_id;
                if (EvaluateMouseOver(&(ptrButton->buttonProperties._rect))) { // Mouse over?
                    if (!lockedElementID)  {
                        if (_MouseState & JUICYGUI_CONTROL_ID_LMB) {
                            lockedElementID = elementID;
                            state = JUICYGUI_ACTION_PRESSED;
                        } else {
                            state = JUICYGUI_ACTION_HOVER;
                        }
                    } else {
                        if (lockedElementID == elementID) {
                            if (_MouseState & JUICYGUI_CONTROL_ID_LMB) {
                                state = JUICYGUI_ACTION_PRESSED;
                            } else {
                                lockedElementID = 0;
                                state = JUICYGUI_ACTION_RELEASED;
                            }
                        }
                    }
                } else { // no mouse over
                    if (lockedElementID == elementID){
                        if (!(_MouseState & JUICYGUI_CONTROL_ID_LMB)) {
                            lockedElementID = 0;
                            state = JUICYGUI_ACTION_NONE;
                        } else {
                            state = JUICYGUI_ACTION_PRESSED;
                        }
                    } else {
                        state= JUICYGUI_ACTION_NONE;
                    }
                }
                ptrButton->_action = state;
            }
            break;
        default:
            break;
    }
}


SDL_Texture* JuicyGUI::CreateTexturePNG(const char* path) {
    ElevateRenderer(true);
    SDL_Texture* texture = NULL;
    SDL_Surface* cache = IMG_Load(path);
    if (cache == NULL) return texture;
    texture = SDL_CreateTextureFromSurface(_Renderer, cache);
    SDL_FreeSurface(cache);
    ElevateRenderer(false);
    return texture;
}

SDL_Surface* JuicyGUI::CreateSurfacePNG(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    return surface;
}

SDL_Texture* JuicyGUI::CreateTextureTXT(const char* text, SDL_Point* dimensions, TTF_Font* font, JuicyGUI_Color color) {
    ElevateRenderer(true);
    SDL_Color cacheColor;
    cacheColor.r = color >> 24;
    cacheColor.g = color >> 16;
    cacheColor.b = color >> 8;
    cacheColor.a = color;
    SDL_Surface* cache = TTF_RenderText_Blended(font, text, cacheColor);
    if (cache != NULL) {
        if (dimensions != NULL) TTF_SizeText(font, text, &(dimensions->x), &(dimensions->y));
        SDL_Texture* texture = NULL;
        texture = SDL_CreateTextureFromSurface(_Renderer, cache);
        SDL_FreeSurface(cache);
        ElevateRenderer(false);
        return texture;
    } else {
        ElevateRenderer(false);
        return NULL;
    }
}

SDL_Surface* JuicyGUI::CreateSurfaceTXT(const char* text, SDL_Point* dimensions, TTF_Font* font, JuicyGUI_Color color) {
    SDL_Color cacheColor;
    cacheColor.r = color >> 24;
    cacheColor.g = color >> 16;
    cacheColor.b = color >> 8;
    cacheColor.a = color;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, cacheColor);
    if (dimensions != NULL) TTF_SizeText(font, text, &(dimensions->x), &(dimensions->y));
    return surface;
}

void JuicyGUI::ElevateRenderer(bool exclusive) {
    static SDL_Texture* cacheTarget = NULL;
    static uint8_t cacheColorA = 0xff;
    static uint8_t cacheColorR = 0xff;
    static uint8_t cacheColorG = 0xff;
    static uint8_t cacheColorB = 0xff;
    static SDL_BlendMode cacheBlendMode = SDL_BLENDMODE_NONE; // is this the correct init value?
    if (exclusive) {
        SDL_GetRenderDrawBlendMode(_Renderer, &cacheBlendMode);
        SDL_GetRenderDrawColor(_Renderer, &cacheColorR, &cacheColorG, &cacheColorB, &cacheColorA);
        cacheTarget = SDL_GetRenderTarget(_Renderer);
        SDL_SetRenderTarget(_Renderer, NULL);
    } else {
        SDL_SetRenderTarget(_Renderer, cacheTarget);
        SDL_SetRenderDrawColor(_Renderer, cacheColorR, cacheColorG, cacheColorB, cacheColorA);
        SDL_SetRenderDrawBlendMode(_Renderer, cacheBlendMode);
    }
}



