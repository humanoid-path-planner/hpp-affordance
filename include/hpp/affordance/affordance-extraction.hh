//
// Copyright (c) 2016 CNRS
// Authors: Anna Seppala
//
// This file is part of hpp-affordance
// hpp-affordance is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-affordance is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-affordance  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef HPP_AFFORDANCE_AFFORDANCE_EXTRACTION_HH
#define HPP_AFFORDANCE_AFFORDANCE_EXTRACTION_HH

#include <hpp/affordance/config.hh>
#include <hpp/affordance/fwd.hh>

namespace hpp {
namespace affordance {

/// Helper struct that saves the global position of the triangle
/// vertices of a coal::Triangle.
struct TrianglePoints {
  coal::Vec3f p1, p2, p3;
};
/// Helper class to save triangle information.
struct Triangle {
  Triangle() {}
  /// Constructor that takes in a TrianglePoints object.
  Triangle(const TrianglePoints& inPoints) : points(inPoints) {
    TriangleArea(points);
    TriangleNormal(points);
  }
  /// Computes the area of a triangle.
  /// \param tri The global position of a triangles vertices
  void TriangleArea(TrianglePoints& tri) {
    double a, b, c;
    a = (tri.p1 - tri.p2).norm();
    b = (tri.p2 - tri.p3).norm();
    c = (tri.p3 - tri.p1).norm();
    double s = 0.5 * (a + b + c);
    area = sqrt(s * (s - a) * (s - b) * (s - c));
  }
  /// Computes the normal vector of a triangle based on the
  /// global position of its vertices. The normal is subject to convention!
  /// \param tri The global position of a triangles vertices
  void TriangleNormal(TrianglePoints& tri) {
    normal = (tri.p2 - tri.p1).cross(tri.p3 - tri.p1);
    normal.normalize();
  }
  /// The global position of a triangles vertices.
  TrianglePoints points;
  /// The area of a triangle.
  double area;
  /// The normal vector of a triangle.
  coal::Vec3f normal;
};

// helper function to extract mesh model of an coal::collisionObstacle
BVHModelOBConst_Ptr_t GetModel(FclConstCollisionObjectPtr_t object);

/// \addtogroup affordance
/// \{

/// Class that saves a reference collision object and indices to
/// those of its mesh triangles that form one affordance object.
/// This information will later be used to create coal::collisionObjects
/// for each individual affordance.
class Affordance {
 public:
  Affordance() {}
  /// Constructor for an Affordance object
  ///
  /// \param idxVec vector of triangle indices corresponding to
  ///        the mesh model within the reference coal::collisionObject
  /// \param colObj reference to pointer to coal::collisionObject
  ///        containing the found affordance objects.
  Affordance(const std::vector<unsigned int>& idxVec,
             FclConstCollisionObjectPtr_t colObj)
      : indices_(idxVec), colObj_(colObj) {}
  /// Triangle indices that correspond to the triangles of the reference
  /// collisionObstacle, and form the affordance object.
  std::vector<unsigned int> indices_;
  /// Reference to the collisionObstacle the surfaces of which are used to
  /// create the affordance object.
  FclConstCollisionObjectPtr_t colObj_;
};
///	Class containing a vector of vectors of Affordance objects.
/// The length of the outer vector corresponds to the amount of different
/// affordance types found, and that of the inner vector to the amount
/// of affordance objects found for each affordance type.
class SemanticsData {
 public:
  SemanticsData() {}
  /// Vector of vectors of Affordance objects. The size of the outer
  /// vector corresponds to the number of affordance types, and that
  /// of the inner vector to the amount of individual affordance objects
  /// for each affordance type.
  std::vector<std::vector<AffordancePtr_t> > affordances_;

 private:
  SemanticsData(const SemanticsData&);  // Prevent copy-construction
  SemanticsData& operator=(const SemanticsData&);
};

/// Free function that searches through a vector of mesh triangles
/// and saves the triangle indices that form a potential affordance
/// object. Given a reference triangle index that fullfils the affordance
/// requirements of a given affordance type, the function recursively goes
/// all triangles that ara linked to the the reference. Also saves the
/// through total area of found potential affordance.
///
/// \param listPotential reference to the vector of triangle indices
///				 that form one potential affordance object. At
/// every recursive
///        step maximum one triangle is added to the vector.
/// \param refOp operation that determines which affordance type the
/// 			 triangles will be tested for.
/// \param allTris all triangles of a given coal::collisionObject mesh.
///				 This parameter is only used as to verify global
///(affordance type)
/// 			 and local (neighbouring triangles) requirements, and is
/// not
///				 modified within the function.
/// \param searchableTris vector of triangle indices that should be tested
/// 			 in the search for more triangles. Whenever a triangle
/// is
/// found, 			 it is deleted from the vector and will not be
/// tested again.
///				 Similarly, if a triangle is tested once and
/// does not
/// fullfil 				 the global requirement set by the
/// affordance
/// type, it is deleted 			 and will not be tested again in
/// subsequent recursive steps. \param refTriIdx index corresponding to the last
/// found triangle that
///				 fullfils both the local and the global
/// requirement. It
/// is then 				 used as reference in the following
/// recursive step. \param area total area of all triangles that are part of the
/// potential
///				 affordance object. Every time a triangle
/// fulfilling all
/// set 				 requirements is found, its are is added
/// to the previous total 				 before going to the
/// next recursive step.
void searchLinkedTriangles(std::vector<unsigned int>& listPotential,
                           const OperationBasePtr_t& refOp,
                           const std::vector<Triangle>& allTris,
                           std::vector<unsigned int>& searchableTris,
                           const unsigned int& refTriIdx, double& area);

/// Free function that extracts all affordances (of all types) from a given
/// coal::collisionObject.
///
/// \param colObj reference to a coal::collisionObject pointer the triangles
///				 of which will be searched for affordance
/// objects.
/// \param opVec vector of operation objects that determine which requirements
///				 are set for which affordance type. The length
/// of this vector 				 corresponds to the amount of
/// different
/// affordance types considered.
SemanticsDataPtr_t affordanceAnalysis(FclConstCollisionObjectPtr_t colObj,
                                      const OperationBases_t& opVec);

/// Free function that, given a semanticsData pointer, creates one
/// coal::collisionObject for every Affordance object.
///
/// \param sData reference to all found Affordance objects.
std::vector<CollisionObjects_t> getAffordanceObjects(
    const SemanticsDataPtr_t& sData);

/// Free function that, given a semanticsData pointer, creates one
/// coal::collisionObject for every Affordance object.
/// The object are reduced of \param reduceSize : each vertice are moved toward
/// the center of this value
///
/// \param sData reference to all found Affordance objects.
/// NOTE : if there is changes in affordance.impl:: Afford::createOperations (),
/// you should be carefull to the index of the affordance type. The order of the
/// values in reduceSizes correspond to the order of the affordance type in
/// affordance.impl:: Afford::createOperations ()
/// FIXME : not working with plane that contain the z axis
std::vector<CollisionObjects_t> getReducedAffordanceObjects(
    const SemanticsDataPtr_t& sData, std::vector<double> reduceSizes);

/// \}
}  // namespace affordance
}  // namespace hpp

#endif  // HPP_AFFORDANCE_AFFORDANCE_EXTRACTION_HH
