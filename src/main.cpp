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

  Texture2D tx = LoadTexture ("grass.png");

  Image noise = GenImagePerlinNoise (16, 16, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage (noise);

  std::vector<Block> ents (33296);

  // Color* pixel = LoadImageColors (noise);

  // int height = noise.height;
  // int width = noise.width;

  // for (int x = 0; x < width; ++x)
  // {
  //   for (int z = 0; z < height; ++z)
  //   {
  //     float normalizedHeight = GetGrayScale (pixel[z * width + x]);
  //     int yHeight = lround (normalizedHeight * 0.1f);
  //     println ("Y SIZE {}", yHeight);
  //     for (int y = 0; y < yHeight; ++y)
  //     {

  //       Entity en =
  //         Entity { .model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1,
  //         1)),
  //                  .Position =
  //                    Vector3 { (float) x * 2, (float) y * 2, (float) z * 2 }
  //                    };

  //       en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
  //       ents.push_back (en);
  //     }
  //   }
  // }
  for (int x = 0; x < 16; ++x)
  {
    for (int z = 0; z < 16; ++z)
    {
      for (int y = 0; y < 32; ++y)
      {
        Block en =
          Block { .model = LoadModelFromMesh (GenMeshCube (1.0f, 1.0f, 1.0f)),
                  .position = Vector3 { (float) x, (float) y, (float) z },
                  .type = BLOCK_TYPE::GROUND };

        en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        ents[PointToIndex (en.position)] = en;

        // ents.push_back (en);
      }
    }
  }

  for (Block& b : ents)
  {
    if (b.type == BLOCK_TYPE::AIR)
    {
      b.render = false;
      continue;
    }
    bool drawY = false;
    Vector3 bPos = b.position;
    if (getBlock ({ bPos.x, bPos.y + 1, bPos.z }, ents) == BLOCK_TYPE::GROUND &&
        getBlock ({ bPos.x, bPos.y - 1, bPos.z }, ents) ==
          BLOCK_TYPE::GROUND) // handle blocks above and below
      drawY = true;

    bool drawX = false;
    if (getBlock ({ bPos.x + 1, bPos.y, bPos.z }, ents) == BLOCK_TYPE::GROUND &&
        getBlock ({ bPos.x - 1, bPos.y, bPos.z }, ents) ==
          BLOCK_TYPE::GROUND) // handle blocks above and below
      drawX = true;

    bool drawZ = false;
    if (getBlock ({ bPos.x, bPos.y, bPos.z + 1 }, ents) == BLOCK_TYPE::GROUND &&
        getBlock ({ bPos.x, bPos.y, bPos.z - 1 }, ents) ==
          BLOCK_TYPE::GROUND) // handle blocks above and below
      drawZ = true;

    if (drawY && drawZ && drawX)
      b.render = false;

    // println ("BLOCK END");
  }

  // Entity* pln = new Entity { plane, Vector3 { 0.0f, 0.0f, 0.0f } };

  // printAll (ents);

  World world;

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, world);

  // SetTargetFPS (60);

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

      for (Block& e : ents)
      {
        if (!e.render)
          continue;
        e.DrawBlock ();
      }

      //

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
