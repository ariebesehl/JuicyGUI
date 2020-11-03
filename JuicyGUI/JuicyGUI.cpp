#include <iostream> // need for srand of color modulator

#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"
#include "JuicyGUI_Button.h"
#include "JuicyGUI_Charset.h"


JuicyGUI::JuicyGUI(SDL_Window* iWindow, SDL_Renderer* iRenderer) {
    engine = new JEN(iWindow, iRenderer);
    element.setCredentials(this, this, NULL, NULL, JUICYGUI_LOWLVL_ROOT_ID_NUM, JUICYGUI_TYPE_ID_GUI);
    element.setRect(engine->GetWindowRect());
    elements = NULL;
    numElement = 0;
    ctrEvents = 0;
    numEvents = 0;
    msFocusElement = JUICYGUI_LOWLVL_EMPTY_ID_NUM;
    events = NULL;
    eventsFlushed = true;
}

JuicyGUI::~JuicyGUI() {
    if (events != NULL) {
        for (JD_INDEX i = 0; i < numEvents; i++) {if (events[i] != NULL) {delete events[i];}}
        JDM_DeleteList((void***)&events, &numEvents);
    }
    if (elements != NULL) {
        for (JD_INDEX i = 0; i < numElement; i++) {
            if (elements[i] != NULL) {
                switch (elements[i]->getType()) {
                    case JUICYGUI_TYPE_ID_BOX: delete (JuicyGUI_Box*)(elements[i]->getOwner()); break;
                    case JUICYGUI_TYPE_ID_CHARSET: delete (JuicyGUI_Charset*)(elements[i]->getOwner()); break;
                    case JUICYGUI_TYPE_ID_BUTTON: delete (JuicyGUI_Button*)(elements[i]->getOwner()); break;
                    default: break;
                }
            }
        }
        JDM_DeleteList((void***)&elements, &numElement);
    }
    if (engine != NULL) {delete engine;}
}

bool JuicyGUI::registerElement(JuicyGUI_Element* iElement) {
    if (iElement != NULL) {
        if (iElement->getHost() == this && iElement->getOwner() != NULL) {
            if (!(iElement->isRoot())) {
                JDM_AddToList((void***)&elements, numElement, (void*)iElement);
                numElement++;
                return true;
            }
        }
    }
    return false;
}

void JuicyGUI::updateEvents() {
    static JD_INDEX numIteration = 0;

    element.setEvent(JUICYGUI_EVENT_NONE);
    if (engine->LoopHandle() && numIteration) {
        JD_Point screenSize;
        engine->GetWindowSize(&screenSize);
        element.setSize(&screenSize);
        element.attachEvent(JUICYGUI_EVENT_RESIZE);
    }

    if (!updateMouse()) {msFocusElement = JUICYGUI_LOWLVL_EMPTY_ID_NUM;}

    ctrEvents = 0;
    for (JD_INDEX i = 0; i < (1 + numElement); i++) {
        if (i ? evaluateElement(elements[numElement - i]) : (element.getEvent() != JUICYGUI_EVENT_NONE)) {
            if (ctrEvents >= numEvents) {
                JDM_AddToList((void***)&events, numEvents, (void*)new JuicyGUI_Event);
                numEvents++;
            }
            events[ctrEvents]->id = i ? (elements[numElement - i]->getID()) : element.getID();
            events[ctrEvents]->type = i ? (elements[numElement - i]->getEvent()) : element.getEvent();
            ctrEvents++;
        }
    }

    numIteration++;
}
bool JuicyGUI::PollEvents(JuicyGUI_Event* oEvent) {
    if (eventsFlushed) {
        eventsFlushed = false;
        updateEvents();
    }
    if (ctrEvents) {
        if (oEvent != NULL) {
            *oEvent = *(events[ctrEvents - 1]);
        }
        ctrEvents--;
        return true;
    } else {
        eventsFlushed = true;
        return false;
    }
}

void JuicyGUI::GetScreenSize(JD_Point* oSize) {if (oSize != NULL) element.getSize(oSize);}
JD_I JuicyGUI::GetScreenSizeBigger(void) {return (element.getWidth() > element.getHeight()) ? element.getWidth() : element.getHeight();}
JD_I JuicyGUI::GetScreenSizeSmaller(void) {return (element.getWidth() > element.getHeight()) ? element.getHeight() : element.getWidth();}

void JuicyGUI::DrawElements(void) {
    for (JD_INDEX i = 0; i < numElement; i++) {elements[i]->draw();}
    engine->Present();
}

bool JuicyGUI::evaluateElement(JuicyGUI_Element* iElement) {
    JD_FLAG currentEvent = JUICYGUI_EVENT_NONE;
    if (!(iElement->getFlag() & JUICYGUI_ELEMENTFLAG_DISABLED) && (iElement->getFlag() & JUICYGUI_ELEMENTFLAG_SHOW)) {
        if (mouseOver(iElement->getRect())) { // Mouse over?
            currentEvent |= JUICYGUI_EVENT_HOVER;
            if (mouseState) {
                if (msFocusElement == JUICYGUI_LOWLVL_EMPTY_ID_NUM && mouseState & JUICYGUI_CONTROL_ID_LMB_CLICKED) {msFocusElement = iElement->getID();}
                if (msFocusElement == iElement->getID()) {
                    currentEvent |= (mouseState & JUICYGUI_CONTROL_ID_LMB) ? JUICYGUI_EVENT_PRESSED : JUICYGUI_EVENT_NONE;
                    currentEvent |= (mouseState & JUICYGUI_CONTROL_ID_LMB_CLICKED) ? JUICYGUI_EVENT_CLICKED : JUICYGUI_EVENT_NONE;
                    currentEvent |= (mouseState & JUICYGUI_CONTROL_ID_LMB_RELEASED) ? JUICYGUI_EVENT_RELEASED : JUICYGUI_EVENT_NONE;
                }
            }
        }
    }
    if (iElement->getEvent() != currentEvent) {
        iElement->setEvent(currentEvent);
        iElement->UpdateSprites();
    }
    return (iElement->getEvent() != JUICYGUI_EVENT_NONE);
}

JD_FLAG JuicyGUI::GetMousePos(JD_Point* oPosition) {
    if (oPosition != NULL) {*oPosition = mousePos;}
    return mouseState;
}
JD_FLAG JuicyGUI::GetMouseMovement(JD_Point* oMovement) {
    if (oMovement != NULL) {*oMovement = mouseMove;}
    return mouseState;
}

JD_FLAG JuicyGUI::updateMouse() {
    JD_FLAG newMouseState;
    JD_FLAG released;
    JD_FLAG clicked;
    JD_Point newMousePos;
    newMouseState = engine->GetMouseState(&newMousePos);
    clicked = ((newMouseState ^ (mouseState & 0xf)) & newMouseState) << 4;
    released = ((newMouseState ^ (mouseState & 0xf)) & mouseState) << 8;
    mouseState = newMouseState | clicked | released;
    mouseMove = newMousePos - mousePos;
    mousePos = newMousePos;
    return mouseState;
}

bool JuicyGUI::mouseOver(const JD_Rect* iRect) {return JDM_RectContains(iRect, &mousePos);}

JD_COLOR JuicyGUI::GetJuicyColor(void) {
    JD_COLOR juicy = 0;
    for (JD_INDEX i = 0; i < 3; i++) {
        JD_COLOR cache = rand() % 0x100;
        cache = (cache < JUICYGUI_COLOR_MOD_MIN) ? (JUICYGUI_COLOR_MOD_MAX - (JUICYGUI_COLOR_MOD_MIN - cache)) : (cache > JUICYGUI_COLOR_MOD_MAX) ? (JUICYGUI_COLOR_MOD_MIN + (cache - JUICYGUI_COLOR_MOD_MAX)) : cache;
        juicy |= (cache & 0xff);
        juicy <<= 8;
    }
    juicy |= 0xff;
    return juicy;
}

JD_COLOR JuicyGUI::GetJuicy(void) {
    JD_TIME tmil_now = engine->GetTicks();
    static JD_TIME tmil_last = tmil_now;
    JD_COLOR currentColor = 0;
    static JD_COLOR lastColor = GetJuicyColor();
    static JD_COLOR targetColor = GetJuicyColor();
    while ((tmil_now - tmil_last) >= JUICYGUI_COLOR_MOD_PERIOD) {
        lastColor = targetColor;
        targetColor = GetJuicyColor();
        tmil_last += JUICYGUI_COLOR_MOD_PERIOD;
    }
    JD_I timeDelta = (tmil_now - tmil_last);
    for (JD_INDEX i = 0; i < 3; i++) {
        JD_I last = (lastColor >> (i * 8 + 8)) & 0xff;
        JD_I target = (targetColor >> (i * 8 + 8)) & 0xff;
        JD_I current = last + (((target - last) * timeDelta) / JUICYGUI_COLOR_MOD_PERIOD); // Must use INT
        currentColor |= (current & 0xff);
        currentColor <<= 8;
    }
    currentColor |= 0xff;
    return currentColor;
}
