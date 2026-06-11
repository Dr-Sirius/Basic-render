#include <print>
#include <string>
#include <vector>

#include "raylib.h"

#include "rcamera.h"

#include "Block.h"
#include "Entity.h"
#include "Player.h"
#include "Util.h"
#include "resource_dir.h"

int
main ()
{

  SetConfigFlags (FLAG_WINDOW_HIGHDPI);

  InitWindow (1280, 720, "Hello Raylib");

  SearchAndSetResourceDir ("resources");
  HideCursor ();
  DisableCursor ();

  Image noise = GenImagePerlinNoise (128, 128, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage (noise);

  std::unordered_map<std::string, Block> ents = Build2 (noise);

  SetAndDetermineRender2 (ents);

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, ents);

  // SetTargetFPS (60);

  Model test = LoadModelFromMesh (GenMeshCube (2.0f, 2.0f, 2.0f));

  player->position = Vector3Zero ();
  player->camera.position = Vector3Zero ();

  // game loop
  while (!WindowShouldClose ())
  {
    player->update ();

    //  drawing
    BeginDrawing ();

    // Setup the back buffer for drawing (clear color and depth buffers)
    ClearBackground (BLACK);
    {
      BeginMode3D (player->camera);

      DrawGrid (32, 1.0f);
      // player->CheckBlockCol ();

      for (auto& [pos, e] : ents)
      {
        // if (!e.render)
        //   continue;
        e.DrawBlock ();
        e.Update ();
      }

      EndMode3D ();
    }

    DrawText (
      std::format ("FPS:{}", GetFPS ()).c_str (), 0.0f, 45.0f, 15, WHITE);

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
