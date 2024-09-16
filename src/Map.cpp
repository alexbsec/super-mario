#include "Map.hpp"

int Tile::GetTypeId() const {
  return type_id_;
}

sf::IntRect Tile::GetTexture() const {
  return texture_rect_;
}

bool Tile::IsSolid() const {
  return solid_;
}

TileType Tile::GetTileType() const {
  return tile_type_;
}

const sf::Sprite& Tile::GetSprite() const {
  return sprite_;
}

sf::Sprite& Tile::GetSprite() {
  return sprite_;
}

bool Tile::HasItem() const {
  return item_ != nullptr;
}

std::shared_ptr<Item> Tile::ReleaseItem() const {
  if (HasItem()) {
    auto released = item_;
    return released;
  }

  return nullptr;
}

void Tile::SetItem(std::shared_ptr<Item> item) {
  item_ = item;
}

/* Map class implementation */

Map::Map(const std::string& tileset_path) : tiles_({}) {
  if (!tileset_texture_.loadFromFile(tileset_path)) {
        // Handle
  }
}

Map::Map(const std::string& tileset_path, const std::string& item_texture_path) : tiles_({}) {
  if (!tileset_texture_.loadFromFile(tileset_path)) {
    // handle
  }

  if (!item_texture_.loadFromFile(item_texture_path)) {
    // handle
  }
}

void Map::LoadBackground(
  const std::string& tileset,
  const std::vector<std::vector<int>>& bg
) {
  const int tile_size = 16;

  for (size_t y = 0; y < bg.size(); y++) {
    for (size_t x = 0; x < bg[y].size(); x++) {
      int tile_type = bg[y][x];
      sf::IntRect texture_rect;
      bool is_solid = false;
      TileType behavior = TileType::SIMPLE;

      switch (tile_type) {
        case 1:
          texture_rect = sf::IntRect(3 * tile_size, 21 * tile_size, tile_size, tile_size);
          break;
        case 0:
          continue;
      }

      Tile tile(tile_type, texture_rect, is_solid, behavior, tileset_texture_);
      tile.GetSprite().setPosition(x * tile_size, y * tile_size);

      bg_tiles_.push_back(tile);
    }
  }
}

void Map::LoadMap(
  const std::string& tileset,
  const std::vector<std::vector<int>>& level_data
) {
  const int tile_size = 16;
  // const int tileset_cols = tileset_texture_.getSize().x / tile_size;

  for (size_t y = 0; y < level_data.size(); y++) {
    for (size_t x = 0; x < level_data[y].size(); x++) {
      int tile_type = level_data[y][x];
      sf::IntRect texture_rect, item_rect;
      bool is_solid = false;
      TileType behavior = TileType::SIMPLE;
      std::shared_ptr<Item> item = nullptr;

      switch (tile_type) {
        case 1:
          // Ground
          texture_rect = sf::IntRect(0 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 2:
          // Brick 1 (unbreakable)
          texture_rect = sf::IntRect(1 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 3:
          // Brick 2 (breakable)
          texture_rect = sf::IntRect(2 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::BREAKABLE;
          break;

        case 4:
          // Used brick
          texture_rect = sf::IntRect(3 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 5:
          // Stone block
          texture_rect = sf::IntRect(0 * tile_size, 1 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 6:
          // Pipe top-left
          texture_rect = sf::IntRect(0 * tile_size, 8 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::INTERACTABLE;
          break;

        case 7:
          // Pipe top-right
          texture_rect = sf::IntRect(1 * tile_size, 8 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::INTERACTABLE;
          break;

        case 8:
          // Pipe bottom-left;
          texture_rect = sf::IntRect(0 * tile_size, 9 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 9:
          // Pipe bottom-right
          texture_rect = sf::IntRect(1 * tile_size, 9 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::SIMPLE;
          break;

        case 10:
          // Item block with mushroom
          texture_rect = sf::IntRect(24 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::INTERACTABLE;
          item_rect = sf::IntRect(1 * tile_size, 0 * tile_size, tile_size, tile_size);

          item = std::make_shared<Mushroom>(item_texture_, item_rect);
          item->SetInitialAndTargetPosition(x * tile_size, y * tile_size, y * tile_size - tile_size);
          break;

        case 11:
          // Item block with flower
          texture_rect = sf::IntRect(24 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::INTERACTABLE;
          item_rect = sf::IntRect(0 * tile_size, 0 * tile_size, tile_size, tile_size);

          item  = std::make_shared<Flower>(item_texture_, item_rect);
          item->SetInitialAndTargetPosition(x * tile_size, y * tile_size, y * tile_size - tile_size);
          break;

          case 12:
          // Item block with star
          texture_rect = sf::IntRect(24 * tile_size, 0 * tile_size, tile_size, tile_size);
          is_solid = true;
          behavior = TileType::INTERACTABLE;
          item_rect = sf::IntRect(2 * tile_size, 0 * tile_size, tile_size, tile_size);

          item = std::make_shared<Star>(item_texture_, item_rect);
          item->SetInitialAndTargetPosition(x * tile_size, y * tile_size, y * tile_size - tile_size);
          break;

        case 0:
          continue;
      }

      Tile tile(tile_type, texture_rect, is_solid, behavior, tileset_texture_);
      tile.GetSprite().setPosition(x * tile_size, y * tile_size);

      if (item) tile.SetItem(item);

      tiles_.push_back(tile);
    }
  }
}

void Map::DrawBackground(sf::RenderWindow& window) {
  for (const auto& tile : bg_tiles_) {
    window.draw(tile.GetSprite());
  }
}

void Map::Draw(sf::RenderWindow& window) {
  for (const auto& tile : tiles_) {
    window.draw(tile.GetSprite());
  }
}

std::vector<sf::FloatRect> Map::GetCollisionBounds() {
  std::vector<sf::FloatRect> bounds;
  for (const auto& tile : tiles_) {
    bounds.push_back(tile.GetSprite().getGlobalBounds());
  }

  return bounds;
}

std::shared_ptr<Item> Map::ReleaseItemFromBlock(int tile_x, int tile_y) {
  for (auto& tile : tiles_) {
    if (tile.GetTileType() == TileType::INTERACTABLE && tile.HasItem()) {
      return tile.ReleaseItem();
    }
  }
  return nullptr;
}

void Map::PlaceItem(int tile_x, int tile_y, ItemType item_type) {
}

const std::vector<Tile>& Map::GetTiles() const {
  return tiles_;
}
