#ifndef JUICY_SDL_H_INCLUDED
#define JUICY_SDL_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


bool operator!= (const SDL_Point& param0, const SDL_Point& param1);
bool operator== (const SDL_Point& param0, const SDL_Point& param1);
SDL_Point operator+ (const SDL_Point& param0, const SDL_Point& param1);
SDL_Point operator- (const SDL_Point& param0, const SDL_Point& param1);

bool JSDL_IsRectEmpty(const SDL_Rect* iRect);

void JSDL_SetRectPos(SDL_Rect* iRect, const SDL_Point* iPosition);



#endif // JUICY_SDL_H_INCLUDED
