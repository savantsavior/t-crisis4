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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "input.h"

#include "visuals.h"
#include "screens.h"
#include "audio.h"
#include "logic.h"

extern Visuals* visuals;
extern Screens* screens;
extern Audio* audio;
extern Logic* logic;

//-------------------------------------------------------------------------------------------------
Input::Input(void)
{
    DEBUG = 0;

    EXIT_Game = false;

    DelayAllUserInput = 0;

    LastEventWasWindowResize = 0;

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;

    for (int padIndex = 0; padIndex < 4; padIndex++)
    {
        JoystickDisabled[padIndex] = true;
    }

    if (UseNewGamepadAPI == true)
    {
        int i;
        for(i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                printf("Gamepad \'%i\' is a compatible controller, named \'%s\'\n", i, SDL_GameControllerNameForIndex(i));
                GamepadController[i] = SDL_GameControllerOpen(i);
                Gamepad[i] = SDL_GameControllerGetJoystick(GamepadController[i]);
                JoystickDisabled[i] = false;
                SDL_strlcpy( JoystickName[i], SDL_GameControllerName(GamepadController[i]), sizeof JoystickName[i] );
            } else {
                printf("Index \'%i\' is not a compatible controller.\n", i);
                GamepadController[i] = NULL;
                JoystickDisabled[i] = true;
            }
        }
    }
    else if (UseNewGamepadAPI == false)
    {
        for (int index = 0; index < 4; index++)
        {
            JoystickDevices[index] = NULL;
            NumberOfJoyButtons[index] = 0;
            NumberOfJoyHats[index] = 0;
            NumberOfJoyAxises[index] = 0;
            JoystickDisabled[index] = true;
        }

        if (SDL_NumJoysticks() == 0)
        {
            printf("No USB joysticks are plugged in.\n");
        }
        else
        {
            for (int joyIndex = 0; joyIndex < 4; joyIndex++)
            {
                if (SDL_NumJoysticks() > joyIndex)
                {
                    JoystickDevices[joyIndex] = SDL_JoystickOpen(joyIndex);
                    printf("SDL2 Joystick #%i ''%s'' opened.\n", joyIndex, SDL_JoystickName(JoystickDevices[joyIndex]));

                    NumberOfJoyButtons[joyIndex] = SDL_JoystickNumButtons(JoystickDevices[joyIndex]);
                    NumberOfJoyHats[joyIndex] = SDL_JoystickNumHats(JoystickDevices[joyIndex]);
                    NumberOfJoyAxises[joyIndex] = SDL_JoystickNumAxes(JoystickDevices[joyIndex]);
                    if (NumberOfJoyAxises[joyIndex] > 3)  NumberOfJoyAxises[joyIndex] = 3;
                    printf("SDL2 Joystick #%i #Axis=%i, #Buttons=%i, #Hats=%i\n", joyIndex, NumberOfJoyAxises[joyIndex], NumberOfJoyButtons[joyIndex], NumberOfJoyHats[joyIndex]);

                    SDL_strlcpy(JoystickName[joyIndex], SDL_JoystickName(JoystickDevices[joyIndex]), sizeof JoystickName[joyIndex]);
                    JoystickDisabled[joyIndex] = false;
                }
            }
        }
    }

    for (int joy = 0; joy < 4; joy++)
    {
        JoyUP[joy] = Axis1;
        JoyDOWN[joy] = Axis1;
        JoyLEFT[joy] = Axis0;
        JoyRIGHT[joy] = Axis0;
        JoyButton1[joy] = Button0;
        JoyButton2[joy] = Button1;
        JoyPause[joy] = Button2;

        GamepadUP[joy] = SDL_CONTROLLER_BUTTON_DPAD_UP;
        GamepadDOWN[joy] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        GamepadLEFT[joy] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        GamepadRIGHT[joy] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        GamepadButton1[joy] = SDL_CONTROLLER_BUTTON_X;
        GamepadButton2[joy] = SDL_CONTROLLER_BUTTON_Y;
        GamepadPause[joy] = SDL_CONTROLLER_BUTTON_B;
    }

    JoystickSetupProcess = JoySetupNotStarted;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizontal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
        JoystickPause[index] = OFF;
    }

    KeyboardSetupProcess = KeyboardSetupNotStarted;
    UserDefinedKeyButtonOne = -1;
    UserDefinedKeyButtonTwo = -1;
    UserDefinedKeyUP = -1;
    UserDefinedKeyRIGHT = -1;
    UserDefinedKeyDOWN = -1;
    UserDefinedKeyLEFT = -1;
    UserDefinedKeyPause = -1;

    DelayAllUserInput = -1;
}

//-------------------------------------------------------------------------------------------------
Input::~Input(void)
{
    for (int index = 0; index < 4; index++)
    {
        if (UseNewGamepadAPI == true)
        {
            if (GamepadController[index] != NULL && JoystickDisabled[index] == false)  SDL_GameControllerClose(GamepadController[index]);
        }
        else
        {
            if (JoystickDevices[index] != NULL)  SDL_JoystickClose(JoystickDevices[index]);
        }
    }
}

//-------------------------------------------------------------------------------------------------
int Input::QueryJoysticksForAction(Uint8 joy)
{
int returnValue = JoyNotPressed;

	if (DelayAllUserInput > 0)  return(returnValue);

    if (UseNewGamepadAPI == true)
    {
        if (JoystickDisabled[joy] == false)
        {
            if (GetOnlyGamepadButtons == false)
            {
                for ( int indexTwo = SDL_CONTROLLER_BUTTON_DPAD_UP; indexTwo < (SDL_CONTROLLER_BUTTON_DPAD_RIGHT+1); indexTwo++ )
                {
                    if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                 }

                for ( int indexTwo = SDL_CONTROLLER_AXIS_LEFTX; indexTwo < (SDL_CONTROLLER_AXIS_RIGHTY+1); indexTwo++ )
                {
                    if (  SDL_GameControllerGetAxis( GamepadController[joy], (SDL_GameControllerAxis)indexTwo ) < (-32768/1.5)  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetAxis( GamepadController[joy], (SDL_GameControllerAxis)indexTwo ) > (32768/1.5)  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetAxis( GamepadController[joy], (SDL_GameControllerAxis)indexTwo ) < (-32768/1.5)  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetAxis( GamepadController[joy], (SDL_GameControllerAxis)indexTwo ) > (32768/1.5)  )
                    {
                        returnValue = indexTwo;
                    }
                }
            }
            else if (GetOnlyGamepadButtons == true)
            {
                for ( int indexTwo = SDL_CONTROLLER_BUTTON_A; indexTwo < (SDL_CONTROLLER_BUTTON_RIGHTSHOULDER+1); indexTwo++ )
                {
                    if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                    else if (  SDL_GameControllerGetButton( GamepadController[joy], (SDL_GameControllerButton)indexTwo ) == 1  )
                    {
                        returnValue = indexTwo;
                    }
                }
            }
        }
    }
    else if (UseNewGamepadAPI == false)
    {
        if (NumberOfJoyHats[joy] > 0)
        {
            JoystickHat[joy] = SDL_JoystickGetHat(JoystickDevices[joy], 0);

            JoystickHatDoingSomething[joy] = CENTER;

            if (JoystickHat[joy] == SDL_HAT_LEFT)
            {
                JoystickHatDoingSomething[joy] = LEFT;
                returnValue = Hat0;
            }
            else if (JoystickHat[joy] == SDL_HAT_RIGHT)
            {
                JoystickHatDoingSomething[joy] = RIGHT;
                returnValue = Hat0;
            }
            else if (JoystickHat[joy] == SDL_HAT_UP)
            {
                JoystickHatDoingSomething[joy] = UP;
                returnValue = Hat0;
            }
            else if (JoystickHat[joy] == SDL_HAT_DOWN)
            {
                JoystickHatDoingSomething[joy] = DOWN;
                returnValue = Hat0;
            }
        }

        for (int index = 0; index < NumberOfJoyButtons[joy]; index++)
        {
            if ( SDL_JoystickGetButton(JoystickDevices[joy], index) )
            {
                returnValue = Button0+index;
            }
        }

        if (JoystickDisabled[joy] != true)
        {
            if (NumberOfJoyAxises[joy] > 0)
            {
                for (int index = 0; index < NumberOfJoyAxises[joy]; index++)
                {
                    Sint16 joyAxis = 0;
                    joyAxis = SDL_JoystickGetAxis(JoystickDevices[joy], index);

                    if ( joyAxis < (-32768/2) || joyAxis > (32767/2) )
                    {
                        returnValue = Axis0+index;
                    }
                }
            }
        }
    }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Input::GetAllUserInput(void)
{
    SDL_PumpEvents();

    KeyOnKeyboardPressedByUser = -1;

    MouseButtonPressed[0] = false;
    MouseButtonPressed[1] = false;
    MouseButtonPressed[2] = false;

    for (int index = 0; index < 5; index++)
    {
        JoystickDirectionHorizontal[index] = CENTER;
        JoystickDirectionVertical[index] = CENTER;
        JoystickButtonOne[index] = OFF;
        JoystickButtonTwo[index] = OFF;
        JoystickPause[index] = OFF;
    }

    JoystickDirectionHorizontal[Any] = CENTER;
    JoystickDirectionVertical[Any] = CENTER;
    JoystickButtonOne[Any] = OFF;
    JoystickButtonTwo[Any] = OFF;
    JoystickPause[Any] = OFF;

    for (int index = 0; index < 4; index++)  JoystickHat[0] = -1;

    if (DelayAllUserInput > 0)
    {
        DelayAllUserInput--;
        return;
    }
    else  DelayAllUserInput = 0;

    KeyModState = SDL_GetModState();
    if ( (KeyModState & KMOD_LSHIFT) || (KeyModState & KMOD_RSHIFT) )  ShiftKeyPressed = true;
    else  ShiftKeyPressed = false;

    while ( SDL_PollEvent(&Event) )
    {
        switch (Event.type)
        {
            case SDL_QUIT:
                EXIT_Game = true;
                screens->ScreenTransitionStatus = FadeOut;
                return;
                break;

            case SDL_WINDOWEVENT:
                if (Event.window.event == SDL_WINDOWEVENT_RESIZED || Event.window.event == SDL_WINDOWEVENT_SHOWN || Event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                {
                    screens->ScreenIsDirty = 2;
                    LastEventWasWindowResize = 2;
                }
                break;

            case SDL_KEYDOWN:
                KeyOnKeyboardPressedByUser = SDL_GetKeyFromScancode(Event.key.keysym.scancode);

                if (KeyOnKeyboardPressedByUser == SDLK_LSHIFT || KeyOnKeyboardPressedByUser == SDLK_RSHIFT)
                    KeyOnKeyboardPressedByUser = -1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                MouseButtonClicked = Event.button.clicks;

                MouseX = Event.button.x;
                MouseY = Event.button.y;

                break;

            case SDL_MOUSEMOTION:
                if (screens->ScreenToDisplay == PlayingGameScreen || screens->ScreenToDisplay == PlayingStoryGameScreen)
                {
                    MouseX = Event.button.x;
                    MouseY = Event.button.y;
                }
                break;


            default:
                break;
        }
    }

    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if (screens->ScreenToDisplay != NameInputKeyboardScreen)
    {
        if (UserDefinedKeyButtonOne == -1 && UserDefinedKeyButtonTwo == -1)
        {
            if      (keyboardState[   SDL_SCANCODE_UP])  JoystickDirectionVertical[Keyboard] = UP;
            else if (keyboardState[ SDL_SCANCODE_DOWN])  JoystickDirectionVertical[Keyboard] = DOWN;

            if      (keyboardState[ SDL_SCANCODE_LEFT])  JoystickDirectionHorizontal[Keyboard] = LEFT;
            else if (keyboardState[SDL_SCANCODE_RIGHT])  JoystickDirectionHorizontal[Keyboard] = RIGHT;

            if      (keyboardState[    SDL_SCANCODE_Z] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
            else if (keyboardState[    SDL_SCANCODE_X] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;
        }
        else
        {
            if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyButtonOne) ] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonOne[Keyboard] = ON;
            else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyButtonTwo) ] && screens->ScreenToDisplay != NameInputKeyboardScreen)  JoystickButtonTwo[Keyboard] = ON;

            if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyUP) ])  JoystickDirectionVertical[Keyboard] = UP;
            else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyDOWN) ])  JoystickDirectionVertical[Keyboard] = DOWN;

            if      (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyLEFT) ])  JoystickDirectionHorizontal[Keyboard] = LEFT;
            else if (keyboardState[ SDL_GetScancodeFromKey(UserDefinedKeyRIGHT) ])  JoystickDirectionHorizontal[Keyboard] = RIGHT;
        }
    }

    if (KeyOnKeyboardPressedByUser == SDLK_ESCAPE)
    {
        screens->ScreenTransitionStatus = FadeOut;

        UserPressedESC = 1;

        DelayAllUserInput = 30;

        logic->GameForfeit = true;

        logic->PlayerData[0].Score = 0;
        logic->PlayerData[1].Score = 0;
        logic->PlayerData[2].Score = 0;
        logic->PlayerData[3].Score = 0;

        if (screens->ScreenToDisplay == PlayingGameScreen || screens->ScreenToDisplay == PlayingStoryGameScreen)
        {
            if (logic->HumanStillAlive == true)
            {
                for (int index = 0; index < 4; index++)

                if (logic->PlayerData[index].PlayerInput != CPU)
                {
                    if (logic->PlayerData[index].PlayerStatus != GameOver)
                    {
                        logic->PlayerData[index].Score = 0;
                        logic->PlayerData[index].PlayerStatus = GameOver;
                    }
                }
            }
        }
    }

//------------------------------------------------------------------------

    for (int index = 0; index < 3; index++)
    {
        if ( SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(index+1) )
        {
            if (MouseButtonWasClicked[index] == false)
            {
                MouseButtonPressed[index] = true;
                MouseButtonWasClicked[index] = true;
            }
        }
        else  MouseButtonWasClicked[index] = false;
    }

//------------------------------------------------------------------------

    if (UseNewGamepadAPI == true)
    {
        if (JoystickSetupProcess == JoySetupNotStarted)
        {
            for (int index = 0; index < 4; index++)
            {
                if (JoystickDisabled[index] == false)
                {
                    for ( int indexTwo = SDL_CONTROLLER_BUTTON_DPAD_UP; indexTwo < (SDL_CONTROLLER_BUTTON_DPAD_RIGHT+1); indexTwo++ )
                    {
                        if (  GamepadUP[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickDirectionVertical[JoystickOne+index] = UP;
                        }
                        else if (  GamepadDOWN[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickDirectionVertical[JoystickOne+index] = DOWN;
                        }
                        else if (  GamepadLEFT[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                        }
                        else if (  GamepadRIGHT[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
                        }
                     }

                    for ( int indexTwo = SDL_CONTROLLER_AXIS_LEFTX; indexTwo < (SDL_CONTROLLER_AXIS_RIGHTY+1); indexTwo++ )
                    {
                        if (  GamepadUP[index] == indexTwo
                        && SDL_GameControllerGetAxis( GamepadController[index], (SDL_GameControllerAxis)indexTwo ) < (-32768/1.5)  )
                        {
                            JoystickDirectionVertical[JoystickOne+index] = UP;
                        }
                        else if (  GamepadDOWN[index] == indexTwo
                        && SDL_GameControllerGetAxis( GamepadController[index], (SDL_GameControllerAxis)indexTwo ) > (32768/1.5)  )
                        {
                            JoystickDirectionVertical[JoystickOne+index] = DOWN;
                        }
                        else if (  GamepadLEFT[index] == indexTwo
                        && SDL_GameControllerGetAxis( GamepadController[index], (SDL_GameControllerAxis)indexTwo ) < (-32768/1.5)  )
                        {
                            JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                        }
                        else if (  GamepadRIGHT[index] == indexTwo
                        && SDL_GameControllerGetAxis( GamepadController[index], (SDL_GameControllerAxis)indexTwo ) > (32768/1.5)  )
                        {
                            JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
                        }
                    }

                    for ( int indexTwo = SDL_CONTROLLER_BUTTON_A; indexTwo < (SDL_CONTROLLER_BUTTON_RIGHTSHOULDER+1); indexTwo++ )
                    {
                        if (  GamepadButton1[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickButtonOne[JoystickOne+index] = ON;
                        }
                        else if (  GamepadButton2[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickButtonTwo[JoystickOne+index] = ON;
                        }
                        else if (  GamepadPause[index] == indexTwo
                        && SDL_GameControllerGetButton( GamepadController[index], (SDL_GameControllerButton)indexTwo ) == 1  )
                        {
                            JoystickPause[JoystickOne+index] = ON;
                        }
                    }
                }
            }
        }
    }
    else if (UseNewGamepadAPI == false)
    {
        if (JoystickSetupProcess == JoySetupNotStarted)
        {
            for (int index = 0; index < 4; index++)
            {
                if (JoystickDisabled[index] == false)
                {
                    JoystickHat[index] = SDL_JoystickGetHat(JoystickDevices[index], 0);
                    if (JoyUP[index] == Hat0 && JoystickHat[index] == SDL_HAT_UP)
                    {
                        JoystickDirectionVertical[JoystickOne+index] = UP;
                    }
                    else if (JoyDOWN[index] == Hat0 && JoystickHat[index] == SDL_HAT_DOWN)
                    {
                        JoystickDirectionVertical[JoystickOne+index] = DOWN;
                    }
                    if (JoyLEFT[index] == Hat0 && JoystickHat[index] == SDL_HAT_LEFT)
                    {
                        JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                    }
                    else if (JoyRIGHT[index] == Hat0 && JoystickHat[index] == SDL_HAT_RIGHT)
                    {
                        JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
                    }

                    Sint16 joystickXmovement = 0;
                    if (JoyLEFT[index] > Hat0)
                    {
                        if ( SDL_JoystickGetButton(JoystickDevices[index], JoyLEFT[index]-9) )  JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                        else  if ( SDL_JoystickGetButton(JoystickDevices[index], JoyRIGHT[index]-9) )  JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
                    }
                    else joystickXmovement = SDL_JoystickGetAxis(JoystickDevices[index], JoyLEFT[index]);

                    Sint16 joystickYmovement = 0;
                    if (JoyUP[index] > Hat0)
                    {
                        if ( SDL_JoystickGetButton(JoystickDevices[index], JoyUP[index]-9) )  JoystickDirectionVertical[JoystickOne+index] = UP;
                        else  if ( SDL_JoystickGetButton(JoystickDevices[index], JoyDOWN[index]-9) )  JoystickDirectionVertical[JoystickOne+index] = DOWN;
                    }
                    else joystickYmovement = SDL_JoystickGetAxis(JoystickDevices[index], JoyUP[index]);

                    if ( joystickYmovement < (-32768/1.5) )
                    {
                        JoystickDirectionVertical[JoystickOne+index] = UP;
                    }
                    else if ( joystickYmovement > (32767/1.5) )
                    {
                        JoystickDirectionVertical[JoystickOne+index] = DOWN;
                    }

                    if ( joystickXmovement < (-32768/1.5) )
                    {
                        JoystickDirectionHorizontal[JoystickOne+index] = LEFT;
                    }
                    else if ( joystickXmovement > (32768/1.5) )
                    {
                        JoystickDirectionHorizontal[JoystickOne+index] = RIGHT;
                    }

                    if (JoyButton1[index] > Hat0)
                    {
                        if ( SDL_JoystickGetButton(JoystickDevices[index], JoyButton1[index]-9) )  JoystickButtonOne[JoystickOne+index] = ON;
                    }
                    else
                    {
                        Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDevices[index], JoyButton1[index]);
                        if (padAsButton < (-32768/2))  JoystickButtonOne[JoystickOne+index] = ON;
                    }

                    if (JoyButton2[index] > Hat0)
                    {
                        if ( SDL_JoystickGetButton(JoystickDevices[index], JoyButton2[index]-9) )  JoystickButtonTwo[JoystickOne+index] = ON;
                    }
                    else
                    {
                        Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDevices[index], JoyButton2[index]);
                        if (padAsButton < (-32768/1.5))  JoystickButtonTwo[JoystickOne+index] = ON;
                    }

                    if (JoyPause[index] > Hat0)
                    {
                        if ( SDL_JoystickGetButton(JoystickDevices[index], JoyPause[index]-9) )  JoystickPause[JoystickOne+index] = ON;
                    }
                    else
                    {
                        Sint16 padAsButton = SDL_JoystickGetAxis(JoystickDevices[index], JoyPause[index]);
                        if (padAsButton < (-32768/1.5))  JoystickPause[JoystickOne+index] = ON;
                    }
                }
            }
        }
    }
//------------------------------------------------------------------------

    for (int index = 0; index < 5; index++)
    {
        if (JoystickDirectionHorizontal[Any] == CENTER)  JoystickDirectionHorizontal[Any] = JoystickDirectionHorizontal[index];
        if (JoystickDirectionVertical[Any] == CENTER)  JoystickDirectionVertical[Any] = JoystickDirectionVertical[index];
        if (JoystickButtonOne[Any] == OFF && JoystickButtonOnePressed[index] == false)  JoystickButtonOne[Any] = JoystickButtonOne[index];
        if (JoystickButtonTwo[Any] == OFF)  JoystickButtonTwo[Any] = JoystickButtonTwo[index];
        if (JoystickPause[Any] == OFF)  JoystickPause[Any] = JoystickPause[index];
    }

    if (screens->ScreenToDisplay != NameInputJoystickScreen)
    {
        for (int index = 0; index < 5; index++)
        {
            if (JoystickButtonOne[index] == ON)  JoystickButtonOnePressed[index] = true;
            else  JoystickButtonOnePressed[index] = false;
        }
    }
//------------------------------------------------------------------------
}
