// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "Open3D/Geometry/VoxelGrid.h"
#include "Open3D/Geometry/Octree.h"
#include "Open3D/Geometry/PointCloud.h"
#include "Python/docstring.h"
#include "Python/geometry/geometry.h"
#include "Python/geometry/geometry_trampoline.h"

#include <sstream>

using namespace open3d;

void pybind_voxelgrid(py::module &m) {
    py::class_<geometry::Voxel, std::shared_ptr<geometry::Voxel>> voxel(
            m, "Voxel", "Base Voxel class, containing grid id and color");
    py::detail::bind_default_constructor<geometry::Voxel>(voxel);
    py::detail::bind_copy_functions<geometry::Voxel>(voxel);
    voxel.def("__repr__",
              [](const geometry::Voxel &voxel) {
                  std::ostringstream repr;
                  repr << "geometry::Voxel with grid_index: ("
                       << voxel.grid_index_(0) << ", " << voxel.grid_index_(1)
                       << ", " << voxel.grid_index_(2) << "), color: ("
                       << voxel.color_(0) << ", " << voxel.color_(1) << ", "
                       << voxel.color_(2) << ")";
                  return repr.str();
              })
            .def(py::init([](const Eigen::Vector3i &grid_index) {
                     return new geometry::Voxel(grid_index);
                 }),
                 "grid_index"_a)
            .def(py::init([](const Eigen::Vector3i &grid_index,
                             const Eigen::Vector3d &color) {
                     return new geometry::Voxel(grid_index, color);
                 }),
                 "grid_index"_a, "color"_a)
            .def_readwrite("grid_index", &geometry::Voxel::grid_index_,
                           "Int numpy array of shape (3,): Grid coordinate "
                           "index of the voxel.")
            .def_readwrite(
                    "color", &geometry::Voxel::color_,
                    "Float64 numpy array of shape (3,): Color of the voxel.");

    py::class_<geometry::VoxelGrid, PyGeometry3D<geometry::VoxelGrid>,
               std::shared_ptr<geometry::VoxelGrid>, geometry::Geometry3D>
            voxelgrid(m, "VoxelGrid",
                      "VoxelGrid is a collection of voxels which are aligned "
                      "in grid.");
    py::detail::bind_default_constructor<geometry::VoxelGrid>(voxelgrid);
    py::detail::bind_copy_functions<geometry::VoxelGrid>(voxelgrid);
    voxelgrid
            .def("__repr__",
                 [](const geometry::VoxelGrid &voxelgrid) {
                     return std::string("geometry::VoxelGrid with ") +
                            std::to_string(voxelgrid.voxels_.size()) +
                            " voxels.";
                 })
            .def(py::self + py::self)
            .def(py::self += py::self)
            .def_readwrite("voxels", &geometry::VoxelGrid::voxels_,
                           "List of ``Voxel``: Voxels contained in voxel grid")
            .def("has_colors", &geometry::VoxelGrid::HasColors,
                 "Returns ``True`` if the voxel grid contains voxel colors.")
            .def("has_voxels", &geometry::VoxelGrid::HasVoxels,
                 "Returns ``True`` if the voxel grid contains voxels.")
            .def("get_voxel", &geometry::VoxelGrid::GetVoxel, "point"_a,
                 "Returns voxel index given query point.")
            .def("to_octree", &geometry::VoxelGrid::ToOctree, "max_depth"_a,
                 "Convert to Octree.")
            .def("from_octree", &geometry::VoxelGrid::FromOctree,
                 "octree"_a
                 "Convert from Octree.")
            .def_static("create_from_point_cloud",
                        &geometry::VoxelGrid::CreateFromPointCloud,
                        "Function to make voxels from scanned point cloud",
                        "input"_a, "voxel_size"_a)
            .def_readwrite("origin", &geometry::VoxelGrid::origin_,
                           "``float64`` vector of length 3: Coorindate of the "
                           "origin point.")
            .def_readwrite("voxel_size", &geometry::VoxelGrid::voxel_size_);
    docstring::ClassMethodDocInject(m, "VoxelGrid", "has_colors");
    docstring::ClassMethodDocInject(m, "VoxelGrid", "has_voxels");
    docstring::ClassMethodDocInject(m, "VoxelGrid", "get_voxel",
                                    {{"point", "The query point."}});
    docstring::ClassMethodDocInject(
            m, "VoxelGrid", "to_octree",
            {{"max_depth", "int: Maximum depth of the octree."}});
    docstring::ClassMethodDocInject(
            m, "VoxelGrid", "from_octree",
            {{"octree", "geometry.Octree: The source octree."}});
    docstring::ClassMethodDocInject(
            m, "VoxelGrid", "create_from_point_cloud",
            {{"input", "The input PointCloud"},
             {"voxel_size", "Voxel size of of the VoxelGrid construction."}});
}

void pybind_voxelgrid_methods(py::module &m) {}
