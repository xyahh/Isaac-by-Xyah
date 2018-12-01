#pragma once
namespace TEX
{
	extern size_t BASIC_BODY;
	extern size_t ISAAC_HEAD;
	extern size_t EXPLOSION;
}


namespace ST
{
	extern size_t IDLE;
	extern size_t MOVE;
	extern size_t CHARGE_JUMP;
	extern size_t IN_AIR;
	extern size_t CHARGE_SLAM;
	extern size_t SLAM;
}

namespace CMD
{
	extern size_t MOVE_UP;
	extern size_t MOVE_DOWN;
	extern size_t MOVE_LEFT;
	extern size_t MOVE_RIGHT;

	extern size_t SLOW_MOVE_UP;
	extern size_t SLOW_MOVE_DOWN;
	extern size_t SLOW_MOVE_LEFT;
	extern size_t SLOW_MOVE_RIGHT;


	extern size_t CHARGE_JUMP;

	extern size_t START_MOVE;
	extern size_t START_IN_AIR;
	extern size_t START_CHARGE_SLAM;
}

namespace OBJ
{
	extern size_t PLAYER;

	namespace SPRITE
	{
		extern size_t BODY;
		extern size_t HEAD;
	}
}
