
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"

JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iID) {
    init(iHostUI, iID, NULL, JUICYDEFAULT_BOX_PROPERTIES_COLOR, JUICYDEFAULT_BOX_PROPERTIES_FRAMECOLOR);
}
JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions) {
    init(iHostUI, iID, iDimensions, JUICYDEFAULT_BOX_PROPERTIES_COLOR, JUICYDEFAULT_BOX_PROPERTIES_FRAMECOLOR);
}
JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iFrameColor) {
    init(iHostUI, iID, iDimensions, iColor, iFrameColor);
}
void JuicyGUI_Box::init(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iFrameColor) {
    element.setCredentials(iHostUI, this, new JPM((void*)new JuicyGUI_Box_Properties), new JSPR(iHostUI->GetEngine()), iID, JUICYGUI_TYPE_ID_BOX);
    initDefault(iColor, iFrameColor);
    element.setRect(iDimensions);
    createTextures();
}
void JuicyGUI_Box::initDefault(JD_COLOR iColor, JD_COLOR iFrameColor) {
    JuicyGUI_Box_Properties* polledProps = NULL;
    JD_FLAG eventType = JUICYGUI_EVENT_NONE;
    while (element.properties->PollProperties((void**)&polledProps, &eventType)) {
        switch (eventType) {
            case JUICYGUI_EVENT_NONE:
                if (polledProps == NULL) {
                    polledProps = new JuicyGUI_Box_Properties;
                    element.properties->SetProperties(eventType, (void*)polledProps);
                }
                polledProps->color = iColor;
                polledProps->frameColor = iFrameColor;
                polledProps->frameSize = JUICYDEFAULT_BOX_PROPERTIES_FRAMESIZE;
                break;
            default:
                break;
        }
    }
    defaultProperties = (JuicyGUI_Box_Properties*)element.properties->GetProperties(JUICYGUI_EVENT_NONE);
}
JuicyGUI_Box::~JuicyGUI_Box() {
    if (element.properties != NULL) {
        JuicyGUI_Box_Properties* ptrProps = NULL;
        while (element.properties->PollProperties((void**)&ptrProps, NULL)) {
            if (ptrProps != NULL) {
                delete ptrProps;
            }
        }
    }
}

void JuicyGUI_Box::createTextures() {
    if (element.GetSprites() != NULL && element.getEngine() != NULL) {
        JD_Point cacheDimensions;
        element.getSize(&cacheDimensions);
        if (JDM_GetArea(&cacheDimensions)) {
            for (JD_INDEX i = 0; i < element.properties->GetSize(); i++) {
                JuicyGUI_Box_Properties* ptrProperties = (JuicyGUI_Box_Properties*)(element.properties->GetPropertiesAtIndex(i));
                JuicySprite* boxSprite = NULL;
                if (ptrProperties != NULL) {
                    JD_COLOR* pixelData = NULL;
                    pixelData = new JD_COLOR[cacheDimensions.x * cacheDimensions.y];
                    for (JD_I y = 0; y < cacheDimensions.y; y++) {
                        for (JD_I x = 0; x < cacheDimensions.x; x++) {
                            if ((x >= ptrProperties->frameSize) && (x < (cacheDimensions.x - ptrProperties->frameSize)) && (y >= ptrProperties->frameSize) && (y < (cacheDimensions.y - ptrProperties->frameSize))) {
                                pixelData[x + y * cacheDimensions.x] = ptrProperties->color;
                            } else {
                                pixelData[x + y * cacheDimensions.x] = ptrProperties->frameColor;
                            }
                        }
                    }
                    boxSprite = element.getEngine()->CreateSprite(&cacheDimensions, pixelData);
                    if (pixelData != NULL) {delete[] pixelData;}
                }
                element.GetSprites()->AddSprite(boxSprite);
            }
            element.UpdateSprites();
        }
    }
}

void JuicyGUI_Box::resetTextures() {
    if (element.GetSprites() != NULL) {
        element.GetSprites()->Reset();
    }
    createTextures();
}

JD_I JuicyGUI_Box::GetFrameSize() {
    return GetFrameSize(JUICYGUI_EVENT_NONE);
}
JD_I JuicyGUI_Box::GetFrameSize(JD_FLAG iEvent) {
    return ((JuicyGUI_Box_Properties*)(element.properties->GetProperties(iEvent)))->frameSize;
}
