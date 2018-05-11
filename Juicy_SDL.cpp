#include "Juicy_SDL.h"

bool operator!= (const SDL_Point& param0, const SDL_Point& param1) {
    return (param0.x != param1.x || param0.y != param1.y);
}

bool operator== (const SDL_Point& param0, const SDL_Point& param1) {
    return (param0.x == param1.x && param0.y == param1.y);
}

SDL_Point operator+ (const SDL_Point& param0, const SDL_Point& param1) {
    SDL_Point sum;
    sum.x = param0.x + param1.x;
    sum.y = param0.y + param1.y;
    return sum;
}

SDL_Point operator- (const SDL_Point& param0, const SDL_Point& param1) {
    SDL_Point diff;
    diff.x = param0.x - param1.x;
    diff.y = param0.y - param1.y;
    return diff;
}

bool JSDL_IsRectEmpty(const SDL_Rect& iRect) {
    return (iRect.w == 0 && iRect.h == 0);
}

void JSDL_SetRectPos(SDL_Rect& iRect, const SDL_Point& iPosition) {
        iRect.x = iPosition.x;
        iRect.y = iPosition.y;
}
