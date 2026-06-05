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
      if (debug)
      {
        if (type == BLOCK_TYPE::GROUND)
        {
          DrawCubeWires (position, 1.0f, 1.0f, 1.0f, MAGENTA);
          return;
        }
        DrawCubeWires (position, 1.0f, 1.0f, 1.0f, colliderCol);
      }
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
  Model plane = LoadModelFromMesh (GenMeshPlane (1.0f, 1.0f, 1, 1));
  for (Block& b : blocks)
  {

    if (b.type == BLOCK_TYPE::AIR)
    {
      b.render = false;
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
      println ("BLOCK{}[FACE6DONE]", i);
    }
    b.render = true;
    ++i;

    // println ("BLOCK END");
  }
}

std::vector<Block>*
Build (Image noise)
{
  std::vector<Block>* ents = new std::vector<Block> (14400);

  Color* pixel = LoadImageColors (noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x)
  {
    for (int z = 0; z < height; ++z)
    {
      float normalizedHeight = GetGrayScale (pixel[z * width + x]);
      int yHeight = lround (normalizedHeight * 0.05f);
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

#endif