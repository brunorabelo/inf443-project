#pragma once

#include "cgp/cgp.hpp"

/* Terrain modeled with Perlin noise, based on code from a class example.
 * Parameters can be changed interactively by toggling "terrain modeling mode"
 * under gui_parameters in scene.hpp. */

struct terrain_parameters
{
	// Mesh building parameters, these cannot be changed
	int mesh_sampling = 280;
	float length = 100.0f;

	// Perlin noise parameters
	float scale = 0.7f;
	float perlin_offsetx = 0.082f;
	float perlin_offsety = -0.557f;
	float persistency = 0.412f;
	float frequency_gain = 2.304f;
	int octave = 8;
	float terrain_height = 25.052f;

	// Transformation parameters
	float offsetz = -17.885f;
};

/** Compute a terrain mesh 
	The (x,y) coordinates of the terrain are set in [-length/2, length/2].
	The z coordinates of the vertices are computed using evaluate_terrain_height(x,y).
	The vertices are sampled along a regular grid structure in (x,y) directions. 
	The total number of vertices is N*N (N along each direction x/y) 	*/
cgp::mesh create_terrain_mesh(int N, float terrain_length);
cgp::mesh create_terrain_mesh(terrain_parameters const& tparams);

void update_terrain(cgp::mesh& terrain, cgp::mesh_drawable& terrain_visual, terrain_parameters const& tparams);