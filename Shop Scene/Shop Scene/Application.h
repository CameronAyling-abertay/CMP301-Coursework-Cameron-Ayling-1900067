#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "DXF.h"

class Application : public BaseApplication
{
public:

	Application();
	~Application();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void renderGui();

};

#endif