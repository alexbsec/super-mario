#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
  private:
    sf::Texture player_texture_;
    sf::Sprite player_sprite_;
    float velocity_y_;
    bool is_jumping_;

  public:
    Player(const std::string& texture_file);
    void HandleInput();
    void Update();
    void Draw(sf::RenderWindow& window);
};

#endif
