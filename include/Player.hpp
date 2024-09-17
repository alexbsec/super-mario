#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Physics.hpp"
#include <unordered_map>
#include <vector>

enum class Status {
  NORMAL,
  BIG,
  FIRE,
  IMMORTAL
};

class Player {
  private:
    sf::Texture player_texture_;
    sf::Sprite player_sprite_;
    std::unordered_map<Status, std::vector<sf::Sprite>> player_sprites_;
    BodyPhysics body_;
    bool is_dead_;
    bool has_died_;
    float grace_time_;
    bool jump_dead_;
    Status status_;
    float animation_time_ = 0.0f;
    float x_scale_ = 1.0f;
    bool changing_state_ = false;

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

    // Status methods
    Status GetStatus() const;
    void SetStatus(Status status);
    void Downgrade();
    void Upgrade(Status status);
    bool ChangingState() const;
};

#endif
