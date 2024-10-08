#ifndef CS2GAME_H
#define CS2GAME_H

#ifdef _KERNEL_MODE
extern "C" int _fltused;
#endif

#include "cs2.h"

//
// features.cpp
//
namespace cs2
{
	extern QWORD local_player_gb;
	extern BOOL bhop_enabled;

	namespace features
	{
		void run(void);
		void reset(void);
	}
	namespace bhop
	{
		void run(void);
		void reset(void);
	}
}

//
// implemented by application/driver
//
namespace client
{
	extern void mouse_move(int x, int y);
	extern void mouse1_down(void);
	extern void mouse1_up(void);
	extern void DrawRect(void *hwnd, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
	extern void DrawFillRect(void *hwnd, int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
	extern void DrawText(void *hwnd, int x, int y, wchar_t* text);
}

namespace cs2
{
	inline void run(void)
	{
		if (cs2::running())
		{
			features::run();
			bhop::run();
		}
		else
		{
			features::reset();
		}
	}
}


#endif // CS2GAME_H

