#include "Utils.hpp"


void PlacePipe(std::vector<std::vector<int>>& level, int top, int left, int height) {
  for (int i = 0; i < height; i++) {
     level[top - i][left] = (i < height - 1 ? 8 : 6);
     level[top - i][left + 1] = (i < height - 1 ? 9 : 7);

  }
}

void PlaceGround(std::vector<std::vector<int>>& level, int top, int left, int right, int height, int gid) {
  for (int j = 0; j < height; j++) {
    for (int i = left; i <= right; i++) {
      level[top - j][i] = gid;
    }
  }
}

void PlaceHole(std::vector<std::vector<int>>& level, int top, int left, int width) {
  for (int i = left; i < left + width; i++) {
    level[top][i] = 0;
    level[top + 1][i] = 0;
  }
}

void PlaceAlternate(std::vector<std::vector<int>>&level, int left, int width, int height) {
  for (int i = left; i < left + width; i++) {
    if ((i - left) % 2 == 0) {
      level[height][i] = 2;
    } else {
      level[height][i] = 10;
    }
  }
}

std::vector<std::vector<int>> MakeLevel1() {
  std::vector<std::vector<int>> level1(37, std::vector<int>(224, 0));

  int max_i = level1.size();
  int max_j = level1[0].size();

  PlaceGround(level1, max_i - 1, 0, max_j - 1, 2, 1);

  int top_ground = max_i - 2;
  int three_above = max_i - 6;

  PlaceAlternate(level1, 21, 5, three_above);

  level1[three_above - 3][23] = 10;

  // first pipe
  PlacePipe(level1, top_ground - 1, 29, 2);

  // second pipe
  PlacePipe(level1, top_ground - 1, 39, 3);

  // third pipe
  PlacePipe(level1, top_ground - 1, 47, 4);

  // fourth pipe
  PlacePipe(level1, top_ground - 1, 58, 4);

  // hole
  PlaceHole(level1, top_ground, 70, 2);

  // blocks above at 78
  PlaceAlternate(level1, 78, 3, three_above);

  int eight_above = three_above - 4;
  // Eight above at 81 size range 88
  PlaceGround(level1, eight_above, 81, 88, 1, 1);

  // hole at 87
  PlaceHole(level1, top_ground, 87, 3);

  PlaceGround(level1, eight_above, 90, 92, 1, 1);
  level1[eight_above][93] = 10;

  // Coin block
  level1[three_above][93] = 2;

  return level1;
}
