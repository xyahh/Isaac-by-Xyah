#pragma once
namespace TEX
{
	extern size_t BASIC_BODY;
	extern size_t ISAAC_HEAD;
	extern size_t EXPLOSION;
	extern size_t TEAR;
	extern size_t DEPTHS;
	extern size_t SHADOW;
}


namespace ST
{
	extern size_t IDLE;
	extern size_t MOVE;
	extern size_t CHARGE_JUMP;
	extern size_t IN_AIR;
	extern size_t CHARGE_SLAM;
	extern size_t SLAM;

	extern size_t SHOOT;
	extern size_t DAMAGED;
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

	extern size_t START_CHARGE_JUMP;

	extern size_t START_MOVE;
	extern size_t START_IN_AIR;
	extern size_t START_CHARGE_SLAM;
	extern size_t START_SHOOT;
	extern size_t END_SHOOT;

	extern size_t HEAD_FACE_UP;
	extern size_t HEAD_FACE_DOWN;
	extern size_t HEAD_FACE_RIGHT;
	extern size_t HEAD_FACE_LEFT;

	extern size_t BODY_FACE_UP;
	extern size_t BODY_FACE_DOWN;
	extern size_t BODY_FACE_RIGHT;
	extern size_t BODY_FACE_LEFT;
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
