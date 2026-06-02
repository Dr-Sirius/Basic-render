#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <print>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <vector>

using std::println;

class Player
{
  // vars
public:
  Camera3D camera;
  Vector3 position = { 0 };
  Vector3 velocity = { 0 };

  // constructors and funcs
public:
  Player (Vector3 pos, World& w)
      : position (pos),
        world (w)
  {
    pos.y += headDist;
    camera = Camera3D { .position = pos,
                        .target = Vector3 { 0.0f, 0.0f, 0.0f },
                        .up = Vector3 { 0.0f, 1.0f, 0.0f },
                        .fovy = 90.0f,
                        .projection = CAMERA_PERSPECTIVE };

    pitch = -0.6; // mouseDelta y

    yaw = -2.45; // mouseDelta x

    direction.x = cos (yaw) * cos (pitch);

    direction.y = sin (pitch);

    direction.z = sin (yaw) * cos (pitch);

    cameraFront = Vector3Normalize (direction);

    cameraRight =
      Vector3Normalize (Vector3CrossProduct (camera.up, cameraFront));

    cameraUp = Vector3CrossProduct (direction, cameraRight);

    camera.target = Vector3Add (camera.position, cameraFront);
  }

  void
  update ()
  {

    handleInput ();
    if (free)
    {
      UpdateCamera (&camera, CAMERA_FREE);
      position = camera.position;
      return;
    }
    handlePhysics ();
    // println ("position ({},{},{})", position.x, position.y, position.z);

    Vector3 newPos = Vector3Add (position, velocity);
    // println ("new pos ({},{},{})", newPos.x, newPos.y, newPos.z);
    position = newPos;

    // camPos.y += headDist;
    handleCamera ();
  }

private:
  float speed = 10.0f;
  float headDist = 4.0f;
  float pitch, yaw;

  World world;

  Vector3 direction;

  Vector3 cameraFront;

  Vector3 cameraRight;

  Vector3 cameraUp;

  bool free = false;

private:
  void
  handlePhysics ()
  {

    Vector2 dir = Vector2 {
      (float) (IsKeyDown (KEY_S) - IsKeyDown (KEY_W)),
      (float) (IsKeyDown (KEY_A) - IsKeyDown (KEY_D)),
    };
    Vector3 newDir = handleDir (dir);

    if (dir.x != 0 || dir.y != 0)
    {
      velocity.x = newDir.x * speed * GetFrameTime ();
      velocity.z = newDir.z * speed * GetFrameTime ();
    }
    else
    {
      velocity.x = Lerp (velocity.x, 0.001f, 0.1f);
      velocity.z = Lerp (velocity.z, 0.001f, 0.1f);
    }

    if (!isOnGround () && position.y > 0)
      velocity.y += -0.98 * GetFrameTime ();
    if (position.y <= 0)
    {
      velocity.y = 0;
    }
    if (IsKeyPressed (KEY_SPACE))
    {
      velocity.y = 0.4f;
    }
  }

  void
  handleCamera ()
  {
    // UpdateCameraPro (
    //   &camera,
    //   camPos,
    //   Vector3 { GetMouseDelta ().x * 0.05f, GetMouseDelta ().y * 0.05f, 0.0f
    //   }, GetMouseWheelMove () * 2.0f);
    camera.position = position;
    camera.position.y += headDist;

    Vector2 mouseDelta = GetMouseDelta ();
    mouseDelta.x *= 0.3;
    mouseDelta.y *= 0.3;

    yaw += mouseDelta.x * GetFrameTime ();

    pitch += -mouseDelta.y * GetFrameTime ();

    if (pitch > 1.5)
      pitch = 1.5;
    else if (pitch < -1.5)
      pitch = -1.5;

    direction.x = cos (yaw) * cos (pitch);

    direction.y = sin (pitch);

    direction.z = sin (yaw) * cos (pitch);

    cameraFront = Vector3Normalize (direction);

    cameraRight =
      Vector3Normalize (Vector3CrossProduct (camera.up, cameraFront));

    cameraUp = Vector3CrossProduct (direction, cameraRight);

    camera.target = Vector3Add (camera.position, cameraFront);
  }

  Vector3
  handleDir (Vector2 dir)
  {

    dir.x = -dir.x;
    dir.y = dir.y;

    Vector3 desiredDir = (Vector3) {
      dir.y * cameraRight.x + dir.x * cameraFront.x,
      0.0f,
      dir.y * cameraRight.z + dir.x * cameraFront.z,
    };

    return desiredDir;
  }

  void
  handleInput ()
  {
    if (IsKeyPressed (KEY_F1))
    {
      free = !free;
    }
  }

  bool
  isOnGround ()
  {
    for (int i = 0; i < world.size (); ++i)
    {
      if (position.y <= 0)
      {
        return true;
      }
    }
    return false;
  }
};

#endif