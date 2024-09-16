#include "Item.hpp"
#include "Utils.hpp"


void Item::HandleYCollision_(const std::vector<sf::FloatRect>& collision_bounds) {
  sf::FloatRect item_bounds = sprite_.getGlobalBounds();
  sf::Vector2f position = sprite_.getPosition();

  if (!is_picked_ && state_ == ItemState::ACTIVE) {
    for (const auto& bound : collision_bounds) {
      if (item_bounds.intersects(bound)) {
        if (body_.y_speed > 0 && item_bounds.top + item_bounds.height <= bound.top + body_.y_speed) {
          sprite_.setPosition(position.x, bound.top - item_bounds.height);
          body_.y_speed = 0;
        } else if (body_.y_speed < 0 && item_bounds.top >= bound.top + bound.height) {
          sprite_.setPosition(position.x, bound.top + bound.height);
          body_.y_speed = 0;
        }
      }
    }
  }
}

void Item::HandleXCollision_(const std::vector<sf::FloatRect>& collision_bounds) {
  sf::FloatRect item_bounds = sprite_.getGlobalBounds();

  if (!is_picked_ && state_ == ItemState::ACTIVE && item_type_ != ItemType::FLOWER) {
    for (const auto& bound : collision_bounds) {
      if (item_bounds.intersects(bound)) {
        if (body_.is_moving_left && item_bounds.left <= bound.left + bound.width) {
            if (CheckBounds(item_bounds, bound)) {
              body_.is_moving_left = false;
          }
        } else if (!body_.is_moving_left && item_bounds.left + item_bounds.width >= bound.left) {

          if (CheckBounds(item_bounds, bound)) {
                body_.is_moving_left = true;
              }
          }
      }
    }
  }
}

Item::Item(ItemType type, const sf::Texture& texture, const sf::IntRect& texture_rect) : is_picked_(false), visible_(false), state_(ItemState::TO_POP), item_type_(type) {
  sprite_.setTexture(texture);
  sprite_.setTextureRect(texture_rect);

  body_.gravity = 1500.0f;
  body_.x_speed = 0.0f;
  body_.y_speed = 0.0f;
  body_.terminal_velocity = 2240.0f;
  body_.is_moving_left = true;
}

Item::~Item() {}

ItemType Item::GetItemType() const {
  return item_type_;
}

const sf::Sprite& Item::GetSprite() const {
  return sprite_;
}

bool Item::IsPoppingOut() const {
  return state_ == ItemState::POPPING;
}

void Item::PlayPopOutAnimation(float speed, float delta_time) {
  if (!visible_) MakeVisible();

  if (sprite_.getPosition().y > target_y_position_) {
    sprite_.move(0.0f, -speed * delta_time);
  } else {
    state_ = ItemState::ACTIVE;
  }
}

void Item::SetInitialAndTargetPosition(float start_x, float start_y, float target_y) {
  sprite_.setPosition(start_x, start_y);
  target_y_position_ = target_y;
}

void Item::Draw(sf::RenderWindow& window) {
  if (visible_) {
    window.draw(sprite_);
  }
}

void Item::MakeVisible() {
  visible_ = true;
}

ItemState Item::GetState() const {
  return state_;
}

void Item::SetState(ItemState state) {
  state_ = state;
}

Mushroom::Mushroom(const sf::Texture& texture, const sf::IntRect& texture_rect) : Item(ItemType::MUSHROOM, texture, texture_rect) {
  body_.x_speed = 70.0f;
}

void Mushroom::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {
  if (state_ == ItemState::POPPING) {
    PlayPopOutAnimation(10.0f, delta_time);
    return;
  }


  if (state_ == ItemState::ACTIVE) {
    body_.y_speed += body_.gravity * delta_time;
    if (body_.y_speed > body_.terminal_velocity) {
      body_.y_speed = body_.terminal_velocity;
    }
  }

  HandleCollision(collision_bounds);

  if (!is_picked_ && state_ == ItemState::ACTIVE) {
    if (body_.is_moving_left) {
      sprite_.move(-body_.x_speed * delta_time, body_.y_speed * delta_time);
    } else {
      sprite_.move(body_.x_speed * delta_time, body_.y_speed * delta_time);
    }
  }
}

void Mushroom::HandleCollision(const std::vector<sf::FloatRect>& collision_bounds) {
  HandleYCollision_(collision_bounds);
  HandleXCollision_(collision_bounds);
}

Flower::Flower(const sf::Texture& texture, const sf::IntRect& texture_rect) : Item(ItemType::FLOWER, texture, texture_rect) {}

void Flower::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {
   if (state_ == ItemState::ACTIVE) {
    body_.y_speed += body_.gravity * delta_time;
    if (body_.y_speed > body_.terminal_velocity) {
      body_.y_speed = body_.terminal_velocity;
    }
  }

  if (!is_picked_ && state_ == ItemState::ACTIVE) {
    sprite_.move(0.0, body_.y_speed * delta_time);
  }
}

Star::Star(const sf::Texture& texture, const sf::IntRect& texture_rect) : Item(ItemType::STAR, texture, texture_rect) {
  body_.x_speed = 80.0f;
}


void Star::Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) {

  if (state_ == ItemState::ACTIVE) {
    body_.y_speed += body_.gravity * delta_time;
    if (body_.y_speed > body_.terminal_velocity) {
      body_.y_speed = body_.terminal_velocity;
    }
  }

  HandleCollision(collision_bounds);

  if (!is_picked_ && state_ == ItemState::ACTIVE) {
    if (body_.is_moving_left) {
      sprite_.move(-body_.x_speed * delta_time, body_.y_speed * delta_time);
    } else {
      sprite_.move(body_.x_speed * delta_time, body_.y_speed * delta_time);
    }
  }


}

    void Star::HandleCollision(const std::vector<sf::FloatRect>& collision_bounds) {
  HandleYCollision_(collision_bounds);
  HandleXCollision_(collision_bounds);
}
