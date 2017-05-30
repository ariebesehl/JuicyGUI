#ifndef JUICYGUI_BUTTON_H_INCLUDED
#define JUICYGUI_BUTTON_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI.h"

#define NUM_STATES_JUICYGUI_BUTTON 3

typedef struct JuicyGUI_Button_Struct {
    SDL_Rect _rect;
    JuicyGUI_Color _color[NUM_STATES_JUICYGUI_BUTTON];
    int _frameSize[NUM_STATES_JUICYGUI_BUTTON];
    JuicyGUI_Color _frameColor[NUM_STATES_JUICYGUI_BUTTON];
    const char* _label[NUM_STATES_JUICYGUI_BUTTON];
    const char* _font[NUM_STATES_JUICYGUI_BUTTON];
    JuicyGUI_Color _fontColor[NUM_STATES_JUICYGUI_BUTTON];
    int _fontSize[NUM_STATES_JUICYGUI_BUTTON];
} JuicyGUI_Button_Struct;

class JuicyGUI_Button {
    friend class JuicyGUI;
    public:
        JuicyGUI_Button(JuicyGUI*, JuicyGUI_ID, uint32_t, JuicyGUI_Button_Struct*, const char*);
        ~JuicyGUI_Button();
        void enable();
        void disable();
        void setDraw(bool);
        void setRect(SDL_Rect*);
    private:
        JuicyGUI_Button_Struct buttonProperties;
        void draw();
        JuicyGUI_ID _id;
        JuicyGUI_Type _type;
        JuicyGUI_Action _action;
        void clearTextures();
        void createTextures();
        JuicyGUI* _host;
        uint32_t _hostFlag;
        uint32_t _flag;
        SDL_Texture* _textureLabel[NUM_STATES_JUICYGUI_BUTTON];
        SDL_Point _sizeTextureLabel[NUM_STATES_JUICYGUI_BUTTON];
};


#endif // JUICYGUI_BUTTON_H_INCLUDED
