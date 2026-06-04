#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <print>
#include <raylib.h>
#include <raymath.h>
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
    // DrawModel (model, position, 1.0f, WHITE);
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
printAll (const std::vector<Block>& blocks)
{
  print ("[");
  for (const Block& b : blocks)
  {
    print ("{}, ", blockString (b.type));
  }
  print ("]");
  println ("");
}

void
SetAndDetermineRender (std::vector<Block>& blocks)
{
  Texture2D tx = LoadTexture ("grass.png");
  unsigned i = 0;
  for (Block& b : blocks)
  {

    b.face1.render = false;
    b.face2.render = false;
    b.face3.render = false;
    b.face4.render = false;
    b.face5.render = false;
    b.face6.render = false;

    if (b.type == BLOCK_TYPE::AIR)
    {
      b.render = false;
      continue;
    }
    bool drawY = false;
    Vector3 bPos = b.position;
    if (getBlock ({ bPos.x, bPos.y + 1, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face1.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face1.position = { bPos.x, bPos.y + 0.5f, bPos.z };
      b.face1.rotation = { 1.0, 0.0, 0.0 };
      b.face1.angle = 0.0f;
      b.face1.render = true;
      b.face1.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE1DONE]", i);
    }

    if (getBlock ({ bPos.x, bPos.y - 1, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face2.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face2.position = { bPos.x, bPos.y - 0.5f, bPos.z };
      b.face2.rotation = { 1.0, 0.0, 0.0 };
      b.face2.angle = 180.0f;
      b.face2.render = true;
      b.face2.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE2DONE]", i);
    }
    // drawY = true;

    bool drawX = false;
    if (getBlock ({ bPos.x + 1, bPos.y, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face3.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face3.position = { bPos.x + 0.5f, bPos.y, bPos.z };
      b.face3.rotation = { 0.0, 0.0, 1.0 };
      b.face3.angle = -90.0f;
      b.face3.render = true;
      b.face3.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE3DONE]", i);
    }

    if (getBlock ({ bPos.x - 1, bPos.y, bPos.z }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face4.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face4.position = { bPos.x - 0.5f, bPos.y, bPos.z };
      b.face4.rotation = { 0.0, 0.0, 1.0 };
      b.face4.angle = 90.0f;
      b.face4.render = true;
      b.face4.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE4DONE]", i);
    }

    bool drawZ = false;
    if (getBlock ({ bPos.x, bPos.y, bPos.z + 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face5.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face5.position = { bPos.x, bPos.y, bPos.z + 0.5f };
      b.face5.rotation = { 1.0, 0.0, 0.0 };
      b.face5.angle = 90.0f;
      b.face5.render = true;
      b.face5.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE5DONE]", i);
    }

    if (getBlock ({ bPos.x, bPos.y, bPos.z - 1 }, blocks) == BLOCK_TYPE::AIR)
    {
      b.face6.model = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
      b.face6.position = { bPos.x, bPos.y, bPos.z - 0.5f };
      b.face6.rotation = { 1.0, 0.0, 0.0 };
      b.face6.angle = -90.0f;
      b.face6.render = true;
      b.face6.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
      println ("BLOCK{}[FACE6DONE]", i);
    }
    b.render = true;
    ++i;

    // println ("BLOCK END");
  }
}

#endif