#ifndef GLOBALS_H
#define GLOBALS_H

#include <raylib.h>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, Texture> TextureMap;
std::unordered_map<std::string, Model> ModelMap;

Camera* playerCamera;

int ScreenWidth;
int ScreenHeight;

void BuildTextureMap() {
  // build dirt
  Image dirtText = LoadImage("dirt.png");
  ImageResize(&dirtText, dirtText.width * 0.0625, dirtText.height * 0.0625);

  // build grass
  Image grassText = LoadImage("grass.png");
  ImageResize(&grassText, grassText.width * 0.0625, grassText.height * 0.0625);

  // convert to gpu textures
  Texture2D tx = LoadTextureFromImage(dirtText);
  Texture2D grass = LoadTextureFromImage(grassText);

  TextureMap.insert({"DIRT", tx});
  TextureMap.insert({"GRASS", grass});
}

void BuildModelMap() {
  ModelMap.insert({"DIRT", LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1))});

  ModelMap.insert({"GRASS", LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1))});
}

const Texture& GetTexture(std::string name) {
  if (TextureMap.contains(name)) { return TextureMap[name]; }
  return TextureMap.end()->second;
}

const Model& GetModel(std::string name) {
  if (ModelMap.contains(name)) { return ModelMap[name]; }
  return ModelMap.end()->second;
}

#endif