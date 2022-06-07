//
// Created by bruno on 06/06/22.
//

#ifndef PROJECT_BOID_HPP
#define PROJECT_BOID_HPP

#include "cgp/cgp.hpp"
#include "environment_camera/environment_camera.hpp"
#include "bird.hpp"

static int count = 0;

class Boid {

public:
    Bird object;
    bool perching = false;
    float perch_timer;

    cgp::vec3 position; // position
    cgp::vec3 velocity; // velocity
    cgp::vec3 normal;
    std::string name;


    Boid(cgp::vec3 position, cgp::vec3 velocity, float perch_timer);

    void update();

    bool isEqual(Boid b);

    void display(environment_camera environment, bool wireframe);

    void animate(float total_time, float speed_z);
};

#endif //PROJECT_BOID_HPP
