#include "ball.h"

Ball::Ball(): GameObject(), Radius(12.5f), Stuck(true) {}

Ball::Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
  : GameObject(pos, glm::vec2(2*radius), sprite, glm::vec3(1.0f), velocity),
    Radius(12.5f), Stuck(true) {}

glm::vec2 Ball::Move(GLfloat dt, GLuint window_width)
{
  if (!this->Stuck) {
    this->Position += this->Velocity * dt;

    if (this->Position.x < 0.0f) {
      this->Velocity.x = -this->Velocity.x;
      this->Position.x = 0.0f;
    } else if (this->Position.x > window_width - this->Size.x) {
      this->Velocity.x = -this->Velocity.x;
      this->Position.x = window_width - this->Size.x;
    }

    if (this->Position.y <= 0.0f) {
      this->Velocity.y = -this->Velocity.y;
      this->Position.y = 0.0f;
    }
  }

  return this->Position;
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
  this->Position = position;
  this->Velocity = velocity;
  this->Stuck    = true;
}
