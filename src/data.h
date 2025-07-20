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

#ifndef DATA
#define DATA

class Data
{
public:

	Data(void);
	virtual ~Data(void);

	char DataVersionName[110];

    char HighScoresName[7][10][20];
    Uint8 HighScoresLevel[7][10];
    Uint64 HighScoresScore[7][10];

    bool NewHighScoreRegistered;

    int NameInputArayIndex;

    Uint8 PlayerWithHighestScore;
    Uint8 PlayerRankOnGameOver;

    int NameInputJoyCharX;
    int NameInputJoyCharY;
    char NameInputJoyChar;

    void CheckForNewHighScore(void);

    void ClearHighScores(void);

    void LoadHighScoresAndOptions(void);
    void SaveHighScoresAndOptions(void);

};

#endif
