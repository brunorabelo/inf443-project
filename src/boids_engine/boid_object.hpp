//
// Created by bruno on 06/06/22.
//

#ifndef PROJECT_BOID_OBJECT_HPP
#define PROJECT_BOID_OBJECT_HPP

#pragma once

#include "cgp/cgp.hpp"

class boid_object {
public:

    void setup();

    void animate(float total_time, float speed_z);

    void display(environment_camera environment, bool wireframe);

    void rotate(cgp::rotation_transform rt);

    void translate(cgp::vec3 t);
};


#endif //PROJECT_BOID_OBJECT_HPP
