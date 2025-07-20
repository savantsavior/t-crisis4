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

#ifndef INPUT
#define INPUT

class Input
{
public:

	Input(void);
	virtual ~Input(void);

    bool UseNewGamepadAPI = true;

    SDL_GameController *GamepadController[4];
    SDL_Joystick *Gamepad[4];

    int DEBUG;

    bool EXIT_Game;

    int UserPressedESC = -1;

    SDL_Event Event;

    int LastEventWasWindowResize = 0;

    SDL_Keycode KeyOnKeyboardPressedByUser;

    int DelayAllUserInput;

    bool ShiftKeyPressed;

    SDL_Keymod KeyModState;

    bool MouseButtonPressed[3];
    bool MouseButtonWasClicked[3];
    int MouseX, MouseY;
    int MouseButtonClicked;

    #define Keyboard        0
    #define JoystickOne     1
    #define JoystickTwo     2
    #define JoystickThree   3
    #define JoystickFour    4
    #define Mouse           5
    #define Any             6
    #define CPU             7

    #define CENTER      0
    #define UP          1
    #define RIGHT       3
    #define DOWN        5
    #define LEFT        7
    Uint8 JoystickDirectionHorizontal[7];
    Uint8 JoystickDirectionVertical[7];

    #define OFF         0
    #define ON          1
    Uint8 JoystickButtonOne[7];
    Uint8 JoystickButtonTwo[7];
    bool JoystickButtonOnePressed[7];
    bool JoystickButtonTwoPressed[7];
    Uint8 JoystickPause[7];

    #define JoySetupNotStarted          0

    #define Joy1SetupPressUP            1
    #define Joy1SetupPressDOWN          2
    #define Joy1SetupPressLEFT          3
    #define Joy1SetupPressRIGHT         4
    #define Joy1SetupPressBUTTONOne     5
    #define Joy1SetupPressBUTTONTwo     6
    #define Joy1SetupPressPause         7

    #define Joy2SetupPressUP            8
    #define Joy2SetupPressDOWN          9
    #define Joy2SetupPressLEFT          10
    #define Joy2SetupPressRIGHT         11
    #define Joy2SetupPressBUTTONOne     12
    #define Joy2SetupPressBUTTONTwo     13
    #define Joy2SetupPressPause         14

    #define Joy3SetupPressUP            15
    #define Joy3SetupPressDOWN          16
    #define Joy3SetupPressLEFT          17
    #define Joy3SetupPressRIGHT         18
    #define Joy3SetupPressBUTTONOne     19
    #define Joy3SetupPressBUTTONTwo     20
    #define Joy3SetupPressPause         21

    #define Joy4SetupPressUP            22
    #define Joy4SetupPressDOWN          23
    #define Joy4SetupPressLEFT          24
    #define Joy4SetupPressRIGHT         25
    #define Joy4SetupPressBUTTONOne     26
    #define Joy4SetupPressBUTTONTwo     27
    #define Joy4SetupPressPause         28

    int JoystickSetupProcess;

    #define KeyboardSetupNotStarted     0
    #define KeyboardSetupPressOne       1
    #define KeyboardSetupPressTwo       2
    #define KeyboardSetupPressUP        3
    #define KeyboardSetupPressRIGHT     4
    #define KeyboardSetupPressDOWN      5
    #define KeyboardSetupPressLEFT      6
    #define KeyboardSetupPressPause     7
    int KeyboardSetupProcess;

    int joystickToCheck = 0;
    int joystickControl = 0;

    bool SteamDeckSetupGamePad = false;

    int UserDefinedKeyButtonOne;
    int UserDefinedKeyButtonTwo;
    int UserDefinedKeyUP;
    int UserDefinedKeyRIGHT;
    int UserDefinedKeyDOWN;
    int UserDefinedKeyLEFT;
    int UserDefinedKeyPause;

    int JoyUP[5];
    int JoyDOWN[5];
    int JoyLEFT[5];
    int JoyRIGHT[5];
    int JoyButton1[5];
    int JoyButton2[5];
    int JoyPause[5];

    Uint8 JoystickHat[4];

    int JoystickHatDoingSomething[4];

    #define JoyNotPressed   -1

    #define Axis0           0
    #define Axis1           1
    #define Axis2           2
    #define Axis3           3
    #define Axis4           4
    #define Axis5           5
    #define Axis6           6
    #define Axis7           7

    #define Hat0            8

    #define Button0         9
    #define Button1         10
    #define Button2         11
    #define Button3         12
    #define Button4         13
    #define Button5         14
    #define Button6         15
    #define Button7         16
    #define Button8         17
    #define Button9         18
    #define Button10        19
    #define Button11        20
    #define Button12        21
    #define Button13        22
    #define Button14        23
    #define Button15        24
    #define Button16        25

    SDL_Joystick *JoystickDevices[4];

    int NumberOfJoyButtons[4];
    int NumberOfJoyHats[4];
    int NumberOfJoyAxises[4];

    bool JoystickDisabled[4];

    char JoystickName[4][100];

    int GamepadUP[5];
    int GamepadDOWN[5];
    int GamepadLEFT[5];
    int GamepadRIGHT[5];
    int GamepadButton1[5];
    int GamepadButton2[5];
    int GamepadPause[5];

    bool GetOnlyGamepadButtons;

    int QueryJoysticksForAction(Uint8 joy);

    void GetAllUserInput(void);

};

#endif
