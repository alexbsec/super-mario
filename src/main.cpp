#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"
#include "Game.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(600, 400), "Super Mario");
  LoadSoundtrack("assets/song/ground-theme.mp3", false);

  while (true) {
    LoadSoundtrack("assets/song/ground-theme.mp3", true);
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<sf::FloatRect> mario_collision_bounds, enemy_collision_bounds;

    Camera camera = MakeCamera(window, 0.8f);

    enemies = SetEnemies();

    Player mario("assets/animation/mario/MarioCompleteSheet.png");
    Map map = GetTestMap();
    Map bg = GetBackground();

    sf::Clock clock;

    if (!GameLoop(
      window,
      clock,
      mario,
      enemies,
      camera,
      map,
      bg,
      mario_collision_bounds,
      enemy_collision_bounds
    )) break;

  }

  return EXIT_SUCCESS;
}
