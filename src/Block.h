#ifndef BLOCK_H
#define BLOCK_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <print>
#include <unordered_map>
#include <vector>

#include "Globals.h"
#include "Util.h"

using std::print;
using std::println;

enum BLOCK_TYPE {
  AIR,
  GROUND,
};

struct Face {
  Model model;
  Vector3 position;
  Vector3 rotation;
  float angle;
  bool render;
  void DrawFace() {
    if (render) {
      DrawModelEx(model, position, rotation, angle, Vector3One(), WHITE);
    }
  }
};

struct Block {
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

  void DrawBlock() {
    Vector2 screenSpace = GetWorldToScreen(position, *playerCamera);
    if ((screenSpace.x > ScreenWidth * 1.2f ||
         screenSpace.x < -ScreenWidth * 1.2f) &&
        (screenSpace.y > ScreenHeight * 1.2f ||
         screenSpace.y < -ScreenHeight * 1.2f))
      return;
    if (render || type != BLOCK_TYPE::AIR) {
      topFace.DrawFace();
      bottomFace.DrawFace();
      frontFace.DrawFace();
      backFace.DrawFace();
      rightFace.DrawFace();
      leftFace.DrawFace();
    }
    if (debug) {
      if (!render) colliderCol = YELLOW;
      DrawCubeWires(position, 1.0f, 1.0f, 1.0f, colliderCol);
    }
    // DrawModel (model, position, 1.0f, WHITE);
  }

  void Update() {
    if (IsKeyPressed(KEY_F3)) { debug = !debug; }
    if (IsKeyPressed(KEY_F5)) { colliderCol = GREEN; }
    if (IsKeyPressed(KEY_F6)) { colliderCol = WHITE; }
  }

  bool IsFacesNotRendered() {
    return !topFace.render && !bottomFace.render && !frontFace.render &&
           !backFace.render && !rightFace.render && !leftFace.render;
  }
};

Block& getBlock(Vector3 vec, std::unordered_map<std::string, Block>& blocks) {
  auto it = blocks.find(Vector3String(vec));
  if (it == blocks.end()) return blocks.end()->second;
  return it->second;
}

/*
Returns the BLOCK_TYPE of the block at positition Vector3 vec using the
Vector3String representation
*/
BLOCK_TYPE
getBlockType(Vector3 vec, std::unordered_map<std::string, Block>& blocks) {
  auto it = blocks.find(Vector3String(vec));
  if (it == blocks.end()) return BLOCK_TYPE::AIR;
  return it->second.type;
}

/*
Returns a string representation of a BLOCK_TYPE
*/
std::string blockString(BLOCK_TYPE block) {
  switch (block) {
    case GROUND:
      return "GROUND";
      break;

    default:
      return "AIR";
      break;
  }
}


// Enables the given face's rendering with given parameters
void EnableFace(Face& face, const Model& model, const Texture& tx, Vector3 pos,
                Vector3 rot, float angle) {
  face.model = model;
  face.position = pos;
  face.rotation = rot;
  face.angle = -angle;
  face.render = true;
  face.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
}

/*
Iterates through the map of Blocks and determines the render elegibility.
Elegible faces are set to their correct model and render is set true
*/
void SetAndDetermineRender2(std::unordered_map<std::string, Block>& blocks) {
  const Texture2D tx = GetTexture("DIRT");
  const Texture2D grass = GetTexture("GRASS");
  const Model plane = GetModel("DIRT");
  const Model grassPlane = GetModel("GRASS");

  for (auto& [_, b] : blocks) {
    Vector3 pos = b.position;

    b.render = false;
    if (b.type == BLOCK_TYPE::AIR) { continue; }

    b.topFace.render = false;
    b.bottomFace.render = false;
    b.frontFace.render = false;
    b.backFace.render = false;
    b.rightFace.render = false;
    b.leftFace.render = false;

    if (getBlockType({pos.x, pos.y + 1, pos.z}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.topFace, grassPlane, grass, {pos.x, pos.y + 0.5f, pos.z},
                 {1.0, 0.0, 0.0}, 0.0f);

      b.render = true;
      // println ("BLOCK{}[FACE1DONE]", i);
    }

    if (getBlockType({pos.x, pos.y - 1, pos.z}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.bottomFace, plane, tx, {pos.x, pos.y - 0.5f, pos.z},
                 {1.0, 0.0, 0.0}, 180.0f);
      b.render = true;
      // println ("BLOCK{}[FACE2DONE]", i);
    }

    if (getBlockType({pos.x + 1, pos.y, pos.z}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.frontFace, plane, tx, {pos.x + 0.5f, pos.y, pos.z},
                 {0.0, 0.0, 1.0}, 90.0f);
      b.render = true;
      // println ("BLOCK{}[FACE3DONE]", i);
    }

    if (getBlockType({pos.x - 1, pos.y, pos.z}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.backFace, plane, tx, {pos.x - 0.5f, pos.y, pos.z},
                 {0.0, 0.0, 1.0}, -90.0f);
      b.render = true;
      // println ("BLOCK{}[FACE4DONE]", i);
    }

    if (getBlockType({pos.x, pos.y, pos.z + 1}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.rightFace, plane, tx, {pos.x, pos.y, pos.z + 0.5f},
                 {1.0, 0.0, 0.0}, -90.0f);
      b.render = true;
      // println ("BLOCK{}[FACE5DONE]", i);
    }

    if (getBlockType({pos.x, pos.y, pos.z - 1}, blocks) == BLOCK_TYPE::AIR) {
      EnableFace(b.leftFace, plane, tx, {pos.x, pos.y, pos.z - 0.5f},
                 {1.0, 0.0, 0.0}, 90.0f);
      b.render = true;
      // println ("BLOCK{}[FACE6DONE]", i);
    }
    //++i;

    // println ("BLOCK END");
  }
}

void HandleDestruction(std::unordered_map<std::string, Block>& blocks,
                       Vector3 bPos) {
  const Texture2D tx = GetTexture("DIRT");
  const Texture2D grass = GetTexture("GRASS");
  const Model plane = GetModel("DIRT");
  const Model grassPlane = GetModel("GRASS");

  if (getBlockType({bPos.x, bPos.y + 1, bPos.z}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x, bPos.y + 1, bPos.z}, blocks);

    Vector3 nFPos = {block.position.x, block.position.y - 0.5f,
                     block.position.z};
    EnableFace(block.bottomFace, plane, tx, nFPos, {1.0, 0.0, 0.0}, 180.0f);
    block.render = true;
  }
  if (getBlockType({bPos.x, bPos.y - 1, bPos.z}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x, bPos.y - 1, bPos.z}, blocks);
    Vector3 nFPos = {block.position.x, block.position.y + 0.5f,
                     block.position.z};
    EnableFace(block.topFace, grassPlane, grass, nFPos, {1.0, 0.0, 0.0}, 0.0f);
    block.render = true;
  }

  // b f
  if (getBlockType({bPos.x - 1, bPos.y, bPos.z}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x - 1, bPos.y, bPos.z}, blocks);
    Vector3 nFPos = {block.position.x + 0.5f, block.position.y,
                     block.position.z};
    EnableFace(block.frontFace, plane, tx, nFPos, {0.0, 0.0, 1.0}, 90.0f);
    block.render = true;
  }
  if (getBlockType({bPos.x + 1, bPos.y, bPos.z}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x + 1, bPos.y, bPos.z}, blocks);
    Vector3 nFPos = {block.position.x - 0.5f, block.position.y,
                     block.position.z};
    EnableFace(block.backFace, plane, tx, nFPos, {0.0, 0.0, 1.0}, -90.0f);
    block.render = true;
  }

  // l r
  if (getBlockType({bPos.x, bPos.y, bPos.z - 1}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x, bPos.y, bPos.z - 1}, blocks);
    Vector3 nFPos = {block.position.x, block.position.y,
                     block.position.z + 0.5f};
    EnableFace(block.rightFace, plane, tx, nFPos, {1.0, 0.0, 0.0}, -90.0f);
    block.render = true;
  }
  if (getBlockType({bPos.x, bPos.y, bPos.z + 1}, blocks) != BLOCK_TYPE::AIR) {
    Block& block = getBlock({bPos.x, bPos.y, bPos.z + 1}, blocks);
    Vector3 nFPos = {block.position.x, block.position.y,
                     block.position.z - 0.5f};
    EnableFace(block.leftFace, plane, tx, nFPos, {1.0, 0.0, 0.0}, 90.0f);
    block.render = true;
  }
}

bool DetermineDisable(std::unordered_map<std::string, Block>& blocks,
                      Vector3 pos) {
  return getBlockType(pos, blocks) != BLOCK_TYPE::AIR;
}

void HandleCreation(std::unordered_map<std::string, Block>& blocks,
                    Vector3 bPos) {
  const Texture2D tx = GetTexture("DIRT");
  const Texture2D grass = GetTexture("GRASS");
  const Model plane = GetModel("DIRT");
  const Model grassPlane = GetModel("GRASS");

  Block& target = getBlock(bPos, blocks);

  if (DetermineDisable(blocks, {bPos.x, bPos.y + 1, bPos.z})) {
    getBlock({bPos.x, bPos.y + 1, bPos.z}, blocks).bottomFace.render = false;

  } else {
    EnableFace(target.topFace, grassPlane, grass,
               {bPos.x, bPos.y + 0.5f, bPos.z}, {1.0, 0.0, 0.0}, 0.0f);
  }

  if (DetermineDisable(blocks, {bPos.x, bPos.y - 1, bPos.z})) {
    getBlock({bPos.x, bPos.y - 1, bPos.z}, blocks).topFace.render = false;

  } else {
    EnableFace(target.bottomFace, plane, tx, {bPos.x, bPos.y - 0.5f, bPos.z},
               {1.0, 0.0, 0.0}, 180.0f);
  }

  // b f
  if (DetermineDisable(blocks, {bPos.x - 1, bPos.y, bPos.z})) {
    getBlock({bPos.x - 1, bPos.y, bPos.z}, blocks).frontFace.render = false;

  } else {
    EnableFace(target.backFace, plane, tx, {bPos.x - 0.5f, bPos.y, bPos.z},
               {0.0, 0.0, 1.0}, -90.0f);
  }

  if (DetermineDisable(blocks, {bPos.x + 1, bPos.y, bPos.z})) {
    getBlock({bPos.x + 1, bPos.y, bPos.z}, blocks).backFace.render = false;

  } else {
    EnableFace(target.frontFace, plane, tx, {bPos.x + 0.5f, bPos.y, bPos.z},
               {0.0, 0.0, 1.0}, 90.0f);
  }

  // l r
  if (DetermineDisable(blocks, {bPos.x, bPos.y, bPos.z - 1})) {
    getBlock({bPos.x, bPos.y, bPos.z - 1}, blocks).rightFace.render = false;

  } else {
    EnableFace(target.leftFace, plane, tx, {bPos.x, bPos.y, bPos.z - 0.5f},
               {1.0, 0.0, 0.0}, 90.0f);
  }
  if (DetermineDisable(blocks, {bPos.x, bPos.y, bPos.z + 1})) {
    getBlock({bPos.x, bPos.y, bPos.z + 1}, blocks).leftFace.render = false;

  } else {
    EnableFace(target.rightFace, plane, tx, {bPos.x, bPos.y, bPos.z + 0.5f},
               {1.0, 0.0, 0.0}, -90.0f);
  }

  target.render = true;
}

void DetermineRerender(std::unordered_map<std::string, Block>& blocks,
                       Vector3 bPos, bool dest = true) {
  if (dest) {
    HandleDestruction(blocks, bPos);
  } else {
    HandleCreation(blocks, bPos);
  }
}

void HandleBlockColl(Ray ray, RayCollision coll, std::unordered_map<std::string, Block>& blocks) {
  if (coll.hit) {
    Vector3 bpos = coll.point;
    Vector3 min = {bpos.x - 0.5f, bpos.y - 0.5f, bpos.z - 0.5f};
    Vector3 max = {min.x + 1.0f, min.y + 1.0f, min.z + 1.0f};
    DrawBoundingBox({min, max}, WHITE);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (blocks.contains(Vector3String(coll.point))) {
        blocks.erase(Vector3String(coll.point));

        DetermineRerender(blocks, coll.point);
      }
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector3 nBlock = Vector3Add(coll.point, coll.normal);
      nBlock = Vector3Floor(nBlock);
      std::string nPos = Vector3String(nBlock);

      if (blocks.contains(nPos)) {
        blocks[nPos].type = BLOCK_TYPE::GROUND;
      } else {
        blocks.insert({nPos, Block{.topFace = {0},
                                 .bottomFace = {0},
                                 .frontFace = {0},
                                 .backFace = {0},
                                 .rightFace = {0},
                                 .leftFace = {0},
                                 .position = nBlock,
                                 .type = BLOCK_TYPE::GROUND}});
      }
      DetermineRerender(blocks, nBlock, false);
    }
  }
}


/*
Builds block terrain from the given noise and returns an unordered map of blocks

The returned map is of type std::unordered_map<std::string, Block>

The keys are string versions of the block's Vector3 position using the
Vector3String function
*/
std::unordered_map<std::string, Block> Build2(Image noise) {
  std::unordered_map<std::string, Block> ents;

  Color* pixel = LoadImageColors(noise);

  int height = noise.height;
  int width = noise.width;

  for (int x = 0; x < width; ++x) {
    for (int z = 0; z < height; ++z) {
      float normalizedHeight = GetGrayScale(pixel[z * width + x]);
      int yHeight = lround(normalizedHeight * 0.1f);
      // println ("Y SIZE {}", yHeight);
      for (int y = 0; y < yHeight; ++y) {
        Block en = Block{.topFace = {0},
                         .bottomFace = {0},
                         .frontFace = {0},
                         .backFace = {0},
                         .rightFace = {0},
                         .leftFace = {0},
                         .position = Vector3{(float) x, (float) y, (float) z},
                         .type = BLOCK_TYPE::GROUND};

        // en.model.materials->maps[MATERIAL_MAP_DIFFUSE].texture = tx;
        ents.insert({Vector3String(en.position), en});
      }
    }
  }
  return ents;
}

RayCollision CheckCol(std::unordered_map<std::string, Block>& blocks, Ray ray,
                      bool drawDebug = false) {
  float maxDist = MAXFLOAT;
  Vector3 closeBlockPos = {-1};
  bool blockHit = false;
  Vector3 colNorm = {};
  RayCollision coll = {0};
  for (auto& [pos, block] : blocks) {
    if (block.type == BLOCK_TYPE::AIR || !block.render ||
        block.IsFacesNotRendered())
      continue;
    Vector3 bpos = block.position;
    Vector3 min = {bpos.x - 0.5f, bpos.y - 0.5f, bpos.z - 0.5f};
    Vector3 max = {min.x + 1.0f, min.y + 1.0f, min.z + 1.0f};
    BoundingBox bound = {min, max};
    coll = GetRayCollisionBox(ray, bound);
    // DrawSphere (coll.point, 0.5f, RED);
    // DrawBoundingBox (bound, RED);
    // DrawLine3D (ray.position, coll.point, RED);
    if (coll.hit && coll.distance < maxDist) {
      maxDist = coll.distance;
      closeBlockPos = bpos;
      colNorm = coll.normal;
      blockHit = true;

      coll.point = bpos;

      // println ("BLOCK POS {} COLL POS {} BLOCK NORM {}",
      //          Vector3String (block.position),
      //          Vector3String (coll.point),
      //          Vector3String (Vector3Add (bpos, colNorm)));
      if (drawDebug) {
        DrawBoundingBox(bound, WHITE);
        DrawLine3D(bpos, Vector3Add(bpos, coll.normal), RED);
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