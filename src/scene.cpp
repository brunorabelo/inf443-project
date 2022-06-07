#include "scene.hpp"
#include "draw.hpp"

using namespace cgp;

void scene_structure::initialize() {

    skybox.initialize("assets/skybox/");
    // Basic set-up
    // ***************************************** //
    global_frame.initialize(mesh_primitive_frame(), "Frame");

    environment.camera.axis = camera_spherical_coordinates_axis::z;
    environment.camera.distance_to_center = 120.0f;

    // Import shaders & textures
    reflectable_shader = opengl_load_shader("shaders/reflectable/vert.glsl", "shaders/reflectable/frag.glsl");
    water_shader = opengl_load_shader("shaders/water/vert.glsl", "shaders/water/frag.glsl");
    GLuint const terrain_texture = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);
    fbo_reflection = initialize_reflection_buffer();


    // Environment settings
    environment.background_color = {0.529, 0.808, 0.922};
    environment.light = {100, 0, 10};


    // Initialize terrain
    terrain = create_terrain_mesh(tparams);
    terrain_visual.initialize(terrain, "terrain", reflectable_shader, terrain_texture);
    update_terrain(terrain, terrain_visual, tparams);

    // Initialize water
    water = create_water_mesh(200.0f);
    water_visual.initialize(water, "water", water_shader);
    water_visual.shading.color = {0.5f, 0.5f, 1.0f};
    water_visual.shading.alpha = 0.5f;

    // Test object for reflectable shader
    cone = mesh_primitive_cone(20.0f, 40.0f, vec3(0.0f, 0.0f, 35.0f), vec3(0, 0, -1));
    cone_visual.initialize(cone, "cone", reflectable_shader);

    //initalize vector
    boids1 = Boids(100, vec3{0, 50, 70});
    boids2 = Boids(60, vec3{0, -70, 60});

    boids1.setup();
    boids2.setup();
    boid = boids2.get_boid();
    // Christ the Redeemer statue
    christ.initialize(mesh_load_file_obj("assets/christ.obj"), "christ", reflectable_shader);
    christ.texture = opengl_load_texture_image("assets/marmore1.jpg");
    christ.shading.phong = {0.3f, 0.6f, 0, 64.0f};
    christ.transform.scaling = 0.01f;
    christ.transform.translation = {0, -40, 30.0f};
    christ.transform.rotation = rotation_transform::from_axis_angle({0, 0, 1}, Pi);

}


void scene_structure::update_camera() {
    // setting variables
    custom_inputs_keyboard_parameters const &keyboard = inputs.keyboard;
    camera_spherical_coordinates &camera = environment.camera;

    // defining the angles the camera will rotate given the mouse pointer coordinates
    vec2 mouse_position = inputs.mouse.position.current;

    float sensibility = 0.5;

    float theta = mouse_position.y / camera_rotation_damping;
    float phi = mouse_position.x / camera_rotation_damping;
    if (gui.mouse_direction && !inputs.mouse.on_gui && (pow(mouse_position.y, 2) + pow(mouse_position.x, 2)) > 0.10) {
        camera.manipulator_rotate_spherical_coordinates(phi, -theta);
    }

    if (gui.camera_bird) {
        environment.camera.center_of_rotation = boid->position;
    }

    if (keyboard.up || keyboard.w_key) {
        if (keyboard.shift)
            camera.center_of_rotation += camera_speed * vec3{0, 0, 1};
        else if (keyboard.ctrl)
            camera.manipulator_rotate_spherical_coordinates(0, sensibility / camera_rotation_damping);
        else camera.center_of_rotation += camera_speed * camera.front();
    }
    if (keyboard.down || keyboard.s_key) {
        if (keyboard.shift)
            camera.center_of_rotation -= camera_speed * vec3{0, 0, 1};
        else if (keyboard.ctrl)
            camera.manipulator_rotate_spherical_coordinates(0, -sensibility / camera_rotation_damping);
        else camera.center_of_rotation -= camera_speed * camera.front();
    }
    if (keyboard.left || keyboard.a_key) {
        if (keyboard.ctrl)
            camera.manipulator_rotate_spherical_coordinates(sensibility / camera_rotation_damping, 0);
        else
            camera.center_of_rotation -= camera_speed * camera.right();
    }
    if (keyboard.right || keyboard.d_key) {
        if (keyboard.ctrl)
            camera.manipulator_rotate_spherical_coordinates(-sensibility / camera_rotation_damping, 0);
        else
            camera.center_of_rotation += camera_speed * camera.right();
    }

}

void scene_structure::display(float dt, float total_time) {
    draw(skybox, environment);

    // Basic elements of the scene
    environment.light = environment.camera.position();
    if (gui.display_frame)
        draw(global_frame, environment);


    if (gui.display_cone) {
        draw_reflectable(cone_visual, environment, false, gui.compute_lighting);

        if (gui.reflect)
            draw_reflectable(cone_visual, environment, true, gui.compute_lighting);
    }

    // display water
    if (gui.display_water)
        draw_water(water_visual, environment);

    // display terrain
    if (gui.display_terrain) {
        if (gui.reflect)
            draw_reflectable(terrain_visual, environment, true, gui.compute_lighting);

        draw_reflectable(terrain_visual, environment, false, gui.compute_lighting);

        if (gui.display_wireframe)
            draw_wireframe(terrain_visual, environment);
    }


    // display boids
    boids1.animate(dt, total_time);
    boids2.animate(dt, total_time);
    if (gui.display_boids) {
        boids1.display(environment, gui.display_wireframe, gui.display_cube);
        boids2.display(environment, gui.display_wireframe, gui.display_cube);
    }


    draw_reflectable(christ, environment, false, gui.compute_lighting);
    if (gui.reflect)
        draw_reflectable(christ, environment, true, gui.compute_lighting);


}

void scene_structure::display_gui() {
    ImGui::Checkbox("Bird Camera", &gui.camera_bird);
    ImGui::Checkbox("Mouse camera direction", &gui.mouse_direction);
    ImGui::SliderFloat("Camera distance to center", &environment.camera.distance_to_center, 1.0f, 200.0f);
    ImGui::Checkbox("Dev Mode", &gui.dev_mode);
    if (!gui.dev_mode)
        return;

    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);
    ImGui::SliderFloat("Camera Speed", &camera_speed, 1.0f, 10.0f);
    ImGui::SliderFloat("Camera Rotation Damping", &camera_rotation_damping, 1.0f, 50.0f);

    bool pressed = ImGui::Button("Reset Camera");
    if (pressed)
        reset_camera();

    ImGui::Checkbox("Compute lighting", &gui.compute_lighting);

    ImGui::Checkbox("Display terrain", &gui.display_terrain);
    ImGui::Checkbox("Display water", &gui.display_water);
    ImGui::Checkbox("Display cone", &gui.display_cone);

    ImGui::Checkbox("Reflect", &gui.reflect);

    if (gui.display_terrain)
        ImGui::Checkbox("Terrain modeling mode", &gui.terrain_modeling_mode);

    ImGui::SliderFloat("Position Z", &terrain_visual.transform.translation.z, -30.0f, 30.0f);
    if (gui.display_cone)
        ImGui::SliderFloat("Cone translation", &cone_visual.transform.translation.z, -30.0f, 60.0f);

    if (gui.display_terrain && gui.terrain_modeling_mode) {
        bool update = false;
        update |= ImGui::SliderFloat("Persistance", &tparams.persistency, 0.1f, 0.6f);
        update |= ImGui::SliderFloat("Frequency gain", &tparams.frequency_gain, 1.5f, 2.5f);
        update |= ImGui::SliderInt("Octave", &tparams.octave, 1, 8);
        update |= ImGui::SliderFloat("Height", &tparams.terrain_height, 30.0f, 70.0f);
        update |= ImGui::SliderFloat("Scale", &tparams.scale, 0.1f, 2.0f);
        update |= ImGui::SliderFloat("Perlin offset X", &tparams.perlin_offsetx, -2.0f, 2.0f);
        update |= ImGui::SliderFloat("Perlin offset Y", &tparams.perlin_offsety, -2.0f, 2.0f);
        update |= ImGui::SliderFloat("Offset Z", &tparams.offsetz, -50.0f, 10.0f);
        update |= ImGui::SliderFloat("Cutoff length", &tparams.cutoff_len, 0.01f, 1);
        update |= ImGui::SliderInt("Cutoff exponent", &tparams.cutoff_n, 2, 10);

        if (update)// if any slider has been changed - then update the terrain
            update_terrain(terrain, terrain_visual, tparams);
    }

    ImGui::Checkbox("boids_vector", &gui.display_boids);

    if (gui.display_boids)
        ImGui::Checkbox("Boids modeling mode", &gui.boids_modeling_mode);
    if (gui.display_boids && gui.boids_modeling_mode) {

        ImGui::Checkbox("boids_vector", &gui.display_boids);
        ImGui::Checkbox("cube", &gui.display_cube);
        ImGui::SliderFloat("cube_dimension", &boids1.dimension_size, 10, 100);
        ImGui::SliderFloat("damping_factor_rule_1", &boids1.damping_factor_rule1, 1, 100);
        ImGui::SliderFloat("damping_factor_rule_2", &boids1.damping_factor_rule2, 1, 100);
        ImGui::SliderFloat("damping_factor_rule_3", &boids1.damping_factor_rule3, 1, 100);
        ImGui::SliderFloat("damping_speed", &boids1.damping_speed, 0.1, 1.5);
        ImGui::SliderFloat("minimal_distance", &boids1.minimal_distance, 0.5, 10);
        ImGui::SliderFloat("maximal_speed", &boids1.max_speed, 5, 20);
        ImGui::SliderFloat("perch_timer", &boids1.perch_timer, 1, 20);

    }

}

void scene_structure::reset_camera() {
    // reset the camer to the initial state
    environment.camera.axis = camera_spherical_coordinates_axis::z;
    environment.camera.look_at({120.0f, -20.0f, 20.0f}, {0, -40, 0.0f});
}




