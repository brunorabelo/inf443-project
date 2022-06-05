#pragma once

#include "cgp/cgp.hpp"
//#include "cgp/display/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"

GLuint initialize_reflection_buffer();


void draw_reflectable(cgp::mesh_drawable const& drawable,
                    cgp::scene_environment_basic_camera_spherical_coords const& environment,
                    bool reflection, bool compute_lighting = true);

void draw_reflectable(cgp::hierarchy_mesh_drawable const& hierarchy,
	cgp::scene_environment_basic_camera_spherical_coords const& environment,
	bool reflection, bool compute_lighting = true);

void draw_reflectable(cgp::hierarchy_mesh_drawable_node const& node,
    cgp::scene_environment_basic_camera_spherical_coords const& environment,
    bool reflection, bool compute_lighting);

void draw_terrain(cgp::mesh_drawable const& terrain_visual,
    cgp::scene_environment_basic_camera_spherical_coords const& environment,
    GLuint const fbo, bool reflect);


void draw_water(cgp::mesh_drawable const& drawable, cgp::scene_environment_basic_camera_spherical_coords const& environment);