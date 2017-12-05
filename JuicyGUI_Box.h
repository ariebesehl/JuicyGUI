#ifndef JUICYGUI_BOX_H_INCLUDED
#define JUICYGUI_BOX_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"
#include "JuicyGUI.h"


typedef struct {
    JuicyGUI_Color color;
    JuicyGUI_Color borderColor;
    int borderSize;
} JuicyGUI_Box_Template;


class JuicyGUI_Box {
    friend class JuicyGUI;
    public:
        JuicyGUI_Box(JuicyGUI* iHostUI, JuicyGUI_ID iAssignedID, SDL_Rect* iDimensions, JuicyGUI_Color iColor, JuicyGUI_Color iBorderColor);
        ~JuicyGUI_Box(void);
        void enable(bool iEnabled) {element.enable(iEnabled);};
        void show(bool iShow) {element.show(iShow);};
        void setBorderSize(int iSize) {properties.borderSize = iSize; resetTextures();}; // TODO make this work with negative values
        void setPos(const SDL_Point* iPosition) {element.setPos(iPosition);};
        void getPos(SDL_Point* oPosition) {element.getPos(oPosition);};
        const SDL_Rect* getRect(void) {return element.getRect();};
        void setRect(const SDL_Rect* iRect) {if(element.setRect(iRect)) resetTextures();};
    private:
        JuicyGUI_Element element;
        JuicyGUI_Box_Template properties;
        void draw(void);
        void initTextures(void);
        void createTextures(void);
        void resetTextures(void);
        void destroyTextures(void);
        SDL_Texture* _texture;
};



#endif // JUICYGUI_BOX_H_INCLUDED
