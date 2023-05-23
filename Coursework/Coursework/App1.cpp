// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"wood", L"res/wood.png");
	textureMgr->loadTexture(L"blue", L"res/colour.png");
	textureMgr->loadTexture(L"moss", L"res/moss.jpg");

	// Initalise scene variables.
	// Walls
	lightShader = new LightShader(renderer->getDevice(), hwnd);
	staticDepthShader = new StaticDepthShader(renderer->getDevice(), hwnd);

	northWall =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(8, 7, 1),	XMFLOAT3(0, -2, 40.f));
	eastWall =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(1, 7, 25),	XMFLOAT3(-8, -2, 15));
	westWall =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(1, 7, 25),	XMFLOAT3(8, -2, 15));
	southWall =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(8, 7, 1),	XMFLOAT3(0, -2, -10.f));
	roof =			new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(8, 1, 25),	XMFLOAT3(0, 5, 15));
	floor =			new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(4, 2, 20),	XMFLOAT3(0, -7, 20));

	// Pillars
	for (int i = 0; i < 4; i++)
	{
		pillars[2 * i] =		new CylinderMesh(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT2(0.5, 9), XMFLOAT3(-3, -5, 1 + i * 10));
		pillars[2 * i + 1] =	new CylinderMesh(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT2(0.5, 9), XMFLOAT3(3, -5, 1 + i * 10));
	}

	// Shop
	counter =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(2, 0.75, 0.5), XMFLOAT3(0, -4.25, 37));
	backWall =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(3, 2, 0.05), XMFLOAT3(0, -3, 38.95));
	topShelf =		new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(3, 0.05, 0.25), XMFLOAT3(0, -2, 38.7));
	bottomShelf =	new CubeMesh2(renderer->getDevice(), renderer->getDeviceContext(), XMFLOAT3(3, 0.05, 0.25), XMFLOAT3(0, -3, 38.7));

	// Water
	ocean = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100);

	waveShader = new WaveShader(renderer->getDevice(), hwnd);
	waveDepthShader = new WaveDepthShader(renderer->getDevice(), hwnd);

	elapsedTime = 0.0f;

	waves[0].steepness = 0.25f;
	waves[0].direction = XMFLOAT2(1, 0);
	waves[0].wavelength = 1.f;

	waves[1].steepness = 0.445f;
	waves[1].direction = XMFLOAT2(1, 1);
	waves[1].wavelength = 1.f;

	edgeTess.x = 8;
	edgeTess.y = 8;
	edgeTess.z = 8;
	edgeTess.w = 8;

	inTess.x = 8;
	inTess.y = 8;

	// Lights
	int shadowmapWidth = 1024;
	int shadowmapHeight = 1024;

	spotlights[0] = new Light();
	spotlights[0]->setPosition(0, 2, 30);
	spotlights[0]->setDiffuseColour(0.f, 7.f, 0.f, 1.0f);
	spotlights[0]->setAmbientColour(0.143f, 0.083f, 0.211f, 1.0f);
	spotlights[0]->setDirection(0, -1, 2.f);
	spotlights[0]->setSpecularColour(.2f, .2f, .2f, 0.0f);
	spotlights[0]->setSpecularPower(50.f);

	spotlights[1] = new Light();
	spotlights[1]->setPosition(5, 2, 30);
	spotlights[1]->setDiffuseColour(7.f, 0.f, 0.f, 1.0f);
	spotlights[1]->setAmbientColour(0.143f, 0.083f, 0.211f, 1.0f);
	spotlights[1]->setDirection(-1, -1.1, 2.f);
	spotlights[1]->setSpecularColour(.2f, .2f, .2f, 0.0f);
	spotlights[1]->setSpecularPower(50.f);

	spotlights[2] = new Light();
	spotlights[2]->setPosition(-5, 2, 30);
	spotlights[2]->setDiffuseColour(0.f, 0.f, 7.f, 1.0f);
	spotlights[2]->setAmbientColour(0.143f, 0.083f, 0.211f, 1.0f);
	spotlights[2]->setDirection(1, -1.1, 2.f);
	spotlights[2]->setSpecularColour(.2f, .2f, .2f, 1);
	spotlights[2]->setSpecularPower(50.f);

	for (int i = 0; i < 3; i++)
	{
		shadowmaps[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	}

	for (int i = 0; i < 4; i++)
	{
		pointlights[i] = new Light();
		pointlights[i]->setDiffuseColour(.9608f * 8, .8078f * 8, .2588f * 8, 1.0f);
		pointlights[i]->setAmbientColour(0.143f, 0.083f, 0.211f, 1.0f);
		pointlights[i]->setSpecularColour(.09608, .08078, .02588, 1);
		pointlights[i]->setSpecularPower(5000.f);
	}

	pointlights[0]->setPosition(5, 2, 0);
	pointlights[1]->setPosition(-5, 2, 0);
	pointlights[2]->setPosition(5, 2, 20);
	pointlights[3]->setPosition(-5, 2, 20);

	// Moss
	moss = new PointMesh(renderer->getDevice(), renderer->getDeviceContext());

	mossShader = new MossShader(renderer->getDevice(), hwnd);

	mossPositions[0] = XMFLOAT3(7.15, 2.2, 10);
	mossPositions[1] = XMFLOAT3(2, 4, 39.15);
	mossPositions[2] = XMFLOAT3(-7.15, 0.85, 32.6);
	mossPositions[3] = XMFLOAT3(-2.7, 4, 10.7);
	mossPositions[4] = XMFLOAT3(6, 4, -9.15);

	mossRotations[0] = XMFLOAT3(0, acos(-1) / 2, 0);
	mossRotations[1] = XMFLOAT3(-0.02, 0.02, 0.02);
	mossRotations[2] = XMFLOAT3(0, -acos(-1) / 2, 0);
	mossRotations[3] = XMFLOAT3(-0.01, -acos(-1) / 4, -0.03);
	mossRotations[4] = XMFLOAT3(-0.03, acos(-1), 0.02);

	mossScales[0] = 2.5;
	mossScales[1] = 4;
	mossScales[2] = 2.5;
	mossScales[3] = 0.6;
	mossScales[4] = 3;

	// Camera
	camera->setPosition(0, -3, 3);
	
	// Post processing variables
	horiTexture = new RenderTexture(renderer->getDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);
	vertiTexture = new RenderTexture(renderer->getDevice(), screenWidth / 2, screenHeight / 2, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	bloomTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	horiBloomShader = new HorizontalBloomShader(renderer->getDevice(), hwnd);
	vertiBloomShader = new VerticalBloomShader(renderer->getDevice(), hwnd);
	bloomShader = new BloomShader(renderer->getDevice(), hwnd);
	upscaleShader = new UpscaleShader(renderer->getDevice(), hwnd);

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	setWireframe = false;
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	elapsedTime += timer->getTime();

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	depthPass();
	firstPass();
	if (!wireframeToggle)
	{
		horiBloomPass();
		vertiBloomPass();
		upScalePass();
		upScalePass();
		finalPass();
	}
	return true;
}

void App1::depthPass()
{
	for (int i = 0; i < 3; i++)
	{
		shadowmaps[i]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

		spotlights[i]->generateViewMatrix();
		XMMATRIX lightViewMatrix = spotlights[i]->getViewMatrix();
		spotlights[i]->generateProjectionMatrix(0.1, 100);
		XMMATRIX lightProjectionMatrix = spotlights[i]->getProjectionMatrix();
		XMMATRIX worldMatrix = renderer->getWorldMatrix();

		// Room geometry
		staticDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);

		northWall->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), northWall->getIndexCount());
		southWall->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), southWall->getIndexCount());
		eastWall->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), eastWall->getIndexCount());
		westWall->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), westWall->getIndexCount());
		floor->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), floor->getIndexCount());
		roof->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), roof->getIndexCount());

		// Shop
		counter->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), counter->getIndexCount());
		backWall->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), backWall->getIndexCount());
		topShelf->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), topShelf->getIndexCount());
		bottomShelf->sendData(renderer->getDeviceContext());
		staticDepthShader->render(renderer->getDeviceContext(), bottomShelf->getIndexCount());

		// Water
		worldMatrix *= XMMatrixTranslation(-50, -6, -35);
		waveDepthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, elapsedTime, waves, edgeTess, inTess);

		ocean->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		waveDepthShader->render(renderer->getDeviceContext(), ocean->getIndexCount());
		worldMatrix = renderer->getWorldMatrix();

		depthmaps[i] = shadowmaps[i]->getDepthMapSRV();

		renderer->resetViewport();
	}
}

void App1::firstPass()
{
	if (!setWireframe)
	{
		renderTexture->setRenderTarget(renderer->getDeviceContext());
		renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0, 0, 1.f, 1.f);
		wireframeToggle = false;
	}
	else
	{
		renderer->setBackBufferRenderTarget();
		renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
		wireframeToggle = true;
	}

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Room geometry
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), spotlights, pointlights, camera, depthmaps);

	northWall->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), northWall->getIndexCount());
	southWall->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), southWall->getIndexCount());
	eastWall->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), eastWall->getIndexCount());
	westWall->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), westWall->getIndexCount());
	floor->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), floor->getIndexCount());
	roof->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), roof->getIndexCount());

	// Pillars
	for (auto pillar : pillars)
	{
		pillar->sendData(renderer->getDeviceContext());
		lightShader->render(renderer->getDeviceContext(), pillar->getIndexCount());
	}

	// Moss
	moss->sendData(renderer->getDeviceContext());

	for (int i = 0; i < 5; i++)
	{
		worldMatrix *= XMMatrixScaling(mossScales[i], mossScales[i], mossScales[i]);
		worldMatrix *= XMMatrixRotationRollPitchYaw(mossRotations[i].x, acos(-1) + mossRotations[i].y, mossRotations[i].z);
		worldMatrix *= XMMatrixTranslation(mossPositions[i].x, mossPositions[i].y, mossPositions[i].z);
		mossShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"moss"), spotlights, pointlights, camera, depthmaps);
		mossShader->render(renderer->getDeviceContext(), moss->getIndexCount());
		worldMatrix = renderer->getWorldMatrix();
	}

	// Shop
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"wood"), spotlights, pointlights, camera, depthmaps);

	counter->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), counter->getIndexCount());
	backWall->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), backWall->getIndexCount());
	topShelf->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), topShelf->getIndexCount());
	bottomShelf->sendData(renderer->getDeviceContext());
	lightShader->render(renderer->getDeviceContext(), bottomShelf->getIndexCount());

	// Water
	worldMatrix *= XMMatrixTranslation(-50, -6, -35);
	waveShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"blue"), elapsedTime, waves, spotlights, pointlights, camera, depthmaps, edgeTess, inTess);

	ocean->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	waveShader->render(renderer->getDeviceContext(), ocean->getIndexCount());
	worldMatrix = renderer->getWorldMatrix();

	if (setWireframe)
	{
		// Render GUI
		gui();

		// Present the rendered scene to the screen.
		renderer->endScene();
	}
}

void App1::horiBloomPass()
{
	// Clear the scene. (default blue colour)
	horiTexture->setRenderTarget(renderer->getDeviceContext());
	horiTexture->clearRenderTarget(renderer->getDeviceContext(), 0, 1, 0.f, 1.f);

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());
	horiBloomShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView());
	horiBloomShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::vertiBloomPass()
{
	vertiTexture->setRenderTarget(renderer->getDeviceContext());
	vertiTexture->clearRenderTarget(renderer->getDeviceContext(), 0, 1, 0.f, 1.f);

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());
	vertiBloomShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, horiTexture->getShaderResourceView());
	vertiBloomShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::upScalePass()
{
	bloomTexture->setRenderTarget(renderer->getDeviceContext());
	bloomTexture->clearRenderTarget(renderer->getDeviceContext(), 1, 0, 0.f, 1.f);

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());
	upscaleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, vertiTexture->getShaderResourceView());
	upscaleShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::finalPass()
{
	renderer->setBackBufferRenderTarget();
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	orthoMesh->sendData(renderer->getDeviceContext());
	bloomShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, renderTexture->getShaderResourceView(), bloomTexture->getShaderResourceView());
	bloomShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	if (setWireframe)
	{
		wireframeToggle = true;
	}
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &setWireframe);

	ImGui::SliderInt("Edge 1 factor", &edgeTess.x, 1, 64);
	ImGui::SliderInt("Edge 2 factor", &edgeTess.y, 1, 64);
	ImGui::SliderInt("Edge 3 factor", &edgeTess.z, 1, 64);
	ImGui::SliderInt("Edge 4 factor", &edgeTess.w, 1, 64);

	ImGui::SliderInt("Inside 1 factor", &inTess.x, 1, 64);
	ImGui::SliderInt("Inside 2 factor", &inTess.y, 1, 64);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

