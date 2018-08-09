#include <iostream> // need for srand of color modulator

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"
#include "JuicyGUI_Button.h"
#include "JuicyGUI_Charset.h"


JuicyGUI::JuicyGUI(SDL_Window* iWindow, SDL_Renderer* iRenderer, SDL_Event* iEvent) {
    engine = new JEN(iWindow, iRenderer);
    element.setCredentials(this, this, JUICYGUI_LOWLVL_ROOT_ID_NUM, JUICYGUI_TYPE_ID_GUI);
    element.setRect(engine->GetWindowRect());
    elements = NULL;
    numElement = 0;
    ctrAction = 0;
    numAction = 0;
    events = NULL;
}

JuicyGUI::~JuicyGUI() {
    if (events != NULL) {
        for (JD_INDEX i = 0; i < numAction; i++) {
            if (events[i] != NULL) {
                delete events[i];
            }
        }
        JDM_DeleteList((void***)&events, &numAction);
    }
    if (elements != NULL) {
        for (JD_INDEX i = 0; i < numElement; i++) {
        if (elements[i] != NULL) {
                delete elements[i];
            }
        }
        JDM_DeleteList((void***)&elements, &numElement);
    }
}

bool JuicyGUI::registerElement(JuicyGUI_Element* iElement) {
    if (iElement != NULL) {
        if (iElement->getHost() == this && iElement->getOwner() != NULL) {
            if (!(iElement->isRoot())) {
                JDM_AddToList((void***)&elements, &numElement, (void*)iElement);
                return true;
            }
        }
    }
    return false;
}

JD_FLAG JuicyGUI::UpdateState(JuicyGUI_Event*** oEvent, JD_INDEX* numElements) {
    element.setAction(JUICYGUI_ACTION_NONE);
    if (engine->LoopHandle()) {
        JD_Point screenSize;
        engine->GetWindowSize(&screenSize);
        element.setSize(&screenSize);
        element.attachAction(JUICYGUI_ACTION_RESIZE);
    }

    updateMouse();

    ctrAction = 0;
    for (JD_INDEX i = 0; i < (1 + numElement); i++) {
        if (i ? evaluateElement(elements[i-1]) : (element.getAction() != JUICYGUI_ACTION_NONE)) {
            if (ctrAction >= numAction) {
                JDM_AddToList((void***)&events, &numAction, (void*)new JuicyGUI_Event);
            }
            events[ctrAction]->id = i ? (elements[i-1]->getID()) : element.getID();
            events[ctrAction]->action = i ? (elements[i-1]->getAction()) : element.getAction();
            ctrAction++;
        }
    }
    if (numElements != NULL) {*numElements = ctrAction;}
    if (oEvent != NULL) {*oEvent = events;}
    return element.getAction();
}


void JuicyGUI::GetScreenSize(JD_Point* oSize) {
    if (oSize != NULL) element.getSize(oSize);
}
JD_I JuicyGUI::GetScreenSizeBigger(void) {
    return (element.getWidth() > element.getHeight()) ? element.getWidth() : element.getHeight();
}
JD_I JuicyGUI::GetScreenSizeSmaller(void) {
    return (element.getWidth() > element.getHeight()) ? element.getHeight() : element.getWidth();
}

void JuicyGUI::DrawElements(void) {
    for (JD_INDEX i = 0; i < numElement; i++) {
        elements[i]->draw();
    }
}

bool JuicyGUI::evaluateElement(JuicyGUI_Element* iElement) {
    JD_FLAG state = JUICYGUI_ACTION_NONE;
    if (!(iElement->getFlag() & JUICYGUI_ELEMENTFLAG_DISABLED) && (iElement->getFlag() & JUICYGUI_ELEMENTFLAG_SHOW)) {
        if (mouseOver(iElement->getRect())) { // Mouse over?
            state |= JUICYGUI_ACTION_HOVER;
            state |= (mouseState & JUICYGUI_CONTROL_ID_LMB) ? JUICYGUI_ACTION_PRESSED : JUICYGUI_ACTION_NONE;
            state |= (mouseState & JUICYGUI_CONTROL_ID_LMB_RELEASED) ? JUICYGUI_ACTION_RELEASED : JUICYGUI_ACTION_NONE;
        }
    }
    if (iElement->getAction() != state) {
        iElement->setAction(state);
        iElement->updateStatic();
        return true;
    }
    return false;
}

JD_FLAG JuicyGUI::GetMousePos(JD_Point* oPosition) {
    if (oPosition != NULL) {
        *oPosition = mousePos;
    }
    return mouseState;
}
JD_FLAG JuicyGUI::GetMouseMovement(JD_Point* oMovement) {
    if (oMovement != NULL) {
        *oMovement = mouseMove;
    }
    return mouseState;
}

void JuicyGUI::updateMouse() {
    JD_FLAG newMouseState;
    JD_FLAG released;
    JD_FLAG clicked;
    JD_Point newMousePos;
    newMouseState = SDL_GetMouseState(&newMousePos.x, &newMousePos.y);
    clicked = ((newMouseState ^ (mouseState & 0xf)) & newMouseState) << 4;
    released = ((newMouseState ^ (mouseState & 0xf)) & mouseState) << 8;
    mouseState = newMouseState | clicked | released;
    mouseMove = newMousePos - mousePos;
    mousePos = newMousePos;
}

bool JuicyGUI::mouseOver(const JD_Rect* iRect) {
    if (mousePos.y >= iRect->y) {
        if (mousePos.y <= iRect->y + iRect->h) {
            if (mousePos.x >= iRect->x) {
                if (mousePos.x <= iRect->x + iRect->w) {
                    return true;
                }
            }
        }
    }
    return false;
}



JD_COLOR JuicyGUI::GetJuicy(void) {
    JD_TIME tmil_now = engine->GetTicks();
    static JD_TIME tmil_last = tmil_now;
    JD_COLOR currentColor = 0;
    static JD_COLOR lastColor = ((rand() % 0x100) << 24) | ((rand() % 0x100) << 16) | ((rand() % 0x100) << 8) | 0xff;
    static JD_COLOR targetColor = ((rand() % 0x100) << 24) | ((rand() % 0x100) << 16) | ((rand() % 0x100) << 8) | 0xff;
    while ((tmil_now - tmil_last) >= JUICYGUI_COLOR_MOD_PERIOD) {
        lastColor = targetColor;
        targetColor = 0;
        for (JD_INDEX i = 0; i < 3; i++) {
            JD_COLOR cache = rand() % 0x100;
            cache = (cache < JUICYGUI_COLOR_MOD_MIN) ? JUICYGUI_COLOR_MOD_MIN : (cache > JUICYGUI_COLOR_MOD_MAX) ? JUICYGUI_COLOR_MOD_MAX : cache;
            targetColor |= (cache & 0xff);
            targetColor <<= 8;
        }
        targetColor |= 0xff;
        tmil_last += JUICYGUI_COLOR_MOD_PERIOD;
    }
    for (JD_INDEX i = 0; i < 3; i++) {
        JD_I last = (lastColor >> (i * 8 + 8)) & 0xff;
        JD_I target = (targetColor >> (i * 8 + 8)) & 0xff;
        JD_I current = last + (((target - last) * JD_I(tmil_now - tmil_last)) / JUICYGUI_COLOR_MOD_PERIOD); // Must use INT
        currentColor |= (current & 0xff);
        currentColor <<= 8;
    }
    currentColor |= 0xff;
    return currentColor;
}
