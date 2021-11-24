#include "Application.h"

Application::Application()
{

}

void Application::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	//Run the BaseApplication init
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Do initialisation necessary for this program
}

Application::~Application()
{
	//Deconstruct the application
	BaseApplication::~BaseApplication();

	//Delete all pointers
}

bool Application::frame()
{
	//Showing whether the update ran smoothly
	bool result;

	//Run the BaseApplication frame
	result = BaseApplication::frame();
	if (!result) {
		return false;
	}

	//Do program specific updates

	//Render the graphics
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Application::render()
{
	//Define the necessary matrices
	XMMATRIX worldMat, viewMat, projectionMat;

	//Clear the scene
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//Update the camera
	camera->update();

	//Get the necessary matrices
	worldMat = renderer->getWorldMatrix();
	viewMat = camera->getViewMatrix();
	projectionMat = renderer->getProjectionMatrix();

	//Render geometry in the following order:
	//Send geometry data
	//Set shader parameters
	//Render the object

	//Render GUI
	renderGui();

	//End drawing to the buffer
	renderer->endScene();

	return true;
}

void Application::renderGui()
{
	//Turn off shader stages
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	//Build the GUI

	//Render the GUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}