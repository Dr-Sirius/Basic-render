#ifndef UTIL_H
#define UTIL_H

#include <print>
#include <raylib.h>
#include <string>

void
DrawVector3 (Vector3 vec, Vector2 pos, std::string name)
{
  std::string str =
    std::format ("{} ({:.2f},{:.2f},{:.2f})", name, vec.x, vec.y, vec.z);
  DrawText (str.c_str (), pos.x, pos.y, 15, BLACK);
}

float
GetGrayScale (Color color)
{
  return (color.r + color.g + color.b) / 3.0f;
}

#endif