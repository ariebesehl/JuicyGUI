
#include "Juicy_SDL.h"

#include "JuicyGUI.h"
#include "JuicyGUI_Element.h"
#include "JuicyGUI_Definitions.h"

JuicyGUI_Element::JuicyGUI_Element() {
    _isRoot = false;
    _isRegistered = false;
    _host = NULL;
    _owner = NULL;
    _id = JUICYGUI_LOWLVL_EMPTY_ID_NUM;
    _rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
    _rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
    _rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
    _rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    _type = JUICYGUI_TYPE_ID_NONE;
    _flag = JUICYGUI_ELEMENTFLAG_INIT;
    _action = JUICYGUI_ACTION_NONE;
}

JuicyGUI_Element::JuicyGUI_Element(JuicyGUI* iHostUI, void* iOwner, uint32_t iID, uint32_t iType, const SDL_Rect* iDimensions) {
    setCredentials(iHostUI, iOwner, iID, iType);
    if (iDimensions != NULL) {
        _rect = *iDimensions;
    } else {
        _rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
        _rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
        _rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
        _rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    }
}

JuicyGUI_Element::~JuicyGUI_Element(void) {}


void JuicyGUI_Element::setCredentials(JuicyGUI* iHostUI, void* iOwner, uint32_t iID, uint32_t iType) {
    _host = iHostUI;
    _owner = iOwner;
    _isRoot = (_owner != NULL && _host == _owner);
    _id = iID;
    _type = iType;
    _flag = JUICYGUI_ELEMENTFLAG_INIT;
    _action = JUICYGUI_ACTION_NONE;
    _rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
    _rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
    _rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
    _rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    _isRegistered = (_host != NULL) ? _host->RegisterElement(this) : false;
}

bool JuicyGUI_Element::setRect(const SDL_Rect* iRect) {
    bool sizeChange = false;
    if (iRect != NULL) {
        sizeChange = (_rect.w != iRect->w || _rect.h != iRect->h);
        _rect = *iRect;
    }
    return sizeChange;
}
