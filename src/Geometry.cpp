#include "Geometry.h"

Geometry::Geometry(std::vector<glm::vec2>& v, std::vector<glm::vec3>& c)
  : vertexBuffer(0)
  , colourBuffer(0)
  , vertexArray(0)
  , vertices(v)
  , colours(c)
{
}