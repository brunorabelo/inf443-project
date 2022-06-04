//
// Created by bruno on 04/06/22.
//

#ifndef PROJECT_BOIDS_HPP
#define PROJECT_BOIDS_HPP
#pragma once

#include "cgp/cgp.hpp"

class Boid {

public:
    cgp::mesh_drawable mesh_drawable;
    cgp::vec3 position; // position
    cgp::vec3 velocity; // velocity

    Boid(cgp::vec3 position, cgp::vec3 speed) {
        position = position;
        speed = speed;
        mesh_drawable.initialize(mesh_primitive_cone(0.5f, 1.0f, position, {1, 0, 0}));
    }

    void update();

};

class Boids {


public:
    std::vector<Boid> boids;
    cgp::mesh_drawable rectangle_mesh_drawable;


    void addBoid(cgp::vec3 position, cgp::vec3 speed);

    void addBoid(Boid b);

    void setup();

    void move_new_positions(float d);
};


#endif //PROJECT_BOIDS_HPP
