#pragma once

#include "cgp/cgp.hpp"
#include "water.hpp"
#include "terrain.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame      = false;
	bool display_wireframe  = false;
	bool display_terrain    = false;
	bool display_water      = true;
	bool reflect = false;
	bool terrain_modeling_mode = false;
};



// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                       // Standard GUI element storage

	GLuint reflectable_shader;
	GLuint fbo_reflection;

	cgp::mesh terrain;
	cgp::mesh_drawable terrain_visual;
	terrain_parameters tparams;

	cgp::mesh water;
	cgp::mesh_drawable water_visual;

	cgp::mesh cone;
	cgp::mesh_drawable cone_visual;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop


};





