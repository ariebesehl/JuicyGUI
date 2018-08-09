#ifndef JUICYTX_H_INCLUDED
#define JUICYTX_H_INCLUDED

#include "JuicyGUI_Definitions.h"

#define JUICYTX_MAX_DRAW_NUM 0x1000

#define JUICYTX_FLAG_INIT 0x0
#define JUICYTX_FLAG_FIXED 0x1
#define JUICYTX_FLAG_NOFLUSH 0x2

typedef struct {
	JD_INDEX index;
	JD_Rect rect;
} JuicyTX_DrawInstruction;

class JuicyGUI;

class JuicyTX {
	public:
		JuicyTX(JuicyGUI* iHost);
		JuicyTX(JuicyGUI* iHost, JD_INDEX iSize);
		~JuicyTX();
		SDL_Texture* GetTexture(JD_INDEX iIndex);
		JD_INDEX AddTexture(SDL_Texture* iTexture, JD_Point* iSize);
		void AddSpecificTexture(JD_INDEX iIndex, SDL_Texture* iTexture, JD_Point* iSize);
		void Reset(void);
		JD_INDEX AddInstruction(JD_INDEX iIndex, const JD_Rect* iRect);
		//JuicyTX_DrawInstruction* GetLastInstruction();
		void ClearInstructions() {flushInstructions();};
		void RepeatDraw(void) {RepeatDraw(true);};
		void RepeatDraw(bool iRepeat) {if (iRepeat) flag |= JUICYTX_FLAG_NOFLUSH; else flag &= ~JUICYTX_FLAG_NOFLUSH;};
		void SingleDraw(void) {RepeatDraw(false);};
		void Draw(void);
	private:
		JuicyGUI* host;
		JuicyGUI_Flag flag;

		void init(JD_INDEX iSize);
		void destroy(void);

		JD_INDEX ctrTextures;
		JD_INDEX numTextures;
		SDL_Texture** textures;
		void destroyTextures();

		JD_INDEX ctrInstructions;
		JD_INDEX numInstructions;
		JuicyTX_DrawInstruction** instructions;
		void flushInstructions();
		void destroyInstructions();
};


#endif // JUICYTEX_H_INCLUDED
