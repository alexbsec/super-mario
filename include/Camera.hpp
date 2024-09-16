#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Camera {
  private:
    sf::View view_;

    float screen_width_;
    float screen_height_;
    float level_width_;
    float level_height_;
    float zoom_factor_;

  public:
    Camera(float screen_width, float screen_height, float level_width, float level_height);

    void SetZoomFactor(float zoom_factor) {
      zoom_factor_ = zoom_factor;
      view_.zoom(zoom_factor);
    }

    void Update(const sf::Vector2f& player_position);

    void Apply(sf::RenderWindow& window);
    sf::View GetView() const;
    sf::FloatRect GetViewBounds() const;

};

#endif
