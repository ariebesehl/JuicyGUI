#ifndef JSPR_H_INCLUDED
#define JSPR_H_INCLUDED

#include "JuicyGUI_Definitions.h"

#define JSPR_MAX_DRAW_NUM 0x1000

#define JSPR_FLAG_INIT 0x0
#define JSPR_FLAG_FIXED 0x1
#define JSPR_FLAG_NOFLUSH 0x2

typedef struct {
	JD_INDEX index;
	JD_Rect rect;
} JSPR_DrawInstruction;


class JSPR {
	public:
		JSPR(JEN* iEngine);
		JSPR(JEN* iEngine, JD_INDEX iSize);
		~JSPR();
		SDL_Texture* GetTexture(JD_INDEX iIndex);
		JD_INDEX AddTexture(SDL_Texture* iTexture);
		void AddSpecificTexture(JD_INDEX iIndex, SDL_Texture* iTexture);
		void Reset(void);
		JD_INDEX AddInstruction(JD_INDEX iIndex, const JD_Rect* iRect);
		//JSPR_DrawInstruction* GetLastInstruction();
		void ClearInstructions() {flushInstructions();};
		void RepeatDraw(void) {RepeatDraw(true);};
		void RepeatDraw(bool iRepeat) {if (iRepeat) flag |= JSPR_FLAG_NOFLUSH; else flag &= ~JSPR_FLAG_NOFLUSH;};
		void SingleDraw(void) {RepeatDraw(false);};
		void Draw(void);
	private:
		JEN* engine;
		JD_FLAG flag;

		void init(JD_INDEX iSize);
		void destroy(void);

		JD_INDEX ctrTextures;
		JD_INDEX numTextures;
		SDL_Texture** textures;
		void destroyTextures();

		JD_INDEX ctrInstructions;
		JD_INDEX numInstructions;
		JSPR_DrawInstruction** instructions;
		void flushInstructions();
		void destroyInstructions();
};


#endif // JSPR_H_INCLUDED
