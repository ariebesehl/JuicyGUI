
#include "JDM.h"

#ifndef NULL
#define NULL 0x0
#endif // NULL

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
bool operator!= (const JD_Rect& param0, const int& param1) {
    return ((param0.x != param1) || (param0.y != param1) || (param0.w != param1) || (param0.h != param1));
}

bool operator== (const JD_Rect& param0, const JD_Rect& param1) {
    return ((param0.x == param1.x) && (param0.y == param1.y) && (param0.w == param1.w) && (param0.h == param1.h));
}
bool operator!= (const JD_Rect& param0, const JD_Rect& param1) {
    return ((param0.x != param1.x) || (param0.y != param1.y) || (param0.w != param1.w) || (param0.h != param1.h));
}

JD_FLAG JDM_GetLSB(JD_FLAG iFlag) {
    for (JD_INDEX i = 0; i < JDM_BITS; i++) {
        if (iFlag & (1 << i)) {return (iFlag & (1 << i));}
    }
    return 0;
}
JD_FLAG JDM_GetMSB(JD_FLAG iFlag) {
    for (JD_INDEX i = JDM_BITS; i > 0; i--) {
        if (iFlag & (1 << (i - 1))) {return (iFlag & (1 << (i - 1)));}
    }
    return 0;
}
JD_INDEX JDM_GetBitNum(JD_FLAG iFlag) {
    JD_INDEX n = 0;
    for (JD_FLAG i = 0; i < JDM_BITS; i++) {
        if (iFlag & (1 << i)) {n++;}
    }
    return n;
}
JD_INDEX JDM_GetBitIndex(JD_FLAG iFlag) {
    JD_INDEX i = 0;
    while (i < JDM_BITS) {
        if ((iFlag >> i) & 1) {return i;}
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
    return (JD_COLOR*)JDM_InitData(iNum, (JD_U)iColor);
}
JD_COLOR* JDM_InitPixelData(const JD_Point* iSize, JD_COLOR iColor) {
    return (JD_COLOR*)JDM_InitData(JDM_GetArea(iSize), (JD_U)iColor);
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
    return iPoint->x || iPoint->y;
}

void JDM_EmptyPoint(JD_Point* oPoint) {
    oPoint->x = 0;
    oPoint->y = 0;
}
JD_U JDM_GetArea(const JD_UPoint* iUPoint) {
    return (iUPoint->x * iUPoint->y);
}
JD_I JDM_GetArea(const JD_Point* iPoint) {
    return (iPoint->x * iPoint->y);
}
JD_I JDM_GetArea(const JD_Rect* iRect) {
    return (iRect->w * iRect->h);
}

void JDM_SetPoint(JD_Point* oPoint, const JD_I iX, const JD_I iY) {
    oPoint->x = iX;
    oPoint->y = iY;
}
void JDM_SetPoint(JD_UPoint* oPoint, const JD_I iX, const JD_I iY) {
    oPoint->x = iX;
    oPoint->y = iY;
}

bool JDM_RectContains(const JD_Rect* iRect, const JD_Point* iPoint) {
    if (iPoint->x >= iRect->x) {
        if (iPoint->x < iRect->x + iRect->w) {
            if (iPoint->y >= iRect->y) {
                if (iPoint->y < iRect->y + iRect->h) {
                    return true;
                }
            }
        }
    }
    return false;
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
void JDM_ClipRect(JD_Rect* ioRect, const JD_Point* iSize) {
    if (ioRect->x < 0) {
        ioRect->w += ioRect->x;
        ioRect->x = 0;
    } else {
        if (ioRect->x > iSize->x) {
            ioRect->x = iSize->x;
        }
    }
    if (ioRect->y < 0) {
        ioRect->y += ioRect->y;
        ioRect->y = 0;
    } else {
        if (ioRect->y > iSize->y) {
            ioRect->y = iSize->y;
        }
    }
    if (iSize->x - ioRect->x < ioRect->w) {
        ioRect->w = iSize->x - ioRect->x;
    }
    if (iSize->y - ioRect->y < ioRect->h) {
        ioRect->h = iSize->y - ioRect->y;
    }
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



JD_U* JDM_InitData(const JD_INDEX iSize, JD_U iInitVal) {
    JD_U* data = NULL;
    if (iSize) {
        data = JDM_SetData(new JD_U[iSize], iSize, iInitVal);
    }
    return data;
}
JD_U* JDM_InitData(const JD_UPoint* iSize, JD_U iInitVal) {
    return JDM_InitData(JDM_GetArea(iSize), iInitVal);
}
JD_U* JDM_InitData(const JD_Point* iSize, JD_U iInitVal) {
    return JDM_InitData(JDM_GetArea(iSize), iInitVal);
}

JD_U* JDM_SetData(JD_U* iData, JD_INDEX iNum, JD_U iVal) {
    for (JD_INDEX i = 0; i < iNum; i++) {
        iData[i] = iVal;
    }
    return iData;
}

JD_U* JDM_ZeroData(JD_U* iData, JD_INDEX iNum) {
    return JDM_SetData(iData, iNum, 0x0);
}
JD_U* JDM_CopyData(const JD_U* iSrc, JD_U* iDst, JD_INDEX iNum) {
    if (iNum) {
        if (iSrc != NULL && iDst != NULL) {
            for (JD_INDEX i = 0; i < iNum; i++) {
                iDst[i] = iSrc[i];
            }
        }
    }
    return iDst;
}
void JDM_AddData(JD_U** iDataPtr, JD_INDEX iSize, JD_U iData) {
    if (iSize ? (iDataPtr != NULL) : true) {
        JD_U* newData = new JD_U[iSize + 1];
        JDM_CopyData(*iDataPtr, newData, iSize);
        newData[iSize] = iData;
        if (iDataPtr != NULL) {delete[] *iDataPtr;}
        *iDataPtr = newData;
    }
}
void JDM_RemoveData(JD_U** iDataPtr, JD_INDEX iSize, JD_INDEX iItemIndex) {
    if (iDataPtr != NULL) {
        if (iItemIndex < iSize) {
            JD_U* newData = (iSize - 1) ? new JD_U[iSize - 1] : NULL;
            JDM_CopyData(*iDataPtr, newData, iItemIndex);
            JDM_CopyData(&((*iDataPtr)[iItemIndex + 1]), &(newData[iItemIndex]), iSize - (iItemIndex + 1));
            delete[] *iDataPtr;
            *iDataPtr = newData;
        }
    }
}

void JDM_InitList(JD_List* ioList, JD_INDEX* iSize) {
    JDM_InitList(ioList, iSize, NULL);
}
void JDM_InitList(JD_List* ioList, JD_INDEX* iSize, void* iItem) {
    if (ioList != NULL && iSize != NULL) {
        *ioList = new void*[*iSize];
        JDM_SetList(*ioList, *iSize, NULL);
    }
}
JD_List JDM_InitList(JD_INDEX iSize) {
    return JDM_InitList(iSize, NULL);
}
JD_List JDM_InitList(JD_INDEX iSize, void* iItem) {
    JD_List newList = new void*[iSize];
    JDM_SetList(newList, iSize, iItem);
    return newList;
}

void JDM_SetList(JD_List ioList, const JD_INDEX* iSize, const void* iListItem) {if (iSize != NULL) {JDM_SetList(ioList, *iSize, iListItem);}}
void JDM_SetList(JD_List ioList, const JD_INDEX iSize, const void* iListItem) {
    if (ioList != NULL) {
        for (JD_INDEX i = 0; i < iSize; i++) {
            ioList[i] = (void*)iListItem;
        }
    }
}

void JDM_NullList(JD_List ioList, const JD_INDEX* iSize) {if (iSize != NULL) {JDM_NullList(ioList, *iSize);}}
void JDM_NullList(JD_List ioList, const JD_INDEX iSize) {if (ioList != NULL) {JDM_SetList(ioList, iSize, NULL);}}

void JDM_CopyList(JD_List iList, const JD_INDEX* iSize, JD_List oList) {if (iSize != NULL) {JDM_CopyList(iList, *iSize, oList);}}
void JDM_CopyList(JD_List iList, const JD_INDEX iSize, JD_List oList) {
    if (iList != NULL) {
        for (JD_INDEX i = 0; i < iSize; i++) {
            oList[i] = iList[i];
        }
    }
}
void JDM_DeleteList(JD_List ioList) {delete[] ioList;}
void JDM_DeleteList(JD_List* ioList, JD_INDEX* ioSize) {
    if (*ioList != NULL) {
        delete[] (*ioList);
        *ioList = NULL;
        if (ioSize != NULL) {*ioSize = 0;}
    }
}
void JDM_AddToList(JD_List* ioList, JD_INDEX ioSize, void* iItem) {
    if (ioList != NULL) {
        void** listPtr = *ioList;
        if (ioSize ? (listPtr != NULL) : true) {
            void** newList = new void*[ioSize + 1];
            for (JD_INDEX i = 0; i < ioSize; i++) {
                newList[i] = listPtr[i];
            }
            newList[ioSize] = iItem;
            if (listPtr != NULL) {delete[] listPtr;}
            (ioSize)++;
            *ioList = newList;
        }
    }
}
void JDM_RemoveFromList(JD_List* ioList, JD_INDEX ioSize, JD_INDEX iIndex) {if (ioList != NULL) {*ioList = JDM_RemoveFromList(*ioList, ioSize, iIndex);}}
JD_List JDM_RemoveFromList(JD_List ioList, JD_INDEX ioSize, JD_INDEX iIndex) {
    if (ioList != NULL) {
        if (iIndex < ioSize && ioSize) {
            void** newList = NULL;
            (ioSize)--;
            if (ioSize) {
                newList = new void*[ioSize];
                JD_INDEX skippedItems = 0;
                for (JD_INDEX i = 0; i < ioSize + 1; i++) {
                    if (i == iIndex) {
                        skippedItems++;
                    } else {
                        newList[i - skippedItems] = ioList[i];
                    }
                }
            }
            delete[] ioList;
            return newList;
        }
    }
    return NULL;
}

void* JDM_MemZero(void* iData, JD_INDEX iNum) {
    return JDM_MemSet(iData, 0x0, iNum);
}
void* JDM_MemFill(void* iData, JD_INDEX iNum) {
    return JDM_MemSet(iData, 0xff, iNum);
}
void* JDM_MemSet(void* iData, JD_CHAR iVal, JD_INDEX iNum) {
    JD_CHAR* dataPtr = (JD_CHAR*)iData;
    for (JD_INDEX i = 0; i < iNum; i++) {
        dataPtr[i] = iVal;
    }
    return iData;
}
void* JDM_MemCpy(const void* iSrc, void* iDst, JD_INDEX iNum) {
    JD_CHAR* ptrSrc = (JD_CHAR*)iSrc;
    JD_CHAR* ptrDst = (JD_CHAR*)iDst;
    for (JD_INDEX i = 0; i < iNum; i++) {
        ptrDst[i] = ptrSrc[i];
    }
    return iDst;
}


#ifndef JDM_NO_SDL
SDL_Rect JDM_XSDL_Rect(const JD_Rect* iRect) {
    SDL_Rect rect;
    rect.x = iRect->x;
    rect.y = iRect->y;
    rect.w = iRect->w;
    rect.h = iRect->h;
    return rect;
}
#endif // JDM_NO_SDL
