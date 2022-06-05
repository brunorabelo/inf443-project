#include "draw.hpp"
#include "environment_camera/environment_camera.hpp"

using namespace cgp;

GLuint initialize_reflection_buffer() {
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	return fbo;
}

void draw_reflectable(mesh_drawable const& drawable,
                      environment_camera const& environment,
	bool reflection, bool compute_lighting)
{
	// Setup shader
	assert_cgp(drawable.shader != 0, "Try to draw reflectable without shader [name:" + drawable.name + "]");
	glUseProgram(drawable.shader); opengl_check;

	// Send custom uniforms to it
	// compute_lighting => if false, will display texture color instead, no shading calculations done
	// reflection => if false, draw real object; if true, draw its reflection
	opengl_uniform(drawable.shader, "compute_lighting", compute_lighting);
	opengl_uniform(drawable.shader, "reflection", reflection);

	// Rest of the work
	draw(drawable, environment);
}

void draw_reflectable(hierarchy_mesh_drawable_node const& node,
                      environment_camera const& environment,
	bool reflection, bool compute_lighting)
{
	// copy of the mesh drawable (lightweight element) - to preserve its uniform parameters
	mesh_drawable visual_element = node.drawable;

	// Update local uniform values (and combine them with uniform already stored in the mesh)
	visual_element.transform = node.global_transform * visual_element.transform;

	if (visual_element.shader != 0)
		draw_reflectable(visual_element, environment, reflection, compute_lighting); // only change with respect to draw()
}

void draw_reflectable(hierarchy_mesh_drawable const& hierarchy,
                      environment_camera const& environment,
			bool reflection, bool compute_lighting)
{
	const size_t N = hierarchy.elements.size();
	for (size_t k = 0; k < N; ++k)
	{
		hierarchy_mesh_drawable_node const& node = hierarchy.elements[k];
		draw_reflectable(node, environment, reflection, compute_lighting);  // only change with respect to draw()
	}
}

// Deactivates z-buffer to draw semitransparent water
void draw_water(mesh_drawable const& water_visual, environment_camera const& environment)
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