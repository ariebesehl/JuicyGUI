
#include "JuicyTX.h"

JuicyTX::JuicyTX(JuicyGUI* iHost) {
	host = iHost;
	init(0);
}
JuicyTX::JuicyTX(JuicyGUI* iHost, JD_INDEX iSize) {
	host = iHost;
	init(iSize);
}

JuicyTX::~JuicyTX() {
	destroy();
}

void JuicyTX::Reset() {
	destroy();
	init((flag & JUICYTX_FLAG_FIXED) ? numTextures : 0);
}

JD_INDEX JuicyTX::AddTexture(SDL_Texture* iTexture, JD_Point* iSize) {
	if (flag & JUICYTX_FLAG_FIXED) {
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

void JuicyTX::AddSpecificTexture(JD_INDEX iIndex, SDL_Texture* iTexture, JD_Point* iSize) {
	if (flag & JUICYTX_FLAG_FIXED) {
		if (textures == NULL) {
			textures = new SDL_Texture*[numTextures];
			JDM_NullList((void**)textures, &numTextures);
		}
		textures[iIndex % numTextures] = iTexture;
	}
}

SDL_Texture* JuicyTX::GetTexture(JD_INDEX iIndex) {
    return textures[iIndex % numTextures];
}
JD_INDEX JuicyTX::AddInstruction(JD_INDEX iIndex, const JD_Rect* iRect) {
    if (ctrInstructions < JUICYTX_MAX_DRAW_NUM) {
        if (ctrInstructions >= numInstructions) {
            JDM_AddToList((void***)&instructions, &numInstructions, (void*)new JuicyTX_DrawInstruction);
        }
        instructions[ctrInstructions]->index = iIndex;
        instructions[ctrInstructions]->rect = *iRect;
        ctrInstructions++;
    }
	return ctrInstructions;
}
/*
JuicyTX_DrawInstruction* JuicyTX::GetLastInstruction() {
	if (instructions != NULL) {
		return instructions[ctrInstructions];
	}
}
*/

void JuicyTX::Draw() {
	if (instructions != NULL) {
		for (JD_INDEX i = 0; i < ctrInstructions; i++) {
			if (instructions[i] != NULL)  {
                JD_INDEX index = (GetTexture(instructions[i]->index) != NULL) ? (instructions[i]->index) : 0;
				SDL_Texture* drawTexture = GetTexture(index);
				host->RenderTexture(drawTexture, &(instructions[i]->rect));
			}
		}
		if (!(flag & JUICYTX_FLAG_NOFLUSH)) {
			flushInstructions();
		}
	}
}

void JuicyTX::init(JD_INDEX iSize) {
	flag = (iSize ? JUICYTX_FLAG_FIXED : 0) | JUICYTX_FLAG_INIT;
	ctrTextures = 0;
	numTextures = iSize;
	textures = NULL;
	ctrInstructions = 0;
	numInstructions = 0;
	instructions = NULL;
}

void JuicyTX::flushInstructions() {
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

void JuicyTX::destroy() {
	destroyInstructions();
	destroyTextures();
}

void JuicyTX::destroyTextures() {
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

void JuicyTX::destroyInstructions() {
	if (instructions != NULL) {
		for (JD_INDEX i = 0; i < numInstructions; i++) {
			if (instructions[i] != NULL) {
				delete instructions[i];
			}
		}
		JDM_DeleteList((void***)&instructions, &numInstructions);
	}
}
