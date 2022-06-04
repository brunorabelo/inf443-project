//
// Created by bruno on 04/06/22.
//

#include "bird.hpp"

using namespace cgp;


void Bird::setup() {
    mesh m;
    m.push_back(mesh_primitive_arrow());
    m.push_back(mesh_primitive_ellipsoid({1, 1, 1.5}));
    body.initialize(m, "body");
    body.shading.color = {0, 0, 0};

    head.initialize(mesh_primitive_sphere(0.9f), "head");
    head.shading.color = {0.0f, 0, 0};

    wing_left.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_left");
    wing_left.transform.translation = {0,0,1};
    wing_left.shading.color = {0.0f, 0, 0};

    wing_right.initialize(mesh_load_file_obj("assets/wing.obj"), "wing_right");
    wing_right.transform.rotation = rotation_transform::from_axis_angle({0, 0, 1}, Pi);
    wing_right.transform.translation = {0,0,1};
    wing_right.shading.color = {1, 0, 0};

    eye_left.initialize(mesh_primitive_sphere(0.2f), "eye_left");
    eye_left.shading.color = {0.2f, 0.2f, 0.2f};

    eye_right.initialize(mesh_primitive_sphere(0.2f), "eye_right");
    eye_right.shading.color = {0.2f, 0.2f, 0.2f};

    // Generat the hierarchy
    hierarchy.add(body);
    hierarchy.add(head, "body", {0, 0, 1.8});
    hierarchy.add(eye_right, "head", {0.4f, 0.3f, 0.6f});
    hierarchy.add(eye_left, "head", {-0.4f, 0.3f, 0.6f});
    hierarchy.add(wing_left, "body", {0, 1.03f, 0});
    hierarchy.add(wing_right, "body", {0, 1.03f, 0});

}

void Bird::display(cgp::scene_environment_basic_camera_spherical_coords environment) {
    draw(hierarchy, environment);
}

void Bird::display_wireframe(cgp::scene_environment_basic_camera_spherical_coords coords) {
    draw_wireframe(hierarchy, coords);
}


