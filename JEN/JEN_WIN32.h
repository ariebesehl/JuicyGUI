#ifndef JEN_WIN32_H_INCLUDED
#define JEN_WIN32_H_INCLUDED

// needed so that AlphaBlend function is included
#define WINVER 0x0501

#include <windows.h>

//#define JDM_NO_SDL // 2020-11-03: Set this as GLOBAL COMPILER DEFINE instead
#include "../JDM/JDM.h"

#define JM_KEYUP 0x100
#define JM_KEYDOWN 0x200
#define JM_MOUSEMOTION 0x1000
#define JM_MOUSEBUTTONUP 0x2000
#define JM_MOUSEBUTTONDOWN 0x4000
#define JM_MOUSEWHEEL 0x8000
#define JM_QUIT 0x80000000


#define JEN_WINDOW_FLAG_NONE 0x0
#define JEN_WINDOW_FLAG_TYPE_DEFINED 0x1
#define JEN_WINDOW_FLAG_REGISTERED_CLASS 0x2
#define JEN_WINDOW_FLAG_CREATED 0x4
#define JEN_WINDOW_FLAG_INIT_DESTROY 0x8
#define JEN_WINDOW_FLAG_DESTROYED 0x10

#define JEN_WINDOW_BLENDMODE_BLOT 0x0
#define JEN_WINDOW_BLENDMODE_ALPHA 0x1

#define JEN_WINDOW_TARGET_BUFFER 0x0
#define JEN_WINDOW_TARGET_SCREEN 0x1

#define JEN_WINDOW_TYPE_UNKNOWN 0x0
#define JEN_WINDOW_TYPE_WIN32 0x1

#define JEN_WINDOW_PALETTE_RGBA 0x0
#define JEN_WINDOW_PALETTE_ARGB 0x1


#ifdef __WIN32__
#define JEN_WINDOW_TYPE_COMPILED JEN_WINDOW_TYPE_WIN32
#define JEN_WINDOW_WIN32_STYLE (WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX)
#define JEN_WINDOW_WIN32_CLASS "JUICYENGINEWIN32"
#else
#define JEN_WINDOW_TYPE_COMPILED JEN_WINDOW_TYPE_UNKNOWN
#endif

#define JEN_WINDEFAULT_COLOR_BACKGROUND 0x00000000
#define JEN_WINDEFAULT_LABEL_TITLE "Default Juicy Engine Window Title"
#define JEN_WINDEFAULT_PALETTE_IMPORT JEN_WINDOW_PALETTE_RGBA
#define JEN_WINDEFAULT_PALETTE_EXPORT JEN_WINDOW_PALETTE_RGBA

class JEN_Window {
    public:
        JEN_Window() {pNull(); pInit(NULL, NULL);};
        JEN_Window(const char* iTitle) {pNull(); pInit(iTitle, NULL);};
        JEN_Window(const char* iTitle, const JD_Rect* iRect) {pNull(); pInit(iTitle, iRect);};
        ~JEN_Window() {pExit();};
        bool IsAlive() {return !(flag & JEN_WINDOW_FLAG_DESTROYED);};
        bool IsClosing() {return (flag & JEN_WINDOW_FLAG_INIT_DESTROY);};
        void Close();
        void Kill() {Close();}
        JD_TIME GetTicks();
        JD_FLAG GetStatus() {return flag;};
        JD_FLAG GetType() {return type;};
        JD_FLAG GetStyle() {return style;};
        JD_FLAG GetMouseState() {return mouseState;}
        JD_FLAG GetMouseState(JD_Point* oMousePos) {if (oMousePos != NULL) {*oMousePos = mousePos;} return mouseState;}
        JD_I GetMouseX() {return mousePos.x;}
        JD_I GetMouseY() {return mousePos.y;}
        const JD_Point* GetMousePos() {return &mousePos;}
        void GetMousePos(JD_Point* oMousePos) {if (oMousePos != NULL) {*oMousePos = mousePos;}}
        void GetRect(JD_Rect* oRect) {if (oRect != NULL) {*oRect = rect;}};
        void GetPos(JD_Point* oPos) {if (oPos != NULL) {JDM_GetRectPos(&rect, oPos);}};
        void GetSize(JD_Point* oSize) {if (oSize != NULL) {JDM_GetRectSize(&rect, oSize);}};
        JD_I GetPosX() {return rect.x;};
        JD_I GetPosY() {return rect.y;};
        JD_I GetWidth() {return rect.w;};
        JD_I GetHeight() {return rect.h;};
        void SetPos(JD_I iX, JD_I iY);
        void SetPos(const JD_Point* iPos);
        void SetSize(JD_I iWidth, JD_I iHeight);
        void SetSize(const JD_Point* iSize);
        void SetRect(JD_I iX, JD_I iY, JD_I iWidth, JD_I iHeight);
        void SetRect(const JD_Rect* iRect);
        void SetBackgroundColor(JD_COLOR iColor);
        void Clear();
        void SystemTXT(const char* iText, const JD_Rect* iTextBox);
        void DrawBackground() {pSurfaceFill(colorBG, JEN_WINDOW_PALETTE_RGBA, JEN_WINDOW_BLENDMODE_ALPHA, JEN_WINDOW_TARGET_BUFFER);};
        void DrawBackground(JD_COLOR iColor) {pSurfaceFill(iColor, JEN_WINDOW_BLENDMODE_ALPHA, JEN_WINDOW_TARGET_BUFFER);};
        void DrawPixels(const JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst) {pSurfaceDrawPixels(iPixels, iDimensions, iDst, JEN_WINDOW_BLENDMODE_ALPHA, JEN_WINDOW_TARGET_BUFFER);};
        void Present();
        void PollMessages();
        JD_COLOR* LoadBitmapImage(const char* iPath, JD_Point* oDimensions) {return LoadBitmapImageApplyingAlpha(iPath, oDimensions, 0xff000000);};
        JD_COLOR* LoadBitmapImageApplyingAlpha(const char* iPath, JD_Point* oDimensions, JD_INDEX iOpacity) {return LoadBitmapImageApplyingAlpha(iPath, oDimensions, paletteExport, iOpacity);};
        JD_COLOR* LoadBitmapImageApplyingAlpha(const char* iPath, JD_Point* oDimensions, JD_FLAG iPaletteExport, JD_INDEX iOpacity);
        JD_COLOR* GetPixelsFromText(const char* iText, JD_Point* oDimensions, JD_I iFontSize, JD_COLOR iFontColor, JD_FLAG iFontStyle) {return GetPixelsFromText(iText, oDimensions, paletteExport, iFontSize, iFontColor, iFontStyle);};
        JD_COLOR* GetPixelsFromText(const char* iText, JD_Point* oDimensions, JD_FLAG iPaletteExport, JD_I iFontSize, JD_COLOR iFontColor, JD_FLAG iFontStyle);
        void ConvertPixelPalette(JD_COLOR* iPixels, JD_INDEX iSize, JD_FLAG iInputPalette, JD_FLAG iOutputPalette) {pColorConversion(iPixels, iSize, iInputPalette, iOutputPalette);};
        void SetImportPalette(JD_FLAG iImportPalette) {paletteImport = iImportPalette;};
        void SetExportPalette(JD_FLAG iExportPalette) {paletteExport = iExportPalette;};
    private:
        void pNull();
        void pInit(const char* iTitle, const JD_Rect* iRect);
        void pDelete();
        void pReset() {pDelete(); pInit(title, &rect);};
        void pExit();
        void pClearBuffer();
        void pSurfaceFill(JD_COLOR iColor, JD_FLAG iBlendmode, JD_FLAG iTarget) {pSurfaceFill(iColor, paletteImport, iBlendmode, iTarget);};
        void pSurfaceFill(JD_COLOR iColor, JD_FLAG iPaletteImport, JD_FLAG iBlendmode, JD_FLAG iTarget);
        void pSurfaceDrawPixels(const JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst, JD_FLAG iBlendmode, JD_FLAG iTarget) {pSurfaceDrawPixels(iPixels, iDimensions, iDst, paletteImport, iBlendmode, iTarget);};
        void pSurfaceDrawPixels(const JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst, JD_FLAG iPaletteImport, JD_FLAG iBlendmode, JD_FLAG iTarget);
        JD_COLOR pColorRGBA_ARGB(JD_COLOR iColor) {return ((iColor << 24) | (iColor >> 8));};
        JD_COLOR pColorARGB_RGBA(JD_COLOR iColor) {return ((iColor >> 24) | (iColor << 8));};
        JD_COLOR pColorRGBA_ABGR(JD_COLOR iColor) {return ((iColor << 24) | (iColor >> 24) | ((iColor >> 8) & 0xff00) | ((iColor << 8) & 0xff0000));};
        void pColorConversion(JD_COLOR* iPixels, const JD_Point* iSize, JD_FLAG iInputType, JD_FLAG iOutputType) {if (iSize != NULL) {pColorConversion(iPixels, JDM_GetArea(iSize), iInputType, iOutputType);}};
        void pColorConversion(JD_COLOR* iPixels, JD_INDEX iSize, JD_FLAG iInputType, JD_FLAG iOutputType);


        void pWIN32_Init();
        void pWIN32_Delete();
        void pWIN32_Exit();
        void pWIN32_CloseWindow();
        void pWIN32_InitDC();
        void pWIN32_ClearDC();
        void pWIN32_ResetDC() {pWIN32_ClearDC(); pWIN32_InitDC();};
        void pWIN32_SetRects();
        bool pWIN32_PixelIsTransparent(JD_COLOR iPixel, JD_COLOR iTransparency);
        JD_COLOR pWIN32_PixelApplyAlpha(JD_COLOR iPixel);
        JD_COLOR* pWIN32_LoadBitmap(const char* iFilepath, JD_Point* oDimensions, JD_COLOR iAlphaMask);
        JD_COLOR* pWIN32_GetPixelsFromTXT(const char* iText, JD_Point* oDimensions, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle);
        JD_COLOR* pWIN32_BitmapToPixel(HBITMAP* iBitmap, JD_Point* oDimensions, JD_INDEX iOpacity, JD_COLOR iAlphaMask);
        void pWIN32_DrawPixels(JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst, JD_FLAG iBlendmode, JD_FLAG iTarget);
        void pWIN32_PrintTXT(const char* iText, const JD_Rect* iRect);
        void pWIN32_Present();
        JD_TIME pWIN32_GetTicks() {return (JD_TIME)timeGetTime();};
        JEN_Window* pWIN32_GetThis() {return (JEN_Window*)GetProp(win32Handle, "this");};
        void pWIN32_SetThis() {SetProp(win32Handle, "this", this);};
        void pWIN32_RemoveThis() {RemoveProp(win32Handle, "this");};
        void pWIN32_PollMessages();
        JD_INDEX pWIN32_PostMessage(JD_FLAG iWM_Event, JD_FLAG iWParam, JD_FLAG iLParam);
        static LRESULT CALLBACK pWIN32_Main(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        void pWM_Move(JD_I iX, JD_I iY);
        void pWM_Size(JD_I iWidth, JD_I iHeight);
        void pWM_Paint();
        void pWM_EraseBkgnd();
        void pWM_Close();
        void pWM_Destroy();
        void pWM_NCDestroy();
        void pWM_MouseMove(JD_FLAG iWParam, JD_FLAG iLParam);
        void pWM_MouseWheel();

        JD_FLAG flag;
        JD_FLAG type;
        JD_FLAG style;
        JD_Rect rect;
        JD_COLOR colorBG;
        JD_FLAG paletteImport;
        JD_FLAG paletteExport;
        const char* title;
        JD_Point mousePos;
        JD_FLAG mouseState;

        HINSTANCE win32Instance;
        HWND win32Handle;
        DWORD win32Style;
        MSG win32Message;
        HDC win32DC;
        HDC win32DCBuffer;
        HDC win32DCPipe;
        HDC win32DCFont;
        RECT win32Rect;
        RECT win32Size;
};


#endif // JEN_WIN32_H_INCLUDED
