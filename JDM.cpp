
#include "JDM.h"


bool operator!= (const JD_2U& param0, const JD_2U& param1) {
    return (param0.x != param1.x || param0.y != param1.y);
}

bool operator== (const JD_2U& param0, const JD_2U& param1) {
    return (param0.x == param1.x && param0.y == param1.y);
}

JD_2U operator+ (const JD_2U& param0, const JD_2U& param1) {
    JD_2U sum;
    sum.x = param0.x + param1.x;
    sum.y = param0.y + param1.y;
    return sum;
}

JD_2U operator- (const JD_2U& param0, const JD_2U& param1) {
    JD_2U diff;
    diff.x = param0.x - param1.x;
    diff.y = param0.y - param1.y;
    return diff;
}

void operator+= (JD_2U& param0, const JD_2U& param1) {
    param0.x += param1.x;
    param0.y += param1.y;
}
void operator-= (JD_2U& param0, const JD_2U& param1) {
    param0.x -= param1.x;
    param0.y -= param1.y;
}

void operator+= (JD_2U& param0, const unsigned int& param1) {
    param0.x += param1;
    param0.y += param1;
}
void operator-= (JD_2U& param0, const unsigned int& param1) {
    param0.x -= param1;
    param0.y -= param1;
}
void operator*= (JD_2U& param0, const unsigned int& param1) {
    param0.x *= param1;
    param0.y *= param1;
}
void operator/= (JD_2U& param0, const unsigned int& param1) {
    param0.x /= param1;
    param0.y /= param1;
}


JD_2I operator- (const JD_2I& param0) {
    JD_2I newPoint;
    newPoint.x = -param0.x;
    newPoint.y = -param0.y;
    return newPoint;
}

bool operator!= (const JD_2I& param0, const JD_2I& param1) {
    return (param0.x != param1.x || param0.y != param1.y);
}

bool operator== (const JD_2I& param0, const JD_2I& param1) {
    return (param0.x == param1.x && param0.y == param1.y);
}

JD_2I operator+ (const JD_2I& param0, const JD_2I& param1) {
    JD_2I sum;
    sum.x = param0.x + param1.x;
    sum.y = param0.y + param1.y;
    return sum;
}
JD_2I operator- (const JD_2I& param0, const JD_2I& param1) {
    JD_2I diff;
    diff.x = param0.x - param1.x;
    diff.y = param0.y - param1.y;
    return diff;
}
JD_2I operator* (const JD_2I& param0, const JD_2I& param1) {
    JD_2I prod;
    prod.x = param0.x * param1.x;
    prod.y = param0.y * param1.y;
    return prod;
}
JD_2I operator/ (const JD_2I& param0, const JD_2I& param1) {
    JD_2I quot;
    quot.x = param0.x / param1.x;
    quot.y = param0.y / param1.y;
    return quot;
}
JD_2I operator+ (const JD_2I& param0, const int& param1) {
    JD_2I sum;
    sum.x = param0.x + param1;
    sum.y = param0.y + param1;
    return sum;
}
JD_2I operator- (const JD_2I& param0, const int& param1) {
    JD_2I diff;
    diff.x = param0.x - param1;
    diff.y = param0.y - param1;
    return diff;
}
JD_2I operator* (const JD_2I& param0, const int& param1) {
    JD_2I prod;
    prod.x = param0.x * param1;
    prod.y = param0.y * param1;
    return prod;
}
JD_2I operator/ (const JD_2I& param0, const int& param1) {
    JD_2I quot;
    quot.x = param0.x / param1;
    quot.y = param0.y / param1;
    return quot;
}
JD_2I operator<< (const JD_2I& param0, const int& param1) {
    JD_2I lshift;
    lshift.x = param0.x << param1;
    lshift.y = param0.y << param1;
    return lshift;
}
JD_2I operator>> (const JD_2I& param0, const int& param1) {
    JD_2I shift;
    shift.x = param0.x >> param1;
    shift.y = param0.y >> param1;
    return shift;
}

void operator+= (JD_2I& param0, const JD_2I& param1) {
    param0.x += param1.x;
    param0.y += param1.y;
}
void operator-= (JD_2I& param0, const JD_2I& param1) {
    param0.x -= param1.x;
    param0.y -= param1.y;
}

void operator+= (JD_2I& param0, const int& param1) {
    param0.x += param1;
    param0.y += param1;
}
void operator-= (JD_2I& param0, const int& param1) {
    param0.x -= param1;
    param0.y -= param1;
}
void operator*= (JD_2I& param0, const int& param1) {
    param0.x *= param1;
    param0.y *= param1;
}
void operator/= (JD_2I& param0, const int& param1) {
    param0.x /= param1;
    param0.y /= param1;
}
void operator<<= (JD_2I& param0, const int& param1) {
    param0.x <<= param1;
    param0.y <<= param1;
}
void operator>>= (JD_2I& param0, const int& param1) {
    param0.x >>= param1;
    param0.y >>= param1;
}


bool operator== (const JD_Rect& param0, const int& param1) {
    return ((param0.x == param1) && (param0.y == param1) && (param0.w == param1) && (param0.h == param1));
}

JD_INDEX JDM_GetBitIndex(JD_FLAG iFlag) {
    JD_INDEX i = 0;
    while (i < JDM_BITS) {
        if ((iFlag << i) & 1) {return i;}
        i++;
    }
    return i;
}

JD_INDEX JDM_GetFlagIndex(JD_FLAG iFlag) {
    if (iFlag) {
        for (JD_INDEX i = 1; i < JDM_BITS; i++) {
            if (iFlag & (1 << (i - 1))) return i;
        }
    }
    return 0;
}

JD_COLOR* JDM_InitPixelData(const JD_I iNum, JD_COLOR iColor) {
    JD_COLOR* pixelData = NULL;
    if (iNum > 0) {
        pixelData = new JD_COLOR[iNum];
        for (JD_I i = 0; i < iNum; i++) {
            pixelData[i] = iColor;
        }
    }
    return pixelData;
}
JD_COLOR* JDM_InitPixelData(const JD_Point* iSize, JD_COLOR iColor) {
    JD_COLOR* pixelData = NULL;
    if (iSize != NULL) {
        pixelData = JDM_InitPixelData(JDM_GetArea(iSize), iColor);
    }
    return pixelData;
}
JD_COLOR* JDM_CopyPixelData(const JD_COLOR* iPixeldata, const JD_I iNum) {
    JD_COLOR* pixelCopy = NULL;
    if (iPixeldata != NULL && iNum) {
        pixelCopy = new JD_COLOR[iNum];
        for (JD_I i = 0; i < iNum; i++) {
            pixelCopy[i] = iPixeldata[i];
        }
    }
    return pixelCopy;
}
JD_COLOR* JDM_CopyPixelData(const JD_COLOR* iPixeldata, const JD_Point* iSize) {
    JD_COLOR* pixelCopy = NULL;
    if (iSize != NULL) {
        pixelCopy = JDM_CopyPixelData(iPixeldata, JDM_GetArea(iSize));
    }
    return pixelCopy;
}


bool JDM_IsNotEmptyPoint(const JD_Point* iPoint) {
    return iPoint->x && iPoint->y;
}

void JDM_EmptyPoint(JD_Point* oPoint) {
    oPoint->x = 0;
    oPoint->y = 0;
}
JD_I JDM_GetArea(const JD_Point* iPoint) {
    return (iPoint->x * iPoint->y);
}
JD_I JDM_GetArea(const JD_Rect* iRect) {
    return (iRect->w * iRect->h);
}

void JDM_EmptyRect(JD_Rect* oRect) {
    oRect->x = 0;
    oRect->y = 0;
    oRect->w = 0;
    oRect->h = 0;
}
void JDM_EmptyRectPos(JD_Rect* oRect) {
    oRect->x = 0;
    oRect->y = 0;
}
void JDM_EmptyRectSize(JD_Rect* oRect) {
    oRect->w = 0;
    oRect->h = 0;
}
void JDM_SetRect(JD_Rect* oRect, const int iX, const int iY, const int iW, const int iH) {
    oRect->x = iX;
    oRect->y = iY;
    oRect->w = iW;
    oRect->h = iH;
}
void JDM_SetRectPos(JD_Rect* oRect, const JD_Point* iPos) {
    oRect->x = iPos->x;
    oRect->y = iPos->y;
}
void JDM_SetRectPos(JD_Rect* oRect, const int iX, const int iY) {
    oRect->x = iX;
    oRect->y = iY;
}
void JDM_SetRectSize(JD_Rect* oRect, const JD_Point* iSize) {
    oRect->w = iSize->x;
    oRect->h = iSize->y;
}
void JDM_SetRectSize(JD_Rect* oRect, const int iW, const int iH) {
    oRect->w = iW;
    oRect->h = iH;
}
void JDM_GetRectPos(const JD_Rect* iRect, JD_Point* oPos) {
    oPos->x = iRect->x;
    oPos->y = iRect->y;
}
void JDM_GetRectPos(const JD_Rect* iRect, int* oX, int* oY) {
    *oX = iRect->x;
    *oY = iRect->y;
}
void JDM_GetRectSize(const JD_Rect* iRect, JD_Point* oSize) {
    oSize->x = iRect->w;
    oSize->y = iRect->h;
}
void JDM_GetRectSize(const JD_Rect* iRect, int* oWidth, int* oHeight) {
    *oWidth = iRect->w;
    *oHeight = iRect->h;
}
void JDM_CopyList(JD_List iList, const JD_INDEX* iSize, JD_List oList) {
    if (iList != NULL && iSize != NULL) {
        for (JD_INDEX i = 0; i < (*iSize); i++) {
            oList[i] = iList[i];
        }
    }
}
void JDM_NullList(JD_List ioList, const JD_INDEX* ioSize) {
    if (ioList != NULL && ioSize != NULL) {
        for (JD_INDEX i = 0; i < (*ioSize); i++) {
            ioList[i] = NULL;
        }
    }
}
void JDM_NullList(JD_List ioList, const JD_INDEX iSize) {
    if (ioList != NULL) {
        for (JD_INDEX i = 0; i < iSize; i++) {
            ioList[i] = NULL;
        }
    }
}
void JDM_DeleteList(JD_List* ioList, JD_INDEX* ioSize) {
    if (*ioList != NULL) {
        delete[] (*ioList);
        *ioList = NULL;
        if (ioSize != NULL) {*ioSize = 0;}
    }
}
void JDM_AddToList(JD_List* ioList, JD_INDEX* ioSize, void* iItem) {
    *ioList = JDM_AddToList(*ioList, ioSize, iItem);
}
JD_List JDM_AddToList(JD_List ioList, JD_INDEX* ioSize, void* iItem) {
    if (ioSize != NULL) {
        if (*ioSize ? (ioList != NULL) : true) {
            void** newList = new void*[(*ioSize) + 1];
            for (JD_INDEX i = 0; i < (*ioSize); i++) {
                newList[i] = ioList[i];
            }
            newList[(*ioSize)] = iItem;
            if (ioList != NULL) {delete[] ioList;}
            (*ioSize)++;
            return newList;
        }
    }
    return NULL;
}

#if JDM_USING_SDL
SDL_Rect JDM_XSDL_Rect(const JD_Rect* iRect) {
    SDL_Rect rect;
    rect.x = iRect->x;
    rect.y = iRect->y;
    rect.w = iRect->w;
    rect.h = iRect->h;
    return rect;
}
#endif // JDM_USING_SDL
