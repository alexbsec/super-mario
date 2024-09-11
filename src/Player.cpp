#include "Player.hpp"

Player::Player(const std::string& texture_file) {
  if (!player_texture_.loadFromFile(texture_file)) {
    // handle
  }

  player_sprite_.setTexture(player_texture_);

  body_.gravity = 0.002f;
  body_.terminal_velocity = 0.4f;
  body_.x_speed = 0.0f;
  body_.y_speed = 0.0f;
  body_.jump_speed = 0.65f;
  body_.is_jumping = false;

  player_sprite_.setPosition(10, 50);
}

void Player::HandleInput() {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    body_.x_speed = -0.1f;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    body_.x_speed = 0.1f;
  }


  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !body_.is_jumping) {
    body_.y_speed = -body_.jump_speed;
    body_.is_jumping = true;
  }
}

void Player::Update(const std::vector<sf::FloatRect>& collision_bounds) {
  // Gravity
  body_.y_speed += body_.gravity;
  if (body_.y_speed >= body_.terminal_velocity) {
    body_.y_speed = body_.terminal_velocity;
  }

  player_sprite_.move(0.0f, body_.y_speed);

  // Horizontal movement check (left-right)
  sf::Vector2f player_position = player_sprite_.getPosition();
  sf::FloatRect player_bounds = player_sprite_.getGlobalBounds();

    // Vertical movement check (up-down)
  for (const auto& bound : collision_bounds) {
    if (player_sprite_.getGlobalBounds().intersects(bound)) {
      if (body_.y_speed > 0) {
        player_sprite_.setPosition(player_position.x, bound.top - player_bounds.height);
        body_.y_speed = 0;
        body_.is_jumping = false;
      } else if (body_.y_speed < 0) {
        player_sprite_.setPosition(player_position.x, bound.top + bound.height);
        body_.y_speed = 0;
      }
      break;
    }
  }

  player_sprite_.move(body_.x_speed, 0.0f);

  player_position = player_sprite_.getPosition();
  player_bounds = player_sprite_.getGlobalBounds();


  for (const auto& bound : collision_bounds) {
    if (player_bounds.intersects(bound)) {
      // Move back if a collision is detected
      if (body_.x_speed > 0) {
        if (player_bounds.top + player_bounds.height > bound.top && player_bounds.top < bound.top + bound.height) {
          player_sprite_.setPosition(bound.left - player_bounds.width, player_position.y);
        }
      } else if (body_.x_speed < 0) {
        if (player_bounds.top + player_bounds.height > bound.top && player_bounds.top < bound.top + bound.height) {
          player_sprite_.setPosition(bound.left + bound.width, player_position.y);
        }
      }
      body_.x_speed = 0;
      break;
    }
  }

  body_.x_speed = 0;

}

void Player::Draw(sf::RenderWindow& window) {
  window.draw(player_sprite_);
}
