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
	opengl_uniform(drawable.shader, "compute_lighting", compute_lighting, false);
	opengl_uniform(drawable.shader, "reflection", reflection, false);

	// Blend reflections into image, supposing alpha mask already drawn by draw_water_mask()
	if (reflection) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_ALPHA, GL_DST_ALPHA);
		//glDepthMask(true);

		draw(drawable, environment);

		glDisable(GL_BLEND);
	}
	else
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

// Draws "mask" on buffer alpha values based on water Fresnel coefficients
// to tell where reflections should be visible
void draw_water_mask(mesh_drawable const& water_visual, environment_camera const& environment)
{
	glEnable(GL_BLEND);
	glDepthMask(false);

	glUseProgram(water_visual.shader); // assuming the water has the right shader
	opengl_uniform(water_visual.shader, "alpha_only", true);

	glBlendFunc(GL_ONE, GL_ONE);
	draw(water_visual, environment);

	glDepthMask(true);
	glDisable(GL_BLEND);
}

// Draws semitransparent water
void draw_water(mesh_drawable const& water_visual, environment_camera const& environment) {
	glEnable(GL_BLEND);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth buffer writing, important
	glDepthMask(false);

	draw(water_visual, environment);

	glDepthMask(true);
	glDisable(GL_BLEND);
}