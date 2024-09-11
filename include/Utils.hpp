#ifndef UTILS_HPP
#define UTILS_HPP

#include "Map.hpp"
#include <vector>

// Brute force xD

void PlacePipe(std::vector<std::vector<int>>& level, int top, int left, int height);

void PlaceGround(std::vector<std::vector<int>>& level, int top, int left, int right, int height, int gid);

void PlaceHole(std::vector<std::vector<int>>& level, int top, int left, int width);

void PlaceAlternate(std::vector<std::vector<int>>& level, int left, int width, int height);

std::vector<std::vector<int>> MakeLevel1();

#endif
