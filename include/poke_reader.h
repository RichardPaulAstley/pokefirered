#ifndef GUARD_POKE_READER_H
#define GUARD_POKE_READER_H

#include "global.h"

void HandlePokeReaderInput(void);
bool8 ShouldPauseGame(void);
void ResetPokeReaderState(void);
void UpdatePokeReaderFrame(void);
void SetPokeReaderAfterTitleScreen(void);

#endif // GUARD_POKE_READER_H 