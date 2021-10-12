#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <string>
#include "SceneManager.h"
#include "Debug.h"

  
int main(int argc, char* args[]) {
	static_assert(sizeof(void*) == 4, "This program is not ready for 64-bit build");
	std::string name = { "Graphics Game Engine" };
	Debug::DebugInit(name + "_Log");
	Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);
	
	SceneManager* gsm = new SceneManager();
	if (gsm->Initialize(name, 1280, 720) ==  true) {
		gsm->Run();
	} 
	delete gsm;
	
	_CrtDumpMemoryLeaks();
	exit(0);

}