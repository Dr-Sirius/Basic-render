#ifndef UTIL_H
#define UTIL_H

#include <print>
#include <raylib.h>
#include <string>

/*
Returns string representation of Vector3 vec

vec = {x=20.0f,y=-2.0f,z=5.02478f} returns "(20.00,-2.00,5.02)"
*/
std::string
Vector3String (Vector3 vec)
{
  return std::format ("({:.2f},{:.2f},{:.2f})", vec.x, vec.y, vec.z);
}

/*
Draws text to screen at given Vector2 position with string representation of
Vector3 vec and the name of the vector

A call of DrawVector3({x=20.0f,y=-2.0f,z=5.02478f}, {x=0.0,y=0.0},
"vector") displays:

vector (20.00,-2.00,5.02)

*/
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