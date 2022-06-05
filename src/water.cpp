#include "water.hpp"

using namespace cgp;

mesh create_water_mesh(float const len) {
	mesh water = mesh_primitive_grid({ -len,-len,0 }, { len,-len,0 }, { len,len,0 }, { -len,len,0 }, 10, 10);
	return water;
}