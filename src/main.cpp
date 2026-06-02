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
#include "Util.h"
#include "resource_dir.h"

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

  Texture2D tx = LoadTexture ("texture_13.png");

  Image noise = GenImagePerlinNoise (20, 20, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage (noise);

  std::vector<Entity> ents;

  Color* pixel = LoadImageColors (noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x)
  {
    for (int z = 0; z < height; ++z)
    {
      float normalizedHeight = GetGrayScale (pixel[z * width + x]);
      int yHeight = lround (normalizedHeight * 0.1f);

      for (int y = 0; y < yHeight; ++y)
      {

        Entity en =
          Entity { .model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1)),
                   .Position =
                     Vector3 { (float) x * 2, (float) y * 2, (float) z * 2 } };

        en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        ents.push_back (en);
      }
    }
  }
  // Entity* pln = new Entity { plane, Vector3 { 0.0f, 0.0f, 0.0f } };

  World world;

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, world);

  SetTargetFPS (60);

  // game loop
  while (!WindowShouldClose ())
  {
    player->update ();

    //  drawing
    BeginDrawing ();

    // Setup the back buffer for drawing (clear color and depth buffers)
    ClearBackground (SKYBLUE);
    {
      BeginMode3D (player->camera);

      DrawGrid (32, 1.0f);

      for (Entity e : ents)
      {
        e.DrawEntity ();
      }

      EndMode3D ();
    }

    DrawTexture (noiseTex, 0, 0, WHITE);

    DrawVector3 (player->position, { 0.0f, 0.0f }, "Position");

    DrawVector3 (player->velocity, { 0.0f, 15.0f }, "Velocity");

    DrawVector3 (player->camera.position, { 0.0f, 30.0f }, "Camera Pos");

    EndDrawing ();
  }
  UnloadImage (noise);
  UnloadTexture (noiseTex);
  // destroy the window and cleanup the OpenGL context
  CloseWindow ();
  return 0;
}
