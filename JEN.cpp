
#include "JEN.h"

JEN::JEN(SDL_Window* iWindow, SDL_Renderer* iRenderer) {
	window = iWindow;
	renderer = iRenderer;
    JDM_SetRectPos(&rectScreen, JUICYENGINE_LL_ORIGIN_X, JUICYENGINE_LL_ORIGIN_Y);
    runScreenMetrics();
	textureCanvas = NULL;
	surfaceCanvas = NULL;
    tmilNow = GetTicks();
    numFps = 0;
}

JEN::~JEN() {
	FreeSurface(surfaceCanvas);
	FreeTexture(textureCanvas);
}

JD_TIME JEN::getTicks() {
    return SDL_GetTicks();
}

bool JEN::LoopHandle() {
    static JD_TIME tmilLast = tmilNow;
    tmilLast = tmilNow;
    tmilNow = getTicks();
    tmilDelta = tmilNow - tmilLast;

    static JD_TIME tmilFps = tmilNow;
    static JD_INDEX nFrame = 0;
    nFrame++;
    if ((tmilNow - tmilFps) >= JUICYENGINE_LL_FPS_UPDATE) {
        numFps = ((nFrame * JUICYENGINE_LL_MS1000) / JUICYENGINE_LL_FPS_UPDATE);
        nFrame = 0;
        tmilFps = tmilNow - ((tmilNow - tmilFps) % JUICYENGINE_LL_FPS_UPDATE);
    }

    return runScreenMetrics();
}

bool JEN::runScreenMetrics() {
	JD_Point newSizeScreen;
	SDL_GetWindowSize(window, &(newSizeScreen.x), &(newSizeScreen.y));
	bool sizeChange = newSizeScreen != sizeScreen;
	if (sizeChange) {
		sizeScreen = newSizeScreen;
		JDM_SetRectSize(&rectScreen, &sizeScreen);
	}
	return sizeChange;
}

void JEN::FillScreen(JD_COLOR iColor) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(renderer, &cacheRect);
}
void JEN::ModScreen(JD_COLOR iColor) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(renderer, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(renderer, &cacheRect);
}


SDL_Surface* JEN::CreateSurfaceFill(const JD_Point* iSize, JD_COLOR iColor) {
	SDL_Surface* newSurface = NULL;
	if (iSize != NULL) {
		newSurface = SDL_CreateRGBSurface(0, iSize->x, iSize->y, JUICYENGINE_LL_DEPTH, JUICYENGINE_LL_MASKR, JUICYENGINE_LL_MASKG, JUICYENGINE_LL_MASKB, JUICYENGINE_LL_MASKA);
		SDL_FillRect(newSurface, NULL, iColor);
	}
    return newSurface;
}
SDL_Surface* JEN::CreateSurfaceIMG(const char* iFilepath) {
    return CreateSurfaceIMG(iFilepath, NULL);
}
SDL_Surface* JEN::CreateSurfaceIMG(const char* iFilepath, const JD_Rect* iRect) {
    SDL_Surface* surface = NULL;
	if (iFilepath != NULL) {
		if (iRect != NULL) {
			SDL_Surface* cache = IMG_Load(iFilepath);
			if (cache != NULL) {
				SDL_Rect cacheRect = JDM_XSDL_Rect(iRect);
				JD_Point cropSize;
				JDM_GetRectSize(iRect, &cropSize);
				surface = CreateSurfaceFill(&cropSize, 0x0);
				SDL_BlitScaled(cache, NULL, surface, &cacheRect);
				SDL_FreeSurface(cache);
			} else {
				surface = NULL;
			}
		} else {
			surface = IMG_Load(iFilepath);
		}
	}
    return surface;
}
SDL_Surface* JEN::CreateSurfaceTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor) {
    SDL_Surface* surface = NULL;
	if (iText != NULL && iFont != NULL) {
		SDL_Color cacheColor;
		cacheColor.r = iColor >> 24;
		cacheColor.g = iColor >> 16;
		cacheColor.b = iColor >> 8;
		cacheColor.a = iColor;
		surface = TTF_RenderText_Blended(iFont, iText, cacheColor);
		if (oDimensions != NULL) {TTF_SizeText(iFont, iText, &(oDimensions->x), &(oDimensions->y));}
	}
    return surface;
}
SDL_Texture* JEN::CreateTextureFill(const JD_Point* iSize, JD_COLOR iColor) {
	return CreateTextureAndDestroySurface(CreateSurfaceFill(iSize, iColor));
}
SDL_Texture* JEN::CreateTextureIMG(const char* iFilepath) {
    return CreateTextureIMG(iFilepath, NULL);
}
SDL_Texture* JEN::CreateTextureIMG(const char* iFilepath, const JD_Rect* iRect) {
	return CreateTextureAndDestroySurface(CreateSurfaceIMG(iFilepath, iRect));
}
SDL_Texture* JEN::CreateTextureTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor) {
	return CreateTextureAndDestroySurface(CreateSurfaceTXT(iText, oDimensions, iFont, iColor));
}

void JEN::FillSurface(SDL_Surface* iSurface, JD_COLOR iColor) {
    FillSurface(iSurface, iColor, NULL);
}
void JEN::FillSurface(SDL_Surface* iSurface, JD_COLOR iColor, const JD_Rect* iRect) {
    if (iRect != NULL) {
        SDL_Rect rect = JDM_XSDL_Rect(iRect);
        SDL_FillRect(iSurface, &rect, iColor);
    } else {
        SDL_FillRect(iSurface, NULL, iColor);
    }
}

void JEN::BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf) {
    BlitSurfaces(iSurf, oSurf, NULL, NULL);
}
void JEN::BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf, const JD_Rect* iRect, const JD_Rect* oRect) {
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

void JEN::RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
    if (iTexture != NULL) {
		if (oTexture != NULL) {SDL_SetRenderTarget(renderer, oTexture);}
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        if (iRect != NULL) {
            SDL_Rect cacheRect = JDM_XSDL_Rect(iRect);
            SDL_RenderCopy(renderer, iTexture, NULL, &cacheRect);
        } else {
            SDL_RenderCopy(renderer, iTexture, NULL, NULL);
        }
    }
}
void JEN::RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, NULL);
}
void JEN::RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, NULL);
	FreeTexture(iTexture);
}
void JEN::RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
	RenderTexture(iTexture, iRect, oTexture);
	FreeTexture(iTexture);
}
void JEN::RenderVirtual(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, textureCanvas);
}


void JEN::createVirtual() {
	destroyVirtual();
    textureCanvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sizeScreen.x, sizeScreen.y);
    SDL_SetTextureBlendMode(textureCanvas, SDL_BLENDMODE_BLEND);
}
void JEN::destroyVirtual() {
	FreeTexture(textureCanvas);
}
void JEN::clearVirtual() {
	if (textureCanvas != NULL) {
        SDL_SetRenderTarget(renderer, textureCanvas);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, NULL);
	}
}
void JEN::PresentVirtual() {
	RenderTexture(textureCanvas, NULL);
	clearVirtual();
}

void JEN::FreeSurface(SDL_Surface* iSurface) {
	if (iSurface != NULL) {
		SDL_FreeSurface(iSurface);
		iSurface = NULL;
	}
}
void JEN::FreeTexture(SDL_Texture* iTexture) {
	if (iTexture != NULL) {
		SDL_DestroyTexture(iTexture);
		iTexture = NULL;
	}
}
