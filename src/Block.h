#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <print>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>

#include "Util.h"

using std::print;
using std::println;

enum BLOCK_TYPE
{
  AIR,
  GROUND,
};

struct Face
{
  Model model;
  Vector3 position;
  Vector3 rotation;
  float angle;
  bool render;
  void
  DrawFace ()
  {
    if (render)
      DrawModelEx (model, position, rotation, angle, Vector3One (), WHITE);
  }
};

struct Block
{
  // Mesh block;
  // Model model;
  Face face1;
  Face face2;
  Face face3;
  Face face4;
  Face face5;
  Face face6;

  Vector3 position;
  BLOCK_TYPE type;

  bool render = true;
  bool debug = false;

  Color colliderCol = RED;

  void
  DrawBlock ()
  {
    if (render)
    {
      face1.DrawFace ();
      face2.DrawFace ();
      face3.DrawFace ();
      face4.DrawFace ();
      face5.DrawFace ();
      face6.DrawFace ();
    }
    if (debug)
    {
      if (!render)
        colliderCol = YELLOW;
      DrawCubeWires (position, 1.0f, 1.0f, 1.0f, colliderCol);
    }
    // DrawModel (model, position, 1.0f, WHITE);
  }

  void
  Update ()
  {
    if (IsKeyPressed (KEY_F3))
    {
      debug = !debug;
    }
    if (IsKeyPressed (KEY_F5))
    {
      colliderCol = GREEN;
    }
    if (IsKeyPressed (KEY_F6))
    {
      colliderCol = WHITE;
    }
  }

  bool
  IsFacesNotRendered ()
  {
    return !face1.render && !face2.render && !face3.render && !face4.render &&
           !face5.render && !face6.render;
  }
};

BLOCK_TYPE
getBlock (Vector3 vec, std::vector<Block> blocks)
{
  int index = PointToIndex (vec);
  if (index >= blocks.size () || index < 0)
  {
    return BLOCK_TYPE::AIR;
  }
  return blocks[index].type;
}

/*
Returns the BLOCK_TYPE of the block at positition Vector3 vec using the
Vector3String representation
*/
BLOCK_TYPE
getBlock2 (Vector3 vec, std::unordered_map<std::string, Block>& blocks)
{
  auto it = blocks.find (Vector3String (vec));
  if (it == blocks.end ())
    return BLOCK_TYPE::AIR;
  return it->second.type;
}

/*
Returns a string representation of a BLOCK_TYPE
*/
std::string
blockString (BLOCK_TYPE block)
{
  switch (block)
  {
    case GROUND:
      return "GROUND";
      break;

    default:
      return "AIR";
      break;
  }
}

void
SetAndDetermineRender (std::vector<Block>& blocks)
{
  Texture2D tx = LoadTexture ("grass.png");
  unsigned i = 0;
  Model plane = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
  for (Block& b : blocks)
  {

    b.render = false;
    if (b.type == BLOCK_TYPE::AIR)
    {
      continue;
    }

    b.face1.render = false;
    b.face2.render = false;
    b.face3.render = false;
    b.face4.render = false;
    b.face5.render = false;
    b.face6.render = false;
    bool drawY = false;
    Vector3 bPos = b.position;
    if (getBlock ({ bPos.x, bPos.y + 1, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face1.model = plane;
      b.face1.position = { bPos.x, bPos.y + 0.5f, bPos.z };
      b.face1.rotation = { 1.0, 0.0, 0.0 };
      b.face1.angle = 0.0f;
      b.face1.render = true;
      b.face1.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE1DONE]", i);
    }

    if (getBlock ({ bPos.x, bPos.y - 1, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face2.model = plane;
      b.face2.position = { bPos.x, bPos.y - 0.5f, bPos.z };
      b.face2.rotation = { 1.0, 0.0, 0.0 };
      b.face2.angle = 180.0f;
      b.face2.render = true;
      b.face2.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE2DONE]", i);
    }
    // drawY = true;

    bool drawX = false;
    if (getBlock ({ bPos.x + 1, bPos.y, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face3.model = plane;
      b.face3.position = { bPos.x + 0.5f, bPos.y, bPos.z };
      b.face3.rotation = { 0.0, 0.0, 1.0 };
      b.face3.angle = -90.0f;
      b.face3.render = true;
      b.face3.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE3DONE]", i);
    }

    if (getBlock ({ bPos.x - 1, bPos.y, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face4.model = plane;
      b.face4.position = { bPos.x - 0.5f, bPos.y, bPos.z };
      b.face4.rotation = { 0.0, 0.0, 1.0 };
      b.face4.angle = 90.0f;
      b.face4.render = true;
      b.face4.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE4DONE]", i);
    }

    bool drawZ = false;
    if (getBlock ({ bPos.x, bPos.y, bPos.z + 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face5.model = plane;
      b.face5.position = { bPos.x, bPos.y, bPos.z + 0.5f };
      b.face5.rotation = { 1.0, 0.0, 0.0 };
      b.face5.angle = 90.0f;
      b.face5.render = true;
      b.face5.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE5DONE]", i);
    }

    if (getBlock ({ bPos.x, bPos.y, bPos.z - 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face6.model = plane;
      b.face6.position = { bPos.x, bPos.y, bPos.z - 0.5f };
      b.face6.rotation = { 1.0, 0.0, 0.0 };
      b.face6.angle = -90.0f;
      b.face6.render = true;
      b.face6.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      println ("BLOCK{}[FACE6DONE]", i);
    }
    ++i;

    // println ("BLOCK END");
  }
}

/*
Iterates through the map of Blocks and determines the render elegibility.
Elegible faces are set to their correct model and render is set true
*/
void
SetAndDetermineRender2 (std::unordered_map<std::string, Block>& blocks)
{

  Image dirtText = LoadImage ("dirt.png");
  ImageResize (&dirtText, dirtText.width * 0.0625, dirtText.height * 0.0625);

  Image grassText = LoadImage ("grass.png");
  ImageResize (&grassText, grassText.width * 0.0625, grassText.height * 0.0625);

  Texture2D tx = LoadTextureFromImage (dirtText);
  Texture2D grass = LoadTextureFromImage (grassText);
  // unsigned i = 0;
  Model plane = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
  Model grassPlane = LoadModelFromMesh (GenMeshPlane (
    1.0f, 1.0f, 1, 1)); // needed because meshes are pointers in models
  for (auto& [_, b] : blocks)
  {
    Vector3 pos = b.position;

    b.render = false;
    if (b.type == BLOCK_TYPE::AIR)
    {
      continue;
    }

    b.face1.render = false;
    b.face2.render = false;
    b.face3.render = false;
    b.face4.render = false;
    b.face5.render = false;
    b.face6.render = false;

    if (getBlock2 ({ pos.x, pos.y + 1, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {

      b.face1.model = grassPlane;
      b.face1.position = { pos.x, pos.y + 0.5f, pos.z };
      b.face1.rotation = { 1.0, 0.0, 0.0 };
      b.face1.angle = 0.0f;
      b.face1.render = true;
      b.face1.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = grass;

      b.render = true;
      // println ("BLOCK{}[FACE1DONE]", i);
    }

    if (getBlock2 ({ pos.x, pos.y - 1, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face2.model = plane;
      b.face2.position = { pos.x, pos.y - 0.5f, pos.z };
      b.face2.rotation = { 1.0, 0.0, 0.0 };
      b.face2.angle = 180.0f;
      b.face2.render = true;
      b.face2.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE2DONE]", i);
    }

    if (getBlock2 ({ pos.x + 1, pos.y, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face3.model = plane;
      b.face3.position = { pos.x + 0.5f, pos.y, pos.z };
      b.face3.rotation = { 0.0, 0.0, 1.0 };
      b.face3.angle = -90.0f;
      b.face3.render = true;
      b.face3.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE3DONE]", i);
    }

    if (getBlock2 ({ pos.x - 1, pos.y, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face4.model = plane;
      b.face4.position = { pos.x - 0.5f, pos.y, pos.z };
      b.face4.rotation = { 0.0, 0.0, 1.0 };
      b.face4.angle = 90.0f;
      b.face4.render = true;
      b.face4.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE4DONE]", i);
    }

    if (getBlock2 ({ pos.x, pos.y, pos.z + 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face5.model = plane;
      b.face5.position = { pos.x, pos.y, pos.z + 0.5f };
      b.face5.rotation = { 1.0, 0.0, 0.0 };
      b.face5.angle = 90.0f;
      b.face5.render = true;
      b.face5.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE5DONE]", i);
    }

    if (getBlock2 ({ pos.x, pos.y, pos.z - 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face6.model = plane;
      b.face6.position = { pos.x, pos.y, pos.z - 0.5f };
      b.face6.rotation = { 1.0, 0.0, 0.0 };
      b.face6.angle = -90.0f;
      b.face6.render = true;
      b.face6.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE6DONE]", i);
    }
    //++i;

    // println ("BLOCK END");
  }
}

std::vector<Block>*
Build (Image noise)
{
  std::vector<Block>* ents = new std::vector<Block> (16384);

  Color* pixel = LoadImageColors (noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x)
  {
    for (int z = 0; z < height; ++z)
    {
      float normalizedHeight = GetGrayScale (pixel[z * width + x]);
      int yHeight = lround (normalizedHeight * 0.001f);
      println ("Y SIZE {}", yHeight);
      for (int y = 0; y < yHeight; ++y)
      {

        Block en =
          Block { .face1 = { 0 },
                  .face2 = { 0 },
                  .face3 = { 0 },
                  .face4 = { 0 },
                  .face5 = { 0 },
                  .face6 = { 0 },
                  .position = Vector3 { (float) x, (float) y, (float) z },
                  .type = BLOCK_TYPE::GROUND };

        // en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        (*ents)[PointToIndex (en.position)] = en;
      }
    }
  }
  return ents;
}
/*
Builds block terrain from the given noise and returns an unordered map of blocks

The returned map is of type std::unordered_map<std::string, Block>

The keys are string versions of the block's Vector3 position using the
Vector3String function
*/
std::unordered_map<std::string, Block>
Build2 (Image noise)
{
  std::unordered_map<std::string, Block> ents;

  Color* pixel = LoadImageColors (noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x)
  {
    for (int z = 0; z < height; ++z)
    {
      float normalizedHeight = GetGrayScale (pixel[z * width + x]);
      int yHeight = lround (normalizedHeight * 0.03f);
      // println ("Y SIZE {}", yHeight);
      for (int y = 0; y < yHeight; ++y)
      {

        Block en =
          Block { .face1 = { 0 },
                  .face2 = { 0 },
                  .face3 = { 0 },
                  .face4 = { 0 },
                  .face5 = { 0 },
                  .face6 = { 0 },
                  .position = Vector3 { (float) x, (float) y, (float) z },
                  .type = BLOCK_TYPE::GROUND };

        // en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        ents.insert ({ Vector3String (en.position), en });
      }
    }
  }
  return ents;
}

RayCollision
CheckCol (std::unordered_map<std::string, Block>& blocks,
          Ray ray,
          bool drawDebug = false)
{
  float maxDist = MAXFLOAT;
  Vector3 closeBlockPos = { -1 };
  bool blockHit = false;
  Vector3 colNorm = {};
  RayCollision coll = { 0 };
  for (auto& [pos, block] : blocks)
  {
    if (block.type == BLOCK_TYPE::AIR || !block.render ||
        block.IsFacesNotRendered ())
      continue;
    Vector3 bpos = block.position;
    Vector3 min = { bpos.x - 0.5f, bpos.y - 0.5f, bpos.z - 0.5f };
    Vector3 max = { min.x + 1.0f, min.y + 1.0f, min.z + 1.0f };
    BoundingBox bound = { min, max };
    coll = GetRayCollisionBox (ray, bound);
    // DrawSphere (coll.point, 0.5f, RED);
    // DrawBoundingBox (bound, RED);
    // DrawLine3D (ray.position, coll.point, RED);
    if (coll.hit && coll.distance < maxDist)
    {
      maxDist = coll.distance;
      closeBlockPos = bpos;
      colNorm = coll.normal;
      blockHit = true;

      coll.point = bpos;

      // println ("BLOCK POS {} COLL POS {} BLOCK NORM {}",
      //          Vector3String (block.position),
      //          Vector3String (coll.point),
      //          Vector3String (Vector3Add (bpos, colNorm)));
      if (drawDebug)
      {
        DrawBoundingBox (bound, WHITE);
        DrawLine3D (bpos, Vector3Add (bpos, coll.normal), RED);
      }

      // DrawCubeWires (bpos, 1.0f, 1.0f, 1.0f, WHITE);
    }
  }
  coll.point = closeBlockPos;
  coll.hit = blockHit;
  coll.normal = colNorm;
  return coll;
}

#endif