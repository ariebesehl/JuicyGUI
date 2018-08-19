
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"

JuicyGUI_Element::JuicyGUI_Element() {
    root = false;
    registered = false;
    host = NULL;
    owner = NULL;
    properties = NULL;
    engine = NULL;
    sprites = NULL;
    id = JUICYGUI_LOWLVL_EMPTY_ID_NUM;
    rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
    rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
    rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
    rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    type = JUICYGUI_TYPE_ID_NONE;
    flag = JUICYGUI_ELEMENTFLAG_INIT;
    event = JUICYGUI_EVENT_NONE;
}

JuicyGUI_Element::JuicyGUI_Element(JuicyGUI* iHostUI, void* iOwner, JPM* iProperties, JSPR* iSprites, JD_INDEX iID, JD_INDEX iType, const JD_Rect* iDimensions) {
    setCredentials(iHostUI, iOwner, iProperties, iSprites, iID, iType);
    if (iDimensions != NULL) {
        rect = *iDimensions;
    } else {
        rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
        rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
        rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
        rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    }
}

JuicyGUI_Element::~JuicyGUI_Element(void) {
    if (sprites != NULL) {delete sprites;}
    if (properties != NULL) {delete properties;}
}


void JuicyGUI_Element::setCredentials(JuicyGUI* iHostUI, void* iOwner, JPM* iProperties, JSPR* iSprites, JD_INDEX iID, JD_INDEX iType) {
    host = iHostUI;
    owner = iOwner;
    root = (owner != NULL && host == owner);
    properties = iProperties;
    sprites = iSprites;
    id = iID;
    type = iType;
    flag = JUICYGUI_ELEMENTFLAG_INIT | ((type & JUICYGUI_ID_STATIC_TYPES) ? JUICYGUI_ELEMENTFLAG_STATIC : 0x0);
    event = JUICYGUI_EVENT_NONE;
    JDM_SetRectPos(&rect, JUICYGUI_LOWLVL_EMPTY_POS_X, JUICYGUI_LOWLVL_EMPTY_POS_Y);
    JDM_SetRectSize(&rect, JUICYGUI_LOWLVL_EMPTY_WIDTH, JUICYGUI_LOWLVL_EMPTY_HEIGHT);
    registered = (host != NULL) ? host->registerElement(this) : false;
    if (registered) {engine = host->GetEngine();}
}

bool JuicyGUI_Element::setRect(const JD_Rect* iRect) {
    bool sizeChange = false;
    if (iRect != NULL) {
        sizeChange = (rect.w != iRect->w || rect.h != iRect->h);
        rect = *iRect;
    }
    return sizeChange;
}

void JuicyGUI_Element::UpdateSprites() {
    if (engine != NULL && sprites != NULL) {
        if (flag & JUICYGUI_ELEMENTFLAG_STATIC) {
            sprites->Clear();
            sprites->AddInstruction(JPM_GetEventIndex(event), &rect);
            sprites->RepeatDraw(true);
        }
    }
}

void JuicyGUI_Element::draw() {
	if (engine != NULL && sprites != NULL) {
		if (getFlag() & JUICYGUI_ELEMENTFLAG_SHOW) {
            sprites->Draw();
		}
	}
}

