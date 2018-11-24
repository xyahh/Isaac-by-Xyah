#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>
#include <iostream>
#include <float.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "Math.h"

//#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION

enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

/* Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f

#define	STD		std::

constexpr float UPDATE_FREQUENCY	{ 120.f };
constexpr float UPDATE_TIME			{ 1.f / UPDATE_FREQUENCY };

using id_type = STD string;

template<class T>
inline u_int LastIdx(STD vector<T> v)
{
	return static_cast<u_int>(v.size() - 1);
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
class ActorGraphics;
class VisualGraphics;
class Graphics;
class Physics;
class Input;

namespace FMOD
{
	class Sound;
	class DSP;
	class System;
	class Channel;
}