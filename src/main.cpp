#include <print>
#include <string>
#include <thread>
#include <vector>

#include "raylib.h"

#include "rcamera.h"

#include "Block.h"
#include "Globals.h"
#include "Player.h"
#include "Util.h"
#include "resource_dir.h"

unsigned refresh = 0;

int main() {
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);

  InitWindow(1280, 720, "Hello Raylib");

  SearchAndSetResourceDir("resources");
  HideCursor();
  DisableCursor();


  Image noise = GenImagePerlinNoise(32, 32, 0.0, 0.0, 1.0f);
  Texture2D noiseTex = LoadTextureFromImage(noise);

  BuildTextureMap();
  BuildModelMap();

  std::unordered_map<std::string, Block> ents = Build2(noise);

  SetAndDetermineRender2(ents);

  Player* player = new Player(Vector3{10.0f, 2.0f, 10.0f}, ents);

  Vector2 cent = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

  ScreenHeight = GetScreenHeight();
    ScreenWidth = GetScreenWidth();

  while (!WindowShouldClose()) {
    if (IsWindowResized()) {
      ScreenHeight = GetScreenHeight();
    ScreenWidth = GetScreenWidth();
    }

    std::thread playerThread(&Player::update, player);

    for (auto& [pos, e] : ents) { e.Update(); }

    if (playerThread.joinable()) playerThread.detach();

    BeginDrawing();

    ClearBackground(BLACK);
    {
      BeginMode3D(player->camera);

      DrawGrid(32, 1.0f);

      for (auto& [pos, e] : ents) { e.DrawBlock(); }

      std::thread collThread([&cent, &player, &ents]() {
        Ray ray = GetScreenToWorldRay(cent, player->camera);

        RayCollision coll = CheckCol(ents, ray, true);

        HandleBlockColl(ray, coll, ents);
      });

      if (collThread.joinable()) collThread.join();

      EndMode3D();
    }

    DrawTexture(noiseTex, GetScreenWidth() - noiseTex.width, 0, WHITE);

    DrawVector3(Vector3Ceil(player->position), {0.0f, 0.0f}, "Position");

    DrawVector3(player->velocity, {0.0f, 15.0f}, "Velocity");

    DrawVector3(player->camera.position, {0.0f, 30.0f}, "Camera Pos");

    DrawText(std::format("FPS:{}", GetFPS()).c_str(), 0.0f, 45.0f, 15, WHITE);

    DrawText(std::format("Refresh:{}", refresh).c_str(), 0.0f, 60.0f, 15,
             WHITE);

    DrawText(std::format("Block #:{}", ents.size()).c_str(), 0.0f, 75.0f, 15,
             WHITE);

    DrawCircle(cent.x, cent.y, 1.0f, WHITE);

    EndDrawing();

    std::thread refreshThread([&ents]() {
      ++refresh;
      if (refresh > 65536) {
        SetAndDetermineRender2(ents);
        println("REFRESH");
        refresh = 0;
      }
    });
    if (refreshThread.joinable()) {refreshThread.detach();}
  }
  UnloadImage(noise);
  UnloadTexture(noiseTex);
  // destroy the window and cleanup the OpenGL context
  CloseWindow();
  return 0;
}
