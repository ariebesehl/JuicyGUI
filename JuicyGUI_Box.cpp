
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"

JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iBorderColor) {
    element.setCredentials(iHostUI, this, iID, JUICYGUI_TYPE_ID_BOX);
    element.setRect(iDimensions);
    properties.color = iColor;
    properties.borderColor = iBorderColor;
    properties.borderSize = JUICYGUI_BOX_BORDER_SIZE;
	textureEngine = new JSPR(element.getEngine());
    createTextures();
}

JuicyGUI_Box::~JuicyGUI_Box() {
	if (textureEngine != NULL) delete textureEngine;
}

void JuicyGUI_Box::createTextures() {
    if (textureEngine != NULL) {
        JD_Point cacheDimensions;
        element.getSize(&cacheDimensions);
        SDL_Surface* cacheSurface = element.getEngine()->CreateSurfaceFill(&cacheDimensions, properties.color);
        SDL_Surface* cacheBorder = element.getEngine()->CreateSurfaceFill(&cacheDimensions, properties.borderColor);
        JD_Rect drawRect;
        drawRect.x = 0;
        drawRect.y = 0;
        drawRect.h = properties.borderSize;
        drawRect.w = cacheDimensions.x;
        element.getEngine()->BlitSurfaces(cacheBorder, cacheSurface, &drawRect, &drawRect);
        drawRect.y = cacheDimensions.y - properties.borderSize;
        element.getEngine()->BlitSurfaces(cacheBorder, cacheSurface, &drawRect, &drawRect);
        drawRect.y = 0;
        drawRect.h = cacheDimensions.y;
        drawRect.w = properties.borderSize;
        element.getEngine()->BlitSurfaces(cacheBorder, cacheSurface, &drawRect, &drawRect);
        drawRect.x = cacheDimensions.x - properties.borderSize;
        element.getEngine()->BlitSurfaces(cacheBorder, cacheSurface, &drawRect, &drawRect);
        textureEngine->AddTexture(element.getEngine()->CreateTexture(cacheSurface));
        SDL_FreeSurface(cacheBorder);
        SDL_FreeSurface(cacheSurface);
        element.SetTextureEngine(textureEngine);
    }
}

void JuicyGUI_Box::resetTextures() {
	textureEngine->Reset();
    createTextures();
}

