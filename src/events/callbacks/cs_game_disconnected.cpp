#include "../events.h"

void events::cs_game_disconnected(i_game_event* event)
{
	world->UndoNightMode(true);
}