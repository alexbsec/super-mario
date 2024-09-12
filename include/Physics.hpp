#ifndef PHYSICS_HPP
#define PHYSICS_HPP

struct EnemyPhysics {
  float gravity;
  float terminal_velocity;
  float x_speed;
  float y_speed;
  bool is_moving_left;
};

struct BodyPhysics {
  float gravity;
  float terminal_velocity;
  float x_speed;
  float y_speed;
  float jump_speed;
  float acceleration;
  float max_x_speed;
  bool is_jumping;
  bool is_accelerating;
  bool has_jumped;
};

#endif
