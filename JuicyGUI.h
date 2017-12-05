#ifndef JUICYGUI_H_INCLUDED
#define JUICYGUI_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"

typedef uint32_t JuicyGUI_Color;
typedef uint32_t JuicyGUI_ID;
typedef uint32_t JuicyGUI_Type;
typedef uint32_t JuicyGUI_Action;
typedef uint32_t JuicyGUI_Time;

class JuicyGUI_Box;
class JuicyGUI_Button;
class JuicyGUI_Charset;


class JuicyGUI {
    friend class JuicyGUI_Box;
    friend class JuicyGUI_Button;
    friend class JuicyGUI_Charset;
    friend class JuicyGUI_Element;
    friend class JTE;
    public:
        JuicyGUI(SDL_Window* iWindow, SDL_Renderer* iRenderer, SDL_Event* iEvent);
        ~JuicyGUI(void);
        JuicyGUI_Action UpdateState(JuicyGUI_ID* oElementID, JuicyGUI_Action* oActionID);
        bool RegisterElementOLD(void* iPtrElement, JuicyGUI_Type iTypeElement);
        void DrawBackground(JuicyGUI_Color iColor);
        void DrawElements(void);
        void GetScreenSize(SDL_Point* oSize);
        int GetScreenWidth(void) {return Element.getWidth();};
        int GetScreenHeight(void) {return Element.getHeight();};
        int GetScreenSizeBigger(void);
        int GetScreenSizeSmaller(void);
        JuicyGUI_Time GetTimeDelta(void);
        SDL_Renderer* GetRenderer(void) {return _Renderer;};
    private:
        JuicyGUI_Element Element;
        bool RegisterElement(JuicyGUI_Element* iElement);
        bool EvaluateMouseOver(const SDL_Rect* iRect);
        void EvaluateState(JuicyGUI_Element* iElement);
        void RenderTexture(SDL_Texture* iTexture, const SDL_Rect* iRect);
        void RenderTextureVirtual(SDL_Texture* iTexture, SDL_Texture* iScreen, const SDL_Rect* iRect);
        void ClearVirtualScreen(SDL_Texture* iScreen);
        SDL_Texture* CreateTextureFill(const SDL_Point* iDimensions, JuicyGUI_Color iColor);
        SDL_Texture* CreateTextureSurface(SDL_Surface* iSurface) {if (iSurface != NULL) return SDL_CreateTextureFromSurface(_Renderer, iSurface); else return NULL;};
        SDL_Texture* CreateTexturePNG(const char* iFilepath);
        SDL_Texture* CreateTextureTXT(const char* iText, SDL_Point* oDimensions, TTF_Font* iFont, JuicyGUI_Color iColor);
        SDL_Texture* CreateVirtualScreen(void);
        SDL_Surface* CreateSurfaceFill(const SDL_Point* iDimensions, JuicyGUI_Color iColor);
        SDL_Surface* CreateSurfacePNG(const char* iFilepath);
        SDL_Surface* CreateSurfaceTXT(const char* iText, SDL_Point* oDimensions, TTF_Font* iFont, JuicyGUI_Color iColor);
        void ElevateRenderer(bool iState);
        SDL_Window* _Window;
        SDL_Renderer* _Renderer;
        SDL_Event* _Event;
        JuicyGUI_Time _TmilNow;
        JuicyGUI_Time _TmilLast;
        JuicyGUI_Time _TmilDelta;
        SDL_Point _MousePos;
        uint32_t _MouseState;
        JuicyGUI_Element** _ElementList;
        uint32_t _ElementListSize;
};

#endif // JUICYGUI_H_INCLUDED
