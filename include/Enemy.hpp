#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Physics.hpp"

class Enemy {
  protected:
    sf::Sprite sprite_;
    sf::Texture texture_;
    EnemyPhysics body_;
    bool is_dead_;
    sf::Clock death_timer_;
    bool death_timer_started_;

  public:
    Enemy(const std::string& texture_file);

    virtual ~Enemy();

    virtual void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time);
    virtual void HandleCollision(const std::vector<sf::FloatRect>& collision_bounds);
    virtual void Draw(sf::RenderWindow& window);
    virtual void OnJumpedOn() = 0;

    sf::FloatRect GetBounds() const;
    bool IsDead();
    void SetPosition(float x, float y);
    sf::Vector2f GetPosition() const;
};

class Goomba : public Enemy {
  public:
    Goomba(const std::string& texture_file);

    virtual void OnJumpedOn() override;
};

class KoopaTroopa : public Enemy {
  private:
    bool in_shell_;
    bool is_shell_moving_;

  public:
    KoopaTroopa(const std::string& texture_file);

    virtual void OnJumpedOn() override;
    virtual void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) override;
};

#endif
