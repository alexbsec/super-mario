#include "Player.hpp"

void Player::CheckXMovement_() {
  if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::X)) body_.x_speed = 0;
  if (body_.is_accelerating && !sf::Keyboard::isKeyPressed(sf::Keyboard::X)) body_.is_accelerating = false;
  if (!body_.is_accelerating) body_.x_speed = 0;

}

void Player::PlayDeathAnimation_(float delta_time) {
  if (!has_died_ && jump_dead_) {
    has_died_ = true;
    body_.x_speed = 0;
    body_.y_speed = -body_.jump_speed;
  }

  body_.y_speed += body_.gravity * delta_time;
  if (body_.y_speed >= body_.terminal_velocity) {
     body_.y_speed = body_.terminal_velocity;
  }
  player_sprite_.move(0.0f, body_.y_speed * delta_time);
}

Player::Player(const std::string& texture_file) : is_dead_(false), has_died_(false), grace_time_(0.0f), jump_dead_(true), status_(Status::NORMAL) {
  if (!player_texture_.loadFromFile(texture_file)) {
    // handle
  }

  // Normal state sprite (small mario)
  for (int i = 0; i < 6; i++) {
    sf::Sprite sprite;
    sprite.setTexture(player_texture_);
    sprite.setTextureRect(sf::IntRect(i * 12, 0 * 16, 12, 16));
    //sprite.setScale(12.0f / 16.0f, 16.0f / 32.0f);
    player_sprites_[Status::NORMAL].push_back(sprite);
  }


  // Big state (super mario)
  for (int i = 0; i < 7; i++) {
    sf::Sprite sprite;
    sprite.setTexture(player_texture_);
    sprite.setTextureRect(sf::IntRect(i * 16, 1 * 32 - 16, 16, 32));
    player_sprites_[Status::BIG].push_back(sprite);
  }

  // Fire state (flower mario)
  for (int i = 0; i < 7; i++) {
    sf::Sprite sprite;
    sprite.setTexture(player_texture_);
    sprite.setTextureRect(sf::IntRect(i * 16, 2 * 32 - 16, 16, 32));
    player_sprites_[Status::FIRE].push_back(sprite);
  }

  body_.gravity = 1500.0f;
  body_.terminal_velocity = 2240.0f;
  body_.x_speed = 0.0f;
  body_.y_speed = 0.0f;
  body_.jump_speed = 480.0f;
  body_.is_jumping = false;
  body_.acceleration = 100.0f;
  body_.max_x_speed = 145.0f;
  body_.is_accelerating = false;
  body_.has_jumped = false;

  player_sprite_ = player_sprites_[Status::NORMAL][0];
  player_sprite_.setPosition(10, 50);

}

sf::Vector2f Player::GetPosition() const {
  return player_sprite_.getPosition();
}

void Player::HandleInput(const sf::Event& event, float delta_time) {
  if (!is_dead_) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        if (body_.x_speed > -100.0f) body_.x_speed = -100.0f;
        body_.x_speed -= body_.acceleration * delta_time;
        body_.is_accelerating = true;
        if (body_.x_speed < -body_.max_x_speed) {
          body_.x_speed = -body_.max_x_speed;
        }
      } else {
        body_.is_accelerating = false;
        body_.x_speed = -100.0f;
      }

      x_scale_ = -1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        if (body_.x_speed < 100.0f) body_.x_speed = 100.0f;
        body_.x_speed += body_.acceleration * delta_time;
        body_.is_accelerating = true;
        if (body_.x_speed > body_.max_x_speed) {
          body_.x_speed = body_.max_x_speed;
        }
      } else {
        body_.is_accelerating = false;
        body_.x_speed = 100.0f;
      }

      x_scale_ = 1.0f;
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && event.key.code == sf::Keyboard::Up) {
      if (!body_.is_jumping && !body_.has_jumped) {
        body_.y_speed = -body_.jump_speed;
        body_.is_jumping = true;
        body_.has_jumped = true;
      }
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      body_.has_jumped = false;
    }
  }
}

void Player::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {
  // Apply only if player is alive
  if (!is_dead_) {
    if (grace_time_ > 0.0f) {
      grace_time_ -= delta_time;
    } else {
      changing_state_ = false;
    }

    sf::Vector2f current_position = player_sprite_.getPosition();

    // Animation
    animation_time_ += delta_time;
    const float animation_speed = 0.1f;
    if (animation_time_ >= animation_speed && std::abs(body_.x_speed) > 0) {

      animation_time_ = 0.0f;

      std::vector<sf::Sprite>& current_sprites = player_sprites_[status_];
      static int frame_index = 0;
      if (status_ == Status::NORMAL) frame_index = (frame_index + 1) % (current_sprites.size() - 2);
      else frame_index = (frame_index + 1) % (current_sprites.size() - 3);
      player_sprite_ = current_sprites[frame_index];
    }

    if (std::abs(body_.x_speed) <= 0.01f) player_sprite_ = player_sprites_[status_][0];

    player_sprite_.setPosition(current_position);
    player_sprite_.setScale(x_scale_, 1.0f);

    // Gravity
    body_.y_speed += delta_time * body_.gravity;
    if (body_.y_speed >= body_.terminal_velocity) {
      body_.y_speed = body_.terminal_velocity;
    }

    player_sprite_.move(0.0f, body_.y_speed * delta_time);

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

    player_sprite_.move(body_.x_speed * delta_time, 0.0f);

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

    CheckXMovement_();
  } else {
    // Player is dead, play animation
    PlayDeathAnimation_(delta_time);
  }
}

void Player::Draw(sf::RenderWindow& window) {
  window.draw(player_sprite_);
}

sf::FloatRect Player::GetCollisionBounds() const {
  return player_sprite_.getGlobalBounds();
}

float Player::GetYSpeed() const {
  return body_.y_speed;
}

void Player::Bounce() {
  body_.y_speed -= body_.jump_speed;
  body_.is_jumping = true;
  body_.has_jumped = true;
}

bool Player::IsDead() const {
  return is_dead_;
}

void Player::Die(bool jump = true) {
  is_dead_ = true;
  jump_dead_ = jump;
}

void Player::SetGracePeriod(float time) {
  grace_time_ = time;
}

bool Player::IsInGrace() const {
  return grace_time_ > 0.0f;
}

void Player::Reset() {
  player_sprite_.setPosition(10, 50);
  body_.x_speed = 0.0f;
  body_.y_speed = 0.0f;
  body_.is_jumping = false;
  body_.has_jumped = false;
  is_dead_ = false;
  status_ = Status::NORMAL;
}

void Player::Downgrade() {
  changing_state_ = true;
  sf::FloatRect bounds = player_sprite_.getGlobalBounds();
  float bottom = bounds.top + bounds.height;
  if (status_ == Status::NORMAL) {
    Die(true);
  } else {
    status_ = Status::NORMAL;
    player_sprite_.setPosition(player_sprite_.getPosition().x, bottom);
    SetGracePeriod(0.3f);
  }
}

void Player::Upgrade(Status status) {
  status_ = status;
}

bool Player::ChangingState() const {
  return changing_state_;
}
