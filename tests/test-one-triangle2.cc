// Copyright (C) 2016 LAAS-CNRS
// Author: Anna Seppala
//
// This file is part of the hpp-affordance.
//
// hpp-affordance is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hpp-affordance is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with hpp-affordance.  If not, see <http://www.gnu.org/licenses/>.

#include <coal/BVH/BVH_model.h>
#include <coal/shape/geometric_shape_to_BVH_model.h>

#include <hpp/affordance/affordance-extraction.hh>
#include <hpp/affordance/operations.hh>

#define BOOST_TEST_MODULE test - one - triangle2
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_affordance)

using namespace hpp;
BOOST_AUTO_TEST_CASE(one_triangle2) {
  hpp::affordance::SupportOperationPtr_t support(
      new hpp::affordance::SupportOperation());
  hpp::affordance::LeanOperationPtr_t lean(
      new hpp::affordance::LeanOperation(0.1));

  std::vector<hpp::affordance::OperationBasePtr_t> operations;
  operations.push_back(support);
  operations.push_back(lean);

  std::vector<coal::Vec3f> vertices;
  std::vector<coal::Triangle> triangles;

  typedef coal::BVHModel<coal::OBBRSS> Model;
  coal::shared_ptr<Model> model(new Model());

  coal::Vec3f vert1(-1, 0, 0);
  coal::Vec3f vert2(1, 0, 1);
  coal::Vec3f vert3(1, 0, -1);
  vertices.push_back(vert1);
  vertices.push_back(vert2);
  vertices.push_back(vert3);

  coal::Triangle tri(0, 1, 2);
  triangles.push_back(tri);

  coal::Matrix3f R;
  coal::Quaternion3f quat(1, 0, 0, 0);
  R = quat.matrix();
  coal::Vec3f T(0, 0, 0);

  coal::Transform3s pose(R, T);

  model->beginModel();
  model->addSubModel(vertices, triangles);
  model->endModel();

  coal::CollisionObject* obj(new coal::CollisionObject(model, pose));

  hpp::affordance::SemanticsDataPtr_t h =
      hpp::affordance::affordanceAnalysis(obj, operations);

  BOOST_CHECK_MESSAGE(h->affordances_.size() == 2,
                      "strictly two affordance types should exist.");

  BOOST_CHECK_MESSAGE(h->affordances_[1].size() == 1,
                      "strictly one lean affordance should have been found.");

  BOOST_CHECK_MESSAGE(
      h->affordances_[0].size() == 0,
      "no support affordance should have been found at this point.");
}
BOOST_AUTO_TEST_SUITE_END()
