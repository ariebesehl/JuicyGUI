
#include "JuicyTE.h"

JuicyTE::JuicyTE(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_FLAG iFlag) {
    JD_Point iSizeBase;
    iSizeBase.x = JUICYTE_DEFAULT_BASE_SIZE;
    iSizeBase.y = JUICYTE_DEFAULT_BASE_SIZE;
    JD_I iZoomLevels = JUICYTE_DEFAULT_ZOOM_STAGES;
    init(iHostUI, iSizeField, &iSizeBase, iZoomLevels, iFlag);
}
JuicyTE::JuicyTE(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_Point* iSizeBase, JD_I iZoomLevels, JD_FLAG iFlag) {
    init(iHostUI, iSizeField, iSizeBase, iZoomLevels, iFlag);
}

JuicyTE::~JuicyTE() {
	ClearTypes();
	gfxUnload();
    if (engine != NULL) {delete engine;}
}

void JuicyTE::init(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_Point* iSizeBase, JD_I iZoomLevels, JD_FLAG iFlag) {
	host = iHostUI;
	engine = new JEN(host->GetEngine()->GetSDL_Window(), host->GetEngine()->GetSDL_Renderer());
	engine->SetMode(JUICYENGINE_MODE_BUFFERED | JUICYENGINE_MODE_SDL_SURFACE);
	sizeField = *iSizeField;
	sizeBase = *iSizeBase;
	zoomStages = (iZoomLevels > 0) ? iZoomLevels : 1;
	flag = JUICYTE_FLAG_INIT | iFlag;
	sizeCam = sizeField * (sizeBase << (zoomStages - 1));
	posCam = sizeCam / 2;
	zoom = 0;
	gridSize = JUICYTE_DEFAULT_GRID_SIZE;
	gridColor = JUICYTE_DEFAULT_GRID_COLOR;
	edgeSize = JUICYTE_DEFAULT_EDGE_SIZE;
	edgeColor = JUICYTE_DEFAULT_EDGE_COLOR;
	data = NULL;
	dataMask = -1;
	dataShift = 0;
	numTypes = 0;
	types = NULL;
	mapSprite = NULL;
	gridC = NULL;
	gridH = NULL;
	gridV = NULL;
	edgeC = NULL;
	edgeH = NULL;
	edgeV = NULL;
    gfxLoad();
}

void JuicyTE::AddType(JD_INDEX iNum, JuicySprite** iSprites) {
    if (iSprites != NULL) {
        JuicyTE_Type* newType = new JuicyTE_Type;
        newType->numSprite = iNum;
        newType->ptrSprite = new JuicySprite*[iNum];
        JDM_CopyList((void**)iSprites, &iNum, (void**)(newType->ptrSprite));
        JDM_AddToList((void***)&types, numTypes,(void*)newType);
        numTypes++;
        checkReady();
    }
}

void JuicyTE::ClearTypes() {
	if (numTypes) {
		if (types != NULL) {
			for (unsigned int i = 0; i < numTypes; i++) {
				if (types[i] != NULL) {
                    delete[] types[i]->ptrSprite;
                    delete types[i];
                }
			}
			JDM_DeleteList((void***)&types, &numTypes);
		}
	}
	checkReady();
}

void JuicyTE::MoveCam(const JD_Point* iMove) {
	if (flag & JUICYTE_FLAG_READY && iMove != NULL) {
        if (JDM_IsNotEmptyPoint(iMove)) {
            JD_Point moveDelta = *iMove;
            moveDelta *= 1 << (zoomStages - (zoom + 1));
            moveDelta += posCam;
            if (moveDelta.x >= 0 && moveDelta.x < sizeCam.x) {
                posCam.x = moveDelta.x;
            } else {
                if (moveDelta.x < 0) {
                    posCam.x = 0;
                } else {
                    posCam.x = sizeCam.x - 1;
                }
            }
            if (moveDelta.y >= 0 && moveDelta.y < sizeCam.y) {
                posCam.y = moveDelta.y;
            } else {
                if (moveDelta.y < 0) {
                    posCam.y = 0;
                } else {
                    posCam.y = sizeCam.y - 1;
                }
            }
        }
	}
}

void JuicyTE::CenterCam() {
	if (flag & JUICYTE_FLAG_READY) {
        JD_Point cache = sizeField / 2;
        SetCam(&cache);
	}
}
void JuicyTE::SetCam(const JD_Point* iPos) {
    if (iPos != NULL) {
        posCam.x = iPos->x *(sizeBase.x << (zoomStages - 1));
        posCam.y = iPos->y *(sizeBase.y << (zoomStages - 1));
    }
}

JD_I JuicyTE::Zoom(JD_I iAmount) {
	if (flag & JUICYTE_FLAG_READY) {
        int newVal = zoom + iAmount;
        if (newVal < int(zoomStages) && newVal >= 0) {
            zoom = newVal;
        } else {
            if (newVal < 0) {
                zoom = 0;
            } else {
                zoom = zoomStages - 1;
            }
        }
	}
	return zoom;
}
void JuicyTE::SetZoom(unsigned int iLevel) {
    if (iLevel < zoomStages) zoom = iLevel;
}
void JuicyTE::SetData(void** iData) {
    SetData(iData, -1, 0);
}
void JuicyTE::SetData(void** iData, unsigned int iMask, int iShift) {
	data = iData;
	dataMask = iMask;
	dataShift = iShift;
	checkReady();
}

void JuicyTE::checkReady() {
	if (!(flag & JUICYTE_FLAG_READY)) {
		if (data != NULL && (flag & JUICYTE_FLAG_GFX_LOADED)) {flag |= JUICYTE_FLAG_READY;}
	}
}


void JuicyTE::gfxLoad() {
    JD_Point sizeGridC;
    JD_Point sizeEdgeC;
    sizeGridC.x = gridSize;
    sizeGridC.y = gridSize;
    sizeEdgeC.x = edgeSize;
    sizeEdgeC.y = edgeSize;
    JD_COLOR* pixelC = new JD_COLOR[JDM_GetArea(&sizeGridC)];
    for (JD_I i = 0; i < JDM_GetArea(&sizeGridC); i++) {
        pixelC[i] = gridColor;
    }
    gridC = engine->CreateSprite(&sizeGridC, pixelC);
    delete[] pixelC;
    JD_COLOR* pixelEdgeC = new JD_COLOR[JDM_GetArea(&sizeEdgeC)];
    for (JD_I i = 0; i < JDM_GetArea(&sizeEdgeC); i++) {
        pixelEdgeC[i] = edgeColor;
    }
    edgeC = engine->CreateSprite(&sizeEdgeC, pixelEdgeC);
    delete[] pixelEdgeC;
    bool loaded = gridC != NULL && edgeC != NULL;
    gridH = new JuicySprite*[zoomStages];
    gridV = new JuicySprite*[zoomStages];
    edgeH = new JuicySprite*[zoomStages];
    edgeV = new JuicySprite*[zoomStages];
    JDM_NullList((void**)gridH, zoomStages);
    JDM_NullList((void**)gridV, zoomStages);
    JDM_NullList((void**)edgeH, zoomStages);
    JDM_NullList((void**)edgeV, zoomStages);
    JD_Point sizeGridH;
    JD_Point sizeGridV;
    JD_Point sizeEdgeH;
    JD_Point sizeEdgeV;
    sizeGridH.y = gridSize;
    sizeGridV.x = gridSize;
    sizeEdgeH.y = edgeSize;
    sizeEdgeV.x = edgeSize;
    for (JD_INDEX i = 2; i < zoomStages; i++) {
        sizeGridH.x = (sizeBase.x << i) - 2 * gridSize;
        sizeGridV.y = (sizeBase.y << i) - 2 * gridSize;
        JD_COLOR* pixelGrid = new JD_COLOR[gridSize * ((sizeBase.y << i) - 2 * gridSize)];
        for (JD_I ii = 0; ii < (gridSize * ((sizeBase.y << i) - 2 * gridSize)); ii++) {
            pixelGrid[ii] = gridColor;
        }
        gridH[i] = engine->CreateSprite(&sizeGridH, pixelGrid);
        gridV[i] = engine->CreateSprite(&sizeGridV, pixelGrid);
        delete[] pixelGrid;
        loaded &= (gridH[i] != NULL) && (gridV[i] != NULL);
    }
    for (JD_INDEX i = 0; i < zoomStages; i++) {
        sizeEdgeH.x = (sizeBase.x << i);
        sizeEdgeV.y = (sizeBase.y << i);
        JD_COLOR* pixelEdge = new JD_COLOR[edgeSize * (sizeBase.y << i)];
        for (JD_I ii = 0; ii < (edgeSize * (sizeBase.y << i)); ii++) {
            pixelEdge[ii] = edgeColor;
        }
        edgeH[i] = engine->CreateSprite(&sizeEdgeH, pixelEdge);
        edgeV[i] = engine->CreateSprite(&sizeEdgeV, pixelEdge);
        delete[] pixelEdge;
        loaded =  (edgeH[i] != NULL) && (edgeV[i] != NULL);
    }
    flag |= loaded ? JUICYTE_FLAG_GFX_LOADED : 0;

}
void JuicyTE::gfxUnload() {
    engine->FreeSprite(mapSprite);
    mapSprite = NULL;
    engine->FreeSprite(gridC);
    gridC = NULL;
    if (gridH != NULL) {
        for (JD_INDEX i = 1; i < zoomStages; i++) {
            engine->FreeSprite(gridH[i]);
        }
    }
    delete[] gridH;
    gridH = NULL;
    if (gridV != NULL) {
        for (JD_INDEX i = 1; i < zoomStages; i++) {
            engine->FreeSprite(gridV[i]);
        }
    }
    delete[] gridV;
    gridV = NULL;
    engine->FreeSprite(edgeC);
    edgeC = NULL;
    if (edgeH != NULL) {
        for (JD_INDEX i = 1; i < zoomStages; i++) {
            engine->FreeSprite(edgeH[i]);
        }
    }
    delete[] edgeH;
    edgeH = NULL;
    if (edgeV != NULL) {
        for (JD_INDEX i = 1; i < zoomStages; i++) {
            engine->FreeSprite(edgeV[i]);
        }
    }
    delete[] edgeV;
    edgeV = NULL;
    flag &= ~JUICYTE_FLAG_GFX_LOADED;
}
void JuicyTE::gfxReload() {
    gfxUnload();
    gfxLoad();
}

void JuicyTE::drawMetrics() {
	if (flag & JUICYTE_FLAG_READY) {
        JD_Point newScreen;
        engine->GetWindowSize(&newScreen);
        if (newScreen != sizeScreen) {
            sizeScreen = newScreen;
        }
        drawSize = sizeBase << int(zoom);
        drawOffset = (((sizeCam / 2) - posCam) >> (zoomStages - (zoom + 1))) + (sizeScreen - sizeField * drawSize) / 2;
        JD_Rect canvasRect;
        canvasRect.w = sizeScreen.x * (sizeBase.x << (zoomStages - (zoom + 1)));
        canvasRect.h = sizeScreen.y * (sizeBase.y << (zoomStages - (zoom + 1)));
        canvasRect.x = posCam.x - (canvasRect.w / 2);
        canvasRect.y = posCam.y - (canvasRect.h / 2);
        drawStart.x = (canvasRect.x / sizeBase.x) >> (zoomStages - 1);
        drawStart.y = (canvasRect.y / sizeBase.y) >> (zoomStages - 1);
        drawStop.x = (((canvasRect.x + canvasRect.w) / sizeBase.x) >> (zoomStages - 1));
        drawStop.y = (((canvasRect.y + canvasRect.h) / sizeBase.y) >> (zoomStages - 1));
        drawStop += 1;
        if (drawStart.x < 0) drawStart.x = 0;
        if (drawStart.x >= sizeField.x) drawStart.x = sizeField.x;
        if (drawStop.x < 0) drawStop.x = 0;
        if (drawStop.x >= sizeField.x) drawStop.x = sizeField.x;
        if (drawStart.y < 0) drawStart.y = 0;
        if (drawStart.y >= sizeField.y) drawStart.y = sizeField.y;
        if (drawStop.y < 0) drawStop.y = 0;
        if (drawStop.y >= sizeField.y) drawStop.y = sizeField.y;
	}
}

void JuicyTE::Draw() {
	if (flag & JUICYTE_FLAG_READY) {
        drawMetrics();
        JD_U* ptrData = (JD_U*)(*data);
        JD_Point mapSize = (drawStop - drawStart) << zoom;
        JD_Rect drawRect;
        JDM_SetRectSize(&drawRect, &drawSize);
        if (!numTypes) {
            if (mapSprite == NULL) {
                mapSprite = engine->CreateSpriteFill(&mapSize, 0x0);
            } else {
                if (mapSize != mapSprite->dimensions) {
                    engine->FreeSprite(mapSprite);
                    mapSprite = engine->CreateSpriteFill(&mapSize, 0x0);
                }
            }
        }
        JuicySprite* spritePtr = NULL;
        JD_U tileVal;
        JD_Point gridPos;
        JD_FLAG gridConfig;
        for (JD_I y = drawStart.y; y < drawStop.y; y++) {
            for (JD_I x = drawStart.x; x < drawStop.x; x++) {
                drawRect.x = x * drawSize.x + drawOffset.x;
                drawRect.y = y * drawSize.y + drawOffset.y;
                tileVal = *(ptrData + x + y * sizeField.x);
                if (numTypes) {
                    tileVal >>= dataShift;
                    tileVal &= dataMask;
                    if (tileVal >= numTypes) {tileVal = 0;}
                    spritePtr = types[tileVal]->ptrSprite[zoom];
                    engine->Render(spritePtr, &drawRect);
                } else {
                    for (JD_I iy = 0; iy < (1 << zoom); iy++) {
                        for (JD_I ix = 0; ix < (1 << zoom); ix++) {
                            mapSprite->pixels[((x - drawStart.x) << zoom) + ix + (((y - drawStart.y) << zoom) + iy) * mapSize.x] = tileVal;
                        }
                    }
                }
                if (zoom && (flag & JUICYTE_FLAG_GRID)) {
                    JDM_GetRectPos(&drawRect, &gridPos);
                    gridConfig = 0x37;
                    if (!x) {gridConfig &= ~0x24;}
                    if (!y) {gridConfig &= ~0x12;}
                    if (!x && !y) {gridConfig &= ~0x1;}
                    drawGrid(&gridPos, gridConfig);
                }
                if (flag & JUICYTE_FLAG_EDGE) {
                    if (!x || !y || x == sizeField.x - 1 || y == sizeField.y - 1) {
                        JDM_GetRectPos(&drawRect, &gridPos);
                        gridConfig =
                        ((x == sizeField.x - 1) << 7) |
                        ((y == sizeField.y - 1) << 6) |
                        ((!x) << 5) |
                        ((!y) << 4) |
                        ((x == (sizeField.x - 1) && y == (sizeField.y - 1) ) << 3) |
                        ((!x && y == (sizeField.y - 1) ) << 2) |
                        ((x == (sizeField.x - 1) && !y) << 1) |
                        (!x && !y);
                        drawEdge(&gridPos, gridConfig);
                    }
                }
            }
        }
        if (!numTypes) {
            engine->OptimizeSprite(mapSprite);
            JD_Point drawPos = drawStart * drawSize + drawOffset;
            engine->PresentSprite(mapSprite, &drawPos);
        }
        engine->Present();
	}
}

void JuicyTE::IdentifyCell(const JD_Point* iScreenPos, JD_Point* oCell) {
    if (iScreenPos != NULL && oCell != NULL) {
        *oCell = (*iScreenPos - drawOffset) / drawSize;
    }
}

void JuicyTE::drawGrid(const JD_Point* iPos, JD_FLAG iConfig) {
    if (iConfig & 0x1) {
        JD_Rect rectGridC;
        JDM_SetRectPos(&rectGridC, iPos);
        JDM_SetRectSize(&rectGridC, gridSize, gridSize);
        engine->Render(gridC, &rectGridC);
    }
    if (iConfig & 0x2) {
        JD_Rect rectGridCR;
        JDM_SetRectPos(&rectGridCR, iPos->x + drawSize.x - gridSize, iPos->y);
        JDM_SetRectSize(&rectGridCR, gridSize, gridSize);
        engine->Render(gridC, &rectGridCR);
    }
    if (iConfig & 0x4) {
        JD_Rect rectGridCB;
        JDM_SetRectPos(&rectGridCB, iPos->x, iPos->y + drawSize.y - gridSize);
        JDM_SetRectSize(&rectGridCB, gridSize, gridSize);
        engine->Render(gridC, &rectGridCB);
    }
    if (iConfig & 0x8) {
        JD_Rect rectGridCBR;
        JDM_SetRectPos(&rectGridCBR, iPos->x + drawSize.x - gridSize, iPos->y + drawSize.y - gridSize);
        JDM_SetRectSize(&rectGridCBR, gridSize, gridSize);
        engine->Render(gridC, &rectGridCBR);
    }
    if (iConfig & 0x10) {
        JD_Rect rectGridHT;
        JDM_SetRectPos(&rectGridHT, iPos->x + gridSize, iPos->y);
        JDM_SetRectSize(&rectGridHT, (drawSize.x - 2 * gridSize), gridSize);
        engine->Render(gridH[zoom], &rectGridHT);
    }
    if (iConfig & 0x20) {
        JD_Rect rectGridVL;
        JDM_SetRectPos(&rectGridVL, iPos->x, iPos->y + gridSize);
        JDM_SetRectSize(&rectGridVL, gridSize, (drawSize.y - 2 * gridSize));
        engine->Render(gridV[zoom], &rectGridVL);
    }
    if (iConfig & 0x40) {
        JD_Rect rectGridHB;
        JDM_SetRectPos(&rectGridHB, iPos->x + gridSize, iPos->y + drawSize.y - gridSize);
        JDM_SetRectSize(&rectGridHB, (drawSize.x - 2 * gridSize), gridSize);
        engine->Render(gridH[zoom], &rectGridHB);
    }
    if (iConfig & 0x80) {
        JD_Rect rectGridVR;
        JDM_SetRectPos(&rectGridVR, iPos->x + drawSize.x - gridSize, iPos->y + gridSize);
        JDM_SetRectSize(&rectGridVR, gridSize, (drawSize.y - 2 * gridSize));
        engine->Render(gridV[zoom], &rectGridVR);
    }
}

void JuicyTE::drawEdge(const JD_Point* iPos, JD_FLAG iConfig) {
    if (iConfig & 0x1) {
        JD_Rect rectEdgeC;
        JDM_SetRectPos(&rectEdgeC, iPos->x - edgeSize, iPos->y - edgeSize);
        JDM_SetRectSize(&rectEdgeC, edgeSize, edgeSize);
        engine->Render(edgeC, &rectEdgeC);
    }
    if (iConfig & 0x2) {
        JD_Rect rectEdgeCR;
        JDM_SetRectPos(&rectEdgeCR, iPos->x + drawSize.x, iPos->y - edgeSize);
        JDM_SetRectSize(&rectEdgeCR, edgeSize, edgeSize);
        engine->Render(edgeC, &rectEdgeCR);
    }
    if (iConfig & 0x4) {
        JD_Rect rectEdgeCB;
        JDM_SetRectPos(&rectEdgeCB, iPos->x - edgeSize, iPos->y + drawSize.y);
        JDM_SetRectSize(&rectEdgeCB, edgeSize, edgeSize);
        engine->Render(edgeC, &rectEdgeCB);
    }
    if (iConfig & 0x8) {
        JD_Rect rectEdgeCBR;
        JDM_SetRectPos(&rectEdgeCBR, iPos->x + drawSize.x, iPos->y + drawSize.y);
        JDM_SetRectSize(&rectEdgeCBR, edgeSize, edgeSize);
        engine->Render(edgeC, &rectEdgeCBR);
    }
    if (iConfig & 0x10) {
        JD_Rect rectEdgeHT;
        JDM_SetRectPos(&rectEdgeHT, iPos->x, iPos->y - edgeSize);
        JDM_SetRectSize(&rectEdgeHT, drawSize.x, edgeSize);
        engine->Render(edgeH[zoom], &rectEdgeHT);
    }
    if (iConfig & 0x20) {
        JD_Rect rectEdgeVL;
        JDM_SetRectPos(&rectEdgeVL, iPos->x - edgeSize, iPos->y);
        JDM_SetRectSize(&rectEdgeVL, edgeSize, drawSize.y);
        engine->Render(edgeV[zoom], &rectEdgeVL);
    }
    if (iConfig & 0x40) {
        JD_Rect rectEdgeHB;
        JDM_SetRectPos(&rectEdgeHB, iPos->x, iPos->y + drawSize.y);
        JDM_SetRectSize(&rectEdgeHB, drawSize.x, edgeSize);
        engine->Render(edgeH[zoom], &rectEdgeHB);
    }
    if (iConfig & 0x80) {
        JD_Rect rectEdgeVR;
        JDM_SetRectPos(&rectEdgeVR, iPos->x + drawSize.x, iPos->y);
        JDM_SetRectSize(&rectEdgeVR, edgeSize, drawSize.y);
        engine->Render(edgeV[zoom], &rectEdgeVR);
    }
}
