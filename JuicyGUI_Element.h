#ifndef JUICYGUI_ELEMENT_H_INCLUDED
#define JUICYGUI_ELEMENT_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "JuicyGUI_Definitions.h"

class JuicyGUI;

class JuicyGUI_Element {
    public:
        JuicyGUI_Element();
        JuicyGUI_Element(JuicyGUI* iHostUI, void* iOwner, uint32_t iID, uint32_t iType, const SDL_Rect* iDimensions);
        ~JuicyGUI_Element(void);
        void setCredentials(JuicyGUI* iHostUI, void* iOwner, uint32_t iID, uint32_t iType);
        JuicyGUI* getHost(void) {return _host;};
        void* getOwner(void) {return _owner;};
        bool isRoot(void) {return _isRoot;};
        uint32_t getID(void) {return _id;};
        uint32_t getType(void) {return _type;};
        uint32_t getFlag(void) {return _flag;};
        uint32_t setFlag(uint32_t iFlag) {_flag = iFlag; return getFlag();};
        uint32_t attachFlag(uint32_t iFlag) {_flag |= iFlag; return getFlag();};
        uint32_t detachFlag(uint32_t iFlag) {_flag &= ~iFlag; return getFlag();};
        uint32_t getAction(void) {return _action;};
        uint32_t setAction(uint32_t iAction) {_action = iAction; return getAction();};
        uint32_t attachAction(uint32_t iAction) {_action |= iAction; return getAction();};
        uint32_t detachAction(uint32_t iAction) {_action &= ~iAction; return getAction();};
        void show() {attachAction(JUICYGUI_ELEMENTFLAG_SHOW);};
        void hide() {detachAction(JUICYGUI_ELEMENTFLAG_SHOW);};
        void show(bool iShow) { if(iShow) detachAction(JUICYGUI_ELEMENTFLAG_SHOW); else attachAction(JUICYGUI_ELEMENTFLAG_SHOW);};
        void enable() {detachAction(JUICYGUI_ELEMENTFLAG_DISABLED);};
        void disable() {attachAction(JUICYGUI_ELEMENTFLAG_DISABLED);};
        void enable(bool iEnabled) { if(iEnabled) detachAction(JUICYGUI_ELEMENTFLAG_DISABLED); else attachAction(JUICYGUI_ELEMENTFLAG_DISABLED);};
        const SDL_Rect* getRect(void) {return &_rect;};
        bool setRect(const SDL_Rect* iRect);
        void getPos(SDL_Point* oPosition) {if (oPosition != NULL) oPosition->x = _rect.x; oPosition->y = _rect.y;};
        void setPos(const SDL_Point* iPosition) {if (iPosition != NULL) _rect.x = iPosition->x; _rect.y = iPosition->y;};
        int getPosX(void) {return _rect.x;};
        int getPosY(void) {return _rect.y;};
        void setPosX(int iPosX) {_rect.x = iPosX;};
        void setPosY(int iPosY) {_rect.y = iPosY;};
        void getSize(SDL_Point* oSize) {if (oSize != NULL) oSize->x = _rect.w; oSize->y = _rect.h;};
        void setSize(const SDL_Point* iSize) {if (iSize != NULL) _rect.w = iSize->x; _rect.h = iSize->y;};
        void setWidth(int iWidth) {_rect.w = iWidth;};
        void setHeight(int iHeight) {_rect.h = iHeight;};
        int getWidth(void) {return _rect.w;};
        int getHeight(void) {return _rect.h;};
    private:
        bool _isRoot;
        bool _isRegistered;
        JuicyGUI* _host;
        void* _owner;
        uint32_t _id;
        uint32_t _type;
        uint32_t _flag;
        uint32_t _action;
        SDL_Rect _rect;
};

#endif // JUICYGUI_ELEMENT_H_INCLUDED
