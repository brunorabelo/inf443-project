
#include "terrain.hpp"


using namespace cgp;

mesh create_terrain_mesh(int N, float len)
{
    mesh terrain = mesh_primitive_grid({ -len,-len,0 }, { len,-len,0 }, { len,len,0 }, { -len,len,0 }, N, N);
    return terrain;
}

mesh create_terrain_mesh(terrain_parameters const& tparams) {
    return create_terrain_mesh(tparams.mesh_sampling, tparams.length);
}

void update_terrain(mesh& terrain, mesh_drawable& terrain_visual, terrain_parameters const& tparams) {
    int const N = tparams.mesh_sampling;

    // Recompute the new vertices
    for (int ku = 0; ku < N; ++ku) {
        for (int kv = 0; kv < N; ++kv) {

            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            int const idx = ku * N + kv;

            // Compute the Perlin noise
            float const noise = noise_perlin({ u/tparams.scale + tparams.perlin_offsetx,
                v/tparams.scale + tparams.perlin_offsety},
                tparams.octave, tparams.persistency, tparams.frequency_gain);

            // use the noise as height value
            terrain.position[idx].z = tparams.terrain_height * noise + tparams.offsetz;

            // use also the noise as color value
            terrain.color[idx] = 0.3f * vec3(0, 0.5f, 0) + 0.7f * noise * vec3(1, 1, 1);
        }
    }

    // Update the normal of the mesh structure
    terrain.compute_normal();

    // Update step: Allows to update a mesh_drawable without creating a new one
    terrain_visual.update_position(terrain.position);
    terrain_visual.update_normal(terrain.normal);
    terrain_visual.update_color(terrain.color);

}