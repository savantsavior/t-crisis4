/*
    Copyright 2025 Team 16BitSoft

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
    AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cmath>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "screens.h"

#include "input.h"
#include "visuals.h"
#include "interface.h"
#include "data.h"
#include "logic.h"
#include "audio.h"

extern Input* input;
extern Visuals* visuals;
extern Interface* interface;
extern Data* data;
extern Logic* logic;
extern Audio* audio;

//-------------------------------------------------------------------------------------------------
Screens::Screens(void)
{
    ScreenIsDirty = 2;

    ScreenToDisplay = CNETScreen;

    ScreenFadeTransparency = 255;
    ScreenTransitionStatus = FadeAll;
}

//-------------------------------------------------------------------------------------------------
Screens::~Screens(void)
{

}

//-------------------------------------------------------------------------------------------------
void Screens::ApplyScreenFadeTransition(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        visuals->ClearTextCache();
    }
    else if (ScreenTransitionStatus == FadeIn)
    {
        if ( ScreenFadeTransparency > (255/3) )
        {
            ScreenFadeTransparency -= (255/3);
            ScreenIsDirty = 2;
        }
        else
        {
            ScreenFadeTransparency = 0;
            ScreenTransitionStatus = FadeNone;
        }
    }
    else if (ScreenTransitionStatus == FadeOut)
    {
        if ( ScreenFadeTransparency < (255/3) )
        {
            ScreenFadeTransparency += (255/3);
            ScreenIsDirty = 2;
        }
        else
        {
            ScreenFadeTransparency = 255;

            interface->DestroyAllButtons();
            interface->DestroyAllArrowSets();
            interface->DestroyAllIcons();
        }
    }

    if (ScreenTransitionStatus != FadeNone)
    {
        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = ScreenFadeTransparency;
        visuals->DrawSpriteOntoScreenBuffer(0);
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::ProcessScreenToDisplay(void)
{
int windowWidth;
int windowHeight;

    if (input->DEBUG > 0)  ScreenIsDirty = 2;

    if (input->LastEventWasWindowResize > 0)
    {
        ScreenIsDirty = 2;
    }

    SDL_GetWindowSize(visuals->Window, &windowWidth, &windowHeight);
    if (windowWidth != visuals->WindowWidthCurrent || windowHeight != visuals->WindowHeightCurrent)
    {
        visuals->WindowWidthCurrent = windowWidth;
        visuals->WindowHeightCurrent = windowHeight;

        visuals->ClearTextCache();

        ScreenIsDirty = 2;
    }

    if (input->JoystickSetupProcess == JoySetupNotStarted && input->KeyboardSetupProcess == KeyboardSetupNotStarted)
    {
        interface->ProcessAllButtons();
        interface->ProcessAllArrowSets();
        interface->ProcessAllIcons();
    }

    if (input->LastEventWasWindowResize == 0)
    {
        switch(ScreenToDisplay)
        {
            case SDLLogoScreen:
                DisplaySDLLogoScreen();
                break;

            case SixteenBitSoftScreen:
                ScreenToDisplay = TitleScreen;
                break;

            case TitleScreen:
                DisplayTitleScreen();
                break;

            case OptionsScreen:
                DisplayOptionsScreen();
                break;

            case HowToPlayScreen:
                DisplayHowToPlayScreen();
                break;

            case HighScoresScreen:
                DisplayHighScoresScreen();
                break;

            case AboutScreen:
                DisplayAboutScreen();
                break;

            case NewGameOptionsScreen:
                DisplayNewGameOptionsScreen();
                break;

            case PlayingGameScreen:
                DisplayPlayingGameScreen();
                break;

            case PlayingStoryGameScreen:
                DisplayPlayingStoryGameScreen();
                break;

            case ShowStoryScreen:
                DisplayShowStoryScreen();
                break;

            case FlyingFromEarthScreen:
                DisplayFlyingFromEarthScreen();
                break;

            case FlyingToMarsScreen:
                DisplayFlyingToMarsScreen();
                break;

            case FlyingToBaseScreen:
                DisplayFlyingToBaseScreen();
                break;

            case MarsExplodingScreen:
                DisplayMarsExplodingScreen();
                break;

            case TestComputerSkillScreen:
                DisplayTestComputerSkillScreen();
                break;

            case NameInputKeyboardScreen:
                DisplayNameInputKeyboardScreen();
                break;

            case NameInputJoystickScreen:
                DisplayNameInputJoystickScreen();
                break;

            case NameInputMouseScreen:
                DisplayNameInputMouseScreen();
                break;

            case GiveMeJobScreen:
                DisplayGiveMeJobScreen();
                break;

            case CNETScreen:
                DisplayCNETScreen();
                break;

            default:
                break;
        }

        if (input->JoystickSetupProcess == JoySetupNotStarted && input->KeyboardSetupProcess == KeyboardSetupNotStarted)
            interface->DisplayAllButtonsOntoScreenBuffer();

        interface->DisplayAllIconsOntoScreenBuffer();
    }
    else
    {
        ScreenIsDirty = 2;
        input->LastEventWasWindowResize--;

        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        SDL_SetRenderDrawColor(visuals->Renderer, 0, 0, 0, 255);
        SDL_RenderClear(visuals->Renderer);

        SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);
        SDL_RenderPresent(visuals->Renderer);
    }

    if (ScreenToDisplay == CNETScreen || ScreenToDisplay == SDLLogoScreen || ScreenToDisplay == SixteenBitSoftScreen)
    {
        EscKeyScaleTimer++;
        if (EscKeyScaleTimer == 15)
        {
            EscKeyScale = 0.85f;
            ScreenIsDirty = 2;
        }
        else if (EscKeyScaleTimer > 25)
        {
            EscKeyScale = 1.0f;
            EscKeyScaleTimer = 0;
            ScreenIsDirty = 2;
        }

        visuals->Sprites[19].ScreenX = 640 - 30;
        visuals->Sprites[19].ScreenY = 480 - 30;
        visuals->Sprites[19].ScaleX = EscKeyScale;
        visuals->Sprites[19].ScaleY = EscKeyScale;
        visuals->DrawSpriteOntoScreenBuffer(19);
    }

    ApplyScreenFadeTransition();

    if (input->DEBUG > 0 && logic->GameMode != OriginalMode)
    {
        //ScreenIsDirty = 2;

        if (input->DEBUG > 0)
        {
//            char temp[256];
//            SDL_strlcpy(visuals->VariableText, "(", sizeof visuals->VariableText);
//            SDL_snprintf (temp, sizeof temp, "%i", input->MouseX);
//            SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
//            SDL_strlcat(visuals->VariableText, ",", sizeof visuals->VariableText);
//            SDL_snprintf (temp, sizeof temp, "%i", input->MouseY);
//            SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
//            SDL_strlcat(visuals->VariableText, ")", sizeof visuals->VariableText);
//            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 3, 440
//                                              , JustifyLeft, 255, 255, 255, 0, 0, 0);
        }

        SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", visuals->AverageFPS);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 3, 460
                                          , JustifyLeft, 255, 255, 255, 0, 0, 0);
    }

    if (ScreenIsDirty > 0)
    {
        if (visuals->ForceAspectRatio == true)  SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);
        else  SDL_RenderSetLogicalSize(visuals->Renderer, 900, 480);

        SDL_RenderPresent(visuals->Renderer);
        if (ScreenTransitionStatus != FadeIn && ScreenTransitionStatus != FadeOut)
        {
            if (ScreenIsDirty > 0)  ScreenIsDirty--;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplaySDLLogoScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 610/2;
        ScreenTransitionStatus = FadeIn;

        audio->PlayMusic(0, -1);
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_d)
    {
        if (input->DEBUG == 0)  input->DEBUG = 1;
        else  input->DEBUG = 0;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }
    else if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_t)
    {
        ScreenToDisplay = TestComputerSkillScreen;
        ScreenTransitionStatus = FadeAll;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);

        audio->SoundVolume = 0;
        audio->MusicVolume = 0;

        audio->PlayMusic(0, -1);

        logic->PlayersCanJoin = false;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[9].ScreenX = 320;
        visuals->Sprites[9].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(9);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = SixteenBitSoftScreen;

        if (input->UserPressedESC == 1)
        {
            input->UserPressedESC = 1999;
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplaySixteenBitSoftScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 610/2;
        ScreenTransitionStatus = FadeIn;
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_d)
    {
        if (input->DEBUG == 0)  input->DEBUG = 1;
        else  input->DEBUG = 0;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }
    else if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_t)
    {
        ScreenToDisplay = TestComputerSkillScreen;
        ScreenTransitionStatus = FadeAll;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);

        audio->SoundVolume = 0;
        audio->MusicVolume = 0;

        audio->PlayMusic(0, -1);

        logic->PlayersCanJoin = false;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }
    else if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_c)
    {
        logic->TestMode = true;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[1].ScreenX = 320;
        visuals->Sprites[1].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(1);

        visuals->DrawTextOntoScreenBuffer("JeZxLee's ''GT-R Twin TurboCharged'' SDL2 Engine Framework", visuals->Font[7]
                                          , 0, 75, JustifyCenter, 1, 255, 1, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Bringing back old memories from the 16bit era!", visuals->Font[1]
                                          , 0, 307, JustifyCenter, 1, 255, 1, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Team 16BitSoft", visuals->Font[0]
                                          , 0, 447, JustifyCenter, 1, 255, 1, 1, 90, 1);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayTitleScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        if (audio->TitleSpoken == false)
        {
            audio->PlayDigitalSoundFX(18, 0);
            audio->TitleSpoken = true;
        }

        float buttonStartY = 217.0f;
        float buttonOffsetY = 43.0f;
        interface->CreateButton( 1003, 0, buttonStartY );
        interface->CreateButton( 1004, 1, buttonStartY + (buttonOffsetY*1) );
        interface->CreateButton( 1005, 2, buttonStartY + (buttonOffsetY*2) );
        interface->CreateButton( 1006, 3, buttonStartY + (buttonOffsetY*3) );
        interface->CreateButton( 1007, 4, buttonStartY + (buttonOffsetY*4) );
        interface->CreateButton( 1008, 5, buttonStartY + (buttonOffsetY*5) );

        interface->CreateIcon(6, 640-86, 480-110-20);

        input->SteamDeckSetupGamePad = false;

        SkipEntireStoryCutsceneAndPlay = false;

        ScreenTransitionStatus = FadeIn;
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->DrawTextOntoScreenBuffer("JeZxLee's", visuals->Font[7]
                                          , 360, 3, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[3].ScreenX = 320;
        visuals->Sprites[3].ScreenY = 100;
        visuals->Sprites[3].Smooth = true;
        visuals->DrawSpriteOntoScreenBuffer(3);

        visuals->DrawTextOntoScreenBuffer("A.I. 691,000+", visuals->Font[9]
                                          , 577+6, 80+6, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Line Average", visuals->Font[9]
                                          , 577+6, 96+6, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        char temp[256];
        SDL_strlcpy(visuals->VariableText, "''", sizeof visuals->VariableText);
        SDL_strlcat(visuals->VariableText, data->HighScoresName[logic->GameMode][0], sizeof visuals->VariableText);
        SDL_strlcat(visuals->VariableText, "'' Scored: ", sizeof visuals->VariableText);

        #ifdef _WIN32
            SDL_snprintf (temp, sizeof temp, "%I64u", data->HighScoresScore[logic->GameMode][0]);
        #else
            sprintf(temp, "%lu", data->HighScoresScore[logic->GameMode][0]);
        #endif

        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 0, 135+16+12
                                          , JustifyCenter, 255, 255, 0, 1, 1, 1);

//        visuals->DrawTextOntoScreenBuffer("Click For", visuals->Font[7]
//                                          , 640-82, 240-20+40, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

//        visuals->DrawTextOntoScreenBuffer("other games!", visuals->Font[7]
//                                          , 640-82, 240+20+2-20+40+20, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("CNET Tribute", visuals->Font[7]
                                          , 640-82, 462-5-20, JustifyCenterOnPoint, 255, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("Edition # 3", visuals->Font[7]
                                          , 640-82, 462-5-20+20, JustifyCenterOnPoint, 255, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Team 16BitSoft", visuals->Font[0]
                                          , 0, 447+5, JustifyCenter, 255, 255, 255, 1, 1, 1);
    }

    if (interface->IconSelectedByPlayer == 0)
    {
        #ifdef __unix__
            system( "xdg-open 'https://teamjezxlee.itch.io/'");
        #elif defined(_WIN32)
            SDL_OpenURL("https://teamjezxlee.itch.io/");
        #endif
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (interface->ButtonSelectedByPlayer == 1)  ScreenToDisplay = OptionsScreen;
        else if (interface->ButtonSelectedByPlayer == 2)  ScreenToDisplay = HowToPlayScreen;
        else if (interface->ButtonSelectedByPlayer == 3)  ScreenToDisplay = HighScoresScreen;
        else if (interface->ButtonSelectedByPlayer == 4)  ScreenToDisplay = AboutScreen;
        else if (interface->ButtonSelectedByPlayer == 5)  //ScreenToDisplay = GiveMeJobScreen;
        {
            input->EXIT_Game = true;
            ScreenTransitionStatus = FadeAll;
        }
        else if (interface->ButtonSelectedByPlayer == 0)
        {
            if (logic->GameMode < StoryMode)
                ScreenToDisplay = NewGameOptionsScreen;
            else
                ScreenToDisplay = NewGameOptionsScreen;
        }

        if (input->SteamDeckSetupGamePad == true)
        {
            ScreenToDisplay = OptionsScreen;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNewGameOptionsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1003, 0, 454);

        interface->CreateArrowSet(0, 65);
        interface->CreateArrowSet(1, 105);
        interface->CreateArrowSet(2, 145);
        interface->CreateArrowSet(3, 185);
        interface->CreateArrowSet(4, 225);
        interface->CreateArrowSet(5, 265);
        interface->CreateArrowSet(6, 305);
        interface->CreateArrowSet(7, 345);

        audio->PlayMusic(audio->PlayingMusicArray[logic->SelectedMusicTrack], -1);

        ScreenTransitionStatus = FadeIn;
    }

    if (interface->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interface->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = StoryMode;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < StoryMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 1 && audio->MusicJukeboxMode == 0 && logic->GameMode != StoryMode && audio->MusicVolume != 0)
        {
            if (logic->SelectedMusicTrack > 0)  logic->SelectedMusicTrack-=1;
            else  logic->SelectedMusicTrack = (audio->PlayingMusicArrayMax-1);

            audio->PlayMusic(audio->PlayingMusicArray[logic->SelectedMusicTrack], -1);
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 1.5 && audio->MusicJukeboxMode == 0 && logic->GameMode != StoryMode && audio->MusicVolume != 0)
        {
            if ( logic->SelectedMusicTrack < (audio->PlayingMusicArrayMax-1) )  logic->SelectedMusicTrack+=1;
            else  logic->SelectedMusicTrack = 0;

            audio->PlayMusic(audio->PlayingMusicArray[logic->SelectedMusicTrack], -1);
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 2 && logic->GameMode != StoryMode && audio->MusicVolume != 0)
        {
            if (audio->MusicJukeboxMode > 0)  audio->MusicJukeboxMode-=1;
            else  audio->MusicJukeboxMode = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 2.5 && logic->GameMode != StoryMode && audio->MusicVolume != 0)
        {
            if (audio->MusicJukeboxMode < 1)  audio->MusicJukeboxMode+=1;
            else  audio->MusicJukeboxMode = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 3)
        {
            if (logic->GameMode != StoryMode)
            {
                if (logic->SelectedBackground > 0)  logic->SelectedBackground-=1;
                else  logic->SelectedBackground = 6;
            }
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 3.5)
        {
            if (logic->GameMode != StoryMode)
            {
                if (logic->SelectedBackground < 6)  logic->SelectedBackground+=1;
                else  logic->SelectedBackground = 0;
            }
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 4 && logic->GameMode != StoryMode)
        {
            if (logic->NewGameGarbageHeight > 0)  logic->NewGameGarbageHeight-=1;
            else  logic->NewGameGarbageHeight = 14;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 4.5 && logic->GameMode != StoryMode)
        {
            if (logic->NewGameGarbageHeight < 14)  logic->NewGameGarbageHeight+=1;
            else  logic->NewGameGarbageHeight = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 5)
        {
            if (logic->DisplayNextPiece > 0)  logic->DisplayNextPiece-=1;
            else  logic->DisplayNextPiece = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 5.5)
        {
            if (logic->DisplayNextPiece < 1)  logic->DisplayNextPiece+=1;
            else  logic->DisplayNextPiece = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 6)
        {
            if (logic->DisplayDropShadow > 0)  logic->DisplayDropShadow-=1;
            else  logic->DisplayDropShadow = 1;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 6.5)
        {
            if (logic->DisplayDropShadow < 1)  logic->DisplayDropShadow+=1;
            else  logic->DisplayDropShadow = 0;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 7 && logic->GameMode != StoryMode)
        {
            if (logic->TileSet > 0)  logic->TileSet-=1;
            else  logic->TileSet = 5;
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 7.5 && logic->GameMode != StoryMode)
        {
            if (logic->TileSet < 5)  logic->TileSet+=1;
            else  logic->TileSet = 0;
        }

        interface->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        if (logic->GameMode != StoryMode)
        {
        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);
        }
        else
        {
            visuals->Sprites[108].ScreenX = 320;
            visuals->Sprites[108].ScreenY = 240;
            visuals->DrawSpriteOntoScreenBuffer(108);
        }

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 120;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("N E W   G A M E   O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        visuals->DrawTextOntoScreenBuffer("Game Mode:", visuals->Font[0]
                                          , 60, 65-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("1 Player Story+Mode", visuals->Font[0]
                                              , 60, 65-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Music Track:", visuals->Font[0]
                                          , 60, 105-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (audio->MusicVolume == 0)
            visuals->DrawTextOntoScreenBuffer("Volume OFF", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story BGM's", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 1)
            visuals->DrawTextOntoScreenBuffer("Random", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 0)
            visuals->DrawTextOntoScreenBuffer("Track 00", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 1)
            visuals->DrawTextOntoScreenBuffer("Track 01", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 2)
            visuals->DrawTextOntoScreenBuffer("Track 02", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 3)
            visuals->DrawTextOntoScreenBuffer("Track 03", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 4)
            visuals->DrawTextOntoScreenBuffer("Track 04", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 5)
            visuals->DrawTextOntoScreenBuffer("Track 05", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 6)
            visuals->DrawTextOntoScreenBuffer("Track 06", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 7)
            visuals->DrawTextOntoScreenBuffer("Track 07", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 8)
            visuals->DrawTextOntoScreenBuffer("Track 08", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 9)
            visuals->DrawTextOntoScreenBuffer("Track 09", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 10)
            visuals->DrawTextOntoScreenBuffer("Track 10", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 11)
            visuals->DrawTextOntoScreenBuffer("Track 11", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 12)
            visuals->DrawTextOntoScreenBuffer("Track 12", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 13)
            visuals->DrawTextOntoScreenBuffer("Track 13", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 14)
            visuals->DrawTextOntoScreenBuffer("Track 14", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 15)
            visuals->DrawTextOntoScreenBuffer("Track 15", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 16)
            visuals->DrawTextOntoScreenBuffer("Track 16", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 17)
            visuals->DrawTextOntoScreenBuffer("Track 17", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedMusicTrack == 18)
            visuals->DrawTextOntoScreenBuffer("Track 18", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+1) )
            visuals->DrawTextOntoScreenBuffer("Bonus Track 19", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+2) )
            visuals->DrawTextOntoScreenBuffer("Bonus Track 20", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+3) )
            visuals->DrawTextOntoScreenBuffer("Bonus Track 21", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+4) )
            visuals->DrawTextOntoScreenBuffer("Bonus Track 22", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+5) )
            visuals->DrawTextOntoScreenBuffer("Bonus Track 23", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+6) )
            visuals->DrawTextOntoScreenBuffer("nullraum Track 24", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+7) )
            visuals->DrawTextOntoScreenBuffer("nullraum Track 25", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+8) )
            visuals->DrawTextOntoScreenBuffer("nullraum Track 26", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if ( logic->SelectedMusicTrack == (18+9) )
            visuals->DrawTextOntoScreenBuffer("Suno.com A.I. Track 27", visuals->Font[0]
                                              , 60, 105-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Music Mode:", visuals->Font[0]
                                          , 60, 145-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (audio->MusicVolume == 0)
            visuals->DrawTextOntoScreenBuffer("Volume OFF", visuals->Font[0]
                                              , 60, 145-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story", visuals->Font[0]
                                              , 60, 145-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 0)
            visuals->DrawTextOntoScreenBuffer("Repeat", visuals->Font[0]
                                              , 60, 145-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicJukeboxMode == 1)
            visuals->DrawTextOntoScreenBuffer("Random", visuals->Font[0]
                                              , 60, 145-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Background:", visuals->Font[0]
                                          , 60, 185-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story BG's", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 0)
            visuals->DrawTextOntoScreenBuffer("Cathedral", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 1)
            visuals->DrawTextOntoScreenBuffer("Firefox", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 2)
            visuals->DrawTextOntoScreenBuffer("GT-R", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 3)
            visuals->DrawTextOntoScreenBuffer("New York", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 4)
            visuals->DrawTextOntoScreenBuffer("Painting", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 5)
            visuals->DrawTextOntoScreenBuffer("Kittens", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->SelectedBackground == 6)
            visuals->DrawTextOntoScreenBuffer("Psychedelic", visuals->Font[0]
                                              , 60, 185-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Garbage Height:", visuals->Font[0]
                                          , 60, 225-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        char temp[256];
		SDL_snprintf(temp, sizeof temp, "%d", logic->NewGameGarbageHeight);
        SDL_strlcpy(visuals->VariableText, temp, sizeof visuals->VariableText);

        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0]
                                          , 60, 225-15+3-3, JustifyRight
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Display Next Piece:", visuals->Font[0]
                                          , 60, 265-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DisplayNextPiece == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 60, 265-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DisplayNextPiece == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 60, 265-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Display Drop Shadow:", visuals->Font[0]
                                          , 60, 305-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DisplayDropShadow == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0]
                                              , 60, 305-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DisplayDropShadow == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0]
                                              , 60, 305-15+3-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Piece Box Tileset:", visuals->Font[0]
                                          , 60, 345-15+3-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Enter] Or Click [START!] To Begin!"
                                          , visuals->Font[0], 0, 380, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        float x = 390.0f;
        for (int boxIndex = 0; boxIndex < 7; boxIndex++)
        {
            int boxSetToShow = (logic->TileSet*10);
            if (logic->GameMode == StoryMode)  boxSetToShow = 0;

            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScreenX = x;
            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScreenY = 346.0f;
            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScaleX = 1.8f;
            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScaleY = 1.8f;
            visuals->DrawSpriteOntoScreenBuffer( 202+boxIndex+(boxSetToShow) );
            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScaleX = 1.0f;
            visuals->Sprites[202+boxIndex+(boxSetToShow)].ScaleY = 1.0f;

            x+=30;
        }
    }

    if (input->MouseButtonWasClicked[2] == true)
    {
        audio->PlayMusic(0, -1);
        ScreenToDisplay = TitleScreen;
        ScreenTransitionStatus = FadeOut;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenToDisplay = TitleScreen;
        ScreenTransitionStatus = FadeAll;

        if (interface->ButtonSelectedByPlayer == 0)
        {
            logic->SetupForNewGame();

            if (logic->GameMode < StoryMode)
                ScreenToDisplay = PlayingGameScreen;
            else
                ScreenToDisplay = ShowStoryScreen;

            if (logic->GameMode != StoryMode)
            {
                if (audio->MusicJukeboxMode == 0)
                    audio->PlayMusic(audio->PlayingMusicArray[logic->SelectedMusicTrack], -1);
                else
                {
                    audio->PlayMusic(audio->PlayingMusicArray[( rand()%audio->PlayingMusicArrayMax )], 0);
                }

                if (logic->SelectedBackground == 1)  Mix_PauseMusic();
            }
        }
        else
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}
//-------------------------------------------------------------------------------------------------
void Screens::DisplayOptionsScreen(void)
{
    joyAction = -1;

    if (ScreenTransitionStatus == FadeAll)
    {
        JoystickFlash = 0;

        interface->CreateButton(1009, 0, 454);

        interface->CreateArrowSet(0, 60);
        interface->CreateArrowSet(1, 105-4);

        interface->CreateArrowSet(2, 160-7);

        interface->CreateArrowSet(3, 215-10);

        interface->CreateArrowSet(4, 215+45-10);

        interface->CreateArrowSet(5, 215+45-10+45);

        interface->CreateArrowSet(6, 325+16);

        if (input->SteamDeckSetupGamePad == true)
        {
            if (input->JoystickDisabled[0] == false || input->JoystickDisabled[1] == false || input->JoystickDisabled[2] == false || input->JoystickDisabled[3] == false)
            {
                input->JoystickSetupProcess = Joy1SetupPressUP;

                input->joystickToCheck = 0;
                input->joystickControl = 0;

                joySetup = input->JoystickSetupProcess;
            }
        }

        ScreenTransitionStatus = FadeIn;
    }

    if (input->SteamDeckSetupGamePad == true)
    {
        if (input->JoystickSetupProcess == JoySetupNotStarted)
        {
            input->SteamDeckSetupGamePad = false;
            ScreenTransitionStatus = FadeOut;
        }
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_d)
    {
        if (input->DEBUG == 1)  input->DEBUG = 2;

        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->KeyOnKeyboardPressedByUser == SDLK_F2 && input->JoystickSetupProcess == JoySetupNotStarted)
    {
        ScreenIsDirty = 2;
        audio->PlayDigitalSoundFX(1, 0);

        if (input->KeyboardSetupProcess == KeyboardSetupNotStarted)
        {
            input->KeyboardSetupProcess = KeyboardSetupPressOne;
        }
        else
        {
            input->KeyboardSetupProcess = KeyboardSetupNotStarted;

            input->UserDefinedKeyButtonOne = -1;
            input->UserDefinedKeyButtonTwo = -1;
            input->UserDefinedKeyUP = -1;
            input->UserDefinedKeyRIGHT = -1;
            input->UserDefinedKeyDOWN = -1;
            input->UserDefinedKeyLEFT = -1;
            input->UserDefinedKeyPause = -1;
        }
    }

    if (input->KeyboardSetupProcess == KeyboardSetupPressOne && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
        && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonOne = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressTwo;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressTwo && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyButtonTwo = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressUP;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressUP && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyUP = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressRIGHT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressRIGHT && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyRIGHT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressDOWN;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressDOWN && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyDOWN = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressLEFT;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressLEFT && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyLEFT = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupPressPause;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }
    else if (input->KeyboardSetupProcess == KeyboardSetupPressPause && input->KeyOnKeyboardPressedByUser != SDLK_F2 && input->KeyOnKeyboardPressedByUser != SDLK_RETURN
             && input->KeyOnKeyboardPressedByUser != SDLK_SPACE && input->KeyOnKeyboardPressedByUser != -1)
    {
        input->UserDefinedKeyPause = input->KeyOnKeyboardPressedByUser;
        input->KeyboardSetupProcess = KeyboardSetupNotStarted;

        audio->PlayDigitalSoundFX(1, 0);
        input->DelayAllUserInput = 20;
        ScreenIsDirty = 2;
    }

    if ( (input->KeyOnKeyboardPressedByUser == SDLK_F1)
        && input->KeyboardSetupProcess == KeyboardSetupNotStarted && input->DelayAllUserInput == 0
        && (input->JoystickDisabled[0] == false || input->JoystickDisabled[1] == false || input->JoystickDisabled[2] == false || input->JoystickDisabled[3] == false) )
    {
        ScreenIsDirty = 2;
        audio->PlayDigitalSoundFX(1, 0);

        input->joystickToCheck = 0;
        input->joystickControl = 0;

        input->DelayAllUserInput = 25;

        if (input->JoystickSetupProcess == JoySetupNotStarted)
        {
            if (input->GamepadController[0] != NULL && input->JoystickDisabled[0] == false)
            {
                input->JoystickSetupProcess = Joy1SetupPressUP;
                input->joystickToCheck = 0;
            }
            else if (input->GamepadController[1] != NULL && input->JoystickDisabled[1] == false)
            {
                input->JoystickSetupProcess = Joy2SetupPressUP;
                input->joystickToCheck = 1;
            }
            else if (input->GamepadController[2] != NULL && input->JoystickDisabled[2] == false)
            {
                input->JoystickSetupProcess = Joy3SetupPressUP;
                input->joystickToCheck = 2;
            }
            else if (input->GamepadController[3] != NULL && input->JoystickDisabled[3] == false)
            {
                input->JoystickSetupProcess = Joy4SetupPressUP;
                input->joystickToCheck = 3;
            }
            else
            {
                input->JoystickSetupProcess = JoySetupNotStarted;
            }

            joySetup = input->JoystickSetupProcess;
        }
        else
        {
            input->JoystickSetupProcess = JoySetupNotStarted;

            for (int joy = 0; joy < 4; joy++)
            {
                input->JoyUP[joy] = Axis1;
                input->JoyDOWN[joy] = Axis1;
                input->JoyLEFT[joy] = Axis0;
                input->JoyRIGHT[joy] = Axis0;
                input->JoyButton1[joy] = Button0;
                input->JoyButton2[joy] = Button1;
                input->JoyPause[joy] = Button2;

                input->GamepadUP[joy] = SDL_CONTROLLER_BUTTON_DPAD_UP;
                input->GamepadDOWN[joy] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
                input->GamepadLEFT[joy] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
                input->GamepadRIGHT[joy] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
                input->GamepadButton1[joy] = SDL_CONTROLLER_BUTTON_X;
                input->GamepadButton2[joy] = SDL_CONTROLLER_BUTTON_Y;
                input->GamepadPause[joy] = SDL_CONTROLLER_BUTTON_B;
            }
        }
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_F1 && input->JoystickSetupProcess > JoySetupNotStarted && input->DelayAllUserInput == 0)
    {
        input->JoystickSetupProcess = JoySetupNotStarted;

        input->DelayAllUserInput = 25;

        ScreenIsDirty = 2;

        for (int joy = 0; joy < 4; joy++)
        {
            input->JoyUP[joy] = Axis1;
            input->JoyDOWN[joy] = Axis1;
            input->JoyLEFT[joy] = Axis0;
            input->JoyRIGHT[joy] = Axis0;
            input->JoyButton1[joy] = Button0;
            input->JoyButton2[joy] = Button1;
            input->JoyPause[joy] = Button2;

            input->GamepadUP[joy] = SDL_CONTROLLER_BUTTON_DPAD_UP;
            input->GamepadDOWN[joy] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
            input->GamepadLEFT[joy] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
            input->GamepadRIGHT[joy] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
            input->GamepadButton1[joy] = SDL_CONTROLLER_BUTTON_X;
            input->GamepadButton2[joy] = SDL_CONTROLLER_BUTTON_Y;
            input->GamepadPause[joy] = SDL_CONTROLLER_BUTTON_B;
        }
    }

    if (joySetup == input->JoystickSetupProcess)
    {
        joyAction = -1;

        if (input->joystickControl < 4)
        {
            input->GetOnlyGamepadButtons = false;
        }
        else if (input->joystickControl > 3)
        {
            input->GetOnlyGamepadButtons = true;
        }

        joyAction = input->QueryJoysticksForAction(input->joystickToCheck);
        if (joyAction > JoyNotPressed && input->DelayAllUserInput == 0)
        {
            if (input->UseNewGamepadAPI == true)
            {
                if (input->joystickControl == 0)  input->GamepadUP[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 1)  input->GamepadDOWN[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 2)  input->GamepadLEFT[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 3)  input->GamepadRIGHT[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 4)
                {
                    input->GamepadButton1[input->joystickToCheck] = joyAction;
                }
                else if (input->joystickControl == 5)
                {
                    input->GamepadButton2[input->joystickToCheck] = joyAction;
                }
                else if (input->joystickControl == 6)
                {
                    input->GamepadPause[input->joystickToCheck] = joyAction;
                }
            }
            else if (input->UseNewGamepadAPI == false)
            {
                if (input->joystickControl == 0)  input->JoyUP[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 1)  input->JoyDOWN[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 2)  input->JoyLEFT[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 3)  input->JoyRIGHT[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 4)  input->JoyButton1[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 5)  input->JoyButton2[input->joystickToCheck] = joyAction;
                else if (input->joystickControl == 6)  input->JoyPause[input->joystickToCheck] = joyAction;
            }

            joySetup++;

            if (input->joystickControl < 6)
            {
                input->JoystickSetupProcess++;

                input->joystickControl++;
            }
            else
            {
                input->joystickToCheck++;

                if (input->joystickToCheck == 1)
                {
                    if (input->JoystickDisabled[input->joystickToCheck] == false)
                    {
                        input->JoystickSetupProcess = Joy2SetupPressUP;
                        input->joystickControl = 0;
                    }
                    else
                    {
                        input->joystickToCheck++;
                    }
                }

                if (input->joystickToCheck == 2)
                {
                    if (input->JoystickDisabled[input->joystickToCheck] == false)
                    {
                        input->JoystickSetupProcess = Joy3SetupPressUP;
                        input->joystickControl = 0;
                    }
                    else
                    {
                        input->joystickToCheck++;
                    }
                }

                if (input->joystickToCheck == 3)
                {
                    if (input->JoystickDisabled[input->joystickToCheck] == false)
                    {
                        input->JoystickSetupProcess = Joy4SetupPressUP;
                        input->joystickControl = 0;
                    }
                    else
                    {
                        input->joystickToCheck++;
                    }
                }

                if (input->joystickToCheck > 3)
                {
                    input->JoystickSetupProcess = JoySetupNotStarted;
                    joySetup = 999;
                }
            }

            audio->PlayDigitalSoundFX(0, 0);
            input->DelayAllUserInput = 25;
            ScreenIsDirty = 2;
        }
    }

    if (input->JoystickSetupProcess == JoySetupNotStarted
     && input->KeyboardSetupProcess == KeyboardSetupNotStarted)
    {
        if (interface->ArrowSetArrowSelectedByPlayer != -1)
        {
            if (interface->ArrowSetArrowSelectedByPlayer == 0)
            {
                if (audio->MusicVolume > 0)  audio->MusicVolume-=32;
                else  audio->MusicVolume = 128;

                Mix_VolumeMusic(audio->MusicVolume);

                if (audio->MusicVolume > 0 && Mix_PlayingMusic() == 0)
                {
                    audio->PlayMusic(audio->CurrentMusicTrackPlaying, -1);
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
            {
                if (audio->MusicVolume < 128)  audio->MusicVolume+=32;
                else  audio->MusicVolume = 0;

                Mix_VolumeMusic(audio->MusicVolume);

                if (audio->MusicVolume > 0 && Mix_PlayingMusic() == 0)
                {
                    audio->PlayMusic(audio->CurrentMusicTrackPlaying, -1);
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 1)
            {
                if (audio->SoundVolume > 0)  audio->SoundVolume-=32;
                else  audio->SoundVolume = 128;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 1.5)
            {
                if (audio->SoundVolume < 128)  audio->SoundVolume+=32;
                else  audio->SoundVolume = 0;

                Mix_HaltChannel(-1);
                audio->PlayDigitalSoundFX(1, 0);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 2)
            {
                if (visuals->FullScreenMode > 0)  visuals->FullScreenMode--;
                else  visuals->FullScreenMode = 1;

                if (visuals->ForceAspectRatio == true)  SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);

                visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

                if (visuals->FullScreenMode == 0)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == 1)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 2.5)
            {
                if (visuals->FullScreenMode < 1)  visuals->FullScreenMode++;
                else  visuals->FullScreenMode = 0;

                if (visuals->ForceAspectRatio == true)  SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);

                visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

                if (visuals->FullScreenMode == 0)  SDL_SetWindowFullscreen(visuals->Window, 0);
                else if (visuals->FullScreenMode == 1)  SDL_SetWindowFullscreen(visuals->Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 3)
            {
                if (logic->CPUPlayerEnabled > 0)  logic->CPUPlayerEnabled--;
                else
                {
                    logic->CPUPlayerEnabled = 4;
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 3.5)
            {
                if (logic->CPUPlayerEnabled < 4)  logic->CPUPlayerEnabled++;
                else
                {
                    logic->CPUPlayerEnabled = 0;
                }
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 4)
            {
                if (logic->DelayAutoShift > 0)  logic->DelayAutoShift-=1;
                else  logic->DelayAutoShift = 2;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 4.5)
            {
                if (logic->DelayAutoShift < 2)  logic->DelayAutoShift+=1;
                else  logic->DelayAutoShift = 0;
            }

            if (interface->ArrowSetArrowSelectedByPlayer == 5)
            {
                if (logic->PressingUPAction > 0)  logic->PressingUPAction-=1;
                else  logic->PressingUPAction = 3;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 5.5)
            {
                if (logic->PressingUPAction < 3)  logic->PressingUPAction+=1;
                else  logic->PressingUPAction = 0;
            }

            else if (interface->ArrowSetArrowSelectedByPlayer == 6)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 33;
            }
            else if (interface->ArrowSetArrowSelectedByPlayer == 6.5)
            {
                if (logic->PlayingGameFrameLock == 33)  logic->PlayingGameFrameLock = 25;
                else if (logic->PlayingGameFrameLock == 25)  logic->PlayingGameFrameLock = 10;
                else if (logic->PlayingGameFrameLock == 10)  logic->PlayingGameFrameLock = 33;
            }

            interface->ArrowSetArrowSelectedByPlayer = -1;
        }
    }

    if ( JoystickFlash < (190+160+30) )
        JoystickFlash+=1;
    else
        JoystickFlash = 0;

    if (JoystickFlash == 0)
        ScreenIsDirty = 2;
    else if (JoystickFlash == 191)
        ScreenIsDirty = 2;

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("O P T I O N S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        visuals->DrawTextOntoScreenBuffer("Music Volume:"
                                          , visuals->Font[0], 60, 60-15, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (audio->MusicVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->MusicVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 60-15, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Sound Effects Volume:"
                                          , visuals->Font[0], 60, 105-15-4, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (audio->SoundVolume == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 105-15-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 32)
            visuals->DrawTextOntoScreenBuffer("25%", visuals->Font[0], 60, 105-15-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 64)
            visuals->DrawTextOntoScreenBuffer("50%", visuals->Font[0], 60, 105-15-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 96)
            visuals->DrawTextOntoScreenBuffer("75%", visuals->Font[0], 60, 105-15-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (audio->SoundVolume == 128)
            visuals->DrawTextOntoScreenBuffer("100%", visuals->Font[0], 60, 105-15-4, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 107-6-4, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Full Screen Mode:"
                                          , visuals->Font[0], 60, 160-15+3-7-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (visuals->FullScreenMode == 0)
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 160-15+3-7-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (visuals->FullScreenMode == 1)
            visuals->DrawTextOntoScreenBuffer("ON", visuals->Font[0], 60, 160-15+3-7-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 162-6-7, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("C.P.U. Players:"
                                          , visuals->Font[0], 60, 215-15+3-10-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);

        if (logic->CPUPlayerEnabled == 1)
            visuals->DrawTextOntoScreenBuffer("Slow Speed", visuals->Font[0], 60, 215-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->CPUPlayerEnabled == 2)
            visuals->DrawTextOntoScreenBuffer("Medium Speed", visuals->Font[0], 60, 215-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->CPUPlayerEnabled == 3)
            visuals->DrawTextOntoScreenBuffer("Fast Speed", visuals->Font[0], 60, 215-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->CPUPlayerEnabled == 4)
            visuals->DrawTextOntoScreenBuffer("Turbo! Speed", visuals->Font[0], 60, 215-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("OFF", visuals->Font[0], 60, 215-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Delayed Auto Shift:"
                                          , visuals->Font[0], 60, 215+45-15+3-10-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->DelayAutoShift == 0)
            visuals->DrawTextOntoScreenBuffer("Original", visuals->Font[0], 60, 215+45-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DelayAutoShift == 1)
            visuals->DrawTextOntoScreenBuffer("Slow Shift", visuals->Font[0], 60, 215+45-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->DelayAutoShift == 2)
            visuals->DrawTextOntoScreenBuffer("Fast Shift", visuals->Font[0], 60, 215+45-15+3-10-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("[UP] Action:", visuals->Font[0]
                                          , 60, 215+45-10+45-13-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->PressingUPAction == 0)
            visuals->DrawTextOntoScreenBuffer("None", visuals->Font[0]
                                              , 60, 215+45-10+45-13-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 1)
            visuals->DrawTextOntoScreenBuffer("Quick Drop", visuals->Font[0]
                                              , 60, 215+45-10+45-13-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 2)
            visuals->DrawTextOntoScreenBuffer("Smart Rotate", visuals->Font[0]
                                              , 60, 215+45-10+45-13-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PressingUPAction == 3)
            visuals->DrawTextOntoScreenBuffer("Drop & Drag", visuals->Font[0]
                                              , 60, 215+45-10+45-13-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 256-10+42, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Playing Game Speed:", visuals->Font[0]
                                          , 60, 325-15+3+16-3, JustifyLeft
                                          , 255, 255, 255, 1, 1, 1);
        if (logic->PlayingGameFrameLock == 33)
            visuals->DrawTextOntoScreenBuffer("Normal", visuals->Font[0], 60, 325-15+3+16-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PlayingGameFrameLock == 25)
            visuals->DrawTextOntoScreenBuffer("Fast", visuals->Font[0], 60, 325-15+3+16-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->PlayingGameFrameLock == 10)
            visuals->DrawTextOntoScreenBuffer("Turbo!", visuals->Font[0], 60, 325-15+3+16-3, JustifyRight
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("_____________________________________"
                                          , visuals->Font[3], 0, 344-6, JustifyCenter
                                          , 255, 255, 255, 1, 1, 1);

        if (input->JoystickDisabled[0] == false || input->JoystickDisabled[1] == false || input->JoystickDisabled[2] == false || input->JoystickDisabled[3] == false)
        {
            visuals->DrawTextOntoScreenBuffer("Press [F1] On Keyboard To Setup Joystick(s)"
                                              , visuals->Font[1]
                                              , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
        }
        else
        {
            if (JoystickFlash < 160)
            {
                visuals->DrawTextOntoScreenBuffer("* Game Supports USB Joysticks - Go Grab Some & Party! *"
                                                  , visuals->Font[1]
                                                  , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
            }
            else if ( JoystickFlash > 190 && JoystickFlash < (160+190) )
            {
                visuals->DrawTextOntoScreenBuffer("* Requires Restart Of Game After Plugged In *"
                                                  , visuals->Font[1]
                                                  , 0, 374, JustifyCenter, 255, 255, 255, 1, 1, 1);
            }
        }

        visuals->DrawTextOntoScreenBuffer("Press [F2] On Keyboard To Setup Keyboard", visuals->Font[1]
                                          , 0, 399, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        if (input->KeyboardSetupProcess > KeyboardSetupNotStarted)
        {
            visuals->ClearScreenBufferWithColor(0, 0, 0, 1);

            if (input->KeyboardSetupProcess == KeyboardSetupPressOne)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [ROTATE1] now!"
                                                  ,visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressTwo)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [ROTATE2] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [UP] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [RIGHT] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [DOWN] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [LEFT] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            else if (input->KeyboardSetupProcess == KeyboardSetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press key on keyboard for [Pause] now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("Press [F2] key on keyboard to quit setup."
                                              , visuals->Font[0]
                                              , 0, 380, JustifyCenter, 200, 200, 200, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("(Will setup keyboard with default mappings.)"
                                              , visuals->Font[1]
                                              , 0, 410, JustifyCenter, 200, 200, 200, 1, 1, 1);
        }

        if (input->JoystickSetupProcess > JoySetupNotStarted)
        {
            visuals->ClearScreenBufferWithColor(0, 0, 0, 1);

            if (input->JoystickSetupProcess == Joy1SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy1SetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press [Pause] on joystick #1 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy2SetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press [Pause] on joystick #2 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy3SetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press [Pause] on joystick #3 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressUP)
                visuals->DrawTextOntoScreenBuffer("Press [UP] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressDOWN)
                visuals->DrawTextOntoScreenBuffer("Press [DOWN] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressLEFT)
                visuals->DrawTextOntoScreenBuffer("Press [LEFT] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressRIGHT)
                visuals->DrawTextOntoScreenBuffer("Press [RIGHT] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressBUTTONOne)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON1] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressBUTTONTwo)
                visuals->DrawTextOntoScreenBuffer("Press [BUTTON2] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);
            else if (input->JoystickSetupProcess == Joy4SetupPressPause)
                visuals->DrawTextOntoScreenBuffer("Press [Pause] on joystick #4 now!"
                                                  , visuals->Font[0]
                                                  , 0, 214, JustifyCenter, 0, 255, 0, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("Press [F1] key on keyboard to quit setup.", visuals->Font[0]
                                              , 0, 380, JustifyCenter, 200, 200, 200, 1, 1, 1);

            visuals->DrawTextOntoScreenBuffer("(Will setup joystick(s) with default mappings.)", visuals->Font[1]
                                              , 0, 410, JustifyCenter, 200, 200, 200, 1, 1, 1);
        }
    }

    if (input->MouseButtonWasClicked[2] == true)
    {
//        audio->PlayMusic(0, -1);
        ScreenToDisplay = TitleScreen;
        ScreenTransitionStatus = FadeOut;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        input->KeyboardSetupProcess = KeyboardSetupNotStarted;
        input->JoystickSetupProcess = JoySetupNotStarted;

        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayHowToPlayScreen(void)
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1009, 0, 454);

        ScreenTransitionStatus = FadeIn;
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->Sprites[4].ScreenX = 320;

        if (input->UserDefinedKeyButtonOne == -1 && input->UserDefinedKeyButtonTwo == -1)
            visuals->Sprites[4].ScreenY = 240;
        else
            visuals->Sprites[4].ScreenY = 160-1000;

        visuals->DrawSpriteOntoScreenBuffer(4);

        visuals->DrawTextOntoScreenBuffer("H O W   T O   P L A Y:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        if (input->UserDefinedKeyButtonOne > -1 && input->UserDefinedKeyButtonTwo > -1)
        {
            visuals->DrawTextOntoScreenBuffer("CUSTOM CONTROL", visuals->Font[4]
                                              , 0, 50, JustifyCenter, 255, 255, 255, 90, 90, 1);

            keyName = SDL_GetKeyName(input->UserDefinedKeyUP);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Special UP Action", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70, JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyLEFT);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Move Piece Left", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(1*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyRIGHT);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Move Piece Right", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(2*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyDOWN);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Move Piece Down", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(3*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyButtonOne);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Rotate Clockwise", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(4*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyButtonTwo);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Rotate Counter Clockwise", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(5*40), JustifyCenter, 255, 255, 255, 0, 0, 0);

            keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
            SDL_strlcpy(visuals->VariableText, "[ ", sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
            SDL_strlcat(visuals->VariableText, " ] = Pause", sizeof visuals->VariableText);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[3]
                                              , 0, 50+70+(6*40), JustifyCenter, 255, 255, 255, 0, 0, 0);
        }
        else
        {
            visuals->DrawTextOntoScreenBuffer("You can change the keyboard controls in [Options]", visuals->Font[1]
                                             , 0, 360+30, JustifyCenter, 255, 255, 255, 0, 0, 0);
        }
    }

    if (input->MouseButtonWasClicked[2] == true)
    {
//        audio->PlayMusic(0, -1);
        ScreenToDisplay = TitleScreen;
        ScreenTransitionStatus = FadeOut;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayHighScoresScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1009, 0, 454);

        interface->CreateArrowSet(0, 65);

        ScreenTransitionStatus = FadeIn;
    }

    if (interface->ArrowSetArrowSelectedByPlayer != -1)
    {
        if (interface->ArrowSetArrowSelectedByPlayer == 0)
        {
            if (logic->GameMode > 0)  logic->GameMode-=1;
            else  logic->GameMode = StoryMode;

            visuals->ClearTextCache();
        }
        else if (interface->ArrowSetArrowSelectedByPlayer == 0.5)
        {
            if (logic->GameMode < StoryMode)  logic->GameMode+=1;
            else  logic->GameMode = 0;

            visuals->ClearTextCache();
        }

        interface->ArrowSetArrowSelectedByPlayer = -1;
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_BACKSPACE)
    {
        data->ClearHighScores();
        ScreenIsDirty = 2;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E S:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        interface->DisplayAllArrowSetsOntoScreenBuffer();

        if (logic->GameMode == OriginalMode)
            visuals->DrawTextOntoScreenBuffer("Original Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack30Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 30 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack60Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 60 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TimeAttack120Mode)
            visuals->DrawTextOntoScreenBuffer("Time Attack 120 Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == TwentyLineChallengeMode)
            visuals->DrawTextOntoScreenBuffer("20 Line Challenge Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == CrisisMode)
            visuals->DrawTextOntoScreenBuffer("Crisis+Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);
        else if (logic->GameMode == StoryMode)
            visuals->DrawTextOntoScreenBuffer("Story+Mode", visuals->Font[0]
                                              , 0, 65-15, JustifyCenter
                                              , 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("NAME:", visuals->Font[1],
                                          30, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("LEVEL:", visuals->Font[1],
                                          420-30-10, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("SCORE:", visuals->Font[1],
                                          510-50, 88, JustifyLeft, 150, 150, 150, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("1.", visuals->Font[1], 1, 112+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("2.", visuals->Font[1], 1, 142+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("3.", visuals->Font[1], 1, 172+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("4.", visuals->Font[1], 1, 202+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("5.", visuals->Font[1], 1, 232+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("6.", visuals->Font[1], 1, 263+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("7.", visuals->Font[1], 1, 292+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("8.", visuals->Font[1], 1, 322+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("9.", visuals->Font[1], 1, 352+4, JustifyLeft, 150, 150, 150, 1, 1, 1);
        visuals->DrawTextOntoScreenBuffer("10.", visuals->Font[1], 1, 382+4, JustifyLeft, 150, 150, 150, 1, 1, 1);

        float rankY = 112.0f;
        float offsetY = 30.0f;

        for (Uint8 index = 0; index < 10; index++)
        {
            Uint8 greenBlueColorValue = 255;

            for (int playerIndex = 0; playerIndex < NumberOfPlayers; playerIndex++)
            {
                if (data->PlayerWithHighestScore == playerIndex
                    && data->HighScoresScore[logic->GameMode][index] == logic->PlayerData[playerIndex].Score
                    && data->HighScoresLevel[logic->GameMode][index] == logic->PlayerData[playerIndex].Level)
                {
                    greenBlueColorValue = 0;
                }
            }

            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][index], visuals->Font[0]
                                              , 30, rankY, JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);

            if ( (logic->GameMode == CrisisMode || logic->GameMode == StoryMode) && data->HighScoresLevel[logic->GameMode][index] == 10 )
            {
                visuals->DrawTextOntoScreenBuffer("Won!", visuals->Font[0], 420-30-10, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }
            else
            {
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", data->HighScoresLevel[logic->GameMode][index]);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 420-30-10, rankY
                                                  , JustifyLeft, 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);
            }

            #ifdef _WIN32
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%I64u", data->HighScoresScore[logic->GameMode][index]);
            #else
                sprintf(visuals->VariableText, "%lu", data->HighScoresScore[logic->GameMode][index]);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], 510-50, rankY, JustifyLeft
                                              , 255, greenBlueColorValue, greenBlueColorValue, 1, 1, 1);

            rankY += offsetY;
        }

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (input->MouseButtonWasClicked[2] == true)
    {
//        audio->PlayMusic(0, -1);
        ScreenToDisplay = TitleScreen;
        ScreenTransitionStatus = FadeOut;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayAboutScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ReviewScale = 1.0f;
        ReviewShowDelay = 500;

        for (int index = 1100; index < (1100+visuals->TotalNumberOfLoadedStaffTexts); index++)
        {
            if (index == 1100)
            {
                visuals->Sprites[1100].ScreenX = 446+40;
                visuals->Sprites[1100].ScreenY = 491;
            }
            else if (index == 1101)
            {
                visuals->Sprites[1101].ScreenX = 320;
                visuals->Sprites[1101].ScreenY = 510;
            }
            else if (index == 1102)
            {
                visuals->Sprites[1102].ScreenX = 320;
                visuals->Sprites[1102].ScreenY = 540;
            }
            else if ( index < (1100 + visuals->TotalNumberOfLoadedStaffTexts-1) )
            {
                if (visuals->Sprites[index-1].BlueHue == 255
                 && visuals->Sprites[index].BlueHue == 0)
                {
                    visuals->Sprites[index].ScreenX = 320;
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+110;
                }
                else
                {
                    visuals->Sprites[index].ScreenX = 320;
                    visuals->Sprites[index].ScreenY = visuals->Sprites[index-1].ScreenY+35;
                }
            }
            else if (index == 1100 + visuals->TotalNumberOfLoadedStaffTexts-1)
            {
                visuals->Sprites[index].ScreenX = 320;
                visuals->Sprites[index].ScreenY = visuals->Sprites[index-2].ScreenY+250+30;
            }
        }

        if ( (logic->GameMode == StoryMode || logic->GameMode == CrisisMode) && logic->Won == true )
        {
            audio->PlayDigitalSoundFX(11, 0);
        }
        else
        {
            visuals->Sprites[13].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50);
            visuals->Sprites[14].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50 + 320 + 50);
            visuals->Sprites[15].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50 + 320 + 50 + 385);

            visuals->Sprites[11].ScreenY = (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY + 240 + 50 + 320 + 50 + 385 + 2000);
        }

        ScreenTransitionStatus = FadeIn;
    }

    if ( input->KeyOnKeyboardPressedByUser == SDLK_SPACE
        || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
        || input->MouseButtonPressed[0] == true
        || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 60;
        audio->PlayDigitalSoundFX(0, 0);
    }

    float skip = 1.5f;

    if (logic->GameMode == CrisisMode && logic->Won == true)  skip = 0.82f;
    else if (logic->GameMode == StoryMode && logic->Won == true)  skip = 0.82f;

    if (input->JoystickDirectionVertical[Any] == UP)  skip = 13;

    for (int index = 1100; index < (1100+visuals->TotalNumberOfLoadedStaffTexts); index++)
    {
        visuals->Sprites[index].ScreenY-=skip;
    }

    visuals->Sprites[11].ScreenY-=skip;

    if ( (logic->GameMode == StoryMode || logic->GameMode == CrisisMode) && logic->Won == true )
    {
        if (visuals->Sprites[1100+visuals->TotalNumberOfLoadedStaffTexts-1].ScreenY < -40)  ScreenTransitionStatus = FadeOut;
    }
    else
    {
        visuals->Sprites[13].ScreenY-=skip;
        visuals->Sprites[14].ScreenY-=skip;

        if (visuals->Sprites[15].ScreenY > 240)
        {
            visuals->Sprites[15].ScreenY-=skip;
        }
        else if (ReviewShowDelay > 0)
        {
            ReviewShowDelay-=1;
        }
        else
        {
            ReviewScale-=0.01f;
        }

        if (ReviewScale < 0.0)
        {
            if (visuals->Sprites[11].ScreenY < -1000)
            {
                ScreenTransitionStatus = FadeOut;
            }
        }
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

        if (logic->GameMode == StoryMode && logic->Won == true)
        {
            visuals->Sprites[75].ScreenX = 320;
            visuals->Sprites[75].ScreenY = 240;
            visuals->Sprites[75].Transparency = 100;
            visuals->DrawSpriteOntoScreenBuffer(75);
        }
        else if (logic->GameMode == CrisisMode && logic->Won == true)
        {
            visuals->Sprites[17].ScreenX = 320;
            visuals->Sprites[17].ScreenY = 240;
            visuals->Sprites[17].Transparency = 100;
            visuals->DrawSpriteOntoScreenBuffer(17);
        }
        else
        {
            visuals->Sprites[2].ScreenX = 320;
            visuals->Sprites[2].ScreenY = 240;
            visuals->DrawSpriteOntoScreenBuffer(2);

            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 150;
            visuals->DrawSpriteOntoScreenBuffer(0);
        }

        if ( (logic->GameMode == StoryMode || logic->GameMode == CrisisMode) && logic->Won == true )
        {

        }
        else
        {
            visuals->Sprites[13].ScreenX = 320;
            visuals->DrawSpriteOntoScreenBuffer(13);

            visuals->Sprites[14].ScreenX = 320;
            visuals->DrawSpriteOntoScreenBuffer(14);

            visuals->Sprites[15].ScreenX = 320;
            visuals->Sprites[15].ScaleX = ReviewScale;
            visuals->Sprites[15].ScaleY = ReviewScale;
            visuals->DrawSpriteOntoScreenBuffer(15);

            visuals->Sprites[11].ScreenX = 320;
            visuals->DrawSpriteOntoScreenBuffer(11);

        }

        for (  int index = 1100; index < ( (1100 + visuals->TotalNumberOfLoadedStaffTexts) ); index++  )
        {
            if (visuals->Sprites[index].ScreenY > -40 && visuals->Sprites[index].ScreenY < 640+40)  visuals->DrawSpriteOntoScreenBuffer(index);
        }

        visuals->Sprites[11].ScreenX = (640/2);

        ScreenIsDirty = 2;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        visuals->Sprites[2].Transparency = 255;

        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = TitleScreen;

        if (logic->Won == true)
        {
            if (data->PlayerRankOnGameOver < 10)
            {
                if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
                    ScreenToDisplay = NameInputJoystickScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                    ScreenToDisplay = NameInputKeyboardScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                    ScreenToDisplay = NameInputMouseScreen;
            }

            logic->Won = false;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayPlayingGameScreen(void)
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        visuals->FrameLock = logic->PlayingGameFrameLock;

        ScreenTransitionStatus = FadeIn;
    }

    logic->RunTetriGameEngine();

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

    if (logic->PlayersCanJoin == true)
    {
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (input->JoystickButtonOne[logic->PlayerData[player].PlayerInput] == ON
                && logic->PlayerData[player].PlayerStatus == GameOver)  logic->PlayerData[player].PlayerStatus = NewPieceDropping;

            if ( (logic->PlayerData[player].PlayerInput == CPU) && (logic->CPUPlayerEnabled == 0) )
            {
                logic->PlayerData[player].PlayerStatus = GameOver;

            }
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == GameOver)
            {
                if (   input->MouseButtonPressed[0] == true
                && (  input->MouseX > ( logic->PlayerData[player].PlayersPlayfieldScreenX-(156/2) )  )
                && (  input->MouseX < ( logic->PlayerData[player].PlayersPlayfieldScreenX+(156/2) )  )
                && (  input->MouseY > ( logic->PlayerData[player].PlayersPlayfieldScreenY-(458/2) )  )
                && (  input->MouseY < ( logic->PlayerData[player].PlayersPlayfieldScreenY+(458/2) )  )   )
                {
                    logic->PlayerData[player].PlayerStatus = NewPieceDropping;
                }
            }
        }
    }

    if (ScreenIsDirty > 0)
    {
        visuals->Sprites[100+logic->SelectedBackground].ScreenX = 320;
        visuals->Sprites[100+logic->SelectedBackground].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(100+logic->SelectedBackground);

        if (logic->PlayerData[0].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 150;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 150;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[2].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 150;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[3].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 150;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        float mouseScreenX = -999;
        float mouseScreenY = -999;
        int mousePlayfieldY = -999;
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            float boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
            float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

            for (int y = 0; y < 26; y++)
            {
                for (int x = 0; x < 12; x++)
                {
                    if (logic->PlayerData[player].Playfield[x][y] == 1)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 10
                             && logic->PlayerData[player].Playfield[x][y] < 20)
                    {
                        int spriteIndex = 200 + (10*logic->TileSet);

                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);

                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 20
                             && logic->PlayerData[player].Playfield[x][y] < 30)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }

                    if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
                    {
                        if (  input->MouseX >= ( boxScreenX-(13/2) ) && input->MouseX <= ( boxScreenX+(13/2) )
                        && input->MouseY >= ( boxScreenY-(18/2) ) && input->MouseY <= ( boxScreenY+(18/2) )  )
                        {
                            mouseScreenX = boxScreenX-13;
                            mouseScreenY = boxScreenY;
                            mousePlayfieldY = y;
                        }
                    }

                    boxScreenX+=13;
                }

                boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
                boxScreenY+=18;
            }

            if (logic->PlayerData[player].PlayerStatus != GameOver)
            {
                if (logic->BlockAttackTransparency[player] > 0)
                {
                    visuals->Sprites[32].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[32].ScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY;
                    visuals->Sprites[32].Transparency = logic->BlockAttackTransparency[player];
                    visuals->DrawSpriteOntoScreenBuffer(32);
                }
            }

            if (logic->GameMode == TimeAttack30Mode || logic->GameMode == TimeAttack60Mode || logic->GameMode == TimeAttack120Mode)
            {
                Uint32 taTimer = logic->TimeAttackTimer / 200;
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", taTimer);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440+5, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
            else if (logic->GameMode == TwentyLineChallengeMode)
            {
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].TwentyLineCounter);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440+5, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
        }

        for (int player = 0; player < 4; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
            {
                int box = 0;
                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        box++;
                        if (logic->PieceData[logic->PlayerData[player].Piece][logic->PlayerData[player].PieceRotation] [box] == 1)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenX = mouseScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenY = mouseScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 128;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet)+logic->PlayerData[player].Piece);

                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 255;
                        }

                        mouseScreenX+=13;
                    }

                    mouseScreenX-=(4*13);
                    mouseScreenY+=18;
                }

                if (mousePlayfieldY >= logic->PlayerData[player].PiecePlayfieldY)
                {
                    visuals->DrawTextOntoScreenBuffer("MOVE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
                }
                else  visuals->DrawTextOntoScreenBuffer("ROTATE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
        }

        if ( logic->GameMode == CrisisMode)
        {
            if (logic->PlayerData[0].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[1].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[2].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }

            if (logic->PlayerData[3].Level > 6)
            {
                visuals->Sprites[155].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
                visuals->Sprites[155].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
                visuals->Sprites[155].Transparency = 155;
                visuals->DrawSpriteOntoScreenBuffer(155);
            }
        }

        if (logic->PlayerData[0].PlayerStatus == GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 200;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[0].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[1].PlayerStatus == GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 200;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[1].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[2].PlayerStatus == GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 200;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[2].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PlayerData[3].PlayerStatus == GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 200;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[3].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PAUSEgame == false)
        {
            for (int player = 0; player < NumberOfPlayers; player++)
            {
                if (logic->PlayersCanJoin == true)
                {
                    if ( (logic->PlayerData[player].PlayerInput == JoystickOne && input->JoystickDisabled[0] == false)
                        || (logic->PlayerData[player].PlayerInput == JoystickTwo && input->JoystickDisabled[1] == false)
                        || (logic->PlayerData[player].PlayerInput == JoystickThree && input->JoystickDisabled[2] == false)
                        || (logic->PlayerData[player].PlayerInput == JoystickFour && input->JoystickDisabled[3] == false)
                        || (logic->PlayerData[player].PlayerInput == Keyboard) || (logic->PlayerData[player].PlayerInput == Mouse) )
                    {
                        logic->JoinInTimer++;
                        if (logic->JoinInTimer > 30)
                            logic->JoinInTimer = 0;

                        if (logic->JoinInTimer < 15)
                            if (logic->PlayerData[player].PlayerStatus == GameOver)
                                visuals->DrawTextOntoScreenBuffer("JOIN IN!", visuals->Font[1], logic->PlayerData[player].PlayersPlayfieldScreenX, 270,
                                                                  JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
                    }
                }
            }
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            #ifdef _WIN32
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
            #else
                sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2+1
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Keyboard)
                visuals->DrawTextOntoScreenBuffer("Keyboard"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickOne)
                visuals->DrawTextOntoScreenBuffer("Joystick #1"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickTwo)
                visuals->DrawTextOntoScreenBuffer("Joystick #2"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickThree)
                visuals->DrawTextOntoScreenBuffer("Joystick #3"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickFour)
                visuals->DrawTextOntoScreenBuffer("Joystick #4"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == CPU)
            {
                visuals->DrawTextOntoScreenBuffer("C.P.U."
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PlayerData[player].PlayerInput == Mouse)
                visuals->DrawTextOntoScreenBuffer("Mouse"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);

        }

        if (logic->PAUSEgame == false)
        {
            logic->HumanStillAlive = false;
            if (logic->PlayerData[0].PlayerInput != CPU && logic->PlayerData[0].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
            if (logic->PlayerData[1].PlayerInput != CPU && logic->PlayerData[1].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
            if (logic->PlayerData[2].PlayerInput != CPU && logic->PlayerData[2].PlayerStatus != GameOver)  logic->HumanStillAlive = true;
            if (logic->PlayerData[3].PlayerInput != CPU && logic->PlayerData[3].PlayerStatus != GameOver)  logic->HumanStillAlive = true;

            if (logic->HumanStillAlive == false && logic->GameOverTimer == 0 && input->DEBUG == 0)
            {
                logic->AllHumansDeadExitTimer++;
                if (logic->AllHumansDeadExitTimer > 150)  ScreenTransitionStatus = FadeOut;

                logic->ContinueWatchingTimer++;
                if (logic->ContinueWatchingTimer > 20)
                    logic->ContinueWatchingTimer = 0;

                if (logic->ContinueWatchingTimer < 15)
                    visuals->DrawTextOntoScreenBuffer("No Human Players Left Playing, Exiting Game!"
                                                    , visuals->Font[1], 320, 260, JustifyCenter, 255, 255, 255, 1, 1, 1);
            }
        }

        if (logic->PAUSEgame == true && input->DEBUG == 0)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 150;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            if (input->UserDefinedKeyPause == -1)
                visuals->DrawTextOntoScreenBuffer("(Press [P] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            else
            {
                SDL_strlcpy(visuals->VariableText, "(Press [", sizeof visuals->VariableText);
                keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
                SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
                SDL_strlcat(visuals->VariableText, "] On Keyboard To Continue!)", sizeof visuals->VariableText);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            }

            if (logic->PAUSEgameQuitJoy == false)
            {
                visuals->DrawTextOntoScreenBuffer("[Continue Game!]", visuals->Font[1]
                                                  , 0, 265+50, JustifyCenter, 100, 255, 100, 0, 0, 0);
                visuals->DrawTextOntoScreenBuffer("Quit Game!", visuals->Font[1]
                                                  , 0, 265+50+50, JustifyCenter, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PAUSEgameQuitJoy == true)
            {
                visuals->DrawTextOntoScreenBuffer("Continue Game!", visuals->Font[1]
                                                  , 0, 265+50, JustifyCenter, 255, 255, 255, 0, 0, 0);
                visuals->DrawTextOntoScreenBuffer("[Quit Game!]", visuals->Font[1]
                                                  , 0, 265+50+50, JustifyCenter, 255, 100, 100, 0, 0, 0);
            }

            if (input->JoystickDirectionVertical[Any] == UP)
            {
                if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgameQuitJoy = true;
                }
                else  logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
            else if (input->JoystickDirectionVertical[Any] == DOWN)
            {
                if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgameQuitJoy = true;
                }
                else  logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
            else if (input->JoystickButtonOnePressed[Any] == ON || input->JoystickButtonOne[Any] == ON)
            {
                if (logic->PAUSEgameQuitJoy == true)
                {
                    ScreenTransitionStatus = FadeOut;
                    logic->GameForfeit = true;
                }
                else if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgame = false;

                    Mix_ResumeMusic();
                }

                logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
        }

        ScreenIsDirty = 2;
    }

    if (logic->PlayerData[0].PlayerStatus == GameOver && logic->PlayerData[1].PlayerStatus == GameOver
     && logic->PlayerData[2].PlayerStatus == GameOver && logic->PlayerData[3].PlayerStatus == GameOver)
    {
        if (logic->GameOverTimer < 50)
        {
            logic->GameOverTimer++;
        }
        else
        {
            ScreenTransitionStatus = FadeOut;
        }
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->DeletePieceFromPlayfieldMemory(DropShadow);
        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = HighScoresScreen;

        if (logic->GameForfeit == false)
        {
            data->CheckForNewHighScore();

            if (data->PlayerRankOnGameOver < 10)
            {
                if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickFour )
                    ScreenToDisplay = NameInputJoystickScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                    ScreenToDisplay = NameInputKeyboardScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                    ScreenToDisplay = NameInputMouseScreen;
            }
        }

        audio->PlayMusic(0, -1);

        if (logic->Won == true)
        {
            ScreenToDisplay = AboutScreen;
            audio->PlayMusic(25, -1);
        }

        visuals->FrameLock = 16;

        visuals->ClearTextCache();
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayShowStoryScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        Mix_HaltMusic();

        if (logic->PlayerData[1].Level == 0)
        {
            audio->PlayMusic(26 , -1);
        }
        else if (logic->PlayerData[1].Level == 1)
        {
            audio->PlayMusic(26 , -1);
        }
        else if (logic->PlayerData[1].Level == 2)
        {
            audio->PlayMusic(26 , -1);
        }
        else if (logic->PlayerData[1].Level == 3)
        {
            audio->PlayMusic(27 , -1);
        }
        else if (logic->PlayerData[1].Level == 4)
        {
            audio->PlayMusic(27 , -1);
        }
        else if (logic->PlayerData[1].Level == 5)
        {
            audio->PlayMusic(27 , -1);
        }
        else if (logic->PlayerData[1].Level == 6)
        {
            audio->PlayMusic(28 , -1);
        }
        else if (logic->PlayerData[1].Level == 7)
        {
            audio->PlayMusic(28 , -1);
        }
        else if (logic->PlayerData[1].Level == 8)
        {
            audio->PlayMusic(28 , -1);
        }
        else
        {
            audio->PlayMusic(29, -1);
        }

        ScreenDisplayTimer = 350;

        if (logic->PlayerData[1].Level == 6)
        {
            ScreenDisplayTimer = 0;
            input->DelayAllUserInput = 20;
        }

        ScreenTransitionStatus = FadeIn;
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON )
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        SkipEntireStoryCutsceneAndPlay = true;
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        int storyImageToShow = 70;
        if (logic->PlayerData[1].Level == 0)
        {
            storyImageToShow = 70;
            logic->StoryShown[0] = 1;
        }
        else if (logic->PlayerData[1].Level == 1)
        {
            storyImageToShow = 70;
            logic->StoryShown[1] = 1;
        }
        else if (logic->PlayerData[1].Level == 2)
        {
            storyImageToShow = 70;
            logic->StoryShown[2] = 1;
        }
        else if (logic->PlayerData[1].Level == 3)
        {
            storyImageToShow = 71;
            logic->StoryShown[3] = 1;
        }
        else if (logic->PlayerData[1].Level == 4)
        {
            storyImageToShow = 71;
            logic->StoryShown[4] = 1;
        }
        else if (logic->PlayerData[1].Level == 5)
        {
            storyImageToShow = 71;
            logic->StoryShown[5] = 1;
        }
        else if (logic->PlayerData[1].Level == 6)
        {
            storyImageToShow = 73;
            logic->StoryShown[6] = 1;
        }
        else if (logic->PlayerData[1].Level == 7)
        {
            storyImageToShow = 73;
            logic->StoryShown[7] = 1;
        }
        else if (logic->PlayerData[1].Level == 8)
        {
            storyImageToShow = 73;
            logic->StoryShown[8] = 1;
        }
        else if (logic->PlayerData[1].Level == 9)
        {
            storyImageToShow = 72;
            logic->StoryShown[9] = 1;
        }

        visuals->Sprites[storyImageToShow].ScreenX = 320;
        visuals->Sprites[storyImageToShow].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(storyImageToShow);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = PlayingStoryGameScreen;

        if (logic->PlayerData[1].Level == 0)
        {
            ScreenToDisplay = FlyingFromEarthScreen;
        }
        else if (logic->PlayerData[1].Level == 6)
        {
            ScreenToDisplay = FlyingToBaseScreen;
        }

        if (logic->GameForfeit == true)
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayFlyingFromEarthScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        IntroAnimationStep = 0;
        PlanetX = (320);
        PlanetY = (240);
        PlanetScale = 0.0;
        ShipX = (320-120-50);
        ShipY = (240-90);
        ShipScale = 0.0;

        ScreenTransitionStatus = FadeIn;

        if (SkipEntireStoryCutsceneAndPlay == true)
        {
            ScreenTransitionStatus = FadeOut;
        }
    }

    if (IntroAnimationStep == 0)
    {
        if (PlanetScale < 0.5)
        {
            PlanetScale+=0.005f;
        }
        else
        {
            IntroAnimationStep = 1;
        }
    }
    else if (IntroAnimationStep == 1)
    {
        if ( PlanetX > (320-120-50) )  PlanetX-=1;
        else  PlanetX = (320-120-50);

        if ( PlanetY > (240-90) )  PlanetY-=1;
        else  PlanetY = (240-90);

        if ( PlanetX == (320-120-50) && PlanetY == (240-90) )
        {
            IntroAnimationStep = 2;
            audio->PlayDigitalSoundFX(16, 0);
        }
    }
    else if (IntroAnimationStep == 2)
    {
        if (ShipScale < 1.0 )
        {
            ShipScale+=0.005f;
        }
        else  IntroAnimationStep = 3;
    }
    else if (IntroAnimationStep == 3)
    {
        PlanetX-=0.5;
        PlanetY-=0.5;
        PlanetScale-=0.05f;

        ShipX+=5.0;
        ShipY+=5.0;

        if (ShipY > 600)  IntroAnimationStep = 4;
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        SkipEntireStoryCutsceneAndPlay = true;
        ScreenTransitionStatus = FadeOut;
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || IntroAnimationStep == 4
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 20;
        if (IntroAnimationStep < 4)  audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[39].ScreenX = 320;
        visuals->Sprites[39].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(39);

        visuals->Sprites[42].ScreenX = PlanetX;
        visuals->Sprites[42].ScreenY = PlanetY;
        visuals->Sprites[42].ScaleX = PlanetScale;
        visuals->Sprites[42].ScaleY = PlanetScale;
        visuals->DrawSpriteOntoScreenBuffer(42);

        visuals->Sprites[43].ScreenX = ShipX;
        visuals->Sprites[43].ScreenY = ShipY;
        visuals->Sprites[43].ScaleX = ShipScale;
        visuals->Sprites[43].ScaleY = ShipScale;
        visuals->DrawSpriteOntoScreenBuffer(43);

        ScreenIsDirty = 2;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (logic->GameForfeit == false)
            ScreenToDisplay = FlyingToMarsScreen;
        else
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayFlyingToMarsScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        IntroAnimationStep = 0;
        PlanetX = (320+120);
        PlanetY = (240-90);
        PlanetScale = 0.0;
        ShipX = (0-200);
        ShipY = (480+200);
        ShipScale = 1.0;

        audio->PlayDigitalSoundFX(16, 0);

        ScreenTransitionStatus = FadeIn;

        if (SkipEntireStoryCutsceneAndPlay == true)
        {
            ScreenTransitionStatus = FadeOut;
            SkipEntireStoryCutsceneAndPlay = false;
        }
    }

    if (IntroAnimationStep == 0)
    {
        if ( ShipX < (100) )
        {
            ShipX+=2;
            ShipY-=2;
        }
        else  IntroAnimationStep = 1;
    }
    else if (IntroAnimationStep == 1)
    {
        if (PlanetScale < 0.7)
        {
            PlanetScale+=0.005f;
        }
        else
        {
            audio->PlayDigitalSoundFX(16, 0);
            IntroAnimationStep = 2;
        }
    }
    else if (IntroAnimationStep == 2)
    {
        if (ShipScale > 0.0)
        {
            ShipScale-=0.005f;
            ShipX+=1.7f;
            ShipY-=1.15f;
        }
        else  IntroAnimationStep = 3;
    }
    else if (IntroAnimationStep == 3)
    {
        if (PlanetX > 320)  PlanetX-=1;
        else  PlanetX = 320;

        if (PlanetY < 240)  PlanetY+=1;
        else  PlanetY = 240;

        if (PlanetX == 320 && PlanetY == 240)
        {
            IntroAnimationStep = 4;
        }
    }
    else if (IntroAnimationStep == 4)
    {
        if (PlanetScale > 0.0)
        {
            PlanetScale-=0.005f;
        }
        else  IntroAnimationStep = 5;
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || IntroAnimationStep == 5
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 20;
        if (IntroAnimationStep < 5)  audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[39].ScreenX = 320;
        visuals->Sprites[39].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(39);

        visuals->Sprites[40].ScreenX = PlanetX;
        visuals->Sprites[40].ScreenY = PlanetY;
        visuals->Sprites[40].ScaleX = PlanetScale;
        visuals->Sprites[40].ScaleY = PlanetScale;
        visuals->DrawSpriteOntoScreenBuffer(40);

        visuals->Sprites[41].ScreenX = ShipX;
        visuals->Sprites[41].ScreenY = ShipY;
        visuals->Sprites[41].ScaleX = ShipScale;
        visuals->Sprites[41].ScaleY = ShipScale;
        visuals->DrawSpriteOntoScreenBuffer(41);

        ScreenIsDirty = 2;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        if (logic->GameForfeit == false)
            ScreenToDisplay = PlayingStoryGameScreen;
        else
        {
            ScreenToDisplay = TitleScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayFlyingToBaseScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        IntroAnimationStep = 0;
        ShipX = (0-200);
        ShipY = (480+200);
        ShipScale = 1.0;

        audio->PlayDigitalSoundFX(16, 0);

        ScreenTransitionStatus = FadeIn;

        if (SkipEntireStoryCutsceneAndPlay == true)
        {
            ScreenTransitionStatus = FadeOut;
        }
    }

    if (IntroAnimationStep == 0)
    {
        if ( ShipX < (100-30) )
        {
            ShipX+=2;
            ShipY-=2;
        }
        else  IntroAnimationStep = 1;
    }
    else if (IntroAnimationStep == 1)
    {
        if (ShipScale > 0.25)
        {
            ShipScale-=0.005f;
            ShipX+=1.7f;
            ShipY-=1.15f;
        }
        else  IntroAnimationStep = 2;
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || IntroAnimationStep == 2
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 20;
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[73].ScreenX = 320;
        visuals->Sprites[73].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(73);

        visuals->Sprites[41].ScreenX = ShipX;
        visuals->Sprites[41].ScreenY = ShipY;
        visuals->Sprites[41].ScaleX = ShipScale;
        visuals->Sprites[41].ScaleY = ShipScale;
        visuals->DrawSpriteOntoScreenBuffer(41);

        ScreenIsDirty = 2;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = PlayingStoryGameScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayMarsExplodingScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        IntroAnimationStep = 0;
        PlanetX = (320);
        PlanetY = (240);
        PlanetScale = 0.0;
        ShipX = (320);
        ShipY = (240);
        ShipScale = 0.0;
        ExplosionScale = 0.0;
        ExplosionTransparency = 0;

        ScreenTransitionStatus = FadeIn;
    }

    if (IntroAnimationStep == 0)
    {
        if ( PlanetScale < (1.0) )
        {
            PlanetScale+=0.005f;
        }
        else
        {
            audio->PlayDigitalSoundFX(16, 0);
            IntroAnimationStep = 1;
        }
    }
    else if (IntroAnimationStep == 1)
    {
        if (ShipScale < 1.0)
        {
            ShipScale+=0.005f;
        }
        else
        {
            IntroAnimationStep = 2;
        }
    }
    else if (IntroAnimationStep == 2)
    {
        if ( PlanetScale > (0.0) )
        {
            PlanetScale-=0.005f;
        }
        else
        {
            audio->PlayDigitalSoundFX(17, 0);
            ExplosionTransparency = 255;
            IntroAnimationStep = 3;
        }
    }
    else if (IntroAnimationStep == 3)
    {
        if (ExplosionScale < 10)
        {
            ExplosionScale+=0.1f;
        }
        else if (ExplosionTransparency > 0)
        {
            ExplosionTransparency-=1;
        }
        else
        {
            audio->PlayDigitalSoundFX(16, 0);
            IntroAnimationStep = 4;
        }
    }
    else if (IntroAnimationStep == 4)
    {
        if ( ShipX < (640+150) )
        {
            ShipX+=5;
            ShipY+=5;
        }
        else
        {
            IntroAnimationStep = 5;
        }
    }

    if ( (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
       || IntroAnimationStep == 5
       || input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenTransitionStatus = FadeOut;
        input->DelayAllUserInput = 20;
        if (IntroAnimationStep < 5)  audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[39].ScreenX = 320;
        visuals->Sprites[39].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(39);

        visuals->Sprites[40].ScreenX = PlanetX;
        visuals->Sprites[40].ScreenY = PlanetY;
        visuals->Sprites[40].ScaleX = PlanetScale;
        visuals->Sprites[40].ScaleY = PlanetScale;
        visuals->DrawSpriteOntoScreenBuffer(40);

        visuals->Sprites[44].ScreenX = PlanetX;
        visuals->Sprites[44].ScreenY = PlanetY;
        visuals->Sprites[44].ScaleX = ExplosionScale;
        visuals->Sprites[44].ScaleY = ExplosionScale;
        visuals->Sprites[44].Transparency = ExplosionTransparency;
        visuals->DrawSpriteOntoScreenBuffer(44);

        visuals->Sprites[43].ScreenX = ShipX;
        visuals->Sprites[43].ScreenY = ShipY;
        visuals->Sprites[43].ScaleX = ShipScale;
        visuals->Sprites[43].ScaleY = ShipScale;
        visuals->DrawSpriteOntoScreenBuffer(43);

        ScreenIsDirty = 2;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = AboutScreen;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayPlayingStoryGameScreen(void)
{
const char* keyName;

    if (ScreenTransitionStatus == FadeAll)
    {
        logic->SetupForNewLevelStory();

        visuals->FrameLock = logic->PlayingGameFrameLock;

        logic->PlayersCanJoin = false;

        logic->PlayerData[0].PlayerStatus = GameOver;
        logic->PlayerData[2].PlayerStatus = GameOver;
        logic->PlayerData[3].PlayerStatus = GameOver;

        ScreenTransitionStatus = FadeIn;
    }

    if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_s)
    {
        logic->PlayerData[1].Level++;
        if (logic->PlayerData[1].Level == 10)
        {
            logic->Won = true;
            logic->PlayerData[1].PlayerStatus = GameOver;
            visuals->FrameLock = 16;
            audio->PlayMusic(30, -1);
            ScreenToDisplay = MarsExplodingScreen;
        }

        ScreenTransitionStatus = FadeAll;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    logic->RunTetriGameEngine();

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

    if (ScreenIsDirty > 0)
    {
        visuals->Sprites[108].ScreenX = 320;
        visuals->Sprites[108].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(108);

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 150;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        float mouseScreenX = -999;
        float mouseScreenY = -999;
        int mousePlayfieldY = -999;
        for (int player = 1; player < 2; player++)
        {
            float boxScreenX = 237;
            float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

            for (int y = 0; y < 26; y++)
            {
                for (int x = 0; x < 12; x++)
                {
                    if (logic->PlayerData[player].Playfield[x][y] == 1)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 1.0f;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 10
                             && logic->PlayerData[player].Playfield[x][y] < 20)
                    {
                        int spriteIndex = 200;

                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScaleX = 1.0f;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 20
                             && logic->PlayerData[player].Playfield[x][y] < 30)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 1.0f;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }

                    if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
                    {
                        if (  input->MouseX >= ( boxScreenX-(13/2) ) && input->MouseX <= ( boxScreenX+(13/2) )
                        && input->MouseY >= ( boxScreenY-(18/2) ) && input->MouseY <= ( boxScreenY+(18/2) )  )
                        {
                            mouseScreenX = boxScreenX-13;
                            mouseScreenY = boxScreenY;
                            mousePlayfieldY = y;
                        }
                    }

                    boxScreenX+=(13);
                }

                boxScreenX = 237;
                boxScreenY+=18;
            }
        }

        for (int player = 1; player < 2; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Mouse && logic->PlayerData[player].PlayerStatus == PieceFalling)
            {
                int box = 0;
                for (int y = 0; y < 4; y++)
                {
                    for (int x = 0; x < 4; x++)
                    {
                        box++;
                        if (logic->PieceData[logic->PlayerData[player].Piece][logic->PlayerData[player].PieceRotation] [box] == 1)
                        {
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenX = mouseScreenX;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].ScreenY = mouseScreenY;
                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 128;
                            visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet)+logic->PlayerData[player].Piece);

                            visuals->Sprites[201 + (10*logic->TileSet)+logic->PlayerData[player].Piece].Transparency = 255;
                        }

                        mouseScreenX+=13;
                    }

                    mouseScreenX-=(4*13);
                    mouseScreenY+=18;
                }

                if (mousePlayfieldY >= logic->PlayerData[player].PiecePlayfieldY)
                {
                    visuals->DrawTextOntoScreenBuffer("MOVE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
                }
                else  visuals->DrawTextOntoScreenBuffer("ROTATE", visuals->Font[2], mouseScreenX+(13*1), mouseScreenY-(18*2), JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
        }

        if (logic->PlayerData[1].PlayerStatus == GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[31].Transparency = 200;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);

            visuals->DrawTextOntoScreenBuffer("GAME OVER"
                                      , visuals->Font[1], logic->PlayerData[1].PlayersPlayfieldScreenX
                                      , 240, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        for (int player = 1; player < 2; player++)
        {
            #ifdef _WIN32
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
            #else
                sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2+1
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        }

        for (int player = 1; player < 2; player++)
        {
            if (logic->PlayerData[player].PlayerInput == Keyboard)
                visuals->DrawTextOntoScreenBuffer("Keyboard"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickOne)
                visuals->DrawTextOntoScreenBuffer("Joystick #1"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickTwo)
                visuals->DrawTextOntoScreenBuffer("Joystick #2"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == JoystickThree)
                visuals->DrawTextOntoScreenBuffer("Joystick #3"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            else if (logic->PlayerData[player].PlayerInput == CPU)
            {
                visuals->DrawTextOntoScreenBuffer("C.P.U."
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PlayerData[player].PlayerInput == Mouse)
                visuals->DrawTextOntoScreenBuffer("Mouse"
                                          , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                          , 460+3, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);
        }

        if (logic->PAUSEgame == true && input->DEBUG == 0)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 150;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            if (input->UserDefinedKeyPause == -1)
                visuals->DrawTextOntoScreenBuffer("(Press [P] On Keyboard To Continue!)", visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            else
            {
                SDL_strlcpy(visuals->VariableText, "(Press [", sizeof visuals->VariableText);
                keyName = SDL_GetKeyName(input->UserDefinedKeyPause);
                SDL_strlcat(visuals->VariableText, keyName, sizeof visuals->VariableText);
                SDL_strlcat(visuals->VariableText, "] On Keyboard To Continue!)", sizeof visuals->VariableText);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[1]
                                                  , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
            }

            if (logic->PAUSEgameQuitJoy == false)
            {
                visuals->DrawTextOntoScreenBuffer("[Continue Game!]", visuals->Font[1]
                                                  , 0, 265+50, JustifyCenter, 100, 255, 100, 0, 0, 0);
                visuals->DrawTextOntoScreenBuffer("Quit Game!", visuals->Font[1]
                                                  , 0, 265+50+50, JustifyCenter, 255, 255, 255, 0, 0, 0);
            }
            else if (logic->PAUSEgameQuitJoy == true)
            {
                visuals->DrawTextOntoScreenBuffer("Continue Game!", visuals->Font[1]
                                                  , 0, 265+50, JustifyCenter, 255, 255, 255, 0, 0, 0);
                visuals->DrawTextOntoScreenBuffer("[Quit Game!]", visuals->Font[1]
                                                  , 0, 265+50+50, JustifyCenter, 255, 100, 100, 0, 0, 0);
            }

            if (input->JoystickDirectionVertical[Any] == UP)
            {
                if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgameQuitJoy = true;
                }
                else  logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
            else if (input->JoystickDirectionVertical[Any] == DOWN)
            {
                if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgameQuitJoy = true;
                }
                else  logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
            else if (input->JoystickButtonOnePressed[Any] == ON || input->JoystickButtonOne[Any] == ON)
            {
                if (logic->PAUSEgameQuitJoy == true)
                {
                    ScreenTransitionStatus = FadeOut;
                    logic->GameForfeit = true;
                }
                else if (logic->PAUSEgameQuitJoy == false)
                {
                    logic->PAUSEgame = false;

                    Mix_ResumeMusic();
                }

                logic->PAUSEgameQuitJoy = false;

                audio->PlayDigitalSoundFX(1, 0);

                input->DelayAllUserInput = 20;
            }
        }

        SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->StoryLevelAdvanceCounter);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6], logic->PlayerData[1].PlayersPlayfieldScreenX
                                          , 440+5, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        ScreenIsDirty = 2;
    }

    for (logic->Player = 1; logic->Player < 2; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->DeletePieceFromPlayfieldMemory(DropShadow);
        }
    }

    if (logic->PlayerData[1].PlayerStatus == GameOver)
    {
        if (logic->GameOverTimer < 50)
        {
            logic->GameOverTimer++;
        }
        else
        {
            ScreenTransitionStatus = FadeOut;
        }
    }

    if (logic->PlayerData[1].Level == 3 && logic->StoryShown[3] == 0)
    {
        logic->StoryShown[3] = 1;
        ScreenTransitionStatus = FadeOut;
    }
    else if (logic->PlayerData[1].Level == 6 && logic->StoryShown[6] == 0)
    {
        logic->StoryShown[6] = 1;
        ScreenTransitionStatus = FadeOut;
    }
    else if (logic->PlayerData[1].Level == 9 && logic->StoryShown[9] == 0)
    {
        logic->StoryShown[9] = 1;
        ScreenTransitionStatus = FadeOut;
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        visuals->Sprites[201 + (10*logic->TileSet)].ScaleX = 1;
        visuals->FrameLock = 16;
        visuals->ClearTextCache();

        if (logic->PlayerData[1].PlayerStatus != GameOver && logic->GameForfeit != true)
        {
            ScreenToDisplay = ShowStoryScreen;

        }
        else if (logic->GameForfeit != true)
        {
            int spriteIndex;
            for (  spriteIndex = ( 200 + (10*logic->TileSet) ); spriteIndex < ( 200 + (10*logic->TileSet) + 7 ); spriteIndex++  )
            {
                visuals->Sprites[spriteIndex].ScaleX = 1;
            }

            ScreenToDisplay = HighScoresScreen;

            data->CheckForNewHighScore();

            if (data->PlayerRankOnGameOver < 10 && logic->GameForfeit == false)
            {
                if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree
                         || logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickFour )
                    ScreenToDisplay = NameInputJoystickScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Keyboard)
                    ScreenToDisplay = NameInputKeyboardScreen;
                else if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == Mouse)
                    ScreenToDisplay = NameInputMouseScreen;
            }

            audio->PlayMusic(0, -1);

            if (logic->Won == true)
            {
                ScreenToDisplay = MarsExplodingScreen;
                audio->PlayMusic(30, -1);
            }
        }
        else
        {
            ScreenToDisplay = HighScoresScreen;
            audio->PlayMusic(0, -1);
        }

        ScreenTransitionStatus = FadeAll;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputKeyboardScreen(void)
{
bool lastKeyWasNotAcceptable = false;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputArayIndex = 0;
    }

    if (input->KeyOnKeyboardPressedByUser == SDLK_RETURN)
    {
        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';

        audio->PlayDigitalSoundFX(0, 0);
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_BACKSPACE)
    {
        if (data->NameInputArayIndex == 19)
        {
            data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        }

        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

        input->DelayAllUserInput = 10;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_SPACE)
    {
        if (data->NameInputArayIndex < HighScoreNameMaxIndex-1)
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                = ' ';

        if (data->NameInputArayIndex < HighScoreNameMaxIndex)  data->NameInputArayIndex++;

        input->DelayAllUserInput = 10;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
    }
    else if (input->KeyOnKeyboardPressedByUser > -1)
    {
        if (data->NameInputArayIndex < HighScoreNameMaxIndex)
        {
            if (input->KeyOnKeyboardPressedByUser >= SDLK_a && input->KeyOnKeyboardPressedByUser <= SDLK_z)
            {
                if (input->ShiftKeyPressed == false)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = input->KeyOnKeyboardPressedByUser;
                else  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ( char(input->KeyOnKeyboardPressedByUser) - 32 );
            }
            else if (input->KeyOnKeyboardPressedByUser >= SDLK_0 && input->KeyOnKeyboardPressedByUser <= SDLK_9)
            {
                if (input->ShiftKeyPressed == false)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = input->KeyOnKeyboardPressedByUser;
            }
            else if (input->ShiftKeyPressed == true && input->KeyOnKeyboardPressedByUser == SDLK_EQUALS)
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '+';
            else
                lastKeyWasNotAcceptable = true;
        }

        if (lastKeyWasNotAcceptable == false && data->NameInputArayIndex < HighScoreNameMaxIndex)
        {
            data->NameInputArayIndex++;

            input->DelayAllUserInput = 10;

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = 2;
        }
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Enter your name using the keyboard:", visuals->Font[0],
                                          0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 207, JustifyCenter, 255, 255, 0, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Backspace] key to erase.", visuals->Font[0],
                                          0, 305, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Press [Enter] key when finished.", visuals->Font[0],
                                          0, 345, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputMouseScreen(void)
{
char temp[256];
char charToAccept[256];
bool lastKeyWasNotAcceptable = false;

    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputArayIndex = 0;

        float screenY = 250;
        float screenX = 63;
        int index = -1;
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 13; x++)
            {
                index++;

                interface->CreateIcon(1020+index, screenX, screenY);

                screenX+=(26+17);
            }

            screenX = 63;
            screenY+=(36+2);
        }
    }

    if (interface->IconSelectedByPlayer == 64)
    {
        if (data->NameInputArayIndex == HighScoreNameMaxIndex)
        {
            data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        }

        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
        if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
    }
    else if (interface->IconSelectedByPlayer == 63)
    {
        if (data->NameInputArayIndex < HighScoreNameMaxIndex-1)
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        if (data->NameInputArayIndex < HighScoreNameMaxIndex)  data->NameInputArayIndex++;

        input->DelayAllUserInput = 20;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
    }
    else if (interface->IconSelectedByPlayer > -1 && interface->IconSelectedByPlayer < 63)
    {
        if (data->NameInputArayIndex < HighScoreNameMaxIndex-1)
        {

            if (interface->IconSelectedByPlayer == 62)
            {
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '+';
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > -1 && interface->IconSelectedByPlayer < 13+13)
            {
                SDL_snprintf ( temp, sizeof temp, "%c", (65+interface->IconSelectedByPlayer) );
                SDL_strlcpy(charToAccept, temp, sizeof charToAccept);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > 13+13-1 && interface->IconSelectedByPlayer < 13+13+13+13)
            {
                SDL_snprintf ( temp, sizeof temp, "%c", (97+(interface->IconSelectedByPlayer-26)) );
                SDL_strlcpy(charToAccept, temp, sizeof charToAccept);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
            else if (interface->IconSelectedByPlayer > 13+13+13+13-1 && interface->IconSelectedByPlayer < 13+13+13+13+11)
            {
                SDL_snprintf ( temp, sizeof temp, "%c", (48+(interface->IconSelectedByPlayer-26-26)) );
                SDL_strlcpy(charToAccept, temp, sizeof charToAccept);
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = charToAccept[0];
                data->NameInputArayIndex++;
                lastKeyWasNotAcceptable = true;
            }
        }

        if (lastKeyWasNotAcceptable == false && data->NameInputArayIndex < HighScoreNameMaxIndex)
        {
            data->NameInputArayIndex++;

            input->DelayAllUserInput = 20;

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = 2;
        }
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Enter your name using the mouse:", visuals->Font[0],
                                          0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 207-25, JustifyCenter, 255, 255, 0, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayNameInputJoystickScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        interface->CreateButton(1008, 0, 454);

        ScreenTransitionStatus = FadeIn;

        data->NameInputJoyCharX = 0;
        data->NameInputJoyCharY = 0;
        data->NameInputJoyChar = 'A';
        data->NewHighScoreRegistered = false;

        data->NameInputArayIndex = 0;
    }

    if (input->JoystickButtonOne[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
        {
            if (data->NameInputJoyChar != (char)NULL)
            {
                if (data->NameInputJoyCharY == 5)
                {
                    if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] == '\0')
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] = ' ';

                    ScreenTransitionStatus = FadeOut;
                }
                else if (data->NameInputJoyChar == '<')
                {
                    if (data->NameInputArayIndex == HighScoreNameMaxIndex)
                    {
                        data->NameInputArayIndex--;
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                    }

                    data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                    if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
                    data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
                }
                else if (data->NameInputJoyChar == '_')
                {
                    if (data->NameInputArayIndex < HighScoreNameMaxIndex-1)
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                            = ' ';

                    if (data->NameInputArayIndex < HighScoreNameMaxIndex)  data->NameInputArayIndex++;
                }
                else if (data->NameInputJoyCharY != 5)
                {
                    if (data->NameInputArayIndex < HighScoreNameMaxIndex-1)
                        data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex]
                            = data->NameInputJoyChar;

                    if (data->NameInputArayIndex < HighScoreNameMaxIndex)  data->NameInputArayIndex++;
                }

                audio->PlayDigitalSoundFX(0, 0);
                ScreenIsDirty = 2;
                input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
            }
        }
    }
    else if (input->JoystickButtonTwo[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == ON)
    {
        if (input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == false)
        {
            if (data->NameInputArayIndex == HighScoreNameMaxIndex)
            {
                data->NameInputArayIndex--;
                data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
            }

            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';
            if (data->NameInputArayIndex > 0)  data->NameInputArayIndex--;
            data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = '\0';

            audio->PlayDigitalSoundFX(0, 0);
            ScreenIsDirty = 2;
            input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = true;
        }
    }
    else
    {
        input->JoystickButtonOnePressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
        input->JoystickButtonTwoPressed[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] = false;
    }

    if (input->JoystickDirectionVertical[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == UP)
    {
        if (data->NameInputJoyCharY > 0)  data->NameInputJoyCharY--;
        else  data->NameInputJoyCharY = 5;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionVertical[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == DOWN)
    {
        if (data->NameInputJoyCharY < 5)  data->NameInputJoyCharY++;
        else  data->NameInputJoyCharY = 0;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionHorizontal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == LEFT)
    {
        if (data->NameInputJoyCharX > 0)  data->NameInputJoyCharX--;
        else  data->NameInputJoyCharX = 12;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
        input->DelayAllUserInput = 6;
    }
    else if (input->JoystickDirectionHorizontal[logic->PlayerData[data->PlayerWithHighestScore].PlayerInput] == RIGHT)
    {
        if (data->NameInputJoyCharX < 12)  data->NameInputJoyCharX++;
        else  data->NameInputJoyCharX = 0;

        audio->PlayDigitalSoundFX(0, 0);
        ScreenIsDirty = 2;
        input->DelayAllUserInput = 6;
    }

    if (data->NameInputJoyCharY == 0)
    {
        data->NameInputJoyChar = char('A'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 1)
    {
        data->NameInputJoyChar = char('N'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 2)
    {
        data->NameInputJoyChar = char('a'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 3)
    {
        data->NameInputJoyChar = char('n'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX < 10)
    {
        data->NameInputJoyChar = char('0'+data->NameInputJoyCharX);
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 10)
    {
        data->NameInputJoyChar = '+';
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 11)
    {
        data->NameInputJoyChar = '_';
    }
    else if (data->NameInputJoyCharY == 4 && data->NameInputJoyCharX == 12)
    {
        data->NameInputJoyChar = '<';
    }
    else if (data->NameInputJoyCharY == 5)
    {
        data->NameInputJoyChar = '/';
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[2].ScreenX = 320;
        visuals->Sprites[2].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(2);

        visuals->Sprites[0].ScreenX = 320;
        visuals->Sprites[0].ScreenY = 240;
        visuals->Sprites[0].Transparency = 200;
        visuals->DrawSpriteOntoScreenBuffer(0);

        visuals->DrawTextOntoScreenBuffer("H I G H   S C O R E   N A M E   I N P U T:", visuals->Font[0]
                                          , 0, 1, JustifyCenter, 255, 255, 1, 90, 90, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 7-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("You have achieved a new high score!", visuals->Font[0],
                                          0, 80, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickOne)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #1:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);
        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickTwo)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #2:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);
        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickThree)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #3:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);
        if (logic->PlayerData[data->PlayerWithHighestScore].PlayerInput == JoystickFour)
            visuals->DrawTextOntoScreenBuffer("Enter your name using Joystick #4:", visuals->Font[0],
                                              0, 125, JustifyCenter, 255, 255, 255, 1, 1, 1);

        if (data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][0] != (char)NULL)
            visuals->DrawTextOntoScreenBuffer(data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver],
                                              visuals->Font[0], 0, 190, JustifyCenter, 255, 255, 0, 1, 1, 1);

        float x = 45;
        for (char alphaNumeral = 'A'; alphaNumeral < 'N'; alphaNumeral++)
        {
            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 239, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'N'; alphaNumeral < '['; alphaNumeral++)
        {
            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 269, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'a'; alphaNumeral < 'n'; alphaNumeral++)
        {
            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 299, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = 'n'; alphaNumeral < '{'; alphaNumeral++)
        {
            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 329, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        x = 45;
        for (char alphaNumeral = '0'; alphaNumeral < ':'; alphaNumeral++)
        {
            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%c", alphaNumeral);

            if (alphaNumeral == data->NameInputJoyChar)
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
            else
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            x+=45;
        }

        if (data->NameInputJoyChar == '+')
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("+", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '_')
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("_", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyChar == '<')
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("<", visuals->Font[0], x, 359, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        x+=45;

        if (data->NameInputJoyCharY == 5)
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint, 255, 1, 1, 1, 1, 1);
        else
            visuals->DrawTextOntoScreenBuffer("END", visuals->Font[0], 320, 389, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 404-6+30;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
        ScreenToDisplay = HighScoresScreen;

        if (data->NameInputArayIndex == 0)  data->HighScoresName[logic->GameMode][data->PlayerRankOnGameOver][data->NameInputArayIndex] = ' ';

        logic->Won = false;

        input->DelayAllUserInput = 20;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayTestComputerSkillScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        logic->GameMode = OriginalMode;

        logic->CPUPlayerEnabled = 3;

        audio->MusicVolume = 0;
        audio->SoundVolume = 0;

        logic->SetupForNewGame();

        visuals->FrameLock = 16;

        input->DelayAllUserInput = 20;

        ScreenTransitionStatus = FadeIn;
    }

    logic->RunTetriGameEngine();

    if (input->KeyOnKeyboardPressedByUser == SDLK_t)
    {
        input->DelayAllUserInput = 20;

        if (visuals->FrameLock == 16)  visuals->FrameLock = 0;
        else  visuals->FrameLock = 16;
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_MINUS)
    {
        if (logic->Multiplier > -10)  logic->Multiplier-=0.025f;

        printf("Value: %f\n", logic->Multiplier);

        logic->SetupForNewGame();

        logic->TotalCPUPlayerLines = 0;
        logic->NumberofCPUGames = 4;
        logic->TotalOneLines = 0;
        logic->TotalTwoLines = 0;
        logic->TotalThreeLines = 0;
        logic->TotalFourLines = 0;

        input->DelayAllUserInput = 5;
    }
    else if (input->KeyOnKeyboardPressedByUser == SDLK_EQUALS)
    {
        if (logic->Multiplier < 10)  logic->Multiplier+=0.025f;

        printf("Value: %f\n", logic->Multiplier);

        logic->SetupForNewGame();

        logic->TotalCPUPlayerLines = 0;
        logic->NumberofCPUGames = 4;
        logic->TotalOneLines = 0;
        logic->TotalTwoLines = 0;
        logic->TotalThreeLines = 0;
        logic->TotalFourLines = 0;

        input->DelayAllUserInput = 5;
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        logic->PlayerData[logic->Player].TimeToDropPiece = 47;

        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->AddPieceToPlayfieldMemory(DropShadow);
            logic->AddPieceToPlayfieldMemory(Current);
        }
    }

    if (visuals->FrameLock > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 0);

        if (logic->PlayerData[0].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[0].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[0].PlayersPlayfieldScreenY;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 0;
            visuals->Sprites[31].Transparency = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[1].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[1].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[1].PlayersPlayfieldScreenY;
            visuals->Sprites[31].RedHue = 255;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[2].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[2].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[2].PlayersPlayfieldScreenY;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 255;
            visuals->Sprites[31].BlueHue = 0;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        if (logic->PlayerData[3].PlayerStatus != GameOver)
        {
            visuals->Sprites[31].ScreenX = logic->PlayerData[3].PlayersPlayfieldScreenX;
            visuals->Sprites[31].ScreenY = logic->PlayerData[3].PlayersPlayfieldScreenY;
            visuals->Sprites[31].RedHue = 0;
            visuals->Sprites[31].GreenHue = 0;
            visuals->Sprites[31].BlueHue = 255;
            visuals->DrawSpriteOntoScreenBuffer(31);
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            float boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
            float boxScreenY = logic->PlayerData[player].PlayersPlayfieldScreenY-212;

            for (int y = 0; y < 26; y++)
            {
                for (int x = 2; x < 12; x++)
                {
                    if (logic->PlayerData[player].Playfield[x][y] == 1)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 70;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 10
                             && logic->PlayerData[player].Playfield[x][y] < 20)
                    {
                        int spriteIndex = 200 + (10*logic->TileSet);

                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenX = boxScreenX;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].ScreenY = boxScreenY;
                        visuals->Sprites[spriteIndex-9+logic->PlayerData[player].Playfield[x][y]].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(spriteIndex-9+logic->PlayerData[player].Playfield[x][y]);

                    }
                    else if (logic->PlayerData[player].Playfield[x][y] > 20
                             && logic->PlayerData[player].Playfield[x][y] < 30)
                    {
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[201 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->Sprites[201 + (10*logic->TileSet)].Transparency = 255;
                        visuals->DrawSpriteOntoScreenBuffer(201 + (10*logic->TileSet));
                    }

                    if (logic->PlayerData[player].PlayfieldAI[x][y] == 999)
                    {
                        visuals->Sprites[200 + (10*logic->TileSet)].ScreenX = boxScreenX;
                        visuals->Sprites[200 + (10*logic->TileSet)].ScreenY = boxScreenY;
                        visuals->DrawSpriteOntoScreenBuffer(200 + (10*logic->TileSet));
                    }

                    boxScreenX+=13;
                }

                boxScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX-57;
                boxScreenY+=18;
            }

            if (logic->GameMode == TimeAttack30Mode || logic->GameMode == TimeAttack60Mode || logic->GameMode == TimeAttack120Mode)
            {
                Uint32 taTimer = logic->TimeAttackTimer / 200;
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", taTimer);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
            else if (logic->GameMode == TwentyLineChallengeMode)
            {
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].TwentyLineCounter);
                visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                                  , 440, JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
            }
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            #ifdef _WIN32
                SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%I64u", logic->PlayerData[player].Score);
            #else
                sprintf(visuals->VariableText, "%lu", logic->PlayerData[player].Score);
            #endif

            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[5]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX, 9-2
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Lines);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX-59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);

            SDL_snprintf (visuals->VariableText, sizeof visuals->VariableText, "%d", logic->PlayerData[player].Level);
            visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[6]
                                              , logic->PlayerData[player].PlayersPlayfieldScreenX+59, 62-1+6
                                              , JustifyCenterOnPoint, 255, 255, 255, 1, 1, 1);
        }

        for (int player = 0; player < NumberOfPlayers; player++)
        {
            visuals->DrawTextOntoScreenBuffer("C.P.U."
                                      , visuals->Font[2], logic->PlayerData[player].PlayersPlayfieldScreenX
                                      , 460, JustifyCenterOnPoint, 255, 255, 255, 0, 0, 0);

            if (logic->GameMode == CrisisMode && logic->Crisis7BGMPlayed == true)
            {
                for (int player = 0; player < NumberOfPlayers; player++)
                {
                    visuals->Sprites[155].ScreenX = logic->PlayerData[player].PlayersPlayfieldScreenX;
                    visuals->Sprites[155].ScreenY = 240;
                    visuals->Sprites[155].Transparency = 128;// 0.20f;
                    visuals->DrawSpriteOntoScreenBuffer(155);
                }
            }
        }

        if (logic->PAUSEgame == true && input->DEBUG == 0)
        {
            visuals->Sprites[0].ScreenX = 320;
            visuals->Sprites[0].ScreenY = 240;
            visuals->Sprites[0].Transparency = 128;// 0.85;
            visuals->DrawSpriteOntoScreenBuffer(0);

            visuals->DrawTextOntoScreenBuffer("G A M E   P A U S E D", visuals->Font[0]
                                              , 0, 225, JustifyCenter, 255, 255, 255, 90, 90, 90);

            visuals->DrawTextOntoScreenBuffer("(Press [SpaceBar] On Keyboard To Continue!)", visuals->Font[1]
                                              , 0, 265, JustifyCenter, 255, 255, 255, 90, 90, 90);
        }

        char temp[256];

        visuals->DrawTextOntoScreenBuffer("A.I. TEST", visuals->Font[7]
                                          , 0, 90+22, JustifyCenter, 255, 255, 255, 0, 0, 0);

        SDL_strlcpy(visuals->VariableText, "Number Of Games: ", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", logic->NumberofCPUGames);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 0, 130
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        SDL_strlcpy(visuals->VariableText, "Number Of Lines: ", sizeof visuals->VariableText);
        logic->TotalCPUPlayerLines = ( logic->TotalOneLines+(2*logic->TotalTwoLines)+(3*logic->TotalThreeLines)+(4*logic->TotalFourLines) );
        SDL_snprintf (temp, sizeof temp, "%d", logic->TotalCPUPlayerLines);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 0, 150
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        SDL_strlcpy(visuals->VariableText, "Completed Lines: 1=", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", logic->TotalOneLines);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        SDL_strlcat(visuals->VariableText, " 2=", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", logic->TotalTwoLines);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        SDL_strlcat(visuals->VariableText, " 3=", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", logic->TotalThreeLines);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        SDL_strlcat(visuals->VariableText, " 4=", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", logic->TotalFourLines);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 0, 195
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        visuals->DrawTextOntoScreenBuffer("Press [T] On Keyboard To Toggle Speed!", visuals->Font[7]
                                          , 0, 195+25, JustifyCenter, 255, 255, 255, 0, 0, 0);

        logic->TotalCPUPlayerLines = ( logic->TotalOneLines+(2*logic->TotalTwoLines)+(3*logic->TotalThreeLines)+(4*logic->TotalFourLines) );

        int averageLinesPerGame = logic->TotalCPUPlayerLines / logic->NumberofCPUGames;
        printf("Average Lines = %i \n", averageLinesPerGame);

        SDL_strlcpy(visuals->VariableText, "Average Lines Per Game: ", sizeof visuals->VariableText);
        SDL_snprintf (temp, sizeof temp, "%d", averageLinesPerGame);
        SDL_strlcat(visuals->VariableText, temp, sizeof visuals->VariableText);
        visuals->DrawTextOntoScreenBuffer(visuals->VariableText, visuals->Font[7], 0, 170
                                          , JustifyCenter, 255, 255, 255, 0, 0, 0);

        ScreenIsDirty = 2;
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus != FlashingCompletedLines
            && logic->PlayerData[logic->Player].PlayerStatus != ClearingCompletedLines)
        {
            logic->DeletePieceFromPlayfieldMemory(Current);
            logic->DeletePieceFromPlayfieldMemory(DropShadow);
        }
    }

    for (logic->Player = 0; logic->Player < NumberOfPlayers; logic->Player++)
    {
        if (logic->PlayerData[logic->Player].PlayerStatus == GameOver)
        {
            logic->NumberofCPUGames++;

            for (int y = 0; y < 26; y++)
                for (int x = 0; x < 15; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 255; /* Collision detection value */

            for (int y = 2; y < 5; y++)
                for (int x = 5; x < 9; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 0;

            for (int y = 5; y < 24; y++)
                for (int x = 2; x < 12; x++)
                    logic->PlayerData[logic->Player].Playfield[x][y] = 0;

            logic->PlayerData[logic->Player].PiecePlayfieldX = 5;
            logic->PlayerData[logic->Player].PiecePlayfieldY = 0;

            logic->FillPieceBag(logic->Player);
            logic->PlayerData[logic->Player].Piece = logic->PlayerData[logic->Player].PieceBag[0][logic->PlayerData[logic->Player].PieceBagIndex];
            logic->PlayerData[logic->Player].PieceBagIndex = 1;
            logic->PlayerData[logic->Player].NextPiece = logic->PlayerData[logic->Player].PieceBag[0][logic->PlayerData[logic->Player].PieceBagIndex];

            logic->PlayerData[logic->Player].Piece = logic->PlayerData[logic->Player].NextPiece;
            logic->PlayerData[logic->Player].PieceBagIndex = 2;
            logic->PlayerData[logic->Player].NextPiece = logic->PlayerData[logic->Player].PieceBag[0][logic->PlayerData[logic->Player].PieceBagIndex];

            logic->PlayerData[logic->Player].PieceMovementDelay = 0;
            logic->PlayerData[logic->Player].PieceRotation = 1;

            logic->PlayerData[logic->Player].PlayerStatus = NewPieceDropping;

            logic->PlayerData[logic->Player].PieceDropTimer = 0;
            logic->PlayerData[logic->Player].TimeToDropPiece = 47;

            logic->PlayerData[logic->Player].PieceRotated1 = false;
            logic->PlayerData[logic->Player].PieceRotated2 = false;

            logic->PlayerData[logic->Player].FlashCompletedLinesTimer = 0;
            logic->PlayerData[logic->Player].ClearCompletedLinesTimer = 0;

            logic->PlayerData[logic->Player].Score = 0;
            logic->PlayerData[logic->Player].DropBonus = 0;
            logic->PlayerData[logic->Player].Level = 0;
            logic->PlayerData[logic->Player].Lines = 0;

        }
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayGiveMeJobScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        if (Mix_PlayingMusic() == 1)  Mix_HaltMusic();

        interface->CreateButton( 1010, 0, 417 );
        interface->CreateButton( 1008, 1, 457 );

        interface->ButtonSelectedByKeyboard = 1;

        input->DelayAllUserInput = 50;

        ScreenTransitionStatus = FadeIn;
    }

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->DrawTextOntoScreenBuffer("The programmer/tester of this video game", visuals->Font[3]
                                  , 0, 1, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("is currently looking for employment:", visuals->Font[3]
                                  , 0, 1+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 1+26+40;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("INTERESTED IN:", visuals->Font[3]
                                  , 0, 1+26+20+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("- Entry-level QA Tester/Programmer positions -", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("- Work-from-home(telecommute) -", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("- Part-time schedule(15 hours a week max) -", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("- Minimum hourly wage -", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26+26+26, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 1+26+20+26+26+26+26+26+26+40;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);

        visuals->DrawTextOntoScreenBuffer("Click on [Send Email] button below to contact me!", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26+26+26+40+26-10, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("(will open default email client app)", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26+26+26+40+26+26-10, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->DrawTextOntoScreenBuffer("Thank you in advance!", visuals->Font[3]
                                  , 0, 1+26+20+26+26+26+26+26+26+40+26+26+26+10, JustifyCenter, 255, 255, 255, 1, 1, 1);

        visuals->Sprites[7].ScreenX = 320;
        visuals->Sprites[7].ScreenY = 385;
        visuals->Sprites[7].BlueHue = 0;
        visuals->DrawSpriteOntoScreenBuffer(7);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        if (interface->ButtonSelectedByPlayer == 0)
        {
            if (visuals->ForceAspectRatio == true)  SDL_RenderSetLogicalSize(visuals->Renderer, 640, 480);
            if (visuals->FullScreenMode == 1)  SDL_SetWindowFullscreen(visuals->Window, 0);


            #ifdef __unix__
                system( "xdg-open 'mailto:JesseLeePalser@Gmail.com'");
            #elif defined(_WIN32)
                SDL_OpenURL("mailto:JesseLeePalser@Gmail.com");
            #endif
        }

        visuals->Sprites[7].BlueHue = 255;

        input->EXIT_Game = true;
        ScreenTransitionStatus = FadeAll;
    }
}

//-------------------------------------------------------------------------------------------------
void Screens::DisplayCNETScreen(void)
{
    if (ScreenTransitionStatus == FadeAll)
    {
        ScreenDisplayTimer = 1000;
        ScreenTransitionStatus = FadeIn;
    }

    if (input->MouseButtonPressed[0] == true
       || input->KeyOnKeyboardPressedByUser == SDLK_SPACE
       || input->KeyOnKeyboardPressedByUser == SDLK_RETURN
       || input->JoystickButtonOne[Any] == ON)
    {
        ScreenDisplayTimer = 0;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }

    if (ScreenDisplayTimer > 0)  ScreenDisplayTimer--;
    else if (ScreenTransitionStatus != FadeIn)  ScreenTransitionStatus = FadeOut;

    if (ScreenIsDirty > 0)
    {
        visuals->ClearScreenBufferWithColor(0, 0, 0, 255);

        visuals->Sprites[5].ScreenX = 320;
        visuals->Sprites[5].ScreenY = 240;
        visuals->DrawSpriteOntoScreenBuffer(5);
    }

    if (ScreenTransitionStatus == FadeOut && ScreenFadeTransparency == 255)
    {
        ScreenTransitionStatus = FadeAll;

        ScreenToDisplay = SDLLogoScreen;

        if (input->UserPressedESC == 1)
        {
            input->UserPressedESC = 1999;
            ScreenToDisplay = SDLLogoScreen;
            audio->PlayMusic(0, -1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
