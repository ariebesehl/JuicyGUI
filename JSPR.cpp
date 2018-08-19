#include <iostream>
#include "JSPR.h"

JSPR::JSPR(JEN* iEngine) {
	engine = iEngine;
	init(0);
}
JSPR::JSPR(JEN* iEngine, JD_INDEX iSize) {
	engine = iEngine;
	init(iSize);
}

JSPR::~JSPR() {
	destroyInstructions();
	destroySprites();
}

void JSPR::init(JD_INDEX iSize) {
	flag = (iSize ? JSPR_FLAG_FIXED : 0) | JSPR_FLAG_INIT;
	ctrSprites = 0;
	numSprites = iSize;
	sprites = NULL;
	ctrInstructions = 0;
	numInstructions = 0;
	instructions = NULL;
}

void JSPR::Reset() {
    flushInstructions();
    clearSprites();
}

JD_INDEX JSPR::AddSprite(JuicySprite* iSprite) {
	if (flag & JSPR_FLAG_FIXED) {
		if (sprites == NULL) {
			sprites = new JuicySprite*[numSprites];
			JDM_NullList((void**)sprites, &numSprites);
		}
		sprites[ctrSprites % numSprites] = iSprite;
		ctrSprites++;
		return ctrSprites;
	} else {
	    if (ctrSprites >= numSprites) {
            JDM_AddToList((void***)&sprites, &ctrSprites, (void*)iSprite);
            numSprites = ctrSprites;
	    } else {
	        sprites[ctrSprites] = iSprite;
	        ctrSprites++;
	    }
		return ctrSprites;
	}
}
JD_INDEX JSPR::AddSpecificSprite(JD_INDEX iIndex, JuicySprite* iSprite) {
	if (flag & JSPR_FLAG_FIXED) {
		if (sprites == NULL) {
			sprites = new JuicySprite*[numSprites];
			JDM_NullList((void**)sprites, &numSprites);
		}
		sprites[iIndex % numSprites] = iSprite;
		return (iIndex % numSprites);
	}
	return 0;
}

JuicySprite* JSPR::GetSprite(JD_INDEX iIndex) {
    if (sprites != NULL) {return sprites[iIndex % numSprites];} else {return NULL;}
}


JD_INDEX JSPR::AddInstruction(JD_INDEX iIndex, const JD_Rect* iRect) {
    if (sprites != NULL) {
        if (ctrInstructions < JSPR_MAX_DRAW_NUM) {
            if (ctrInstructions >= numInstructions) {
                JDM_AddToList((void***)&instructions, &numInstructions, (void*)new JSPR_DrawInstruction);
            }
            instructions[ctrInstructions]->index = iIndex;
            instructions[ctrInstructions]->rect = *iRect;
            ctrInstructions++;
        }
    }
	return ctrInstructions;
}

void JSPR::Draw() {
	if (instructions != NULL) {
		for (JD_INDEX i = 0; i < ctrInstructions; i++) {
			if (instructions[i] != NULL)  {
                JuicySprite* sprite = (GetSprite(instructions[i]->index) != NULL) ? (GetSprite(instructions[i]->index)) : GetSprite(0);
				if (sprite != NULL) {engine->Render(sprite, &(instructions[i]->rect));}
			}
		}
		if (!(flag & JSPR_FLAG_NOFLUSH)) {
			flushInstructions();
		}
	}
}


void JSPR::flushInstructions() {
	ctrInstructions = 0;
}
void JSPR::clearInstructions() {
	if (instructions != NULL) {
        for (JD_INDEX i = 0; i < numInstructions; i++) {
			if (instructions[i] != NULL) {
				delete instructions[i];
				instructions[i] = NULL;
			}
        }
        flushInstructions();
	}
}
void JSPR::destroyInstructions() {
	if (instructions != NULL) {
        clearInstructions();
		JDM_DeleteList((void***)&instructions, &numInstructions);
	}
}

void JSPR::flushSprites() {
	ctrSprites = 0;
}
void JSPR::clearSprites() {
	if (sprites != NULL) {
        for (JD_INDEX i = 0; i < numSprites; i++) {
            engine->FreeSprite(sprites[i]);
            sprites[i] = NULL;
        }
        flushSprites();
	}
}
void JSPR::destroySprites() {
	if (sprites != NULL) {
        clearSprites();
		JDM_DeleteList((void***)&sprites, &numSprites);
	}
}

