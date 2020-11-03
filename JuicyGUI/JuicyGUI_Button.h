#ifndef JUICYGUI_BUTTON_H_INCLUDED
#define JUICYGUI_BUTTON_H_INCLUDED

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"
#include "JuicyGUI_Charset.h"

typedef struct {
    const char* label;
    JuicyGUI_Box_Properties boxProperties;
    JuicyGUI_Charset_Properties fontProperties;
} JuicyGUI_Button_Properties;

class JuicyGUI_Button {
    public:
        JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iAssignedID, const char* iLabel);
        JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iAssignedID, const char* iLabel, const JD_Rect* iRect);
        ~JuicyGUI_Button(void);
        void Enable(bool iEnabled) {element.enable(iEnabled);};
        void Show(bool iShow) {element.show(iShow);};
        void SetRect(const JD_Rect* iRect) {if(element.setRect(iRect)) resetTextures();};
    private:
        void init(JuicyGUI* iHostUI, JD_INDEX iID, const char* iLabel, const JD_Rect* iRect, JD_FLAG iFlag);
        JD_FLAG flag;
        JuicyGUI_Element element;
        JuicyGUI_Button_Properties* defaultProperties;
        void createTextures(void);
        void resetTextures(void);

        void initDefault(const char* iLabel);
};


#endif // JUICYGUI_BUTTON_H_INCLUDED
