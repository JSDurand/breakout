#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "object.h"
#include "sprite.h"
#include "resource_manager.h"

class GameLevel
{
public:
  std::vector<GameObject> Bricks;

  GameLevel() {};

  void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

  void Draw(SpriteRenderer &renderer);

  GLboolean IsCompleted();

private:
  void init(std::vector<std::vector<GLuint> > tileData, GLuint levelWidth, GLuint levelHeight);
};
