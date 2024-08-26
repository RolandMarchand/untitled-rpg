#include <string.h>

#include "raylib.h"
#include "resources.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight,
		   "raylib [models] example - loading gltf animations");

	// Define the camera to look into our 3d world
	Camera camera = { 0 };
	camera.position = (Vector3){ 6.0f, 6.0f, 6.0f }; // Camera position
	camera.target =
		(Vector3){ 0.0f, 2.0f, 0.0f }; // Camera looking at point
	camera.up =
		(Vector3){ 0.0f, 1.0f,
			   0.0f }; // Camera up vector (rotation towards target)
	camera.fovy = 70.0f; // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

	int resourcesPathLength = GetResourcesPath(NULL, 0);
	if (resourcesPathLength <= 0) {
		PRINT_ERR("Unable to get resources.\n");
		abort();
	}
	char *resourcesPath = (char *)malloc(resourcesPathLength + 1);
	if (resourcesPath == NULL) {
		perror("Unable to allocate memory for the resources path.");
		abort();
	}

	GetResourcesPath(resourcesPath, resourcesPathLength + 1);

	char *modelFilename = MODELS_PATH "/simple.obj";
	size_t modelPathLength = resourcesPathLength + strlen(modelFilename);
	char *modelPath = (char *)malloc(modelPathLength + 1);
	if (modelPath == NULL) {
		perror("Unable to allocate memory for the resources model path.");
		abort();
	}
	snprintf(modelPath, modelPathLength + 1, "%s%s", resourcesPath, modelFilename);
	free(resourcesPath);

	for (size_t i = 0; i < strlen(modelPath); i++) {
		if (modelPath[i] == '/') {
			modelPath[i] = '\\';
		}
	}

	Model model = LoadModel(modelPath);
	free(modelPath);
	Vector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	DisableCursor();

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		printf("%g, %g, %g\n", camera.position.x, camera.position.y,
		       camera.position.z);
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera, CAMERA_FREE);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		DrawModel(model, position, 1 / 32.0f,
			  WHITE); // Draw animated model
		DrawGrid(10, 1.0f);
		EndMode3D();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadModel(model); // Unload model and meshes/material

	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
