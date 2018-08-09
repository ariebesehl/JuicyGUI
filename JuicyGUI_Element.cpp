
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Element.h"

JuicyGUI_Element::JuicyGUI_Element() {
    root = false;
    registered = false;
    host = NULL;
    owner = NULL;
    engine = NULL;
    textureEngine = NULL;
    id = JUICYGUI_LOWLVL_EMPTY_ID_NUM;
    rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
    rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
    rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
    rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    type = JUICYGUI_TYPE_ID_NONE;
    flag = JUICYGUI_ELEMENTFLAG_INIT;
    action = JUICYGUI_ACTION_NONE;
}

JuicyGUI_Element::JuicyGUI_Element(JuicyGUI* iHostUI, void* iOwner, JD_INDEX iID, JD_INDEX iType, const JD_Rect* iDimensions) {
    setCredentials(iHostUI, iOwner, iID, iType);
    if (iDimensions != NULL) {
        rect = *iDimensions;
    } else {
        rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
        rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
        rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
        rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;
    }
}

JuicyGUI_Element::~JuicyGUI_Element(void) {}


void JuicyGUI_Element::setCredentials(JuicyGUI* iHostUI, void* iOwner, JD_INDEX iID, JD_INDEX iType) {
    host = iHostUI;
    owner = iOwner;
    root = (owner != NULL && host == owner);
    id = iID;
    type = iType;
    flag = JUICYGUI_ELEMENTFLAG_INIT | ((type & JUICYGUI_ID_STATIC_TYPES) ? JUICYGUI_ELEMENTFLAG_STATIC : 0x0);
    action = JUICYGUI_ACTION_NONE;
    JDM_EmptyRect(&rect);
    /*rect.x = JUICYGUI_LOWLVL_EMPTY_POS_X;
    rect.y = JUICYGUI_LOWLVL_EMPTY_POS_Y;
    rect.w = JUICYGUI_LOWLVL_EMPTY_WIDTH;
    rect.h = JUICYGUI_LOWLVL_EMPTY_HEIGHT;*/
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

JD_INDEX JuicyGUI_Element::getActionIndex() {
    return getActionIndex(action);
}
JD_INDEX JuicyGUI_Element::getActionIndex(JD_FLAG iAction) {
    for (JD_INDEX index = (JUICYGUI_NUM_ACTION - 1); index > 0; index--) {
        if (iAction & (1 << (index - 1))) return index;
    }
    return 0;
}
void JuicyGUI_Element::updateStatic() {
    if (flag & JUICYGUI_ELEMENTFLAG_STATIC) {
        if (textureEngine != NULL) {
            textureEngine->ClearInstructions();
            textureEngine->AddInstruction(getActionIndex(), &rect);
            textureEngine->RepeatDraw(true);
        }
    }
}

void JuicyGUI_Element::draw() {
	if (textureEngine != NULL) {
		if (getFlag() & JUICYGUI_ELEMENTFLAG_SHOW) {
            textureEngine->Draw();
		}
	}
}

