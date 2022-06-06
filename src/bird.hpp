//
// Created by bruno on 04/06/22.
//
#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"
#include "boids_engine/boid_object.hpp"

#ifndef PROJECT_BIRD_HPP
#define PROJECT_BIRD_HPP


class Bird : public boid_object {

    cgp::mesh_drawable body;
    cgp::mesh_drawable head;
    cgp::mesh_drawable wing_right;
    cgp::mesh_drawable wing_left;
    cgp::mesh_drawable eye_right;
    cgp::mesh_drawable eye_left;
    cgp::mesh_drawable tail;
    cgp::hierarchy_mesh_drawable hierarchy;

    void display_wireframe(environment_camera coords);

public:


    void setup();

    void display(environment_camera environment, bool wireframe);

    void animate(float t, float speed_z);

    void rotate(cgp::rotation_transform rt);

    void translate(cgp::vec3 t);
};


#endif //PROJECT_BIRD_HPP
