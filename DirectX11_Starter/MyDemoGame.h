#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "GameObject.h"
#include "Material.h"
#include "Camera.h"
#include "Mesh.h"
#include <math.h>
#include "Lights.h"
#include "imgui_impl_dx11.h"
#include <imgui.h>
#include <d3d11.h>
#include <vector>
#include "SkyBox.h"
#include "btBulletDynamicsCommon.h"
// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders(); 
	void CreateGeometry();
	void CreateMatrices();
	void LoadDynamicsWorld();


	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* normalMappingPS;
	SimpleVertexShader* parallaxVS;
	SimplePixelShader* parallaxPS;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;


	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	POINT p;

	// Meshes
	Mesh* _cube;
	Mesh* _cube2;
	Mesh* sbCube;

	// GameObjects
	std::vector<GameObject*> gameObjects;

	//Skyboxes
	SkyBox* _skybox;
	//SRV
	ID3D11ShaderResourceView* texSRV = 0;
	ID3D11ShaderResourceView* texSRV1 = 0;
	ID3D11ShaderResourceView* nMapSRV = 0;
	ID3D11ShaderResourceView* dMapSRV = 0;
	ID3D11ShaderResourceView* skySRV = 0;

	// Samplers
	ID3D11SamplerState* samplerState;
	ID3D11RasterizerState*		rasState;
	ID3D11DepthStencilState*	depthState;

	//Materials
	Material* _cubeMaterial2;
	Material* _cubeMaterial;
	Material* skyBoxMaterial;


	Camera* myCamera;
	WPARAM btnState;

	DirectionLight directionLight;
	DirectionLight directionalLight2;
	PointLight pointLight;
	SpecularLight specularLight;

	std::vector<ID3D11ShaderResourceView*> srvContainer;
	bool show_test_window = false;
	bool show_another_window = false;
	ImVec4 clear_col = ImColor(114, 144, 154);
	 float r = 1.0f;
	 float g = 0.0f;
	 float b = 0.0f;
};
