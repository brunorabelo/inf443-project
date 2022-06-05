//
// Created by bruno on 04/06/22.
//

#ifndef PROJECT_BOIDS_HPP
#define PROJECT_BOIDS_HPP
#pragma once

#include "cgp/cgp.hpp"
#include "bird.hpp"

static int count=0;

class Boid {


public:
    Bird bird;
    bool perching = false;
    float perch_timer;

    cgp::vec3 position; // position
    cgp::vec3 velocity; // velocity
    cgp::vec3 normal;
    std::string name;


    Boid(cgp::vec3 position, cgp::vec3 velocity, float perch_timer);

    void update();

    bool isEqual(Boid b);

};

class Boids {

private:

    cgp::vec3 dimension_center;
    float initial_dimension_size = 20.0f;

    cgp::vec3 rule1(Boid &boid);

    cgp::vec3 rule2(Boid &boid);

    cgp::vec3 rule3(Boid &boid);

    void addBoid(cgp::vec3 position, cgp::vec3 speed);

public:
    std::vector<Boid> boids_vector;
    cgp::mesh_drawable cube_mesh_drawable;

    float dimension_size = 80.0f;
    float damping_factor_rule1 = 71.0f;
    float damping_factor_rule2 = 10.0f;
    float damping_factor_rule3 = 40.0f;
    float minimal_distance = 7.0f;
    float damping_speed = 0.245f;
    float max_speed = 15.0f;
    float perch_timer = 10.0f;

    Boids(float size, cgp::vec3 center);

    Boids();

    void setup();

    void animate(float d, float d1);

    void update();

    cgp::vec3 bound_position(Boid &boid);

    cgp::vec3 limit_velocity(cgp::vec3 velocity);
};


#endif //PROJECT_BOIDS_HPP
