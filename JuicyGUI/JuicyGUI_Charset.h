#ifndef JUICYGUI_CHARSET_H_INCLUDED
#define JUICYGUI_CHARSET_H_INCLUDED

#include "JuicyGUI_Definitions.h"

typedef struct {
    JD_I fontSize;
    const char* fontPath;
    JD_COLOR color;
    JD_COLOR fillColor;
    JD_I lineMargin;
	JD_FLAG style;
} JuicyGUI_Charset_Properties;

class JuicyGUI_Charset {
    public:
        JuicyGUI_Charset(JuicyGUI* iHostUI, JD_INDEX iID, const char* iFontPath, JD_I iFontSize, JD_COLOR iFontColor);
        ~JuicyGUI_Charset(void);
        void CenterTXT(const char* iText, const JD_Point* iPosition);
        void CenterlnTXT(const char* iText) {CenterlnTXT(iText, NULL);};
        void CenterlnTXT(const char* iText, const JD_Point* iPosition);
        void CenterINT(JD_I iInteger, const JD_Point* iPosition);
        void PrintTXT(const char* iText) {PrintTXT(iText, NULL);};
        void PrintTXT(const char* iText, JD_Point* iPosition);
        void PrintlnTXT(const char* iText){PrintlnTXT(iText, NULL);};
        void PrintlnTXT(const char* iText, JD_Point* iPosition);
        void PrintINT(JD_I iInteger) {PrintINT(iInteger, NULL);};
        void PrintINT(JD_I iInteger, JD_Point* iPosition);
        void PrintlnINT(JD_I iInteger) {PrintlnINT(iInteger, NULL);};
        void PrintlnINT(JD_I iInteger, JD_Point* iPosition);
        void PrintHEX(JD_I iInteger) {PrintHEX(iInteger, NULL);};
        void PrintHEX(JD_I iInteger, JD_Point* iPosition);
        void PrintlnHEX(JD_I iInteger) {PrintlnHEX(iInteger, NULL);};
        void PrintlnHEX(JD_I iInteger, JD_Point* iPosition);
        void GetSizeTXT(const char* iText, JD_Point* oDimensions);
        void GetSizeINT(JD_I iInteger, JD_Point* oDimensions);
        void SetCursor(JD_Point* iPosition);
        void GetCursor(JD_Point* oPosition);
        void SetFontSize(JD_I iSize);
        JD_I GetFontSize(void) {return ((JuicyGUI_Charset_Properties*)element.properties->GetPropertiesAtIndex(0))->fontSize;};
        void SetLineMargin(JD_I iSize) {((JuicyGUI_Charset_Properties*)element.properties->GetPropertiesAtIndex(0))->lineMargin = iSize;};
        JD_I GetLineMargin(void) {return ((JuicyGUI_Charset_Properties*)element.properties->GetPropertiesAtIndex(0))->lineMargin;};
        void Enable(bool iEnabled) {element.enable(iEnabled);};
        void Show(bool iShow) {element.show(iShow);};
        void SetStyle(JD_FLAG iStyle);
    private:
        JuicyGUI_Element element;
        JuicyGUI_Charset_Properties* defaultProperties;
        void initDefault(const char* iFontPath, JD_I iFontsize, JD_COLOR iFontColor);
        void printer(const char* iText);
        void printerINT(JD_I iInteger);
        void printerHEX(JD_I iInteger);
        void printerChar(JD_INDEX iCharNum, JD_FLAG iOptions);
        void sizer(const char* iText, JD_Point* iTarget);
        void sizerINT(JD_I iInteger, JD_Point* iTarget);
        void sizerHEX(JD_I iInteger, JD_Point* iTarget);
        void newLine(bool iEnable);
        void carrierReturn(void);
        bool createTextures(void);
        void resetTextures(void);
		JD_Point** charSizes;
		JD_INDEX styleOffset;
		void setStyleOffset();
};


#endif // JUICYGUI_CHARSET_H_INCLUDED
