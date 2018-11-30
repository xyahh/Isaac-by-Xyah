#pragma once
namespace ST
{
	extern size_t IDLE;
	extern size_t MOVING;
	extern size_t JUMPING;
}

namespace CMD
{
	extern size_t MOVE_UP;
	extern size_t MOVE_DOWN;
	extern size_t MOVE_LEFT;
	extern size_t MOVE_RIGHT;
	extern size_t JUMP;
	extern size_t START_MOVING;
	extern size_t START_JUMPING;
}

namespace OBJ
{
	extern size_t PLAYER;

	namespace PERSON
	{
		extern size_t BODY;
		extern size_t HEAD;
	}
}
