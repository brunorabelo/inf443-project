//
// Created by bruno on 04/06/22.
//

#include "boids.hpp"
#include "bird.hpp"

using namespace cgp;


void Boids::addBoid(vec3 position = {0, 0, 1}, vec3 velocity = {0, 0, 0}) {

    boids_vector.emplace_back(Boid(position, velocity, 10.0f));
}

Boids::Boids(float size, cgp::vec3 center) {
    this->dimension_size = size;
    this->dimension_center = center;
    this->initial_dimension_size = size;
}

Boids::Boids() : Boids(80.0f, {0, 0, 40.0f}) {};


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
    cube_mesh_drawable.initialize(mesh_primitive_cube(center, initial_dimension_size));

}

vec3 Boids::limit_velocity(vec3 velocity) {
    float curr_speed = norm(velocity);
    vec3 new_speed = velocity;
    if (curr_speed > max_speed) {
        new_speed = (velocity / curr_speed) * max_speed;
    }
    return new_speed;
}


void Boids::update() {
    cube_mesh_drawable.transform.scaling = dimension_size / initial_dimension_size;
}

void Boids::animate(float d, float time) {
    vec3 v1, v2, v3;

    for (auto &b: boids_vector) {
        if (b.perching) {
            if (b.perch_timer > 0) {
                b.perch_timer -= d;
                continue;
            } else {
                b.perching = false;
                b.perch_timer = perch_timer;
            }
        }
        v1 = rule1(b);
        v2 = rule2(b);
        v3 = rule3(b);
        b.velocity += v1 / damping_factor_rule1;
        b.velocity += v2 / damping_factor_rule2;
        b.velocity += v3 / damping_factor_rule3;

        b.velocity = limit_velocity(b.velocity);
        b.velocity = bound_position(b);

        // apply physics
        b.position += b.velocity * d;
        b.normal = b.velocity / norm(b.velocity);

        b.animate(time, b.velocity.z);

    }
}

cgp::vec3 Boids::rule1(Boid &boid) {
    // Rule 1: Boids try to fly towards the centre of mass of neighbouring boids_vector.

    vec3 perceived_centre_mass = {0, 0, 0};
    for (auto &it: boids_vector) {
        if (it.isEqual(boid) || it.perching)
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
        if (boid.isEqual(it) || it.perching)
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
        if (boid.isEqual(it) || it.perching)
            continue;
        move_vector += it.velocity;
    }
    move_vector = move_vector / (boids_vector.size() - 1);

    move_vector += -boid.velocity;
    return move_vector;
}

cgp::vec3 Boids::bound_position(Boid &boid) {

    vec3 new_speed = boid.velocity;

    vec3 min = dimension_center - dimension_size / 2.0f;
    vec3 max = dimension_center + dimension_size / 2.0f;

    if (boid.position.x < min.x)
        new_speed.x += damping_speed;
    else if (boid.position.x > max.x)
        new_speed.x += -damping_speed;
    if (boid.position.y < min.y)
        new_speed.y += damping_speed;
    if (boid.position.y > max.y)
        new_speed.y += -damping_speed;
    if (boid.position.z < min.z)
        new_speed.z += damping_speed;
    if (boid.position.z > max.z)
        new_speed.z += -damping_speed;

    if (boid.position.z < dimension_center.z)
        new_speed.z += cgp::abs(dimension_center.z - boid.position.z) * 0.5f;

    if (boid.position.z < min.z) {
        boid.position.z = min.z;
        boid.perching = true;
    }

    return new_speed;
}

void Boids::display(environment_camera environment, bool wireframe = false, bool cube = false) {
    update();
    for (Boid &boid: boids_vector) {
        boid.update();
        boid.display(environment, wireframe);
    }
    if (cube) {
        draw_wireframe(cube_mesh_drawable, environment);
    }

}

Boid *Boids::get_boid() {
    return &boids_vector[0];
}

