#ifndef JUICYGUI_BUTTON_H_INCLUDED
#define JUICYGUI_BUTTON_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI.h"

#define JUICYGUI_BUTTON_SUBTYPE_BASIC 0x00000000
#define JUICYGUI_BUTTON_SUBTYPE_IMAGE 0x00000002

#define JUICYGUI_BUTTON_NUM_STATES 3
#define JUICYGUI_BUTTON_TEXTURE_ID_NONE 0
#define JUICYGUI_BUTTON_TEXTURE_ID_HOVER 1
#define JUICYGUI_BUTTON_TEXTURE_ID_PRESSED 2
#define JUICYGUI_BUTTON_TEXTURE_ID_RELEASED JUICYGUI_BUTTON_TEXTURE_ID_HOVER

typedef struct JuicyGUI_Button_Struct {
    SDL_Rect _rect;
    JuicyGUI_Color _color[JUICYGUI_BUTTON_NUM_STATES];
    int _frameSize[JUICYGUI_BUTTON_NUM_STATES];
    JuicyGUI_Color _frameColor[JUICYGUI_BUTTON_NUM_STATES];
    const char* _label[JUICYGUI_BUTTON_NUM_STATES];
    const char* _font[JUICYGUI_BUTTON_NUM_STATES];
    JuicyGUI_Color _fontColor[JUICYGUI_BUTTON_NUM_STATES];
    int _fontSize[JUICYGUI_BUTTON_NUM_STATES];
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
        uint32_t getTextureID();
        void draw();
        JuicyGUI_ID _id;
        JuicyGUI_Type _type;
        JuicyGUI_Action _action;
        void destroyTextures();
        void createTextures();
        JuicyGUI* _host;
        uint32_t _elementFlag;
        uint32_t _subType;
        SDL_Texture* _texture[JUICYGUI_BUTTON_NUM_STATES];
};


#endif // JUICYGUI_BUTTON_H_INCLUDED
