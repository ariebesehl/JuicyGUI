#include "JuicyGUI_Element.h"
#include "JuicyGUI_Box.h"
#include "JuicyGUI_Definitions.h"

JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JuicyGUI_ID iID, SDL_Rect* iDimensions, JuicyGUI_Color iColor, JuicyGUI_Color iBorderColor) {
    element.setCredentials(iHostUI, this, iID, JUICYGUI_TYPE_ID_BOX);
    element.setRect(iDimensions);
    properties.color = iColor;
    properties.borderColor = iBorderColor;
    properties.borderSize = JUICYGUI_BOX_BORDER_SIZE;
    initTextures();
    createTextures();
}

JuicyGUI_Box::~JuicyGUI_Box() {
    destroyTextures();
}

void JuicyGUI_Box::draw() {
    if ( element.getFlag() & JUICYGUI_ELEMENTFLAG_SHOW) {
        element.getHost()->RenderTexture(_texture, element.getRect());
    }
}

void JuicyGUI_Box::initTextures() {
    _texture = NULL;
}


void JuicyGUI_Box::createTextures() {
    SDL_Point cacheDimensions;
    element.getSize(&cacheDimensions);
    SDL_Surface* cacheBorderSurface0 = element.getHost()->CreateSurfaceFill(&cacheDimensions, properties.borderColor);
    cacheDimensions.x -= properties.borderSize * 2;
    cacheDimensions.y -= properties.borderSize * 2;
    SDL_Surface* cacheBorderSurface1 = element.getHost()->CreateSurfaceFill(&cacheDimensions, properties.color);
    SDL_Rect drawRect;
    drawRect.x = properties.borderSize;
    drawRect.y = properties.borderSize;
    drawRect.w = cacheDimensions.x;
    drawRect.h = cacheDimensions.y;
    SDL_BlitSurface(cacheBorderSurface1, NULL, cacheBorderSurface0, &drawRect);
    _texture = element.getHost()->CreateTextureSurface(cacheBorderSurface0);
    SDL_FreeSurface(cacheBorderSurface0);
    SDL_FreeSurface(cacheBorderSurface1);
}

void JuicyGUI_Box::resetTextures() {
    destroyTextures();
    initTextures();
    createTextures();
}

void JuicyGUI_Box::destroyTextures() {
    if (_texture != NULL) SDL_DestroyTexture(_texture);
}
