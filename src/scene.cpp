#include "scene.hpp"
#include "draw.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //

	global_frame.initialize(mesh_primitive_frame(), "Frame");
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 15.0f,6.0f,6.0f }, { 0,0,0 });

	// Import shaders & textures
	reflectable_shader = opengl_load_shader("shaders/reflectable/vert.glsl", "shaders/reflectable/frag.glsl");
	GLuint const terrain_texture = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);
	fbo_reflection = initialize_reflection_buffer();

	// Background color
	environment.background_color = { 0.529, 0.808, 0.922 };

	// Initialize terrain
	terrain = create_terrain_mesh(tparams);
	terrain_visual.initialize(terrain, "terrain", reflectable_shader, terrain_texture);
	update_terrain(terrain, terrain_visual, tparams);

	// Initialize water
	water = create_water_mesh(100.0f);
	water_visual.initialize(water, "water");
	water_visual.shading.color = { 0.5f, 0.5f, 1.0f };
	water_visual.shading.alpha = 0.5f;

	// Test object for mesh_reflectable
	cone = mesh_primitive_cone(1.0f, 2.0f, vec3(0.0f, 0.0f, -0.5f));
	cone_visual.initialize(cone, "cone", reflectable_shader);
}


void scene_structure::display()
{

	// Basic elements of the scene
	environment.light = environment.camera.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Display the other elements:

	if(gui.display_cone)
		draw_reflectable(cone_visual, environment, GL_FRAMEBUFFER, false);

	if(gui.reflect)
		draw_reflectable(cone_visual, environment, GL_FRAMEBUFFER, true);

	if (gui.display_terrain) {
		draw_reflectable(terrain_visual, environment, GL_FRAMEBUFFER, false);

		if (gui.reflect)
			draw_reflectable(terrain_visual, environment, GL_FRAMEBUFFER, true);
		if (gui.display_wireframe)
			draw_wireframe(terrain_visual, environment);
	}

	if (gui.display_water)
		draw_water(water_visual, environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	ImGui::Checkbox("Display terrain", &gui.display_terrain);
	ImGui::Checkbox("Display water", &gui.display_water);

	ImGui::Checkbox("Reflect", &gui.reflect);

	ImGui::Checkbox("Terrain modeling mode", &gui.terrain_modeling_mode);

	if (gui.terrain_modeling_mode) {
		bool update = false;
		update |= ImGui::SliderFloat("Persistance", &tparams.persistency, 0.1f, 0.6f);
		update |= ImGui::SliderFloat("Frequency gain", &tparams.frequency_gain, 1.5f, 2.5f);
		update |= ImGui::SliderInt("Octave", &tparams.octave, 1, 8);
		update |= ImGui::SliderFloat("Height", &tparams.terrain_height, 10.0f, 50.0f);
		update |= ImGui::SliderFloat("Scale", &tparams.scale, 0.1f, 2.0f);
		update |= ImGui::SliderFloat("Perlin offset X", &tparams.perlin_offsetx, -2.0f, 2.0f);
		update |= ImGui::SliderFloat("Perlin offset Y", &tparams.perlin_offsety, -2.0f, 2.0f);
		update |= ImGui::SliderFloat("Offset Z", &tparams.offsetz, -50.0f, 10.0f);

		if (update)// if any slider has been changed - then update the terrain
			update_terrain(terrain, terrain_visual, tparams);
	}
}


