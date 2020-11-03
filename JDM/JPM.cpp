
#include "JPM.h"

JPM::JPM() {
    init(0, NULL);
}
JPM::JPM(void* iDefault) {
    init(0, iDefault);
}
JPM::JPM(JD_INDEX iNumExtras) {
    init(iNumExtras, NULL);
}
JPM::JPM(JD_INDEX iNumExtras, void* iDefault) {
    init(iNumExtras, iDefault);
}
void JPM::init(JD_INDEX iNumExtras, void* iDefault) {
    indexDefault = JPM_GetEventIndex(JUICYGUI_EVENT_NONE);
    ctrPoll = 0;
    numProperties = JUICYGUI_NUM_EVENT;
    numExtras = iNumExtras;
    numTotal = numProperties + numExtras;
    properties = new void*[numTotal];
    JDM_NullList(properties, numTotal);
    properties[indexDefault] = iDefault;
}

JPM::~JPM() {
    if (properties != NULL) {
        delete[] properties;
    }
}
bool JPM::PollProperties(void** oProperties, JD_FLAG* oEvent) {
    if (oProperties != NULL && ctrPoll < numTotal) {
        *oProperties = properties[ctrPoll];
        if (oEvent != NULL) {
            if (ctrPoll) {
                *oEvent = (1 << (ctrPoll - 1));
            } else {
                *oEvent = JUICYGUI_EVENT_NONE;
            }
        }
        if ((ctrPoll + 1) < numTotal) {
            ctrPoll++;
            return true;
        } else {
            ctrPoll = 0;
        }
    }
    return false;
}

void JPM::SetProperties(JD_FLAG iEvent, void* iProperty) {
    JD_INDEX index = JPM_GetEventIndex(iEvent);
    if (properties != NULL && iProperty != NULL && index < numTotal) {
        properties[index] = iProperty;
    }
}
void* JPM::GetProperties(JD_FLAG iEvent) {
    JD_INDEX index = JPM_GetEventIndex(iEvent);
    if (properties != NULL && index < numTotal) {
        return properties[index] != NULL ? properties[index] : properties[indexDefault];
    }
    return NULL;
}

void JPM::SetPropertiesAtIndex(JD_INDEX iIndex, void* iProperty) {
    if (properties != NULL && iProperty != NULL && iIndex < numTotal) {
        properties[iIndex] = iProperty;
    }
}
void* JPM::GetPropertiesAtIndex(JD_INDEX iIndex) {
    if (properties != NULL && iIndex < numTotal) {
        return properties[iIndex];
    }
    return NULL;
}

JD_INDEX JPM_GetEventIndex(JD_FLAG iEvent) {
    if (iEvent) {
        for (JD_INDEX index = (JUICYGUI_NUM_EVENT - 1); index > 0; index--) {
            if (iEvent & (1 << (index - 1))) return index;
        }
    }
    return 0;
}
