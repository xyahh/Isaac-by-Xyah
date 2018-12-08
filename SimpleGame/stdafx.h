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
#include <queue>
#include <set>
#include <map>

#include "Math.h"
#include "Indices.h"

#define	STD		std::

#define CYAN_DEBUG_CONSOLE
#define CYAN_DEBUG_STATES
#define CYAN_DEBUG_COLLISION

/* For the Depth Test */
#define FARTHEST 1.f
#define NEAREST -1.f

constexpr float UPDATE_FREQUENCY	{ 120.f };
constexpr float UPDATE_TIME			{ 1.f / UPDATE_FREQUENCY };

template<class T>
inline size_t Last(const STD vector<T>& v)
{
	return v.size() - 1;
}

template<class T>
inline void EraseByValue(STD vector<T>& v, const T& value)
{
	size_t Count = 0;
	v.erase(STD remove(v.begin(), v.end(), value), v.end());
}


template<class T>
inline void EraseByIndex(STD vector<T>& v, size_t Index)
{
	size_t Count = 0;
	v.erase(STD remove_if(v.begin(), v.end(), [&](const T& value)
	{
		return Count++ == Index;
	}), v.end());
}



/* Core */
class Window;
class Scene;
class Cyan;
class Timer;

/* Components */
class Graphics;
class Physics;
class Input;
class Sprite;

class State;
class Command;
class Gamepad;
class Renderer;

class EventDispatcher;
class Observer;

namespace FMOD
{
	class Sound;
	class DSP;
	class System;
	class Channel;
}