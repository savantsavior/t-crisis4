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

#include "interface.h"

#include "input.h"
#include "visuals.h"
#include "screens.h"
#include "audio.h"
#include "logic.h"

extern Input* input;
extern Visuals* visuals;
extern Screens* screens;
extern Audio* audio;
extern Logic* logic;

//-------------------------------------------------------------------------------------------------
Interface::Interface(void)
{
    for (int index = 0; index < NumberOfButtons; index++)
    {
        ButtonGUIs[index].SpriteIndex = -1;
        ButtonGUIs[index].ScreenIndex = -1;
        ButtonGUIs[index].Scale = 1;
        ButtonGUIs[index].ScreenY = -100;
        ButtonGUIs[index].AnimationTimer = -1;
    }

    NumberOfButtonsOnScreen = 0;

    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        ArrowSetGUIs[index].LeftArrowScreenIndex = -1;
        ArrowSetGUIs[index].LeftArrowScale = 1;
        ArrowSetGUIs[index].RightArrowScale = 1;
        ArrowSetGUIs[index].ScreenY = -100;
        ArrowSetGUIs[index].AnimationTimer = -1;
    }

    NumberOfArrowSetsOnScreen = 0;

    for (int index = 0; index < NumberOfIcons; index++)
    {
        IconGUIs[index].SpriteIndex = -1;
        IconGUIs[index].ScreenIndex = -1;
        IconGUIs[index].ScreenX = -999;
        IconGUIs[index].ScreenY = -999;
        IconGUIs[index].AnimationTimer = -1;
        IconGUIs[index].Scale = 1;
    }
}

//-------------------------------------------------------------------------------------------------
Interface::~Interface(void)
{

}

//-------------------------------------------------------------------------------------------------
void Interface::CreateButton(Uint16 SpriteIndex, Uint8 ScreenIndex, float ScreenY)
{
    ButtonSelectedByKeyboard = 0;
    ButtonSelectedByPlayer = -1;

    for (int index = 0; index < NumberOfButtons; index++)
    {
        if (ButtonGUIs[index].ScreenIndex == -1)
        {
            ButtonGUIs[index].SpriteIndex = SpriteIndex;
            ButtonGUIs[index].ScreenIndex = ScreenIndex;
            ButtonGUIs[index].Scale = 1;
            ButtonGUIs[index].ScreenY = ScreenY;
            ButtonGUIs[index].AnimationTimer = -1;

            index = NumberOfButtons;
        }
    }

    NumberOfButtonsOnScreen++;
}

//-------------------------------------------------------------------------------------------------
void Interface::DisplayAllButtonsOntoScreenBuffer(void)
{
    for (int index = 0; index < NumberOfButtons; index++)
    {
        if (ButtonGUIs[index].ScreenIndex != -1)
        {
            visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].ScreenX = 320.0f;
            visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].ScreenY = ButtonGUIs[index].ScreenY;
            visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].ScaleX = ButtonGUIs[index].Scale;
            visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].ScaleY = ButtonGUIs[index].Scale;
            visuals->DrawSpriteOntoScreenBuffer(ButtonGUIs[index].SpriteIndex);

            if (ButtonSelectedByKeyboard == ButtonGUIs[index].ScreenIndex && ButtonGUIs[1].ScreenIndex != -1)
            {
                visuals->Sprites[1000].ScreenX = 320-125;
                visuals->Sprites[1000].ScreenY = ButtonGUIs[index].ScreenY;
                visuals->Sprites[1000].ScaleX = 1;
                visuals->Sprites[1000].ScaleY = 1;
                visuals->DrawSpriteOntoScreenBuffer(1000);

                visuals->Sprites[1001].ScreenX = 320+125;
                visuals->Sprites[1001].ScreenY = ButtonGUIs[index].ScreenY;
                visuals->Sprites[1001].ScaleX = 1;
                visuals->Sprites[1001].ScaleY = 1;
                visuals->DrawSpriteOntoScreenBuffer(1001);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Interface::ProcessAllButtons(void)
{
    bool thereIsButton = false;
    for (int index = 0; index < NumberOfButtons; index++)
    {
        if (ButtonGUIs[index].ScreenIndex != -1)
        {
            thereIsButton = true;
        }
    }

    if (thereIsButton == false)  return;

    if (input->DelayAllUserInput == 0)
    {
        Sint8 MinButtonIndex = 0;
        Sint8 MaxButtonIndex = -1;
        for (int index = 0; index < NumberOfButtons; index++)
        {
            if (ButtonGUIs[index].ScreenIndex > MaxButtonIndex)
                MaxButtonIndex = ButtonGUIs[index].ScreenIndex;
        }

        if (input->JoystickDirectionVertical[Any] == UP && NumberOfButtonsOnScreen > 1)
        {
            if (ButtonSelectedByKeyboard > MinButtonIndex)  ButtonSelectedByKeyboard--;
            else  ButtonSelectedByKeyboard = MaxButtonIndex;

            screens->ScreenIsDirty = 2;
            input->DelayAllUserInput = 10;
            audio->PlayDigitalSoundFX(0, 0);
        }
        else if (input->JoystickDirectionVertical[Any] == DOWN && NumberOfButtonsOnScreen > 1)
        {
            if (ButtonSelectedByKeyboard < MaxButtonIndex)  ButtonSelectedByKeyboard++;
            else  ButtonSelectedByKeyboard = MinButtonIndex;

            screens->ScreenIsDirty = 2;
            input->DelayAllUserInput = 10;
            audio->PlayDigitalSoundFX(0, 0);
        }
        else if (  (input->JoystickButtonOne[Any] == ON) ||
                ( (input->KeyOnKeyboardPressedByUser == SDLK_RETURN)
                && screens->ScreenToDisplay != NameInputJoystickScreen )  )
        {
            if (input->KeyOnKeyboardPressedByUser == SDLK_SPACE && screens->ScreenToDisplay == NameInputKeyboardScreen)
            {

            }
            else if (input->JoystickButtonOne[Any] == ON && screens->ScreenToDisplay == NameInputJoystickScreen)
            {

            }
            else
            {
                ButtonGUIs[ButtonSelectedByKeyboard].Scale = 0.9f;
                ButtonGUIs[ButtonSelectedByKeyboard].AnimationTimer = 10;
                screens->ScreenIsDirty = 2;
                input->DelayAllUserInput = 50;

                logic->PlayerData[1].PlayerInput = Keyboard;

                if (input->JoystickButtonOne[JoystickOne] == ON)
                {
                    logic->PlayerData[1].PlayerInput = JoystickOne;
                }
                else if (input->JoystickButtonOne[JoystickTwo] == ON)
                {
                    logic->PlayerData[1].PlayerInput = JoystickTwo;
                }
                else if (input->JoystickButtonOne[JoystickThree] == ON)
                {
                    logic->PlayerData[1].PlayerInput = JoystickThree;
                }
                else if (input->JoystickButtonOne[JoystickFour] == ON)
                {
                    logic->PlayerData[1].PlayerInput = JoystickFour;
                }

                audio->PlayDigitalSoundFX(1, 0);
            }
        }

        if (input->MouseButtonPressed[0] == true)
        {
            for (int index = 0; index < NumberOfButtons; index++)
            {
                if (ButtonGUIs[index].ScreenIndex != -1)
                {
                    if (   (  input->MouseY > ( ButtonGUIs[index].ScreenY - (visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].TextureHeightOriginal / 2) )  )
                       && (  input->MouseY < ( ButtonGUIs[index].ScreenY + (visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].TextureHeightOriginal / 2) )  )
                       && (  input->MouseX > (  320 - (visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].TextureWidthOriginal / 2) )  )
                       && (  input->MouseX < ( 320 + (visuals->Sprites[ ButtonGUIs[index].SpriteIndex ].TextureWidthOriginal / 2) )  )   )
                    {
                        ButtonSelectedByKeyboard = ButtonGUIs[index].ScreenIndex;
                        ButtonGUIs[index].Scale = 0.9f;
                        ButtonGUIs[index].AnimationTimer = 10;
                        screens->ScreenIsDirty = 2;
                        input->DelayAllUserInput = 20;

                        logic->PlayerData[1].PlayerInput = Keyboard;

                        audio->PlayDigitalSoundFX(1, 0);

                    }
                }
            }
        }
    }

    for (int index = 0; index < NumberOfButtons; index++)
    {
        if (ButtonGUIs[index].ScreenIndex != -1)
        {
            if (ButtonGUIs[index].AnimationTimer > -1)
            {
                ButtonGUIs[index].AnimationTimer--;

                if (ButtonGUIs[index].AnimationTimer == -1)
                {
                    ButtonSelectedByPlayer = ButtonSelectedByKeyboard;
                    screens->ScreenTransitionStatus = FadeOut;
                    ButtonGUIs[index].Scale = 1;
                    screens->ScreenIsDirty = 2;
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Interface::DestroyAllButtons(void)
{
    for (int index = 0; index < NumberOfButtons; index++)
    {
        ButtonGUIs[index].SpriteIndex = 0;
        ButtonGUIs[index].ScreenIndex = -1;
        ButtonGUIs[index].Scale = 1;
        ButtonGUIs[index].ScreenY = -100;
        ButtonGUIs[index].AnimationTimer = -1;
    }

    NumberOfButtonsOnScreen = 0;
}

//-------------------------------------------------------------------------------------------------
void Interface::CreateArrowSet(float ScreenIndex, float ScreenY)
{
    ArrowSetSelectedByKeyboard = 0;
    ArrowSetArrowSelectedByPlayer = -1;

    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        if (ArrowSetGUIs[index].LeftArrowScreenIndex == -1)
        {
            ArrowSetGUIs[index].LeftArrowScreenIndex = ScreenIndex;
            ArrowSetGUIs[index].LeftArrowScale = 1;
            ArrowSetGUIs[index].RightArrowScale = 1;
            ArrowSetGUIs[index].ScreenY = ScreenY;
            ArrowSetGUIs[index].AnimationTimer = -1;

            index = NumberOfArrowSets;
        }
    }

    NumberOfArrowSetsOnScreen++;
}

//-------------------------------------------------------------------------------------------------
void Interface::DisplayAllArrowSetsOntoScreenBuffer(void)
{
    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        if (ArrowSetGUIs[index].LeftArrowScreenIndex != -1)
        {
            if (ArrowSetSelectedByKeyboard == ArrowSetGUIs[index].LeftArrowScreenIndex)
            {
                visuals->Sprites[1002].ScreenX = 320;
                visuals->Sprites[1002].ScreenY = ArrowSetGUIs[index].ScreenY;
                visuals->Sprites[1002].Smooth = true;
                visuals->Sprites[1002].BlueHue = 0;
                visuals->Sprites[1002].Transparency = 120;
                visuals->DrawSpriteOntoScreenBuffer(1002);
            }

            visuals->Sprites[1001].ScreenX = 320-295;
            visuals->Sprites[1001].ScreenY = ArrowSetGUIs[index].ScreenY;
            visuals->Sprites[1001].ScaleX = ArrowSetGUIs[index].LeftArrowScale;
            visuals->Sprites[1001].ScaleY = ArrowSetGUIs[index].LeftArrowScale;
            visuals->DrawSpriteOntoScreenBuffer(1001);

            visuals->Sprites[1000].ScreenX = 320+295;
            visuals->Sprites[1000].ScreenY = ArrowSetGUIs[index].ScreenY;
            visuals->Sprites[1000].ScaleX = ArrowSetGUIs[index].RightArrowScale;
            visuals->Sprites[1000].ScaleY = ArrowSetGUIs[index].RightArrowScale;
            visuals->DrawSpriteOntoScreenBuffer(1000);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Interface::ProcessAllArrowSets(void)
{
    bool thereIsArrowSet = false;
    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        if (ArrowSetGUIs[index].LeftArrowScreenIndex != -1)
        {
            thereIsArrowSet = true;
        }
    }

    if (thereIsArrowSet == false)  return;

    float MinArrowSetIndex = 0.0f;
    float MaxArrowSetIndex = -1.0f;
    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        if (ArrowSetGUIs[index].LeftArrowScreenIndex > MaxArrowSetIndex)
            MaxArrowSetIndex = ArrowSetGUIs[index].LeftArrowScreenIndex;
    }

    if (input->JoystickDirectionVertical[Any] == UP && NumberOfArrowSetsOnScreen > 1)
    {
        if (ArrowSetSelectedByKeyboard > MinArrowSetIndex)  ArrowSetSelectedByKeyboard--;
        else  ArrowSetSelectedByKeyboard = (Uint8)MaxArrowSetIndex;

        screens->ScreenIsDirty = 2;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }
    else if (input->JoystickDirectionVertical[Any] == DOWN && NumberOfArrowSetsOnScreen > 1)
    {
        if (ArrowSetSelectedByKeyboard < MaxArrowSetIndex)  ArrowSetSelectedByKeyboard++;
        else  ArrowSetSelectedByKeyboard = (Uint8)MinArrowSetIndex;

        screens->ScreenIsDirty = 2;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(0, 0);
    }
    else if (input->JoystickDirectionHorizontal[Any] == LEFT)
    {
        ArrowSetGUIs[ArrowSetSelectedByKeyboard].LeftArrowScale = 0.8f;
        ArrowSetGUIs[ArrowSetSelectedByKeyboard].AnimationTimer = 10;

        screens->ScreenIsDirty = 2;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }
    else if (input->JoystickDirectionHorizontal[Any] == RIGHT)
    {
        ArrowSetGUIs[ArrowSetSelectedByKeyboard].RightArrowScale = 0.8f;
        ArrowSetGUIs[ArrowSetSelectedByKeyboard].AnimationTimer = 10;

        screens->ScreenIsDirty = 2;
        input->DelayAllUserInput = 20;
        audio->PlayDigitalSoundFX(1, 0);
    }

    if (input->MouseButtonPressed[0] == true)
    {
        for (int index = 0; index < NumberOfArrowSets; index++)
        {
            if (ArrowSetGUIs[index].LeftArrowScreenIndex != -1)
            {
                if (  ( input->MouseY > (ArrowSetGUIs[index].ScreenY - 19) )
                   && ( input->MouseY < (ArrowSetGUIs[index].ScreenY + 19) )
                   && ( input->MouseX > (320-295-23) )
                   && ( input->MouseX < (320-295+23) )  )
                {
                    ArrowSetSelectedByKeyboard = (Uint8)ArrowSetGUIs[index].LeftArrowScreenIndex;
                    ArrowSetGUIs[index].LeftArrowScale = 0.8f;
                    ArrowSetGUIs[index].AnimationTimer = 10;
                    screens->ScreenIsDirty = 2;
                    input->DelayAllUserInput = 20;
                    audio->PlayDigitalSoundFX(1, 0);
                }
                else if (  ( input->MouseY > (ArrowSetGUIs[index].ScreenY - 21) )
                   && ( input->MouseY < (ArrowSetGUIs[index].ScreenY + 21) )
                   && ( input->MouseX > (320+295-25) )
                   && ( input->MouseX < (320+295+25) )  )
                {
                    ArrowSetSelectedByKeyboard = (Uint8)ArrowSetGUIs[index].LeftArrowScreenIndex;
                    ArrowSetGUIs[index].RightArrowScale = 0.8f;
                    ArrowSetGUIs[index].AnimationTimer = 10;
                    screens->ScreenIsDirty = 2;
                    input->DelayAllUserInput = 20;
                    audio->PlayDigitalSoundFX(1, 0);
                }
            }
        }
    }

    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        if (ArrowSetGUIs[index].LeftArrowScreenIndex != -1)
        {
            if (ArrowSetGUIs[index].AnimationTimer > 0)  ArrowSetGUIs[index].AnimationTimer--;
            else if (ArrowSetGUIs[index].AnimationTimer == 0)
            {
                if (ArrowSetGUIs[index].LeftArrowScale < 1)
                {
                    ArrowSetGUIs[index].LeftArrowScale = 1;
                    ArrowSetArrowSelectedByPlayer = (float)ArrowSetSelectedByKeyboard;
                }
                else if (ArrowSetGUIs[index].RightArrowScale < 1)
                {
                    ArrowSetGUIs[index].RightArrowScale = 1;
                    ArrowSetArrowSelectedByPlayer = (float)ArrowSetSelectedByKeyboard+0.5f;
                }

                ArrowSetGUIs[index].AnimationTimer = -1;
                screens->ScreenIsDirty = 2;
            }
        }
    }

}

//-------------------------------------------------------------------------------------------------
void Interface::DestroyAllArrowSets(void)
{
    for (int index = 0; index < NumberOfArrowSets; index++)
    {
        ArrowSetGUIs[index].LeftArrowScreenIndex = -1;
        ArrowSetGUIs[index].LeftArrowScale = 1;
        ArrowSetGUIs[index].RightArrowScale = 1;
        ArrowSetGUIs[index].ScreenY = -100;
        ArrowSetGUIs[index].AnimationTimer = -1;
    }

    NumberOfArrowSetsOnScreen = 0;
}

//-------------------------------------------------------------------------------------------------
void Interface::CreateIcon(Sint16 spriteIndex, float screenX, float screenY)
{
    IconGUIs[NumberOfIconsOnScreen].SpriteIndex = spriteIndex;
    IconGUIs[NumberOfIconsOnScreen].ScreenIndex = NumberOfIconsOnScreen;
    IconGUIs[NumberOfIconsOnScreen].ScreenX = screenX;
    IconGUIs[NumberOfIconsOnScreen].ScreenY = screenY;
    IconGUIs[NumberOfIconsOnScreen].AnimationTimer = -1;
    IconGUIs[NumberOfIconsOnScreen].Scale = 1;

    NumberOfIconsOnScreen++;

    IconSelectedByPlayer = -1;
}

//-------------------------------------------------------------------------------------------------
void Interface::DisplayAllIconsOntoScreenBuffer(void)
{
    for (int index = 0; index < NumberOfIcons; index++)
    {
        if (IconGUIs[index].ScreenIndex != -1)
        {
            visuals->Sprites[ IconGUIs[index].SpriteIndex ].ScreenX = IconGUIs[index].ScreenX;
            visuals->Sprites[ IconGUIs[index].SpriteIndex ].ScreenY = IconGUIs[index].ScreenY;
            visuals->Sprites[ IconGUIs[index].SpriteIndex ].ScaleX = IconGUIs[index].Scale;
            visuals->Sprites[ IconGUIs[index].SpriteIndex ].ScaleY = IconGUIs[index].Scale;
            visuals->DrawSpriteOntoScreenBuffer(IconGUIs[index].SpriteIndex);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Interface::ProcessAllIcons(void)
{
    IconSelectedByPlayer = -1;

    if (input->MouseButtonPressed[0] == true)
    {
        for (int index = 0; index < NumberOfIcons; index++)
        {
            if (IconGUIs[index].ScreenIndex != -1)
            {
                if (   (  input->MouseY > ( IconGUIs[index].ScreenY - (visuals->Sprites[ IconGUIs[index].SpriteIndex ].TextureHeightOriginal / 2) )  )
                   && (  input->MouseY < ( IconGUIs[index].ScreenY + (visuals->Sprites[ IconGUIs[index].SpriteIndex ].TextureHeightOriginal / 2) )  )
                   && (  input->MouseX > ( IconGUIs[index].ScreenX - (visuals->Sprites[ IconGUIs[index].SpriteIndex ].TextureWidthOriginal / 2) )  )
                   && (  input->MouseX < ( IconGUIs[index].ScreenX + (visuals->Sprites[ IconGUIs[index].SpriteIndex ].TextureWidthOriginal / 2) )  )   )
                {
                    IconGUIs[index].Scale = 0.9f;
                    IconGUIs[index].AnimationTimer = 10;
                    screens->ScreenIsDirty = 2;
                    input->DelayAllUserInput = 20;

                    audio->PlayDigitalSoundFX(1, 0);

                }
            }
        }
    }

    for (int index = 0; index < NumberOfIcons; index++)
    {
        if (IconGUIs[index].ScreenIndex != -1)
        {
            if (IconGUIs[index].AnimationTimer > -1)
            {
                IconGUIs[index].AnimationTimer--;

                if (IconGUIs[index].AnimationTimer == -1)
                {
                    IconSelectedByPlayer = IconGUIs[index].ScreenIndex;
                    IconGUIs[index].Scale = 1;
                    screens->ScreenIsDirty = 2;
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Interface::DestroyAllIcons(void)
{
    for (int index = 0; index < NumberOfIcons; index++)
    {
        IconGUIs[index].SpriteIndex = -1;
        IconGUIs[index].ScreenIndex = -1;
        IconGUIs[index].ScreenX = -999;
        IconGUIs[index].ScreenY = -999;
        IconGUIs[index].AnimationTimer = -1;
        IconGUIs[index].Scale = 1;
    }

    NumberOfIconsOnScreen = 0;

    IconSelectedByPlayer = -1;
}

//-------------------------------------------------------------------------------------------------
