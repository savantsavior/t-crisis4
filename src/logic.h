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

#ifndef LOGIC
#define LOGIC

class Logic
{
public:

    bool TestMode = false;

    int StoryLevelAdvanceCounter;
    int StoryLevelAdvanceValue;

    int StoryShown[10];

    #define OriginalMode                0
    #define TimeAttack30Mode            1
    #define TimeAttack60Mode            2
    #define TimeAttack120Mode           3
    #define TwentyLineChallengeMode     4
    #define CrisisMode                  5
    #define StoryMode                   6
    Uint8 GameMode;

    Uint8 MaxRotationArray[8];
    Uint8 PieceDropStartHeight[8];

    Uint8 CPUPlayerEnabled;

    Uint8 SelectedBackground;
    Uint8 SelectedMusicTrack;
    Uint8 NewGameGarbageHeight;

	Uint8 PieceData[8][5][17];

	Uint8 Player;
    #define NumberOfPlayers     4
    struct PlayData
    {
        int Playfield[15][26];
        int PlayfieldBackup[15][26];
        int PlayfieldAI[15][26];

        int PlayfieldStartX;
        int PlayfieldEndX;

        Uint8 Piece;

        int PieceBagIndex;
        int PieceBag[2][8];
        bool PieceSelectedAlready[8];

        Sint8 PieceMovementDelay;
        Uint8 PieceRotation;
        Uint8 PiecePlayfieldX;
        Uint8 PiecePlayfieldY;
        Uint8 NextPiece;

        #define GameOver					-1
        #define NewPieceDropping			0
        #define PieceFalling				1
        #define FlashingCompletedLines		2
        #define ClearingCompletedLines		3
        #define ClearingPlayfield           4
        Sint16 PlayerStatus;

        float PlayersPlayfieldScreenX;
        float PlayersPlayfieldScreenY;
        int PieceDropTimer;
        int TimeToDropPiece;
        bool PieceRotated1;
        bool PieceRotated2;

        bool MovePieceCollision[15][5];

        float MovePieceHeight[15][5];
        float MoveTrappedHoles[15][5];
        float MoveOneBlockCavernHoles[15][5];
        float MovePlayfieldBoxEdges[15][5];
        float MoveCompletedLines[15][5];

        int MoveSetup4Line[15][5];
        int MovePlayfieldTop[15][5];
        int MovePlayfieldTotalHeight[15][5];
        int MovePlayfieldBumbs[15][5];
        int Stress;
        int LocationOfPossible4Line;
        int MoveTrappedHolesBefore[15][5];

        int BestMoveX;
        int BestRotation;
        bool MovedToBestMove;
        bool BestMoveCalculated;

        bool UPActionTaken;
        Uint8 RotateDirection;

        int PlayerInput;

        Uint8 FlashCompletedLinesTimer;
        Uint8 ClearCompletedLinesTimer;

    	Uint8 AttackLines[10][12];

        Uint8 TwentyLineCounter;

        int SkipAFrameForCPUPieceDrop;
        int CPUFrame;

        Uint64 Score;
        Uint32 DropBonus;
        Uint32 Level;
        Uint32 Lines;

    } PlayerData[NumberOfPlayers];

    float ValMovePieceHeight;
    float ValMoveTrappedHoles;
    float ValMoveOneBlockCavernHoles;
    float ValMovePlayfieldBoxEdges;
    float ValMoveCompletedLines;

    float BestMovePieceHeight;
    float BestMoveTrappedHoles;
    float BestMoveOneBlockCavernHoles;
    float BestMovePlayfieldBoxEdges;
    float BestMoveCompletedLines;
    int BestTotalLinesPerGame;

    float Multiplier;
    int MultiplierSelected;

    bool CrisisModeOnePlayerLeftPlayfieldCleared;

    Uint32 TotalCPUPlayerLines;

    Uint32 NumberofCPUGames;
    Uint32 TotalOneLines;
    Uint32 TotalTwoLines;
    Uint32 TotalThreeLines;
    Uint32 TotalFourLines;

    Uint32 PlayingGameFrameLock;

    Uint8 DisplayNextPiece;
    Uint8 DisplayDropShadow;
    #define None            0
    #define Fall            1
    #define Rotate          2
    #define DropAndDrag     3
    Uint8 PressingUPAction;

    Uint8 DelayAutoShift;

    bool PlayersCanJoin;

    Uint32 TimeAttackTimer;

    Uint16 ThinkRussianTimer;

    Uint16 CrisisModeTimer;

    Uint8 BlockAttackTransparency[NumberOfPlayers];

    bool Crisis7BGMPlayed;
    bool Won;

    bool GameWasJustPlayed;

	bool PAUSEgame;
    bool PAUSEgameQuitJoy;

	bool GameForfeit;

    int GameOverTimer;

    int JoinInTimer;
	int ContinueWatchingTimer;

    bool HumanStillAlive;
    int AllHumansDeadExitTimer;

    Uint8 TileSet;

	Logic(void);
	virtual ~Logic(void);

	void InitializePieceData(void);

	void ClearPlayfieldsWithCollisionDetection(void);

    void FillPieceBag(int player);

    #define CollisionNotTrue            0
    #define CollisionWithPlayfield      1
	int PieceCollision(void);
	int PieceCollisionDown(void);
	int PieceCollisionLeft(void);
	int PieceCollisionRight(void);

    int DangerRepeat;

	#define Current		    0
	#define Next		    1
	#define DropShadow	    2
    #define Temp            3
	void AddPieceToPlayfieldMemory(int TempOrCurrentOrNextOrDropShadow);
	void DeletePieceFromPlayfieldMemory(int CurrentOrDropShadow);

    void SetupNewPiece(void);

	void CheckForCompletedLines(void);

    void CheckForDanger(void);

	void MovePieceDown(bool Force);
    void MovePieceDownFast(void);
    void MovePieceDownFastDropAndDrag(void);
	bool RotatePieceCounterClockwise(void);
	bool RotatePieceClockwise(void);
	void MovePieceLeft(void);
	void MovePieceRight(void);

	void SetupForNewGame(void);
    void SetupForNewLevelStory(void);

	void FlashCompletedLines(void);
	void ClearCompletedLines(void);

	void AddAnAttackLineToEnemiesPlayfield(void);

    bool AddAnIncompleteLineToPlayfieldCrisisMode(void);

	void RunTetriGameEngine(void);

    bool CrisisModeClearPlayfield(void);

    void ComputeComputerPlayerMove(void);
};

#endif
