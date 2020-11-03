
#include "main.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    gGame_Size.x = 512;
    gGame_Size.y = 256;
    if (gApp_Init()) {
        gMix_Load();
        gGUI_Init();
        gApp_SetMenu();
        if (gSDL_Music != NULL) {
            Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
            Mix_PlayMusic(gSDL_Music, -1);
        }
        while (gApp_Flag & GLOBAL_FLAG_NO_QUIT){
            while (UI_Main->PollEvents(&gGUI_Event)) {gApp_UserInteraction(gGUI_Event.id, gGUI_Event.type);}
            while(SDL_PollEvent(&gSDL_Event)) {gApp_SDL_Interaction();}
			if (gGame_Main != NULL) {
                JD_Point mouseMove;
                JD_FLAG mouseState = UI_Main->GetMouseMovement(&mouseMove);
                if ((gApp_Flag & GLOBAL_FLAG_IN_MENU) ? false : gApp_numEvents ? false : true) {
                    if ((mouseState & JUICYGUI_CONTROL_ID_LMB_CLICKED)) {gGame_Main->FlipCell();}
                    if (mouseState & JUICYGUI_CONTROL_ID_RMB) {
                        JD_Point moveVector = -mouseMove;
                        gGame_Main->Move(&moveVector);
                    }
                }
                gGame_Main->LoopHandle();
			}
			if (gApp_Flag & GLOBAL_FLAG_USE_VSYNC) {
                if ((UI_Main->GetTicks() - gApp_TmilDraw) > 16) {
                    gApp_Draw();
                    gApp_TmilDraw = UI_Main->GetTicks() - ((UI_Main->GetTicks() - gApp_TmilDraw) % 17);
                }
			} else {
			    gApp_Draw();
            }
        }
    } else {
        gApp_Exit();
        return 1;
    }
	gApp_Exit();
	return 0;
}



bool gApp_Init(void) {
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0) {
        gSDL_Window = SDL_CreateWindow(GLOBAL_WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GLOBAL_WINDOW_SIZE_X, GLOBAL_WINDOW_SIZE_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (gSDL_Window != NULL) {
            gSDL_Renderer = SDL_CreateRenderer(gSDL_Window, -1, SDL_RENDERER_ACCELERATED | ((gApp_Flag & GLOBAL_FLAG_USE_VSYNC) ? SDL_RENDERER_PRESENTVSYNC : 0x0));
            if (gSDL_Renderer != NULL) {
                SDL_RendererInfo rendererInfo;
                SDL_GetRendererInfo(gSDL_Renderer, &rendererInfo);
                if (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) {
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0) {
                        if (IMG_Init(IMG_INIT_PNG) == 2) {
                            if (TTF_Init() == 0) {
                                return true;
                            } else {
                                std::cout << "TTF_Init() error!" << std::endl;
                            }
                        } else {
                            std::cout << "IMG_Init(IMG_INIT_PNG) error!" << std::endl;
                        }
                    } else {
                        std::cout << "Mix_OpenAudio() error!" << std::endl;
                    }
                } else {
                    std::cout << "Unsupported renderer!" << std::endl;
                }
            } else {
                std::cout << "SDL_CreateRenderer error! " << SDL_GetError() << std::endl;
            }
        } else {
            std::cout << "SDL_CreateWindow error! " << SDL_GetError() << std::endl;
        }
    } else {
        std::cout << "SDL_Init error! " << SDL_GetError() << std::endl;
    }
	return false;
}

void gApp_Exit() {
    if (gGame_Main != NULL) {delete gGame_Main;}
    gMix_Unload();
    gGUI_Exit();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
	SDL_DestroyRenderer(gSDL_Renderer);
	SDL_DestroyWindow(gSDL_Window);
	SDL_Quit();
}

void gApp_Draw() {
    SDL_RenderClear(gSDL_Renderer);
    UI_Main->GetEngine()->FillScreen(0x2b2b2bff);
    if (gGame_Main != NULL) {
        gGame_Main->Draw();
    }
    if (gApp_Flag & GLOBAL_FLAG_IN_MENU) {
        JD_Rect cacheRectMenu = *(gGUI_MenuBox->GetRect());
        JD_Point cachePos;
        JD_Point cacheLabelSize;
        gGUI_FontTitle->GetSizeTXT(gTXT_MenuLabelPtr, &cacheLabelSize);
        cachePos.x = cacheRectMenu.x + ((cacheRectMenu.w - cacheLabelSize.x) / 2);
        cachePos.y = cacheRectMenu.y + (1 * cacheRectMenu.h / 8) - (cacheLabelSize.y / 2);
        gGUI_FontTitle->PrintlnTXT(gTXT_MenuLabelPtr, &cachePos);
        switch (gApp_MenuFlag) {
            case GLOBAL_MENU_FLAG_MAIN:
                break;
            case GLOBAL_MENU_FLAG_SETTINGS:
                break;
            case GLOBAL_MENU_FLAG_HELP:
                gGUI_FontMain->SetCursor(&gPos_HelpText);
                for (unsigned int i = 0; i < JCON_LABEL_HELP_NUM; i++) {
                    gGUI_FontMain->CenterlnTXT(gTXT_Help[i], NULL);
                }
                break;
            case GLOBAL_MENU_FLAG_CREDITS:
                gGUI_FontMain->SetCursor(&gPos_CreditsText);
                for (unsigned int i = 0; i < JCON_LABEL_CREDITS_NUM; i++) {
                    gGUI_FontMain->CenterlnTXT(gTXT_Credits[i], NULL);
                }
                break;
            default:
                break;
        }
    } else {
        if (gGame_Main != NULL) {
            gGUI_FontMain->PrintTXT("fps: ", &gPos_FPS);
            gGUI_FontMain->PrintlnINT(gGame_Main->GetFPS(), NULL);
            gGUI_FontMain->PrintTXT("T: ", NULL);
            gGUI_FontMain->PrintINT(gGame_Main->GetPeriod(), NULL);
            gGUI_FontMain->PrintlnTXT(" ms", NULL);
            gGUI_FontMain->PrintTXT("1/T: ", NULL);
            gGUI_FontMain->PrintINT(1000 / gGame_Main->GetPeriod(), NULL);
            gGUI_FontMain->PrintlnTXT(" Hz", NULL);
            gGUI_FontMain->PrintTXT("ips: ", NULL);
            gGUI_FontMain->PrintlnINT(gGame_Main->GetIPS(), NULL);
            gGUI_FontMain->PrintTXT("i: ", NULL);
            gGUI_FontMain->PrintlnINT(gGame_Main->GetIteration(), NULL);
            gGUI_FontMain->PrintTXT("n_nom: ", NULL);
            gGUI_FontMain->PrintINT(gGame_SpawnRatio, NULL);
            gGUI_FontMain->PrintlnTXT("%", NULL);
            gGUI_FontMain->PrintTXT("n/N: ", NULL);
            gGUI_FontMain->PrintINT((gGame_Main->GetRatio() / 100), NULL);
            gGUI_FontMain->PrintTXT(".", NULL);
            gGUI_FontMain->PrintINT((gGame_Main->GetRatio() % 100) / 10, NULL);
            gGUI_FontMain->PrintINT((gGame_Main->GetRatio() % 10), NULL);
            gGUI_FontMain->PrintlnTXT("%", NULL);
            gGUI_FontMain->PrintTXT("n: ", NULL);
            gGUI_FontMain->PrintlnINT(gGame_Main->GetCount(), NULL);
            gGUI_FontMain->PrintTXT("N: ", NULL);
            JD_Point sizeField;
            gGame_Main->GetSize(&sizeField);
            gGUI_FontMain->PrintlnINT(JDM_GetArea(&sizeField), NULL);
        }
    }
    UI_Main->GetEngine()->ModScreen(UI_Main->GetJuicy());
    UI_Main->DrawElements();
    SDL_RenderPresent(gSDL_Renderer);
}



void gGUI_Init(void) {
    UI_Main = new JuicyGUI(gSDL_Window, gSDL_Renderer);
    gGUI_MenuBox = new JuicyGUI_Box(UI_Main, JCON_BOX_MENU_ID, &gPos_Menu);
    for (unsigned int i = 0; i < JCON_BOX_HUD_NUM; i++) {gGUI_HUDBox[i] = new JuicyGUI_Box(UI_Main, gGUI_HUDBoxIDs[i], &gPos_HUD[i]);}
    for (unsigned int i = 0; i < JCON_BUTTON_HUD_NUM; i++) {gGUI_HUDButtons[i] = new JuicyGUI_Button(UI_Main, gGUI_HUDButtonIDs[i], gTXT_HUDButtonLabels[i], NULL);}
    for (unsigned int i = 0; i < JCON_BUTTON_MENU_NUM; i++) {gGUI_MenuButtons[i] = new JuicyGUI_Button(UI_Main, gGUI_MenuButtonIDs[i], gTXT_MenuButtonLabels[i], NULL);}
    for (unsigned int i = 0; i < JCON_BUTTON_SETTINGS_NUM; i++) {gGUI_SettingsButtons[i] = new JuicyGUI_Button(UI_Main, gGUI_SettingsButtonIDs[i], gTXT_SettingsButtonLabels[i], NULL);}
    for (unsigned int i = 0; i < JCON_BUTTON_HELP_NUM; i++) {gGUI_HelpButtons[i] = new JuicyGUI_Button(UI_Main, gGUI_HelpButtonIDs[i], gTXT_HelpButtonLabels[i], NULL);}
    for (unsigned int i = 0; i < JCON_BUTTON_CREDITS_NUM; i++) {gGUI_CreditsButtons[i] = new JuicyGUI_Button(UI_Main, gGUI_CreditspButtonIDs[i], gTXT_CreditsButtonLabels[i], NULL);}
    gGUI_FontTitle = new JuicyGUI_Charset(UI_Main, JCON_FONT_TITLE_ID, JCON_FONT_TITLE_PATH, JCON_FONT_TITLE_SIZE, JCON_FONT_TITLE_COLOR);
    gGUI_FontMain = new JuicyGUI_Charset(UI_Main, JCON_FONT_MAIN_ID, JCON_FONT_MAIN_PATH, JCON_FONT_MAIN_SIZE, JCON_FONT_MAIN_COLOR);
    gGUI_SetSizes();
}

void gGUI_Exit() {
    delete UI_Main;
}

void gGUI_SetSizes() {
    gGUI_SizeMetrics();
    gGUI_MenuBox->SetRect(&gPos_Menu);
    for (unsigned int i = 0; i < JCON_BOX_HUD_NUM; i++) {gGUI_HUDBox[i]->SetRect(&(gPos_HUD[i]));}
    for (unsigned int i = 0; i < JCON_BUTTON_HUD_NUM; i++) {gGUI_HUDButtons[i]->SetRect(&(gPos_HUDButtons[i]));}
    for (unsigned int i = 0; i < JCON_BUTTON_MENU_NUM; i++) {gGUI_MenuButtons[i]->SetRect(&(gPos_MenuButtons[i]));}
    for (unsigned int i = 0; i < JCON_BUTTON_SETTINGS_NUM; i++) {gGUI_SettingsButtons[i]->SetRect(&(gPos_SettingsButtons[i]));}
    for (unsigned int i = 0; i < JCON_BUTTON_HELP_NUM; i++) {gGUI_HelpButtons[i]->SetRect(&(gPos_HelpButtons[i]));}
    for (unsigned int i = 0; i < JCON_BUTTON_CREDITS_NUM; i++) {gGUI_CreditsButtons[i]->SetRect(&(gPos_CreditsButtons[i]));}
    gGUI_FontMain->SetFontSize(gGUI_FontMainSize);
    gGUI_FontTitle->SetFontSize(gGUI_FontTitleSize);
}
void gGUI_SizeMetrics() {
    JD_Point screenSize;
    UI_Main->GetScreenSize(&screenSize);

    gPos_Menu.w = screenSize.x / JCON_BOX_MENU_WIDTH_DIVISOR;
    gPos_Menu.h = screenSize.y / JCON_BOX_MENU_HEIGHT_DIVISOR;
    gPos_Menu.x = (screenSize.x - gPos_Menu.w) / 2;
    gPos_Menu.y = (screenSize.y - gPos_Menu.h) / 2;

    gGUI_FontMainSize = JCON_FONT_MAIN_SIZE * screenSize.y / GLOBAL_WINDOW_SIZE_Y;
    gGUI_FontTitleSize = JCON_FONT_TITLE_SIZE * screenSize.y / GLOBAL_WINDOW_SIZE_Y;

    gPos_CreditsText.x = gPos_Menu.x + gPos_Menu.w / 2;
    gPos_CreditsText.y = gPos_Menu.y + (gPos_Menu.h * 2) / 8 - gGUI_FontMainSize / 2;
    gPos_HelpText = gPos_CreditsText;

    JD_Rect buttonRect;
    buttonRect.w = screenSize.x / JCON_BUTTON_MENU_WIDTH_DIVISOR;
    buttonRect.h = screenSize.y / JCON_BUTTON_MENU_HEIGHT_DIVISOR;
    buttonRect.x = (screenSize.x - buttonRect.w) / 2;
    for (unsigned int i = 0; i < JCON_BUTTON_MENU_NUM; i++) {
        buttonRect.y = gPos_Menu.y + (((gPos_Menu.h * (2 + i)) / 8) - buttonRect.h / 2);
        gPos_MenuButtons[i] = buttonRect;
    }
    gPos_SettingsButtons[0] = buttonRect;
    gPos_HelpButtons[0] = buttonRect;
    gPos_CreditsButtons[0] = buttonRect;

    buttonRect.w = screenSize.x / JCON_BUTTON_HUD_WIDTH_DIVISOR;
    buttonRect.h = screenSize.y / JCON_BUTTON_HUD_HEIGHT_DIVISOR;
    buttonRect.x = screenSize.x - (buttonRect.w + buttonRect.h / 2);
    buttonRect.y = buttonRect.h / 2;
    gPos_HUDButtons[0] = buttonRect;
    gPos_HUD[1].w = buttonRect.w + buttonRect.h + 2 * JCON_BOX_HUD_FRAMESIZE;
    gPos_HUD[1].h = 2 * (buttonRect.h + JCON_BOX_HUD_FRAMESIZE);
    gPos_HUD[1].x = screenSize.x - gPos_HUD[1].w;
    gPos_HUD[1].y = 0 - JCON_BOX_HUD_FRAMESIZE;
    buttonRect.x = buttonRect.h / 2;
    for (unsigned int i = 0; i < (JCON_BUTTON_HUD_NUM - 1); i++) {
        gPos_HUDButtons[i + 1] = buttonRect;
        buttonRect.y += (3 * buttonRect.h / 2);
    }
    gPos_HUD[0].w = buttonRect.w + buttonRect.h + 2 * JCON_BOX_HUD_FRAMESIZE;
    gPos_HUD[0].h = 2 * JCON_BOX_HUD_FRAMESIZE + screenSize.y;
    gPos_HUD[0].x = 0;
    gPos_HUD[0].y = 0 - JCON_BOX_HUD_FRAMESIZE;
    JDM_GetRectPos(&buttonRect, &gPos_FPS);
    gPos_FPS.y = screenSize.y - (JCON_LABEL_HUD_NUM * gGUI_FontMainSize * 7 / 4);
}

void gMix_Load() {
    gSDL_Music = Mix_LoadMUS(MUSIC_PATH_1);
}
void gMix_Unload() {
    if (gSDL_Music != NULL) {
        Mix_FreeMusic(gSDL_Music);
        gSDL_Music = NULL;
    }
}

void gApp_SetMenu(void) {
    bool inMenu = gApp_Flag & GLOBAL_FLAG_IN_MENU;
    gGUI_MenuBox->Show(inMenu);
    gGUI_FontTitle->Show(inMenu);
    gTXT_MenuLabelPtr = gTXT_MenuLabels[JDM_GetBitIndex(gApp_MenuFlag)];
    for (unsigned int i = 0; i < JCON_BOX_HUD_NUM; i++) {
        gGUI_HUDBox[i]->Show(!inMenu);
    }
    for (unsigned int i = 0; i < JCON_BUTTON_HUD_NUM; i++) {
        gGUI_HUDButtons[i]->Show(!inMenu);
    }
    for (unsigned int i = 0; i < JCON_BUTTON_MENU_NUM; i++) {
        gGUI_MenuButtons[i]->Show(inMenu && (gApp_MenuFlag & GLOBAL_MENU_FLAG_MAIN));
    }
    for (unsigned int i = 0; i < JCON_BUTTON_SETTINGS_NUM; i++) {
        gGUI_SettingsButtons[i]->Show(inMenu && (gApp_MenuFlag & GLOBAL_MENU_FLAG_SETTINGS));
    }
    for (unsigned int i = 0; i < JCON_BUTTON_HELP_NUM; i++) {
        gGUI_HelpButtons[i]->Show(inMenu && (gApp_MenuFlag & GLOBAL_MENU_FLAG_HELP));
    }
    for (unsigned int i = 0; i < JCON_BUTTON_CREDITS_NUM; i++) {
        gGUI_CreditsButtons[i]->Show(inMenu && (gApp_MenuFlag & GLOBAL_MENU_FLAG_CREDITS));
    }
}

bool gGame_CreateNewGame() {
    if (gGame_Main == NULL) {
        gGame_Main = new ConwaySDL(UI_Main, gGame_Size.x, gGame_Size.y);
        if (gGame_Main != NULL) {
            gGame_Main->SetZoom(1);
            gGame_Spawn();
            return true;
        }
    }
    return false;
}
void gGame_DeleteGame() {
    if (gGame_Main != NULL) {
        delete gGame_Main;
        gGame_Main = NULL;
    }
}
void gGame_Wipe() {
    if (gGame_Main != NULL) {
        JD_Point cellNum;
        JD_Point gameSize;
        gGame_Main->GetSize(&gameSize);
        for (cellNum.y = 0; cellNum.y < gameSize.y; cellNum.y++) {
            for (cellNum.x = 0; cellNum.x < gameSize.x; cellNum.x++) {
                gGame_Main->SetCell(&cellNum, false);
            }
        }
        gGame_Main->ForceUpdateStats();
    }
}
void gGame_Spawn() {
    if (gGame_Main != NULL) {
        JD_Point cellNum;
        JD_Point gameSize;
        gGame_Main->GetSize(&gameSize);
        for (cellNum.y = 0; cellNum.y < gameSize.y; cellNum.y++) {
            for (cellNum.x = 0; cellNum.x < gameSize.x; cellNum.x++) {
                gGame_Main->SetCell(&cellNum, (((rand() % 100) + 1) > (100 - gGame_SpawnRatio)));
            }
        }
        gGame_Main->ForceUpdateStats();
    }
}

void gApp_UserInteraction(JD_INDEX iID, JD_FLAG iEvent) {
    if (iEvent & JUICYGUI_EVENT_PRESSED) {
        switch (iID) {
            case JCON_BUTTON_HUD_ID_RATIO_UP: {
                    static JD_TIME tmilLast = UI_Main->GetTicks();
                    if (gGame_SpawnRatio < 100 && (UI_Main->GetTicks() - tmilLast) >= 200) {
                        tmilLast = UI_Main->GetTicks();
                        gGame_SpawnRatio++;
                    }
                }
                break;
            case JCON_BUTTON_HUD_ID_RATIO_DOWN: {
                    static JD_TIME tmilLast = UI_Main->GetTicks();
                    if (gGame_SpawnRatio && (UI_Main->GetTicks() - tmilLast) >= 200) {
                        tmilLast = UI_Main->GetTicks();
                        gGame_SpawnRatio--;
                    }
                }
                break;
            default:
                break;
        }
    }
    if (iEvent & JUICYGUI_EVENT_RELEASED) {
        switch (iID) {
            case JCON_BUTTON_HUD_ID_MENU:
                if (gApp_Flag & GLOBAL_FLAG_IN_MENU) {
                    gApp_Flag &= ~GLOBAL_FLAG_IN_MENU;
                } else {
                    gApp_Flag |= GLOBAL_FLAG_IN_MENU;
                    if (gGame_Main != NULL) {gGame_Main->Pause();}
                }
                gApp_SetMenu();
                break;
            case JCON_BUTTON_HUD_ID_WIPE:
                gGame_Wipe();
                break;
            case JCON_BUTTON_HUD_ID_SPAWN:
                gGame_Spawn();
                break;
            case JCON_BUTTON_HUD_ID_FASTER:
                if (gGame_Main != NULL) {
                    gGame_Main->RunFaster();
                }
                break;
            case JCON_BUTTON_HUD_ID_SLOWER:
                if (gGame_Main != NULL) {
                    gGame_Main->RunSlower();
                }
                break;
            case JCON_BUTTON_HUD_ID_RUN:
                if (gGame_Main != NULL) {
                    gGame_Main->TogglePause();
                }
                break;
            case JCON_BUTTON_HUD_ID_ZOOM_IN:
                if (gGame_Main != NULL) {
                    gGame_Main->Zoom(1);
                }
                break;
            case JCON_BUTTON_HUD_ID_ZOOM_OUT:
                if (gGame_Main != NULL) {
                    gGame_Main->Zoom(-1);
                }
                break;
            case JCON_BUTTON_HUD_ID_GRID:
                if (gGame_Main != NULL) {
                    gGame_Main->ToggleGrid();
                }
                break;
            case JCON_BUTTON_HUD_ID_HOME:
                if (gGame_Main != NULL) {
                    gGame_Main->SetZoom(1);
                    gGame_Main->ResetCamera();
                }
                break;
            case JCON_BUTTON_MENU_ID_PLAY:
                gApp_Flag &= ~GLOBAL_FLAG_IN_MENU;
                if (gGame_CreateNewGame()) {gGame_Main->Unpause();}
                gApp_SetMenu();
                break;
            case JCON_BUTTON_MENU_ID_RESET:
                gGame_DeleteGame();
                break;
            case JCON_BUTTON_MENU_ID_SETTINGS:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_MAIN;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_SETTINGS;
                gApp_SetMenu();
                break;
            case JCON_BUTTON_MENU_ID_HELP:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_MAIN;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_HELP;
                gApp_SetMenu();
                break;
            case JCON_BUTTON_MENU_ID_CREDITS:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_MAIN;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_CREDITS;
                gApp_SetMenu();
                break;
            case JCON_BUTTON_MENU_ID_EXIT:
                gApp_Flag &= ~GLOBAL_FLAG_NO_QUIT;
                break;
            case JCON_BUTTON_SETTINGS_ID_BACK:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_SETTINGS;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_MAIN;
                gApp_SetMenu();
                break;
            case JCON_BUTTON_HELP_ID_BACK:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_HELP;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_MAIN;
                gApp_SetMenu();
                break;
            case JCON_BUTTON_CREDITS_ID_BACK:
                gApp_MenuFlag &= ~GLOBAL_MENU_FLAG_CREDITS;
                gApp_MenuFlag |= GLOBAL_MENU_FLAG_MAIN;
                gApp_SetMenu();
                break;
            default:
                break;
        }
    } else {
        switch (iID) {
            case JUICYGUI_ID:
                if (iEvent & JUICYGUI_EVENT_RESIZE) {
                    gGUI_SetSizes();
                }
                break;
            default:
                break;
        }
    }
}

void gApp_SDL_Interaction() {
    if (gGame_Main != NULL && !(gApp_Flag & GLOBAL_FLAG_IN_MENU)) {
        if (gSDL_Event.type == SDL_KEYUP) {
            switch (gSDL_Event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    gGame_Main->Pause();
                    break;
                case SDLK_SPACE:
                    gGame_Main->TogglePause();
                    break;
                case SDLK_e:
                    gGame_Main->ToggleEdge();
                    break;
                case SDLK_g:
                    gGame_Main->ToggleGrid();
                    break;
                case SDLK_h:
                case SDLK_HOME:
                    gGame_Main->SetZoom(1);
                    gGame_Main->ResetCamera();
                    break;
                case SDLK_F5:
                    break;
                default:
                    break;
            }
        }
        if (gSDL_Event.type == SDL_KEYDOWN) {
            JD_Point moveVector;
            JDM_EmptyPoint(&moveVector);
            switch (gSDL_Event.key.keysym.sym) {
                case SDLK_w:
                    moveVector.y = -GLOBAL_CONTROL_KB_SENSITIVITY;
                    break;
                case SDLK_a:
                    moveVector.x = -GLOBAL_CONTROL_KB_SENSITIVITY;
                    break;
                case SDLK_s:
                    moveVector.y = GLOBAL_CONTROL_KB_SENSITIVITY;
                    break;
                case SDLK_d:
                    moveVector.x = GLOBAL_CONTROL_KB_SENSITIVITY;
                    break;
                case SDLK_PAGEUP:
                case SDLK_KP_PLUS:
                    gGame_Main->Zoom(1);
                    break;
                case SDLK_PAGEDOWN:
                case SDLK_KP_MINUS:
                    gGame_Main->Zoom(-1);
                    break;
                default:
                    break;
            }
            if (JDM_IsNotEmptyPoint(&moveVector)) {gGame_Main->Move(&moveVector);}
        }
        if (gSDL_Event.type == SDL_MOUSEWHEEL) {
            gGame_Main->ZoomAndCenter(gSDL_Event.wheel.y);
        }
    }
    if (gGame_Main != NULL) {
        if (gSDL_Event.type == SDL_KEYUP && gSDL_Event.key.keysym.sym == SDLK_ESCAPE) {
            gApp_Flag ^= GLOBAL_FLAG_IN_MENU;
            gApp_SetMenu();
        }
    }
    if(gSDL_Event.type == SDL_QUIT) {
        gApp_Flag &= ~GLOBAL_FLAG_NO_QUIT;
    }
}
