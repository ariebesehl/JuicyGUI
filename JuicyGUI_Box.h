#ifndef JUICYGUI_BOX_H_INCLUDED
#define JUICYGUI_BOX_H_INCLUDED

#include "JuicyGUI_Definitions.h"


typedef struct {
    JD_COLOR color;
    JD_COLOR borderColor;
    int borderSize;
} JuicyGUI_Box_Properties;


class JuicyGUI_Box {
    public:
        JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iAssignedID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iBorderColor);
        ~JuicyGUI_Box(void);
        void enable(bool iEnabled) {element.enable(iEnabled);};
        void show(bool iShow) {element.show(iShow);};
        void setBorderSize(int iSize) {properties.borderSize = iSize; resetTextures();}; // TODO make this work with negative values
        void setPos(const JD_Point* iPosition) {element.setPos(iPosition);};
        void getPos(JD_Point* oPosition) {element.getPos(oPosition);};
        const JD_Rect* getRect(void) {return element.getRect();};
        void setRect(const JD_Rect* iRect) {if(element.setRect(iRect)) resetTextures();};
    private:
		JSPR* textureEngine;
        JuicyGUI_Element element;
        JuicyGUI_Box_Properties properties;
        void createTextures(void);
        void resetTextures(void);
};



#endif // JUICYGUI_BOX_H_INCLUDED
