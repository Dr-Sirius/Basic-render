#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include <vector>

struct Entity
{
  Model model;
  Vector3 Position;
  void
  DrawEntity ()
  {
    DrawModel (model, Position, 2.0f, WHITE);
  }
};

typedef std ::vector<Entity*> World;

#endif