
#include "JEN.h"

JEN::JEN(SDL_Window* iWindow, SDL_Renderer* iRenderer) {
	windowSDL = iWindow;
	rendererSDL = iRenderer;
    JDM_SetRectPos(&rectScreen, JUICYENGINE_LL_ORIGIN_X, JUICYENGINE_LL_ORIGIN_Y);
    runScreenMetrics();
    screenCanvas = NULL;
    tmilNow = GetTicks();
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

bool JEN::LoopHandle() {
    tmilLast = tmilNow;
    tmilNow = getTicks();
    tmilDelta = tmilNow - tmilLast;
    numFrames++;
    if ((tmilNow - tmilFps) >= JUICYENGINE_LL_FPS_UPDATE) {
        numFps = ((numFrames * JUICYENGINE_LL_MS1000) / JUICYENGINE_LL_FPS_UPDATE);
        numFrames = 0;
        tmilFps = tmilNow - ((tmilNow - tmilFps) % JUICYENGINE_LL_FPS_UPDATE);
    }

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
        newSprite->flag = JUICYSPRITE_TYPE_RAW;
        newSprite->sprite = (void*)JDM_CopyPixelData(iPixeldata, iSize);
    }
    return newSprite;
}
JuicySprite* JEN::CreateSpriteFill(const JD_Point* iSize, JD_COLOR iColor) {
    JuicySprite* newSprite = NULL;
    if (iSize != NULL) {
        JD_COLOR* iPixeldata = JDM_InitPixelData(JDM_GetArea(iSize), iColor);
        newSprite = CreateSprite(iSize, iPixeldata);
        if (iPixeldata != NULL) {delete[] iPixeldata;}
    }
    return newSprite;
}
JuicySprite* JEN::CreateSpriteText(const char* iText, const char* iFontpath, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle) {
    JuicySprite* newSprite = NULL;
    if (iText != NULL && iFontpath != NULL) {
        newSprite = new JuicySprite;
        newSprite->flag = JUICYSPRITE_TYPE_RAW;
        newSprite->sprite = (void*)getPixelsFromText(iText, &(newSprite->dimensions), iFontpath, iFontsize, iFontcolor, iFontstyle);
        if(newSprite->sprite == NULL) {
            delete newSprite;
            newSprite = NULL;
        }
    }
    return newSprite;
}
void JEN::RasterizeSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        switch (iSprite->flag & JUICYSPRITE_TYPE_MASK) {
            case JUICYSPRITE_TYPE_RAW:
                switch (mode & JUICYENGINE_MODE_MASK) {
                    case JUICYENGINE_MODE_SDL_TEXTURE: {
                            SDL_Surface* newSurface = NULL;
                            newSurface = createSDL_Surface(&(iSprite->dimensions), (JD_COLOR*)iSprite->sprite);
                            if (newSurface != NULL) {
                                delete[] (JD_COLOR*)(iSprite->sprite);
                                iSprite->flag = JUICYSPRITE_TYPE_SDL_TEXTURE;
                                iSprite->sprite = (void*)createSDL_Texture(newSurface);
                                freeSDL_Surface(newSurface);
                            }
                        }
                        break;
                    case JUICYENGINE_MODE_SDL_SURFACE: {
                            SDL_Surface* newSurface = NULL;
                            newSurface = createSDL_Surface(&iSprite->dimensions, (JD_COLOR*)iSprite->sprite);
                            if (newSurface != NULL) {
                                delete[] (JD_COLOR*)iSprite->sprite;
                                iSprite->flag = JUICYSPRITE_TYPE_SDL_SURFACE;
                                iSprite->sprite = (void*)newSurface;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;
            case JUICYSPRITE_TYPE_SDL_SURFACE:
                switch (mode & JUICYENGINE_MODE_MASK) {
                    case JUICYENGINE_MODE_SDL_TEXTURE: {
                            SDL_Texture* newTexture = createSDL_Texture((SDL_Surface*)iSprite->sprite);
                            if (newTexture != NULL) {
                                freeSDL_Surface((SDL_Surface*)iSprite->sprite);
                                iSprite->flag = JUICYSPRITE_TYPE_SDL_TEXTURE;
                                iSprite->sprite = (void*)newTexture;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}
SDL_Surface* JEN::createSDL_Surface(const JD_Point* iSize, JD_COLOR* iPixeldata) {
    SDL_Surface* newSurface = NULL;
    if (iSize != NULL && iPixeldata != NULL) {
        newSurface = SDL_CreateRGBSurfaceFrom((void*)JDM_CopyPixelData(iPixeldata, iSize), iSize->x, iSize->y, JUICYENGINE_LL_DEPTH, JUICYENGINE_LL_BYTESPP * iSize->x, JUICYENGINE_LL_MASKR, JUICYENGINE_LL_MASKG, JUICYENGINE_LL_MASKB, JUICYENGINE_LL_MASKA);
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
                    screenCanvas = (void*)createSDL_Surface(&sizeScreen, canvasPixels);
                    delete[] canvasPixels;
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
        if (iSprite->flag != (mode & JUICYENGINE_MODE_MASK)) {
            RasterizeSprite(iSprite);
        }
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

void JEN::FreeSprite(JuicySprite* iSprite) {
    if (iSprite != NULL) {
        switch ((iSprite->flag) & JUICYSPRITE_TYPE_MASK) {
            case JUICYSPRITE_TYPE_RAW:
                if (iSprite->sprite != NULL) {
                    delete[] (JD_COLOR*)iSprite->sprite;
                }
                break;
            case JUICYSPRITE_TYPE_SDL_TEXTURE:
                freeSDL_Texture((SDL_Texture*)iSprite->sprite);
                break;
            case JUICYSPRITE_TYPE_SDL_SURFACE:
                freeSDL_Surface((SDL_Surface*)iSprite->sprite);
                break;
            default:
                break;
        }
        delete iSprite;
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
        if (iSrc->flag == JUICYSPRITE_TYPE_RAW && oDst->flag == JUICYSPRITE_TYPE_RAW) {
            JD_Rect offsetSrc;
            JD_Rect offsetDst;
            JDM_EmptyRect(&offsetSrc);
            JDM_EmptyRect(&offsetDst);
            if (iRectSrc != NULL) {
                offsetSrc.x = (iRectSrc->x <= iSrc->dimensions.x) ? iRectSrc->x : iSrc->dimensions.x;
                offsetSrc.y = (iRectSrc->y <= iSrc->dimensions.y) ? iRectSrc->y : iSrc->dimensions.y;
                offsetSrc.w = ((offsetSrc.x + iRectSrc->w) <= iSrc->dimensions.x) ? iRectSrc->w : (iSrc->dimensions.x - offsetSrc.x);
                offsetSrc.h = ((offsetSrc.y + iRectSrc->h) <= iSrc->dimensions.y) ? iRectSrc->h : (iSrc->dimensions.y - offsetSrc.y);
            } else {
                JD_Point offsetSrcPos = (oDst->dimensions - iSrc->dimensions) / 2;
                JDM_SetRectPos(&offsetSrc, &offsetSrcPos);
                JDM_SetRectSize(&offsetSrc, &(iSrc->dimensions));
            }
            if (iRectDst != NULL) {
                offsetDst.x = (iRectDst->x <= oDst->dimensions.x) ? iRectDst->x : oDst->dimensions.x;
                offsetDst.y = (iRectDst->y <= oDst->dimensions.y) ? iRectDst->y : oDst->dimensions.y;
                offsetDst.w = ((offsetDst.x + iRectDst->w) <= oDst->dimensions.x) ? iRectDst->w : (oDst->dimensions.x - offsetDst.x);
                offsetDst.h = ((offsetDst.y + iRectDst->h) <= oDst->dimensions.y) ? iRectDst->h : (oDst->dimensions.y - offsetDst.y);
            } else {
                JDM_SetRectSize(&offsetDst, &(oDst->dimensions));
            }
            offsetSrc.x += offsetDst.x;
            offsetSrc.y += offsetDst.y;
            JD_COLOR* pixelSrc = ((JD_COLOR*)(iSrc->sprite));
            JD_COLOR* pixelDst = ((JD_COLOR*)(oDst->sprite));
            for (JD_I y = offsetDst.y; y < offsetDst.y + offsetDst.h; y++) {
                for (JD_I x = offsetDst.x; x < offsetDst.x + offsetDst.w; x++) {
                    if ((x >= offsetSrc.x) && (x < offsetSrc.x + offsetSrc.w) && (y >= offsetSrc.y) && (y < offsetSrc.y + offsetSrc.h)) {
                        JD_COLOR colorSrc = pixelSrc[(x - offsetSrc.x) + ((y - offsetSrc.y) * iSrc->dimensions.x)];
                        JD_COLOR colorDst = pixelDst[x + y * oDst->dimensions.x];
                        pixelDst[x + y * oDst->dimensions.x] = blendPixel(colorSrc, colorDst);
                    }
                }
            }
        }
    }
}

JD_COLOR JEN::blendPixel(JD_COLOR iBlend, JD_COLOR iBase) {
    JD_COLOR result = 0;
    JD_COLOR blendA = iBlend & 0xff;
    JD_COLOR baseA = iBase & 0xff;
    for (JD_INDEX i = 0; i < 3; i++) {
        JD_COLOR blendCh = (iBlend >> (24 - 8 * i)) & 0xff;
        JD_COLOR baseCh = (iBase >> (24 - 8 * i)) & 0xff;
        result |= ((blendCh * blendA + baseCh * (0xff - blendA)) / 0xff) & 0xff;
        result <<= 8;
    }
    result |= (blendA + baseA * (0xff - blendA) / 0xff) & 0xff;
    return result;
}
