#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"
#include "boids_engine/boids.hpp"
#include "water.hpp"
#include "terrain.hpp"
#include "draw.hpp"
#include "inputs/inputs.hpp"

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
    bool display_frame = true;
    bool display_wireframe = true;
    bool mouse_direction = false;
    bool display_terrain = true;
    bool display_boids = false;
    bool display_cube = false;
    bool compute_lighting = true;
    bool display_water = true;
    bool display_cone = false;
    bool reflect = true;
    bool terrain_modeling_mode = false;
    bool boids_modeling_mode = false;
};


// The structure of the custom scene
struct scene_structure {

    // ****************************** //
    // Elements and shapes of the scene
    // ****************************** //

    cgp::mesh_drawable global_frame;          // The standard global frame
    environment_camera environment; // Standard environment controler
    custom_inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension_size)

    gui_parameters gui;                       // Standard GUI element storage


    GLuint reflectable_shader;
    GLuint water_shader;
    GLuint fbo_reflection;

    cgp::skybox_drawable skybox;

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

    mesh_drawable christ;

    // Timer used for the interpolation of the position
    cgp::timer_event_periodic timer;

    float camera_speed = 2.0f; // camera speed
    float camera_rotation_damping = 50.0f;

    // ****************************** //
    // Functions
    // ****************************** //

    void initialize();  // Standard initialization to be called before the animation loop

    void update_camera(); //to update the camera position and orientation in response to user input

    void display(float dt, float total_time);     // The frame display to be called within the animation loop
    void display_gui(); // The display of the GUI, also called within the animation loop


    void reset_camera();
};





