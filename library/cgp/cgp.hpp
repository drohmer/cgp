#pragma once

/*! \mainpage CGP Code Documentation
 *
 * # Introduction
 *
 * cgp - Visual Computing Library - is a simple lightweight library on top of OpenGL provided to ease learning of 3D programming while avoiding re-coding everything from scratch (matrices, mesh structures, etc.). The library provides several helper structure and functions to set up 3D scene with interactive and/or animated elements.
 *
 * The objective of the library is to be simple as simple as possible to read and use. The majority of cgp structures and functions are aimed to be minimalistic without hidden states. The code remains fully compatible with direct raw OpenGL calls and user-defined shaders. The animation loop itself is entirely defined by the user.
 *
 * 
 *
 */


#include "cgp_parameters.hpp"


#include "01_base/base.hpp"
#include "02_numarray/numarray.hpp"
#include "03_files/files.hpp"
#include "04_grid_container/grid_container.hpp"
#include "05_vec/vec.hpp"
#include "06_mat/mat.hpp"
#include "07_image/image.hpp"
#include "08_random_noise/random_noise.hpp"
#include "09_geometric_transformation/geometric_transformation.hpp"
#include "10_camera_model/camera_model.hpp"
#include "11_mesh/mesh.hpp"
#include "12_shape/shape.hpp"
#include "13_opengl/opengl.hpp"
#include "14_window/window.hpp"
#include "15_input_devices/input_devices.hpp"
#include "16_drawable/drawable.hpp"
#include "17_timer/timer.hpp"
#include "18_user_interactions/user_interactions.hpp"
#include "19_camera_controller/camera_controller.hpp"
#include "20_format_parser/format_parser.hpp"
#include "21_scene_project_helper/scene_project_helper.hpp"

