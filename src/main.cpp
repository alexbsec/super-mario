#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Super Mario");

  Player mario("assets/MarioIdle.png");

  // Main game loop

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    mario.HandleInput();
    mario.Update();

    window.clear();

    mario.Draw(window);

    window.display();
  }

  return EXIT_SUCCESS;
}
