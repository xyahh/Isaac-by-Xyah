#pragma once
#include "targetver.h"

#include <tchar.h>
#include <Windows.h>
#include <iostream>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>

#include "Math.h"
#include "Indices.h"

#define	STD		std::

#define CYAN_DEBUG_CONSOLE
//#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION

/* For the Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f


constexpr float UPDATE_FREQUENCY	{ 60.f };
constexpr float UPDATE_TIME			{ 1.f / UPDATE_FREQUENCY };

template<class T>
inline size_t Last(const STD vector<T>& v)
{
	return v.size() - 1;
}

/* Core */
class Window;
class Scene;
class Cyan;

class State;
class Command;
class Gamepad;
class Renderer;

/* Components */
class Graphics;
class Physics;
class Input;
class Sprite;

namespace FMOD
{
	class Sound;
	class DSP;
	class System;
	class Channel;
}