#ifndef JUICYGUI_BUTTON_H_INCLUDED
#define JUICYGUI_BUTTON_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"
#include "JuicyGUI.h"


typedef struct {
    const char* label;
    JuicyGUI_Color color;
    int frameSize;
    JuicyGUI_Color frameColor;
    const char* fontPath;
    JuicyGUI_Color fontColor;

} JuicyGUI_Button_Template;

class JuicyGUI_Button {
    friend class JuicyGUI;
    public:
        JuicyGUI_Button(JuicyGUI* iHostUI, JuicyGUI_ID iAssignedID, uint32_t iSubType, const char* iLabel, const SDL_Rect* iRect);
        ~JuicyGUI_Button(void);
        void enable(bool iEnabled) {element.enable(iEnabled);};
        void show(bool iShow) {element.show(iShow);};
        void setRect(const SDL_Rect* iRect) {if(element.setRect(iRect)) resetTextures();};
        void getTemplate(uint32_t iActionID, JuicyGUI_Button_Template* oTemplate) {*oTemplate = properties[getTextureID(iActionID)];};
        void setTemplate(uint32_t iActionID, JuicyGUI_Button_Template* iTemplate) {if (iTemplate != NULL) properties[getTextureID(iActionID)] = *iTemplate; resetTextures();};
    private:
        JuicyGUI_Element element;
        JuicyGUI_Button_Template properties[JUICYGUI_BUTTON_NUM_STATES];
        uint32_t getTextureID(void);
        uint32_t getTextureID(uint32_t iActionID);
        void draw(void);
        void initTextures(void);
        void createTextures(void);
        void resetTextures(void);
        void destroyTextures(void);
        uint32_t _subType;
        SDL_Texture* _texture[JUICYGUI_BUTTON_NUM_STATES];
};


#endif // JUICYGUI_BUTTON_H_INCLUDED
