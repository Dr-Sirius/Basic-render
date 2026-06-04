#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <print>
#include <raylib.h>
#include <vector>

#include "Util.h"

using std::print;
using std::println;

enum BLOCK_TYPE
{
  AIR,
  GROUND,
};

struct Block
{
  // Mesh block;
  Model model;
  Vector3 position;
  BLOCK_TYPE type;
  bool render = true;

  void
  DrawBlock ()
  {
    if (render)
      DrawModel (model, position, 1.0f, WHITE);
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

#endif