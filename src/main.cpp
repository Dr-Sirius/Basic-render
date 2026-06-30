#include <print>
#include <string>
#include <vector>

#include "raylib.h"

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

  Image noise = GenImagePerlinNoise (4, 4, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage (noise);

  std::unordered_map<std::string, Block> ents = Build2 (noise);

  SetAndDetermineRender2 (ents);

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, ents);

  // SetTargetFPS (60);

  Model test = LoadModelFromMesh (GenMeshCube (2.0f, 2.0f, 2.0f));

  player->position = Vector3Zero ();

  // game loop
  while (!WindowShouldClose ())
  {
    player->update ();

    //  drawing
    BeginDrawing ();

    // Setup the back buffer for drawing (clear color and depth buffers)
    ClearBackground (BLACK);
    {
      player->camera.BeginMode3D ();

      player->CheckBlockCol ();

      DrawGrid (32, 1.0f);
      // player->CheckBlockCol ();

      for (auto& [pos, e] : ents)
      {
        // if (!e.render)
        //   continue;
        e.DrawBlock ();
        e.Update ();
      }

      player->camera.EndMode3D ();
    }

    DrawText (
      std::format ("FPS:{}", GetFPS ()).c_str (), 0.0f, 45.0f, 15, WHITE);

    DrawTexture (noiseTex, GetScreenWidth () - noiseTex.width, 0, WHITE);

    DrawVector3 (player->position, { 0.0f, 0.0f }, "Position");

    DrawVector3 (player->velocity, { 0.0f, 15.0f }, "Velocity");

    DrawVector3 (
      player->camera.GetCameraPosition (), { 0.0f, 30.0f }, "Camera Pos");

    EndDrawing ();
  }
  UnloadImage (noise);
  UnloadTexture (noiseTex);

  for (auto& [pos, e] : ents)
  {
    UnloadModel (e.face1.model);
    UnloadModel (e.face2.model);
    UnloadModel (e.face3.model);
    UnloadModel (e.face4.model);
    UnloadModel (e.face5.model);
    UnloadModel (e.face6.model);
  }

  // destroy the window and cleanup the OpenGL context
  CloseWindow ();
  return 0;
}
