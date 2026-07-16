#include <print>
#include <string>
#include <vector>

#include "raylib.h"

#include "rcamera.h"

#include "Block.h"
#include "Entity.h"
#include "Globals.h"
#include "Player.h"
#include "Util.h"
#include "resource_dir.h"

unsigned refresh = 0;

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

  BuildTextureMap ();
  BuildModelMap ();

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

      // player->CheckFloorCol ();

      for (auto& [pos, e] : ents)
      {
        // if (!e.render)
        //   continue;
        e.DrawBlock ();
        e.Update ();
      }

      Ray ray = GetMouseRay (cent, player->camera);
      DrawRay (ray, RED);

      RayCollision coll = CheckCol (ents, ray);

      if (coll.hit)
      {
        Vector3 bpos = coll.point;
        Vector3 min = { bpos.x - 0.5f, bpos.y - 0.5f, bpos.z - 0.5f };
        Vector3 max = { min.x + 1.0f, min.y + 1.0f, min.z + 1.0f };
        DrawBoundingBox ({ min, max }, WHITE);
        if (IsMouseButtonDown (MOUSE_BUTTON_LEFT))
        {
          if (ents.contains (Vector3String (coll.point)))
          {
            ents[Vector3String (coll.point)].type = BLOCK_TYPE::AIR;
            ents[Vector3String (coll.point)].render = false;
            DetermineRerender (ents, getBlock (coll.point, ents));
            println ("dest");
          }
        }
        if (IsMouseButtonPressed (MOUSE_BUTTON_RIGHT))
        {

          Vector3 nBlock = Vector3Add (coll.point, coll.normal);
          nBlock = Vector3Floor (nBlock);
          std::string nPos = Vector3String (nBlock);

          if (ents.contains (nPos))
          {
            ents[nPos].type = BLOCK_TYPE::GROUND;
          }
          else
          {
            ents.insert ({ nPos,
                           Block { .topFace = { 0 },
                                   .bottomFace = { 0 },
                                   .frontFace = { 0 },
                                   .backFace = { 0 },
                                   .rightFace = { 0 },
                                   .leftFace = { 0 },
                                   .position = nBlock,
                                   .type = BLOCK_TYPE::GROUND } });
          }
          DetermineRerender (ents, getBlock (nBlock, ents), false);
          println ("create NORM {}", nPos);
        }
      }

      EndMode3D ();
    }

    DrawText (
      std::format ("FPS:{}", GetFPS ()).c_str (), 0.0f, 45.0f, 15, WHITE);

    DrawTexture (noiseTex, GetScreenWidth () - noiseTex.width, 0, WHITE);

    DrawVector3 (Vector3Ceil (player->position), { 0.0f, 0.0f }, "Position");

    DrawVector3 (player->velocity, { 0.0f, 15.0f }, "Velocity");

    DrawVector3 (player->camera.position, { 0.0f, 30.0f }, "Camera Pos");

    DrawText (
      std::format ("Refresh:{}", refresh).c_str (), 0.0f, 60.0f, 15, WHITE);

    DrawCircle (cent.x, cent.y, 1.0f, WHITE);

    EndDrawing ();

    ++refresh;
    if (refresh > 65536)
    {
      SetAndDetermineRender2 (ents);
      println ("REFRESH");
      refresh = 0;
    }
  }
  UnloadImage (noise);
  UnloadTexture (noiseTex);
  // destroy the window and cleanup the OpenGL context
  CloseWindow ();
  return 0;
}
