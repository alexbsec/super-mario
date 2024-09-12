#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

enum class TileType {
  BREAKABLE,
  COLLECTABLE,
  ANIMATED,
  INTERACTABLE,
  SIMPLE
};

class Tile {
  private:
    int type_id_;
    sf::IntRect texture_rect_;
    bool solid_;
    TileType tile_type_;
    sf::Sprite sprite_;

  public:
    Tile(
      int type,
      const sf::IntRect& texture_rect,
      bool solid,
      TileType tile_type,
      sf::Texture& texture
    ) : type_id_(type), texture_rect_(texture_rect), solid_(solid), tile_type_(tile_type) {
      sprite_.setTexture(texture);
      sprite_.setTextureRect(texture_rect);
    }

    int GetTypeId() const;
    sf::IntRect GetTexture() const;
    bool IsSolid() const;
    TileType GetTileType() const;
    const sf::Sprite& GetSprite() const;
    sf::Sprite& GetSprite();
};

class Map {
  private:
    std::vector<Tile> bg_tiles_;
    std::vector<Tile> tiles_;
    sf::Texture tileset_texture_;

  public:
    Map(const std::string& tileset_path);
    void LoadBackground(const std::string& tileset, const std::vector<std::vector<int>>& bg);
    void LoadMap(const std::string& tileset, const std::vector<std::vector<int>>& level_data);
    void DrawBackground(sf::RenderWindow& window);
    void Draw(sf::RenderWindow& window);
    std::vector<sf::FloatRect> GetCollisionBounds();
};

#endif
