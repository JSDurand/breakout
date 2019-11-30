/*******************************************************************
 ** This code is part of Breakout.
 **
 ** Breakout is free software: you can redistribute it and/or modify
 ** it under the terms of the CC BY 4.0 license as published by
 ** Creative Commons, either version 4 of the License, or (at your
 ** option) any later version.
 ******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>

#include "level.h"

enum Direction { UP, DOWN, LEFT, RIGHT };

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(300.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const GLfloat BALL_RADIUS(12.5f);

enum GameState
  {
   GAME_ACTIVE,
   GAME_STOP,
   GAME_MENU,
   GAME_WIN
  };

class Game
{
public:
  // Game state
  GameState              State;	
  GLboolean              Keys[1024];
  GLboolean              KeysProcessed[1024];
  GLuint                 Width, Height;
  GLuint Lives;

  std::vector<GameLevel> Levels;
  GLuint Level;
  // Constructor/Destructor
  Game(GLuint width, GLuint height);
  ~Game();
  
  void Init();

  void DoCollisions();

  void ResetLevel();
  void ResetPlayer();

  void Stop();
  
  void ProcessInput(GLfloat dt);
  void Update(GLfloat dt);
  void Render();
};

#endif
