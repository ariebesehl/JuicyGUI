
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Button.h"

JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iID, const char* iLabel, const JD_Rect* iRect) {
    init(iHostUI, iID, iLabel, iRect, JUICYGUI_BUTTON_FLAG_INIT);
}
JuicyGUI_Button::JuicyGUI_Button(JuicyGUI* iHostUI, JD_INDEX iID, const char* iLabel) {
    init(iHostUI, iID, iLabel, NULL, JUICYGUI_BUTTON_FLAG_INIT);
}

JuicyGUI_Button::~JuicyGUI_Button() {
    if (element.properties != NULL) {
        JuicyGUI_Button_Properties* ptrProps = NULL;
        while (element.properties->PollProperties((void**)&ptrProps, NULL)) {
            if (ptrProps != NULL) {
                delete ptrProps;
            }
        }
    }
}

void JuicyGUI_Button::init(JuicyGUI* iHostUI, JD_INDEX iID, const char* iLabel, const JD_Rect* iRect, JD_FLAG iFlag) {
    element.setCredentials(iHostUI, this, new JPM(), new JSPR(iHostUI->GetEngine()), iID, JUICYGUI_TYPE_ID_BUTTON);
    element.setRect(iRect);
    flag = iFlag;
    initDefault(iLabel);
    createTextures();
}
void JuicyGUI_Button::initDefault(const char* iLabel) {
    JuicyGUI_Button_Properties* polledProps = NULL;
    JD_FLAG eventType = JUICYGUI_EVENT_NONE;
    while (element.properties->PollProperties((void**)&polledProps, &eventType)) {
        switch (eventType) {
            case JUICYGUI_EVENT_NONE:
                if (polledProps == NULL) {
                    polledProps = new JuicyGUI_Button_Properties;
                    element.properties->SetProperties(eventType, (void*)polledProps);
                }
                polledProps->label = iLabel != NULL ? iLabel : JUICYDEFAULT_BUTTON_PROPERTIES_LABEL;
                polledProps->boxProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_COLOR;
                polledProps->boxProperties.frameColor = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_FRAMECOLOR;
                polledProps->boxProperties.frameSize = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_FRAMESIZE;
                polledProps->fontProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_COLOR;
                polledProps->fontProperties.fillColor = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_FILLCOLOR;
                polledProps->fontProperties.fontPath = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_FONTPATH;
                polledProps->fontProperties.lineMargin = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_LINEMARGIN;
                polledProps->fontProperties.style = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_STYLE;
                break;
            case JUICYGUI_EVENT_HOVER:
                if (polledProps == NULL) {
                    polledProps = new JuicyGUI_Button_Properties;
                    element.properties->SetProperties(eventType, (void*)polledProps);
                }
                *((JuicyGUI_Button_Properties*)polledProps) = *((JuicyGUI_Button_Properties*)(element.properties->GetProperties(JUICYGUI_EVENT_NONE)));
                polledProps->boxProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_COLOR_HOVER;
                polledProps->boxProperties.frameColor = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_FRAMECOLOR_HOVER;
                polledProps->fontProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_COLOR_HOVER;
                break;
            case JUICYGUI_EVENT_PRESSED:
                if (polledProps == NULL) {
                    polledProps = new JuicyGUI_Button_Properties;
                    element.properties->SetProperties(eventType, (void*)polledProps);
                }
                *((JuicyGUI_Button_Properties*)polledProps) = *((JuicyGUI_Button_Properties*)(element.properties->GetProperties(JUICYGUI_EVENT_NONE)));
                polledProps->boxProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_COLOR_PRESSED;
                polledProps->boxProperties.frameColor = JUICYDEFAULT_BUTTON_PROPERTIES_BOX_PROPERTIES_FRAMECOLOR_PRESSED;
                polledProps->fontProperties.color = JUICYDEFAULT_BUTTON_PROPERTIES_CHARSET_PROPERTIES_COLOR_PRESSED;
                break;
            default:
                break;
        }
    }
    defaultProperties = (JuicyGUI_Button_Properties*)element.properties->GetProperties(JUICYGUI_EVENT_NONE);
}

void JuicyGUI_Button::createTextures() {
    if (element.GetSprites() != NULL && element.getEngine() != NULL) {
        JD_Point buttonSize;
        element.getSize(&buttonSize);
        if (JDM_GetArea(&buttonSize)) {
            for (JD_INDEX i = 0; i < element.properties->GetSize(); i++) {
                JuicyGUI_Button_Properties* ptrProperties = (JuicyGUI_Button_Properties*)element.properties->GetPropertiesAtIndex(i);
                JD_COLOR* pixelData = NULL;
                JuicySprite* buttonSprite = NULL;
                if (ptrProperties != NULL) {
                    pixelData = new JD_COLOR[buttonSize.x * buttonSize.y];
                    for (JD_I y = 0; y < buttonSize.y; y++) {
                        for (JD_I x = 0; x < buttonSize.x; x++) {
                            if ((x >= ptrProperties->boxProperties.frameSize) && (x < (buttonSize.x - ptrProperties->boxProperties.frameSize)) && (y >= ptrProperties->boxProperties.frameSize) && (y < (buttonSize.y - ptrProperties->boxProperties.frameSize))) {
                                pixelData[x + y * buttonSize.x] = ptrProperties->boxProperties.color;
                            } else {
                                pixelData[x + y * buttonSize.x] = ptrProperties->boxProperties.frameColor;
                            }
                        }
                    }
                    if (pixelData != NULL) {
                        buttonSprite = element.getEngine()->CreateSprite(&buttonSize, pixelData);
                        if (buttonSprite != NULL) {
                            switch (flag & JUICYGUI_BUTTON_FLAG_MASK_TYPE) {
                                case JUICYGUI_BUTTON_FLAG_IMAGE: {
                                    }
                                    break;
                                case JUICYGUI_BUTTON_FLAG_NORMAL: {
                                        JuicySprite* textSprite = element.getEngine()->CreateSpriteText(ptrProperties->label, ptrProperties->fontProperties.fontPath, element.getHeight() / 2, ptrProperties->fontProperties.color, ptrProperties->fontProperties.style);
                                        if (textSprite != NULL) {
                                            element.getEngine()->BlendSprites(textSprite, buttonSprite);
                                            element.getEngine()->FreeSprite(textSprite);
                                        }
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                        delete[] pixelData;
                    }
                }
                element.GetSprites()->AddSprite(buttonSprite);
            }
            element.UpdateSprites();
        }
    }
}

void JuicyGUI_Button::resetTextures() {
    if (element.GetSprites() != NULL) {
        element.GetSprites()->Reset();
    }
	createTextures();
}


