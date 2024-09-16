#ifndef UTILS_HPP
#define UTILS_HPP

#include "Map.hpp"
#include "Enemy.hpp"
#include <vector>
#include <iostream>

// Utilities

void Print(const std::string& text);

bool CheckBounds(const sf::FloatRect& first_bounds, const sf::FloatRect& second_bounds);

// Enemies

std::vector<std::unique_ptr<Enemy>> SetEnemies();

// Background

std::vector<std::vector<int>> MakeBackground();

// Brute force xD

void PlaceRightTriangle(std::vector<std::vector<int>>& level, int bottom, int left, int height);

void PlaceLeftTriangle(std::vector<std::vector<int>>& level, int bottom, int right, int height);

void PlacePipe(std::vector<std::vector<int>>& level, int top, int left, int height);

void PlaceGround(std::vector<std::vector<int>>& level, int top, int left, int right, int height, int gid);

void PlaceHole(std::vector<std::vector<int>>& level, int top, int left, int width);

void PlaceAlternate(std::vector<std::vector<int>>& level, int left, int width, int height);

std::vector<std::vector<int>> MakeLevel1();

#endif
