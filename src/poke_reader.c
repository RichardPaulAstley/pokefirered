#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "field_control_avatar.h"
#include "task.h"
#include "text_window.h"
#include "text.h"
#include "window.h"
#include "random.h"
#include "poke_reader.h"

// Structure pour stocker l'état du PokeReader
struct PokeReaderState {
    bool8 isPaused;
    bool8 frameAdvance;
    bool8 showInfo;
    bool8 afterTitleScreen;
    u8 infoWindowId;
    u32 frameCount;
    u32 initialSeed;
};

static struct PokeReaderState sPokeReaderState = {0};

// Fonction pour gérer les entrées du PokeReader
void HandlePokeReaderInput(void)
{
    if (JOY_HELD(SELECT_BUTTON | L_BUTTON))
    {
        if (!sPokeReaderState.isPaused)
        {
            sPokeReaderState.isPaused = TRUE;
            sPokeReaderState.frameAdvance = FALSE;
        }
    }
    else if (sPokeReaderState.isPaused)
    {
        if (JOY_NEW(R_BUTTON))
        {
            sPokeReaderState.frameAdvance = TRUE;
        }
        else if (JOY_NEW(A_BUTTON))
        {
            sPokeReaderState.isPaused = FALSE;
            sPokeReaderState.frameAdvance = FALSE;
        }
    }

    if (JOY_HELD(SELECT_BUTTON | DPAD_DOWN))
    {
        if (!sPokeReaderState.showInfo)
        {
            sPokeReaderState.showInfo = TRUE;
            sPokeReaderState.infoWindowId = AddWindow(&sWindowTemplate_PokeReader);
            PutWindowTilemap(sPokeReaderState.infoWindowId);
            CopyWindowToVram(sPokeReaderState.infoWindowId, COPYWIN_MAP);
        }
    }
    else if (sPokeReaderState.showInfo)
    {
        RemoveWindow(sPokeReaderState.infoWindowId);
        sPokeReaderState.showInfo = FALSE;
    }

    if (sPokeReaderState.showInfo)
    {
        u8 text[32];
        if (sPokeReaderState.afterTitleScreen)
        {
            ConvertIntToDecimalStringN(text, sPokeReaderState.initialSeed, STR_CONV_MODE_LEADING_ZEROS, 8);
            AddTextPrinterParameterized(sPokeReaderState.infoWindowId, FONT_NORMAL, text, 0, 0, 0, NULL);
            ConvertIntToDecimalStringN(text, sPokeReaderState.frameCount, STR_CONV_MODE_LEADING_ZEROS, 8);
            AddTextPrinterParameterized(sPokeReaderState.infoWindowId, FONT_NORMAL, text, 0, 16, 0, NULL);
        }
        else
        {
            AddTextPrinterParameterized(sPokeReaderState.infoWindowId, FONT_NORMAL, gText_NotAfterTitle, 0, 0, 0, NULL);
        }
        CopyWindowToVram(sPokeReaderState.infoWindowId, COPYWIN_BOTH);
    }
}

// Fonction pour vérifier si le jeu doit être mis en pause
bool8 ShouldPauseGame(void)
{
    return sPokeReaderState.isPaused && !sPokeReaderState.frameAdvance;
}

// Fonction pour réinitialiser l'état du PokeReader
void ResetPokeReaderState(void)
{
    sPokeReaderState.isPaused = FALSE;
    sPokeReaderState.frameAdvance = FALSE;
    sPokeReaderState.showInfo = FALSE;
    sPokeReaderState.afterTitleScreen = FALSE;
    sPokeReaderState.frameCount = 0;
    sPokeReaderState.initialSeed = 0;
    if (sPokeReaderState.infoWindowId != 0xFF)
    {
        RemoveWindow(sPokeReaderState.infoWindowId);
        sPokeReaderState.infoWindowId = 0xFF;
    }
}

void UpdatePokeReaderFrame(void)
{
    if (!sPokeReaderState.isPaused || sPokeReaderState.frameAdvance)
    {
        sPokeReaderState.frameCount++;
        sPokeReaderState.frameAdvance = FALSE;
    }
}

void SetPokeReaderAfterTitleScreen(void)
{
    sPokeReaderState.afterTitleScreen = TRUE;
    sPokeReaderState.initialSeed = gRngValue;
    sPokeReaderState.frameCount = 0;
} 