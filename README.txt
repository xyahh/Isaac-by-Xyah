MOVEMENT: W, A, S, D
SHOOTING: Direction Keys
JUMP: Space (Hold to Jump Higher)
SLAM: Press Space while in Air

To Enable Command Console (for Debugging):
1) Go to "SimpleGame.cpp" and change
		Engine.Init("Binding of Isaac", 800, 800);
	to
		Engine.Init("Binding of Isaac", 800, 800, "DevConsole");
2) To debug STATES, go to "stdafx.h" and uncomment
		#define CYAN_DEBUG_STATES

NOTE:
- Window Resizing is supported. 