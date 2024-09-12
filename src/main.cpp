#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"

const int TILE_SIZE = 16;
const int TILES_X = 16;
const int TILES_Y = 15;

Map GetTestMap() {
  std::vector<std::vector<int>> level_data = MakeLevel1();
  Map map("assets/lv1-tileset.png");
  map.LoadMap("assets/lv1-tileset.png", level_data);

  return map;
}

Map GetBackground() {
  std::vector<std::vector<int>> bg = MakeBackground();
  Map map("assets/lv1-tileset.png");
  map.LoadBackground("assets/lv1-tileset.png", bg);

  return map;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(600, 300), "Super Mario");


  sf::Vector2u window_size = window.getSize();
  float scale_x = static_cast<float>(window_size.x) / (TILES_X * TILE_SIZE);
  float scale_y = static_cast<float>(window_size.y) / (TILES_Y * TILE_SIZE);

  Camera camera(scale_x * TILE_SIZE * TILES_X, scale_y * TILE_SIZE * TILES_Y, 224 * TILE_SIZE, 37 * TILE_SIZE);

  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<sf::FloatRect> mario_collision_bounds, enemy_collision_bounds;

  enemies = SetEnemies();
  //camera.SetZoomFactor(0.5f);

  if (enemies.empty()) std::cout << "ops" << std::endl;
  Player mario("assets/MarioIdle.png");
  Map map = GetTestMap();
  Map bg = GetBackground();

  sf::Clock clock;
  // Main game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    mario_collision_bounds.clear();
    enemy_collision_bounds.clear();

    mario_collision_bounds = map.GetCollisionBounds();
    enemy_collision_bounds = map.GetCollisionBounds();
    enemy_collision_bounds.push_back(mario.GetCollisionBounds());

    sf::Time deltaTime = clock.restart();

    for (auto& enemy : enemies) {
      mario_collision_bounds.push_back(enemy->GetBounds());
      enemy->Update(enemy_collision_bounds, deltaTime.asSeconds());
    }

    enemies.erase(
      std::remove_if(enemies.begin(), enemies.end(), [](std::unique_ptr<Enemy>& enemy) {
        return enemy->IsDead();
      }),
      enemies.end()
   );

    mario.HandleInput(event, deltaTime.asSeconds());
    mario.Update(map.GetCollisionBounds(), deltaTime.asSeconds());

    for (auto& enemy : enemies) {
      sf::FloatRect mario_bounds = mario.GetCollisionBounds();
      sf::FloatRect enemy_bounds = enemy->GetBounds();

      if (mario_bounds.intersects(enemy_bounds)) {
        if (mario.GetYSpeed() > 0 && (mario_bounds.top + mario_bounds.height) <= (enemy_bounds.top + enemy_bounds.height * 0.2f)) {
          enemy->OnJumpedOn();
          mario.Bounce();
        }
      }
    }

    camera.Update(mario.GetPosition());

    window.clear();

    camera.Apply(window);
    bg.DrawBackground(window);
    map.Draw(window);
    mario.Draw(window);


    for (auto& enemy : enemies) {
      enemy->Draw(window);
    }




    window.display();
  }

  return EXIT_SUCCESS;
}
