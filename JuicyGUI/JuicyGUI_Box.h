#ifndef JUICYGUI_BOX_H_INCLUDED
#define JUICYGUI_BOX_H_INCLUDED

#include "JuicyGUI_Definitions.h"


typedef struct {
    JD_COLOR color;
    JD_COLOR frameColor;
    JD_I frameSize;
} JuicyGUI_Box_Properties;

JuicyGUI_Box_Properties* JPM_CreateDefault_Box();

class JuicyGUI_Box {
    public:
        JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iAssignedID);
        JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iAssignedID, JD_Rect* iDimensions);
        JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iAssignedID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iBorderColor);
        ~JuicyGUI_Box(void);
        void Enable(bool iEnabled) {element.enable(iEnabled);};
        void Show(bool iShow) {element.show(iShow);};
        //void SetFrameSize(int iSize) {properties.frameSize = iSize; resetTextures();}; // TODO make this work with negative values
        void SetPos(const JD_Point* iPosition) {element.setPos(iPosition);};
        void GetPos(JD_Point* oPosition) {element.getPos(oPosition);};
        const JD_Rect* GetRect(void) {return element.getRect();};
        void SetRect(const JD_Rect* iRect) {if(element.setRect(iRect)) {resetTextures();}};
        JD_I GetFrameSize(JD_FLAG iEvent);
        JD_COLOR GetFrameColor(JD_FLAG iEvent);
        JD_COLOR GetColor(JD_FLAG iEvent);
        void SetFrameSize(JD_FLAG iEvent, JD_I iSize);
        void SetFrameSize(JD_I iSize);
        void SetFrameColor(JD_FLAG iEvent, JD_COLOR iColor);
        void SetFrameColor(JD_COLOR iColor);
        void SetColor(JD_FLAG iEvent, JD_COLOR iColor);
        void SetColor(JD_COLOR iColor);
    private:
        JuicyGUI_Element element;
        JuicyGUI_Box_Properties* defaultProperties;
		void init(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iFrameColor);
		void initDefault(JD_COLOR iColor, JD_COLOR iFrameColor);
        void createTextures(void);
        void resetTextures(void);
};



#endif // JUICYGUI_BOX_H_INCLUDED
