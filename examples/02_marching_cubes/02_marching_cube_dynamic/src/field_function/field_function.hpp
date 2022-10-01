#pragma once

#include "cgp/cgp.hpp"

// Compute a grid filled with the value of some scalar function - the size of the grid is given by the domain
cgp::grid_3D<float> compute_scalar_field(cgp::spatial_domain_grid_3D const& domain);