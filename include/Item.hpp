#ifndef ITEM_HPP
#define ITEM_HPP

#include <SFML/Graphics.hpp>
#include "Physics.hpp"

enum class ItemType {
  MUSHROOM,
  FLOWER,
  STAR
};

enum class ItemState {
  TO_POP,
  POPPING,
  ACTIVE
};

class Item {
  protected:
    bool is_picked_;
    bool visible_;
    sf::Sprite sprite_;
    ItemState state_;
    ItemType item_type_;
    ItemPhysics body_;
    float target_y_position_;


    void HandleYCollision_(const std::vector<sf::FloatRect>& collision_bounds);

    void HandleXCollision_(const std::vector<sf::FloatRect>& collision_bounds);

  public:
    Item(ItemType type, const sf::Texture& texture, const sf::IntRect& texture_rect);

    virtual ~Item();
    ItemType GetItemType() const;
    const sf::Sprite& GetSprite() const;
    virtual void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) = 0;

    bool IsPoppingOut() const;
    void PlayPopOutAnimation(float speed, float delta_time);

    void SetInitialAndTargetPosition(float start_x, float start_y, float target_y);

    void Draw(sf::RenderWindow& window);

    void MakeVisible();

    ItemState GetState() const;
    void SetState(ItemState state);
};

class Mushroom : public Item {
  public:
    Mushroom(const sf::Texture& texture, const sf::IntRect& texture_rect);

    void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) override;
    void HandleCollision(const std::vector<sf::FloatRect>& collision_bounds);
};

class Flower : public Item {
  public:
    Flower(const sf::Texture& texture, const sf::IntRect& texture_rect);

    void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) override;
};

class Star : public Item {
  public:
    Star(const sf::Texture& texture, const sf::IntRect& texture_rect);

    void Update(const std::vector<sf::FloatRect>& collision_bounds, float delta_time) override;

    void HandleCollision(const std::vector<sf::FloatRect>& collision_bounds);
};


#endif
