#
# Copyright (c) 2016, 2020 CNRS
# Author: Anna Seppala, Guilhem Saurel
#
#
# This file is part of hpp-model
# hpp-model is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-model is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-model  If not, see
# <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT_NAME hpp-affordance)
SET(PROJECT_DESCRIPTION "Implementation to Extract Whole-Body Affordances for Mutli-Contact Planning.")

SET(CXX_DISABLE_WERROR TRUE)
SET(PROJECT_USE_CMAKE_EXPORT TRUE)

INCLUDE(cmake/hpp.cmake)

COMPUTE_PROJECT_ARGS(PROJECT_ARGS LANGUAGES CXX)
PROJECT(${PROJECT_NAME} ${PROJECT_ARGS})

ADD_PROJECT_DEPENDENCY(Eigen3)
ADD_PROJECT_DEPENDENCY(hpp-fcl)

SET(${PROJECT_NAME}_HEADERS
  include/hpp/affordance/fwd.hh
  include/hpp/affordance/affordance-extraction.hh
  include/hpp/affordance/operations.hh
  )

SET(${PROJECT_NAME}_SOURCES
  src/affordance-extraction.cc
  )

ADD_LIBRARY(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} hpp-fcl::hpp-fcl)

INSTALL(TARGETS ${PROJECT_NAME} EXPORT ${TARGETS_EXPORT_NAME} DESTINATION lib)

ADD_SUBDIRECTORY(tests)

CONFIG_FILES(include/hpp/affordance/doc.hh)
PKG_CONFIG_APPEND_LIBS("hpp-affordance")

INSTALL(FILES package.xml DESTINATION share/${PROJECT_NAME})
