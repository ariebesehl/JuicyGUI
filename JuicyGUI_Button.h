#ifndef JUICYGUI_BUTTON_H_INCLUDED
#define JUICYGUI_BUTTON_H_INCLUDED

#include "JuicyGUI_Definitions.h"

typedef struct {
    const char* label;
    JD_COLOR color;
    int frameSize;
    JD_COLOR frameColor;
    const char* fontPath;
    JD_COLOR fontColor;
} JuicyGUI_Button_Properties;

class JuicyGUI_Button {
    public:
        JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iAssignedID, JD_FLAG iFlag, const char* iLabel, const JD_Rect* iRect);
        ~JuicyGUI_Button(void);
        void Enable(bool iEnabled) {element.enable(iEnabled);};
        void Show(bool iShow) {element.show(iShow);};
        void SetRect(const JD_Rect* iRect) {if(element.setRect(iRect)) resetTextures();};
        bool GetProperties(JuicyGUI_Button_Properties* oProperties);
        bool GetProperties(JD_FLAG iActionID, JuicyGUI_Button_Properties* oProperties);// {*oTemplate = properties[getTextureID(iActionID)];};
        bool SetProperties(JuicyGUI_Button_Properties* iProperties);
        bool SetProperties(JD_FLAG iActionID, JuicyGUI_Button_Properties* iProperties);// {if (iTemplate != NULL) properties[getTextureID(iActionID)] = *iTemplate; resetTextures();};
    private:
        JD_FLAG flag;
        JuicyGUI_Element element;
        JuicyGUI_Button_Properties** states;
		JSPR* textureEngine;
        void initStates(void);
        void resetStates(void);
        void destroyStates(void);
        void createTextures(void);
        void resetTextures(void);

        void initDefault(const char* iLabel);
};


#endif // JUICYGUI_BUTTON_H_INCLUDED
