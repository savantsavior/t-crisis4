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

#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:

	Interface(void);
	virtual ~Interface(void);

    #define NumberOfButtons     8
    struct ButtonGUI
    {
        Uint16 SpriteIndex;
        Sint8 ScreenIndex;
        float Scale;
        float ScreenY;
        Sint16 AnimationTimer;
    } ButtonGUIs[NumberOfButtons];

    int NumberOfButtonsOnScreen;

    Uint8 ButtonSelectedByKeyboard;
    Sint8 ButtonSelectedByPlayer;

    #define NumberOfArrowSets     10
    struct ArrowSetGUI
    {
        float LeftArrowScreenIndex;
        float LeftArrowScale;
        float RightArrowScale;
        float ScreenY;
        Sint16 AnimationTimer;
    } ArrowSetGUIs[NumberOfArrowSets];

    int NumberOfArrowSetsOnScreen;

    Uint8 ArrowSetSelectedByKeyboard;
    float ArrowSetArrowSelectedByPlayer;

    #define NumberOfIcons     100
    struct IconGUI
    {
        Sint16 SpriteIndex;
        Sint8 ScreenIndex;
        float ScreenX;
        float ScreenY;
        Sint16 AnimationTimer;
        float Scale;
    } IconGUIs[NumberOfIcons];

    int NumberOfIconsOnScreen;
    Sint8 IconSelectedByPlayer;

    void CreateButton(Uint16 SpriteIndex, Uint8 ScreenIndex, float ScreenY);
    void DisplayAllButtonsOntoScreenBuffer(void);
    void ProcessAllButtons(void);
    void DestroyAllButtons(void);

    void CreateArrowSet(float ScreenIndex, float ScreenY);
    void DisplayAllArrowSetsOntoScreenBuffer(void);
    void ProcessAllArrowSets(void);
    void DestroyAllArrowSets(void);

    void CreateIcon(Sint16 spriteIndex, float iconScreenX, float iconScreenY);
    void DisplayAllIconsOntoScreenBuffer(void);
    void ProcessAllIcons(void);
    void DestroyAllIcons(void);
};

#endif

