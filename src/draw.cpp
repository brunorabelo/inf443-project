#include "draw.hpp"

using namespace cgp;

GLuint initialize_reflection_buffer() {
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	return fbo;
}

void draw_reflectable(mesh_drawable const& drawable,
	scene_environment_basic_camera_spherical_coords const& environment, GLuint const fbo,
	bool reflect, bool compute_lighting)
{
   // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	if (drawable.number_triangles == 0) return;

	// Setup shader
	assert_cgp(drawable.shader != 0, "Try to draw mesh_drawable without shader [name:" + drawable.name + "]");
	assert_cgp(drawable.texture != 0, "Try to draw mesh_drawable without texture [name:" + drawable.name + "]");
	glUseProgram(drawable.shader); opengl_check;

	// Send uniforms for this shader
	opengl_uniform(drawable.shader, environment);
	opengl_uniform(drawable.shader, drawable.shading);
	opengl_uniform(drawable.shader, "model", drawable.model_matrix());

	// Set texture
	glActiveTexture(GL_TEXTURE0); opengl_check;
	glBindTexture(GL_TEXTURE_2D, drawable.texture); opengl_check;
	opengl_uniform(drawable.shader, "image_texture", 0);  opengl_check;

	// ------- The crucial change with respect to draw() -------
	// Our shader "reflectable" accepts a uniform that tells it to invert z coordinates
	// and another that tells it to shut off lighting computations (e.g. for baked lighting)
	opengl_uniform(drawable.shader, "reflect", reflect, false);
	opengl_uniform(drawable.shader, "compute_lighting", compute_lighting, false);

	// Call draw function
	assert_cgp(drawable.number_triangles > 0, "Try to draw mesh_drawable with 0 triangles [name:" + drawable.name + "]"); opengl_check;
	glBindVertexArray(drawable.vao);   opengl_check;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.vbo.at("index")); opengl_check;
	glDrawElements(GL_TRIANGLES, GLsizei(drawable.number_triangles * 3), GL_UNSIGNED_INT, nullptr); opengl_check;

	// Clean buffers
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Alpha blending needed for terrain ?
void draw_terrain(mesh_drawable const& terrain_visual, scene_environment_basic_camera_spherical_coords const& environment, GLuint const fbo, bool reflect) {
	// Enable use of alpha component as color blending for transparent elements
//  alpha = current_color.alpha
//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	draw_reflectable(terrain_visual, environment, fbo, reflect);

	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}


// This function blends reflections and refractions into one image
void draw_water(mesh_drawable const& water_visual, scene_environment_basic_camera_spherical_coords const& environment)
{
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	draw(water_visual, environment);

	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}