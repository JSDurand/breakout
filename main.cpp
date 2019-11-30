#define GL_SILENCE_DEPRECATION
// #define __gl_h_
// #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "resource_manager.h"
#include "sprite.h"
#include "object.h"
#include "level.h"
#include "ball.h"
#include "text_renderer.h"

const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 768;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

Game Breakout(WINDOW_WIDTH, WINDOW_HEIGHT);

int main(int argc, char **argv)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout", nullptr, nullptr);

  if (window == nullptr) {
    cout << "cannot create window: " << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "cannot load GLAD" << endl;
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Breakout.Init();

  GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

  Breakout.State = GAME_MENU;

  while (!glfwWindowShouldClose(window)) {
    while (glfwGetTime() - lastFrame < 1.0f / 50.0f) {}

    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();

    Breakout.ProcessInput(deltaTime);
    Breakout.Update(deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Breakout.Render();

    glfwSwapBuffers(window);
  }

  ResourceManager::Clear();

  glfwTerminate();
  return 0;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key == GLFW_KEY_A && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      Breakout.Keys[key] = GL_TRUE;
    else if (action == GLFW_RELEASE) {
      Breakout.Keys[key] = GL_FALSE;
      Breakout.KeysProcessed[key] = GL_FALSE;
    }
  }
}
