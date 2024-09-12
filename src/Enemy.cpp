#include "Enemy.hpp"
#include <iostream>

Enemy::Enemy(const std::string& texture_file) : is_dead_(false), death_timer_started_(false) {
  if (!texture_.loadFromFile(texture_file)) {
    std::cerr << "failed " << std::endl;
  }

  sprite_.setTexture(texture_);
  body_.gravity = 1500.0f;
  body_.terminal_velocity = 2240.0f;
  body_.x_speed = 70.0f;
  body_.y_speed = 0.0f;
  body_.is_moving_left = true;
}

Enemy::~Enemy() {}

void Enemy::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {
  body_.y_speed += body_.gravity * delta_time;
  if (body_.y_speed > body_.terminal_velocity) {
    body_.y_speed = body_.terminal_velocity;
  }

  HandleCollision(collision_bounds);

  if (!is_dead_) {
    if (body_.is_moving_left) {
      sprite_.move(-body_.x_speed * delta_time, body_.y_speed * delta_time);
    } else {
      sprite_.move(body_.x_speed * delta_time, body_.y_speed * delta_time);
    }
  }
}

void Enemy::HandleCollision(const std::vector<sf::FloatRect>& collision_bounds) {
    sf::FloatRect enemy_bounds = sprite_.getGlobalBounds();
    sf::Vector2f position = sprite_.getPosition();

    // First loop: Handle vertical collisions (landing on the ground or hitting the ceiling)
    for (const auto& bound : collision_bounds) {
        if (enemy_bounds.intersects(bound)) {
            // Handle vertical collision (falling onto the ground)
            if (body_.y_speed > 0 && enemy_bounds.top + enemy_bounds.height <= bound.top + body_.y_speed) {
                // Goomba is falling and hits the ground
                if (!is_dead_) sprite_.setPosition(position.x, bound.top - enemy_bounds.height);  // Adjust y-position to land
                body_.y_speed = 0;  // Stop vertical movement
            }
            else if (body_.y_speed < 0 && enemy_bounds.top >= bound.top + bound.height) {
                if (!is_dead_) sprite_.setPosition(position.x, bound.top + bound.height);  // Adjust y-position to stay below ceiling
                body_.y_speed = 0;  // Stop upward movement
            }
        }
    }

    // Second loop: Handle horizontal collisions (hitting walls)
    for (const auto& bound : collision_bounds) {
        if (enemy_bounds.intersects(bound)) {
            // Handle horizontal collision (hitting a wall)
            if (body_.is_moving_left && enemy_bounds.left <= bound.left + bound.width) {
                if (enemy_bounds.top + enemy_bounds.height > bound.top && enemy_bounds.top < bound.top + bound.height) {
                    // Goomba hits a wall on the left
                    body_.is_moving_left = false;  // Turn right
                }
            } else if (!body_.is_moving_left && enemy_bounds.left + enemy_bounds.width >= bound.left) {
                if (enemy_bounds.top + enemy_bounds.height > bound.top && enemy_bounds.top < bound.top + bound.height) {
                    // Goomba hits a wall on the right
                    body_.is_moving_left = true;  // Turn left
                }
            }
        }
    }
}


void Enemy::Draw(sf::RenderWindow& window) {
  window.draw(sprite_);
}

sf::FloatRect Enemy::GetBounds() const {
  return sprite_.getGlobalBounds();
}

bool Enemy::IsDead() {
  if (is_dead_) {
    if (death_timer_.getElapsedTime().asSeconds() >= 2.0f) {
      return true;
    }
  }

  return false;
}

void Enemy::SetPosition(float x, float y) {
  sprite_.setPosition(x, y);
}

sf::Vector2f Enemy::GetPosition() const {
  return sprite_.getPosition();
}

Goomba::Goomba(const std::string& texture_file)
: Enemy(texture_file) {}

void Goomba::OnJumpedOn() {
  is_dead_ = true;
  sprite_.setScale(1.0f, 0.2f);
  if (!death_timer_started_) {
    death_timer_.restart();
    death_timer_started_ = true;
  }
}

KoopaTroopa::KoopaTroopa(const std::string& texture_file)
: Enemy(texture_file) {}

void KoopaTroopa::OnJumpedOn() {
  if (!in_shell_) {
    // Go into shell if not already
    in_shell_ = true;
    // add logic sprite_.setTextureRect();
  } else if (in_shell_ && !is_shell_moving_) {
    // start moving the shell if mario jumps on it
    is_shell_moving_ = true;
    body_.x_speed = 200.0f; // shell speed
  } else {
    // Stop the shell if its moving
    is_shell_moving_ = false;
    body_.x_speed = 0;
  }
}

void KoopaTroopa::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {
  if (in_shell_ && !is_shell_moving_) {
    body_.x_speed = 0.0f;
  }

  Enemy::Update(collision_bounds, delta_time);
}
