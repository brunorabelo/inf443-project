//
// Created by bruno on 04/06/22.
//

#include "boids.hpp"

using namespace cgp;

bool Boid::isEqual(Boid b) {
    return this->name.compare(b.name) == 0;
}

Boid::Boid(cgp::vec3 position, cgp::vec3 velocity) {
    this->position = position;
    this->velocity = velocity;
    this->normal = {0, 0, 1};
    this->name = "boid" + std::to_string(count++);
    mesh m = mesh_primitive_cone(0.5f, 1.0f, {0, 0, -0.25}, normal);
    m.push_back(mesh_primitive_frame());
    this->mesh_drawable.initialize(m, name);
}

void Boids::addBoid(vec3 position = {0, 0, 1}, vec3 velocity = {0, 0, 0}) {
    boids_vector.emplace_back(position, velocity);
}

Boids::Boids(float size = 20.0f, cgp::vec3 center = {0, 0, 5.0f}) {
    this->dimension_size = size;
    this->dimension_center = center;
}

void Boids::setup() {
    vec3 center = this->dimension_center;
    int N = 30;
    assert(N > 1);
    std::vector<cgp::vec3> positions;
    for (int k = 0; k < N; ++k) {
        float const x = rand_interval(center[0] - dimension_size / 2, center[1] + dimension_size / 2);
        float const y = center[1] + (rand_interval() - 0.5f) * dimension_size;
        float const z = center[2] + (rand_interval() - 0.5f) * dimension_size;


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
        float const x = (rand_interval() - 0.5f) * 2.0f;
        float const y = (rand_interval() - 0.5f) * 2.0f;
        float const z = (rand_interval() - 0.5f) * 2.0f;
        this->addBoid(position, {x, y, z});
//        this->addBoid(position, {0, 1, 10});
    }
    cube_mesh_drawable.initialize(mesh_primitive_cube(center, dimension_size));

}

vec3 limit_velocity(vec3 velocity) {
    float limit_speed = 2;

    float curr_speed = norm(velocity);
    vec3 new_speed = velocity;
    if (curr_speed > limit_speed) {
        new_speed = (velocity / curr_speed) * limit_speed;
    }
    return new_speed;
}


void Boids::move_new_positions(float dt) {
    vec3 v1, v2, v3;

    for (auto &b: boids_vector) {
        v1 = rule1(b);
        v2 = rule2(b);
        v3 = rule3(b);
        b.velocity += v1 / damping_factor_rule1;
        b.velocity += v2 / damping_factor_rule2;
        b.velocity += v3 / damping_factor_rule3;
        b.velocity = limit_velocity(b.velocity);
//        b.position += b.velocity * dt;
//        vec3 g = vec3{0, 0, -10.0f / 5};
        b.position += b.velocity * dt;
        b.normal = b.velocity / norm(b.velocity);
    }
}

cgp::vec3 Boids::rule1(Boid &boid) {
    // Rule 1: Boids try to fly towards the centre of mass of neighbouring boids_vector.

    vec3 perceived_centre_mass = {0, 0, 0};
    for (auto &it: boids_vector) {
        if (it.isEqual(boid))
            continue;

        perceived_centre_mass += it.position;
    }
    if (boids_vector.size() > 1)
        perceived_centre_mass /= (boids_vector.size() - 1);

    vec3 move_vector = (perceived_centre_mass - boid.position);

    return move_vector;
}

cgp::vec3 Boids::rule2(Boid &boid) {
    // Boids try to keep a small distance away from other objects (including other boids_vector).
    vec3 move_vector = {0, 0, 0};
    for (auto &it: boids_vector) {
        if (boid.isEqual(it))
            continue;
        if (norm(boid.position - it.position) < minimal_distance) {
            move_vector += -it.position + boid.position;
        }
    }
    return move_vector;
}

cgp::vec3 Boids::rule3(Boid &boid) {
    //Boids try to match velocity with near boids.
    vec3 move_vector = {0, 0, 0};

    for (auto &it: boids_vector) {
        if (boid.isEqual(it))
            continue;
        move_vector += it.velocity;
    }
    move_vector = move_vector / (boids_vector.size() - 1);

    move_vector += -boid.velocity;
    return move_vector;
}

void Boid::update() {
    mesh_drawable.transform.rotation = rotation_transform::between_vector({0, 0, 1}, normal);
    this->mesh_drawable.transform.translation = this->position;
}