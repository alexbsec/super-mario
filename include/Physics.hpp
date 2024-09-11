#ifndef PHYSICS_HPP
#define PHYSICS_HPP

struct BodyPhysics {
  float gravity;
  float terminal_velocity;
  float x_speed;
  float y_speed;
  float jump_speed;
  bool is_jumping;
};

#endif
