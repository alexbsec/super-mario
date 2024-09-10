#include "Player.hpp"

Player::Player(const std::string& texture_file) {
  if (!player_texture_.loadFromFile(texture_file)) {
    // handle
  }

  player_sprite_.setTexture(player_texture_);
  is_jumping_ = false;
  velocity_y_ = 0.0f;
}

void Player::HandleInput() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    player_sprite_.move(-0.5f, 0.0f);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    player_sprite_.move(0.5f, 0.0f);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !is_jumping_) {
    velocity_y_ = -5.0f;
    is_jumping_ = true;
  }
}

void Player::Update() {
  // Gravity
  velocity_y_ += 0.1f;
  player_sprite_.move(0, velocity_y_);

  if (player_sprite_.getPosition().y > 500) {
    // Ground collision
    player_sprite_.setPosition(player_sprite_.getPosition().x, 500);
    velocity_y_ = 0;
    is_jumping_ = false;
  }
}

void Player::Draw(sf::RenderWindow& window) {
  window.draw(player_sprite_);
}
