#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#include <iostream>
#include <time.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../JuicyGUI/Juicy.h"
#include "conway_sdl.h"

#define MUSIC_PATH_1 "res/audio/music/Lee_Rosevere_-_03_-_User_Friendly_future_mix.ogg"


#define JCON_MENU_NUM 4
#define JCON_BOX_MENU_ID 99
#define JCON_BOX_MENU_WIDTH_DIVISOR 4
#define JCON_BOX_MENU_HEIGHT_DIVISOR 2
#define JCON_LABEL_MENU_TITLE_MAIN "Menu"
#define JCON_LABEL_MENU_TITLE_SETTINGS "Settings"
#define JCON_LABEL_MENU_TITLE_HELP "Help"
#define JCON_LABEL_MENU_TITLE_CREDITS "Credits"
const char* gTXT_MenuLabels[JCON_MENU_NUM] = {JCON_LABEL_MENU_TITLE_MAIN, JCON_LABEL_MENU_TITLE_SETTINGS, JCON_LABEL_MENU_TITLE_HELP, JCON_LABEL_MENU_TITLE_CREDITS};
const char* gTXT_MenuLabelPtr = gTXT_MenuLabels[0];
JuicyGUI_Box* gGUI_MenuBox = NULL;
JD_Rect gPos_Menu;


#define JCON_BUTTON_HUD_NUM 12
#define JCON_BUTTON_HUD_WIDTH_DIVISOR 12
#define JCON_BUTTON_HUD_HEIGHT_DIVISOR 27
#define JCON_BUTTON_HUD_ID_MENU 200
#define JCON_BUTTON_HUD_ID_SLOWER 201
#define JCON_BUTTON_HUD_ID_RUN 202
#define JCON_BUTTON_HUD_ID_FASTER 203
#define JCON_BUTTON_HUD_ID_SPAWN 204
#define JCON_BUTTON_HUD_ID_WIPE 205
#define JCON_BUTTON_HUD_ID_RATIO_UP 206
#define JCON_BUTTON_HUD_ID_RATIO_DOWN 207
#define JCON_BUTTON_HUD_ID_ZOOM_IN 208
#define JCON_BUTTON_HUD_ID_ZOOM_OUT 209
#define JCON_BUTTON_HUD_ID_GRID 210
#define JCON_BUTTON_HUD_ID_HOME 211
#define JCON_LABEL_BUTTON_HUD_MENU "Menu"
#define JCON_LABEL_BUTTON_HUD_SLOWER "speed-"
#define JCON_LABEL_BUTTON_HUD_RUN "Run / Pause"
#define JCON_LABEL_BUTTON_HUD_FASTER "speed+"
#define JCON_LABEL_BUTTON_HUD_SPAWN "spawn Pop"
#define JCON_LABEL_BUTTON_HUD_WIPE "wipe Pop"
#define JCON_LABEL_BUTTON_HUD_RATIO_UP "ratio+"
#define JCON_LABEL_BUTTON_HUD_RATIO_DOWN "ratio-"
#define JCON_LABEL_BUTTON_HUD_ZOOM_IN "zoom+"
#define JCON_LABEL_BUTTON_HUD_ZOOM_OUT "zoom-"
#define JCON_LABEL_BUTTON_HUD_GRID "toggle Grid"
#define JCON_LABEL_BUTTON_HUD_HOME "reset Cam"
const char* gTXT_HUDButtonLabels[JCON_BUTTON_HUD_NUM] = {
    JCON_LABEL_BUTTON_HUD_MENU,
    JCON_LABEL_BUTTON_HUD_RUN,
    JCON_LABEL_BUTTON_HUD_FASTER,
    JCON_LABEL_BUTTON_HUD_SLOWER,
    JCON_LABEL_BUTTON_HUD_SPAWN,
    JCON_LABEL_BUTTON_HUD_WIPE,
    JCON_LABEL_BUTTON_HUD_RATIO_UP,
    JCON_LABEL_BUTTON_HUD_RATIO_DOWN,
    JCON_LABEL_BUTTON_HUD_HOME,
    JCON_LABEL_BUTTON_HUD_GRID,
    JCON_LABEL_BUTTON_HUD_ZOOM_IN,
    JCON_LABEL_BUTTON_HUD_ZOOM_OUT,
};
JD_INDEX gGUI_HUDButtonIDs[JCON_BUTTON_HUD_NUM] = {
    JCON_BUTTON_HUD_ID_MENU,
    JCON_BUTTON_HUD_ID_RUN,
    JCON_BUTTON_HUD_ID_FASTER,
    JCON_BUTTON_HUD_ID_SLOWER,
    JCON_BUTTON_HUD_ID_SPAWN,
    JCON_BUTTON_HUD_ID_WIPE,
    JCON_BUTTON_HUD_ID_RATIO_UP,
    JCON_BUTTON_HUD_ID_RATIO_DOWN,
    JCON_BUTTON_HUD_ID_HOME,
    JCON_BUTTON_HUD_ID_GRID,
    JCON_BUTTON_HUD_ID_ZOOM_IN,
    JCON_BUTTON_HUD_ID_ZOOM_OUT,
};
JuicyGUI_Button* gGUI_HUDButtons[JCON_BUTTON_HUD_NUM] = {NULL};
JD_Rect gPos_HUDButtons[JCON_BUTTON_HUD_NUM];

#define JCON_BOX_HUD_NUM 2
#define JCON_BOX_HUD_FRAMESIZE 1
#define JCON_BOX_HUD_ID_LEFT 97
#define JCON_BOX_HUD_ID_RIGHT 98
#define JCON_BOX_HUD_LEFT_WIDTH_DIVISOR 4
#define JCON_BOX_HUD_LEFT_HEIGHT_DIVISOR 2
#define JCON_BOX_HUD_RIGHT_WIDTH_DIVISOR 4
#define JCON_BOX_HUD_RIGHT_HEIGHT_DIVISOR 2
JD_INDEX gGUI_HUDBoxIDs[JCON_BOX_HUD_NUM] = {
    JCON_BOX_HUD_ID_LEFT,
    JCON_BOX_HUD_ID_RIGHT,
};
JuicyGUI_Box* gGUI_HUDBox[JCON_BOX_HUD_NUM] = {NULL};
JD_Rect gPos_HUD[JCON_BOX_HUD_NUM];

#define JCON_BUTTON_MENU_NUM 6
#define JCON_BUTTON_MENU_WIDTH_DIVISOR 8
#define JCON_BUTTON_MENU_HEIGHT_DIVISOR 22
#define JCON_BUTTON_MENU_ID_PLAY 100
#define JCON_BUTTON_MENU_ID_RESET 101
#define JCON_BUTTON_MENU_ID_SETTINGS 102
#define JCON_BUTTON_MENU_ID_HELP 103
#define JCON_BUTTON_MENU_ID_CREDITS 104
#define JCON_BUTTON_MENU_ID_EXIT 105
#define JCON_LABEL_BUTTON_MENU_PLAY "Play"
#define JCON_LABEL_BUTTON_MENU_RESET "Reset"
#define JCON_LABEL_BUTTON_MENU_SETTINGS "Settings"
#define JCON_LABEL_BUTTON_MENU_HELP "Help"
#define JCON_LABEL_BUTTON_MENU_CREDITS "Credits"
#define JCON_LABEL_BUTTON_MENU_EXIT "Exit"
const char* gTXT_MenuButtonLabels[JCON_BUTTON_MENU_NUM] = {
    JCON_LABEL_BUTTON_MENU_PLAY,
    JCON_LABEL_BUTTON_MENU_RESET,
    JCON_LABEL_BUTTON_MENU_SETTINGS,
    JCON_LABEL_BUTTON_MENU_HELP,
    JCON_LABEL_BUTTON_MENU_CREDITS,
    JCON_LABEL_BUTTON_MENU_EXIT,
};
JD_INDEX gGUI_MenuButtonIDs[JCON_BUTTON_MENU_NUM] = {
    JCON_BUTTON_MENU_ID_PLAY,
    JCON_BUTTON_MENU_ID_RESET,
    JCON_BUTTON_MENU_ID_SETTINGS,
    JCON_BUTTON_MENU_ID_HELP,
    JCON_BUTTON_MENU_ID_CREDITS,
    JCON_BUTTON_MENU_ID_EXIT,
};
JuicyGUI_Button* gGUI_MenuButtons[JCON_BUTTON_MENU_NUM] = {NULL};
JD_Rect gPos_MenuButtons[JCON_BUTTON_MENU_NUM];


#define JCON_BUTTON_SETTINGS_NUM 1
#define JCON_BUTTON_SETTINGS_ID_BACK 110
#define JCON_LABEL_BUTTON_SETTINGS_BACK "Back"
const char* gTXT_SettingsButtonLabels[JCON_BUTTON_SETTINGS_NUM] = {
    JCON_LABEL_BUTTON_SETTINGS_BACK,
};
JD_INDEX gGUI_SettingsButtonIDs[JCON_BUTTON_SETTINGS_NUM] = {
    JCON_BUTTON_SETTINGS_ID_BACK,
};
JuicyGUI_Button* gGUI_SettingsButtons[JCON_BUTTON_SETTINGS_NUM] = {NULL};
JD_Rect gPos_SettingsButtons[JCON_BUTTON_SETTINGS_NUM];


#define JCON_BUTTON_HELP_NUM 1
#define JCON_BUTTON_HELP_ID_BACK 120
#define JCON_LABEL_BUTTON_HELP_BACK "Back"
#define JCON_LABEL_HELP_NUM 7
#define JCON_LABEL_HELP_0 "Left-click a cell to populate/wipe."
#define JCON_LABEL_HELP_1 "Right-click and hold to move camera."
#define JCON_LABEL_HELP_2 "Mousewheel to zoom."
#define JCON_LABEL_HELP_3 "Space bar to pause/resume."
#define JCON_LABEL_HELP_4 ""
#define JCON_LABEL_HELP_5 "Based on John Conway's"
#define JCON_LABEL_HELP_6 "\"The Game of Life\"."
const char* gTXT_HelpButtonLabels[JCON_BUTTON_HELP_NUM] = {
    JCON_LABEL_BUTTON_HELP_BACK,
};
JD_INDEX gGUI_HelpButtonIDs[JCON_BUTTON_HELP_NUM] = {
    JCON_BUTTON_HELP_ID_BACK,
};
JuicyGUI_Button* gGUI_HelpButtons[JCON_BUTTON_HELP_NUM] = {NULL};
JD_Rect gPos_HelpButtons[JCON_BUTTON_HELP_NUM];
const char* gTXT_Help[JCON_LABEL_HELP_NUM] = {
    JCON_LABEL_HELP_0,
    JCON_LABEL_HELP_1,
    JCON_LABEL_HELP_2,
    JCON_LABEL_HELP_3,
    JCON_LABEL_HELP_4,
    JCON_LABEL_HELP_5,
    JCON_LABEL_HELP_6,
};
JD_Point gPos_HelpText;


#define JCON_BUTTON_CREDITS_NUM 1
#define JCON_BUTTON_CREDITS_ID_BACK 130
#define JCON_LABEL_BUTTON_CREDITS_BACK "Back"
#define JCON_LABEL_CREDITS_NUM 8
#define JCON_LABEL_CREDITS_0 "by Andreas Riebesehl"
#define JCON_LABEL_CREDITS_1 "https://morgen.ist/"
#define JCON_LABEL_CREDITS_2 ""
#define JCON_LABEL_CREDITS_3 "GCC, SDL, Code::Blocks and many more!"
#define JCON_LABEL_CREDITS_4 "OpenSans for font"
#define JCON_LABEL_CREDITS_5 "Lee Rosevere for music"
#define JCON_LABEL_CREDITS_6 ""
#define JCON_LABEL_CREDITS_7 __DATE__
const char* gTXT_CreditsButtonLabels[JCON_BUTTON_CREDITS_NUM] = {
    JCON_LABEL_BUTTON_CREDITS_BACK,
};
JD_INDEX gGUI_CreditspButtonIDs[JCON_BUTTON_CREDITS_NUM] = {
    JCON_BUTTON_CREDITS_ID_BACK,
};
JuicyGUI_Button* gGUI_CreditsButtons[JCON_BUTTON_CREDITS_NUM] = {NULL};
JD_Rect gPos_CreditsButtons[JCON_BUTTON_CREDITS_NUM];
const char* gTXT_Credits[JCON_LABEL_CREDITS_NUM] = {
    JCON_LABEL_CREDITS_0,
    JCON_LABEL_CREDITS_1,
    JCON_LABEL_CREDITS_2,
    JCON_LABEL_CREDITS_3,
    JCON_LABEL_CREDITS_4,
    JCON_LABEL_CREDITS_5,
    JCON_LABEL_CREDITS_6,
    JCON_LABEL_CREDITS_7,
};
JD_Point gPos_CreditsText;

#define JCON_FONT_MAIN_ID 10
#define JCON_FONT_MAIN_PATH JUICYDEFAULT_FONT_PATH
#define JCON_FONT_MAIN_SIZE 12
#define JCON_FONT_MAIN_COLOR 0xfafafaff
JuicyGUI_Charset* gGUI_FontMain = NULL;
JD_I gGUI_FontMainSize = JCON_FONT_MAIN_SIZE;

#define JCON_FONT_TITLE_ID 11
#define JCON_FONT_TITLE_PATH JUICYDEFAULT_FONT_PATH
#define JCON_FONT_TITLE_SIZE 20
#define JCON_FONT_TITLE_COLOR 0xfafafaff
JuicyGUI_Charset* gGUI_FontTitle = NULL;
JD_I gGUI_FontTitleSize = JCON_FONT_TITLE_SIZE;

#define JCON_LABEL_HUD_NUM 9
JD_Point gPos_FPS;

#define GLOBAL_WINDOW_TITLE "The Game of Life"
#define GLOBAL_WINDOW_SIZE_X 1366
#define GLOBAL_WINDOW_SIZE_Y 768

#define GLOBAL_FLAG_NONE 0x0
#define GLOBAL_FLAG_IN_MENU 0x1
#define GLOBAL_FLAG_USE_VSYNC 0x100
#define GLOBAL_FLAG_NO_QUIT 0x80000000
#define GLOBAL_FLAG_INIT (GLOBAL_FLAG_NONE | GLOBAL_FLAG_IN_MENU | GLOBAL_FLAG_USE_VSYNC | GLOBAL_FLAG_NO_QUIT)

#define GLOBAL_MENU_FLAG_NONE 0x0
#define GLOBAL_MENU_FLAG_MAIN 0x1
#define GLOBAL_MENU_FLAG_SETTINGS 0x2
#define GLOBAL_MENU_FLAG_HELP 0x4
#define GLOBAL_MENU_FLAG_CREDITS 0x8
#define GLOBAL_MENU_FLAG_INIT (GLOBAL_MENU_FLAG_NONE | GLOBAL_MENU_FLAG_MAIN)

#define GLOBAL_CONTROL_KB_SENSITIVITY 10


// SDL Globals
SDL_Window* gSDL_Window = NULL;
SDL_Renderer* gSDL_Renderer = NULL;
SDL_Event gSDL_Event;
Mix_Music* gSDL_Music = NULL;

// App Globals
unsigned int gApp_Flag = GLOBAL_FLAG_INIT;
unsigned int gApp_MenuFlag = GLOBAL_MENU_FLAG_INIT;
unsigned int gApp_numEvents = 0;
unsigned int gApp_TmilDraw = 0;
JuicyGUI_Event gGUI_Event;

// Global variables
JuicyGUI* UI_Main = NULL;

// Function Protos
bool gApp_Init(void);
void gApp_SetMenu(void);
void gApp_Draw();
void gApp_UserInteraction(JD_INDEX iID, JD_FLAG iEvent);
void gApp_SDL_Interaction();
void gApp_Exit(void);
void gGUI_Init(void);
void gGUI_SetSizes();
void gGUI_SizeMetrics();
void gGUI_Exit();
void gMix_Load();
void gMix_Unload();
bool gGame_CreateNewGame();
void gGame_DeleteGame();
void gGame_Wipe();
void gGame_Spawn();


// Conway Specific
ConwaySDL* gGame_Main = NULL;
JD_I gGame_SpawnRatio = 11;
JD_Point gGame_Size;
JD_INDEX gGame_NumFPS = 0;
JD_INDEX gGame_NumIteration = 0;

#endif // MAIN_H_INCLUDED
