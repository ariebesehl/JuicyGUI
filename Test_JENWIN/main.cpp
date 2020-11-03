
#include "main.h"
#include <iostream>

int main() {
    JD_TIME tNow = 0;
    JD_TIME tDraw = 0;

    JD_Rect windowRect;
    JD_Point windowSize;
    JD_Rect windowRectSize1;
    JD_Rect windowRectSize2;
    JD_Rect windowRectSize3;
    bool flip = true;
    JDM_SetRect(&windowRect, 200, 200, 640, 480);
    JDM_GetRectSize(&windowRect, &windowSize);
    windowSize /= 2;
    JDM_SetRectPos(&windowRectSize1, 320, 240);
    JDM_SetRectPos(&windowRectSize2, 160, 120);
    JDM_SetRectSize(&windowRectSize1, &windowSize);
    JDM_SetRectSize(&windowRectSize2, &windowSize);

    JEN_Window* window = new JEN_Window("Test Juicy Engine WIN32", &windowRect);
    window->SetBackgroundColor(0xbf3f7fff);
    window->SetImportPalette(JEN_WINDOW_PALETTE_ARGB);
    window->SetExportPalette(JEN_WINDOW_PALETTE_ARGB);
    JD_COLOR* pixels1 = GetPixelData(&windowRectSize1, 0);
    JD_COLOR* pixels2 = GetPixelData(&windowRectSize2, 1);
    JD_Point imageDim;
    JD_COLOR* pixels3 = window->LoadBitmapImageApplyingAlpha("image.bmp", &imageDim, 0x7f);
    JDM_SetRectPos(&windowRectSize3, 25, 25);
    JDM_SetRectSize(&windowRectSize3, &imageDim);
    JD_Rect textRect;
    JDM_EmptyRect(&textRect);
    JD_Point textSize;
    JD_COLOR* pixels4 = window->GetPixelsFromText("Testing testing testing", &textSize, 72, 0x3fff5bcf, 0x0);
    JDM_SetRectSize(&textRect, &textSize);
    while (window->IsAlive()) {
        tNow = window->GetTicks();
        window->PollMessages();
        if (tNow - tDraw > 32){
            tDraw = tNow;
            window->Clear();
            window->DrawBackground();
            window->DrawPixels(pixels1, &windowSize, flip ? &windowRectSize1 : &windowRectSize2);
            window->DrawPixels(pixels2, &windowSize, flip ? &windowRectSize2 : &windowRectSize1);
            window->DrawPixels(pixels3, &imageDim, &windowRectSize3);
            window->DrawPixels(pixels4, &textSize, &textRect);
            window->SystemTXT(flip ? "Hello" : "World!", &windowRectSize3);
            window->Present();
            if (window->IsClosing()) {window->Kill();}
            flip = !flip;
        }
    }
    delete pixels1;
    delete pixels2;
    delete pixels3;
    delete pixels4;
    delete window;
    return 0;
}


JD_COLOR* GetPixelData(const JD_Rect* iRect, JD_INDEX iVariation) {
    JD_COLOR* pixelData = NULL;
    if (iRect != NULL) {
        JD_INDEX length = JDM_GetArea(iRect);
        pixelData = (JD_COLOR*)JDM_InitData(length, 0x0);
        switch (iVariation) {
            case 1: for (JD_INDEX i = 0; i < length; i++) {pixelData[i] = (((i % 9) * 0x1f) << 16) | (((i % 5) * 0x3f) << 8) | (((i % 3) * 0x7f)) | 0x7f000000;} break;
            case 0: default: for (JD_INDEX i = 0; i < length; i++) {pixelData[i] = (0xff << ((i % 3) * 8)) | 0x7f000000;} break;
        }
    }
    return pixelData;
}

