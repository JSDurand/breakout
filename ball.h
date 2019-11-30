#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "sprite.h"
#include "object.h"

class Ball : public GameObject
{
public:
  GLfloat Radius;
  GLboolean Stuck;

  Ball();
  Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

  glm::vec2 Move(GLfloat dt, GLuint window_width);
  void Reset(glm::vec2 position, glm::vec2 velocity);
};
