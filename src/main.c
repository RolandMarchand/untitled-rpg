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

	char resourcesDir[PATH_MAX];
	int len = GetResourcesPath(resourcesDir, PATH_MAX);
	if (len <= 0) {
		ERROR("Unable to get resources.\n");
		abort();
	}

	char modelPath[PATH_MAX + len];
	snprintf(modelPath, PATH_MAX + len, "%s" MODELS_PATH "/simple.obj",
		 resourcesDir);

	Model model = LoadModel(modelPath);
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
