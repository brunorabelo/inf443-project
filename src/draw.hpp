#pragma once

#include "cgp/cgp.hpp"

GLuint initialize_reflection_buffer();

template <typename SCENE_ENVIRONMENT>
void draw_reflection(cgp::mesh_drawable const& drawable,
	SCENE_ENVIRONMENT const& environment, GLuint const fbo);

void draw_water();