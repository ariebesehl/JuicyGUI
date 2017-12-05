#ifndef JUICYGUI_CHARSET_H_INCLUDED
#define JUICYGUI_CHARSET_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"
#include "JuicyGUI.h"


typedef struct {
    SDL_Point position;
    uint32_t charNum;
} JuicyGUI_Printjob;

typedef struct {
    int fontSize;
    const char* fontPath;
    JuicyGUI_Color color;
    JuicyGUI_Color fillColor;
    int lineMargin;
    uint32_t style;
} JuicyGUI_Charset_Template;

class JuicyGUI_Charset {
    friend class JuicyGUI;
    public:
        JuicyGUI_Charset(JuicyGUI* iHostUI, JuicyGUI_ID iID, const char* iFontPath, int iFontSize, JuicyGUI_Color iFontColor);
        ~JuicyGUI_Charset(void);
        void printTXT(const char* iText, SDL_Point* iPosition);
        void printlnTXT(const char* iText, SDL_Point* iPosition);
        void printINT(int iInteger, SDL_Point* iPosition);
        void printlnINT(int iInteger, SDL_Point* iPosition);
        void printHEX(int iInteger, SDL_Point* iPosition);
        void printlnHEX(int iInteger, SDL_Point* iPosition);
        void getSizeTXT(const char* iText, SDL_Point* oDimensions);
        void getSizeINT(int iInteger, SDL_Point* oDimensions);
        void setCursor(SDL_Point* iPosition);
        void getCursor(SDL_Point* oPosition);
        void setFontSize(int iSize);
        int getFontSize(void) {return properties.fontSize;};
        void setLineMargin(int iSize) {properties.lineMargin = iSize;};
        int getLineMargin(void) {return properties.lineMargin;};
    private:
        JuicyGUI_Element element;
        JuicyGUI_Charset_Template properties;
        JuicyGUI_Printjob _printjobs[JUICYGUI_CHARSET_MAX_PRINTJOBS];
        uint32_t _ctrPrintjobs;
        void printer(const char* iText);
        void printerINT(int iInteger);
        void printerHEX(int iInteger);
        void printerChar(uint32_t iCharNum, uint32_t iOptions);
        void sizer(const char* iText, SDL_Point* iTarget);
        void sizerINT(int iInteger, SDL_Point* iTarget);
        void sizerHEX(int iInteger, SDL_Point* iTarget);
        void newLine(bool iEnable);
        void carrierReturn(void);
        void draw(void);
        void initTextures(void);
        bool createTextures(void);
        void resetTextures(void);
        void destroyTextures(void);
        int _textureWidth[JUICYGUI_CHARSET_SIZE];
        int _textureHeight[JUICYGUI_CHARSET_SIZE];
        SDL_Texture* _texture[JUICYGUI_CHARSET_SIZE];
        SDL_Texture* _textureFill[JUICYGUI_CHARSET_SIZE];
};


#endif // JUICYGUI_CHARSET_H_INCLUDED
