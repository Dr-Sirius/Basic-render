#ifndef PLAYER_H
#define PLAYER_H

#include <print>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

using std::println;

class Player
{
  // vars
public:
  Camera3D camera;
  Vector3 position;
  Vector3 velocity;

  // constructors and funcs
public:
  Player (Vector3 pos)
      : position (pos)
  {
    camera = Camera3D { .position = pos,
                        .target = Vector3 { 0.0f, 0.0f, 0.0f },
                        .up = Vector3 { 0.0f, 1.0f, 0.0f },
                        .fovy = 90.0f,
                        .projection = CAMERA_PERSPECTIVE };
  }

  void
  update ()
  {

    handleInput ();
    Vector3 newPos = Vector3Add (position, velocity);
    Vector3 camPos = Vector3Subtract (position, newPos);
    position = newPos;
    handleCamera (camPos);
  }

private:
  float speed = 10.0f;

private:
  void
  handleInput ()
  {

    Vector2 dir = Vector2 {
      (float) (IsKeyDown (KEY_S) - IsKeyDown (KEY_W)),
      (float) (IsKeyDown (KEY_A) - IsKeyDown (KEY_D)),
    };

    if (dir.x != 0 || dir.y != 0)
    {
      velocity.x = dir.x * speed * GetFrameTime ();
      velocity.y = dir.y * speed * GetFrameTime ();
    }
    else
    {
      velocity = Vector3Lerp (velocity, Vector3Zero (), 0.1f);
    }
  }

  void
  handleCamera (Vector3 camPos)
  {
    UpdateCameraPro (
      &camera,
      camPos,
      Vector3 { GetMouseDelta ().x * 0.05f, GetMouseDelta ().y * 0.05f, 0.0f },
      GetMouseWheelMove () * 2.0f);
  }
};

#endif