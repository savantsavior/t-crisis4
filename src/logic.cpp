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
#include <cstdlib>
#include <float.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "logic.h"

#include "audio.h"
#include "screens.h"
#include "input.h"
#include "visuals.h"

extern Audio* audio;
extern Screens* screens;
extern Input* input;
extern Visuals* visuals;

//-------------------------------------------------------------------------------------------------
Logic::Logic(void)
{
    ValMovePieceHeight = 0.0f;
    ValMoveTrappedHoles = 0.0f;
    ValMoveOneBlockCavernHoles = 0.0f;
    ValMovePlayfieldBoxEdges = 0.0f;
    ValMoveCompletedLines = 0.0f;

    TimeAttackTimer = 0;

    for (int index = 0; index < 10; index++)
    {
        StoryShown[index] = -1;
    }

    StoryLevelAdvanceCounter = -1;
    StoryLevelAdvanceValue = -1;

    PlayersCanJoin = false;

    Uint8 piece, rotation, box;
    for (piece = 0; piece < 8; piece++)
        for (rotation = 0; rotation < 5; rotation++)
            for (box = 0; box < 17; box++)
                PieceData[piece][rotation][box] = 0;

    Multiplier = 0.0f;
    MultiplierSelected = -1;

    JoinInTimer = -1;

    HumanStillAlive = false;

    GameOverTimer = -1;

    GameForfeit = false;

    DangerRepeat = -1;

    CrisisModeTimer = -1;

    CrisisModeOnePlayerLeftPlayfieldCleared = false;

    Crisis7BGMPlayed = false;

    ContinueWatchingTimer = -1;

    for (int index = 0; index < 4; index++)
    {
        BlockAttackTransparency[index] = 255;
    }

    BestMovePieceHeight = 0.0f;
    BestMoveTrappedHoles = 0.0f;
    BestMoveOneBlockCavernHoles = 0.0f;
    BestMovePlayfieldBoxEdges = 0.0f;
    BestMoveCompletedLines = 0.0f;
    BestTotalLinesPerGame = -1;

    AllHumansDeadExitTimer = -1;

    GameMode = CrisisMode;

    CPUPlayerEnabled = 1;

    SelectedBackground = 0;
    SelectedMusicTrack = 2;
    NewGameGarbageHeight = 0;

    DisplayNextPiece = true;
    DisplayDropShadow = true;

    PressingUPAction = Rotate;

    for (int player = 0; player < NumberOfPlayers; player++)
    {
        PlayerData[player].PlayerInput = -1;

        PlayerData[player].UPActionTaken = false;
        PlayerData[player].RotateDirection = 0;

        PlayerData[player].Score = 0;
        PlayerData[player].DropBonus = 0;
        PlayerData[player].Level = 0;
        PlayerData[player].Lines = 0;
    }

    PlayingGameFrameLock = 33;

    TileSet = 0;

    GameWasJustPlayed = false;
    Won = false;

    ThinkRussianTimer = 0;

    TotalCPUPlayerLines = 0;
    NumberofCPUGames = 4;
    TotalOneLines = 0;
    TotalTwoLines = 0;
    TotalThreeLines = 0;
    TotalFourLines = 0;

    MaxRotationArray[0] = 0;
    MaxRotationArray[1] = 2;
    MaxRotationArray[2] = 2;
    MaxRotationArray[3] = 4;
    MaxRotationArray[4] = 4;
    MaxRotationArray[5] = 4;
    MaxRotationArray[6] = 1;
    MaxRotationArray[7] = 2;

    PieceDropStartHeight[0] = 0;
    PieceDropStartHeight[1] = 4;
    PieceDropStartHeight[2] = 4;
    PieceDropStartHeight[3] = 4;
    PieceDropStartHeight[4] = 4;
    PieceDropStartHeight[5] = 4;
    PieceDropStartHeight[6] = 3;
    PieceDropStartHeight[7] = 5;

    Player = 0;

    DelayAutoShift = 2;

    PAUSEgame = false;

    BestTotalLinesPerGame = 0;
}

//-------------------------------------------------------------------------------------------------
Logic::~Logic(void)
{

}

//-------------------------------------------------------------------------------------------------
void Logic::InitializePieceData(void)
{
Uint8 piece, rotation, box;

    for (rotation = 0; rotation < 5; rotation++)
        for (piece = 0; piece < 8; piece++)
            for (box = 0; box < 17; box++)
                PieceData [piece] [rotation] [box] = 0;

    /* RED "S Piece"... */
    PieceData [1] [1] [10] = 1; /* 01 02 03 04 */
    PieceData [1] [1] [11] = 1; /* 05 06 07 08 */
    PieceData [1] [1] [13] = 1; /* 09 [] [] 12 */
    PieceData [1] [1] [14] = 1; /* [] [] 15 16 */

    PieceData [1] [2] [ 5] = 1;
    PieceData [1] [2] [ 9] = 1;
    PieceData [1] [2] [10] = 1;
    PieceData [1] [2] [14] = 1;

    PieceData [1] [3] [10] = 1;
    PieceData [1] [3] [11] = 1;
    PieceData [1] [3] [13] = 1;
    PieceData [1] [3] [14] = 1;

    PieceData [1] [4] [ 5] = 1;
    PieceData [1] [4] [ 9] = 1;
    PieceData [1] [4] [10] = 1;
    PieceData [1] [4] [14] = 1;

    /* ORANGE "Z Piece"... */
    PieceData [2] [1] [ 9] = 1;
    PieceData [2] [1] [10] = 1;
    PieceData [2] [1] [14] = 1;
    PieceData [2] [1] [15] = 1;

    PieceData [2] [2] [ 6] = 1;
    PieceData [2] [2] [ 9] = 1;
    PieceData [2] [2] [10] = 1;
    PieceData [2] [2] [13] = 1;

    PieceData [2] [3] [ 9] = 1;
    PieceData [2] [3] [10] = 1;
    PieceData [2] [3] [14] = 1;
    PieceData [2] [3] [15] = 1;

    PieceData [2] [4] [ 6] = 1;
    PieceData [2] [4] [ 9] = 1;
    PieceData [2] [4] [10] = 1;
    PieceData [2] [4] [13] = 1;

    /* AQUA "T Piece"... */
    PieceData [3] [1] [ 9] = 1;
    PieceData [3] [1] [10] = 1;
    PieceData [3] [1] [11] = 1;
    PieceData [3] [1] [14] = 1;

    PieceData [3] [2] [ 6] = 1;
    PieceData [3] [2] [ 9] = 1;
    PieceData [3] [2] [10] = 1;
    PieceData [3] [2] [14] = 1;

    PieceData [3] [3] [ 6] = 1;
    PieceData [3] [3] [ 9] = 1;
    PieceData [3] [3] [10] = 1;
    PieceData [3] [3] [11] = 1;

    PieceData [3] [4] [ 6] = 1;
    PieceData [3] [4] [10] = 1;
    PieceData [3] [4] [11] = 1;
    PieceData [3] [4] [14] = 1;

    /* YELLOW "L Piece"... */
    PieceData [4] [1] [ 9] = 1;
    PieceData [4] [1] [10] = 1;
    PieceData [4] [1] [11] = 1;
    PieceData [4] [1] [13] = 1;

    PieceData [4] [2] [ 5] = 1;
    PieceData [4] [2] [ 6] = 1;
    PieceData [4] [2] [10] = 1;
    PieceData [4] [2] [14] = 1;

    PieceData [4] [3] [ 7] = 1;
    PieceData [4] [3] [ 9] = 1;
    PieceData [4] [3] [10] = 1;
    PieceData [4] [3] [11] = 1;

    PieceData [4] [4] [ 6] = 1;
    PieceData [4] [4] [10] = 1;
    PieceData [4] [4] [14] = 1;
    PieceData [4] [4] [15] = 1;

    /* GREEN "Backwards L Piece"... */
    PieceData [5] [1] [ 9] = 1;
    PieceData [5] [1] [10] = 1;
    PieceData [5] [1] [11] = 1;
    PieceData [5] [1] [15] = 1;

    PieceData [5] [2] [ 6] = 1;
    PieceData [5] [2] [10] = 1;
    PieceData [5] [2] [13] = 1;
    PieceData [5] [2] [14] = 1;

    PieceData [5] [3] [ 5] = 1;
    PieceData [5] [3] [ 9] = 1;
    PieceData [5] [3] [10] = 1;
    PieceData [5] [3] [11] = 1;

    PieceData [5] [4] [ 6] = 1;
    PieceData [5] [4] [ 7] = 1;
    PieceData [5] [4] [10] = 1;
    PieceData [5] [4] [14] = 1;

    /* BLUE "Box Piece"... */
    PieceData [6] [1] [10] = 1;
    PieceData [6] [1] [11] = 1;
    PieceData [6] [1] [14] = 1;
    PieceData [6] [1] [15] = 1;

    PieceData [6] [2] [10] = 1;
    PieceData [6] [2] [11] = 1;
    PieceData [6] [2] [14] = 1;
    PieceData [6] [2] [15] = 1;

    PieceData [6] [3] [10] = 1;
    PieceData [6] [3] [11] = 1;
    PieceData [6] [3] [14] = 1;
    PieceData [6] [3] [15] = 1;

    PieceData [6] [4] [10] = 1;
    PieceData [6] [4] [11] = 1;
    PieceData [6] [4] [14] = 1;
    PieceData [6] [4] [15] = 1;

    /* PURPLE "Line Piece"... */
    PieceData [7] [1] [ 9] = 1;
    PieceData [7] [1] [10] = 1;
    PieceData [7] [1] [11] = 1;
    PieceData [7] [1] [12] = 1;

    PieceData [7] [2] [ 2] = 1;
    PieceData [7] [2] [ 6] = 1;
    PieceData [7] [2] [10] = 1;
    PieceData [7] [2] [14] = 1;

    PieceData [7] [3] [ 9] = 1;
    PieceData [7] [3] [10] = 1;
    PieceData [7] [3] [11] = 1;
    PieceData [7] [3] [12] = 1;

    PieceData [7] [4] [ 2] = 1;
    PieceData [7] [4] [ 6] = 1;
    PieceData [7] [4] [10] = 1;
    PieceData [7] [4] [14] = 1;
}

//-------------------------------------------------------------------------------------------------
void Logic::ClearPlayfieldsWithCollisionDetection(void)
{
	for (int player = 0; player < NumberOfPlayers; player++)
	{
		for (int y = 0; y < 26; y++)
			for (int x = 0; x < 15; x++)
				PlayerData[player].Playfield[x][y] = 255; /* Collision detection value */

        for (int y = 2; y < 5; y++)
            for (int x = 5; x < 9; x++)
                PlayerData[player].Playfield[x][y] = 0;

        for (int y = 5; y < 24; y++)
            for (int x = 2; x < 12; x++)
                PlayerData[player].Playfield[x][y] = 0;
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::FillPieceBag(int player)
{
bool done = false;

    PlayerData[player].PieceBagIndex = 1;

    for (int index = 0; index < 8; index++)
    {
        PlayerData[player].PieceBag[0][index] = -1;
        PlayerData[player].PieceSelectedAlready[index] = false;
    }

    PlayerData[player].PieceBag[0][1] = ( rand()%7 + 1 );
    PlayerData[player].PieceSelectedAlready[ PlayerData[player].PieceBag[0][1] ] = true;
    while (done == false)
    {
        for (int x = 2; x < 8; x++)
        {
            int randomPieceToTry = ( rand()%7 + 1 );
            if (PlayerData[player].PieceSelectedAlready[randomPieceToTry] == false)
            {
                PlayerData[player].PieceBag[0][x] = randomPieceToTry;
                PlayerData[player].PieceSelectedAlready[randomPieceToTry] = true;

                if (x == 7)  done = true;
            }
            else
            {
                x--;
            }
        }
    }

    PlayerData[player].NextPiece = PlayerData[player].PieceBag[0][2];
    PlayerData[Player].PieceBagIndex = 1;
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollision(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 0; y < 4; y++)
        for (x = 0; x < 4; x++)
        {
            if (   (  ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 10)
               &&(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] < 20) )
               ||(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] == 255)  )
               &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0)   )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollisionDown(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 1; y < 5; y++)
        for (x = 0; x < 4; x++)
        {
            if (   (  ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 10)
               &&(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] < 20) )
               ||(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] == 255)  )
               &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0)   )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollisionLeft(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 0; y < 4; y++)
        for (x = -1; x < 3; x++)
        {
            if (   (  ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 10)
               &&(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] < 20) )
               ||(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] == 255)  )
               &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0)   )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
int Logic::PieceCollisionRight(void)
{
Sint8 x, y;
Uint8 box = 1;
int returnValue = CollisionNotTrue;

    for (y = 1; y < 5; y++)
        for (x = 1; x < 5; x++)
        {
            if (   (  ( (PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] > 10)
               &&(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] < 20) )
               ||(PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX + x ][ PlayerData[Player].PiecePlayfieldY + y ] == 255)  )
               &&(PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [box] > 0)   )
                    returnValue = CollisionWithPlayfield;

            box++;
        }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Logic::AddPieceToPlayfieldMemory(int TempOrCurrentOrNextOrDropShadow)
{
    if (DisplayDropShadow == false && TempOrCurrentOrNextOrDropShadow == DropShadow)  return;

	Uint8 TEMP_Piece = PlayerData[Player].Piece;
	Uint8 TEMP_PieceRotation = PlayerData[Player].PieceRotation;
	Uint8 TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
	Uint8 TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

	int value = PlayerData[Player].Piece+10;

    if (TempOrCurrentOrNextOrDropShadow == Next)
	{
		PlayerData[Player].Piece = PlayerData[Player].NextPiece;
		value = PlayerData[Player].NextPiece+10;
		PlayerData[Player].PieceRotation = 1;

        if      (Player == 0)  PlayerData[Player].PiecePlayfieldX = 5;
        else if (Player == 1)  PlayerData[Player].PiecePlayfieldX = 5;
        else if (Player == 2)  PlayerData[Player].PiecePlayfieldX = 5;

		PlayerData[Player].PiecePlayfieldY = 0;
	}
	else if (TempOrCurrentOrNextOrDropShadow == DropShadow)
	{
		for (int y = PlayerData[Player].PiecePlayfieldY; y < 23; y++)
		{
			PlayerData[Player].PiecePlayfieldY = y;
			if (PieceCollision() != CollisionNotTrue)
			{
				if (y - TEMP_PiecePlayfieldY > 4)
				{
					value = 1;
					PlayerData[Player].PiecePlayfieldY = y-1;
					y = 100;
				}
				else
				{
					PlayerData[Player].Piece = TEMP_Piece;
					PlayerData[Player].PieceRotation = TEMP_PieceRotation;
					PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
					PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
					return;
				}
			}
		}
	}
    else if (TempOrCurrentOrNextOrDropShadow == Temp)
    {
        value = 999;
    }

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = value;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = value;

	PlayerData[Player].Piece = TEMP_Piece;
	PlayerData[Player].PieceRotation = TEMP_PieceRotation;
	PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
	PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
}

//-------------------------------------------------------------------------------------------------
void Logic::DeletePieceFromPlayfieldMemory(int CurrentOrDropShadow)
{
    if (DisplayDropShadow == false && CurrentOrDropShadow == DropShadow)  return;

    if (PlayerData[Player].PlayerStatus == FlashingCompletedLines || PlayerData[Player].PlayerStatus == ClearingCompletedLines)  return;

	Uint8 TEMP_Piece = PlayerData[Player].Piece;
	Uint8 TEMP_PieceRotation = PlayerData[Player].PieceRotation;
	Uint8 TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
	Uint8 TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

	if (CurrentOrDropShadow == DropShadow)
	{
		for (int y = PlayerData[Player].PiecePlayfieldY; y < 23; y++)
		{
			PlayerData[Player].PiecePlayfieldY = y;
			if (PieceCollision() != CollisionNotTrue)
			{
				if (y - TEMP_PiecePlayfieldY > 4)
				{
					PlayerData[Player].PiecePlayfieldY = y-1;
					y = 100;
				}
				else
				{
					PlayerData[Player].Piece = TEMP_Piece;
					PlayerData[Player].PieceRotation = TEMP_PieceRotation;
					PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
					PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
					return;
				}
			}
		}
	}

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;

	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
	if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;

	PlayerData[Player].Piece = TEMP_Piece;
	PlayerData[Player].PieceRotation = TEMP_PieceRotation;
	PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
	PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
}

//-------------------------------------------------------------------------------------------------
void Logic::SetupNewPiece(void)
{
	PlayerData[Player].Piece = PlayerData[Player].NextPiece;
	PlayerData[Player].PieceMovementDelay = 0;
	PlayerData[Player].PieceRotation = 1;

    if (Player == 0)  PlayerData[0].PiecePlayfieldX = 5;
    else if (Player == 1)  PlayerData[1].PiecePlayfieldX = 5;
    else if (Player == 2)  PlayerData[2].PiecePlayfieldX = 5;
    else if (Player == 3)  PlayerData[3].PiecePlayfieldX = 5;

	PlayerData[Player].PiecePlayfieldY = 0;

    if (PlayerData[Player].PieceBagIndex < 7)
    {
        PlayerData[Player].Piece = PlayerData[Player].NextPiece;
    	PlayerData[Player].NextPiece = PlayerData[Player].PieceBag[0][ PlayerData[Player].PieceBagIndex+1 ];
        PlayerData[Player].PieceBagIndex++;
    }
    else if (PlayerData[Player].PieceBagIndex == 7)
    {
        PlayerData[Player].Piece = PlayerData[Player].NextPiece;
        FillPieceBag(Player);
    	PlayerData[Player].NextPiece = PlayerData[Player].PieceBag[0][ PlayerData[Player].PieceBagIndex ];
        PlayerData[Player].PieceBagIndex = 1;
    }

	PlayerData[Player].PlayerStatus = NewPieceDropping;

	PlayerData[Player].PieceDropTimer = 0;

	PlayerData[Player].PieceRotated1 = false;
	PlayerData[Player].PieceRotated2 = false;

	PlayerData[Player].FlashCompletedLinesTimer = 0;
	PlayerData[Player].ClearCompletedLinesTimer = 0;

	PlayerData[Player].DropBonus = 0;

    PlayerData[Player].BestMoveX = -1;
    PlayerData[Player].BestRotation = -1;
    PlayerData[Player].BestMoveCalculated = false;
    PlayerData[Player].MovedToBestMove = false;
}

//-------------------------------------------------------------------------------------------------
void Logic::CheckForCompletedLines(void)
{
int numberOfCompletedLines = 0;

    AddPieceToPlayfieldMemory(Current);

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 20) )
				boxTotal++;
		}

		if (boxTotal == 10)
            numberOfCompletedLines++;
	}

	if (numberOfCompletedLines > 0)
	{
		if (numberOfCompletedLines == 1)
		{
			PlayerData[Player].Score += (40 * (PlayerData[Player].Level+1) );
            TotalOneLines++;
		}
		else if (numberOfCompletedLines == 2)
		{
			PlayerData[Player].Score += (100 * (PlayerData[Player].Level+1) );
            TotalTwoLines++;
		}
		else if (numberOfCompletedLines == 3)
		{
			PlayerData[Player].Score += (300 * (PlayerData[Player].Level+1) );
            TotalThreeLines++;
		}
		else if (numberOfCompletedLines == 4)
		{
			PlayerData[Player].Score += (1200 * (PlayerData[Player].Level+1) );
            TotalFourLines++;
			audio->PlayDigitalSoundFX(7, 0);
		}

		PlayerData[Player].PlayerStatus = FlashingCompletedLines;
		PlayerData[Player].FlashCompletedLinesTimer = 1;
	}
	else  SetupNewPiece();
}

//-------------------------------------------------------------------------------------------------
void Logic::CheckForDanger(void)
{
bool inDanger = false;

    if (GameMode == StoryMode)
    {
        for ( int y = 5; y < (5+4); y++ )
        {
            for (int x = 2; x < 12; x++)
            {
                if (PlayerData[Player].Playfield[x][y] > 10 && PlayerData[Player].Playfield[x][y] < 20)
                {
                    inDanger = true;
                }
            }
        }

        if (inDanger == true)
        {
            if (DangerRepeat < 100)
            {
                DangerRepeat++;
            }
            else
            {
                DangerRepeat = 0;

                audio->PlayDigitalSoundFX(15, 0);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDown(bool Force)
{
    if (Force == false)
    {
        if (PlayerData[Player].PlayerInput == CPU)
        {
            if (CPUPlayerEnabled == 1 || CPUPlayerEnabled == 6)
            {
                PlayerData[Player].CPUFrame++;
                if (PlayerData[Player].CPUFrame < 10)
                {
                    return;
                }
                else  PlayerData[Player].CPUFrame = 0;
            }
            else if (CPUPlayerEnabled == 2 || CPUPlayerEnabled == 7)
            {
                PlayerData[Player].CPUFrame++;
                if (PlayerData[Player].CPUFrame < 5)
                {
                    return;
                }
                else  PlayerData[Player].CPUFrame = 0;
            }
        }
    }

    DeletePieceFromPlayfieldMemory(Current);

	PlayerData[Player].PiecePlayfieldY++;

	if (PieceCollision() == CollisionWithPlayfield)
	{
        if (PlayersCanJoin == true && Player == 1 && GameMode != StoryMode)
        {
            if (PlayerData[0].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[0].PlayerInput = CPU;
                PlayerData[0].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[1].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[1].PlayerInput = CPU;
                PlayerData[1].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[2].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[2].PlayerInput = CPU;
                PlayerData[2].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[3].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[3].PlayerInput = CPU;
                PlayerData[3].PlayerStatus = NewPieceDropping;
            }

            if ( CPUPlayerEnabled > 0
                 && (PlayerData[0].PlayerInput == CPU || PlayerData[2].PlayerInput == CPU || PlayerData[2].PlayerInput == CPU) )  audio->PlayDigitalSoundFX(13, 0);

            PlayersCanJoin = false;
        }

		PlayerData[Player].PiecePlayfieldY--;

		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(3, 0);

		PlayerData[Player].Score += PlayerData[Player].DropBonus;

		PlayerData[Player].DropBonus = 0;
		PlayerData[Player].PieceDropTimer = 0;

		if (PlayerData[Player].PlayerStatus == NewPieceDropping)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            audio->PlayDigitalSoundFX(11, 0);
        }
		else  CheckForCompletedLines();
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDownFast(void)
{
	while (PieceCollision() == CollisionNotTrue)
    {
        PlayerData[Player].PiecePlayfieldY++;
        PlayerData[Player].DropBonus++;
    }

	if (PieceCollision() == CollisionWithPlayfield)
	{
        if (PlayersCanJoin == true && Player == 1)
        {
            if (PlayerData[0].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[0].PlayerInput = CPU;
                PlayerData[0].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[1].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[1].PlayerInput = CPU;
                PlayerData[1].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[2].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[2].PlayerInput = CPU;
                PlayerData[2].PlayerStatus = NewPieceDropping;
            }

            if (PlayerData[3].PlayerStatus == GameOver && CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5)
            {
                PlayerData[3].PlayerInput = CPU;
                PlayerData[3].PlayerStatus = NewPieceDropping;
            }

            if ( CPUPlayerEnabled != 0 && CPUPlayerEnabled != 5
                 && (PlayerData[0].PlayerInput == CPU || PlayerData[2].PlayerInput == CPU || PlayerData[3].PlayerInput == CPU) )  audio->PlayDigitalSoundFX(13, 0);

            PlayersCanJoin = false;
        }

		PlayerData[Player].PiecePlayfieldY--;

		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(3, 0);

		PlayerData[Player].Score += PlayerData[Player].DropBonus;

		PlayerData[Player].DropBonus = 0;
		PlayerData[Player].PieceDropTimer = 0;

		if (PlayerData[Player].PlayerStatus == NewPieceDropping)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            audio->PlayDigitalSoundFX(11, 0);
        }
		else  CheckForCompletedLines();
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceDownFastDropAndDrag(void)
{
    if (PieceCollisionDown() == CollisionWithPlayfield)
    {
    }
    else
    {
        while (PieceCollision() == CollisionNotTrue)
        {
            PlayerData[Player].PiecePlayfieldY++;
            PlayerData[Player].DropBonus++;
        }

        PlayerData[Player].PiecePlayfieldY--;

        PlayerData[Player].PieceDropTimer = 0;
    }
}

//-------------------------------------------------------------------------------------------------
bool Logic::RotatePieceCounterClockwise(void)
{
	if (PlayerData[Player].PieceRotation > 1)  PlayerData[Player].PieceRotation--;
	else  PlayerData[Player].PieceRotation = 4;

	if (PieceCollision() == CollisionNotTrue)
	{
		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(5, 0);

        return(true);
	}
	else
	{
		if (PlayerData[Player].PieceRotation < 4)  PlayerData[Player].PieceRotation++;
		else  PlayerData[Player].PieceRotation = 1;

        if (PlayerData[Player].RotateDirection == 0)  PlayerData[Player].RotateDirection = 1;
        else  PlayerData[Player].RotateDirection = 0;
    }

    return(false);
}

//-------------------------------------------------------------------------------------------------
bool Logic::RotatePieceClockwise(void)
{
	if (PlayerData[Player].PieceRotation < 4)  PlayerData[Player].PieceRotation++;
	else  PlayerData[Player].PieceRotation = 1;

	if (PieceCollision() == CollisionNotTrue)
	{
		if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(5, 0);

        return(true);
	}
	else
	{
		if (PlayerData[Player].PieceRotation > 1)  PlayerData[Player].PieceRotation--;
		else  PlayerData[Player].PieceRotation = 4;

        if (PlayerData[Player].RotateDirection == 0)  PlayerData[Player].RotateDirection = 1;
        else  PlayerData[Player].RotateDirection = 0;
	}

    return(false);
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceLeft(void)
{
    if (DelayAutoShift == 0)
    {
        if (PlayerData[Player].PieceMovementDelay > -15)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay == -5)
           ||(PlayerData[Player].PieceMovementDelay == -9)
           ||(PlayerData[Player].PieceMovementDelay < -14) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
    else if (DelayAutoShift == 1)
    {
        if (PlayerData[Player].PieceMovementDelay > -16)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay < -15) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
    else if (DelayAutoShift == 2)
    {
        if (PlayerData[Player].PieceMovementDelay > -6)  PlayerData[Player].PieceMovementDelay--;
        if ( (PlayerData[Player].PieceMovementDelay == -1)
           ||(PlayerData[Player].PieceMovementDelay < -5) )
        {
            PlayerData[Player].PiecePlayfieldX--;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX++;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::MovePieceRight(void)
{
    if (DelayAutoShift == 0)
    {
        if (PlayerData[Player].PieceMovementDelay < 15)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay == 5)
           ||(PlayerData[Player].PieceMovementDelay == 9)
           ||(PlayerData[Player].PieceMovementDelay > 14) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
    else if (DelayAutoShift == 1)
    {
        if (PlayerData[Player].PieceMovementDelay < 16)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay > 15) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
    else if (DelayAutoShift == 2)
    {
        if (PlayerData[Player].PieceMovementDelay < 6)  PlayerData[Player].PieceMovementDelay++;
        if ( (PlayerData[Player].PieceMovementDelay == 1)
           ||(PlayerData[Player].PieceMovementDelay > 5) )
        {
            PlayerData[Player].PiecePlayfieldX++;

            if (PieceCollision() == CollisionNotTrue)
            {
                if (ThinkRussianTimer == 0 && PlayerData[Player].PlayerInput != CPU)  audio->PlayDigitalSoundFX(2, 0);
            }
            else
            {
                PlayerData[Player].PiecePlayfieldX--;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Logic::SetupForNewGame(void)
{
    StoryLevelAdvanceValue = 10;
    StoryLevelAdvanceCounter = StoryLevelAdvanceValue;

    for (int index = 0; index < 10; index++)  StoryShown[index] = 0;

    DangerRepeat = 0;

    PlayersCanJoin = true;
    GameWasJustPlayed = true;

    GameForfeit = false;

    GameOverTimer = 0;

    JoinInTimer = 0;
    ContinueWatchingTimer = 0;

    HumanStillAlive = true;
    AllHumansDeadExitTimer = 0;

    if (SelectedBackground == 1)
    {
        audio->PlayDigitalSoundFX(9, 0);
        ThinkRussianTimer = 320;
    }
    else  ThinkRussianTimer = 0;

	InitializePieceData();
	ClearPlayfieldsWithCollisionDetection();

    PlayerData[0].PlayersPlayfieldScreenX = 80;
    PlayerData[0].PlayersPlayfieldScreenY = 230;

    if (GameMode < StoryMode)
        PlayerData[1].PlayersPlayfieldScreenX = 240;
    else
        PlayerData[1].PlayersPlayfieldScreenX = 640/2;

    PlayerData[1].PlayersPlayfieldScreenY = 230;

    PlayerData[2].PlayersPlayfieldScreenX = 400;
    PlayerData[2].PlayersPlayfieldScreenY = 230;

    PlayerData[3].PlayersPlayfieldScreenX = 560;
    PlayerData[3].PlayersPlayfieldScreenY = 230;

    PlayerData[0].PiecePlayfieldX = 5;
    PlayerData[0].PiecePlayfieldY = 0;

    PlayerData[1].PiecePlayfieldX = 5;
    PlayerData[1].PiecePlayfieldY = 0;

    PlayerData[2].PiecePlayfieldX = 5;
    PlayerData[2].PiecePlayfieldY = 0;

    PlayerData[3].PiecePlayfieldX = 5;
    PlayerData[3].PiecePlayfieldY = 0;

    PlayerData[0].PlayfieldStartX = 2;
    PlayerData[0].PlayfieldEndX = 12;

    PlayerData[1].PlayfieldStartX = 2;
    PlayerData[1].PlayfieldEndX = 12;

    PlayerData[2].PlayfieldStartX = 2;
    PlayerData[2].PlayfieldEndX = 12;

    PlayerData[3].PlayfieldStartX = 2;
    PlayerData[3].PlayfieldEndX = 12;

	for (int player = 0; player < NumberOfPlayers; player++)
	{
		Player = player;

        FillPieceBag(player);
        PlayerData[player].Piece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];
        PlayerData[player].PieceBagIndex = 1;
        PlayerData[player].NextPiece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];

        PlayerData[player].Piece = PlayerData[player].NextPiece;
        PlayerData[player].PieceBagIndex = 2;
        PlayerData[player].NextPiece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];

		PlayerData[player].PieceMovementDelay = 0;
		PlayerData[player].PieceRotation = 1;

		PlayerData[player].PieceDropTimer = 0;
		PlayerData[player].TimeToDropPiece = 47;

		PlayerData[player].PieceRotated1 = false;
		PlayerData[player].PieceRotated2 = false;

		PlayerData[player].FlashCompletedLinesTimer = 0;
		PlayerData[player].ClearCompletedLinesTimer = 0;

		if (input->DEBUG == 2) PlayerData[player].Score = 3210;
        else  PlayerData[player].Score = 0;

		PlayerData[player].DropBonus = 0;
		PlayerData[player].Level = 0;

		if (input->DEBUG == 2) PlayerData[player].Lines = 9;
        else  PlayerData[player].Lines = 0;

		for (int y = 0; y < 12; y++)
			for (int x = 0; x < 10; x++)
				PlayerData[player].AttackLines[x][y] = 0;

        if (GameMode == TwentyLineChallengeMode)  PlayerData[player].TwentyLineCounter = 20;

        PlayerData[Player].BestMoveX = -1;
        PlayerData[Player].BestRotation = -1;
        PlayerData[Player].BestMoveCalculated = false;
        PlayerData[Player].MovedToBestMove = false;

        PlayerData[Player].CPUFrame = 0;

        BlockAttackTransparency[Player] = 0;
	}

    PlayerData[0].PlayerStatus = GameOver;
    PlayerData[1].PlayerStatus = NewPieceDropping;
    PlayerData[2].PlayerStatus = GameOver;
    PlayerData[3].PlayerStatus = GameOver;

    if (screens->ScreenToDisplay != TestComputerSkillScreen)
    {
        if (PlayerData[1].PlayerInput == Keyboard)
        {
            if (input->JoystickDisabled[0] == false)  PlayerData[0].PlayerInput = JoystickOne;
            else
            {
                PlayerData[0].PlayerInput = CPU;
                if (CPUPlayerEnabled != 0  && CPUPlayerEnabled != 4)  PlayerData[0].PlayerStatus = NewPieceDropping;
                else  PlayerData[0].PlayerStatus = GameOver;
            }

            if (input->JoystickDisabled[1] == false)  PlayerData[2].PlayerInput = JoystickTwo;
            else
            {
                PlayerData[2].PlayerInput = CPU;
                if (CPUPlayerEnabled != 0  && CPUPlayerEnabled != 4)  PlayerData[2].PlayerStatus = NewPieceDropping;
                else  PlayerData[2].PlayerStatus = GameOver;
            }

            if (input->JoystickDisabled[2] == false)  PlayerData[3].PlayerInput = JoystickThree;
            else
            {
                PlayerData[3].PlayerInput = Mouse;
                PlayerData[3].PlayerStatus = GameOver;
            }
        }
        else if (PlayerData[1].PlayerInput >= JoystickOne && PlayerData[1].PlayerInput <= JoystickFour)
        {
            PlayerData[0].PlayerInput = CPU;
            PlayerData[0].PlayerStatus = GameOver;
            PlayerData[2].PlayerInput = Mouse;
            PlayerData[2].PlayerStatus = GameOver;
            PlayerData[3].PlayerInput = Keyboard;
            PlayerData[3].PlayerStatus = GameOver;

            bool joyUsed[6];
            for (int index = 0; index < 6; index++)
            {
                joyUsed[index] = false;
            }
            joyUsed[PlayerData[1].PlayerInput] = true;

            int joyToCheck = JoystickOne;
            bool done;
            for (int index = 0; index < 4; index++)
            {
                if (index == 1)  index = 2;

                done = false;
                while (done == false and joyToCheck < 5)
                {
                    if (input->JoystickDisabled[joyToCheck-1] == false && joyUsed[joyToCheck] == false)
                    {
                        PlayerData[index].PlayerInput = joyToCheck;
                        joyUsed[joyToCheck] = true;
                        PlayerData[index].PlayerStatus = GameOver;
                        done = true;
                    }

                    joyToCheck++;
                }
            }
        }
    }
    else
    {
        for (int player = 0; player < 4; player++)
        {
            PlayerData[player].PlayerInput = CPU;
            PlayerData[player].PlayerStatus = NewPieceDropping;
        }
    }

    if (GameMode == StoryMode)
    {
        for (int player = 0; player < 4; player++)
        {
            if (player != 1)
            {
                PlayerData[player].PlayerInput = CPU;
                PlayerData[player].PlayerStatus = GameOver;
                PlayersCanJoin = false;
            }
        }
    }

    if (input->DEBUG > 1 && GameMode != CrisisMode)
    {
        PlayerData[0].Score = 34656422096;
        PlayerData[0].Level = 9015;
        PlayerData[0].Lines = 90153;
        PlayerData[1].Score = 36666433807;
        PlayerData[1].Level = 9108;
        PlayerData[1].Lines = 91084;
        PlayerData[2].Score = 32647654097;
        PlayerData[2].Level = 9905;
        PlayerData[2].Lines = 99052;
        PlayerData[3].Score = 37539763964;
        PlayerData[3].Level = 9094;
        PlayerData[3].Lines = 90947;
    }

	Player = 0;

	PAUSEgame = false;
    PAUSEgameQuitJoy = false;

	if (GameMode == TimeAttack30Mode)  TimeAttackTimer = 6000;
	else if (GameMode == TimeAttack60Mode)  TimeAttackTimer = 12000;
	else if (GameMode == TimeAttack120Mode)  TimeAttackTimer = 24000;
    else  TimeAttackTimer = 0;

    CrisisModeTimer = 0;
    Crisis7BGMPlayed = false;

    Won = false;
    CrisisModeOnePlayerLeftPlayfieldCleared = false;

    if (NewGameGarbageHeight > 0 && GameMode != StoryMode)
    {
        for (int player = 0; player < NumberOfPlayers; player++)
        {
            for (int y = 23; y > 23-NewGameGarbageHeight; y--)
            {
                int boxTotal = 0;
                for (int x = 2; x < 12; x++)
                {
                    Uint32 box = rand()%8;
                    if (box > 0)  boxTotal++;

                    if (boxTotal < 10)
                    {
                        if (box != 0)  PlayerData[player].Playfield[x][y] = (int)box+10;
                        else  PlayerData[player].Playfield[x][y] = 0;
                    }
                    else  PlayerData[player].Playfield[x][y] = 0;
                }
           }
        }
    }

//    TestMode = true;
    if (TestMode == true)
    {
        PlayerData[1].Score = 13000;
        PlayerData[1].Level = 9;
        PlayerData[1].Lines = 0;
//        PlayerData[0].TwentyLineCounter = 1;
    }

    ValMovePieceHeight = 0.90f;
    ValMoveTrappedHoles = 2.36f;
    ValMoveOneBlockCavernHoles = 0.20f;
    ValMovePlayfieldBoxEdges = 1.38f;
    ValMoveCompletedLines = 1.87f;
}

//-------------------------------------------------------------------------------------------------
void Logic::SetupForNewLevelStory(void)
{
    DangerRepeat = 0;

    PlayerData[1].PlayersPlayfieldScreenX = 640/2;

    PlayerData[1].PlayersPlayfieldScreenY = 230;

    PlayerData[1].PiecePlayfieldX = 5;
    PlayerData[1].PiecePlayfieldY = 0;

    PlayerData[1].PlayfieldStartX = 2;
    PlayerData[1].PlayfieldEndX = 12;


    int player = 1;
    Player = player;

    FillPieceBag(player);
    PlayerData[player].Piece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];
    PlayerData[player].PieceBagIndex = 1;
    PlayerData[player].NextPiece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];

    PlayerData[player].Piece = PlayerData[player].NextPiece;
    PlayerData[player].PieceBagIndex = 2;
    PlayerData[player].NextPiece = PlayerData[player].PieceBag[0][PlayerData[player].PieceBagIndex];

    PlayerData[player].PieceMovementDelay = 0;
    PlayerData[player].PieceRotation = 1;

    PlayerData[player].PieceDropTimer = 0;
    PlayerData[player].TimeToDropPiece = 47 - (PlayerData[0].Level * 4);
    if (PlayerData[1].TimeToDropPiece < 4)  PlayerData[1].TimeToDropPiece = 4;

    PlayerData[player].PieceRotated1 = false;
    PlayerData[player].PieceRotated2 = false;

    PlayerData[player].FlashCompletedLinesTimer = 0;
    PlayerData[player].ClearCompletedLinesTimer = 0;

    PlayerData[player].DropBonus = 0;

    for (int y = 0; y < 12; y++)
        for (int x = 0; x < 10; x++)
            PlayerData[player].AttackLines[x][y] = 0;

    PlayerData[Player].BestMoveX = -1;
    PlayerData[Player].BestRotation = -1;
    PlayerData[Player].BestMoveCalculated = false;
    PlayerData[Player].MovedToBestMove = false;

    PlayerData[Player].CPUFrame = 0;

    BlockAttackTransparency[Player] = 0;

    PlayerData[0].PlayerStatus = GameOver;
    PlayerData[1].PlayerStatus = NewPieceDropping;
    PlayerData[2].PlayerStatus = GameOver;
    PlayerData[3].PlayerStatus = GameOver;

    if (GameMode == StoryMode)
    {
        for (int player = 0; player < 4; player++)
        {
            if (player != 1)
            {
                PlayerData[player].PlayerInput = CPU;
                PlayerData[player].PlayerStatus = GameOver;
                PlayersCanJoin = false;
            }
        }
    }

	Player = 0;

	PAUSEgame = false;

    CrisisModeTimer = 0;
    Crisis7BGMPlayed = false;

    Won = false;
    CrisisModeOnePlayerLeftPlayfieldCleared = false;
}

//-------------------------------------------------------------------------------------------------
void Logic::FlashCompletedLines(void)
{
int numberOfCompletedLines = 0;

	if (PlayerData[Player].FlashCompletedLinesTimer < 21)  PlayerData[Player].FlashCompletedLinesTimer++;

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 30) )
				boxTotal++;
		}

		if (boxTotal == 10)
		{
            numberOfCompletedLines++;

			if (PlayerData[Player].FlashCompletedLinesTimer % 2 == 0)
			{
				for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][y] = PlayerData[Player].Playfield[xTwo][y] + 10;
			}
			else
			{
				for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][y] = PlayerData[Player].Playfield[xTwo][y] - 10;
			}
		}
	}

	if (PlayerData[Player].FlashCompletedLinesTimer == 21)
	{
		PlayerData[Player].PlayerStatus = ClearingCompletedLines;

        if (CrisisModeOnePlayerLeftPlayfieldCleared == false)
        {
            for (int y = 5; y < 24; y++)
            {
                int boxTotal = 0;

                for (int x = 2; x < 12; x++)
                {
                    if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 30) )
                        boxTotal++;
                }

                if (boxTotal == 10 && numberOfCompletedLines > 1)
                {
                    for (int attackY = 1; attackY < 12; attackY++)
                        for (int attackX = 0; attackX < 10; attackX++)
                            PlayerData[Player].AttackLines[attackX][attackY-1] = PlayerData[Player].AttackLines[attackX][attackY];

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 1] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 2] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 3] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 4] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 5] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 6] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 7] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 8] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+1 ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [ 9] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [10] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [11] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [12] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+2 ] = 0;

                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [13] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [14] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+1 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [15] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+2 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;
                    if (PieceData [PlayerData[Player].Piece] [PlayerData[Player].PieceRotation] [16] == 1)
                        PlayerData[Player].Playfield[ PlayerData[Player].PiecePlayfieldX+3 ][ PlayerData[Player].PiecePlayfieldY+3 ] = 0;

                    int attackX = 0;
                    for (int xThree = 2; xThree < 12; xThree++)
                    {
                        PlayerData[Player].AttackLines[attackX][11] = PlayerData[Player].Playfield[xThree][y];
                        attackX++;
                    }
                }

                AddPieceToPlayfieldMemory(Current);
            }
        }
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::ClearCompletedLines(void)
{
bool thereWasACompletedLine = false;

	for (int y = 5; y < 24; y++)
	{
		int boxTotal = 0;

		for (int x = 2; x < 12; x++)
		{
			if ( (PlayerData[Player].Playfield[x][y] > 9)&&(PlayerData[Player].Playfield[x][y] < 20) )
				boxTotal++;
		}

		if (boxTotal == 10)
		{
			thereWasACompletedLine = true;

			if (PlayerData[Player].ClearCompletedLinesTimer < 40)  PlayerData[Player].ClearCompletedLinesTimer++;

			if (PlayerData[Player].ClearCompletedLinesTimer % 10 == 0)
			{
				for (int yTwo = y; yTwo > 5; yTwo--)
		            for (int xTwo = 2; xTwo < 12; xTwo++)
						PlayerData[Player].Playfield[xTwo][yTwo] = PlayerData[Player].Playfield[xTwo][yTwo-1];

                for (int xTwo = 2; xTwo < 12; xTwo++)
					PlayerData[Player].Playfield[xTwo][5] = 0;

				PlayerData[Player].Lines++;

                if (input->DEBUG > 1 && GameMode == CrisisMode)
                {
                    if (PlayerData[Player].Level > 0)  PlayerData[Player].Lines = (PlayerData[Player].Level+1) * 10;
                    else PlayerData[Player].Lines = 10;
                }

                if (PlayerData[Player].TwentyLineCounter > 0)  PlayerData[Player].TwentyLineCounter--;

				if (GameMode != StoryMode && PlayerData[Player].Lines % 10 == 0)
				{
                    Uint8 playersAlive = 0;
                    if (PlayerData[0].PlayerStatus != GameOver)  playersAlive++;
                    if (PlayerData[1].PlayerStatus != GameOver)  playersAlive++;
                    if (PlayerData[2].PlayerStatus != GameOver)  playersAlive++;
                    if (PlayerData[3].PlayerStatus != GameOver)  playersAlive++;

                    if ( (GameMode == CrisisMode && PlayerData[Player].Level < 9 && playersAlive == 1) || (GameMode < CrisisMode) )
                    {
                        PlayerData[Player].Level++;

                        if (GameMode == CrisisMode && PlayerData[Player].Level == 7 && Crisis7BGMPlayed == false)
                        {
                            audio->PlayMusic(24, -1);
                            audio->PlayDigitalSoundFX(12, 0);
                            audio->PlayDigitalSoundFX(15, 0);
                            Crisis7BGMPlayed = true;
                        }

                        PlayerData[Player].TimeToDropPiece-=5;
                        audio->PlayDigitalSoundFX(8, 0);
                    }
                    else if (PlayerData[Player].Level > 8 && GameMode == CrisisMode)
                    {
                        PlayerData[0].PlayerStatus = GameOver;
                        PlayerData[1].PlayerStatus = GameOver;
                        PlayerData[2].PlayerStatus = GameOver;
                        PlayerData[3].PlayerStatus = GameOver;

                        PlayerData[Player].Level++;

                        Won = true;
                    }
				}
				else if (GameMode == StoryMode)
				{
                    if (PlayerData[Player].Level < 10)
                    {
                        StoryLevelAdvanceCounter--;
                        if (StoryLevelAdvanceCounter == 0)
                        {
                            PlayerData[Player].Level++;

                            StoryLevelAdvanceValue+=0;//5;
                            StoryLevelAdvanceCounter = StoryLevelAdvanceValue;

                            PlayerData[Player].TimeToDropPiece-=5;
                            audio->PlayDigitalSoundFX(8, 0);
                        }
                    }

                    if (PlayerData[Player].Level > 9)
                    {
                        PlayerData[0].PlayerStatus = GameOver;
                        PlayerData[1].PlayerStatus = GameOver;
                        PlayerData[2].PlayerStatus = GameOver;
                        PlayerData[3].PlayerStatus = GameOver;

                        Won = true;
                    }
				}

				audio->PlayDigitalSoundFX(6, 0);
			}
		}
	}

	if (thereWasACompletedLine == false)
	{
		SetupNewPiece();
		PlayerData[Player].PlayerStatus = NewPieceDropping;
	}
}

//-------------------------------------------------------------------------------------------------
void Logic::AddAnAttackLineToEnemiesPlayfield(void)
{
int TEMP_Player = Player;

    for (Player = 0; Player < NumberOfPlayers; Player++)
    {
        if (Player != TEMP_Player && PlayerData[Player].PlayerStatus != GameOver)
        {
            if (PlayerData[Player].PlayerStatus != FlashingCompletedLines && PlayerData[Player].PlayerStatus != ClearingCompletedLines)
            {
                for (int x = 2; x < 12; x++)
                {
                    if (PlayerData[Player].Playfield[x][5] > 10 && PlayerData[Player].Playfield[x][5] < 20)
                    {
                        PlayerData[Player].PlayerStatus = GameOver;
                        return;
                    }
                }

                if (PieceCollisionDown() == CollisionWithPlayfield)  MovePieceDown(true);

                if (PlayerData[Player].PlayerStatus != FlashingCompletedLines
                    && PlayerData[Player].PlayerStatus != ClearingCompletedLines)
                {
                    if (ThinkRussianTimer == 0)  audio->PlayDigitalSoundFX(10, 0);

                    for (int y = 5; y < 23; y++)
                    {
                        for (int x = 2; x < 12; x++)
                        {
                            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y+1];
                        }
                    }

                    for (int x = 2; x < 12; x++)
                        PlayerData[Player].Playfield[x][23] = 0;

                    int attackX = 0;
                    for (int x = 2; x < 12; x++)
                    {
                        PlayerData[Player].Playfield[x][23] = PlayerData[TEMP_Player].AttackLines[attackX][11];
                        attackX++;
                    }
                }
            }
            else
            {
                audio->PlayDigitalSoundFX(14,0);
                BlockAttackTransparency[Player] = 255;
            }
        }
    }

    for (int y = 11; y > 0; y--)
    {
        for (int x = 0; x < 10; x++)
        {
            PlayerData[TEMP_Player].AttackLines[x][y] = PlayerData[TEMP_Player].AttackLines[x][y-1];
        }
    }

    Player = TEMP_Player;
}

//-------------------------------------------------------------------------------------------------
bool Logic::AddAnIncompleteLineToPlayfieldCrisisMode(void)
{
    if (PlayerData[Player].PlayerStatus == FlashingCompletedLines || PlayerData[Player].PlayerStatus == ClearingCompletedLines)
    {
        return(false);
    }

    if (ThinkRussianTimer == 0)  audio->PlayDigitalSoundFX(10, 0);

    for (int x = 2; x < 12; x++)
    {
        if (PlayerData[Player].Playfield[x][5] > 10 && PlayerData[Player].Playfield[x][5] < 20)
        {
            PlayerData[Player].PlayerStatus = GameOver;
            return(true);
        }
    }

    if (PieceCollisionDown() == CollisionWithPlayfield)  MovePieceDown(true);

    for (int y = 5; y < 23; y++)
	    for (int x = 2; x < 12; x++)
            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y+1];

    for (int x = 2; x < 12; x++)
        PlayerData[Player].Playfield[x][23] = 0;

    int boxTotal = 0;
    for (int x = 2; x < 12; x++)
    {
        Uint32 box = rand()%8;
        if (box > 0)  boxTotal++;

        if (boxTotal < 10)
        {
            if (box != 0)  PlayerData[Player].Playfield[x][23] = (int)box+10;
            else  PlayerData[Player].Playfield[x][23] = 0;
        }
        else  PlayerData[Player].Playfield[x][23] = 0;
    }

    return(true);
}

//-------------------------------------------------------------------------------------------------
void Logic::RunTetriGameEngine(void)
{
    if (  (input->KeyOnKeyboardPressedByUser == SDLK_p && input->UserDefinedKeyPause == -1)
       || (input->JoystickPause[Any] == ON)  )
	{
		if (PAUSEgame == false)
        {
            PAUSEgame = true;
            Mix_PauseMusic();
        }
		else
        {
            PAUSEgame = false;
            PAUSEgameQuitJoy = false;
            Mix_ResumeMusic();
        }

        input->DelayAllUserInput = 20;

		audio->PlayDigitalSoundFX(0, 0);
	}

    if (ThinkRussianTimer > 0)  ThinkRussianTimer--;
    if (ThinkRussianTimer == 1)
    {
        audio->PlayMusic(1+SelectedMusicTrack, -1);
        Mix_ResumeMusic();
        ThinkRussianTimer = 0;
    }

    if (GameMode != StoryMode && ThinkRussianTimer == 0 && audio->MusicJukeboxMode == 1 && Crisis7BGMPlayed == false)
    {
        if ( Mix_PlayingMusic() == 0 )
        {
            audio->PlayMusic(audio->PlayingMusicArray[( rand()%audio->PlayingMusicArrayMax )], 0);
        }
    }

	if (PAUSEgame == false)
	{
		for (Player = 0; Player < NumberOfPlayers; Player++)
		{
            if (PlayerData[Player].PlayerStatus != GameOver)
            {
                if (input->DEBUG == 0)  PlayerData[Player].PieceDropTimer++;

                if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == DOWN)
                {
                    PlayerData[Player].PieceDropTimer = 1+PlayerData[Player].TimeToDropPiece;
                }
                else  PlayerData[Player].DropBonus = 0;

                if (PlayerData[Player].PlayerStatus == NewPieceDropping)
                {
                    PlayerData[Player].UPActionTaken = true;
                    PlayerData[Player].DropBonus = 0;

                    if (PlayerData[Player].PiecePlayfieldY < PieceDropStartHeight[ PlayerData[Player].Piece ])
                        MovePieceDown(true);
                    else
                    {
                        if (DisplayNextPiece == 1)  AddPieceToPlayfieldMemory(Next);
                        PlayerData[Player].PlayerStatus = PieceFalling;
                    }
                }
                else if (PlayerData[Player].PlayerStatus == PieceFalling)
                {
                    if (GameMode == TwentyLineChallengeMode && PlayerData[Player].TwentyLineCounter == 0)
                    {
                        PlayerData[Player].PlayerStatus = GameOver;
                    }

                    if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)
                    {
                        MovePieceDown(false);
                    }

                    if (PlayerData[Player].PlayerInput == CPU)
                    {
                        ComputeComputerPlayerMove();
                    }
                    else
                    {
                        if (PressingUPAction == Rotate)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].RotateDirection == 0)
                                {
                                    if (PlayerData[Player].UPActionTaken == false)  RotatePieceCounterClockwise();
                                    PlayerData[Player].UPActionTaken = true;
                                }
                                else if (PlayerData[Player].RotateDirection == 1)
                                {
                                    if (PlayerData[Player].UPActionTaken == false)  RotatePieceClockwise();
                                    PlayerData[Player].UPActionTaken = true;
                                }
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }
                        else if (PressingUPAction == Fall)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].UPActionTaken == false)
                                {
                                    MovePieceDownFast();
                                }

                                PlayerData[Player].UPActionTaken = true;
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }
                        else if (PressingUPAction == DropAndDrag)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] == UP)
                            {
                                if (PlayerData[Player].UPActionTaken == false)
                                {
                                    MovePieceDownFastDropAndDrag();
                                }

                                PlayerData[Player].UPActionTaken = true;
                            }
                            else  PlayerData[Player].UPActionTaken = false;
                        }

                        if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)
                        {
                            if (input->JoystickDirectionVertical[PlayerData[Player].PlayerInput] != DOWN)
                            {
                                if (ThinkRussianTimer == 0) audio->PlayDigitalSoundFX(2, 0);
                                PlayerData[Player].DropBonus = 0;
                            }
                            else  PlayerData[Player].DropBonus++;
                        }

                        if (PlayerData[Player].PlayerInput == Keyboard)
                        {
                            if (input->KeyOnKeyboardPressedByUser == SDLK_SPACE && input->UserDefinedKeyPause == -1)
                                MovePieceDownFast();
                        }

                        if (input->JoystickButtonOne[PlayerData[Player].PlayerInput] == ON)
                        {
                            if (PlayerData[Player].PieceRotated1 == false)
                            {
                                RotatePieceCounterClockwise();
                                PlayerData[Player].PieceRotated1 = true;
                            }
                        }
                        else PlayerData[Player].PieceRotated1 = false;

                        if (input->JoystickButtonTwo[PlayerData[Player].PlayerInput] == ON)
                        {
                            if (PlayerData[Player].PieceRotated2 == false)
                            {
                                RotatePieceClockwise();
                                PlayerData[Player].PieceRotated2 = true;
                            }
                        }
                        else PlayerData[Player].PieceRotated2 = false;

                        if (PlayerData[Player].PlayerInput != Mouse)
                        {
                            if (input->JoystickDirectionHorizontal[PlayerData[Player].PlayerInput] == LEFT)  MovePieceLeft();
                            else if (input->JoystickDirectionHorizontal[PlayerData[Player].PlayerInput] == RIGHT)  MovePieceRight();
                            else  PlayerData[Player].PieceMovementDelay = 0;
                        }
                    }

                    if (GameMode == CrisisMode || GameMode == StoryMode)
                    {
                        for (int x = 0; x < 10; x++)
                        {
                            if (PlayerData[Player].AttackLines[x][11] > 0)
                            {
                                x = 100;
                                AddAnAttackLineToEnemiesPlayfield();
                            }
                        }

                        if ( (Player == 0 && PlayerData[1].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 1 && PlayerData[0].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 2 && PlayerData[0].PlayerStatus == GameOver && PlayerData[1].PlayerStatus == GameOver && PlayerData[3].PlayerStatus == GameOver)
                           ||(Player == 3 && PlayerData[0].PlayerStatus == GameOver && PlayerData[1].PlayerStatus == GameOver && PlayerData[2].PlayerStatus == GameOver) )
                        {
                            if ( GameMode == CrisisMode && CrisisModeOnePlayerLeftPlayfieldCleared == false
                                && PlayersCanJoin == false && PlayerData[Player].PlayerStatus == PieceFalling)
                            {
                                DeletePieceFromPlayfieldMemory(Current);
                                DeletePieceFromPlayfieldMemory(DropShadow);

                            	PlayerData[Player].PlayerStatus = ClearingPlayfield;
                            }

                            if (CrisisModeTimer < 300)  CrisisModeTimer++;
                            else
                            {
                                if (AddAnIncompleteLineToPlayfieldCrisisMode() == true)  CrisisModeTimer = 0;
                            }
                        }
                    }

                    int mousePlayfieldX = -999;
                    int mousePlayfieldY = -999;
                    for (int player = 0; player < NumberOfPlayers; player++)
                    {
                        float boxScreenX = PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
                        float boxScreenY = PlayerData[player].PlayersPlayfieldScreenY-212;

                        for (int y = 0; y < 26; y++)
                        {
                            for (int x = 0; x < 12; x++)
                            {
                                if (PlayerData[player].PlayerInput == Mouse && PlayerData[player].PlayerStatus == PieceFalling)
                                {
                                    if (  input->MouseX >= ( boxScreenX-(13/2) ) && input->MouseX <= ( boxScreenX+(13/2) )
                                    && input->MouseY >= ( boxScreenY-(18/2) ) && input->MouseY <= ( boxScreenY+(18/2) )  )
                                    {
                                        mousePlayfieldX = x-1;
                                        mousePlayfieldY = y;

                                        x = 999; y = 999;
                                    }
                                }

                                boxScreenX+=13;
                            }

                            boxScreenX = PlayerData[player].PlayersPlayfieldScreenX-57-(2*13);
                            boxScreenY+=18;
                        }
                    }

                    if (PlayerData[Player].PlayerInput == Mouse && input->MouseButtonPressed[0] == true)
                    {
                        if (mousePlayfieldY < PlayerData[Player].PiecePlayfieldY)  RotatePieceClockwise();
                        else
                        {
                            if (mousePlayfieldX < PlayerData[Player].PiecePlayfieldX)  MovePieceLeft();
                            else if (mousePlayfieldX > PlayerData[Player].PiecePlayfieldX)  MovePieceRight();
                            else if (mousePlayfieldY > PlayerData[Player].PiecePlayfieldY)  MovePieceDown(false);

                            if (mousePlayfieldX != PlayerData[Player].PiecePlayfieldX || mousePlayfieldY > PlayerData[Player].PiecePlayfieldY)
                                input->MouseButtonWasClicked[0] = false;
                        }
                    }
                }
                else if (PlayerData[Player].PlayerStatus == FlashingCompletedLines)  FlashCompletedLines();
                else if (PlayerData[Player].PlayerStatus == ClearingCompletedLines)  ClearCompletedLines();
                else if (PlayerData[Player].PlayerStatus == ClearingPlayfield)
                {
                    if ( CrisisModeClearPlayfield() == false )
                    {
                        PlayerData[Player].PlayerStatus = PieceFalling;
                        CrisisModeOnePlayerLeftPlayfieldCleared = true;
                    }
                }

                if (PlayerData[Player].PieceDropTimer > PlayerData[Player].TimeToDropPiece)  PlayerData[Player].PieceDropTimer = 0;

                if (GameMode == TimeAttack30Mode || GameMode == TimeAttack60Mode || GameMode == TimeAttack120Mode)
                {
                    TimeAttackTimer--;

                    if (TimeAttackTimer == 0)
                    {
                        PlayerData[0].PlayerStatus = GameOver;
                        PlayerData[1].PlayerStatus = GameOver;
                        PlayerData[2].PlayerStatus = GameOver;
                        PlayerData[3].PlayerStatus = GameOver;
                    }
                }
            }

            if (GameMode == CrisisMode)
            {
                Uint8 playersAlive = 0;
                if (PlayerData[0].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[1].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[2].PlayerStatus != GameOver)  playersAlive++;
                if (PlayerData[3].PlayerStatus != GameOver)  playersAlive++;

                if (playersAlive > 1)  PlayerData[Player].Score = 0;
            }
            else if (GameMode == StoryMode && Player == 1)  CheckForDanger();

            if (BlockAttackTransparency[Player] > 0.0f)  BlockAttackTransparency[Player]-=5;
		}
	}
}

//-------------------------------------------------------------------------------------------------
bool Logic::CrisisModeClearPlayfield(void)
{
bool returnValue = false;

	for (int y = 23; y > 5; y--)
	{
		for (int x = 2; x < 12; x++)
		{
            PlayerData[Player].Playfield[x][y] = PlayerData[Player].Playfield[x][y-1];
		}
    }

    for (int x = 2; x < 12; x++)
    {
        PlayerData[Player].Playfield[x][5] = 0;
    }

	for (int y = 5; y < 24; y++)
	{
		for (int x = 2; x < 12; x++)
		{
            if (PlayerData[Player].Playfield[x][y] != 0)  returnValue = true;
		}
    }

    return(returnValue);
}

//-------------------------------------------------------------------------------------------------
void Logic::ComputeComputerPlayerMove(void)
{
    if (PlayerData[Player].PlayerStatus != PieceFalling)  return;

    if (PlayerData[Player].BestMoveCalculated == false)
    {
        int TEMP_PieceRotation;
        int TEMP_PiecePlayfieldX;
        int TEMP_PiecePlayfieldY;
        int posY;
        int posX;

        DeletePieceFromPlayfieldMemory(Current);
        for (int pieceTestX = (PlayerData[Player].PlayfieldStartX-2); pieceTestX < (PlayerData[Player].PlayfieldEndX-1); pieceTestX+=1)
        {
            for (int rotationTest = 1; rotationTest <= MaxRotationArray[ PlayerData[Player].Piece ]; rotationTest+=1)
            {
                TEMP_PieceRotation = PlayerData[Player].PieceRotation;
                TEMP_PiecePlayfieldX = PlayerData[Player].PiecePlayfieldX;
                TEMP_PiecePlayfieldY = PlayerData[Player].PiecePlayfieldY;

                PlayerData[Player].PiecePlayfieldX = pieceTestX;
                PlayerData[Player].PieceRotation = rotationTest;

                PlayerData[Player].MovePieceCollision[pieceTestX][rotationTest] = false;
                PlayerData[Player].MovePieceHeight[pieceTestX][rotationTest] = 0;
                if (PieceCollision() == CollisionNotTrue)
                {
                    for (posY = PlayerData[Player].PiecePlayfieldY; posY < 23; posY+=1)
                    {
                        PlayerData[Player].PiecePlayfieldY  = posY;
                        if (PieceCollision() != CollisionNotTrue)
                        {
                            PlayerData[Player].PiecePlayfieldY = posY-1;
                            PlayerData[Player].MovePieceHeight[pieceTestX][rotationTest] = PlayerData[Player].PiecePlayfieldY;
                            posY = 100;
                        }
                    }

                    AddPieceToPlayfieldMemory(Current);

                    PlayerData[Player].MoveTrappedHoles[pieceTestX][rotationTest] = 0;
                    for (posX = PlayerData[Player].PlayfieldStartX; posX < PlayerData[Player].PlayfieldEndX; posX+=1)
                    {
                        int numberOfEmpties;
                        numberOfEmpties = 0;
                        for (posY = 23; posY > 4; posY-=1)
                        {
                            if (PlayerData[Player].Playfield[posX][posY] == 0)
                            {
                                numberOfEmpties+=1;
                            }
                            else if (PlayerData[Player].Playfield[posX][posY] > 10 && PlayerData[Player].Playfield[posX][posY] < 20)
                            {
                                PlayerData[Player].MoveTrappedHoles[pieceTestX][rotationTest]+=numberOfEmpties;
                                numberOfEmpties = 0;
                            }
                        }
                    }

                    PlayerData[Player].MoveCompletedLines[pieceTestX][rotationTest] = 0;
                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest] = 0;
                    for (posY = 5; posY < 25; posY+=1)
                    {
                        int boxTotal;
                        boxTotal = 0;
                        for ( posX = (PlayerData[Player].PlayfieldStartX-1); posX < PlayerData[Player].PlayfieldEndX; posX+=1 )
                        {
                            if ( (PlayerData[Player].Playfield[posX][posY] > 10 && PlayerData[Player].Playfield[posX][posY] < 20)
                                || PlayerData[Player].Playfield[posX][posY] == 255 )
                            {
                                if (PlayerData[Player].Playfield[posX][posY] != 255)  boxTotal+=1;

                                if (PlayerData[Player].Playfield[posX][(posY-1)] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[posX][(posY+1)] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[(posX-1)][posY] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;

                                if (PlayerData[Player].Playfield[(posX+1)][posY] == 0)
                                    PlayerData[Player].MovePlayfieldBoxEdges[pieceTestX][rotationTest]+=1;
                            }
                        }

                        if (boxTotal == 10)  PlayerData[Player].MoveCompletedLines[pieceTestX][rotationTest]+=1;
                    }

                    PlayerData[Player].MoveOneBlockCavernHoles[pieceTestX][rotationTest] = 0;
                    for (posY = 5; posY < 24; posY+=1)
                    {
                        for (posX = PlayerData[Player].PlayfieldStartX; posX < PlayerData[Player].PlayfieldEndX; posX+=1)
                        {
                            if (PlayerData[Player].Playfield[posX][posY] == 0
                            && PlayerData[Player].Playfield[(posX-1)][posY] != 0 && PlayerData[Player].Playfield[(posX+1)][posY] != 0)
                                PlayerData[Player].MoveOneBlockCavernHoles[pieceTestX][rotationTest]+=1;
                        }
                    }

                    DeletePieceFromPlayfieldMemory(Current);
                }
                else  PlayerData[Player].MovePieceCollision[pieceTestX][rotationTest] = true;

                PlayerData[Player].PieceRotation = TEMP_PieceRotation;
                PlayerData[Player].PiecePlayfieldX = TEMP_PiecePlayfieldX;
                PlayerData[Player].PiecePlayfieldY = TEMP_PiecePlayfieldY;
            }
        }

        PlayerData[Player].BestMoveX = -1;
        PlayerData[Player].BestRotation = -1;
        float bestValue;
        bestValue = FLT_MAX;
        for (posX = (PlayerData[Player].PlayfieldStartX-1); posX < (PlayerData[Player].PlayfieldEndX-1); posX+=1)
        {
            for (int rot = 1; rot <= MaxRotationArray[ PlayerData[Player].Piece ]; rot+=1)
            {
                if (PlayerData[Player].MovePieceCollision[posX][rot] == false)
                {
                    PlayerData[Player].MovePieceHeight[posX][rot]+=PlayerData[Player].MoveCompletedLines[posX][rot];

                    float testValue;
                    /* -- JeZxLee's ["Gift Of Sight" Tetri A.I. Algorithm ~691,000+]--------------------------------------- */
                    testValue = ( (3*PlayerData[Player].MoveTrappedHoles[posX][rot])
                                +(1*PlayerData[Player].MoveOneBlockCavernHoles[posX][rot])
                                +(1*PlayerData[Player].MovePlayfieldBoxEdges[posX][rot])
                                -(1*PlayerData[Player].MovePieceHeight[posX][rot]) );
                    /* --------------------------------------- JeZxLee's ["Gift Of Sight" Tetri A.I. Algorithm ~691,000+]-- */

                    if (testValue <= bestValue)
                    {
                        bestValue = testValue;
                        PlayerData[Player].BestMoveX = posX;
                        PlayerData[Player].BestRotation = rot;
                    }
                }
            }
        }
    }

    if (PlayerData[Player].MovedToBestMove == false && PlayerData[Player].BestMoveX != -1 && PlayerData[Player].BestRotation != -1)
    {
        if (PlayerData[Player].PieceRotation < PlayerData[Player].BestRotation)  RotatePieceClockwise();
        else if (PlayerData[Player].PieceRotation > PlayerData[Player].BestRotation)  RotatePieceCounterClockwise();

        if (PlayerData[Player].BestMoveX < PlayerData[Player].PiecePlayfieldX)
            MovePieceLeft();
        else if (PlayerData[Player].BestMoveX > PlayerData[Player].PiecePlayfieldX)
            MovePieceRight();
        else if (PlayerData[Player].PieceRotation == PlayerData[Player].BestRotation)
        {
            if (CPUPlayerEnabled != 4)  MovePieceDown(true);
            PlayerData[Player].MovedToBestMove = true;
        }
    }
    else
    {
        if (CPUPlayerEnabled != 4)  MovePieceDown(false);
        else  MovePieceDownFast();
    }
}

//-------------------------------------------------------------------------------------------------
