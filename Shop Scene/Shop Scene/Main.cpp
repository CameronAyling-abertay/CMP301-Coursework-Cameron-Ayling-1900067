#include "System.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Application* app = new Application();
	System* sys;

	//Create the system
	sys = new System(app, 1200, 675, true, false);

	//Run the system
	sys->run();

	//Release the system
	delete sys;
	sys = 0;

	return 0;
}