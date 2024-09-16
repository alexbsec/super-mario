#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Physics.hpp"

class Player {
  private:
    sf::Texture player_texture_;
    sf::Sprite player_sprite_;
    BodyPhysics body_;
    bool is_dead_;
    bool has_died_;
    float grace_time_;
    bool jump_dead_;

    void CheckXMovement_();
    void PlayDeathAnimation_(float delta_time);

  public:
    Player(const std::string& texture_file);
    sf::Vector2f GetPosition() const;
    void HandleInput(const sf::Event& event, float delta_time);
    void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time);
    void Draw(sf::RenderWindow& window);
    sf::FloatRect GetCollisionBounds() const;
    float GetYSpeed() const;
    void Bounce();
    bool IsDead() const;
    void Die(bool jump);
    void SetGracePeriod(float time);
    bool IsInGrace() const;
    void Reset();
};

#endif
