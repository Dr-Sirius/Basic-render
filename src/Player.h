#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "rlFPCamera.cpp"

#include <print>
#include <raylib.h>
#include <raymath.h>
#include <vector>

using std::println;

class Player
{
  // vars
public:
  rlFPCamera camera;
  Vector3 position = { 0 };
  Vector3 velocity = { 0 };

  // constructors and funcs
public:
  Player (Vector3 pos, std::unordered_map<std::string, Block>& b)
      : position (pos),
        blocks (b)
  {
    pos.y += headDist;
    camera.Setup (45, { pos.x, pos.y, pos.z });
    camera.MoveSpeed.z = speed;
    camera.MoveSpeed.x = speed;

    // camera.FarPlane = 5000;
  }

  void
  update ()
  {

    handleInput ();
    // if (free)
    // {
    //   UpdateCamera (&camera, CAMERA_FREE);
    //   position = camera.position;
    //   return;
    // }

    handlePhysics ();
    Vector3 newPos = Vector3Add (position, velocity);
    position = newPos;
    // camera.SetCameraPosition ({ position.x, position.y, position.z });
    camera.Update ();
    position = camera.GetCameraPosition ();
  }

  void
  CheckBlockCol ()
  {

    RayCollision collision = { 0 };
    collision.distance = __FLT_MAX__;
    collision.hit = false;

    ray = GetScreenToWorldRay (GetMousePosition (), camera.GetCamera ());

    DrawLine3D (ray.position, Vector3Add (ray.position, ray.direction), RED);
    for (auto& [pos, b] : blocks)
    {
      if (b.face1.render)
      {
        checkCol (collision, b.face1.model);
      }
      if (b.face2.render)
      {
        checkCol (collision, b.face2.model);
      }
      if (b.face3.render)
      {
        checkCol (collision, b.face3.model);
      }
      if (b.face4.render)
      {
        checkCol (collision, b.face4.model);
      }
      if (b.face5.render)
      {
        checkCol (collision, b.face5.model);
      }
      if (b.face6.render)
      {
        checkCol (collision, b.face6.model);
      }
    }
  }

private:
  float speed = 10.0f;
  float headDist = 1.0f;
  float pitch, yaw;
  float distance = 2.0f;

  Ray ray = { 0 };

  World world;
  std::unordered_map<std::string, Block>& blocks;

  Vector3 direction;

  Vector3 cameraFront;

  Vector3 cameraRight;

  Vector3 cameraUp;

  bool free = true;

private:
  void
  checkCol (RayCollision& collision, const Model& mod)
  {
    for (int m = 0; m < mod.meshCount; ++m)
    {
      collision = GetRayCollisionMesh (ray, mod.meshes[m], mod.transform);
      if (collision.hit)
      {
        println ("HIT at {}", Vector3String (collision.point));
        break;
      }
    }
  }

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
      velocity.x = 0.0f; // Lerp (velocity.x, 0.001f, 0.1f);
      velocity.z = 0.0f; // Lerp (velocity.z, 0.001f, 0.1f);
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

    // camera.position = position;
    // camera.position.y += headDist;

    // float dt = GetFrameTime ();

    // Vector2 mouseDelta = GetMouseDelta ();
    // // mouseDelta.x *= 0.3;
    // // mouseDelta.y *= 0.3;

    // yaw += mouseDelta.x * dt;

    // pitch += -mouseDelta.y * dt;

    // if (pitch > 1.5)
    //   pitch = 1.5;
    // else if (pitch < -1.5)
    //   pitch = -1.5;

    // direction.x = cos (yaw) * cos (pitch);

    // direction.y = sin (pitch);

    // direction.z = sin (yaw) * cos (pitch);

    // cameraFront = Vector3Normalize (direction);

    // cameraRight =
    //   Vector3Normalize (Vector3CrossProduct (camera.up, cameraFront));

    // cameraUp = Vector3CrossProduct (direction, cameraRight);

    // camera.target = Vector3Add (camera.position, cameraFront);
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

    if (IsKeyPressed (KEY_F2))
    {
      position = Vector3Zero ();
      if (free)
      {
        // camera.position = position;
      }
    }

    if (IsKeyPressed (KEY_UP))
    {
      distance += 1;
    }

    if (IsKeyPressed (KEY_DOWN))
    {
      distance -= 1;
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