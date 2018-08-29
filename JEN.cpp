
#include "JEN.h"

JEN::JEN(SDL_Window* iWindow, SDL_Renderer* iRenderer) {
	windowSDL = iWindow;
	rendererSDL = iRenderer;
    screenCanvas = NULL;
	JDM_EmptyPoint(&sizeScreen);
    JDM_SetRectPos(&rectScreen, JUICYENGINE_LL_ORIGIN_X, JUICYENGINE_LL_ORIGIN_Y);
    JDM_SetRectSize(&rectScreen, &sizeScreen);
    runScreenMetrics();
    tmilNow = getTicks();
    tmilFps = tmilNow;
    numFrames = 0;
    numFps = 0;
    mode = JUICYENGINE_MODE_INIT;
    fontSDL = NULL;
    fontPath = NULL;
    fontSize = 0;
}

JEN::~JEN() {
    if (fontSDL != NULL) {TTF_CloseFont(fontSDL);}
    destroyBuffer();
}

JD_TIME JEN::getTicks() {
    return SDL_GetTicks();
}
void JEN::runTimeMetrics() {
    tmilLast = tmilNow;
    tmilNow = getTicks();
    tmilDelta = tmilNow - tmilLast;
    numFrames++;
    if ((tmilNow - tmilFps) >= JUICYENGINE_LL_FPS_UPDATE) {
        numFps = ((numFrames * JUICYENGINE_LL_MS1000) / JUICYENGINE_LL_FPS_UPDATE);
        numFrames = 0;
        tmilFps = tmilNow - ((tmilNow - tmilFps) % JUICYENGINE_LL_FPS_UPDATE);
    }
}

bool JEN::LoopHandle() {
    if (!(mode & JUICYENGINE_MODE_BUFFERED)) {runTimeMetrics();}
    return runScreenMetrics();
}

bool JEN::runScreenMetrics() {
	JD_Point newSizeScreen;
	SDL_GetWindowSize(windowSDL, &(newSizeScreen.x), &(newSizeScreen.y));
	bool sizeChange = newSizeScreen != sizeScreen;
	if (sizeChange) {
		sizeScreen = newSizeScreen;
		JDM_SetRectSize(&rectScreen, &sizeScreen);
		if (screenCanvas != NULL) {
            createBuffer();
		}
	}
	return sizeChange;
}

void JEN::SetMode(JD_FLAG iMode) {
    if (iMode != mode) {
        destroyBuffer();
        mode = iMode;
    }
}

void JEN::FillScreen(JD_COLOR iColor) {
    SDL_SetRenderTarget(rendererSDL, NULL);
    SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rendererSDL, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(rendererSDL, &cacheRect);
}
void JEN::ModScreen(JD_COLOR iColor) {
    SDL_SetRenderTarget(rendererSDL, NULL);
    SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(rendererSDL, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(rendererSDL, &cacheRect);
}

JuicySprite* JEN::CreateSprite(const JD_Point* iSize, JD_COLOR* iPixeldata) {
    JuicySprite* newSprite = NULL;
    if (iSize != NULL && iPixeldata != NULL) {
        newSprite = new JuicySprite;
        newSprite->dimensions = *iSize;
        newSprite->type = JUICYSPRITE_TYPE_RAW;
        newSprite->sprite = NULL;
        newSprite->pixels = JDM_CopyPixelData(iPixeldata, iSize);
    }
    return newSprite;
}
JuicySprite* JEN::CreateSpriteText(const char* iText, const char* iFontpath, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle) {
    JuicySprite* newSprite = NULL;
    if (iText != NULL && iFontpath != NULL) {
        newSprite = new JuicySprite;
        newSprite->type = JUICYSPRITE_TYPE_RAW;
        newSprite->sprite = NULL;
        newSprite->pixels = getPixelsFromText(iText, &(newSprite->dimensions), iFontpath, iFontsize, iFontcolor, iFontstyle);
        if(newSprite->pixels == NULL) {
            delete newSprite;
            newSprite = NULL;
        }
    }
    return newSprite;
}
JuicySprite* JEN::CreateSpriteFill(const JD_Point* iSize, JD_COLOR iColor) {
    JuicySprite* newSprite = NULL;
    if (iSize != NULL) {
        JD_COLOR* iPixeldata = JDM_InitPixelData(JDM_GetArea(iSize), iColor);
        if (iPixeldata != NULL) {
            newSprite = CreateSprite(iSize, iPixeldata);
            delete[] iPixeldata;
        }
    }
    return newSprite;
}
void JEN::InflateSprite(JuicySprite* iSprite, JD_I iNum) {
    if (iSprite != NULL && iNum > 0) {
        if (iSprite->pixels != NULL) {
            JD_Point inflatedSize = iSprite->dimensions << iNum;
            JD_COLOR* inflatedPixels = new JD_COLOR[JDM_GetArea(&inflatedSize)];
            for (JD_I y = 0; y < inflatedSize.y; y++) {
                for (JD_I x = 0; x < inflatedSize.x; x++) {
                    inflatedPixels[x + y * inflatedSize.x] = iSprite->pixels[(x >> iNum) + (y >> iNum) * iSprite->dimensions.x];
                }
            }
            iSprite->dimensions = inflatedSize;
            delete[] iSprite->pixels;
            iSprite->pixels = inflatedPixels;
            resetSprite(iSprite);
        }
    }
}
void JEN::OptimizeSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        clearSprite(iSprite);
        if (iSprite->pixels != NULL) {
            switch (mode & JUICYENGINE_MODE_MASK) {
                case JUICYENGINE_MODE_SDL_TEXTURE: {
                        SDL_Surface* newSurface = NULL;
                        newSurface = createSDL_Surface(&(iSprite->dimensions), iSprite->pixels, 0x1);
                        if (newSurface != NULL) {
                            iSprite->type = JUICYSPRITE_TYPE_SDL_TEXTURE;
                            iSprite->flag |= JUICYSPRITE_FLAG_OPTIMIZED;
                            iSprite->sprite = (void*)createSDL_Texture(newSurface);
                            freeSDL_Surface(newSurface, 0x0);
                        }
                    }
                    break;
                case JUICYENGINE_MODE_SDL_SURFACE: {
                        SDL_Surface* newSurface = NULL;
                        newSurface = createSDL_Surface(&iSprite->dimensions, iSprite->pixels, 0x1);
                        if (newSurface != NULL) {
                            iSprite->type = JUICYSPRITE_TYPE_SDL_SURFACE;
                            iSprite->flag |= JUICYSPRITE_FLAG_OPTIMIZED;
                            iSprite->sprite = (void*)newSurface;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}
void JEN::checkSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        if ((iSprite->type) != (mode & JUICYENGINE_MODE_MASK) || !(iSprite->flag & JUICYSPRITE_FLAG_OPTIMIZED)) {
            OptimizeSprite(iSprite);
        }
    }
}
void JEN::resetSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        iSprite->flag &= ~JUICYSPRITE_FLAG_OPTIMIZED;
    }
}
void JEN::clearSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        if (iSprite->sprite != NULL) {
            switch ((iSprite->type)) {
                case JUICYSPRITE_TYPE_SDL_TEXTURE:
                    freeSDL_Texture((SDL_Texture*)iSprite->sprite);
                    iSprite->type = JUICYSPRITE_TYPE_RAW;
                    iSprite->flag &= ~JUICYSPRITE_FLAG_OPTIMIZED;
                    break;
                case JUICYSPRITE_TYPE_SDL_SURFACE:
                    freeSDL_Surface((SDL_Surface*)iSprite->sprite, 0x0);
                    iSprite->type = JUICYSPRITE_TYPE_RAW;
                    iSprite->flag &= ~JUICYSPRITE_FLAG_OPTIMIZED;
                    break;
                default:
                    break;
            }
            iSprite->sprite = NULL;
        }
    }
}

void JEN::FreeSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        clearSprite(iSprite);
        if (iSprite->pixels != NULL) {
            delete[] (JD_COLOR*)iSprite->pixels;
        }
        delete iSprite;
    }
}
SDL_Surface* JEN::createSDL_Surface(const JD_Point* iSize, JD_COLOR* iPixeldata) {
    return createSDL_Surface(iSize, iPixeldata, 0x0);
}
SDL_Surface* JEN::createSDL_Surface(const JD_Point* iSize, JD_COLOR* iPixeldata, JD_FLAG iFlag) {
    SDL_Surface* newSurface = NULL;
    if (iSize != NULL && iPixeldata != NULL) {
        JD_COLOR* pixelSrc = (iFlag & 0x1)  ? iPixeldata : JDM_CopyPixelData(iPixeldata, iSize);
        newSurface = SDL_CreateRGBSurfaceFrom((void*)pixelSrc, iSize->x, iSize->y, JUICYENGINE_LL_DEPTH, JUICYENGINE_LL_BYTESPP * iSize->x, JUICYENGINE_LL_MASKR, JUICYENGINE_LL_MASKG, JUICYENGINE_LL_MASKB, JUICYENGINE_LL_MASKA);
        if (newSurface) {
            SDL_SetSurfaceRLE(newSurface, 0x1);
        }
    }
    return newSurface;
}

void JEN::blitSDL_Surfaces(SDL_Surface* iSurf, SDL_Surface* oSurf) {
    blitSDL_Surfaces(iSurf, oSurf, NULL, NULL);
}
void JEN::blitSDL_Surfaces(SDL_Surface* iSurf, SDL_Surface* oSurf, const JD_Rect* iRect, const JD_Rect* oRect) {
    if (iRect != NULL && oRect != NULL) {
        if (iRect->w == oRect->w && iRect->h == oRect->h) {
            SDL_Rect iSDL_Rect = JDM_XSDL_Rect(iRect);
            SDL_Rect oSDL_Rect = JDM_XSDL_Rect(oRect);
            SDL_BlitSurface(iSurf, &iSDL_Rect, oSurf, &oSDL_Rect);
        } else {
            SDL_Rect iSDL_Rect = JDM_XSDL_Rect(iRect);
            SDL_Rect oSDL_Rect = JDM_XSDL_Rect(oRect);
            SDL_BlitScaled(iSurf, &iSDL_Rect, oSurf, &oSDL_Rect);
        }
    }
    if (iRect != NULL && oRect == NULL) {
        SDL_Rect iSDL_Rect = JDM_XSDL_Rect(iRect);
        SDL_BlitSurface(iSurf, &iSDL_Rect, oSurf, NULL);
    }
    if (iRect == NULL && oRect != NULL) {
        SDL_Rect oSDL_Rect = JDM_XSDL_Rect(oRect);
        SDL_BlitSurface(iSurf, NULL, oSurf, &oSDL_Rect);
    }
    if (iRect == NULL && oRect == NULL) {
        SDL_BlitSurface(iSurf, NULL, oSurf, NULL);
    }
}

void JEN::renderSDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
    if (iTexture != NULL) {
		SDL_SetRenderTarget(rendererSDL, oTexture);
        SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_BLEND);
        if (iRect != NULL) {
            SDL_Rect cacheRect = JDM_XSDL_Rect(iRect);
            SDL_RenderCopy(rendererSDL, iTexture, NULL, &cacheRect);
        } else {
            SDL_RenderCopy(rendererSDL, iTexture, NULL, NULL);
        }
    }
}
void JEN::renderSDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	renderSDL_Texture(iTexture, iRect, NULL);
}
void JEN::renderAndDestroySDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	renderSDL_Texture(iTexture, iRect, NULL);
	freeSDL_Texture(iTexture);
}
void JEN::renderAndDestroySDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
	renderSDL_Texture(iTexture, iRect, oTexture);
	freeSDL_Texture(iTexture);
}

void JEN::createBuffer() {
	destroyBuffer();
    switch (mode & JUICYENGINE_MODE_MASK) {
        case JUICYENGINE_MODE_SDL_TEXTURE:
            screenCanvas = (void*)SDL_CreateTexture(rendererSDL, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sizeScreen.x, sizeScreen.y);
            SDL_SetTextureBlendMode((SDL_Texture*)screenCanvas, SDL_BLENDMODE_BLEND);
            break;
        case JUICYENGINE_MODE_SDL_SURFACE: {
                JD_COLOR* canvasPixels = JDM_InitPixelData(&sizeScreen, 0x0);
                if (canvasPixels != NULL) {
                    screenCanvas = (void*)createSDL_Surface(&sizeScreen, canvasPixels, 0x1);
                }
            }
            break;
        default:
            break;
    }
}
void JEN::destroyBuffer() {
    switch (mode & JUICYENGINE_MODE_MASK) {
        case JUICYENGINE_MODE_SDL_TEXTURE:
            freeSDL_Texture((SDL_Texture*)screenCanvas);
            screenCanvas = NULL;
            break;
        case JUICYENGINE_MODE_SDL_SURFACE:
            freeSDL_Surface((SDL_Surface*)screenCanvas);
            screenCanvas = NULL;
            break;
        default:
            break;
    }
}
void JEN::clearBuffer() {
	if (screenCanvas != NULL) {
        switch (mode & JUICYENGINE_MODE_MASK) {
            case JUICYENGINE_MODE_SDL_TEXTURE:
                SDL_SetRenderTarget(rendererSDL, (SDL_Texture*)screenCanvas);
                SDL_SetRenderDrawBlendMode(rendererSDL, SDL_BLENDMODE_NONE);
                SDL_SetRenderDrawColor(rendererSDL, 0, 0, 0, 0);
                SDL_RenderFillRect(rendererSDL, NULL);
                break;
            case JUICYENGINE_MODE_SDL_SURFACE:
                SDL_LockSurface((SDL_Surface*)screenCanvas);
                for (JD_I i = 0; i < (((SDL_Surface*)screenCanvas)->w * ((SDL_Surface*)screenCanvas)->h); i++) {
                    ((JD_COLOR*)(((SDL_Surface*)screenCanvas)->pixels))[i] = 0x0;
                }
                SDL_UnlockSurface((SDL_Surface*)screenCanvas);
                break;
            default:
                break;
        }
	}
}
void JEN::Render(JuicySprite* iSprite, const JD_Point* iPos) {
    if (iSprite != NULL) {
        JD_Rect drawRect;
        JDM_SetRectPos(&drawRect, iPos);
        JDM_SetRectSize(&drawRect, &(iSprite->dimensions));
        Render(iSprite, &drawRect);
    }
}
void JEN::Render(JuicySprite* iSprite, const JD_Rect* iRect) {
    if (iSprite != NULL) {
        checkSprite(iSprite);
        if (mode & JUICYENGINE_MODE_BUFFERED) {
            if (screenCanvas == NULL) {
                createBuffer();
            }
            switch (mode & JUICYENGINE_MODE_MASK) {
                case JUICYENGINE_MODE_SDL_TEXTURE:
                    renderSDL_Texture((SDL_Texture*)(iSprite->sprite), iRect, (SDL_Texture*)screenCanvas);
                    break;
                case JUICYENGINE_MODE_SDL_SURFACE:
                    blitSDL_Surfaces((SDL_Surface*)(iSprite->sprite), (SDL_Surface*)screenCanvas, NULL, iRect);
                    break;
                default:
                    break;
            }
        } else {
            switch (mode & JUICYENGINE_MODE_MASK) {
                case JUICYENGINE_MODE_SDL_TEXTURE:
                    renderSDL_Texture((SDL_Texture*)iSprite->sprite, iRect);
                    break;
                case JUICYENGINE_MODE_SDL_SURFACE:
                    renderAndDestroySDL_Texture(createSDL_Texture((SDL_Surface*)(iSprite->sprite)), iRect);
                    break;
                default:
                    break;
            }
        }
    }
}
void JEN::Present() {
    if ((screenCanvas != NULL) && (mode & JUICYENGINE_MODE_BUFFERED)) {
        runTimeMetrics();
        switch (mode & JUICYENGINE_MODE_MASK) {
            case JUICYENGINE_MODE_SDL_TEXTURE:
                SDL_SetRenderTarget(rendererSDL, NULL);
                renderSDL_Texture((SDL_Texture*)screenCanvas, NULL);
                break;
            case JUICYENGINE_MODE_SDL_SURFACE:
                renderAndDestroySDL_Texture(createSDL_Texture((SDL_Surface*)screenCanvas), NULL);
                break;
            default:
                break;
        }
        clearBuffer();
    }
}
void JEN::PresentSprite(JuicySprite* iSprite, const JD_Point* iPos) {
    if (iSprite != NULL) {
        JD_Rect drawRect;
        JDM_SetRectPos(&drawRect, iPos);
        JDM_SetRectSize(&drawRect, &(iSprite->dimensions));
        PresentSprite(iSprite, &drawRect);
    }
}
void JEN::PresentSprite(JuicySprite* iSprite, const JD_Rect* iRect) {
    if (iSprite != NULL) {
        checkSprite(iSprite);
        switch (mode & JUICYENGINE_MODE_MASK) {
            case JUICYENGINE_MODE_SDL_TEXTURE:
                renderSDL_Texture((SDL_Texture*)iSprite->sprite, iRect);
                break;
            case JUICYENGINE_MODE_SDL_SURFACE:
                renderAndDestroySDL_Texture(createSDL_Texture((SDL_Surface*)(iSprite->sprite)), iRect);
                break;
            default:
                break;
        }
    }
}
void JEN::freeSDL_Surface(SDL_Surface* iSurface) {
    freeSDL_Surface(iSurface, 0x1);
}
void JEN::freeSDL_Surface(SDL_Surface* iSurface, JD_FLAG iFlag) {
	if (iSurface != NULL) {
        if (iFlag & 0x1) {if (iSurface->pixels != NULL) {delete[] (JD_COLOR*)(iSurface->pixels);}}
		SDL_FreeSurface(iSurface);
	}
}
void JEN::freeSDL_Texture(SDL_Texture* iTexture) {
	if (iTexture != NULL) {
		SDL_DestroyTexture(iTexture);
	}
}

JD_COLOR* JEN::getPixelsFromImage(const char* iFilepath, JD_Point* oSize) {
    return (JD_COLOR*)loadSDL_Image(iFilepath, oSize);
}
void* JEN::loadSDL_Image(const char* iFilepath, JD_Point* oSize) {
    void* pixelData = NULL;
    if (iFilepath != NULL && oSize != NULL) {
        SDL_Surface* surface = NULL;
        surface = IMG_Load(iFilepath);
        if (surface != NULL) {
            oSize->x = surface->w;
            oSize->y = surface->h;
            pixelData = (void*)JDM_CopyPixelData((JD_COLOR*)surface->pixels, (surface->w * surface->h));
            freeSDL_Surface(surface, 0x0);
        }
    }
    return pixelData;
}

JD_COLOR* JEN::getPixelsFromText(const char* iText, JD_Point* oSize, const char* iFontpath, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle) {
    JD_COLOR* pixelData = NULL;
	if (iText != NULL && oSize != NULL && iFontpath != NULL) {
		if (fontSDL == NULL || iFontpath != fontPath || iFontsize != fontSize) {
            if (fontSDL != NULL) {
                TTF_CloseFont(fontSDL);
                fontSDL = NULL;
            }
            fontSDL = TTF_OpenFont(iFontpath, iFontsize);
            fontPath = iFontpath;
            fontSize = iFontsize;
		}
        SDL_Surface* surface = NULL;
		if (fontSDL != NULL) {
            TTF_SetFontStyle(fontSDL, iFontstyle);
            SDL_Color cacheColor;
            cacheColor.r = (iFontcolor >> 24) & 0xff;
            cacheColor.g = (iFontcolor >> 16) & 0xff;
            cacheColor.b = (iFontcolor >> 8) & 0xff;
            cacheColor.a = iFontcolor & 0xff;
            surface = TTF_RenderText_Blended(fontSDL, iText, cacheColor);
            if (surface != NULL) {
                oSize->x = surface->w;
                oSize->y = surface->h;
                pixelData = JDM_CopyPixelData((JD_COLOR*)(surface->pixels), (surface->w * surface->h));
                for (JD_I i = 0; i < (surface->w * surface->h); i++) {
                    pixelData[i] = ((pixelData[i] & 0xffffff) << 8) | (pixelData[i] >> 24);
                }
                freeSDL_Surface(surface, 0x0);
            }
		}
	}
    return pixelData;
}
void JEN::BlendSprites(const JuicySprite* iSrc, JuicySprite* oDst) {
    BlendSprites(iSrc, oDst, NULL, NULL);
}
void JEN::BlendSprites(const JuicySprite* iSrc, JuicySprite* oDst, const JD_Rect* iRectSrc, const JD_Rect* iRectDst) {
    if (iSrc != NULL && oDst != NULL) {
        if (iSrc->pixels != NULL && oDst->pixels != NULL) {
            JD_Point indexShiftSrc;
            JD_Rect rectTarget;
            JD_Rect rectSrc;
            JD_Rect rectDst;
            switch ((iRectDst != NULL) | ((iRectSrc != NULL) << 1)) {
                default:
                case 0:
                    rectTarget.w = (iSrc->dimensions.x < oDst->dimensions.x) ? iSrc->dimensions.x : oDst->dimensions.x;
                    rectTarget.h = (iSrc->dimensions.y < oDst->dimensions.y) ? iSrc->dimensions.y : oDst->dimensions.y;
                    rectTarget.x = (oDst->dimensions.x - rectTarget.w) >> 1;
                    rectTarget.y = (oDst->dimensions.y - rectTarget.h) >> 1;
                    indexShiftSrc.x = rectTarget.x - ((iSrc->dimensions.x - rectTarget.w) >> 1);
                    indexShiftSrc.y = rectTarget.y - ((iSrc->dimensions.y - rectTarget.h) >> 1);
                    break;
                case 1:
                    rectDst = *iRectDst;
                    JDM_ClipRect(&rectDst, &(oDst->dimensions));
                    rectTarget.x = rectDst.x;
                    rectTarget.y = rectDst.y;
                    rectTarget.w = (rectDst.w < iSrc->dimensions.x) ? rectDst.w : iSrc->dimensions.x;
                    rectTarget.h = (rectDst.h < iSrc->dimensions.y) ? rectDst.h : iSrc->dimensions.y;
                    indexShiftSrc.x = rectTarget.x - ((iSrc->dimensions.x - rectTarget.w) >> 1);
                    indexShiftSrc.y = rectTarget.y - ((iSrc->dimensions.y - rectTarget.h) >> 1);
                    break;
                case 2:
                    rectSrc = *iRectSrc;
                    JDM_ClipRect(&rectSrc, &(iSrc->dimensions));
                    rectTarget.w = (rectSrc.w < oDst->dimensions.x) ? rectSrc.w : oDst->dimensions.x;
                    rectTarget.h = (rectSrc.h < oDst->dimensions.y) ? rectSrc.h : oDst->dimensions.y;
                    rectTarget.x = ((oDst->dimensions.x - rectTarget.w) >> 1);
                    rectTarget.y = ((oDst->dimensions.y - rectTarget.h) >> 1);
                    indexShiftSrc.x = rectTarget.x - rectSrc.x;
                    indexShiftSrc.y = rectTarget.y - rectSrc.y;
                    break;
                case 3:
                    rectSrc = *iRectSrc;
                    JDM_ClipRect(&rectSrc, &(iSrc->dimensions));
                    rectDst = *iRectDst;
                    JDM_ClipRect(&rectDst, &(oDst->dimensions));
                    rectTarget.x = rectDst.x;
                    rectTarget.y = rectDst.y;
                    rectTarget.w = (rectDst.w < rectSrc.w) ? rectDst.w : rectSrc.w;
                    rectTarget.h = (rectDst.h < rectSrc.h) ? rectDst.h : rectSrc.h;
                    indexShiftSrc.x = rectTarget.x - rectSrc.x;
                    indexShiftSrc.y = rectTarget.y - rectSrc.y;
                    break;
            }
            JD_COLOR colorSrc = 0;
            JD_COLOR colorDst = 0;
            JD_I indexOffsetDst = 0;
            for (JD_I y = rectTarget.y; y < rectTarget.y + rectTarget.h; y++) {
                for (JD_I x = rectTarget.x; x < rectTarget.x + rectTarget.w; x++) {
                    indexOffsetDst = x + y * oDst->dimensions.x;
                    colorSrc = iSrc->pixels[(x - indexShiftSrc.x) + ((y - indexShiftSrc.y) * iSrc->dimensions.x)];
                    colorDst = oDst->pixels[indexOffsetDst];
                    oDst->pixels[indexOffsetDst] = blendPixel(colorSrc, colorDst);
                }
            }
            resetSprite(oDst);
        }
    }
}

JD_COLOR JEN::blendPixel(JD_COLOR iBlend, JD_COLOR iBase) {
    JD_COLOR result = 0;
    JD_COLOR blendCh = 0;
    JD_COLOR baseCh = 0;
    JD_COLOR blendA = iBlend & 0xff;
    JD_COLOR baseA = iBase & 0xff;
    for (JD_INDEX i = 0; i < 3; i++) {
        blendCh = (iBlend >> (24 - 8 * i)) & 0xff;
        baseCh = (iBase >> (24 - 8 * i)) & 0xff;
        result |= ((blendCh * blendA + baseCh * (0xff - blendA)) / 0xff) & 0xff;
        result <<= 8;
    }
    result |= (blendA + baseA * (0xff - blendA) / 0xff) & 0xff;
    return result;
}
