/*******************************************************************************************
 *
 *   raylib [models] example - loading gltf with animations
 *
 *   LIMITATIONS:
 *     - Only supports 1 armature per file, and skips loading it if there are multiple armatures
 *     - Only supports linear interpolation (default method in Blender when checked
 *       "Always Sample Animations" when exporting a GLTF file)
 *     - Only supports translation/rotation/scale animation channel.path,
 *       weights not considered (i.e. morph targets)
 *
 *   Example originally created with raylib 3.7, last time updated with raylib 4.2
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2020-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"
#include "resource.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	LoadResource();
	
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [models] example - loading gltf animations");

	// Define the camera to look into our 3d world
	Camera camera = { 0 };
	camera.position = (Vector3){ 6.0f, 6.0f, 6.0f };    // Camera position
	camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

	// Load gltf model
	Model model = LoadModel("map.glb");
	Vector3 position = { 0.0f, 0.0f, 0.0f }; // Set model position

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	DisableCursor();

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
		{
			// Update
			//----------------------------------------------------------------------------------
			UpdateCamera(&camera, CAMERA_FREE);

			// Draw
			//----------------------------------------------------------------------------------
			BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
			DrawModel(model, position, 1.0f, WHITE);    // Draw animated model
			DrawGrid(10, 1.0f);
			EndMode3D();

			EndDrawing();
			//----------------------------------------------------------------------------------
		}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadModel(model);         // Unload model and meshes/material

	CloseWindow();              // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
