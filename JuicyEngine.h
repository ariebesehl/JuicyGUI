#ifndef JUICYENGINE_H_INCLUDED
#define JUICYENGINE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JDM.h"


#define JUICYENGINE_LL_DEPTH 32
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	#define JUICYENGINE_LL_MASKR 0xff000000
	#define JUICYENGINE_LL_MASKG 0xff0000
	#define JUICYENGINE_LL_MASKB 0xff00
	#define JUICYENGINE_LL_MASKA 0xff
#else
    #define JUICYENGINE_LL_MASKR 0xff
    #define JUICYENGINE_LL_MASKG 0xff00
    #define JUICYENGINE_LL_MASKB 0xff0000
    #define JUICYENGINE_LL_MASKA 0xff000000
#endif // SDL_BYTEORDER == SDL_LIL_ENDIAN

#define JUICYENGINE_FLAG_SURFACE 0x1

/*
typedef struct {
	void* sprite;
	JD_FLAG flag;
	JD_Point dimensions;
} JuicyEngine_Sprite;
*/

class JuicyEngine {
	public:
		JuicyEngine(SDL_Window* iWindow, SDL_Renderer* iTarget);
		~JuicyEngine();

		void GetWindowSize(JD_Point* oSize) {if (oSize != NULL) {*oSize = sizeScreen;}};
		void GetWindowRect(JD_Rect* oRect) {if (oRect != NULL) {*oRect = rectScreen;}};
		bool LoopHandle();
		//void Draw();

		void FillScreen(JD_COLOR iColor);
		void ModScreen(JD_COLOR iColor);

		SDL_Surface* CreateSurfaceFill(const JD_Point* iSize, JD_COLOR iColor);
		SDL_Surface* CreateSurfaceIMG(const char* iFilepath);
		SDL_Surface* CreateSurfaceIMG(const char* iFilepath, const JD_Rect* iSize);
		SDL_Surface* CreateSurfaceTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor);
		SDL_Texture* CreateTexture(SDL_Surface* iSurface) {if (iSurface != NULL) {return SDL_CreateTextureFromSurface(renderer, iSurface);} else {return NULL;}};
		SDL_Texture* CreateTextureAndDestroySurface(SDL_Surface* iSurface) {if (iSurface != NULL) {SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, iSurface); FreeSurface(iSurface); return texture;} else {return NULL;}};
		SDL_Texture* CreateTextureFill(const JD_Point* iSize, JD_COLOR iColor);
		SDL_Texture* CreateTextureIMG(const char* iFilepath);
		SDL_Texture* CreateTextureIMG(const char* iFilepath, const JD_Rect* iSize);
		SDL_Texture* CreateTextureTXT(const char* iText, JD_Point* oDimensions, TTF_Font* iFont, JD_COLOR iColor);


        void BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf);
        void BlitSurfaces(SDL_Surface* iSurf, SDL_Surface* oSurf, const JD_Rect* iRect, const JD_Rect* oRect);

		void RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect);
		void RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect);
		void RenderTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTarget);
		void RenderAndDestroyTexture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTarget);
		void RenderVirtual(SDL_Texture* iTexture, const JD_Rect* iRect);

		void PresentVirtual();

		void FreeSurface(SDL_Surface* iSurface);
		void FreeTexture(SDL_Texture* iTexture);
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		JD_Point sizeScreen;
		JD_Rect rectScreen;
		void createVirtual();
		void clearVirtual();
		void destroyVirtual();
		SDL_Texture* textureCanvas;
		SDL_Surface* surfaceCanvas;
};

#endif // JUICYENGINE_H_INCLUDED
