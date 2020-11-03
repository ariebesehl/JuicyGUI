#ifndef CONWAY_SDL_H_INCLUDED
#define CONWAY_SDL_H_INCLUDED

#include "../JuicyGUI/Juicy.h"
#include "../JuicyTE/JuicyTE.h"
#include "conway_core.h"

#define CONWAY_ZOOM_LEVELS 8
#define CONWAY_BASE_X 1
#define CONWAY_BASE_Y CONWAY_BASE_X
#define CONWAY_SPEED_DEFAULT 0x20
#define CONWAY_SPEED_MIN 0x2000

#define CONWAY_FLAG_INIT CONWAY_FLAG_PAUSED
#define CONWAY_FLAG_PAUSED 0x2

#define CONWAY_VALUE_EMPTY 0x000000ff
#define CONWAY_VALUE_ALIVE 0xffffffff


class ConwaySDL {
    public:
        ConwaySDL(JuicyGUI* hostUI, unsigned int size_x, unsigned int size_y);
        ~ConwaySDL();
		void LoopHandle();
		void Draw();
		void SetCell(const JD_Point* iCell, bool iAlive);
		void FlipCell();
        void FlipCell(const JD_Point* iCell);
		void TogglePause() {flag ^= CONWAY_FLAG_PAUSED;};
		void Pause() {flag |= CONWAY_FLAG_PAUSED;};
		void Unpause() {flag &= ~CONWAY_FLAG_PAUSED;};
		void Move(const JD_Point* iMove);
		void Zoom(int iAmount);
		void ZoomAndCenter(int iAmount);
		void RunFaster();
		void RunSlower();
		void ResetCamera();
		void ToggleGrid();
		void ToggleEdge();
		JD_INDEX GetFPS() {return tileEngine->GetFPS();};
		JD_INDEX GetIPS() {return numIPS;};
		JD_FLAG GetPeriod() {return gameSpeed;};
		JD_INDEX GetIteration() {return numIteration;};
		JD_INDEX GetCount() {return numAlive;};
		JD_INDEX GetRatio() {return numRatio;};
		void ForceUpdateStats();
		void SetZoom(JD_INDEX iZoom) {if (tileEngine != NULL) tileEngine->SetZoom(iZoom);};
		void GetSize(JD_Point* oSize) {if (oSize != NULL) {*oSize = sizeField;}};
    private:
		JuicyGUI* host;
		JuicyTE* tileEngine;
		ConwayGOL* gameMain;
		JD_INDEX numIteration;
		JD_INDEX numAlive;
		JD_INDEX numRatio;
		JD_FLAG flag;
		JD_FLAG gameSpeed;
		JD_Point sizeField;
		JD_TIME tmilLast;
		JD_TIME tmilIPS;
		JD_INDEX ctrIPS;
		JD_INDEX numIPS;
		JD_TIME countDownTimer;

		void initGame();
		void exitGame();

		void loadGFX();
		void clearGFX();

		JuicySprite** spriteEmpty;
		JuicySprite** spriteAlive;
};


#endif // CONWAY_SDL_H_INCLUDED
