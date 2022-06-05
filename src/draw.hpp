#pragma once

#include "cgp/cgp.hpp"

GLuint initialize_reflection_buffer();


void draw_reflectable(cgp::mesh_drawable const& drawable,
                    cgp::scene_environment_basic_camera_spherical_coords const& environment,
                    GLuint const fbo, bool reflect);



void draw_water(cgp::mesh_drawable const& drawable, cgp::scene_environment_basic_camera_spherical_coords const& environment);