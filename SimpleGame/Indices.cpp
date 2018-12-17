#include "stdafx.h"
#include "Indices.h"

namespace TEX
{
	size_t BASIC_BODY;
	size_t ISAAC_HEAD;
	size_t EXPLOSION;
	size_t TEAR;
	size_t DEPTHS;
}


namespace ST
{
	size_t IDLE;
	size_t MOVE;
	size_t CHARGE_JUMP;
	size_t IN_AIR;
	size_t CHARGE_SLAM;
	size_t SLAM;
	size_t SHOOT;
	size_t DAMAGED;
}

namespace CMD
{
	size_t MOVE_UP;
	size_t MOVE_DOWN;
	size_t MOVE_LEFT;
	size_t MOVE_RIGHT;

	size_t SLOW_MOVE_UP;
	size_t SLOW_MOVE_DOWN;
	size_t SLOW_MOVE_LEFT;
	size_t SLOW_MOVE_RIGHT;

	size_t START_CHARGE_JUMP;

	size_t START_MOVE;
	size_t START_IN_AIR;
	size_t START_CHARGE_SLAM;
	size_t START_SHOOT;
	size_t END_SHOOT;

	size_t HEAD_FACE_UP;
	size_t HEAD_FACE_DOWN;
	size_t HEAD_FACE_RIGHT;
	size_t HEAD_FACE_LEFT;

	size_t BODY_FACE_UP;
	size_t BODY_FACE_DOWN;
	size_t BODY_FACE_RIGHT;
	size_t BODY_FACE_LEFT;

}

namespace OBJ
{
	size_t PLAYER;
	namespace SPRITE
	{
		size_t SHADOW;
		size_t BODY;
		size_t HEAD;
	}
}
