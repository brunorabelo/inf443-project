//
// Created by bruno on 04/06/22.
//

#include "boids.hpp"

using namespace cgp;


void Boids::addBoid(vec3 position = {0, 0, 1}, vec3 speed = {0, 0, 0}) {
    Boid b = Boid(position, speed);
    boids.push_back(b);
}

void Boids::addBoid(Boid b) {
    boids.push_back(b);
}

void Boids::setup() {
    vec3 center = {0, 0, 5};
    int N = 20;
    float dimension = 10.0f;
    std::vector<cgp::vec3> positions;
    for (int k = 0; k < N; ++k) {
        float const x = rand_interval(center[0] - dimension / 2, center[1] + dimension / 2);
        float const y = center[1] + (rand_interval() - 0.5f) * dimension;
        float const z = center[2] + (rand_interval() - 0.5f) * dimension;


        vec3 const p = {x, y, z};

        bool to_add = true;
        for (int k2 = 0; to_add && k2 < positions.size(); ++k2) {
            vec3 const &p2 = positions[k2];
            if (norm(p2 - p) < 0.8f)
                to_add = false;
        }
        if (to_add)
            positions.push_back(p);
    }
    for (cgp::vec3 position: positions) {
        addBoid(position, {01, 0, 0});
    }
    vec3 q1, q2, q3, q4;

    q1 = center + vec3{dimension / 2, 0, 0};
    q2 = center + vec3{dimension / 2, 0, 0};
    q3 = center + vec3{dimension / 2, 0, 0};
    q4 = center + vec3{dimension / 2, 0, 0};
    rectangle_mesh_drawable.initialize(mesh_primitive_cube(center, dimension));
}

void Boids::move_new_positions(float dt) {
    vec3 v1, v2, v3;
    for (Boid &b: boids) {
//        v1 = rule1(b);
//        v2 = rule2(b);
//        v3 = rule3(b);

//        b.velocity = b.velocity + v1 + v2 + v3;
//        b.position = b.position + b.velocity * dt;
    }
}

void Boid::update() {
    this->mesh_drawable.transform.translation = this->position;
}
