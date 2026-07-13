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

  Image noise = GenImagePerlinNoise (32, 32, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage (noise);

  std::unordered_map<std::string, Block> ents = Build2 (noise);

  SetAndDetermineRender2 (ents);

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, ents);

  // SetTargetFPS (60);

  Model test = LoadModelFromMesh (GenMeshCube (2.0f, 2.0f, 2.0f));

  player->position = Vector3Zero ();
  player->camera.position = Vector3Zero ();

  // game loop
  Vector2 cent = { GetScreenWidth () / 2.0f, GetScreenHeight () / 2.0f };
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

      Ray ray = GetScreenToWorldRay (cent, player->camera);
      DrawRay (ray, RED);

      RayCollision coll = CheckCol (ents, ray);

      if (coll.hit)
      {
        if (IsMouseButtonPressed (MOUSE_BUTTON_LEFT))
        {
          ents[Vector3String (coll.point)].type = BLOCK_TYPE::AIR;
          SetAndDetermineRender2 (ents);
          println ("dest");
        }
      }

      EndMode3D ();
    }

    DrawText (
      std::format ("FPS:{}", GetFPS ()).c_str (), 0.0f, 45.0f, 15, WHITE);

    DrawTexture (noiseTex, GetScreenWidth () - noiseTex.width, 0, WHITE);

    DrawVector3 (player->position, { 0.0f, 0.0f }, "Position");

    DrawVector3 (player->velocity, { 0.0f, 15.0f }, "Velocity");

    DrawVector3 (player->camera.position, { 0.0f, 30.0f }, "Camera Pos");

    DrawCircle (cent.x, cent.y, 1.0f, WHITE);

    EndDrawing ();
  }
  UnloadImage (noise);
  UnloadTexture (noiseTex);
  // destroy the window and cleanup the OpenGL context
  CloseWindow ();
  return 0;
}
