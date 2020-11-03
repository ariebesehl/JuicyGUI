
#include "../JuicyGUI/Juicy.h"

#define JUICYTE_DEFAULT_ZOOM_STAGES 4
#define JUICYTE_DEFAULT_BASE_SIZE 1
#define JUICYTE_DEFAULT_GRID_SIZE 1
#define JUICYTE_DEFAULT_GRID_COLOR 0xffffff7f
#define JUICYTE_DEFAULT_EDGE_SIZE 1
#define JUICYTE_DEFAULT_EDGE_COLOR 0xffffffff

#define JUICYTE_FLAG_NONE 0x0
#define JUICYTE_FLAG_READY 0x1
#define JUICYTE_FLAG_GRID 0x4
#define JUICYTE_FLAG_EDGE 0x8
#define JUICYTE_FLAG_GFX_LOADED 0x10
#define JUICYTE_FLAG_INIT (JUICYTE_FLAG_NONE | JUICYTE_FLAG_EDGE)

typedef struct {
	JD_INDEX numSprite;
	JuicySprite** ptrSprite;
} JuicyTE_Type;

class JuicyTE {
	public:
	    JuicyTE(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_FLAG iFlag);
		JuicyTE(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_Point* iSizeBase, JD_I iZoomLevels, JD_FLAG iFlag);
		~JuicyTE();
		void LoopHandle() {engine->LoopHandle();};
		void Draw();
		void AddType(JD_INDEX iNum, JuicySprite** iSprite);
		void ClearTypes();
		void MoveCam(const JD_Point* iMove);
		void CenterCam();
		void SetCam(const JD_Point* iPos);
		JD_I Zoom(JD_I iAmount);
		JD_I GetZoom() {return zoom;};
		void SetZoom(JD_INDEX iLevel);
		void SetGrid(bool iGrid) {if (iGrid) {flag |= JUICYTE_FLAG_GRID;} else {flag &= ~JUICYTE_FLAG_GRID;}};
		void ToggleGrid() {flag ^= JUICYTE_FLAG_GRID;};
		void SetEdge(bool iGrid) {if (iGrid) {flag |= JUICYTE_FLAG_EDGE;} else {flag &= ~JUICYTE_FLAG_EDGE;}};
		void ToggleEdge() {flag ^= JUICYTE_FLAG_EDGE;};
		void SetData(void** iData);
		void SetData(void** iData, JD_FLAG iMask, JD_I iShift);
		void IdentifyCell(const JD_Point* iScreenPos, JD_Point* oCell);
		JD_INDEX GetFPS() {return engine->GetFPS();};
	private:
	    void init(JuicyGUI* iHostUI, JD_Point* iSizeField, JD_Point* iSizeBase, JD_I iZoomLevels, JD_FLAG iFlag);
		JEN* engine;
		JuicyGUI* host;
		JD_FLAG flag;
		JD_INDEX numTypes;
		JuicyTE_Type** types;

		void** data;
		JD_FLAG dataMask;
		JD_I dataShift;

		JD_INDEX zoomStages;
		JD_I zoom;
		JD_Point sizeField;
		JD_Point sizeBase;
		JD_Point sizeCam;
		JD_Point sizeScreen;
		JD_Point posCam;
		JD_Point drawSize;
		JD_Point drawOffset;
		JD_Point drawStart;
		JD_Point drawStop;

		void checkReady();
		JuicySprite* mapSprite;
		JD_I gridSize;
		JD_COLOR gridColor;
		JuicySprite* gridC;
		JuicySprite** gridH;
		JuicySprite** gridV;
		JD_I edgeSize;
		JD_COLOR edgeColor;
		JuicySprite* edgeC;
		JuicySprite** edgeH;
		JuicySprite** edgeV;
		void gfxLoad();
		void gfxReload();
		void gfxUnload();
        void drawMetrics();
        void drawGrid(const JD_Point* iPos, JD_FLAG iConfig);
        void drawEdge(const JD_Point* iPos, JD_FLAG iConfig);
};
