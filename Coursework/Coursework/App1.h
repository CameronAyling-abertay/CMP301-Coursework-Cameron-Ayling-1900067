// Base application, everything runs from here
// 
// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
// Framework
#include "DXF.h"

// Shaders
#include "LightShader.h"
#include "WaveShader.h"
#include "WaveDepthShader.h"
#include "StaticDepthShader.h"
#include "MossShader.h"
#include "HorizontalBloomShader.h"
#include "VerticalBloomShader.h"
#include "BloomShader.h"
#include "UpscaleShader.h"

// Meshes
#include "CubeMesh2.h"
#include "CylinderMesh.h"

class App1 : public BaseApplication
{
public:
	// Constructor/destructor
	App1();
	~App1();
	
	// Initialisation
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	// Every frame function
	bool frame();

protected:
	// Render functions (including GUI)
	bool render();
	void depthPass();
	void firstPass();
	void horiBloomPass();
	void vertiBloomPass();
	void upScalePass();
	void finalPass();
	void gui();

private:
	// Shaders
	LightShader* lightShader;
	WaveShader* waveShader;
	MossShader* mossShader;
	StaticDepthShader* staticDepthShader;
	WaveDepthShader* waveDepthShader;
	HorizontalBloomShader* horiBloomShader;
	VerticalBloomShader* vertiBloomShader;
	BloomShader* bloomShader;
	UpscaleShader* upscaleShader;

	// Room geometry
	// Walls/floor/ceiling
	CubeMesh2* northWall;
	CubeMesh2* eastWall;
	CubeMesh2* westWall;
	CubeMesh2* southWall;
	CubeMesh2* roof;
	CubeMesh2* floor;

	// Shop
	CubeMesh2* counter;
	CubeMesh2* backWall;
	CubeMesh2* topShelf;
	CubeMesh2* bottomShelf;

	// Pillars
	CylinderMesh* pillars[8];

	// Water
	PlaneMesh* ocean;

	float elapsedTime;

	Wave waves[2];

	XMINT4 edgeTess;
	XMINT2 inTess;

	// Moss
	PointMesh* moss;
	XMFLOAT3 mossPositions[5];
	XMFLOAT3 mossRotations[5];
	float mossScales[5];

	// Lights and shadows
	Light* spotlights[3];
	Light* pointlights[4];

	ShadowMap* shadowmaps[3];
	ID3D11ShaderResourceView* depthmaps[3];

	// Render to texture for post-processing effects
	OrthoMesh* orthoMesh;
	RenderTexture* horiTexture;
	RenderTexture* vertiTexture;
	RenderTexture* renderTexture;
	RenderTexture* bloomTexture;

	bool setWireframe;
};

#endif