#include "Camera.hpp"

Camera::Camera(float screen_width, float screen_height, float level_width, float level_height) : screen_width_(screen_width), screen_height_(screen_height), level_width_(level_width), level_height_(level_height), zoom_factor_(1.0f) {
  view_.setSize(screen_width, screen_height);
  view_.setCenter(screen_width / 2, screen_height / 2 - 30);
}

void Camera::Update(const sf::Vector2f& player_position) {
  float camera_x = player_position.x;
  float camera_y = 0;

  float x = screen_width_ * zoom_factor_ / 2;
  float y = screen_height_ * zoom_factor_ / 2;

  if (camera_x < x) {
    camera_x = x;
  }

  if (camera_y < y) {
    camera_y = y;
  }

  if (camera_x > level_width_ - x) {
    camera_x = level_width_ - x;
  }

  if (camera_y > level_height_ - y) {
    camera_y = level_height_ - y;
  }

  view_.setCenter(camera_x, camera_y);
}

void Camera::Apply(sf::RenderWindow& window) {
  window.setView(view_);
}
