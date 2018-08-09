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
        void printTXT(const char* iText, JD_Point* iPosition);
        void printlnTXT(const char* iText, JD_Point* iPosition);
        void printINT(JD_I iInteger, JD_Point* iPosition);
        void printlnINT(JD_I iInteger, JD_Point* iPosition);
        void printHEX(JD_I iInteger, JD_Point* iPosition);
        void printlnHEX(JD_I iInteger, JD_Point* iPosition);
        void getSizeTXT(const char* iText, JD_Point* oDimensions);
        void getSizeINT(JD_I iInteger, JD_Point* oDimensions);
        void setCursor(JD_Point* iPosition);
        void getCursor(JD_Point* oPosition);
        void setFontSize(JD_I iSize);
        JD_I getFontSize(void) {return properties.fontSize;};
        void setLineMargin(JD_I iSize) {properties.lineMargin = iSize;};
        JD_I getLineMargin(void) {return properties.lineMargin;};
        void enable(bool iEnabled) {element.enable(iEnabled);};
        void show(bool iShow) {element.show(iShow);};
        void setStyle(JD_FLAG iStyle);
    private:
		JSPR* textureEngine;
        JuicyGUI_Element element;
        JuicyGUI_Charset_Properties properties;
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
