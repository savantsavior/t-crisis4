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

#ifndef VISUALS
#define VISUALS

class Visuals
{
public:

	Visuals(void);
	virtual ~Visuals(void);

    bool CoreFailure;

    bool ForceAspectRatio;

    int FullScreenMode;

    SDL_Window *Window;
    int WindowWidthCurrent;
    int WindowHeightCurrent;
    SDL_Renderer *Renderer;

    Uint32 FrameLock;
    Uint32 SystemTicks;
    Uint32 NextFrameTicks;
    Uint32 NumberOfFrames;
    Uint8 CurrentFramePerSecond;
    Uint32 FramesPerSecondArray[10];
    Uint32 NextSecondTick;
    Uint32 AverageFPS;

    void CalculateFramerate(void);
    void ProcessFramerate(void);

    bool InitializeWindow(void);

    void ClearScreenBufferWithColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

    int TotalNumberOfLoadedStaffTexts;

    #define NumberOfSprites     1300
    struct Sprite
    {
        SDL_Texture *Texture;
        float ScreenX;
        float ScreenY;
        float ScaleX;
        float ScaleY;
        double RotationDegree;
        Uint8 RedHue;
        Uint8 GreenHue;
        Uint8 BlueHue;
        Uint8 Transparency;
        bool Smooth;
        bool FlipX;
        bool FlipY;
        int TextureWidthOriginal;
        int TextureHeightOriginal;
        int AnimationTimer;
    } Sprites[NumberOfSprites];

    bool LoadSpritesAndInitialize(void);

    bool LoadBoxSpriteSheetIntoMemory(void);

    bool LoadInterfaceIntoMemory(void);

    bool LoadStaffTextIntoMemory(const char *staffText, Uint8 textBlue);
    bool PreloadStaffTextsIntoMemory(void);

    void DrawSpriteOntoScreenBuffer(Uint16 index);

    #define JustifyLeft             0
    #define JustifyCenter           1
    #define JustifyRight            2
    #define JustifyCenterOnPoint    3

    #define NumberOfFonts           10
    TTF_Font *Font[NumberOfFonts];
    char VariableText[64];

    int TextCacheCurrentIndex;
    #define NumberOfTextsCached     250
    SDL_Texture *TextTexture[NumberOfTextsCached];
    SDL_Texture *TextOutlineTexture[NumberOfTextsCached];
    char TextCachedText[NumberOfTextsCached][64];
    float TextCachedScreenX[NumberOfTextsCached];
    float TextCachedScreenY[NumberOfTextsCached];
    int TextCachedWidth[NumberOfTextsCached];
    int TextCachedHeight[NumberOfTextsCached];
    Uint8 TextCacheRed[NumberOfTextsCached];
    Uint8 TextCacheGreen[NumberOfTextsCached];
    Uint8 TextCacheBlue[NumberOfTextsCached];

    bool LoadFontsIntoMemory(void);
    void UnloadFontsFromMemory(void);

    void ClearTextCache(void);
    void DrawTextOntoScreenBuffer(const char *textToDisplay, TTF_Font *font, float posX, float posY
                                 , Uint8 XJustification, Uint8 textRed, Uint8 textGreen, Uint8 textBlue
                                 , Uint8 outlineRed, Uint8 outlineGreen, Uint8 outlineBlue);
};

#endif
