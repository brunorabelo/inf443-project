//
// Created by bruno on 04/06/22.
//

#ifndef PROJECT_BOIDS_HPP
#define PROJECT_BOIDS_HPP
#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"
#include "boid.hpp"


class Boids {
private:
    cgp::vec3 dimension_center;
    float initial_dimension_size = 20.0f;
    std::vector<Boid> boids_vector;
    cgp::mesh_drawable cube_mesh_drawable;

    cgp::vec3 rule1(Boid &boid);

    cgp::vec3 rule2(Boid &boid);

    cgp::vec3 rule3(Boid &boid);

    cgp::vec3 bound_position(Boid &boid);

    cgp::vec3 limit_velocity(cgp::vec3 velocity);

    void addBoid(cgp::vec3 position, cgp::vec3 speed);

    void update();

    static boid_object create_boid_object(std::string name);


public:

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

    void animate(float dt, float total_time);


    void display(environment_camera environment, bool wireframe, bool cube);

};


#endif //PROJECT_BOIDS_HPP
