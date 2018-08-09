
#include "JuicyEngine.h"

JuicyEngine::JuicyEngine(SDL_Window* iWindow, SDL_Renderer* iRenderer) {
	window = iWindow;
	renderer = iRenderer;
	JDM_EmptyPoint(&sizeScreen);
	JDM_EmptyRect(&rectScreen);
	textureCanvas = NULL;
	surfaceCanvas = NULL;
}

JuicyEngine::~JuicyEngine() {
	FreeSurface(surfaceCanvas);
	FreeTexture(textureCanvas);
}

bool JuicyEngine::LoopHandle() {
	JD_Point newSizeScreen;
	SDL_GetWindowSize(window, &(newSizeScreen.x), &(newSizeScreen.y));
	bool sizeChange = newSizeScreen != sizeScreen;
	if (sizeChange) {
		sizeScreen = newSizeScreen;
		JDM_SetRectSize(&rectScreen, &sizeScreen);
	}
	return sizeChange;
}

void JuicyEngine::FillScreen(JD_COLOR iColor) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(renderer, &cacheRect);
}
void JuicyEngine::ModScreen(JD_COLOR iColor) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(renderer, (iColor >> 24) & 0xff, (iColor >> 16) & 0xff, (iColor >> 8) & 0xff, iColor & 0xff);
    SDL_Rect cacheRect = JDM_XSDL_Rect(&rectScreen);
    SDL_RenderFillRect(renderer, &cacheRect);
}


SDL_Surface* JuicyEngine::CreateSurfaceFill(const JD_Point* iSize, JD_COLOR iColor) {
	SDL_Surface* newSurface = NULL;
	if (iSize != NULL) {
		newSurface = SDL_CreateRGBSurface(0, iSize->x, iSize->y, JUICYENGINE_LL_DEPTH, JUICYENGINE_LL_MASKR, JUICYENGINE_LL_MASKG, JUICYENGINE_LL_MASKB, JUICYENGINE_LL_MASKA);
		SDL_FillRect(newSurface, NULL, iColor);
	}
    return newSurface;
}
SDL_Surface* JuicyEngine::CreateSurfaceIMG(const char* iFilepath) {
    return CreateSurfaceIMG(iFilepath, NULL);
}
SDL_Surface* JuicyEngine::CreateSurfaceIMG(const char* iFilepath, const JD_Rect* iRect) {
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
SDL_Surface* JuicyEngine::CreateSurfaceTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor) {
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
SDL_Texture* JuicyEngine::CreateTextureFill(const JD_Point* iSize, JD_COLOR iColor) {
	return CreateTextureAndDestroySurface(CreateSurfaceFill(iSize, iColor));
}
SDL_Texture* JuicyEngine::CreateTextureIMG(const char* iFilepath) {
    return CreateTextureIMG(iFilepath, NULL);
}
SDL_Texture* JuicyEngine::CreateTextureIMG(const char* iFilepath, const JD_Rect* iRect) {
	return CreateTextureAndDestroySurface(CreateSurfaceIMG(iFilepath, iRect));
}
SDL_Texture* JuicyEngine::CreateTextureTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor) {
	return CreateTextureAndDestroySurface(CreateSurfaceTXT(iText, oDimensions, iFont, iColor));
}


void JuicyEngine::BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf) {
    BlitSurfaces(iSurf, oSurf, NULL, NULL);
}
void JuicyEngine::BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf, const JD_Rect* iRect, const JD_Rect* oRect) {
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

void JuicyEngine::RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
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
void JuicyEngine::RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, NULL);
}
void JuicyEngine::RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, NULL);
	FreeTexture(iTexture);
}
void JuicyEngine::RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTexture) {
	RenderTexture(iTexture, iRect, oTexture);
	FreeTexture(iTexture);
}
void JuicyEngine::RenderVirtual(SDL_Texture* iTexture, const JD_Rect* iRect) {
	RenderTexture(iTexture, iRect, textureCanvas);
}


void JuicyEngine::createVirtual() {
	destroyVirtual();
    textureCanvas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sizeScreen.x, sizeScreen.y);
    SDL_SetTextureBlendMode(textureCanvas, SDL_BLENDMODE_BLEND);
}
void JuicyEngine::destroyVirtual() {
	FreeTexture(textureCanvas);
}
void JuicyEngine::clearVirtual() {
	if (textureCanvas != NULL) {
        SDL_SetRenderTarget(renderer, textureCanvas);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, NULL);
	}
}
void JuicyEngine::PresentVirtual() {
	RenderTexture(textureCanvas, NULL);
	clearVirtual();
}

void JuicyEngine::FreeSurface(SDL_Surface* iSurface) {
	if (iSurface != NULL) {
		SDL_FreeSurface(iSurface);
		iSurface = NULL;
	}
}
void JuicyEngine::FreeTexture(SDL_Texture* iTexture) {
	if (iTexture != NULL) {
		SDL_DestroyTexture(iTexture);
		iTexture = NULL;
	}
}
