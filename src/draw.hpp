#pragma once

#include "cgp/cgp.hpp"

GLuint initialize_reflection_buffer();


void draw_reflection(cgp::mesh_drawable const& drawable,
                      cgp::scene_environment_basic_camera_spherical_coords const& environment, GLuint const fbo);



void draw_water();