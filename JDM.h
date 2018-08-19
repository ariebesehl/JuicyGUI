

#ifndef JDM_H_INCLUDED
#define JDM_H_INCLUDED

#define JDM_BITS 32

#define JDM_USING_SDL 1

#if JDM_USING_SDL
#include <SDL.h>
#endif // JDM_USING_SDL


typedef int JD_I;
typedef unsigned int JD_U;

typedef uint8_t JD_U8;
typedef uint16_t JD_U16;
typedef uint32_t JD_U32;
//typedef uint64_t JD_U64;

typedef int8_t JD_I8;
typedef int16_t JD_I16;
typedef int32_t JD_I32;
//typedef int64_t JD_I64;

typedef JD_U32 JD_INDEX;
typedef JD_U32 JD_FLAG;
typedef JD_U32 JD_COLOR;
typedef JD_U32 JD_TIME;


typedef struct {
	JD_U x;
	JD_U y;
} JD_2U;
typedef struct {
	JD_U x1;
	JD_U y1;
	JD_U x2;
	JD_U y2;
} JD_2x2U;
typedef struct {
	JD_U x;
	JD_U y;
	JD_U w;
	JD_U h;
} JD_URect;

typedef struct {
	JD_I x;
	JD_I y;
} JD_2I;
typedef struct {
	JD_I x1;
	JD_I y1;
	JD_I x2;
	JD_I y2;
} JD_2x2I;
typedef struct {
	JD_I x;
	JD_I y;
	JD_I w;
	JD_I h;
} JD_Rect;

typedef struct {
	JD_U x;
	JD_U y;
	JD_U z;
} JD_3U;

typedef struct {
	JD_I x;
	JD_I y;
	JD_I z;
} JD_3I;

typedef JD_2I JD_Point;
typedef void** JD_List;


bool operator!= (const JD_2U& param0, const JD_2U& param1);
bool operator== (const JD_2U& param0, const JD_2U& param1);
JD_2U operator+ (const JD_2U& param0, const JD_2U& param1);
JD_2U operator- (const JD_2U& param0, const JD_2U& param1);
void operator+= (JD_2U& param0, const JD_2U& param1);
void operator-= (JD_2U& param0, const JD_2U& param1);
void operator+= (JD_2U& param0, const JD_I& param1);
void operator-= (JD_2U& param0, const JD_I& param1);
void operator*= (JD_2U& param0, const JD_I& param1);
void operator/= (JD_2U& param0, const JD_I& param1);

JD_2I operator- (const JD_2I& param0);
bool operator!= (const JD_2I& param0, const JD_2I& param1);
bool operator== (const JD_2I& param0, const JD_2I& param1);
JD_2I operator+ (const JD_2I& param0, const JD_2I& param1);
JD_2I operator- (const JD_2I& param0, const JD_2I& param1);
JD_2I operator* (const JD_2I& param0, const JD_2I& param1);
JD_2I operator/ (const JD_2I& param0, const JD_2I& param1);
void operator+= (JD_2I& param0, const JD_2I& param1);
void operator-= (JD_2I& param0, const JD_2I& param1);

JD_2I operator+ (const JD_2I& param0, const JD_I& param1);
JD_2I operator- (const JD_2I& param0, const JD_I& param1);
JD_2I operator* (const JD_2I& param0, const JD_I& param1);
JD_2I operator/ (const JD_2I& param0, const JD_I& param1);
JD_2I operator<< (const JD_2I& param0, const JD_I& param1);
JD_2I operator>> (const JD_2I& param0, const JD_I& param1);
void operator+= (JD_2I& param0, const JD_I& param1);
void operator-= (JD_2I& param0, const JD_I& param1);
void operator*= (JD_2I& param0, const JD_I& param1);
void operator/= (JD_2I& param0, const JD_I& param1);
void operator<<= (JD_2I& param0, const JD_I& param1);
void operator>>= (JD_2I& param0, const JD_I& param1);

bool operator== (const JD_Rect& param0, const JD_I& param1);

JD_FLAG JDM_GetLSB(JD_FLAG iFlag);
JD_FLAG JDM_GetMSB(JD_FLAG iFlag);

JD_INDEX JDM_GetBitIndex(JD_FLAG iFlag);
JD_INDEX JDM_GetFlagIndex(JD_FLAG iFlag);

JD_COLOR* JDM_InitPixelData(const JD_I iNum, JD_COLOR iColor);
JD_COLOR* JDM_InitPixelData(const JD_Point* iSize, JD_COLOR iColor);
JD_COLOR* JDM_CopyPixelData(const JD_COLOR* iPixeldata, const JD_I iNum);
JD_COLOR* JDM_CopyPixelData(const JD_COLOR* iPixeldata, const JD_Point* iSize);

bool JDM_IsNotEmptyPoint(const JD_Point* iPoint);
void JDM_EmptyPoint(JD_Point* oPoint);
JD_I JDM_GetArea(const JD_Point* iPoint);
JD_I JDM_GetArea(const JD_Rect* iRect);


void JDM_EmptyRect(JD_Rect* oRect);
void JDM_EmptyRectPos(JD_Rect* oRect);
void JDM_EmptyRectSize(JD_Rect* oRect);
void JDM_SetRect(JD_Rect* oRect, const JD_I iX, const JD_I iY, const JD_I iW, const JD_I iH);
void JDM_SetRectPos(JD_Rect* oRect, const JD_Point* iPos);
void JDM_SetRectPos(JD_Rect* oRect, const JD_I iX, const JD_I iY);
void JDM_SetRectSize(JD_Rect* oRect, const JD_Point* iSize);
void JDM_SetRectSize(JD_Rect* oRect, const JD_I iW, const JD_I iH);
void JDM_GetRectPos(const JD_Rect* iRect, JD_Point* oPos);
void JDM_GetRectPos(const JD_Rect* iRect, JD_I* oX, JD_I* oY);
void JDM_GetRectSize(const JD_Rect* iRect, JD_Point* oSize);
void JDM_GetRectSize(const JD_Rect* iRect, JD_I* oWidth, JD_I* oHeight);

void JDM_CopyList(JD_List iList, const JD_INDEX* iSize, JD_List oList);
void JDM_NullList(JD_List ioList, const JD_INDEX* iSize);
void JDM_NullList(JD_List ioList, const JD_INDEX iSize);
void JDM_DeleteList(JD_List* ioList, JD_INDEX* ioSize);
void JDM_AddToList(JD_List* ioList, JD_INDEX* ioSize, void* iItem);
JD_List JDM_AddToList(JD_List ioList, JD_INDEX* ioSize, void* iItem);


#if JDM_USING_SDL
SDL_Rect JDM_XSDL_Rect(const JD_Rect* iRect);
#endif // JDM_USING_SDL

#endif // JDM_H_INCLUDED
