#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Camera.hpp"

const int TILE_SIZE = 16;
const int TILES_X = 16;
const int TILES_Y = 15;

Map GetTestMap() {
  std::vector<std::vector<int>> level_data = MakeLevel1();
  Map map("assets/lv1-tileset.png");
  map.LoadMap("assets/lv1-tileset.png", level_data);

  return map;
}

int main() {
  sf::RenderWindow window(sf::VideoMode(600, 400), "Super Mario");


  sf::Vector2u window_size = window.getSize();
  float scale_x = static_cast<float>(window_size.x) / (TILES_X * TILE_SIZE);
  float scale_y = static_cast<float>(window_size.y) / (TILES_Y * TILE_SIZE);

  Camera camera(scale_x * TILE_SIZE * TILES_X, scale_y * TILE_SIZE * TILES_Y, 224 * TILE_SIZE, 37 * TILE_SIZE);

  camera.SetZoomFactor(0.5f);
  Player mario("assets/MarioIdle.png");
  Map map = GetTestMap();

  // Main game loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    mario.HandleInput();
    mario.Update(map.GetCollisionBounds());

    camera.Update(mario.GetPosition());

    window.clear();

    camera.Apply(window);

    map.Draw(window);
    mario.Draw(window);

    window.display();
  }

  return EXIT_SUCCESS;
}
