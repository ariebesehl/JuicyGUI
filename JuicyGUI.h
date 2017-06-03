#ifndef JUICYGUI_H_INCLUDED
#define JUICYGUI_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//#define GAME_UI_BACKGROUND_COLOR 0xfffac1ff

#define JUICYGUI_ACTION_NONE 0x00000000
#define JUICYGUI_ACTION_HOVER 0x00000001
#define JUICYGUI_ACTION_PRESSED 0x00000002
#define JUICYGUI_ACTION_RESIZE 0x00000004

#define JUICYGUI_HOSTFLAG_SHOW 0x00000001

#define JUICYGUI_TYPE_ID_BUTTON 0x01

#define JUICYGUI_BUTTON_DISABLED 0x01
#define JUICYGUI_BUTTON_IMAGE 0x02

#define JUICYGUI_CHARSET_SIZE 256
#define JUICYGUI_CHARSET_MAX_LENGTH 4096
#define JUICYGUI_CHARSET_REFERENCE_HEIGHT 65 // 'A'
#define JUICYGUI_CHARSET_LINE_MARGIN_BIT_SHIFT 8

typedef uint32_t JuicyGUI_Color;
typedef uint32_t JuicyGUI_ID;
typedef uint32_t JuicyGUI_Type;
typedef uint32_t JuicyGUI_Action;
typedef uint32_t JuicyGUI_Time;

class JuicyGUI_Button;

class JuicyGUI {
    friend class JuicyGUI_Button;
    friend class JTE;
    public:
        JuicyGUI(SDL_Window*, SDL_Renderer*, SDL_Event*);
        ~JuicyGUI();
        JuicyGUI_Action UpdateState(JuicyGUI_ID*, JuicyGUI_Action*);
        bool RegisterElement(void*, JuicyGUI_Type);
        void DrawBackground(JuicyGUI_Color);
        void DrawElements();
        SDL_Point* GetScreenSize() {return &_ScreenSize;};
        int GetScreenSizeBigger() {return (_ScreenSize.x > _ScreenSize.y) ? _ScreenSize.x : _ScreenSize.y;};
        int GetScreenSizeSmaller() {return (_ScreenSize.x > _ScreenSize.y) ? _ScreenSize.y : _ScreenSize.x;};
        void PrintTXT(const char*, SDL_Point*);
        void PrintLnTXT(const char*, SDL_Point*);
        void SetPrintCursor(SDL_Point*);
        JuicyGUI_Time GetTimeDelta();
    private:
        bool EvaluateMouseOver(SDL_Rect*);
        JuicyGUI_Action EvaluateState(JuicyGUI_ID, JuicyGUI_Type, SDL_Rect*);
        SDL_Texture* CreateTexturePNG(const char*);
        SDL_Texture* CreateTextureTXT(const char*, SDL_Point*, TTF_Font*, JuicyGUI_Color color);
        SDL_Window* _Window;
        SDL_Renderer* _Renderer;
        SDL_Event* _Event;
        JuicyGUI_ID _CommandCache;
        JuicyGUI_ID _CommandQueue;
        JuicyGUI_Time _TmilNow;
        JuicyGUI_Time _TmilLast;
        JuicyGUI_Time _TmilDelta;
        SDL_Point _ScreenSize;
        SDL_Point _MousePos;
        uint8_t _MouseState;
        TTF_Font* _Font;
        SDL_Texture* _Charset[JUICYGUI_CHARSET_SIZE];
        int _CharsetWidth[JUICYGUI_CHARSET_SIZE];
        int _CharsetHeight;
        int _CharsetLineMargin;
        SDL_Point _CharsetCursor;
        JuicyGUI_Button** _ButtonList;
        uint32_t _ButtonListSize;
};

#endif // JUICYGUI_H_INCLUDED
