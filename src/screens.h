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

#ifndef SCREENS
#define SCREENS

class Screens
{
public:

	Screens(void);
	virtual ~Screens(void);

    int joyAction = -1;

    Uint8 ScreenFadeTransparency;
    #define FadeIn          0
    #define FadeNone        1
    #define FadeOut         2
    #define FadeAll         3
    Uint8 ScreenTransitionStatus;
    Uint32 ScreenDisplayTimer;

    #define SDLLogoScreen               1
    #define SixteenBitSoftScreen        2
    #define TitleScreen                 3
    #define OptionsScreen               4
    #define HowToPlayScreen             5
    #define HighScoresScreen            6
    #define AboutScreen                 7
    #define NewGameOptionsScreen        8
    #define PlayingGameScreen           9
    #define PlayingStoryGameScreen      10
    #define ShowStoryScreen             11
    #define FlyingFromEarthScreen       12
    #define FlyingToMarsScreen          13
    #define FlyingToBaseScreen          14
    #define MarsExplodingScreen         15
    #define TestComputerSkillScreen     16
    #define NameInputKeyboardScreen     17
    #define NameInputJoystickScreen     18
    #define NameInputMouseScreen        19
    #define GiveMeJobScreen             20
    #define CNETScreen                  21
    int ScreenToDisplay;
    int ScreenIsDirty;

    int ImageToDisplay = 0;

    float EscKeyScale = 1.0f;
    int EscKeyScaleTimer = 0;

    int HighScoreNameMaxIndex = 12;

    bool SkipEntireStoryCutsceneAndPlay = false;

    void ApplyScreenFadeTransition(void);

    void ProcessScreenToDisplay(void);

    void DisplaySDLLogoScreen(void);

    void DisplaySixteenBitSoftScreen(void);

    void DisplayTitleScreen(void);

    void DisplayNewGameOptionsScreen(void);

    int JoystickFlash;
    int joySetup;
    void DisplayOptionsScreen(void);

    void DisplayHowToPlayScreen(void);

    void DisplayHighScoresScreen(void);

    int ReviewShowDelay;
    float ReviewScale;
    void DisplayAboutScreen(void);

    void DisplayPlayingGameScreen(void);

    void DisplayShowStoryScreen(void);

    int IntroAnimationStep;
    float PlanetX;
    float PlanetY;
    float PlanetScale;
    float ShipX;
    float ShipY;
    float ShipScale;
    float ExplosionScale;
    int ExplosionTransparency;
    void DisplayFlyingFromEarthScreen(void);
    void DisplayFlyingToMarsScreen(void);
    void DisplayFlyingToBaseScreen(void);
    void DisplayMarsExplodingScreen(void);

    void DisplayPlayingStoryGameScreen(void);

    void DisplayNameInputKeyboardScreen(void);
    void DisplayNameInputMouseScreen(void);
    void DisplayNameInputJoystickScreen(void);

    void DisplayTestComputerSkillScreen(void);

    void DisplayGiveMeJobScreen(void);

    void DisplayCNETScreen(void);
};

#endif
