#pragma once

#include "cgp/cgp.hpp"

GLuint initialize_reflection_buffer();


void draw_reflectable(cgp::mesh_drawable const& drawable,
                    cgp::scene_environment_basic_camera_spherical_coords const& environment,
                    GLuint const fbo, bool reflect, bool compute_lighting = true);

void draw_terrain(cgp::mesh_drawable const& terrain_visual,
    cgp::scene_environment_basic_camera_spherical_coords const& environment,
    GLuint const fbo, bool reflect);


void draw_water(cgp::mesh_drawable const& drawable, cgp::scene_environment_basic_camera_spherical_coords const& environment);