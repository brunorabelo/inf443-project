#pragma once

#include "cgp/cgp.hpp"
//#include "cgp/display/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "environment_camera/environment_camera.hpp"

GLuint initialize_reflection_buffer();


void draw_reflectable(cgp::mesh_drawable const& drawable,
                      environment_camera const& environment,
                    bool reflection, bool compute_lighting = true);

void draw_reflectable(cgp::hierarchy_mesh_drawable const& hierarchy,
                      environment_camera const& environment,
	bool reflection, bool compute_lighting = true);

void draw_reflectable(cgp::hierarchy_mesh_drawable_node const& node,
                      environment_camera const& environment,
    bool reflection, bool compute_lighting);

void draw_terrain(cgp::mesh_drawable const& terrain_visual,
                  environment_camera const& environment,
    GLuint const fbo, bool reflect);

void draw_water_mask(cgp::mesh_drawable const& water_visual, environment_camera const& environment);
void draw_water(cgp::mesh_drawable const& drawable, environment_camera const& environment);