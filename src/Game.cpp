#include "Game.hpp"
#include <iostream>
#include "Utils.hpp"

const int TILE_SIZE = 16;
const int TILES_X = 16;
const int TILES_Y = 15;

void LoadSoundtrack(const std::string& path, bool start) {
  static sf::Music background_music;
  if (!background_music.openFromFile(path)) {
    std::cerr << "Error: could not load mp3" << std::endl;
  }

  if (start) {
    background_music.stop();

    // Reset the music to the beginning by setting the playing offset to 0
    background_music.setPlayingOffset(sf::Time::Zero);

    // Optionally start the music again
    background_music.play();
    return;
  }

  background_music.setLoop(true);
  background_music.setVolume(50.0f);
  background_music.play();
}

Map GetTestMap() {
  std::vector<std::vector<int>> level_data = MakeLevel1();
  Map map("assets/lv1-tileset.png", "assets/item_tilesheet.png");
  map.LoadMap("assets/lv1-tileset.png", level_data);

  return map;
}

Map GetBackground() {
  std::vector<std::vector<int>> bg = MakeBackground();
  Map map("assets/lv1-tileset.png");
  map.LoadBackground("assets/lv1-tileset.png", bg);

  return map;
}

Camera MakeCamera(sf::RenderWindow& window, float zoom_factor) {
  sf::Vector2u window_size = window.getSize();
  float scale_x = static_cast<float>(window_size.x) / (TILES_X * TILE_SIZE);
  float scale_y = static_cast<float>(window_size.y) / (TILES_Y * TILE_SIZE);

  Camera camera(scale_x * TILE_SIZE * TILES_X, scale_y * TILE_SIZE * TILES_Y, 224 * TILE_SIZE, 37 * TILE_SIZE);

  camera.SetZoomFactor(zoom_factor);

  return camera;
}

void PlayFadeout(sf::RenderWindow& window, float& fade_alpha, sf::Font& font, bool& show_play_again, const sf::View& camera_view) {
    if (fade_alpha < 255.0f) {
        fade_alpha += 1.0f;  // Control the speed of the fadeout
    }

    // Set the view to follow the camera
    window.setView(camera_view);

    // Get the size of the camera's view, not the window
    sf::Vector2f view_size = camera_view.getSize();
    sf::Vector2f view_center = camera_view.getCenter();

    // Create a rectangle that covers the camera's view
    sf::RectangleShape fade_rect(view_size);
    fade_rect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
    fade_rect.setPosition(view_center.x - view_size.x / 2.0f, view_center.y - view_size.y / 2.0f);

    // Draw the fading rectangle
    window.draw(fade_rect);

    // If the fadeout is complete, show the "Play again" text
    if (fade_alpha >= 255.0f) {
        show_play_again = true;

        sf::Text play_again_text;
        play_again_text.setFont(font);
        play_again_text.setString("Play again? Press Enter.");
        play_again_text.setCharacterSize(15);
        play_again_text.setFillColor(sf::Color::White);

        // Center the text in the camera view
        sf::FloatRect text_bounds = play_again_text.getLocalBounds();
        play_again_text.setPosition(
            view_center.x - text_bounds.width / 2.0f,
            view_center.y - text_bounds.height / 2.0f
        );

        window.draw(play_again_text);
    }
}

void InteractWithItem(Player& player, std::vector<std::shared_ptr<Item>>& items) {
  const int x_range = 5.0f;
  const int y_range = 5.0f;
  auto it = items.begin();
  while (it != items.end()) {
    sf::Vector2f item_position = (*it)->GetSprite().getPosition();
    sf::Vector2f player_position = player.GetPosition();
    if ((*it)->GetState() != ItemState::ACTIVE) {
      ++it;
      continue;
    }

    if (item_position.x <= player_position.x + x_range && item_position.x >= player_position.x - x_range) {
      if (item_position.y <= player_position.y + y_range && item_position.y >= player_position.y - y_range) {
        Status player_status;
        switch ((*it)->GetItemType()) {
          case ItemType::MUSHROOM:
            player_status = Status::BIG;
            break;
          case ItemType::FLOWER:
            player_status = Status::FIRE;
            break;
          case ItemType::STAR:
            player_status = Status::FIRE;
            break;
        }

        player.Upgrade(player_status);
        it = items.erase(it);
      } else {
        ++it;
      }
    } else {
      ++it;
    }
  }
}

bool IsPlayerOnScreen(const Player& mario, const Camera& camera) {
  sf::FloatRect mario_bounds = mario.GetCollisionBounds();
  sf::FloatRect view_bounds = camera.GetViewBounds();

  return mario_bounds.intersects(view_bounds);
}

void UpdateEnemies(Player mario, std::vector<sf::FloatRect>& mario_bounds, std::vector<std::unique_ptr<Enemy>>& enemies, Map& map, float delta_time) {
  mario_bounds = map.GetCollisionBounds();
  std::vector<sf::FloatRect> enemy_collision_bounds = map.GetCollisionBounds();
  enemy_collision_bounds.push_back(mario.GetCollisionBounds());

  for (auto& enemy : enemies) {
    mario_bounds.push_back(enemy->GetBounds());
    enemy->Update(enemy_collision_bounds, delta_time);
  }

  enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(), [](std::unique_ptr<Enemy>& enemy) {
        return enemy->IsDead();
      }),
      enemies.end()
  );
}

void UpdatePlayer(sf::Event event, Player& mario, std::vector<std::unique_ptr<Enemy>>& enemies, Map& map, float delta_time) {
  mario.HandleInput(event, delta_time);
  mario.Update(map.GetCollisionBounds(), delta_time);

  if (!mario.IsDead()) {
    if (mario.GetPosition().y > 350.0f) mario.Die(false);

    for (auto& enemy : enemies) {
      sf::FloatRect mario_bounds = mario.GetCollisionBounds();
      sf::FloatRect enemy_bounds = enemy->GetBounds();
      if (mario_bounds.intersects(enemy_bounds)) {
        if (mario.GetYSpeed() > 0 && (mario_bounds.top + mario_bounds.height) <= (enemy_bounds.top + enemy_bounds.height * 0.2f)) {
          enemy->OnJumpedOn();
          mario.Bounce();
          mario.SetGracePeriod(0.3f);
          continue;
        }

        if (!mario.IsInGrace() && mario_bounds.top + mario_bounds.height > enemy_bounds.top && mario_bounds.top < enemy_bounds.top + enemy_bounds.height) {
          mario.Downgrade();
        }
      }
    }
  }
}

// TO implement
void UpdateItem(Player& player, Map& map, float delta_time) {
  std::vector<sf::FloatRect> collision_bounds = map.GetCollisionBounds();

  for (auto& tile : map.GetTiles()) {
    if (tile.GetTileType() == TileType::INTERACTABLE && tile.HasItem()) {
      std::shared_ptr<Item> item = tile.ReleaseItem();
      if (!item) continue;

      item->Update(collision_bounds, delta_time);
    }
  }
}

void UpdateCamera(Camera& camera, Player mario) {
  camera.Update(mario.GetPosition());
}

void DrawBackground(sf::RenderWindow& window, Map& bg) {
  bg.DrawBackground(window);
}

void DrawMap(sf::RenderWindow& window, Map& map) {
  map.Draw(window);
}

void DrawPlayer(sf::RenderWindow& window, Player& player) {
  player.Draw(window);
}

void DrawEnemy(sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies) {
  for (auto& enemy : enemies) {
    enemy->Draw(window);
  }
}

void DrawItem(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& items) {
  for (auto& item : items) {
    std::cout << "Drawing item at position: " << item->GetSprite().getPosition().x << "," << item->GetSprite().getPosition().y << std::endl;
    item->Draw(window);
  }
}

const float EPSILON = 0.5f;

void CheckPlayerTileCollision(Player& player, Map& map, float delta_time, std::vector<std::shared_ptr<Item>>& items) {
  sf::FloatRect player_bounds = player.GetCollisionBounds();

  for (const auto& tile : map.GetTiles()) {
    sf::FloatRect tile_bounds = tile.GetSprite().getGlobalBounds();
    // Only proceed if Mario is moving upwards and the tile is interactable with an item
    if (player.GetYSpeed() >= 0 || tile.GetTileType() != TileType::INTERACTABLE || !tile.HasItem()) {
      continue;  // Skip to the next tile if conditions aren't met
    }
    // Check horizontal overlap
    bool horizontal_overlap = (player_bounds.left + player_bounds.width > tile_bounds.left + EPSILON) &&
                              (player_bounds.left < tile_bounds.left + tile_bounds.width - EPSILON);
    if (!horizontal_overlap) {
      continue;  // Skip this tile if there's no horizontal overlap
    }
    // Check vertical overlap
    bool vertical_overlap = (player_bounds.top <= tile_bounds.top + tile_bounds.height + 3 * EPSILON) &&
                            (player_bounds.top + player_bounds.height >= tile_bounds.top - 3 * EPSILON);
    if (!vertical_overlap) {
      continue;  // Skip this tile if there's no vertical overlap
    }
    // If both overlaps are true, interact with the tile
    std::shared_ptr<Item> released_item = tile.ReleaseItem();
    if (released_item && released_item->GetState() == ItemState::TO_POP) {
      released_item->SetState(ItemState::POPPING);
      if (items.empty() || items.back() != released_item) {
        items.push_back(released_item);
      }
    }
  }
}


bool GameLoop(sf::RenderWindow& window, sf::Clock clock, Player& mario, std::vector<std::unique_ptr<Enemy>>& enemies, Camera& camera, Map& map, Map& bg, std::vector<sf::FloatRect>& mario_collision_bounds, std::vector<sf::FloatRect>& enemy_collision_bounds) {
  float fade_alpha = 0.0f;
  bool show_play_again = false;
  bool play_again = false;

  std::vector<std::shared_ptr<Item>> active_items;

  sf::Font font;
  if (!font.loadFromFile("assets/font/SuperMario256.ttf")) {
    // handle error
    std::cerr << "Error loading font" << std::endl;
    return false;
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (show_play_again && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        play_again = true;
        break;
      }
    }

    if (play_again) break;

    sf::Time delta_time = clock.restart();

    mario_collision_bounds.clear();
    enemy_collision_bounds.clear();

    UpdatePlayer(event, mario, enemies, map, delta_time.asSeconds());

    if (!mario.IsDead() && !mario.ChangingState()) {
      UpdateEnemies(mario, mario_collision_bounds, enemies, map, delta_time.asSeconds());
      UpdateCamera(camera, mario);
    }

    CheckPlayerTileCollision(mario, map, delta_time.asSeconds(), active_items);

    UpdateItem(mario, map, delta_time.asSeconds());

    window.clear();
    camera.Apply(window);

    DrawBackground(window, bg);

    for (auto& item : active_items) {
      if (item->GetState() == ItemState::POPPING) {
        item->Draw(window);
      }
    }

    DrawMap(window, map);

    for (auto& item : active_items) {
      if (item->GetState() == ItemState::ACTIVE) {
        item->Draw(window);
      }
    }

    DrawPlayer(window, mario);
    DrawEnemy(window, enemies);

    InteractWithItem(mario, active_items);

    if (mario.IsDead() && !IsPlayerOnScreen(mario, camera)) {
      PlayFadeout(window, fade_alpha, font, show_play_again, camera.GetView());
    }

    window.display();
  }

  return play_again;
}
