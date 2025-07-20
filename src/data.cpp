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

using namespace std;
#include <fstream>
#include <string.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "data.h"

#include "audio.h"
#include "input.h"
#include "logic.h"
#include "visuals.h"
#include "screens.h"

extern Audio* audio;
extern Input* input;
extern Logic* logic;
extern Visuals* visuals;
extern Screens* screens;

//-------------------------------------------------------------------------------------------------
Data::Data(void)
{
	SDL_strlcpy(DataVersionName, "TC4-110-CNET-50", sizeof DataVersionName);

    ClearHighScores();

    PlayerWithHighestScore = 255;
    PlayerRankOnGameOver = 10;
    NameInputJoyCharX = 0;
    NameInputJoyCharY = 0;
    NameInputJoyChar = (char)NULL;
    NewHighScoreRegistered = false;
}

//-------------------------------------------------------------------------------------------------
Data::~Data(void)
{

}

//-------------------------------------------------------------------------------------------------
void Data::CheckForNewHighScore(void)
{
int playerRank = 10;

    for (int index = 0; index < 4; index++)
        if (logic->PlayerData[index].PlayerInput == CPU)  logic->PlayerData[index].Score = 0;

    if (logic->PlayerData[0].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[0].Score >= logic->PlayerData[2].Score
        && logic->PlayerData[0].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 0;

    if (logic->PlayerData[1].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[1].Score >= logic->PlayerData[2].Score
        && logic->PlayerData[1].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 1;

    if (logic->PlayerData[2].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[2].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[2].Score >= logic->PlayerData[3].Score)  PlayerWithHighestScore = 2;

    if (logic->PlayerData[3].Score >= logic->PlayerData[0].Score
        && logic->PlayerData[3].Score >= logic->PlayerData[1].Score
        && logic->PlayerData[3].Score >= logic->PlayerData[2].Score)  PlayerWithHighestScore = 3;

    if (logic->PlayerData[PlayerWithHighestScore].Score < 1 || logic->PlayerData[PlayerWithHighestScore].PlayerInput == CPU)
    {
        PlayerWithHighestScore = 255;
        PlayerRankOnGameOver = 10;
        return;
    }

    for (int rank = 0; rank < 10; rank++)
    {
        if (logic->PlayerData[PlayerWithHighestScore].Score >= HighScoresScore[logic->GameMode][rank])
        {
            playerRank = rank;

            for (int index = 9; index > rank; index--)
            {
                SDL_strlcpy(HighScoresName[logic->GameMode][index], HighScoresName[logic->GameMode][index-1], sizeof HighScoresName[logic->GameMode][index]);
                HighScoresLevel[logic->GameMode][index] = HighScoresLevel[logic->GameMode][index-1];
                HighScoresScore[logic->GameMode][index] = HighScoresScore[logic->GameMode][index-1];
            }

            char erase[20];
            for (int letter = 0; letter < 20; letter++)  erase[letter] = '\0';
            for (int letterTwo = 0; letterTwo < 20; letterTwo++)  HighScoresName[logic->GameMode][rank][letterTwo] = '\0';
            SDL_strlcpy(HighScoresName[logic->GameMode][rank], erase, sizeof HighScoresName[logic->GameMode][rank]);

            HighScoresLevel[logic->GameMode][rank] = logic->PlayerData[PlayerWithHighestScore].Level;
            HighScoresScore[logic->GameMode][rank] = logic->PlayerData[PlayerWithHighestScore].Score;

            rank = 10;
        }
    }

    PlayerRankOnGameOver = playerRank;
    if (PlayerRankOnGameOver == 10)
    {
        PlayerWithHighestScore = 255;
        return;
    }

    char nameClear[19] = { " " };
    for (int index = 0; index < 19; index++)
        HighScoresName[logic->GameMode][playerRank][index] = nameClear[index];

    NameInputJoyCharX = 0;
    NameInputJoyCharY = 0;
    NameInputJoyChar = 'A';
}

//-------------------------------------------------------------------------------------------------
void Data::ClearHighScores(void)
{
char name1st[19]  = { 'J', 'e', 'Z', 'x', 'L', 'e', 'e', '\0' };
char name2nd[19]  = { 'A', 'l', 'e', 'x', 'e', 'y', ' ', 'P', 'a', 'j', 'i', 't', 'n', 'o', 'v', '\0' };
char name3rd[19]  = { 'n', 'u', 'l', 'l', 'r', 'a', 'u', 'm', '\0' };
char name4th[19]  = { 'D', 'a', 'o', 't', 'h', 'e', 'm', 'a', 'n', '\0' };
char name5th[19]  = { 'm', 'a', 't', 't', 'm', 'a', 't', 't', 'e', 'h', '\0' };
char name6th[19]  = { 'S', 'D', 'L', '2', '\0' };
char name7th[19]  = { 'D', 'J', ' ', 'F', 'a', 'd', 'i', 'n', 'g', ' ', 'T', 'w', 'i', 'l', 'i', 'g', 'h', 't', '\0' };
char name8th[19]  = { 'L', 'i', 'n', 'u', 'x', ' ', 'M', 'i', 'n', 't', '\0' };
char name9th[19]  = { 'C', 'o', 'd', 'e', 'B', 'l', 'o', 'c', 'k', 's', '\0' };
char name10th[19] = { 'Y', 'o', 'u', '!', '\0' };

    for (int gameMode = 0; gameMode < 7; gameMode++)
        for (int index = 0; index < 10; index++)
            for (int indexTwo = 0; indexTwo < 20; indexTwo++)
                HighScoresName[gameMode][index][indexTwo] = (char)NULL;

    for (int gameMode = 0; gameMode < 7; gameMode++)
    {
        for (int loop = 0; loop < 10; loop++)
        {
            switch (loop)
            {
                case 0:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name1st[index];

                    HighScoresLevel[gameMode][loop] = 10;
                    HighScoresScore[gameMode][loop] = 1000;
                    break;

                case 1:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name2nd[index];

                    HighScoresLevel[gameMode][loop] = 9;
                    HighScoresScore[gameMode][loop] = 900;
                    break;

                case 2:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name3rd[index];

                    HighScoresLevel[gameMode][loop] = 8;
                    HighScoresScore[gameMode][loop] = 800;
                    break;

                case 3:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name4th[index];

                    HighScoresLevel[gameMode][loop] = 7;
                    HighScoresScore[gameMode][loop] = 700;
                    break;

                case 4:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name5th[index];

                    HighScoresLevel[gameMode][loop] = 6;
                    HighScoresScore[gameMode][loop] = 600;
                    break;

                case 5:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name6th[index];

                    HighScoresLevel[gameMode][loop] = 5;
                    HighScoresScore[gameMode][loop] = 500;
                    break;

                case 6:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name7th[index];

                    HighScoresLevel[gameMode][loop] = 4;
                    HighScoresScore[gameMode][loop] = 400;
                    break;

                case 7:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name8th[index];

                    HighScoresLevel[gameMode][loop] = 3;
                    HighScoresScore[gameMode][loop] = 300;
                    break;

                case 8:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name9th[index];

                    HighScoresLevel[gameMode][loop] = 2;
                    HighScoresScore[gameMode][loop] = 200;
                    break;

                case 9:
                    for (int index = 0; index < 19; index++)
                        HighScoresName[gameMode][loop][index] = name10th[index];

                    HighScoresLevel[gameMode][loop] = 1;
                    HighScoresScore[gameMode][loop] = 100;
                    break;

                default:
                    break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Data::LoadHighScoresAndOptions(void)
{
char filename[256];
fstream fileStream;
char textBuffer[100];
char *base_path = SDL_GetPrefPath("16BitSoftInc", DataVersionName);
char *pref_path = NULL;
char joystickNameTemp[4][100];

    if (base_path)
    {
        pref_path = SDL_strdup(base_path);
        SDL_free(base_path);
    }
    else  return;

	SDL_strlcpy(filename, pref_path, sizeof filename);
	SDL_strlcat(filename, "T-Crisis4-Data-Tribute3c", sizeof filename);

	fileStream.open (filename, fstream::in);
	if (fileStream.is_open())
	{
		fileStream.getline (textBuffer, 30);
		audio->MusicVolume = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		audio->SoundVolume = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		visuals->FullScreenMode = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->CPUPlayerEnabled = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->PlayingGameFrameLock = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DelayAutoShift = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->GameMode = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->SelectedMusicTrack = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
		audio->MusicJukeboxMode = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->SelectedBackground = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->NewGameGarbageHeight = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->PressingUPAction = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DisplayNextPiece = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->DisplayDropShadow = (int)atoi(textBuffer);

		fileStream.getline (textBuffer, 30);
		logic->TileSet = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyButtonOne = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyButtonTwo = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyUP = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyRIGHT = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyDOWN = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyLEFT = (int)atoi(textBuffer);

        fileStream.getline (textBuffer, 30);
        input->UserDefinedKeyPause = (int)atoi(textBuffer);

        for (int index = 0; index < 4; index++)
        {
            fileStream.getline (textBuffer, 30);
            input->JoyUP[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyDOWN[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyLEFT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyRIGHT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyButton1[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyButton2[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->JoyPause[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadUP[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadDOWN[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadLEFT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadRIGHT[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadButton1[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadButton2[index] = (int)atoi(textBuffer);

            fileStream.getline (textBuffer, 30);
            input->GamepadPause[index] = (int)atoi(textBuffer);
        }

        for (int gameMode = 0; gameMode < 7; gameMode++)
        {
            for (Uint8 rank = 0; rank < 10; rank++)
            {
                for (int index = 0; index < 18; index++)
                {
                    HighScoresName[gameMode][rank][index] = char(NULL);
                }

                fileStream.getline (textBuffer, 30);

                for (Uint8 index = 0; index < 18; index++)
                {
                    HighScoresName[gameMode][rank][index] = textBuffer[index];
                }

                fileStream.getline (textBuffer, 30);
                HighScoresLevel[gameMode][rank] = atoi(textBuffer);

                fileStream.getline (textBuffer, 30);
                HighScoresScore[gameMode][rank] = atoll(textBuffer);
            }
        }

        for (int index = 0; index < 4; index++)
        {
            for (int indexTwo = 0; indexTwo < 100; indexTwo++)
            {
                joystickNameTemp[index][indexTwo] = char(NULL);
            }

            fileStream.getline (textBuffer, 100);

            for (Uint8 indexThree = 0; indexThree < 100; indexThree++)
            {
                joystickNameTemp[index][indexThree] = textBuffer[indexThree];
            }
        }

        for (Uint8 indexFour = 0; indexFour < 4; indexFour++)
        {
            if (  ( strcmp(joystickNameTemp[indexFour], input->JoystickName[indexFour]) != 0 )  )
            {
                input->JoyUP[indexFour] = Axis1;
                input->JoyDOWN[indexFour] = Axis1;
                input->JoyLEFT[indexFour] = Axis0;
                input->JoyRIGHT[indexFour] = Axis0;
                input->JoyButton1[indexFour] = Button0;
                input->JoyButton2[indexFour] = Button1;
                input->JoyPause[indexFour] = Button2;

                printf("* Joystick #%i data not matched - joystick reset!\n", indexFour);
            }

            else
            {
                SDL_strlcpy(input->JoystickName[indexFour], joystickNameTemp[indexFour], sizeof input->JoystickName[indexFour]);
            }
        }

        fileStream.close();
	}
}

//-------------------------------------------------------------------------------------------------
void Data::SaveHighScoresAndOptions(void)
{
char filename[256];
fstream fileStream;
char textBuffer[100];
char *base_path = SDL_GetPrefPath("16BitSoftInc", DataVersionName);
char *pref_path = NULL;

    if (base_path)
    {
        pref_path = SDL_strdup(base_path);
        SDL_free(base_path);
    }
    else  return;

	SDL_strlcpy(filename, pref_path, sizeof filename);
	SDL_strlcat(filename, "T-Crisis4-Data-Tribute3c", sizeof filename);

	fileStream.open (filename, fstream::out);
	if (fileStream.is_open())
	{
		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", audio->MusicVolume);
		fileStream<<textBuffer;
		fileStream<<"\n";

		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", audio->SoundVolume);
		fileStream<<textBuffer;
		fileStream<<"\n";

		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", visuals->FullScreenMode);
		fileStream<<textBuffer;
		fileStream<<"\n";

		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->CPUPlayerEnabled);
		fileStream<<textBuffer;
		fileStream<<"\n";

		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->PlayingGameFrameLock);
		fileStream<<textBuffer;
		fileStream<<"\n";

		SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->DelayAutoShift);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->GameMode);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->SelectedMusicTrack);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", audio->MusicJukeboxMode);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->SelectedBackground);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->NewGameGarbageHeight);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->PressingUPAction);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->DisplayNextPiece);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->DisplayDropShadow);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", logic->TileSet);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyButtonOne);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyButtonTwo);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyUP);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyRIGHT);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyDOWN);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyLEFT);
		fileStream<<textBuffer;
		fileStream<<"\n";

        SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->UserDefinedKeyPause);
		fileStream<<textBuffer;
		fileStream<<"\n";

        for (int index = 0; index < 4; index++)
        {
            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyUP[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyDOWN[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyLEFT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyRIGHT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyButton1[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyButton2[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->JoyPause[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadUP[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadDOWN[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadLEFT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadRIGHT[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadButton1[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadButton2[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";

            SDL_snprintf (textBuffer, sizeof textBuffer, "%d", input->GamepadPause[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";
        }

        for (int gameMode = 0; gameMode < 7; gameMode++)
        {
            for (Uint8 rank = 0; rank < 10; rank++)
            {
                SDL_snprintf (textBuffer, sizeof textBuffer, "%s", HighScoresName[gameMode][rank]);
                fileStream<<textBuffer;
                fileStream<<"\n";

                SDL_snprintf (textBuffer, sizeof textBuffer, "%d", HighScoresLevel[gameMode][rank]);
                fileStream<<textBuffer;
                fileStream<<"\n";

                #ifdef _WIN32
                    SDL_snprintf (textBuffer, sizeof textBuffer, "%I64u", (long long)HighScoresScore[gameMode][rank]);
                #else
                    sprintf(textBuffer, "%lu", HighScoresScore[gameMode][rank]);
                #endif

                fileStream<<textBuffer;
                fileStream<<"\n";
            }
        }

        for (int index = 0; index < 4; index++)
        {
            SDL_snprintf (textBuffer, sizeof textBuffer, "%s", input->JoystickName[index]);
            fileStream<<textBuffer;
            fileStream<<"\n";
        }

        fileStream.close();
	}
}

//-------------------------------------------------------------------------------------------------
