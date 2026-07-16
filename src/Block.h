#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <print>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <vector>

#include "Globals.h"
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
  Face topFace;
  Face bottomFace;
  Face frontFace;
  Face backFace;
  Face rightFace;
  Face leftFace;

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
      topFace.DrawFace ();
      bottomFace.DrawFace ();
      frontFace.DrawFace ();
      backFace.DrawFace ();
      rightFace.DrawFace ();
      leftFace.DrawFace ();
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
    return !topFace.render && !bottomFace.render && !frontFace.render &&
           !backFace.render && !rightFace.render && !leftFace.render;
  }
};

Block&
getBlock (Vector3 vec, std::unordered_map<std::string, Block>& blocks)
{
  auto it = blocks.find (Vector3String (vec));
  if (it == blocks.end ())
    return blocks.end ()->second;
  return it->second;
}

/*
Returns the BLOCK_TYPE of the block at positition Vector3 vec using the
Vector3String representation
*/
BLOCK_TYPE
getBlockType (Vector3 vec, std::unordered_map<std::string, Block>& blocks)
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

/*
Iterates through the map of Blocks and determines the render elegibility.
Elegible faces are set to their correct model and render is set true
*/
void
SetAndDetermineRender2 (std::unordered_map<std::string, Block>& blocks)
{

  const Texture2D tx = GetTexture ("DIRT");
  const Texture2D grass = GetTexture ("GRASS");
  const Model plane = GetModel ("DIRT");
  const Model grassPlane = GetModel ("GRASS");

  for (auto& [_, b] : blocks)
  {
    Vector3 pos = b.position;

    b.render = false;
    if (b.type == BLOCK_TYPE::AIR)
    {
      continue;
    }

    b.topFace.render = false;
    b.bottomFace.render = false;
    b.frontFace.render = false;
    b.backFace.render = false;
    b.rightFace.render = false;
    b.leftFace.render = false;

    if (getBlockType ({ pos.x, pos.y + 1, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {

      b.topFace.model = grassPlane;
      b.topFace.position = { pos.x, pos.y + 0.5f, pos.z };
      b.topFace.rotation = { 1.0, 0.0, 0.0 };
      b.topFace.angle = 0.0f;
      b.topFace.render = true;
      b.topFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = grass;

      b.render = true;
      // println ("BLOCK{}[FACE1DONE]", i);
    }

    if (getBlockType ({ pos.x, pos.y - 1, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.bottomFace.model = plane;
      b.bottomFace.position = { pos.x, pos.y - 0.5f, pos.z };
      b.bottomFace.rotation = { 1.0, 0.0, 0.0 };
      b.bottomFace.angle = 180.0f;
      b.bottomFace.render = true;
      b.bottomFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE2DONE]", i);
    }

    if (getBlockType ({ pos.x + 1, pos.y, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.frontFace.model = plane;
      b.frontFace.position = { pos.x + 0.5f, pos.y, pos.z };
      b.frontFace.rotation = { 0.0, 0.0, 1.0 };
      b.frontFace.angle = -90.0f;
      b.frontFace.render = true;
      b.frontFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE3DONE]", i);
    }

    if (getBlockType ({ pos.x - 1, pos.y, pos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.backFace.model = plane;
      b.backFace.position = { pos.x - 0.5f, pos.y, pos.z };
      b.backFace.rotation = { 0.0, 0.0, 1.0 };
      b.backFace.angle = 90.0f;
      b.backFace.render = true;
      b.backFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE4DONE]", i);
    }

    if (getBlockType ({ pos.x, pos.y, pos.z + 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.rightFace.model = plane;
      b.rightFace.position = { pos.x, pos.y, pos.z + 0.5f };
      b.rightFace.rotation = { 1.0, 0.0, 0.0 };
      b.rightFace.angle = 90.0f;
      b.rightFace.render = true;
      b.rightFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE5DONE]", i);
    }

    if (getBlockType ({ pos.x, pos.y, pos.z - 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.leftFace.model = plane;
      b.leftFace.position = { pos.x, pos.y, pos.z - 0.5f };
      b.leftFace.rotation = { 1.0, 0.0, 0.0 };
      b.leftFace.angle = -90.0f;
      b.leftFace.render = true;
      b.leftFace.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      b.render = true;
      // println ("BLOCK{}[FACE6DONE]", i);
    }
    //++i;

    // println ("BLOCK END");
  }
}

void
EnableFace (Face& face,
            const Model& model,
            const Texture& tx,
            Vector3 pos,
            Vector3 rot,
            float angle)
{
  face.model = model;
  face.position = pos;
  face.rotation = rot;
  face.angle = -angle;
  face.render = true;
  face.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
}

void
DetermineRerender (std::unordered_map<std::string, Block>& blocks,
                   Block& target,
                   bool dest = true)
{
  Vector3 bPos = target.position;

  const Texture2D tx = GetTexture ("DIRT");
  const Texture2D grass = GetTexture ("GRASS");
  const Model plane = GetModel ("DIRT");
  const Model grassPlane = GetModel ("GRASS");

  if (dest)
  {
    // btm top
    if (getBlockType ({ bPos.x, bPos.y + 1, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y + 1, bPos.z }, blocks);

      Vector3 nFPos = { block.position.x,
                        block.position.y - 0.5f,
                        block.position.z };
      EnableFace (
        block.bottomFace, plane, tx, nFPos, { 1.0, 0.0, 0.0 }, 180.0f);
      block.render = true;
    }
    if (getBlockType ({ bPos.x, bPos.y - 1, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y - 1, bPos.z }, blocks);
      Vector3 nFPos = { block.position.x,
                        block.position.y + 0.5f,
                        block.position.z };
      EnableFace (
        block.topFace, grassPlane, grass, nFPos, { 1.0, 0.0, 0.0 }, 0.0f);
      block.render = true;
    }

    // b f
    if (getBlockType ({ bPos.x - 1, bPos.y, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x - 1, bPos.y, bPos.z }, blocks);
      Vector3 nFPos = { block.position.x + 0.5f,
                        block.position.y,
                        block.position.z };
      EnableFace (block.frontFace, plane, tx, nFPos, { 0.0, 0.0, 1.0 }, 90.0f);
      block.render = true;
    }
    if (getBlockType ({ bPos.x + 1, bPos.y, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x + 1, bPos.y, bPos.z }, blocks);
      Vector3 nFPos = { block.position.x - 0.5f,
                        block.position.y,
                        block.position.z };
      EnableFace (block.backFace, plane, tx, nFPos, { 0.0, 0.0, 1.0 }, -90.0f);
      block.render = true;
    }

    // l r
    if (getBlockType ({ bPos.x, bPos.y, bPos.z - 1 }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y, bPos.z - 1 }, blocks);
      Vector3 nFPos = { block.position.x,
                        block.position.y,
                        block.position.z + 0.5f };
      EnableFace (block.rightFace, plane, tx, nFPos, { 1.0, 0.0, 0.0 }, -90.0f);
      block.render = true;
    }
    if (getBlockType ({ bPos.x, bPos.y, bPos.z + 1 }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y, bPos.z + 1 }, blocks);
      Vector3 nFPos = { block.position.x,
                        block.position.y,
                        block.position.z - 0.5f };
      EnableFace (block.leftFace, plane, tx, nFPos, { 1.0, 0.0, 0.0 }, 90.0f);
      block.render = true;
    }
  }
  else
  {
    if (getBlockType ({ bPos.x, bPos.y + 1, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y + 1, bPos.z }, blocks);
      block.bottomFace.render = false;
    }
    else
    {
      EnableFace (target.topFace,
                  grassPlane,
                  grass,
                  { bPos.x, bPos.y + 0.5f, bPos.z },
                  { 1.0, 0.0, 0.0 },
                  0.0f);
    }
    if (getBlockType ({ bPos.x, bPos.y - 1, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y - 1, bPos.z }, blocks);

      block.topFace.render = false;
    }
    else
    {
      EnableFace (target.bottomFace,
                  plane,
                  tx,
                  { bPos.x, bPos.y - 0.5f, bPos.z },
                  { 1.0, 0.0, 0.0 },
                  180.0f);
    }

    // b f
    if (getBlockType ({ bPos.x - 1, bPos.y, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x - 1, bPos.y, bPos.z }, blocks);

      block.frontFace.render = false;
    }
    else
    {
      EnableFace (target.backFace,
                  plane,
                  tx,
                  { bPos.x - 0.5f, bPos.y, bPos.z },
                  { 0.0, 0.0, 1.0 },
                  -90.0f);
    }
    if (getBlockType ({ bPos.x + 1, bPos.y, bPos.z }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x + 1, bPos.y, bPos.z }, blocks);

      block.backFace.render = false;
    }
    else
    {
      EnableFace (target.frontFace,
                  plane,
                  tx,
                  { bPos.x + 0.5f, bPos.y, bPos.z },
                  { 0.0, 0.0, 1.0 },
                  90.0f);
    }

    // l r
    if (getBlockType ({ bPos.x, bPos.y, bPos.z - 1 }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y, bPos.z - 1 }, blocks);

      block.rightFace.render = false;
    }
    else
    {
      EnableFace (target.leftFace,
                  plane,
                  tx,
                  { bPos.x, bPos.y, bPos.z - 0.5f },
                  { 1.0, 0.0, 0.0 },
                  90.0f);
    }
    if (getBlockType ({ bPos.x, bPos.y, bPos.z + 1 }, blocks) !=
        BLOCK_TYPE::AIR)
    {
      Block& block = getBlock ({ bPos.x, bPos.y, bPos.z + 1 }, blocks);

      block.leftFace.render = false;
    }
    else
    {
      EnableFace (target.rightFace,
                  plane,
                  tx,
                  { bPos.x, bPos.y, bPos.z + 0.5f },
                  { 1.0, 0.0, 0.0 },
                  -90.0f);
    }
    target.render = true;
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
          Block { .topFace = { 0 },
                  .bottomFace = { 0 },
                  .frontFace = { 0 },
                  .backFace = { 0 },
                  .rightFace = { 0 },
                  .leftFace = { 0 },
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
          Block { .topFace = { 0 },
                  .bottomFace = { 0 },
                  .frontFace = { 0 },
                  .backFace = { 0 },
                  .rightFace = { 0 },
                  .leftFace = { 0 },
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