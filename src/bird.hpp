//
// Created by bruno on 04/06/22.
//
#pragma once

#include "cgp/cgp.hpp"

#ifndef PROJECT_BIRD_HPP
#define PROJECT_BIRD_HPP


class Bird {

    cgp::mesh_drawable body;
    cgp::mesh_drawable head;
    cgp::mesh_drawable wing_right;
    cgp::mesh_drawable wing_left;
    cgp::mesh_drawable eye_right;
    cgp::mesh_drawable eye_left;
    cgp::mesh_drawable nose;
    cgp::hierarchy_mesh_drawable hierarchy;

public:
    void setup();

    void display(cgp::scene_environment_basic_camera_spherical_coords environment);

    void display_wireframe(cgp::scene_environment_basic_camera_spherical_coords coords);
};


#endif //PROJECT_BIRD_HPP
