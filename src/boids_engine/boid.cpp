//
// Created by bruno on 06/06/22.
//

#include "boid.hpp"

using namespace cgp;

Boid::Boid(cgp::vec3 position, cgp::vec3 velocity, float perch_timer = 10.0f) {
    this->position = position;
    this->velocity = velocity;
    this->normal = {0, 0, 1};
    this->name = "boid" + std::to_string(count++);
    this->perch_timer = perch_timer;
    object.setup();
}


void Boid::update() {
    object.rotate(rotation_transform::between_vector({0, 0, 1}, normal));
    object.translate(this->position);
}

void Boid::display(environment_camera environment, bool wireframe) {
    object.display(environment, wireframe);
}

bool Boid::isEqual(Boid b) {
    return this->name.compare(b.name) == 0;
}

void Boid::animate(float total_time, float speed_z) {
    object.animate(total_time, speed_z);
}
