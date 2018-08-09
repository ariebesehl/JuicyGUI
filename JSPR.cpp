
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
	destroy();
}

void JSPR::Reset() {
	destroy();
	init((flag & JSPR_FLAG_FIXED) ? numTextures : 0);
}

JD_INDEX JSPR::AddTexture(SDL_Texture* iTexture) {
	if (flag & JSPR_FLAG_FIXED) {
		if (textures == NULL) {
			textures = new SDL_Texture*[numTextures];
			JDM_NullList((void**)textures, &numTextures);
		}
		textures[ctrTextures % numTextures] = iTexture;
		ctrTextures++;
		return ctrTextures;
	} else {
	    JDM_AddToList((void***)&textures, &ctrTextures, (void*)iTexture);
	    numTextures = ctrTextures;
		return ctrTextures;
	}
}

void JSPR::AddSpecificTexture(JD_INDEX iIndex, SDL_Texture* iTexture) {
	if (flag & JSPR_FLAG_FIXED) {
		if (textures == NULL) {
			textures = new SDL_Texture*[numTextures];
			JDM_NullList((void**)textures, &numTextures);
		}
		textures[iIndex % numTextures] = iTexture;
	}
}

SDL_Texture* JSPR::GetTexture(JD_INDEX iIndex) {
    return textures[iIndex % numTextures];
}
JD_INDEX JSPR::AddInstruction(JD_INDEX iIndex, const JD_Rect* iRect) {
    if (ctrInstructions < JSPR_MAX_DRAW_NUM) {
        if (ctrInstructions >= numInstructions) {
            JDM_AddToList((void***)&instructions, &numInstructions, (void*)new JSPR_DrawInstruction);
        }
        instructions[ctrInstructions]->index = iIndex;
        instructions[ctrInstructions]->rect = *iRect;
        ctrInstructions++;
    }
	return ctrInstructions;
}
/*
JSPR_DrawInstruction* JSPR::GetLastInstruction() {
	if (instructions != NULL) {
		return instructions[ctrInstructions];
	}
}
*/

void JSPR::Draw() {
	if (instructions != NULL) {
		for (JD_INDEX i = 0; i < ctrInstructions; i++) {
			if (instructions[i] != NULL)  {
                JD_INDEX index = (GetTexture(instructions[i]->index) != NULL) ? (instructions[i]->index) : 0;
				engine->RenderTexture(GetTexture(index), &(instructions[i]->rect));
			}
		}
		if (!(flag & JSPR_FLAG_NOFLUSH)) {
			flushInstructions();
		}
	}
}

void JSPR::init(JD_INDEX iSize) {
	flag = (iSize ? JSPR_FLAG_FIXED : 0) | JSPR_FLAG_INIT;
	ctrTextures = 0;
	numTextures = iSize;
	textures = NULL;
	ctrInstructions = 0;
	numInstructions = 0;
	instructions = NULL;
}

void JSPR::flushInstructions() {
	/*
	for (JD_INDEX i = 0; i < ctrInstructions; i++) {
		if (instructions[i] != NULL)  {
			delete instructions[i];
			instructions[i] = NULL;
		}
	}
	*/
	ctrInstructions = 0;
}

void JSPR::destroy() {
	destroyInstructions();
	destroyTextures();
}

void JSPR::destroyTextures() {
	if (textures != NULL) {
		for (JD_INDEX i = 0; i < numTextures; i++) {
			if (textures[i] != NULL) {
				SDL_DestroyTexture(textures[i]);
			}
		}
		delete[] textures;
		textures = NULL;
	}
}

void JSPR::destroyInstructions() {
	if (instructions != NULL) {
		for (JD_INDEX i = 0; i < numInstructions; i++) {
			if (instructions[i] != NULL) {
				delete instructions[i];
			}
		}
		JDM_DeleteList((void***)&instructions, &numInstructions);
	}
}
