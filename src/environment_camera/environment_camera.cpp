//
// Created by bruno on 05/06/22.
//

#include "environment_camera.hpp"


using namespace cgp;

environment_camera::environment_camera()
{
    background_color = { 1,1,1 };
    projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
}

void opengl_uniform(GLuint shader, environment_camera const& environment)
{
    // Basic uniform parameters
    opengl_uniform(shader, "projection", environment.projection.matrix());
    opengl_uniform(shader, "view", environment.camera.matrix_view());
    opengl_uniform(shader, "light", environment.light);
}