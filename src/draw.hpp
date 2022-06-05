#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"

GLuint initialize_reflection_buffer();


void draw_reflectable(cgp::mesh_drawable const& drawable,
                      environment_camera environment,
                      GLuint const fbo, bool reflect, bool compute_lighting = true);

void draw_terrain(cgp::mesh_drawable const& terrain_visual,
                  environment_camera environment,
                  GLuint const fbo, bool reflect);


void draw_water(cgp::mesh_drawable const& drawable, environment_camera environment);