#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"


using namespace cgp;

void scene_structure::initialize() {
    // Basic set-up
    // ***************************************** //
    global_frame.initialize(mesh_primitive_frame(), "Frame");
    environment.camera.axis = camera_spherical_coordinates_axis::z;
    environment.camera.look_at({60.0f, 0.0f, 0.0f}, {0, 0, 10.0f});

    // Initialize the terrain
    int N_terrain_samples = 200;
    float terrain_length = 20;
    mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
    terrain.initialize(terrain_mesh, "terrain");
    terrain.shading.color = {0.6f, 0.85f, 0.5f};
    terrain.shading.phong.specular = 0.0f; // non-specular terrain material
    terrain.texture = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);

    // Initialize the trees
    tree.initialize(create_tree(), "Tree");
    tree_position = generate_positions_on_terrain(100, terrain_length);

    // Initialize the mushrooms
    mushroom.initialize(create_mushroom(), "Mushroom");
    mushroom_position = generate_positions_on_terrain(75, terrain_length);

    // Initialize the grass of billboards
    billboard.initialize(mesh_primitive_quadrangle({-0.5f, 0, 0}, {0.5f, 0, 0}, {0.5f, 0, 1}, {-0.5f, 0, 1}), "Quad");
    billboard.texture = opengl_load_texture_image("assets/grass.png");
    billboard.shading.phong = {0.4f, 0.6f, 0, 1};
    billboard.transform.scaling = 0.6f;
    billboard_position = generate_positions_on_terrain(75, terrain_length);

    //initalize boids_vector
    boids.setup();

}


void scene_structure::display() {

    // Basic elements of the scene
    environment.light = environment.camera.position();
    if (gui.display_frame)
        draw(global_frame, environment);


    // Display the other elements:

    if (gui.display_terrain) {
        draw(terrain, environment);
        if (gui.display_wireframe)
            draw_wireframe(terrain, environment);
    }

    if (gui.display_tree) display_trees();
    if (gui.display_mushroom) display_mushroom();
    if (gui.display_billboard) display_billboard();
    if (gui.display_boids) display_boids();

}


void scene_structure::display_trees() {
    vec3 const offset = {0, 0, 0.05f};
    for (vec3 position: tree_position) {
        tree.transform.translation = position - offset;
        draw(tree, environment);

        if (gui.display_wireframe)
            draw_wireframe(tree, environment);
    }
}

void scene_structure::display_mushroom() {
    vec3 const offset = {0, 0, 0.02f};
    for (vec3 position: mushroom_position) {
        mushroom.transform.translation = position - offset;
        draw(mushroom, environment);

        if (gui.display_wireframe)
            draw_wireframe(mushroom, environment);
    }
}

void scene_structure::display_billboard() {
    // Find the rotation such that:
    //  the right-direction of the billboard is turned to match the right-direction of the camera => R*{1,0,0} = camera_right
    //  but the billboard remains oriented upward in the z direction => R*{0,0,1} = {0,0,1}
    vec3 camera_right = environment.camera.right();
    rotation_transform R = rotation_transform::between_vector({1, 0, 0}, {0, 0, 1}, camera_right, {0, 0, 1});
    billboard.transform.rotation = R;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);

    vec3 const offset = {0, 0, 0.02f};
    for (vec3 position: billboard_position) {
        billboard.transform.translation = position - offset;
        draw(billboard, environment);
    }

    glDepthMask(true);
    glDisable(GL_BLEND);


    if (gui.display_wireframe) {
        for (vec3 position: billboard_position) {
            billboard.transform.translation = position - offset;
            draw_wireframe(billboard, environment);
        }
    }
}

void scene_structure::animate() {

    // Update the current time
    float dt = timer.update();
    boids.move_new_positions(dt);
}

void scene_structure::display_boids() {
    boids.update();
    for (Boid &boid: boids.boids_vector) {
        boid.update();
        draw(boid.mesh_drawable, environment);

        if (gui.display_wireframe) {
            draw_wireframe(boid.mesh_drawable, environment);
        }
    }
    if (gui.display_cube) {
        draw_wireframe(boids.cube_mesh_drawable, environment);
    }
}


void scene_structure::display_gui() {
    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);

    ImGui::Checkbox("terrain", &gui.display_terrain);
    ImGui::Checkbox("tree", &gui.display_tree);
    ImGui::Checkbox("mushroom", &gui.display_mushroom);
    ImGui::Checkbox("billboard", &gui.display_billboard);
    ImGui::Checkbox("boids_vector", &gui.display_boids);
    ImGui::Checkbox("cube", &gui.display_cube);

    ImGui::SliderFloat("cube_dimension", &boids.dimension_size, 10, 100);
    ImGui::SliderFloat("damping_factor_rule_1", &boids.damping_factor_rule1, 1, 100);
    ImGui::SliderFloat("damping_factor_rule_2", &boids.damping_factor_rule2, 1, 100);
    ImGui::SliderFloat("damping_factor_rule_3", &boids.damping_factor_rule3, 1, 100);
    ImGui::SliderFloat("damping_speed", &boids.damping_speed, 0.1, 2);
    ImGui::SliderFloat("minimal_distance", &boids.minimal_distance, 0.5, 10);
    ImGui::SliderFloat("maximal_speed", &boids.max_speed, 0.5, 10);
    ImGui::SliderFloat("perch_timer", &boids.perch_timer, 1, 20);

}


