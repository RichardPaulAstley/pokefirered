#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "field_control_avatar.h"
#include "task.h"

// Structure pour stocker l'état du PokeReader
struct PokeReaderState {
    bool8 isPaused;
    bool8 frameAdvance;
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

    // Si le jeu est en pause
    if (sPokeReaderState.isPaused)
    {
        // R pour avancer d'une frame
        if (JOY_NEW(R_BUTTON))
        {
            sPokeReaderState.frameAdvance = TRUE;
        }
        // A pour reprendre le jeu
        else if (JOY_NEW(A_BUTTON))
        {
            sPokeReaderState.isPaused = FALSE;
            sPokeReaderState.frameAdvance = FALSE;
        }
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
} 