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
//#include "vld.h"
#include "DDSTextureLoader.h"

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
	shadowMapSize = 2048;

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
	delete parallaxVS;
	delete parallaxPS;
	delete skyVS;
	delete skyPS;
	delete normalMappingPS;

	delete skyBoxMaterial;
	delete _cubeMaterial;
	delete _cubeMaterial2;

	delete myCamera;
	samplerState->Release();
	rasState->Release();
	depthState->Release();

	ImGui_ImplDX11_Shutdown();
	delete shadowVS;
	
	delete _skybox;
	delete skyBoxCube;

	std::vector<GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
		delete (*it);
	}

	std::vector<ID3D11ShaderResourceView*>::iterator it1;
	for (it1 = srvContainer.begin(); it1 != srvContainer.end(); ++it1) {
		(*it1)->Release();
	}
///<<<<<<< HEAD
	shadowDSV->Release();
	shadowSRV->Release();
	shadowRS->Release();
	shadowSampler->Release();
//=======
	std::vector<Mesh*>::iterator it2;
	for (it2 = meshes.begin(); it2 != meshes.end(); ++it2) {
		delete (*it2);
	}
//>>>>>>> master
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

	//Create Materials
	skyBoxMaterial = new Material(&skyVS, &skyPS);
	skyBoxMaterial->Skybox(&skyVS, &skyPS, &device, &deviceContext, &samplerState, &skySRV, &rasState, &depthState, L"SunnyCubeMap.dds");
	_cubeMaterial = new Material(&vertexShader, &normalMappingPS, &device, &deviceContext, &samplerState, &texSRV, L"bricks2.jpg"); //if I can find 3 textures of differing qualities
																																	//they should be put into materials
	doggoLow = new Material(&vertexShader, &normalMappingPS, &device, &deviceContext, &samplerState, &texSRV, L"FuzzyDoggoLow.jpg");
	doggoMed = new Material(&vertexShader, &normalMappingPS, &device, &deviceContext, &samplerState, &texSRV, L"FuzzyDoggoMed.jpg");
	doggoHigh = new Material(&vertexShader, &normalMappingPS, &device, &deviceContext, &samplerState, &texSRV, L"TextureLol.jpg");


	_cubeMaterial2 = new Material(&parallaxVS, &parallaxPS, &device, &deviceContext, &samplerState, &texSRV1,L"bricks2.jpg", &nMapSRV, L"bricks2_normal.jpg",&dMapSRV,L"bricks2_disp.jpg");
	// Create Material -> Params (Vertexshader, Pixel shader)
	
	srvContainer.push_back(texSRV);
	srvContainer.push_back(texSRV1);
	srvContainer.push_back(nMapSRV);
	srvContainer.push_back(dMapSRV);
	srvContainer.push_back(skySRV);

	GameObject* cube = new GameObject(_cube, _cubeMaterial);
	gameObjects.push_back(cube);
	GameObject* cube2 = new GameObject(_cube2, _cubeMaterial2);
	gameObjects.push_back(cube2);

	cube->SetXPosition(-2);

	GameObject* cube3 = new GameObject(_cube, _cubeMaterial);
	gameObjects.push_back(cube3);
	cube3->SetScale(XMFLOAT3(10, 1, 10));
	cube3->SetYPosition(-3);

	//cube3->SetZPosition(-3);


	//big ass building
	GameObject* building = new GameObject(_cube, doggoHigh, doggoMed,doggoLow, myCamera);
	gameObjects.push_back(building);
	building->SetScale(XMFLOAT3(10, 20, 10));
	building->SetYPosition(3);
	building->SetZPosition(30);
	building->SetRotation(XMFLOAT3(0,0,1.57f));
	skyBoxCube = new GameObject(sbCube, skyBoxMaterial);
	_skybox = new SkyBox(skyBoxCube);
	//  Initialize Lights

	//Directional Light 
	directionLight.AmbientColor = XMFLOAT4(1, 0, 0, 0.0);
	directionLight.DiffuseColor = XMFLOAT4(0, 1, 0, 0);
	directionLight.Direction = XMFLOAT3(0, 0, -1);

	pixelShader->SetData("directionLight", &directionLight, sizeof(directionLight));
	normalMappingPS->SetData("directionLight", &directionLight, sizeof(directionLight));
	parallaxPS->SetData("directionLight", &directionLight, sizeof(directionLight));
	parallaxVS->SetFloat3("dLightPos", directionLight.Direction);
	
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

	skyVS = new SimpleVertexShader(device, deviceContext);
	skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, deviceContext);
	skyPS->LoadShaderFile(L"SkyPS.cso");

	shadowVS = new SimpleVertexShader(device, deviceContext);
	shadowVS->LoadShaderFile(L"ShadowVS.cso");

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	// Create the rasterizer state for the sky
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &rasState);

	// Create the depth stencil state for the sky
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dsDesc, &depthState);

	//SHADOW BLOCK _______________________________________________
	D3D11_TEXTURE2D_DESC shadowMapDesc;
	shadowMapDesc.Width = shadowMapSize;
	shadowMapDesc.Height = shadowMapSize;
	shadowMapDesc.ArraySize = 1;
	shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowMapDesc.CPUAccessFlags = 0;
	shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowMapDesc.MipLevels = 1;
	shadowMapDesc.MiscFlags = 0;
	shadowMapDesc.SampleDesc.Count = 1;
	shadowMapDesc.SampleDesc.Quality = 0;
	shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* shadowTexture;
	device->CreateTexture2D(&shadowMapDesc, 0, &shadowTexture);

	// Create the depth/stencil view for the shadow map
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &dsvDesc, &shadowDSV);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &srvDesc, &shadowSRV);

	// Done with this texture ref
	shadowTexture->Release();

	// Create a better sampler specifically for the shadow map
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.BorderColor[1] = 1.0f;
	sampDesc.BorderColor[2] = 1.0f;
	sampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&sampDesc, &shadowSampler);

	// Create a rasterizer for the shadow creation stage (to apply a bias for us)
	D3D11_RASTERIZER_DESC shRastDesc = {};
	shRastDesc.FillMode = D3D11_FILL_SOLID;
	shRastDesc.CullMode = D3D11_CULL_BACK;
	shRastDesc.FrontCounterClockwise = false;
	shRastDesc.DepthClipEnable = true;
	shRastDesc.DepthBias = 1000; // Not world units - this gets multiplied by the "smallest possible value > 0 in depth buffer"
	shRastDesc.DepthBiasClamp = 0.0f;
	shRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shRastDesc, &shadowRS);

	//SHADOW BLOCK _______________________________________________
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable

	_cube = new Mesh(device, "cube.obj");
	meshes.push_back(_cube);
	_cube2 = new Mesh(device, "cube.obj");
	meshes.push_back(_cube2);
	sbCube = new Mesh(device, "cube.obj");
	meshes.push_back(sbCube);
	distCube = new Mesh(device, "cube.obj");
	meshes.push_back(distCube);
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

	XMMATRIX shView = XMMatrixLookToLH(
		XMVectorSet(0, 20, -20, 0),
		XMVectorSet(0, -1, 1, 0),
		XMVectorSet(0, 1, 0, 0));
	XMStoreFloat4x4(&shadowView, XMMatrixTranspose(shView));

	XMMATRIX shProj = XMMatrixOrthographicLH(
		10.0f,		// Width in world units
		10.0f,		// Height in world units
		0.1f,		// Near plane distance
		100.0f);	// Far plane distance
	XMStoreFloat4x4(&shadowProj, XMMatrixTranspose(shProj));

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

	RenderShadowMap();

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	 

	pixelShader->SetFloat3("cameraPosition", myCamera->camPosition);
	parallaxPS->SetFloat3("cameraPosition", myCamera->camPosition);
	parallaxVS->SetFloat3("viewPos", myCamera->camPosition);

	////
	std::vector<GameObject*>::iterator it;
	int temp = 0;
	for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {
		GameObject* ge = gameObjects.at(temp);
		vertexShader->SetMatrix4x4("world", ge->GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", myCamera->GetviewMatrix());
		vertexShader->SetMatrix4x4("projection", myCamera->GetProjectionMatrix());
		vertexShader->SetMatrix4x4("shadowView", shadowView);
		vertexShader->SetMatrix4x4("shadowProjection", shadowProj);
		normalMappingPS->SetShaderResourceView("shadowMap", shadowSRV);
		normalMappingPS->SetSamplerState("shadowSampler", shadowSampler);

		(*it)->PrepareMaterial(myCamera->GetviewMatrix(), myCamera->GetProjectionMatrix());
		(*it)->Draw(deviceContext);
		temp++;
	}


	
	pixelShader->SetShaderResourceView("shadowMap", 0);

	_skybox->skyBox->PrepareMaterial(myCamera->GetviewMatrix(), myCamera->GetProjectionMatrix());
	_skybox->Draw(deviceContext);

	//Imgui code
	ImGui_ImplDX11_NewFrame();

	ImGui::Begin("Fancy GGP Game Engine");
	ImGui::Text("GUI Frame Work");
	ImGui::SliderFloat("R", &r, 0.0f, 1.0f);
	ImGui::SliderFloat("G", &g, 0.0f, 1.0f);
	ImGui::SliderFloat("B", &b, 0.0f, 1.0f);
	ImGui::ColorEdit3("color value", (float*)&clear_col);
	if (ImGui::Button("Example Window"))
		show_test_window ^= 1;
	if (ImGui::Button("Next Window"))
		show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	// another sample window
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Hello world Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// Shows Test Window
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);     // Normally user code doesn't need/want to call it because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowTestWindow(&show_test_window);
	}
	ImGui::Render();
	//end of gui code


	deviceContext->RSSetState(0);
	deviceContext->OMSetDepthStencilState(0, 0);


	HR(swapChain->Present(0, 0));
}

#pragma endregion

void MyDemoGame::RenderShadowMap()
{
	// Initial setup
	deviceContext->OMSetRenderTargets(0, 0, shadowDSV);
	deviceContext->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	deviceContext->RSSetState(shadowRS);

	// We need a viewport!  This defines how much of the render target to render into
	D3D11_VIEWPORT shadowVP = viewport;
	shadowVP.Width = (float)shadowMapSize;
	shadowVP.Height = (float)shadowMapSize;
	deviceContext->RSSetViewports(1, &shadowVP);

	// Turn on the correct shaders
	shadowVS->SetShader(false); // Don't copy any data yet
	shadowVS->SetMatrix4x4("view", shadowView);
	shadowVS->SetMatrix4x4("projection", shadowProj);
	deviceContext->PSSetShader(0, 0, 0); // Turn off the pixel shader

										 // Actually render everything
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	std::vector<GameObject*>::iterator it;
	int temp = 0;
	for (it = gameObjects.begin(); it != gameObjects.end(); ++it) {

		GameObject* ge = gameObjects.at(temp);
		ID3D11Buffer* vb = ge->getMesh()->GetVertexBuffer();
		ID3D11Buffer* ib = ge->getMesh()->GetIndexBuffer();

		// Set buffers in the input assembler
		deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		shadowVS->SetMatrix4x4("world", ge->GetWorldMatrix());

		// Actually copy the data for this object
		shadowVS->CopyAllBufferData();

		// Finally do the actual drawing
		deviceContext->DrawIndexed(ge->getMesh()->GetIndexCount(), 0, 0);
		temp++;
	}

	// Revert to original DX state
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->RSSetState(0);
}


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