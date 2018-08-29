#ifndef JUICYENGINE_H_INCLUDED
#define JUICYENGINE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JDM.h"


#define JUICYENGINE_LL_FPS_UPDATE 250
#define JUICYENGINE_LL_MS1000 1000

#define JUICYENGINE_LL_ORIGIN_X 0
#define JUICYENGINE_LL_ORIGIN_Y 0
#define JUICYENGINE_LL_DEPTH 32
#define JUICYENGINE_LL_BYTESPP 4
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

#define JUICYENGINE_MODE_MASK 0xff
#define JUICYENGINE_MODE_SDL_TEXTURE 0x1
#define JUICYENGINE_MODE_SDL_SURFACE 0x2
#define JUICYENGINE_MODE_BUFFERED 0x100
#define JUICYENGINE_MODE_INIT JUICYENGINE_MODE_SDL_TEXTURE

#define JUICYSPRITE_TYPE_RAW 0x0
#define JUICYSPRITE_TYPE_SDL_TEXTURE JUICYENGINE_MODE_SDL_TEXTURE
#define JUICYSPRITE_TYPE_SDL_SURFACE JUICYENGINE_MODE_SDL_SURFACE
#define JUICYSPRITE_FLAG_OPTIMIZED 0x1

typedef struct {
	JD_FLAG type;
	JD_FLAG flag;
	JD_Point dimensions;
	JD_COLOR* pixels;
	void* sprite;
} JuicySprite;

class JEN {
	public:
		JEN(SDL_Window* iWindow, SDL_Renderer* iTarget);
		~JEN();

		SDL_Window* GetSDL_Window() {return windowSDL;};
		SDL_Renderer* GetSDL_Renderer() {return rendererSDL;};

		JD_I GetWindowWidth() {return sizeScreen.x;};
		JD_I GetWindowHeight() {return sizeScreen.y;};
		void GetWindowSize(JD_Point* oSize) {if (oSize != NULL) {*oSize = sizeScreen;}};
		void GetWindowRect(JD_Rect* oRect) {if (oRect != NULL) {*oRect = rectScreen;}};
		const JD_Point* GetWindowSize(void) {return &sizeScreen;};
		const JD_Rect* GetWindowRect(void) {return &rectScreen;};
		bool LoopHandle();

		void SetMode(JD_FLAG iMode);
	    JD_TIME GetTicks(void) {return tmilNow;};
        JD_TIME GetDelta(void){return tmilDelta;};
	    JD_INDEX GetFPS(void) {return numFps;};

		void FillScreen(JD_COLOR iColor);
		void ModScreen(JD_COLOR iColor);

		JuicySprite* CreateSprite(const JD_Point* iSize, JD_COLOR* iPixeldata);
		JuicySprite* CreateSpriteFill(const JD_Point* iSize, JD_COLOR iColor);
		JuicySprite* CreateSpriteImage(const char* iFilepath);
		JuicySprite* CreateSpriteText(const char* iText, const char* iFontpath, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle);
		void InflateSprite(JuicySprite* iSprite, JD_I iNum);
		void OptimizeSprite(JuicySprite* iSprite);
		void BlendSprites(const JuicySprite* iSrc, JuicySprite* oDst);
		void BlendSprites(const JuicySprite* iSrc, JuicySprite* oDst, const JD_Rect* iRectSrc, const JD_Rect* iRectDst);

		void Render(JuicySprite* iSprite, const JD_Point* iPos);
		void Render(JuicySprite* iSprite, const JD_Rect* iRect);
		void Present();
		void PresentSprite(JuicySprite* iSprite, const JD_Point* iPos);
		void PresentSprite(JuicySprite* iSprite, const JD_Rect* iRect);

		void FreeSprite(JuicySprite* iSprite);
	private:
		JD_Point sizeScreen;
		JD_Rect rectScreen;
		JD_FLAG mode;

		JD_TIME getTicks();
	    JD_TIME tmilNow;
	    JD_TIME tmilLast;
	    JD_TIME tmilDelta;
	    JD_TIME tmilFps;
	    JD_INDEX numFrames;
	    JD_INDEX numFps;

	    void runTimeMetrics();
		bool runScreenMetrics();

		void createBuffer();
		void clearBuffer();
		void destroyBuffer();
		void* screenCanvas;

	    SDL_Window* windowSDL;
		SDL_Renderer* rendererSDL;

		TTF_Font* fontSDL;
		const char* fontPath;
		JD_I fontSize;

		JD_COLOR* getPixelsFromImage(const char* iFilepath, JD_Point* oSize);
		JD_COLOR* getPixelsFromText(const char* iText, JD_Point* oSize, const char* iFontpath, JD_I iFontsize, JD_COLOR iFontcolor, JD_FLAG iFontstyle);
		JD_COLOR blendPixel(JD_COLOR iPixel1, JD_COLOR iPixel2);
		void checkSprite(JuicySprite* iSprite);
		void resetSprite(JuicySprite* iSprite);
		void clearSprite(JuicySprite* iSprite);
	    void* loadSDL_Image(const char* iFilepath, JD_Point* oSize);

		SDL_Surface* createSDL_Surface(const JD_Point* iSize, JD_COLOR* iPixeldata);
		SDL_Surface* createSDL_Surface(const JD_Point* iSize, JD_COLOR* iPixeldata, JD_FLAG iFlag);
		SDL_Texture* createSDL_Texture(SDL_Surface* iSurface) {if (iSurface != NULL) {return SDL_CreateTextureFromSurface(rendererSDL, iSurface);} else {return NULL;}};
		void freeSDL_Surface(SDL_Surface* iSurface);
		void freeSDL_Surface(SDL_Surface* iSurface, JD_FLAG iFlag);
		void freeSDL_Texture(SDL_Texture* iTexture);
        void blitSDL_Surfaces(SDL_Surface* iSurf, SDL_Surface* oSurf);
        void blitSDL_Surfaces(SDL_Surface* iSurf, SDL_Surface* oSurf, const JD_Rect* iRect, const JD_Rect* oRect);
		void renderSDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect);
		void renderSDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTarget);
		void renderAndDestroySDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect);
		void renderAndDestroySDL_Texture(SDL_Texture* iTexture, const JD_Rect* iRect, SDL_Texture* oTarget);

};

#endif // JUICYENGINE_H_INCLUDED
