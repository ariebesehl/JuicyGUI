#ifndef JUICYGUI_ELEMENT_H_INCLUDED
#define JUICYGUI_ELEMENT_H_INCLUDED

#include "JuicyGUI_Definitions.h"

class JSPR;
class JuicyGUI;

class JuicyGUI_Element {
    public:
        JuicyGUI_Element();
        JuicyGUI_Element(JuicyGUI* iHostUI, void* iOwner, JD_INDEX iID, JD_INDEX iType, const JD_Rect* iDimensions);
        ~JuicyGUI_Element(void);
        void setCredentials(JuicyGUI* iHostUI, void* iOwner, JD_INDEX iID, JD_INDEX iType);
        JEN* getEngine() {return engine;};
        JuicyGUI* getHost(void) {return host;};
        void* getOwner(void) {return owner;};
        bool isRoot(void) {return root;};
        JD_INDEX getID(void) {return id;};
        JD_INDEX getType(void) {return type;};
        JD_FLAG getFlag(void) {return flag;};
        JD_FLAG setFlag(JD_FLAG iFlag) {flag = iFlag; return getFlag();};
        JD_FLAG attachFlag(JD_FLAG iFlag) {flag |= iFlag; return getFlag();};
        JD_FLAG detachFlag(JD_FLAG iFlag) {flag &= ~iFlag; return getFlag();};
        JD_FLAG getAction(void) {return action;};
        JD_FLAG setAction(JD_FLAG iAction) {action = iAction; return getAction();};
        JD_FLAG attachAction(JD_FLAG iAction) {action |= iAction; return getAction();};
        JD_FLAG detachAction(JD_FLAG iAction) {action &= ~iAction; return getAction();};
        void show() {attachFlag(JUICYGUI_ELEMENTFLAG_SHOW);};
        void hide() {detachFlag(JUICYGUI_ELEMENTFLAG_SHOW);};
        void show(bool iShow) {if(iShow) {attachFlag(JUICYGUI_ELEMENTFLAG_SHOW);} else {detachFlag(JUICYGUI_ELEMENTFLAG_SHOW);}};
        void enable() {detachFlag(JUICYGUI_ELEMENTFLAG_DISABLED);};
        void disable() {attachFlag(JUICYGUI_ELEMENTFLAG_DISABLED);};
        void enable(bool iEnabled) {if(iEnabled) {detachFlag(JUICYGUI_ELEMENTFLAG_DISABLED);} else {attachFlag(JUICYGUI_ELEMENTFLAG_DISABLED);}};
        const JD_Rect* getRect(void) {return &rect;};
        bool setRect(const JD_Rect* iRect);
        void getPos(JD_Point* oPosition) {if (oPosition != NULL) oPosition->x = rect.x; oPosition->y = rect.y;};
        void setPos(const JD_Point* iPosition) {if (iPosition != NULL) rect.x = iPosition->x; rect.y = iPosition->y;};
        JD_I getPosX(void) {return rect.x;};
        JD_I getPosY(void) {return rect.y;};
        void setPosX(JD_I iPosX) {rect.x = iPosX;};
        void setPosY(JD_I iPosY) {rect.y = iPosY;};
        void getSize(JD_Point* oSize) {if (oSize != NULL) oSize->x = rect.w; oSize->y = rect.h;};
        void setSize(const JD_Point* iSize) {if (iSize != NULL) rect.w = iSize->x; rect.h = iSize->y;};
        void setWidth(JD_I iWidth) {rect.w = iWidth;};
        void setHeight(JD_I iHeight) {rect.h = iHeight;};
        JD_I getWidth(void) {return rect.w;};
        JD_I getHeight(void) {return rect.h;};

		void SetTextureEngine(JSPR* iEngine) {textureEngine = iEngine; updateStatic();};
		void draw();
        JD_INDEX getActionIndex();
        JD_INDEX getActionIndex(JD_FLAG iActionID);
        void updateStatic();
    private:
        bool root;
        bool registered;
        JuicyGUI* host;
        JEN* engine;
		JSPR* textureEngine;
        void* owner;
        JD_INDEX id;
        JD_INDEX type;
        JD_FLAG flag;
        JD_FLAG action;
        JD_Rect rect;
};

#endif // JUICYGUI_ELEMENT_H_INCLUDED
