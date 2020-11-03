
#include "JEN_WIN32.h"
#include <iostream>


void JEN_Window::pNull() {
    JDM_MemZero((void*)this, sizeof(*this));
}
void JEN_Window::pInit(const char* iTitle, const JD_Rect* iRect) {
    title = iTitle != NULL ? iTitle : JEN_WINDEFAULT_LABEL_TITLE;
    type = JEN_WINDOW_TYPE_COMPILED;
    flag |= JEN_WINDOW_FLAG_TYPE_DEFINED;
    colorBG = JEN_WINDEFAULT_COLOR_BACKGROUND;
    paletteImport = JEN_WINDEFAULT_PALETTE_IMPORT;
    paletteExport = JEN_WINDEFAULT_PALETTE_EXPORT;
    if (iRect != NULL) {
        SetRect(iRect);
    } else {
        SetRect(GetSystemMetrics(SM_CXFULLSCREEN) / 4, GetSystemMetrics(SM_CYFULLSCREEN) / 4, GetSystemMetrics(SM_CXFULLSCREEN) / 2, GetSystemMetrics(SM_CYFULLSCREEN) / 2);
    }
    pWIN32_Init();
}
void JEN_Window::pDelete() {
    pWIN32_Delete();
}
void JEN_Window::pExit() {
    pDelete();
    pWIN32_Exit();
}

void JEN_Window::Close() {
    pWIN32_CloseWindow();
}

void JEN_Window::PollMessages() {
    pWIN32_PollMessages();
}
JD_TIME JEN_Window::GetTicks() {
    return pWIN32_GetTicks();
}

void JEN_Window::SetPos(JD_I iX, JD_I iY) {
    rect.x = iX;
    rect.y = iY;
    pWIN32_SetRects();
}
void JEN_Window::SetPos(const JD_Point* iPos) {
    if (iPos != NULL) {
        JDM_SetRectPos(&rect, iPos);
        pWIN32_SetRects();
    }
}
void JEN_Window::SetSize(JD_I iWidth, JD_I iHeight) {
    rect.w = iWidth;
    rect.h = iHeight;
    pWIN32_SetRects();
}
void JEN_Window::SetSize(const JD_Point* iSize) {
    if (iSize != NULL) {
        JDM_SetRectSize(&rect, iSize);
        pWIN32_SetRects();
    }
}
void JEN_Window::SetRect(JD_I iX, JD_I iY, JD_I iWidth, JD_I iHeight) {
    JDM_SetRect(&rect, iX, iY, iWidth, iHeight);
    pWIN32_SetRects();
}
void JEN_Window::SetRect(const JD_Rect* iRect) {
    if (iRect != NULL) {
        rect = *iRect;
        pWIN32_SetRects();
    }
}

void JEN_Window::SetBackgroundColor(JD_COLOR iColor) {
    switch (paletteImport) {
        case JEN_WINDOW_PALETTE_RGBA:
            colorBG = iColor;
            break;
        default:
        case JEN_WINDOW_PALETTE_ARGB:
            colorBG = pColorARGB_RGBA(iColor);
            break;
    }
}

void JEN_Window::Clear() {pClearBuffer();}
void JEN_Window::Present() {
    pWIN32_Present();
}
void JEN_Window::pClearBuffer() {pSurfaceFill(0x0, JEN_WINDOW_BLENDMODE_BLOT, JEN_WINDOW_TARGET_BUFFER);}
void JEN_Window::pSurfaceFill(JD_COLOR iColor, JD_FLAG iPaletteImport, JD_FLAG iBlendmode, JD_FLAG iTarget) {
    JD_Point windowSize;
    JD_Rect windowRect = rect;
    JDM_EmptyRectPos(&windowRect);
    JDM_GetRectSize(&windowRect, &windowSize);
    JD_COLOR renderColor = 0x0;
    switch (iPaletteImport) {
        case JEN_WINDOW_PALETTE_ARGB:
            renderColor = iColor;
            break;
        default:
        case JEN_WINDOW_PALETTE_RGBA:
            renderColor = pColorRGBA_ARGB(iColor);
            break;
    }
    JD_COLOR* fillPixels = (JD_COLOR*)JDM_InitData(JDM_GetArea(&windowSize), renderColor);
    pSurfaceDrawPixels(fillPixels, &windowSize, &windowRect, JEN_WINDOW_PALETTE_ARGB, iBlendmode, iTarget);
    delete[] fillPixels;
}

void JEN_Window::pSurfaceDrawPixels(const JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst, JD_FLAG iPaletteImport, JD_FLAG iBlendmode, JD_FLAG iTarget) {
    if (iPixels != NULL && iDimensions != NULL && iDst != NULL) {
        JD_COLOR* pixelCopy = NULL;
        JD_COLOR* pixelSrc = NULL;
        if (iBlendmode == JEN_WINDOW_BLENDMODE_BLOT) {
            pixelSrc = (JD_COLOR*)iPixels;
        } else {
            pixelCopy = JDM_CopyData(iPixels, new JD_COLOR[JDM_GetArea(iDimensions)], JDM_GetArea(iDimensions));
            pColorConversion(pixelCopy, JDM_GetArea(iDimensions), iPaletteImport, JEN_WINDOW_PALETTE_ARGB);
            pixelSrc = pixelCopy;
        }
        JD_Rect targetRect = *iDst;
        if (targetRect.w > 0 && targetRect.h > 0) {
            if (targetRect.x < 0) {targetRect.x = 0;}
            if (!(targetRect.x < rect.w)) {targetRect.x = rect.w;}
            if (targetRect.y < 0) {targetRect.y = 0;}
            if (!(targetRect.y < rect.h)) {targetRect.y = rect.h;}
            if (!((targetRect.x + targetRect.w) < rect.w)) {targetRect.w = rect.w - targetRect.x;}
            if (!((targetRect.y + targetRect.h) < rect.h)) {targetRect.h = rect.h - targetRect.y;}
            if (targetRect.w > iDimensions->x) {targetRect.w = iDimensions->x;}
            if (targetRect.h > iDimensions->y) {targetRect.h = iDimensions->y;}
            pWIN32_DrawPixels(pixelSrc, iDimensions, &targetRect, iBlendmode, iTarget);
        }
        if (pixelCopy != NULL) {delete[] pixelCopy;}
    }
}
JD_COLOR* JEN_Window::LoadBitmapImageApplyingAlpha(const char* iFilepath, JD_Point* oDimensions, JD_FLAG iPaletteExport, JD_COLOR iOpacity) {
    JD_COLOR* bitmapPixels = NULL;
    if (iFilepath != NULL && oDimensions != NULL) {
        bitmapPixels = pWIN32_LoadBitmap(iFilepath, oDimensions, iOpacity);
        pColorConversion(bitmapPixels, JDM_GetArea(oDimensions), JEN_WINDOW_PALETTE_ARGB, iPaletteExport);
    }
    return bitmapPixels;
}
void JEN_Window::SystemTXT(const char* iText, const JD_Rect* iTextBox) {
    pWIN32_PrintTXT(iText, iTextBox);
}
JD_COLOR* JEN_Window::GetPixelsFromText(const char* iText, JD_Point* oDimensions, JD_FLAG iPaletteExport, JD_I iFontSize, JD_COLOR iFontColor, JD_FLAG iFontStyle) {
    JD_COLOR* textPixels = NULL;
    if (oDimensions != NULL) {
        textPixels = pWIN32_GetPixelsFromTXT(iText, oDimensions, iFontSize, pColorRGBA_ARGB(iFontColor), iFontStyle);
        pColorConversion(textPixels, JDM_GetArea(oDimensions), JEN_WINDOW_PALETTE_ARGB, iPaletteExport);
    }
    return textPixels;
}

void JEN_Window::pColorConversion(JD_COLOR* iPixels, JD_INDEX iSize, JD_FLAG iInputType, JD_FLAG iOutputType) {
    if (iPixels != NULL && iSize) {
        switch (iInputType) {
            case JEN_WINDOW_PALETTE_ARGB:
                switch (iOutputType) {
                    case JEN_WINDOW_PALETTE_ARGB: break;
                    default:
                    case JEN_WINDOW_PALETTE_RGBA: for (JD_INDEX i = 0; i < iSize; i++) {iPixels[i] = pColorARGB_RGBA(iPixels[i]);} break;
                }
                break;
            default:
            case JEN_WINDOW_PALETTE_RGBA:
                switch (iOutputType) {
                    case JEN_WINDOW_PALETTE_ARGB: for (JD_INDEX i = 0; i < iSize; i++) {iPixels[i] = pColorRGBA_ARGB(iPixels[i]);} break;
                    default:
                    case JEN_WINDOW_PALETTE_RGBA: break;
                }
                break;
        }
    }
}

void JEN_Window::pWIN32_Init() {
    win32Instance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    JDM_MemZero(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &pWIN32_Main;
    wcex.hInstance = win32Instance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wcex.lpszClassName = JEN_WINDOW_WIN32_CLASS;
    wcex.hIconSm = (HICON)LoadImage(win32Instance, MAKEINTRESOURCE(5), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    if (RegisterClassEx(&wcex)) {
        flag |= JEN_WINDOW_FLAG_REGISTERED_CLASS;
        win32Style = JEN_WINDOW_WIN32_STYLE;
        RECT tempRect = win32Rect;
        AdjustWindowRectEx(&tempRect, win32Style, FALSE, 0);
        win32Handle = CreateWindow(JEN_WINDOW_WIN32_CLASS, title, win32Style, tempRect.left, tempRect.top, tempRect.right - tempRect.left, tempRect.bottom - tempRect.top, NULL, NULL, win32Instance, NULL);
        if (win32Handle != NULL) {
            flag |= JEN_WINDOW_FLAG_CREATED;
            pWIN32_SetThis();
            timeBeginPeriod(1);
            pWIN32_InitDC();
            AddFontResourceEx("res/OpenSans/OpenSans-Regular.ttf", FR_PRIVATE, 0);
            ShowWindow(win32Handle, SW_SHOW);
            UpdateWindow(win32Handle);
        }
    }
}
void JEN_Window::pWIN32_Delete() {
    pWIN32_ClearDC();
}
void JEN_Window::pWIN32_Exit() {
    if (!(flag & JEN_WINDOW_FLAG_DESTROYED)) {pWIN32_CloseWindow();}
    RemoveFontResourceEx("res/OpenSans/OpenSans-Regular.ttf", FR_PRIVATE, 0);
    timeEndPeriod(1);
}
void JEN_Window::pWIN32_CloseWindow() {
    DestroyWindow(win32Handle);
}
void JEN_Window::pWIN32_InitDC() {
    if (flag & JEN_WINDOW_FLAG_CREATED)  {
        win32DC = GetDC(win32Handle);
        win32DCBuffer = CreateCompatibleDC(win32DC);
        win32DCPipe = CreateCompatibleDC(win32DC);
        win32DCFont = CreateCompatibleDC(win32DC);
        //SetBkMode(win32DCFont, TRANSPARENT);
        HBITMAP bitmapSize = CreateCompatibleBitmap(win32DC, rect.w, rect.h);
        SelectObject(win32DCBuffer, bitmapSize);
        if (bitmapSize != NULL) {DeleteObject(bitmapSize);}
    }
}
void JEN_Window::pWIN32_ClearDC() {
    if (win32DCFont != NULL) {DeleteDC(win32DCFont); win32DCFont = NULL;}
    if (win32DCPipe != NULL) {DeleteDC(win32DCPipe); win32DCPipe = NULL;}
    if (win32DCBuffer != NULL) {DeleteDC(win32DCBuffer); win32DCBuffer = NULL;}
    if (win32DC != NULL) {ReleaseDC(win32Handle, win32DC); win32DC = NULL;}
}

void JEN_Window::pWIN32_SetRects() {
    RECT oldSize = win32Size;
    win32Rect.left = rect.x;
    win32Rect.top = rect.y;
    win32Rect.right = rect.x + rect.w;
    win32Rect.bottom = rect.y + rect.h;
    win32Size.left = 0;
    win32Size.top = 0;
    win32Size.right = rect.w;
    win32Size.bottom = rect.h;
    if (win32Size.right != oldSize.right || win32Size.bottom != oldSize.bottom) {
        pWIN32_ResetDC();
    }
}

JD_COLOR JEN_Window::pWIN32_PixelApplyAlpha(JD_COLOR iPixel) {
    JD_COLOR pixAlpha = (iPixel >> 24) & 0xff;
    JD_COLOR pixR = ((iPixel >> 16) & 0xff) * pixAlpha / 0xff;
    JD_COLOR pixG = ((iPixel >> 8) & 0xff) * pixAlpha / 0xff;
    JD_COLOR pixB = (iPixel & 0xff) * pixAlpha / 0xff;
    return ((pixAlpha << 24) | (pixR << 16) | (pixG << 8) |  pixB);
}
bool JEN_Window::pWIN32_PixelIsTransparent(JD_COLOR iPixel, JD_COLOR iTransparency) {
    JD_I offTarget = 0;
    JD_I offTransparency = 0;
    JD_I colorTarget = ((~iTransparency) & 0xffffff);
    for (JD_INDEX i = 0; i < 24; i += 8) {
        JD_I chPixel = (iPixel >> i) & 0xff;
        JD_I chTransparency = (iTransparency >> i) & 0xff;
        JD_I chTarget = (colorTarget >> i) & 0xff;
        offTarget += (chTarget - chPixel);
        offTransparency += (chPixel - chTransparency);
    }
    return (offTransparency < offTarget);
}
void JEN_Window::pWIN32_DrawPixels(JD_COLOR* iPixels, const JD_Point* iDimensions, const JD_Rect* iDst, JD_FLAG iBlendmode, JD_FLAG iTarget) {
    HDC* targetDC = NULL;
    switch (iTarget) {
        case JEN_WINDOW_TARGET_SCREEN: targetDC = &win32DC; break;
        case JEN_WINDOW_TARGET_BUFFER: targetDC = &win32DCBuffer; break;
        default: break;
    }
    if (targetDC != NULL) {
        HBITMAP w32Bitmap = NULL;
        switch (iBlendmode) {
            case JEN_WINDOW_BLENDMODE_ALPHA: {
                    BLENDFUNCTION blendFunction;
                    blendFunction.AlphaFormat = AC_SRC_ALPHA;
                    blendFunction.BlendFlags = 0x0;
                    blendFunction.BlendOp = AC_SRC_OVER;
                    blendFunction.SourceConstantAlpha = 0xff;
                    JD_INDEX bmLength = JDM_GetArea(iDimensions);
                    for (JD_INDEX i = 0; i < bmLength; i++) {iPixels[i] = pWIN32_PixelApplyAlpha(iPixels[i]);}
                    w32Bitmap = CreateBitmap(iDimensions->x, iDimensions->y, 1, 32, (void*)iPixels);
                    SelectObject(win32DCPipe, w32Bitmap);
                    AlphaBlend(*targetDC, iDst->x, iDst->y, iDst->w, iDst->h, win32DCPipe, 0, 0, iDimensions->x, iDimensions->y, blendFunction);
                }
                break;
            case JEN_WINDOW_BLENDMODE_BLOT: {
                    w32Bitmap = CreateBitmap(iDimensions->x, iDimensions->y, 1, 32, (void*)iPixels);
                    SelectObject(win32DCPipe, w32Bitmap);
                    BitBlt(*targetDC, 0, 0, rect.w, rect.h, win32DCPipe, 0, 0, SRCCOPY);
                }
                break;
            default:
                break;
        }
        if (w32Bitmap != NULL) {DeleteObject(w32Bitmap);}
    }
}
void JEN_Window::pWIN32_Present() {
    BitBlt(win32DC, 0, 0, rect.w, rect.h, win32DCBuffer, 0, 0, SRCCOPY);
}
JD_COLOR* JEN_Window::pWIN32_LoadBitmap(const char* iFilepath, JD_Point* oDimensions, JD_INDEX iOpacity) {
    JD_COLOR* pixelMap = NULL;
    HBITMAP loadedFile = (HBITMAP)LoadImage(win32Instance, iFilepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (loadedFile != NULL) {
        pixelMap = pWIN32_BitmapToPixel(&loadedFile, oDimensions, iOpacity, 0x0);
        DeleteObject(loadedFile);
    }
    return pixelMap;
}
JD_COLOR* JEN_Window::pWIN32_BitmapToPixel(HBITMAP* iBitmap, JD_Point* oDimensions, JD_INDEX iOpacity, JD_COLOR iAlphaMask) {
    JD_COLOR* pixelMap = NULL;
    if (iBitmap != NULL) {
        BITMAPINFO fileInfo;
        JDM_MemZero(&fileInfo, sizeof(fileInfo));
        fileInfo.bmiHeader.biSize = sizeof(fileInfo.bmiHeader);
        if (GetDIBits(win32DCPipe, *iBitmap, 0, 0, NULL, &fileInfo, DIB_RGB_COLORS)) {
            if (fileInfo.bmiHeader.biWidth > 0 && fileInfo.bmiHeader.biHeight > 0) {
                JD_INDEX width = fileInfo.bmiHeader.biWidth;
                JD_INDEX height = fileInfo.bmiHeader.biHeight;
                JD_INDEX transparency = ((iAlphaMask >> 24) & 0xff);
                JD_COLOR maskAlpha = (iOpacity << 24);
                JD_COLOR maskAlphaTransparent = ((0xff - transparency) << 24);
                JD_COLOR maskTransparent = (iAlphaMask & 0xffffff);
                if (oDimensions != NULL) {JDM_SetPoint(oDimensions, width, height);}
                pixelMap = (JD_COLOR*)JDM_InitData(width * height, 0x0);
                switch (fileInfo.bmiHeader.biBitCount) {
                    case 32:
                        for (JD_INDEX y = 0; y < height; y++) {
                            GetDIBits(win32DCPipe, *iBitmap, y, 1, (void*)&pixelMap[width * (height - y - 1)], &fileInfo, DIB_RGB_COLORS);
                            for (JD_INDEX x = 0; x < width; x++) {
                                pixelMap[width * (height - y - 1) + x] |= maskAlpha;
                            }
                        }
                        if (transparency) {
                            JD_INDEX length = width * height;
                            for (JD_INDEX i = 0; i < length; i++) {
                                //if (pWIN32_PixelIsTransparent(pixelMap[i], maskTransparent)) {
                                if ((pixelMap[i] & 0xffffff) == maskTransparent) {
                                    pixelMap[i] &= 0xffffff;
                                    pixelMap[i] |= maskAlphaTransparent;
                                }
                            }
                        }
                        break;
                    case 1: {
                            BITMAP bmStruct;
                            GetObject(*iBitmap, sizeof(BITMAP), &bmStruct);
                            JD_INDEX scanWidth = bmStruct.bmWidthBytes / 4;
                            if (bmStruct.bmWidthBytes % 4) {scanWidth++;}
                            JD_COLOR* buffer = JDM_InitData(scanWidth, 0x0);
                            if (transparency) {
                                for (JD_INDEX y = 0; y < height; y++) {
                                    GetDIBits(win32DCPipe, *iBitmap, y, 1, (void*)buffer, &fileInfo, DIB_RGB_COLORS);
                                    for (JD_INDEX x = 0; x < width; x++) {
                                        JD_INDEX bitShiftVal = (((x >> 3) + 1) << 3) - (x % 8) - 1;
                                        pixelMap[width * (height - y - 1) + x] = (bool(buffer[x >> 5] & (0x1 << bitShiftVal)) == bool(maskTransparent) ? (0x0 | maskAlphaTransparent) : (0xffffff | maskAlpha));
                                    }
                                }
                            } else {
                                for (JD_INDEX y = 0; y < height; y++) {
                                    GetDIBits(win32DCPipe, *iBitmap, y, 1, (void*)buffer, &fileInfo, DIB_RGB_COLORS);
                                    for (JD_INDEX x = 0; x < width; x++) {
                                        JD_INDEX bitShiftVal = (((x >> 3) + 1) << 3) - (x % 8) - 1;
                                        pixelMap[width * (height - y - 1) + x] = ((buffer[x >> 5] & (0x1 << bitShiftVal)) ? 0xffffff : 0x0) | maskAlpha;
                                    }
                                }
                            }
                            delete[] buffer;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return pixelMap;
}
void JEN_Window::pWIN32_PrintTXT(const char* iText, const JD_Rect* iTextBox) {
    if (iText != NULL) {
        const JD_Rect* rectSrc = &rect;
        if (iTextBox != NULL) {rectSrc = iTextBox;}
        RECT w32Rect;
        w32Rect.left = rectSrc->x;
        w32Rect.top = rectSrc->y;
        w32Rect.right = w32Rect.left + rectSrc->w;
        w32Rect.bottom = w32Rect.top + rectSrc->h;
        DrawText(win32DCBuffer, iText, -1, &w32Rect, DT_NOCLIP);
    }
}
#define JEN_WINDOW_WIN32_FONT_TRANSPARENCY_COLOR 0x808080
JD_COLOR* JEN_Window::pWIN32_GetPixelsFromTXT(const char* iText, JD_Point* oDimensions, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle) {
    JD_COLOR* pixelsTXT = NULL;
    if (iText != NULL) {
        JD_INDEX fontHeight = -MulDiv(iFontsize, GetDeviceCaps(win32DC, LOGPIXELSY), 72);
        HFONT hFont = CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, "Open Sans");
        if (hFont != NULL) {
            SelectObject(win32DCFont, hFont);
            SetTextColor(win32DCFont, 0xffffff);
            SetBkColor(win32DCFont, JEN_WINDOW_WIN32_FONT_TRANSPARENCY_COLOR);
            RECT w32Rect;
            w32Rect.left = 0;
            w32Rect.top = 0;
            DrawText(win32DCFont, iText, -1, &w32Rect, DT_CALCRECT);
            HBITMAP bitmapSize = CreateCompatibleBitmap(win32DC, w32Rect.right - w32Rect.left, w32Rect.bottom - w32Rect.top);
            if (bitmapSize != NULL) {
                SelectObject(win32DCFont, bitmapSize);
                DrawText(win32DCFont, iText, -1, &w32Rect, DT_NOCLIP);
                pixelsTXT = pWIN32_BitmapToPixel(&bitmapSize, oDimensions, (iFontcolor >> 24), (0xff000000 | JEN_WINDOW_WIN32_FONT_TRANSPARENCY_COLOR));
                if (pixelsTXT != NULL) {
                    JD_INDEX length = (w32Rect.right - w32Rect.left) * (w32Rect.bottom - w32Rect.top);
                    JD_COLOR colorMask = ((iFontcolor & 0xffffff) | 0xff000000);
                    for (JD_INDEX i = 0; i < length; i++) {pixelsTXT[i] &= colorMask;}
                }
                DeleteObject(bitmapSize);
            }
            DeleteObject(hFont);
        }
    }
    return pixelsTXT;
}

void JEN_Window::pWM_Move(JD_I iX, JD_I iY) {SetPos(iX, iY);}
void JEN_Window::pWM_Size(JD_I iWidth, JD_I iHeight) {SetSize(iWidth, iHeight);}
void JEN_Window::pWM_Paint() {;}
void JEN_Window::pWM_EraseBkgnd() {;}
void JEN_Window::pWM_Close() {
    if (flag & JEN_WINDOW_FLAG_INIT_DESTROY) {
        pWIN32_CloseWindow();
    } else {
        flag |= JEN_WINDOW_FLAG_INIT_DESTROY;
    }
}
void JEN_Window::pWM_Destroy() {}
void JEN_Window::pWM_NCDestroy() {pWIN32_RemoveThis(); flag |= JEN_WINDOW_FLAG_DESTROYED;}
void JEN_Window::pWM_MouseMove(JD_FLAG iWParam, JD_FLAG iLParam) {
    mousePos.x = JD_SHORT(iLParam & 0xffff);
    mousePos.y = JD_SHORT((iLParam >> 16) & 0xffff);
    mouseState = bool(iWParam & MK_LBUTTON);
    mouseState |= bool(iWParam & MK_MBUTTON) << 1;
    mouseState |= bool(iWParam & MK_RBUTTON) << 2;
}

void JEN_Window::pWIN32_PollMessages() {
    while (PeekMessage(&win32Message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&win32Message);
        pWIN32_Main(win32Handle, win32Message.message, win32Message.wParam, win32Message.lParam);
    }
}

JD_INDEX JEN_Window::pWIN32_PostMessage(JD_FLAG iWM_Event, JD_FLAG iWParam, JD_FLAG iLParam) {
    return (JD_INDEX)pWIN32_Main(win32Handle, (UINT)iWM_Event, (WPARAM)iWParam, (LPARAM)iLParam);
}

LRESULT CALLBACK JEN_Window::pWIN32_Main(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static JEN_Window* _this = NULL;
    if (_this == NULL && hWnd != NULL) {_this = (JEN_Window*)GetProp(hWnd, "this");}
    if (_this != NULL) {
        switch (message) {
            case WM_NULL: break;
            case WM_CREATE: break;
            case WM_DESTROY: _this->pWM_Destroy(); break;
            case WM_MOVE: _this->pWM_Move((JD_I)LOWORD(lParam), (JD_I)HIWORD(lParam)); break;
            case WM_SIZE: _this->pWM_Size((JD_I)LOWORD(lParam), (JD_I)HIWORD(lParam)); break;
            case WM_ACTIVATE: break;
            case WM_SETFOCUS: break;
            case WM_KILLFOCUS: break;
            case WM_ENABLE: break;
            case WM_SETREDRAW: break;
            case WM_SETTEXT: break;
            case WM_GETTEXT: break;
            case WM_GETTEXTLENGTH: break;
            case WM_PAINT: _this->pWM_Paint(); break;
            case WM_CLOSE: _this->pWM_Close(); return 1;
            case WM_QUERYENDSESSION: break;
            case WM_QUIT: break;
            case WM_QUERYOPEN: break;
            case WM_ERASEBKGND: _this->pWM_EraseBkgnd(); break;
            case WM_SYSCOLORCHANGE: break;
            case WM_ENDSESSION: break;
            case WM_SHOWWINDOW: break;
            case WM_WININICHANGE: break;
            case WM_DEVMODECHANGE: break;
            case WM_ACTIVATEAPP: break;
            case WM_FONTCHANGE: break;
            case WM_TIMECHANGE: break;
            case WM_CANCELMODE: break;
            case WM_SETCURSOR: break;
            case WM_MOUSEACTIVATE: break;
            case WM_CHILDACTIVATE: break;
            case WM_QUEUESYNC: break;
            case WM_GETMINMAXINFO: break;
            case WM_PAINTICON: break;
            case WM_ICONERASEBKGND: break;
            case WM_NEXTDLGCTL: break;
            case WM_SPOOLERSTATUS: break;
            case WM_DRAWITEM: break;
            case WM_MEASUREITEM: break;
            case WM_DELETEITEM: break;
            case WM_VKEYTOITEM: break;
            case WM_CHARTOITEM: break;
            case WM_SETFONT: break;
            case WM_GETFONT: break;
            case WM_SETHOTKEY: break;
            case WM_GETHOTKEY: break;
            case WM_QUERYDRAGICON: break;
            case WM_COMPAREITEM: break;
            case WM_COMPACTING: break;
            case WM_COMMNOTIFY: break;
            case WM_WINDOWPOSCHANGING: break;
            case WM_WINDOWPOSCHANGED: break;
            case WM_POWER: break;
            case WM_COPYDATA: break;
            case WM_CANCELJOURNAL: break;
            case WM_NOTIFY: break;
            case WM_INPUTLANGCHANGEREQUEST: break;
            case WM_INPUTLANGCHANGE: break;
            case WM_TCARD: break;
            case WM_HELP: break;
            case WM_USERCHANGED: break;
            case WM_NOTIFYFORMAT: break;
            case WM_CONTEXTMENU: break;
            case WM_STYLECHANGING: break;
            case WM_STYLECHANGED: break;
            case WM_DISPLAYCHANGE: break;
            case WM_GETICON: break;
            case WM_SETICON: break;
            case WM_NCCREATE: break;
            case WM_NCDESTROY: _this->pWM_NCDestroy(); break;
            case WM_NCCALCSIZE: break;
            case WM_NCHITTEST: break;
            case WM_NCPAINT: break;
            case WM_NCACTIVATE: break;
            case WM_GETDLGCODE: break;
            case WM_SYNCPAINT: break;
            case WM_NCMOUSEMOVE: break;
            case WM_NCLBUTTONDOWN: break;
            case WM_NCLBUTTONUP: break;
            case WM_NCLBUTTONDBLCLK: break;
            case WM_NCRBUTTONDOWN: break;
            case WM_NCRBUTTONUP: break;
            case WM_NCRBUTTONDBLCLK: break;
            case WM_NCMBUTTONDOWN: break;
            case WM_NCMBUTTONUP: break;
            case WM_NCMBUTTONDBLCLK: break;
            case EM_GETSEL: break;
            case EM_SETSEL: break;
            case EM_GETRECT: break;
            case EM_SETRECT: break;
            case EM_SETRECTNP: break;
            case EM_SCROLL: break;
            case EM_LINESCROLL: break;
            case EM_SCROLLCARET: break;
            case EM_GETMODIFY: break;
            case EM_SETMODIFY: break;
            case EM_GETLINECOUNT: break;
            case EM_LINEINDEX: break;
            case EM_SETHANDLE: break;
            case EM_GETHANDLE: break;
            case EM_GETTHUMB: break;
            case EM_LINELENGTH: break;
            case EM_REPLACESEL: break;
            case EM_GETLINE: break;
            case EM_LIMITTEXT: break;
            case EM_CANUNDO: break;
            case EM_UNDO: break;
            case EM_FMTLINES: break;
            case EM_LINEFROMCHAR: break;
            case EM_SETTABSTOPS: break;
            case EM_SETPASSWORDCHAR: break;
            case EM_EMPTYUNDOBUFFER: break;
            case EM_GETFIRSTVISIBLELINE: break;
            case EM_SETREADONLY: break;
            case EM_SETWORDBREAKPROC: break;
            case EM_GETWORDBREAKPROC: break;
            case EM_GETPASSWORDCHAR: break;
            case EM_SETMARGINS: break;
            case EM_GETMARGINS: break;
            case EM_GETLIMITTEXT: break;
            case EM_POSFROMCHAR: break;
            case EM_CHARFROMPOS: break;
            case SBM_SETPOS: break;
            case SBM_GETPOS: break;
            case SBM_SETRANGE: break;
            case SBM_GETRANGE: break;
            case SBM_ENABLE_ARROWS: break;
            case SBM_SETRANGEREDRAW: break;
            case SBM_SETSCROLLINFO: break;
            case SBM_GETSCROLLINFO: break;
            case BM_GETCHECK: break;
            case BM_SETCHECK: break;
            case BM_GETSTATE: break;
            case BM_SETSTATE: break;
            case BM_SETSTYLE: break;
            case BM_CLICK: break;
            case BM_GETIMAGE: break;
            case BM_SETIMAGE: break;
            case WM_KEYDOWN: break;
            case WM_KEYUP: break;
            case WM_CHAR: break;
            case WM_DEADCHAR: break;
            case WM_SYSKEYDOWN: break;
            case WM_SYSKEYUP: break;
            case WM_SYSCHAR: break;
            case WM_SYSDEADCHAR: break;
            case WM_KEYLAST: break;
            case WM_IME_STARTCOMPOSITION: break;
            case WM_IME_ENDCOMPOSITION: break;
            case WM_IME_COMPOSITION: break;
            case WM_INITDIALOG: break;
            case WM_COMMAND: break;
            case WM_SYSCOMMAND: break;
            case WM_TIMER: break;
            case WM_HSCROLL: break;
            case WM_VSCROLL: break;
            case WM_INITMENU: break;
            case WM_INITMENUPOPUP: break;
            case WM_MENUSELECT: break;
            case WM_MENUCHAR: break;
            case WM_ENTERIDLE: break;
            case WM_CTLCOLORMSGBOX: break;
            case WM_CTLCOLOREDIT: break;
            case WM_CTLCOLORLISTBOX: break;
            case WM_CTLCOLORBTN: break;
            case WM_CTLCOLORDLG: break;
            case WM_CTLCOLORSCROLLBAR: break;
            case WM_CTLCOLORSTATIC: break;
            case WM_MOUSEMOVE: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_LBUTTONDOWN: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_LBUTTONUP: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_LBUTTONDBLCLK: break;
            case WM_RBUTTONDOWN: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_RBUTTONUP: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_RBUTTONDBLCLK: break;
            case WM_MBUTTONDOWN: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_MBUTTONUP: _this->pWM_MouseMove(wParam, lParam); break;
            case WM_MBUTTONDBLCLK: break;
            case WM_MOUSEWHEEL: break;
            case WM_PARENTNOTIFY: break;
            case WM_ENTERMENULOOP: break;
            case WM_EXITMENULOOP: break;
            case WM_NEXTMENU: break;
            case WM_SIZING: break;
            case WM_CAPTURECHANGED: break;
            case WM_MOVING: break;
            case WM_POWERBROADCAST: break;
            case WM_DEVICECHANGE: break;
            case WM_MDICREATE: break;
            case WM_MDIDESTROY: break;
            case WM_MDIACTIVATE: break;
            case WM_MDIRESTORE: break;
            case WM_MDINEXT: break;
            case WM_MDIMAXIMIZE: break;
            case WM_MDITILE: break;
            case WM_MDICASCADE: break;
            case WM_MDIICONARRANGE: break;
            case WM_MDIGETACTIVE: break;
            case WM_MDISETMENU: break;
            case WM_ENTERSIZEMOVE: break;
            case WM_EXITSIZEMOVE: break;
            case WM_DROPFILES: break;
            case WM_MDIREFRESHMENU: break;
            case WM_IME_SETCONTEXT: break;
            case WM_IME_NOTIFY: break;
            case WM_IME_CONTROL: break;
            case WM_IME_COMPOSITIONFULL: break;
            case WM_IME_SELECT: break;
            case WM_IME_CHAR: break;
            case WM_IME_KEYDOWN: break;
            case WM_IME_KEYUP: break;
            case WM_MOUSEHOVER: break;
            case WM_MOUSELEAVE: break;
            case WM_CUT: break;
            case WM_COPY: break;
            case WM_PASTE: break;
            case WM_CLEAR: break;
            case WM_UNDO: break;
            case WM_RENDERFORMAT: break;
            case WM_RENDERALLFORMATS: break;
            case WM_DESTROYCLIPBOARD: break;
            case WM_DRAWCLIPBOARD: break;
            case WM_PAINTCLIPBOARD: break;
            case WM_VSCROLLCLIPBOARD: break;
            case WM_SIZECLIPBOARD: break;
            case WM_ASKCBFORMATNAME: break;
            case WM_CHANGECBCHAIN: break;
            case WM_HSCROLLCLIPBOARD: break;
            case WM_QUERYNEWPALETTE: break;
            case WM_PALETTEISCHANGING: break;
            case WM_PALETTECHANGED: break;
            case WM_HOTKEY: break;
            case WM_PRINT: break;
            case WM_PRINTCLIENT: break;
            case WM_HANDHELDFIRST: break;
            case WM_HANDHELDLAST: break;
            case WM_AFXFIRST: break;
            case WM_AFXLAST: break;
            case WM_PENWINFIRST: break;
            case WM_PENWINLAST: break;
            default: break;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


