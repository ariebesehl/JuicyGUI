
#include "conway_sdl.h"


ConwaySDL::ConwaySDL(JuicyGUI* host_UI, unsigned int _x, unsigned int _y) {
	sizeField.x = _x;
	sizeField.y = _y;
	host = host_UI;
	numIteration = 0;
	numAlive = 0;
	numRatio = 0;
	flag = CONWAY_FLAG_INIT;
	tmilLast = host->GetTicks();
	gameSpeed = CONWAY_SPEED_DEFAULT;
	countDownTimer = gameSpeed;
	tileEngine = NULL;
	gameMain = NULL;
	spriteEmpty = new JuicySprite*[CONWAY_ZOOM_LEVELS];
	spriteAlive = new JuicySprite*[CONWAY_ZOOM_LEVELS];
	JDM_NullList((void**)spriteEmpty, CONWAY_ZOOM_LEVELS);
	JDM_NullList((void**)spriteAlive, CONWAY_ZOOM_LEVELS);
	initGame();
}

ConwaySDL::~ConwaySDL() {
	exitGame();
}

void ConwaySDL::RunFaster() {
    if (JDM_GetLSB(gameSpeed) >> 1) {
        gameSpeed >>= 1;
    }
}

void ConwaySDL::RunSlower() {
    if ((gameSpeed << 1) < CONWAY_SPEED_MIN) {
        gameSpeed <<= 1;
    }
}
void ConwaySDL::ForceUpdateStats() {
    if (gameMain != NULL) {
        numAlive = gameMain->CountAlive();
        numRatio = numAlive * 10000 / JDM_GetArea(&sizeField);
    }
}
void ConwaySDL::LoopHandle() {
    if (tileEngine != NULL) {
        tileEngine->LoopHandle();
    }
    if (gameMain != NULL) {
        if (!(flag & CONWAY_FLAG_PAUSED)) {
            if ((host->GetTicks() - countDownTimer) >= gameSpeed) {
                numAlive = gameMain->Iterate();
                numRatio = numAlive * 10000 / JDM_GetArea(&sizeField);
                numIteration++;
                countDownTimer = host->GetTicks() - ((host->GetTicks() - countDownTimer) % gameSpeed);
            }
            if ((host->GetTicks() - tmilIPS) > 999) {
                numIPS = numIteration - ctrIPS;
                ctrIPS = numIteration;
                tmilIPS = host->GetTicks() - ((host->GetTicks() - tmilIPS) % 1000);
            }
        }
    }
	tmilLast = host->GetTicks();
}

void ConwaySDL::SetCell(const JD_Point* iCell, bool iValue) {
    if (tileEngine != NULL && iCell != NULL) {
        if (iValue) {
            gameMain->SetCellAlive((unsigned int) iCell->x, (unsigned int) iCell->y);
        } else {
            gameMain->SetCellEmpty((unsigned int) iCell->x, (unsigned int) iCell->y);
        }
    }
}
void ConwaySDL::FlipCell() {
    if (tileEngine != NULL) {
        JD_Point mousePos;
        host->GetMousePos(&mousePos);
        JD_Point cellPos;
        tileEngine->IdentifyCell(&mousePos, &cellPos);
        FlipCell(&cellPos);
    }
}
void ConwaySDL::FlipCell(const JD_Point* iCell) {
    if (tileEngine != NULL && iCell != NULL) {
        gameMain->FlipCell((unsigned int) iCell->x, (unsigned int) iCell->y);
    }
}

void ConwaySDL::Draw() {
    if (gameMain != NULL && tileEngine != NULL) {
        tileEngine->Draw();
    }
}

void ConwaySDL::Zoom(int iAmount) {
    if (tileEngine != NULL) tileEngine->Zoom(iAmount);
}
void ConwaySDL::ZoomAndCenter(int iAmount) {
    if (tileEngine != NULL) {
        JD_Point mousePos;
        host->GetMousePos(&mousePos);
        JD_Point cellPos;
        tileEngine->IdentifyCell(&mousePos, &cellPos);
        if (cellPos.x < 0) cellPos.x = 0;
        if (cellPos.x >= int(sizeField.x)) cellPos.x = int(sizeField.x) - 1;
        if (cellPos.y < 0) cellPos.y = 0;
        if (cellPos.y >= int(sizeField.y)) cellPos.y = int(sizeField.y) - 1;
        if (((tileEngine->Zoom(iAmount) + 1) < CONWAY_ZOOM_LEVELS) && iAmount > 0) tileEngine->SetCam(&cellPos);
    }
}


void ConwaySDL::Move(const JD_Point* iMove) {
    if (tileEngine != NULL) {tileEngine->MoveCam(iMove);}
}

void ConwaySDL::ToggleGrid() {
    if (tileEngine != NULL) {tileEngine->ToggleGrid();}
}
void ConwaySDL::ToggleEdge() {
    if (tileEngine != NULL) {tileEngine->ToggleEdge();}
}

void ConwaySDL::ResetCamera() {
    if (tileEngine != NULL) {tileEngine->CenterCam();}
}


void ConwaySDL::initGame() {
	if (gameMain == NULL) {
		gameMain = new ConwayGOL(sizeField.x, sizeField.y);
		gameMain->SetValueEmpty(CONWAY_VALUE_EMPTY);
		gameMain->SetValueAlive(CONWAY_VALUE_ALIVE);
		loadGFX();
		if (tileEngine == NULL) {
            unsigned int teFlag = JUICYTE_FLAG_NONE;
            JD_Point sizeBase;
            sizeBase.x = CONWAY_BASE_X;
            sizeBase.y = CONWAY_BASE_Y;
            tileEngine = new JuicyTE(host, &sizeField, &sizeBase, CONWAY_ZOOM_LEVELS, teFlag);
            tileEngine->AddType(CONWAY_ZOOM_LEVELS, spriteEmpty);
            tileEngine->AddType(CONWAY_ZOOM_LEVELS, spriteAlive);
            tileEngine->SetData((void**)gameMain->GetDataPtr(), 0x1, 8);
		}
	}
}

void ConwaySDL::exitGame() {
    clearGFX();
    if (spriteEmpty != NULL) {
        delete[] spriteEmpty;
        spriteEmpty = NULL;
    }
    if (spriteAlive != NULL) {
        delete[] spriteAlive;
        spriteAlive = NULL;
    }
    if (tileEngine != NULL) {
        delete tileEngine;
        tileEngine = NULL;
    }
	if (gameMain != NULL) {
		delete gameMain;
		gameMain = NULL;
	}
}

void ConwaySDL::loadGFX() {
    JD_Point sizeCell;
    for (JD_INDEX i = 0; i < CONWAY_ZOOM_LEVELS; i++) {
        sizeCell.x = CONWAY_BASE_X << i;
        sizeCell.y = CONWAY_BASE_Y << i;
        JD_COLOR* pixelEmpty = new JD_COLOR[JDM_GetArea(&sizeCell)];
        JD_COLOR* pixelAlive = new JD_COLOR[JDM_GetArea(&sizeCell)];
        for (JD_I ii = 0; ii < JDM_GetArea(&sizeCell); ii++) {
            pixelEmpty[ii] = 0x000000ff;
            pixelAlive[ii] = 0xffffffff;
        }
        spriteEmpty[i] = host->GetEngine()->CreateSprite(&sizeCell, pixelEmpty);
        spriteAlive[i] = host->GetEngine()->CreateSprite(&sizeCell, pixelAlive);
        delete[] pixelEmpty;
        delete[] pixelAlive;
    }

}

void ConwaySDL::clearGFX() {
    for (JD_INDEX i = 0; i < CONWAY_ZOOM_LEVELS; i++) {
        host->GetEngine()->FreeSprite(spriteEmpty[i]);
        spriteEmpty[i] = NULL;
        host->GetEngine()->FreeSprite(spriteAlive[i]);
        spriteAlive[i] = NULL;
    }
}

