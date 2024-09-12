#include "Utils.hpp"
#include <algorithm>

std::vector<std::unique_ptr<Enemy>> SetEnemies() {
  std::vector<std::unique_ptr<Enemy>> enemies;
  int ground = 257;
  int x_pos;

  int num_goombas = 16;

  for (int i = 0; i < num_goombas; i++) {
    auto goomba = std::make_unique<Goomba>("assets/goomba.png");
    ground = 257;
    switch (i) {
      case 0:
        x_pos = 350;
        break;
      case 1:
        x_pos = 670;
        break;
      case 2:
        x_pos = 820;
        break;
      case 3:
        x_pos = 850;
        break;
      case 4:
        ground = 50;
        x_pos = 1400;
        break;
      case 5:
        ground = 50;
        x_pos = 1350;
        break;
      case 6:
        x_pos = 1550;
        break;
      case 7:
        x_pos = 1600;
        break;
      case 8:
        x_pos = 1700;
        break;
      case 9:
        x_pos = 1750;
        break;
      case 10:
        x_pos = 1850;
        break;
      case 11:
        x_pos = 1900;
        break;
      case 12:
        x_pos = 1950;
        break;
      case 13:
        x_pos = 2000;
        break;
      case 14:
        x_pos = 2640;
        break;
      case 15:
        x_pos = 2700;
        break;
      default:
        continue;
    }

    goomba->SetPosition(x_pos, ground);
    enemies.push_back(std::move(goomba));
  }

  return enemies;
}

std::vector<std::vector<int>> MakeBackground() {
  std::vector<std::vector<int>> bg(18, std::vector<int>(224, 1));
  return bg;
}

void PlaceLeftTriangle(std::vector<std::vector<int>>& level, int bottom, int right, int height) {
    // Get the maximum bounds of the level array
    int max_height = level.size();
    int max_width = level[0].size();

    // Make sure bottom and right are within bounds
    if (bottom < 0 || right < 0 || bottom >= max_height || right >= max_width) {
        return; // Invalid position, exit early
    }

    // Loop from the bottom row upwards to build the triangle
    for (int i = 0; i < height; i++) {
        // Calculate the current width of the triangle at this row
        int triangle_width = height - i;

        // Loop through columns for this row (aligning to the right)
        for (int j = right - triangle_width + 1; j <= right && j >= 0; j++) {
            level[bottom - i][j] = 5;  // Fill the level array with '5's to represent the triangle
        }
    }
}


void PlaceRightTriangle(std::vector<std::vector<int>>& level, int bottom, int left, int height) {
    // Get the maximum bounds of the level array
    int max_height = level.size();
    int max_width = level[0].size();

    // Make sure bottom and left are within bounds
    if (bottom < 0 || left < 0 || bottom >= max_height || left >= max_width) {
        return; // Invalid position, exit early
    }

    // Loop from the bottom row upwards to build the triangle
    for (int i = 0; i < height; i++) {
        // Calculate the current width of the triangle at this row
        int triangle_width = height - i;

        // Loop through columns for this row (always start from `left`)
        for (int j = left; j < left + triangle_width && j < max_width; j++) {
            level[bottom - i][j] = 5;  // Fill the level array with '5's to represent the triangle
        }
    }
}


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

  PlaceGround(level1, max_i - 1 - 37/2, 0, max_j - 1, 2, 1);

  int top_ground = max_i - 2 - 37 / 2;
  int three_above = max_i - 6 - 37 / 2;

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
  PlaceGround(level1, eight_above, 81, 88, 1, 2);

  // hole at 87
  PlaceHole(level1, top_ground, 87, 3);

  PlaceGround(level1, eight_above, 90, 92, 1, 2);
  level1[eight_above][93] = 10;

  // Coin block
  level1[three_above][93] = 2;

  level1[three_above][99] = 2;
  // star block (change later)
  level1[three_above][100] = 2;

  level1[three_above][105] = 10;
  level1[three_above][108] = 10;
  level1[three_above][111] = 10;

  level1[three_above - 3][108] = 10;

  level1[three_above][117] = 2;

  PlaceGround(level1, eight_above, 120, 122, 1, 2);
  PlaceGround(level1, eight_above, 127, 129, 1, 2);
  PlaceGround(level1, three_above, 127, 128, 1, 2);

  PlaceLeftTriangle(level1, top_ground - 1, 135, 4);
  PlaceRightTriangle(level1, top_ground - 1, 138, 4);

  PlaceLeftTriangle(level1, top_ground - 1, 150, 5);
  PlaceRightTriangle(level1, top_ground - 1, 153, 4);

  PlaceHole(level1, top_ground, 151, 2);

  PlacePipe(level1, top_ground - 1, 161, 2);

  PlaceGround(level1, three_above, 166, 169, 1, 2);
  level1[three_above][168] = 10;

  PlacePipe(level1, top_ground - 1, 177, 2);
  PlaceLeftTriangle(level1, top_ground - 1, 187, 9);

  // Flag (end level)
  level1[top_ground - 1][196] = 5;
  return level1;
}
