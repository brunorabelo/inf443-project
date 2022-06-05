//
// Created by bruno on 05/06/22.
//

#ifndef PROJECT_ENVIRONMENT_CAMERA_HPP
#define PROJECT_ENVIRONMENT_CAMERA_HPP

#include "cgp/cgp.hpp"

struct environment_camera {

    // Standard parameters (same as basic environment)
    cgp::vec3 background_color;
    cgp::camera_projection projection;
    cgp::vec3 light;  // stores the light position

    // Change the standard rotating camera to a "head camera" more appropriate to model free-flight (*)
    cgp::camera_spherical_coordinates camera;

    environment_camera();
};


void opengl_uniform(GLuint shader, environment_camera const& environment);
#endif //PROJECT_ENVIRONMENT_CAMERA_HPP
