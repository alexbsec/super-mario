#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Physics.hpp"

class Player {
  private:
    sf::Texture player_texture_;
    sf::Sprite player_sprite_;
    BodyPhysics body_;

    void CheckXMovement_();

  public:
    Player(const std::string& texture_file);
    sf::Vector2f GetPosition() const;
    void HandleInput(const sf::Event& event, float delta_time);
    void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect GetCollisionBounds() const;
    float GetYSpeed() const;
    void Bounce();
};

#endif
