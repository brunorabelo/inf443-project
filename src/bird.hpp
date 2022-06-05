//
// Created by bruno on 04/06/22.
//
#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"

#ifndef PROJECT_BIRD_HPP
#define PROJECT_BIRD_HPP


class Bird {

    cgp::mesh_drawable body;
    cgp::mesh_drawable head;
    cgp::mesh_drawable wing_right;
    cgp::mesh_drawable wing_left;
    cgp::mesh_drawable eye_right;
    cgp::mesh_drawable eye_left;
    cgp::mesh_drawable tail;

public:
    cgp::hierarchy_mesh_drawable hierarchy;

    void setup();

    void display(environment_camera environment);

    void display_wireframe(environment_camera coords);

    void rotate(cgp::rotation_transform rt);

    void translate(cgp::vec3 t);

    void animate(float t, float speed_z);
};


#endif //PROJECT_BIRD_HPP
