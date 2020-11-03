#ifndef JUICYGUI_H_INCLUDED
#define JUICYGUI_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"

typedef struct {
    JD_INDEX id;
    JD_FLAG type;
    JD_FLAG data0;
    JD_FLAG data1;
    JD_FLAG data2;
    JD_FLAG data3;
} JuicyGUI_Event;

class JuicyGUI {
    friend class JuicyGUI_Element;
    public:
        JuicyGUI(SDL_Window* iWindow, SDL_Renderer* iRenderer);//, SDL_Event* iEvent);
        ~JuicyGUI(void);

        bool PollEvents(JuicyGUI_Event* oEvent);

        JEN* GetEngine(void) {return engine;};
	    JD_TIME GetTicks(void) {return engine->GetTicks();};
	    JD_TIME GetFramelessTicks(void) {return engine->GetFramelessTicks();};
        JD_TIME GetDelta(void){return engine->GetDelta();};
        JD_INDEX GetFPS(void) {return engine->GetFPS();};
        JD_FLAG GetMousePos(JD_Point* oPosition);
        JD_FLAG GetMouseMovement(JD_Point* oMovement);

        void GetScreenSize(JD_Point* oSize);
        JD_I GetScreenWidth(void) {return element.getWidth();};
        JD_I GetScreenHeight(void) {return element.getHeight();};
        JD_I GetScreenSizeBigger(void);
        JD_I GetScreenSizeSmaller(void);

        JD_COLOR GetJuicy(void);
        JD_COLOR GetJuicyColor(void);

        void DrawBackground(JD_COLOR iColor) {engine->FillScreen(iColor);};
        void DrawElements(void);

    private:
        JuicyGUI_Element element;
        JEN* engine;

        bool registerElement(JuicyGUI_Element* iElement);
        bool evaluateElement(JuicyGUI_Element* iElement);

        JD_INDEX numElement;
        JD_INDEX msFocusElement;
        JuicyGUI_Element** elements;

        JD_INDEX numEvents;
        JD_INDEX ctrEvents;
        JuicyGUI_Event** events;
        bool eventsFlushed;
        void updateEvents();

        JD_FLAG updateMouse();
        bool mouseOver(const JD_Rect* iRect);
        JD_FLAG mouseState;
        JD_Point mousePos;
        JD_Point mouseMove;
};

#endif // JUICYGUI_H_INCLUDED
