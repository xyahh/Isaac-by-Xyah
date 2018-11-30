#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>
#include <iostream>
#include <float.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>

#include "Math.h"

#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION


/* Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f

#define	STD		std::

constexpr float UPDATE_FREQUENCY	{ 120.f };
constexpr float UPDATE_TIME			{ 1.f / UPDATE_FREQUENCY };

template<class T>
inline u_int Last(STD vector<T> v)
{
	return static_cast<u_int>(v.size() - 1);
}

template<class T>
void SafeDelete(T*& Object)
{
	delete Object;
	Object = NULL;
}

/* Core */
class Framework;
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