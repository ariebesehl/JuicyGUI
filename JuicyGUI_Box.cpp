
#include "JuicyGUI_Definitions.h"
#include "JuicyGUI_Box.h"

JuicyGUI_Box::JuicyGUI_Box(JuicyGUI* iHostUI, JD_INDEX iID, JD_Rect* iDimensions, JD_COLOR iColor, JD_COLOR iBorderColor) {
    element.setCredentials(iHostUI, this, iID, JUICYGUI_TYPE_ID_BOX);
    element.setRect(iDimensions);
    properties.color = iColor;
    properties.borderColor = iBorderColor;
    properties.borderSize = JUICYGUI_BOX_BORDER_SIZE;
	textureEngine = new JSPR(element.getEngine());
    createTextures();
}

JuicyGUI_Box::~JuicyGUI_Box() {
	if (textureEngine != NULL) {delete textureEngine;}
}

void JuicyGUI_Box::createTextures() {
    if (textureEngine != NULL) {
        JD_Point cacheDimensions;
        element.getSize(&cacheDimensions);
        JD_COLOR* pixelData = NULL;
        if (JDM_IsNotEmptyPoint(&cacheDimensions)) {
            pixelData = new JD_COLOR[cacheDimensions.x * cacheDimensions.y];
            for (JD_I y = 0; y < cacheDimensions.y; y++) {
                for (JD_I x = 0; x < cacheDimensions.x; x++) {
                    if ((x >= properties.borderSize) && (x < (cacheDimensions.x - properties.borderSize)) && (y >= properties.borderSize) && (y < (cacheDimensions.y - properties.borderSize))) {
                        pixelData[x + y * cacheDimensions.x] = properties.color;
                    } else {
                        pixelData[x + y * cacheDimensions.x] = properties.borderColor;
                    }
                }
            }
        }
        textureEngine->AddSprite((element.getEngine()->CreateSprite(&cacheDimensions, pixelData)));
        if (pixelData != NULL) {delete[] pixelData;}
        element.SetTextureEngine(textureEngine);
    }
}

void JuicyGUI_Box::resetTextures() {
	textureEngine->Reset();
    createTextures();
}

