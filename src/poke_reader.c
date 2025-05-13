#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "field_control_avatar.h"
#include "task.h"
#include "text_window.h"
#include "text.h"
#include "window.h"

// Structure pour stocker l'état du PokeReader
struct PokeReaderState {
    bool8 isPaused;
    bool8 frameAdvance;
    bool8 showInfo;
    u8 infoWindowId;
    u32 frameCount;
};

static struct PokeReaderState sPokeReaderState = {0};

// Fonction pour gérer les entrées du PokeReader
void HandlePokeReaderInput(void)
{
    // Vérifie si Select + L sont pressés pour mettre en pause
    if (JOY_NEW(SELECT_BUTTON) && JOY_HELD(L_BUTTON))
    {
        sPokeReaderState.isPaused = TRUE;
        sPokeReaderState.frameAdvance = FALSE;
    }

    // Vérifie si Select + Bas sont pressés pour afficher/masquer les infos
    if (JOY_NEW(SELECT_BUTTON) && JOY_HELD(DOWN_BUTTON))
    {
        sPokeReaderState.showInfo = !sPokeReaderState.showInfo;
        if (sPokeReaderState.showInfo)
        {
            // Créer la fenêtre d'info
            struct WindowTemplate template = {
                .bg = 0,
                .tilemapLeft = 20,
                .tilemapTop = 1,
                .width = 10,
                .height = 4,
                .paletteNum = 15,
                .baseBlock = 0x1
            };
            sPokeReaderState.infoWindowId = AddWindow(&template);
            FillWindowPixelBuffer(sPokeReaderState.infoWindowId, PIXEL_FILL(1));
            PutWindowTilemap(sPokeReaderState.infoWindowId);
        }
        else
        {
            // Supprimer la fenêtre d'info
            RemoveWindow(sPokeReaderState.infoWindowId);
        }
    }

    // Si le jeu est en pause
    if (sPokeReaderState.isPaused)
    {
        // R pour avancer d'une frame
        if (JOY_NEW(R_BUTTON))
        {
            sPokeReaderState.frameAdvance = TRUE;
            sPokeReaderState.frameCount++;
        }
        // A pour reprendre le jeu
        else if (JOY_NEW(A_BUTTON))
        {
            sPokeReaderState.isPaused = FALSE;
            sPokeReaderState.frameAdvance = FALSE;
        }
    }
    else
    {
        // Incrémenter le compteur de frames si le jeu n'est pas en pause
        sPokeReaderState.frameCount++;
    }

    // Mettre à jour l'affichage des infos si la fenêtre est visible
    if (sPokeReaderState.showInfo)
    {
        u8 text[32];
        FillWindowPixelBuffer(sPokeReaderState.infoWindowId, PIXEL_FILL(1));
        ConvertIntToDecimalStringN(text, gRngValue, STR_CONV_MODE_LEADING_ZEROS, 8);
        AddTextPrinterParameterized(sPokeReaderState.infoWindowId, FONT_NORMAL, text, 0, 0, TEXT_SKIP_DRAW, NULL);
        ConvertIntToDecimalStringN(text, sPokeReaderState.frameCount, STR_CONV_MODE_LEADING_ZEROS, 8);
        AddTextPrinterParameterized(sPokeReaderState.infoWindowId, FONT_NORMAL, text, 0, 16, TEXT_SKIP_DRAW, NULL);
        CopyWindowToVram(sPokeReaderState.infoWindowId, COPYWIN_GFX);
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
    sPokeReaderState.frameCount = 0;
    if (sPokeReaderState.infoWindowId != 0xFF)
    {
        RemoveWindow(sPokeReaderState.infoWindowId);
        sPokeReaderState.infoWindowId = 0xFF;
    }
} 