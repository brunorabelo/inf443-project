//
// Created by bruno on 04/06/22.
//

#include "bird.hpp"
#include "draw.hpp"
#include "environment_camera/environment_camera.hpp"

using namespace cgp;


void Bird::setup() {
    // initalize a bird
    GLuint shader = opengl_load_shader("shaders/reflectable/vert.glsl", "shaders/reflectable/frag.glsl");

    float scale = 1;
    mesh m;
    m.push_back(mesh_primitive_arrow());
    m.push_back(mesh_primitive_ellipsoid({0.8, 0.8, 2}));
    auto bird_texture = opengl_load_texture_image("assets/bird.jpg");
    body.initialize(m, "body", shader);
    body.texture = opengl_load_texture_image("assets/bird2.jpg");
    body.transform.scaling = scale;

    head.initialize(mesh_primitive_sphere(0.9f), "head", shader);
    head.texture = bird_texture;
    head.transform.scaling = scale;

    wing_left.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_left", shader);
    wing_left.texture = bird_texture;
    wing_left.transform.scaling = scale;

    wing_right.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_right", shader);
    wing_right.transform.rotation = rotation_transform::from_axis_angle({0, 0, 1}, Pi);
    wing_right.texture = bird_texture;
    wing_right.transform.scaling = scale;

    eye_left.initialize(mesh_primitive_sphere(0.2f), "eye_left", shader);
    eye_left.shading.color = {1.0f, 1.0f, 1.0f};
    eye_left.transform.scaling = scale;

    eye_right.initialize(mesh_primitive_sphere(0.2f), "eye_right", shader);
    eye_right.shading.color = {1.0f, 1.0f, 1.0f};
    eye_right.transform.scaling = scale;




    // Generat the hierarchy
    hierarchy.add(body);
    hierarchy.add(head, "body", {0, 0, 1.8});
    hierarchy.add(eye_right, "head", {0.4f, 0.3f, 0.6f});
    hierarchy.add(eye_left, "head", {-0.4f, 0.3f, 0.6f});
    hierarchy.add(wing_left, "body", {0, 0.2f, 0.8f});
    hierarchy.add(wing_right, "body", {0, 0.2f, 0.8f});

    hierarchy["body"].transform.scaling = 0.2f;
    hierarchy.update_local_to_global_coordinates();
}


void Bird::display(environment_camera environment, bool wireframe) {
    draw_reflectable(hierarchy, environment, false);
    draw_reflectable(hierarchy, environment, true);

    if (wireframe)
        draw_wireframe(hierarchy, environment);
}

void Bird::display_wireframe(environment_camera coords) {
    draw_wireframe(hierarchy, coords);
}

void Bird::animate(float t, float speed_z) {
    // animate the wings of the bird according to the z coordinate of the movement
    float phase = 0;
    if (speed_z < 0)
        return;
    float period = speed_z / 10.0;
    hierarchy["wing_left"].transform.rotation = rotation_transform::from_axis_angle({0, 0, 1},
                                                                                    Pi / 8.0f *
                                                                                    (1 + std::cos(period * t + phase)));
    hierarchy["wing_right"].transform.rotation = rotation_transform::from_axis_angle({0, 0, -1}, Pi / 8.0f * (1 +
                                                                                                              std::cos(
                                                                                                                      period *
                                                                                                                      t +
                                                                                                                      phase)));
    hierarchy["head"].transform.translation = vec3{0, 0, 1.8} + vec3{0, 0, 0.1f * std::cos(2.5 * t)};
    hierarchy["head"].transform.rotation = rotation_transform::from_axis_angle({0, 1, 0}, Pi / 8.0f * std::cos(5 * t));
    hierarchy.update_local_to_global_coordinates();
}

void Bird::rotate(cgp::rotation_transform rt) {
    hierarchy["body"].transform.rotation = rt;
    hierarchy.update_local_to_global_coordinates();
}

void Bird::translate(cgp::vec3 t) {
    hierarchy["body"].transform.translation = t;
    hierarchy.update_local_to_global_coordinates();
}


