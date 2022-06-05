#pragma once

#include "cgp/cgp.hpp"
#include "boids.hpp"
#include "bird.hpp"
#include "water.hpp"
#include "terrain.hpp"
#include "draw.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
    bool display_frame = true;
    bool display_wireframe = false;
    bool display_terrain = false;
    bool display_tree = false;
    bool display_mushroom = false;
    bool display_billboard = false;
    bool display_boids = true;
    bool display_cube = true;
    bool display_bird = true;
	bool compute_lighting   = true;
	bool display_water      = true;
	bool display_cone = true;
	bool reflect = true;
	bool terrain_modeling_mode = false;
};



// The structure of the custom scene
struct scene_structure {

    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //

    cgp::mesh_drawable global_frame;          // The standard global frame
    cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
    cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension_size)

    gui_parameters gui;                       // Standard GUI element storage


	GLuint reflectable_shader;
	GLuint water_shader;
    GLuint fbo_reflection;

    cgp::mesh_drawable tree;
    cgp::mesh_drawable mushroom;
    cgp::mesh_drawable billboard;
	cgp::mesh terrain;
	cgp::mesh_drawable terrain_visual;
	terrain_parameters tparams;

    cgp::mesh water;
    cgp::mesh_drawable water_visual;

    cgp::mesh cone;
    cgp::mesh_drawable cone_visual;

    // -----

    std::vector<cgp::vec3> tree_position;
    std::vector<cgp::vec3> mushroom_position;
    std::vector<cgp::vec3> billboard_position;

    Boids boids;

    Bird bird;


    // Timer used for the interpolation of the position
    cgp::timer_event_periodic timer;


    void display_trees();

    void display_mushroom();

    void display_billboard();

    void display_boids();


    // ****************************** //
    // Functions
    // ****************************** //

    void initialize();  // Standard initialization to be called before the animation loop
    void animate();

    void display();     // The frame display to be called within the animation loop
    void display_gui(); // The display of the GUI, also called within the animation loop


};





