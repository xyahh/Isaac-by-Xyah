#include "stdafx.h"
#include "Framework.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"

int main(int argc, char **argv)
{	
	/* Scale Set (0.01m : 1 px) */
	World::SetScale(0.025f, 1);
	
	/* Game Loop */
	Fw.Initialize("The Binding of Isaac", 800, 800, argc, argv);
	Fw.ToScene(new Gameplay);
	Fw.Run();
	Fw.Close();

    return FALSE;
}

