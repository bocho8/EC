#include "cs2game.h"

namespace cs2
{
	namespace bhop
	{
		BOOL onshot = 1;					//do something once untill being reset
	}
}
namespace client
{
	extern void key_down(BYTE virtual_key_code);
	extern void key_up(BYTE virtual_key_code);
}
//UM version (uses windows api calls)
void cs2::bhop::run(void)
{
	if (!bhop_enabled)
	{
		return;
	}

	if (cs2::input::is_button_down(66))
	{
		//check if player is on ground
		if ((cs2::player::get_flags(local_player_gb) & 1) || (cs2::player::get_MoveType(local_player_gb) == 2313))
		{
			if (onshot)
			{
				client::key_down(VK_NUMLOCK);			// vm::write(game_handle, (clientdll_base + jump_offset), &jump, sizeof(jump)); // write +jump
				onshot = 0;
			}
			else if (!onshot)
			{
				client::key_up(VK_NUMLOCK);				// vm::write(game_handle, (clientdll_base + jump_offset), &dontjump, sizeof(dontjump)); // write -jump
				onshot = 1;
			}
			Sleep(18);
		}
		else
		{
			if (!onshot)
			{
				Sleep(24);
				client::key_up(VK_NUMLOCK);				// vm::write(game_handle, (clientdll_base + jump_offset), &dontjump, sizeof(dontjump)); // write -jump
				onshot = 1;
			}
		}
	}
	else if (!cs2::input::is_button_down(66) && !onshot)
	{
		Sleep(24);
		client::key_up(VK_NUMLOCK);				// vm::write(game_handle, (clientdll_base + jump_offset), &dontjump, sizeof(dontjump)); // write -jump
		onshot = 1;
	}
}

void cs2::bhop::reset(void)
{
	onshot = 1;
}
