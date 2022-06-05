//
// Created by bruno on 04/06/22.
//

#include "bird.hpp"
#include "environment_camera/environment_camera.hpp"

using namespace cgp;


void Bird::setup() {
    float scale = 1;
    mesh m;
    m.push_back(mesh_primitive_arrow());
    m.push_back(mesh_primitive_ellipsoid({1, 1, 1.5}));
    body.initialize(m, "body");
    body.shading.color = {0, 0, 0};
    body.transform.scaling = scale;

    head.initialize(mesh_primitive_sphere(0.9f), "head");
    head.shading.color = {0.0f, 0, 0};
    head.transform.scaling = scale;

    wing_left.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_left");
//    wing_left.transform.translation = {0,0,1};
    wing_left.shading.color = {1.0f, 0, 0};
    wing_left.transform.scaling = scale;

    wing_right.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_right");
    wing_right.transform.rotation = rotation_transform::from_axis_angle({0, 0, 1}, Pi);
//    wing_right.transform.translation = {-1,-1,1};
    wing_right.shading.color = {1, 1, 0};
    wing_right.transform.scaling = scale;

    eye_left.initialize(mesh_primitive_sphere(0.2f), "eye_left");
    eye_left.shading.color = {0.2f, 0.2f, 0.2f};
    eye_left.transform.scaling = scale;

    eye_right.initialize(mesh_primitive_sphere(0.2f), "eye_right");
    eye_right.shading.color = {0.2f, 0.2f, 0.2f};
    eye_right.transform.scaling = scale;


    // Generat the hierarchy
    hierarchy.add(body);
    hierarchy.add(head, "body", {0, 0, 1.8});
    hierarchy.add(eye_right, "head", {0.4f, 0.3f, 0.6f});
    hierarchy.add(eye_left, "head", {-0.4f, 0.3f, 0.6f});
    hierarchy.add(wing_left, "body", {0, 0.2f, 0.8f});
    hierarchy.add(wing_right, "body", {0, 0.2f, 0.8f});

}


void Bird::display(environment_camera environment) {
    draw(hierarchy, environment);
}

void Bird::display_wireframe(environment_camera coords) {
    draw_wireframe(hierarchy, coords);
}

void Bird::animate(float t, float speed_z) {

    float phase = 0;
    if (speed_z < 0)
        return;
    float period = speed_z / 8.0;
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


