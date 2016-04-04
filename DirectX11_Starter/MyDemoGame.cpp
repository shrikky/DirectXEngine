// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"
#include <iostream>
// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"My Super Fancy GGP Game";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	// Release any D3D stuff that's still hanging out
	//ReleaseMacro(vertexBuffer);
	//ReleaseMacro(indexBuffer);

	// Delete our simple shaders
	delete vertexShader;
	delete pixelShader;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{	// Camera
	myCamera = new Camera();
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	LoadShaders(); 
	CreateGeometry();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create Material -> Params (Vertexshader, Pixel shader)
	_cubeMaterial = new Material(vertexShader, normalMappingPS, device, deviceContext, L"bricks2.jpg");
	_cubeMaterial2 = new Material(parallaxVS, parallaxPS, device, deviceContext, L"bricks2.jpg",L"bricks2_normal.jpg",L"bricks2_disp.jpg");

	// Create Game Objects -> Params(Mesh, Material)
	cube = new GameObject(_cube, _cubeMaterial);
	cube2 = new GameObject(_cube2, _cubeMaterial2);

	cube->SetXPosition(-2);

	//  Initialize Lights

	//Directional Light
	directionLight.AmbientColor = XMFLOAT4(1, 0, 0, 0.0);
	directionLight.DiffuseColor = XMFLOAT4(0, 1, 0, 0);
	directionLight.Direction = XMFLOAT3(0, 0, -1);

	pixelShader->SetData("directionLight", &directionLight, sizeof(directionLight));
	normalMappingPS->SetData("directionLight", &directionLight, sizeof(directionLight));
	parallaxPS->SetData("directionLight", &directionLight, sizeof(directionLight));
	
	//Point Light
	pointLight.PointLightColor = XMFLOAT4(0, 1, 0, 0);
	pointLight.Position = XMFLOAT3(0, 0, -2);
	pointLight.Strength = 1;
	pixelShader->SetData("pointLight", &pointLight, sizeof(pointLight));
	normalMappingPS->SetData("pointLight", &pointLight, sizeof(pointLight));
	parallaxPS->SetData("pointLight", &pointLight, sizeof(pointLight));
	parallaxVS->SetFloat3("lightPos", pointLight.Position);
	// Specular Light
	specularLight.SpecularStrength = 0.5f;
	specularLight.SpecularColor = XMFLOAT4(1, 0, 0, 1);
	pixelShader->SetData("specularLight", &specularLight, sizeof(specularLight));
	normalMappingPS->SetData("specularLight", &specularLight, sizeof(specularLight));
	parallaxPS->SetData("specularLight", &specularLight, sizeof(specularLight));

	//Gui initialization
	ImGui_ImplDX11_Init(hMainWnd, device, deviceContext);

	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	normalMappingPS = new SimplePixelShader(device, deviceContext);
	normalMappingPS->LoadShaderFile(L"NormalMapping.cso");

	parallaxVS = new SimpleVertexShader(device, deviceContext);
	parallaxVS->LoadShaderFile(L"ParallaxVertexShader.cso");

	parallaxPS = new SimplePixelShader(device, deviceContext);
	parallaxPS->LoadShaderFile(L"ParallaxMapping.cso");
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	_cube = new Mesh(device, "cube.obj");
	_cube2 = new Mesh(device, "cube.obj");
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!
    
	// Create the View matrix
	// - In an actual game, recreate this matrix when the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction you want it to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	myCamera->OnResize(aspectRatio);
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();
	myCamera->OnResize(aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	GetCursorPos(&p);
	if (btnState & 0x0001) {
		OnMouseDown(btnState, p.x, p.y);

	}
	
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	//Cube->SetXPosition((totalTime));
	if (GetAsyncKeyState(VK_SPACE)) {
		myCamera->SetRotationY(sinf(totalTime));
		myCamera->VerticalMovement(0.001f);
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		myCamera->VerticalMovement(-0.001f);
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		myCamera->Forward(0.01f);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		myCamera->Forward(-0.01f);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		myCamera->Strafe(0.01f);
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		myCamera->Strafe(-0.01f);
	}

	myCamera->Update();
	viewMatrix = myCamera->GetviewMatrix();
	
	
	
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{

	
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	cube->PrepareMaterial(myCamera->GetviewMatrix(), myCamera->GetProjectionMatrix());
	cube->Draw(deviceContext);
	cube2->PrepareMaterial(myCamera->GetviewMatrix(), myCamera->GetProjectionMatrix());
	cube2->Draw(deviceContext);

	//pixelShader->SetData("cameraPosition", &myCamera->camPosition, sizeof(myCamera->camPosition));
	pixelShader->SetFloat3("cameraPosition", myCamera->camPosition);
	parallaxPS->SetFloat3("cameraPosition", myCamera->camPosition);
	parallaxVS->SetFloat3("viewPos", myCamera->camPosition);
	//Imgui code 
	ImGui_ImplDX11_NewFrame();
	ImGui::Text("Hello, world!");
	ImGui::Render();
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	//hold left button and move camera
	if (btnState & 0x0001)
	{
		int diffX = x - prevMousePos.x;
		int diffY = y - prevMousePos.y;
		if (diffX > 1000 || diffX <-1000 || diffY > 1000 || diffY< -1000) {
			diffX = diffY = 0;
		}
		myCamera->MouseMovement(diffY * 0.001f, diffX* 0.001f);
		prevMousePos.x = x;
		prevMousePos.y = y;
	}
}
#pragma endregion