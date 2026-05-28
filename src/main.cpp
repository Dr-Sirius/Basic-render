/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir


void handleMouseCallback();


int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	

	// Create the window and OpenGL context
	InitWindow(800, 600, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	Camera3D camera {
		.position=Vector3 {10.0f,10.0f,10.0f},
		.target=Vector3 {0.0f,0.0f,0.0f},
		.up=Vector3 {0.0f,1.0f,0.0f},
		.fovy=90.0f,
		.projection=CAMERA_PERSPECTIVE
	};

	HideCursor();
	DisableCursor();


	Model model = LoadModelFromMesh(GenMeshPlane(10.0f,10.0f,4,3));

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		UpdateCamera(&camera,CAMERA_FIRST_PERSON);
		// drawing
		BeginDrawing();

		BeginMode3D(camera);

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(SKYBLUE);

		DrawGrid(32,1.0f);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,BLACK);
		DrawModel(model,Vector3 {0.0f,0.0f,0.0f},1.0f,BLACK);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		EndMode3D();
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();

		
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
