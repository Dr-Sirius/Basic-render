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

  std::vector<Block> ents (33296);

  // for (int x = 0; x < 16; ++x)
  // {
  //   for (int z = 0; z < 16; ++z)
  //   {
  //     for (int y = 0; y < 32; ++y)
  //     {
  //       Block en =
  //         Block { .face1 = NULL,
  //                 .face2 = NULL,
  //                 .face3 = NULL,
  //                 .face4 = NULL,
  //                 .face5 = NULL,
  //                 .face6 = NULL,
  //                 .position = Vector3 { (float) x, (float) y, (float) z },
  //                 .type = BLOCK_TYPE::GROUND };

  //       // en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
  //       ents[PointToIndex (en.position)] = en;

  //       // ents.push_back (en);
  //     }
  //   }
  // }

  Color* pixel = LoadImageColors (noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x)
  {
    for (int z = 0; z < height; ++z)
    {
      float normalizedHeight = GetGrayScale (pixel[z * width + x]);
      int yHeight = lround (normalizedHeight * 0.1f);
      println ("Y SIZE {}", yHeight);
      for (int y = 0; y < yHeight; ++y)
      {

        Block en =
          Block { .face1 = NULL,
                  .face2 = NULL,
                  .face3 = NULL,
                  .face4 = NULL,
                  .face5 = NULL,
                  .face6 = NULL,
                  .position = Vector3 { (float) x, (float) y, (float) z },
                  .type = BLOCK_TYPE::GROUND };

        // en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        ents[PointToIndex (en.position)] = en;
      }
    }
  }

  SetAndDetermineRender (ents);

  // Entity* pln = new Entity { plane, Vector3 { 0.0f, 0.0f, 0.0f } };

  // printAll (ents);

  World world;

  Player* player = new Player (Vector3 { 10.0f, 2.0f, 10.0f }, world);

  // SetTargetFPS (60);

  Model test = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));

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
      // DrawModelEx (test,
      //              { 0.0, 0.5, 0.0 },
      //              { 1.0, 0.0, 0.0 },
      //              180,
      //              Vector3One (),
      //              RED); // negy

      // DrawModelEx (test,
      //              { 0.0, 1.5, 0.0 },
      //              { 1.0, 0.0, 0.0 },
      //              0,
      //              Vector3One (),
      //              WHITE); // posy

      // DrawModelEx (test,
      //              { 0.0, 1.0, 0.5 },
      //              { 1.0, 0.0, 0.0 },
      //              90,
      //              Vector3One (),
      //              GREEN); // posz

      // DrawModelEx (test,
      //              { 0.0, 1.0, -0.5 },
      //              { 1.0, 0.0, 0.0 },
      //              -90,
      //              Vector3One (),
      //              BLUE); // negz

      // DrawModelEx (test,
      //              { -0.5, 1.0, 0.0 },
      //              { 0.0, 0.0, 1.0 },
      //              90,
      //              Vector3One (),
      //              YELLOW); // negx

      // DrawModelEx (test,
      //              { 0.5, 1.0, 0.0 },
      //              { 0.0, 0.0, 1.0 },
      //              -90,
      //              Vector3One (),
      //              ORANGE); // posx
      // //

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
