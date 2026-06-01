/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit
https://creativecommons.org/publicdomain/zero/1.0/

*/

#include <print>
#include <string>
#include <vector>

#include "raylib.h"
#include "rcamera.h"

#include "Entity.h"
#include "Player.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir

int
main ()
{
  // Tell the window to use vsync and work on high DPI displays
  SetConfigFlags (FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

  // Create the window and OpenGL context
  InitWindow (800, 600, "Hello Raylib");

  // Utility function from resource_dir.h to find the resources folder and set
  // it as the current working directory so we can load from it
  SearchAndSetResourceDir ("resources");

  // Load a texture from the resources directory
  Texture wabbit = LoadTexture ("wabbit_alpha.png");

  HideCursor ();
  DisableCursor ();
  Model model = LoadModelFromMesh (GenMeshCube (5.0f, 5.0f, 5.0f));
  Model plane = LoadModelFromMesh (GenMeshPlane (20.0f, 20.0f, 4, 3));

  plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
    LoadTexture ("texture_13.png");

  Entity* ent = new Entity { model, Vector3 { 0.0f, 0.0f, 0.0f } };
  Entity* pln = new Entity { plane, Vector3 { 0.0f, 0.0f, 0.0f } };

  World world;
  world.push_back (ent);
  world.push_back (pln);

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, world);
  // LoadModelFromMesh (GenMeshPlane (10.0f, 10.0f, 4, 3));

  SetTargetFPS (60);

  // game loop
  while (!WindowShouldClose ()) // run the loop until the user presses ESCAPE or
                                // presses the Close button on the window
  {
    // UpdateCamera(&camera,CAMERA_FIRST_PERSON);
    player->update ();

    // UpdateCameraPro (
    //   &camera,
    //   Vector3 { 0.0, 0.0, 0.0 },
    //   Vector3 { GetMouseDelta ().x * 0.05f, GetMouseDelta ().y * 0.05f, 0.0f
    //   }, GetMouseWheelMove () * 2.0f);

    //  drawing
    BeginDrawing ();

    {
      BeginMode3D (player->camera);

      // Setup the back buffer for drawing (clear color and depth buffers)
      ClearBackground (SKYBLUE);

      DrawGrid (32, 1.0f);

      // draw some text using the default font

      // DrawModel (model, Vector3 { 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);
      ent->DrawEntity ();
      pln->DrawEntity ();
      // draw our texture to the screen
      DrawTexture (wabbit, 400, 200, WHITE);

      EndMode3D ();
    }

    // end the frame and get ready for the next one  (display frame, poll input,
    // etc...)

    DrawText (TextFormat ("Position: (%.1f, %.1f, %.1f)",
                          player->position.x,
                          player->position.y,
                          player->position.z),
              0,
              0,
              15,
              BLACK);

    DrawText (TextFormat ("Velocity: (%06.3f, %06.3f, %06.3f)",
                          player->velocity.x,
                          player->velocity.y,
                          player->velocity.z),
              0,
              20,
              15,
              BLACK);

    DrawText (TextFormat ("camera position: (%06.3f, %06.3f, %06.3f)",
                          player->camera.position.x,
                          player->camera.position.y,
                          player->camera.position.z),
              0,
              35,
              15,
              BLACK);

    EndDrawing ();
  }

  // cleanup
  // unload our texture so it can be cleaned up
  UnloadTexture (wabbit);

  // destroy the window and cleanup the OpenGL context
  CloseWindow ();
  return 0;
}
