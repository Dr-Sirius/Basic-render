#ifndef UTIL_H
#define UTIL_H

#include <print>
#include <raylib.h>
#include <string>

std::string
Vector3String (Vector3 vec)
{
  return std::format ("({:.2f},{:.2f},{:.2f})", vec.x, vec.y, vec.z);
}

void
DrawVector3 (Vector3 vec, Vector2 pos, std::string name)
{
  std::string str = name + " " + Vector3String (vec);
  DrawText (str.c_str (), pos.x, pos.y, 15, WHITE);
}

float
GetGrayScale (Color color)
{
  return (color.r + color.g + color.b) / 3.0f;
}

int
PointToIndex (Vector3 vec)
{
  return vec.x + vec.z * 32 + vec.y * 32 * 32;
}

#endif