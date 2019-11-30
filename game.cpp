#include "game.h"
#include "resource_manager.h"
#include "sprite.h"
#include "ball.h"
#include "particle.h"
#include "text_renderer.h"
#include <sstream>

SpriteRenderer *Renderer;

GameObject *Player;

Ball *Boule;

ParticleGenerator *Particles;

TextRenderer *Text;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game() {
  delete Renderer;
  delete Player;
  delete Boule;
  delete Particles;
}

void Game::Init()
{
  // resources
  ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "sprite");
  ResourceManager::LoadShader("particle.vs", "particle.fs", nullptr, "particle");

  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                    static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
  ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
  ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
    
  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

  ResourceManager::LoadTexture("resource/background.jpg", GL_FALSE, "background");
  ResourceManager::LoadTexture("resource/cutecat.png", GL_TRUE, "face");
  ResourceManager::LoadTexture("resource/block.png", GL_FALSE, "block");
  ResourceManager::LoadTexture("resource/container.jpg", GL_FALSE, "block_solid");
  ResourceManager::LoadTexture("resource/paddle.png", GL_TRUE, "paddle");
  ResourceManager::LoadTexture("resource/blending_transparent_window.png", GL_TRUE, "particle");

  // levels
  GameLevel one, two, three, four;
  one.Load("levels/standard.txt", this->Width, this->Height * 0.5f);
  two.Load("levels/gaps.txt", this->Width, this->Height * 0.5f);
  three.Load("levels/invader.txt", this->Width, this->Height * 0.5f);
  four.Load("levels/bounce.txt", this->Width, this->Height * 0.5f);

  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Level = 2;

  // player
  glm::vec2 PlayerPos(this->Width / 2 - PLAYER_SIZE.x / 2,
                      this->Height - PLAYER_SIZE.y);

  this->Lives = 3;

  Player = new GameObject(PlayerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

  // ball
  glm::vec2 ballPos = PlayerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
  Boule = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                   ResourceManager::GetTexture("face"));

  // partilces
  Particles = new ParticleGenerator(ResourceManager::GetShader("particle"),
                                    ResourceManager::GetTexture("particle"),
                                    500);

  // text
  Text = new TextRenderer(this->Width, this->Height);
  Text->Load("fonts/Cardo104s.ttf", 24);
}

void Game::Update(GLfloat dt)
{
  if (this->State == GAME_ACTIVE) {
    Boule->Move(dt, this->Width);

    this->DoCollisions();

    Particles->Update(dt, *Boule, 2, glm::vec2(Boule->Radius / 2));

    if (Boule->Position.y >= this->Height) {
      --this->Lives;
      if (this->Lives <= 0) {
        this->ResetLevel();
        this->State = GAME_MENU;
      }
      this->ResetPlayer();
    }

    if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted()) {
      this->ResetLevel();
      this->ResetPlayer();
      this->State = GAME_WIN;
    }
  }
}


void Game::ProcessInput(GLfloat dt)
{
  if (this->Keys[GLFW_KEY_S])
    this->Stop();

  if (this->State == GAME_MENU) {
    if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
      this->State = GAME_ACTIVE;
      this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
    }
    if (this->Keys[GLFW_KEY_J] && !this->KeysProcessed[GLFW_KEY_J]) {
      this->Level = (this->Level + 3) % 4;
      this->KeysProcessed[GLFW_KEY_J] = GL_TRUE;
    }
    if (this->Keys[GLFW_KEY_K] && !this->KeysProcessed[GLFW_KEY_K]) {
      this->Level = (this->Level + 1) % 4;
      this->KeysProcessed[GLFW_KEY_K] = GL_TRUE;
    }
  }
  
  if (this->State == GAME_ACTIVE) {

    if (this->Keys[GLFW_KEY_Z])
      this->State = GAME_WIN;

    GLfloat velocity = PLAYER_VELOCITY * dt;

    if (this->Keys[GLFW_KEY_LEFT]) {
      if (Player->Position.x >= 0) {
        Player->Position.x -= velocity;
        if (Boule->Stuck)
          Boule->Position.x -= velocity;
      }
    }

    if (this->Keys[GLFW_KEY_RIGHT]) {
      if (Player->Position.x <= this->Width - Player->Size.x) {
        Player->Position.x += velocity;
        if (Boule->Stuck)
          Boule->Position.x += velocity;
      }
    }

    if (this->Keys[GLFW_KEY_SPACE])
      Boule->Stuck = false;
  }

  if (this->State == GAME_WIN) {
    if (this->Keys[GLFW_KEY_ENTER]) {
      this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
      this->State = GAME_MENU;
    }
  }
}

void Game::Render()
{
  if (this->State == GAME_ACTIVE || this->State == GAME_STOP ||
      this->State == GAME_MENU) {
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0),
                         glm::vec2(this->Width, this->Height), 0.0f);

    this->Levels[this->Level].Draw(*Renderer);

    Player->Draw(*Renderer);
    Particles->Draw();
    Boule->Draw(*Renderer);

    std::stringstream ss;
    ss << this->Lives;
    Text->RenderText("Lives: " + ss.str(), 10, 10, 2, glm::vec3(1.0f, 0.07f, 0.3f));
  }

  if (this->State == GAME_MENU) {
    Text->RenderText("Press ENTER to start.", 300.0f, this->Height / 2, 2, glm::vec3(1.0f, 0.0f, 0.0f));
    Text->RenderText("Press J or K to select levels.", 325.0f, this->Height / 2 + 40, 1.5f,
                     glm::vec3(1.0f, 0.0f, 0.0f));
  }

  if (this->State == GAME_WIN) {
    Text->RenderText("You won!", 200.0f, Height / 2 - 40, 2, glm::vec3(0.0f, 0.3f, 1.0f));
    Text->RenderText("Press ENTER to retry or A to quit!", 200.0f, Height / 2, 1.5f, glm::vec3(1.0f, 0.0f, 1.0f));
  }
}

// GLboolean CheckCollision(GameObject &one, GameObject &two)
// {
//   bool colX = one.Position.x + one.Size.x >= two.Position.x &&
//     two.Position.x + two.Size.x >= one.Position.x;

//   bool colY = one.Position.y + one.Size.y >= two.Position.y &&
//     two.Position.y + two.Size.y >= one.Position.y;

//   return colX && colY;
// }

Direction VectorDirection(glm::vec2 v)
{
  glm::vec2 compass[] =
    {
     glm::vec2(0.0f, 1.0f),
     glm::vec2(0.0f, -1.0f),
     glm::vec2(-1.0f, 0.0f),
     glm::vec2(1.0f, 0.0f)
    };

  GLfloat max = 0.0f;
  GLuint best = -1;
  for (GLuint i = 0; i < 4; ++i) {
    GLfloat dot = glm::dot(glm::normalize(v), compass[i]);
    if (dot > max) {
      max = dot;
      best = i;
    }
  }

  return (Direction)best;
}

Collision CheckCollision(Ball &one, GameObject &two)
{
  glm::vec2 ball_center  = one.Position + one.Radius;
  glm::vec2 half_extents = glm::vec2(two.Size.x / 2, two.Size.y / 2);
  glm::vec2 aabb_center  = two.Position + half_extents;

  glm::vec2 difference = ball_center - aabb_center;
  glm::vec2 clamped    = glm::clamp(difference, -half_extents, half_extents);
  glm::vec2 closest    = aabb_center + clamped;

  difference = ball_center - closest;

  if (glm::length(difference) <= one.Radius) 
    return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
  else
    return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f));
}

void Game::DoCollisions()
{
  // with boxes
  for (GameObject &box : this->Levels[this->Level].Bricks) {
    if (!box.Destroyed) {
      Collision collision = CheckCollision(*Boule, box);

      if (std::get<0>(collision)) {
        if (!box.IsSolid)
          box.Destroyed = GL_TRUE;

        Direction dir  = std::get<1>(collision);
        glm::vec2 diff = std::get<2>(collision);

        if (dir == LEFT || dir == RIGHT) {
          Boule->Velocity.x = -Boule->Velocity.x;
          GLfloat penetration = Boule->Radius - std::abs(diff.x);

          if (dir == LEFT)
            Boule->Position.x -= penetration;
          else
            Boule->Position.x += penetration;
        } else {
          Boule->Velocity.y = -Boule->Velocity.y;
          GLfloat penetration = Boule->Radius - std::abs(diff.y);

          if (dir == DOWN)
            Boule->Position.y -= penetration;
          else
            Boule->Position.y += penetration;
        }
      }
    }
  }

  // with player
  Collision result = CheckCollision(*Boule, *Player);

  if (!Boule->Stuck && std::get<0>(result)) {
    GLfloat boardCenter = Player->Position.x + Player->Size.x / 2;
    GLfloat distance = Boule->Position.x + Boule->Radius - boardCenter;
    GLfloat percentage = 2 * distance / Player->Size.x;

    GLfloat strength = 2.0f;

    glm::vec2 oldVel = Boule->Velocity;
    Boule->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
    Boule->Velocity.y = -1.0f * std::abs(Boule->Velocity.y);
    Boule->Velocity   = glm::normalize(Boule->Velocity) * glm::length(oldVel);
  }
}

void Game::ResetLevel()
{
  this->Lives = 3;
  if (this->Level == 0)
    this->Levels[0].Load("levels/standard.txt", this->Width, this->Height * 0.5f);
  else if (this->Level == 1)
    this->Levels[1].Load("levels/gaps.txt", this->Width, this->Height * 0.5f);
  else if (this->Level == 2)
    this->Levels[2].Load("levels/invader.txt", this->Width, this->Height * 0.5f);
  else if (this->Level == 3)
    this->Levels[3].Load("levels/bounce.txt", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
  Player->Size     = PLAYER_SIZE;
  Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
  Boule->Reset(Player->Position +
               glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

void Game::Stop()
{
  if (this->State == GAME_STOP)
    this->State = GAME_ACTIVE;
  else if (this->State == GAME_ACTIVE)
    this->State = GAME_STOP;
}
