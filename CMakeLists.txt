#
# Copyright (c) 2016, 2020 CNRS Author: Anna Seppala, Guilhem Saurel
#
# This file is part of hpp-model hpp-model is free software: you can
# redistribute it and/or modify it under the terms of the GNU Lesser General
# Public License as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# hpp-model is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more
# details.  You should have received a copy of the GNU Lesser General Public
# License along with hpp-model  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME hpp-affordance)
set(PROJECT_DESCRIPTION
    "Implementation to Extract Whole-Body Affordances for Mutli-Contact Planning."
)

set(CXX_DISABLE_WERROR TRUE)
set(PROJECT_USE_CMAKE_EXPORT TRUE)

include(cmake/hpp.cmake)

compute_project_args(PROJECT_ARGS LANGUAGES CXX)
project(${PROJECT_NAME} ${PROJECT_ARGS})

add_project_dependency(Eigen3)
add_project_dependency(hpp-fcl)

set(${PROJECT_NAME}_HEADERS
    include/hpp/affordance/fwd.hh
    include/hpp/affordance/affordance-extraction.hh
    include/hpp/affordance/operations.hh)

set(${PROJECT_NAME}_SOURCES src/affordance-extraction.cc)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES}
                                   ${${PROJECT_NAME}_HEADERS})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${PROJECT_NAME} hpp-fcl::hpp-fcl)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${TARGETS_EXPORT_NAME}
  DESTINATION lib)

add_subdirectory(tests)

config_files(include/hpp/affordance/doc.hh)
pkg_config_append_libs("hpp-affordance")

install(FILES package.xml DESTINATION share/${PROJECT_NAME})
