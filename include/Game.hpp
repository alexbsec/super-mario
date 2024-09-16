#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "Enemy.hpp"
#include "Item.hpp"

Map GetTestMap();

Map GetBackground();

Camera MakeCamera(sf::RenderWindow& window, float zoom_factor);

void PlayFadeout(sf::RenderWindow& window, float& fade_alpha, sf::Font& font, bool& show_play_again, const sf::View& camera_view);

bool IsPlayerOnScreen(const Player& mario, const Camera& camera);

void UpdateEnemies(Player mario, std::vector<sf::FloatRect>& mario_bounds, std::vector<std::unique_ptr<Enemy>>& enemies, Map& map, float delta_time);

void UpdatePlayer(sf::Event event, Player& mario, std::vector<std::unique_ptr<Enemy>>& enemies, Map& map, float delta_time);

void UpdateItem(Player& player, Map& map, float delta_time);

void UpdateCamera(Camera& camera, Player mario);

void DrawBackground(sf::RenderWindow& window, Map& bg);

void DrawMap(sf::RenderWindow& window, Map& map);

void DrawPlayer(sf::RenderWindow& window, Player& player);

void DrawEnemy(sf::RenderWindow& window, const std::vector<std::unique_ptr<Enemy>>& enemies);

void DrawItem(sf::RenderWindow& window, const std::vector<std::shared_ptr<Item>>& items);

void CheckPlayerTileCollision(Player& player, Map& map, float delta_time, std::vector<std::shared_ptr<Item>>& items);

bool GameLoop(sf::RenderWindow& window, sf::Clock clock, Player& mario, std::vector<std::unique_ptr<Enemy>>& enemies, Camera& camera, Map& map, Map& bg, std::vector<sf::FloatRect>& mario_collision_bounds, std::vector<sf::FloatRect>& enemy_collision_bounds);


#endif
