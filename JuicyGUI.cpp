#include <iostream>

#include "Juicy_SDL.h"
#include "JuicyGUI_Definitions.h"

#include "JuicyGUI.h"
#include "JuicyGUI_Box.h"
#include "JuicyGUI_Button.h"
#include "JuicyGUI_Charset.h"
#include "JuicyGUI_Element.h"


JuicyGUI::JuicyGUI(SDL_Window* Window, SDL_Renderer* Renderer, SDL_Event* Event) {
    _Window = Window;
    _Renderer = Renderer;
    _Event = Event;
    _ElementList = NULL;
    _ElementListSize = 0;
    Element.setCredentials(this, this, JUICYGUI_LOWLVL_ROOT_ID_NUM, JUICYGUI_TYPE_ID_GUI);
    SDL_Rect initScreen;
    initScreen.x = JUICYGUI_LOWLVL_DEFAULT_SCREEN_ORIGIN_X;
    initScreen.y = JUICYGUI_LOWLVL_DEFAULT_SCREEN_ORIGIN_Y;
    SDL_GetWindowSize(_Window, &initScreen.w, &initScreen.h);
    Element.setRect(&initScreen);
    _TmilNow = SDL_GetTicks();
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
}

JuicyGUI::~JuicyGUI() {
    for (uint32_t i = 0; i < _ElementListSize; i++) {
        delete _ElementList[i];
    }
    delete[] _ElementList;
    TTF_Quit();
    IMG_Quit();
}

bool JuicyGUI::RegisterElement(JuicyGUI_Element* iElement) {
    if (iElement != NULL) {
        if (iElement->getHost() == this && iElement->getOwner() != NULL) {
            if (!(iElement->isRoot())) {
                JuicyGUI_Element** ptrCache = new JuicyGUI_Element*[_ElementListSize + 1];
                for (uint32_t i = 0; i < _ElementListSize; i++) {
                    ptrCache[i] = *(_ElementList + i);
                }
                ptrCache[_ElementListSize] = iElement;
                delete[] _ElementList;
                _ElementList = ptrCache;
                _ElementListSize++;
                return true;
            }
        }
    }
    return false;
}

JuicyGUI_Action JuicyGUI::UpdateState(JuicyGUI_ID* elementID, JuicyGUI_Action* elementAction) {
    _TmilLast = _TmilNow;
    _TmilNow = SDL_GetTicks();
    _TmilDelta = _TmilNow - _TmilLast;
    Element.setAction(JUICYGUI_ACTION_NONE);
    SDL_Point lastScreenSize;
    SDL_Point currentScreenSize;
    Element.getSize(&lastScreenSize);
    SDL_GetWindowSize(_Window, &currentScreenSize.x, &currentScreenSize.y);
    if (currentScreenSize != lastScreenSize) {
        Element.setSize(&currentScreenSize);
        Element.attachAction(JUICYGUI_ACTION_RESIZE);
    }
    _MouseState = SDL_GetMouseState(&_MousePos.x, &_MousePos.y);
    uint32_t cacheID = 0;
    JuicyGUI_Action cacheAction = JUICYGUI_ACTION_NONE;
    for (uint32_t i = 0; i < _ElementListSize; i++) {
        EvaluateState(_ElementList[i]);
        if (_ElementList[i]->getAction() != JUICYGUI_ACTION_NONE) {
            cacheID = _ElementList[i]->getID();
            cacheAction = _ElementList[i]->getAction();
        }
    }
    if (elementID != NULL) *elementID = cacheID;
    if (elementAction != NULL) *elementAction = cacheAction;
    return Element.getAction();
}

void JuicyGUI::GetScreenSize(SDL_Point* oSize) {
    if (oSize != NULL) Element.getSize(oSize);
}

int JuicyGUI::GetScreenSizeBigger(void) {
    return (Element.getWidth() > Element.getHeight()) ? Element.getWidth() : Element.getHeight();
}

int JuicyGUI::GetScreenSizeSmaller(void) {
    return (Element.getWidth() > Element.getHeight()) ? Element.getHeight() : Element.getWidth();
}

void JuicyGUI::DrawBackground(JuicyGUI_Color color) {
    ElevateRenderer(true);
    SDL_SetRenderDrawBlendMode(_Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(_Renderer, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
    SDL_RenderFillRect(_Renderer, Element.getRect());
    ElevateRenderer(false);
}

void JuicyGUI::DrawElements() {
    ElevateRenderer(true);
    for (uint32_t i = 0; i < _ElementListSize; i++) {
        switch (_ElementList[i]->getType()) {
            case JUICYGUI_TYPE_ID_NONE:
                break;
            case JUICYGUI_TYPE_ID_GUI:
                break;
            case JUICYGUI_TYPE_ID_BOX:
                static_cast<JuicyGUI_Box*>(_ElementList[i]->getOwner())->draw();
                break;
            case JUICYGUI_TYPE_ID_BUTTON:
                static_cast<JuicyGUI_Button*>(_ElementList[i]->getOwner())->draw();
                break;
            case JUICYGUI_TYPE_ID_CHARSET:
                static_cast<JuicyGUI_Charset*>(_ElementList[i]->getOwner())->draw();
                break;
            default:
                break;
        }
    }
    ElevateRenderer(false);
}

bool JuicyGUI::EvaluateMouseOver(const SDL_Rect* iRect) {
    if (_MousePos.y >= iRect->y) {
        if (_MousePos.y <= iRect->y + iRect->h) {
            if (_MousePos.x >= iRect->x) {
                if (_MousePos.x <= iRect->x + iRect->w) {
                    return true;
                }
            }
        }
    }
    return false;
}

void JuicyGUI::EvaluateState(JuicyGUI_Element* iElement) {
    JuicyGUI_Action state = JUICYGUI_ACTION_NONE;
    JuicyGUI_ID elementID = 0;
    static JuicyGUI_ID lockedElementID = 0;
    switch (iElement->getType()) {
        case JUICYGUI_TYPE_ID_BUTTON: {
                elementID = iElement->getID();
                if (!(iElement->getFlag() & JUICYGUI_ELEMENTFLAG_DISABLED) && (iElement->getFlag() & JUICYGUI_ELEMENTFLAG_SHOW)) {
                    if (EvaluateMouseOver(iElement->getRect())) { // Mouse over?
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
                            state = JUICYGUI_ACTION_NONE;
                        }
                    }
                }
                iElement->setAction(state);
            }
            break;
        default:
            break;
    }
}

void JuicyGUI::RenderTexture(SDL_Texture* iTexture, const SDL_Rect* iRect) {
    if (iTexture != NULL) {
        ElevateRenderer(true);
        SDL_RenderCopy(_Renderer, iTexture, NULL, (iRect != NULL) ? iRect : Element.getRect());
        ElevateRenderer(false);
    }
}

void JuicyGUI::RenderTextureVirtual(SDL_Texture* iTexture, SDL_Texture* iScreen, const SDL_Rect* iRect) {
    if (iTexture != NULL && iScreen != NULL) {
        ElevateRenderer(true);
        SDL_SetRenderTarget(_Renderer, iScreen);
        SDL_RenderCopy(_Renderer, iTexture, NULL, iRect);
        ElevateRenderer(false);
    }
}

void JuicyGUI::ClearVirtualScreen(SDL_Texture* iScreen) {
    if (iScreen != NULL) {
        ElevateRenderer(true);
        SDL_SetRenderTarget(_Renderer, iScreen);
        SDL_SetRenderDrawBlendMode(_Renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(_Renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(_Renderer, NULL);
        ElevateRenderer(false);
    }
}

SDL_Texture* JuicyGUI::CreateTextureFill(const SDL_Point* iDimensions, JuicyGUI_Color iColor) {
    ElevateRenderer(true);
    SDL_Texture* texture = NULL;
    SDL_Surface* cacheDraw = SDL_CreateRGBSurface(0, iDimensions->x, iDimensions->y, JUICYGUI_LOWLVL_DEPTH, JUICYGUI_LOWLVL_MASKR, JUICYGUI_LOWLVL_MASKG, JUICYGUI_LOWLVL_MASKB, JUICYGUI_LOWLVL_MASKA);
    SDL_FillRect(cacheDraw, NULL, iColor);
    texture = SDL_CreateTextureFromSurface(_Renderer, cacheDraw);
    SDL_FreeSurface(cacheDraw);
    ElevateRenderer(false);
    return texture;
}


SDL_Texture* JuicyGUI::CreateTexturePNG(const char* iFilepath) {
    ElevateRenderer(true);
    SDL_Texture* texture = NULL;
    SDL_Surface* cache = IMG_Load(iFilepath);
    if (cache == NULL) return texture;
    texture = SDL_CreateTextureFromSurface(_Renderer, cache);
    SDL_FreeSurface(cache);
    ElevateRenderer(false);
    return texture;
}

SDL_Texture* JuicyGUI::CreateTextureTXT(const char* iText, SDL_Point* oDimensions, TTF_Font* iFont, JuicyGUI_Color iColor) {
    ElevateRenderer(true);
    SDL_Color cacheColor;
    cacheColor.r = iColor >> 24;
    cacheColor.g = iColor >> 16;
    cacheColor.b = iColor >> 8;
    cacheColor.a = iColor;
    SDL_Surface* cache = TTF_RenderText_Blended(iFont, iText, cacheColor);
    if (cache != NULL) {
        if (oDimensions != NULL) TTF_SizeText(iFont, iText, &(oDimensions->x), &(oDimensions->y));
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

SDL_Texture* JuicyGUI::CreateVirtualScreen(void) {
    ElevateRenderer(true);
    SDL_Point screenSize;
    Element.getSize(&screenSize);
    SDL_Texture* _virtualScreen = SDL_CreateTexture(_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenSize.x, screenSize.y);
    SDL_SetTextureBlendMode(_virtualScreen, SDL_BLENDMODE_BLEND);
    ElevateRenderer(false);
    return _virtualScreen;
}


SDL_Surface* JuicyGUI::CreateSurfaceFill(const SDL_Point* iDimensions, JuicyGUI_Color iColor) {
    ElevateRenderer(true);
    SDL_Surface* newSurface = SDL_CreateRGBSurface(0, iDimensions->x, iDimensions->y, JUICYGUI_LOWLVL_DEPTH, JUICYGUI_LOWLVL_MASKR, JUICYGUI_LOWLVL_MASKG, JUICYGUI_LOWLVL_MASKB, JUICYGUI_LOWLVL_MASKA);
    SDL_FillRect(newSurface, NULL, iColor);
    ElevateRenderer(false);
    return newSurface;
}


SDL_Surface* JuicyGUI::CreateSurfacePNG(const char* iFilepath) {
    SDL_Surface* surface = IMG_Load(iFilepath);
    return surface;
}

SDL_Surface* JuicyGUI::CreateSurfaceTXT(const char* iText, SDL_Point* oDimensions, TTF_Font* iFont, JuicyGUI_Color iColor) {
    SDL_Color cacheColor;
    cacheColor.r = iColor >> 24;
    cacheColor.g = iColor >> 16;
    cacheColor.b = iColor >> 8;
    cacheColor.a = iColor;
    SDL_Surface* surface = TTF_RenderText_Blended(iFont, iText, cacheColor);
    if (oDimensions != NULL) TTF_SizeText(iFont, iText, &(oDimensions->x), &(oDimensions->y));
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

