/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkHexahedronCell_h
#define itkHexahedronCell_h

#include "itkQuadrilateralCell.h"
#include "itkHexahedronCellTopology.h"

namespace itk
{
/** \class HexahedronCell
 *  \brief Represents a hexahedron for a Mesh.
 *
 * HexahedronCell represents a hexahedron for a Mesh.
 *
 * \tparam TPixelType The type associated with a point, cell, or boundary
 * for use in storing its data.
 *
 * \tparam TCellTraits Type information of mesh containing cell.
 *
 * \todo When reviewing this class, the documentation of the  template
 * parameters MUST be fixed.
 *
 * \ingroup MeshObjects
 * \ingroup ITKCommon
 */

template< typename TCellInterface >
class ITK_TEMPLATE_EXPORT HexahedronCell:public TCellInterface, private HexahedronCellTopology
{
public:
  /** Standard class type aliases. */
  itkCellCommonTypedefs(HexahedronCell);
  itkCellInheritedTypedefs(TCellInterface);

  /** Standard part of every itk Object. */
  itkTypeMacro(HexahedronCell, CellInterface);

  /** The type of boundary for this triangle's vertices. */
  using VertexType = VertexCell< TCellInterface >;
  using VertexAutoPointer = typename VertexType::SelfAutoPointer;

  /** The type of boundary for this triangle's edges. */
  using EdgeType = LineCell< TCellInterface >;
  using EdgeAutoPointer = typename EdgeType::SelfAutoPointer;

  /** The type of boundary for this hexahedron's faces. */
  using FaceType = QuadrilateralCell< TCellInterface >;
  using FaceAutoPointer = typename FaceType::SelfAutoPointer;

  /** Hexahedron-specific topology numbers. */
  itkStaticConstMacro(NumberOfPoints, unsigned int, 8);
  itkStaticConstMacro(NumberOfVertices, unsigned int, 8);
  itkStaticConstMacro(NumberOfEdges, unsigned int, 12);
  itkStaticConstMacro(NumberOfFaces, unsigned int, 6);
  itkStaticConstMacro(CellDimension, unsigned int, 3);

  /** Implement the standard CellInterface. */
  CellGeometry GetType(void) const override
  { return Superclass::HEXAHEDRON_CELL; }
  void MakeCopy(CellAutoPointer &) const override;

  unsigned int GetDimension(void) const override;

  unsigned int GetNumberOfPoints(void) const override;

  CellFeatureCount GetNumberOfBoundaryFeatures(int dimension) const override;

  bool GetBoundaryFeature(int dimension, CellFeatureIdentifier, CellAutoPointer &) override;
  void SetPointIds(PointIdConstIterator first) override;

  void SetPointIds(PointIdConstIterator first, PointIdConstIterator last) override;

  void SetPointId(int localId, PointIdentifier) override;
  PointIdIterator      PointIdsBegin(void) override;

  PointIdConstIterator PointIdsBegin(void) const override;

  PointIdIterator      PointIdsEnd(void) override;

  PointIdConstIterator PointIdsEnd(void) const override;

  /** Hexahedron-specific interface. */
  virtual CellFeatureCount GetNumberOfVertices() const;

  virtual CellFeatureCount GetNumberOfEdges() const;

  virtual CellFeatureCount GetNumberOfFaces() const;

  virtual bool GetVertex(CellFeatureIdentifier, VertexAutoPointer &);
  virtual bool GetEdge(CellFeatureIdentifier, EdgeAutoPointer &);
  virtual bool GetFace(CellFeatureIdentifier, FaceAutoPointer &);

  /** Evaluate the position inside the cell */
  bool EvaluatePosition(CoordRepType *,
                                PointsContainer *,
                                CoordRepType *,
                                CoordRepType[],
                                double *,
                                InterpolationWeightType *) override;

  /** Visitor interface */
  itkCellVisitMacro(Superclass::HEXAHEDRON_CELL);

protected:
  /** Store the number of points needed for a hexahedron. */
  PointIdentifier m_PointIds[NumberOfPoints];

  void InterpolationDerivs(CoordRepType pcoords[3], CoordRepType derivs[24]);
  void InterpolationFunctions(CoordRepType pcoords[3], InterpolationWeightType sf[8]);
  void EvaluateLocation(int &itkNotUsed(subId), PointsContainer * points, CoordRepType pcoords[3],
                        CoordRepType x[3], InterpolationWeightType * weights);

public:
  HexahedronCell()
  {
    for ( unsigned int i = 0; i < itkGetStaticConstMacro(NumberOfPoints); i++ )
      {
      m_PointIds[i] = NumericTraits< PointIdentifier >::max();
      }
  }

  ~HexahedronCell() override {}

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(HexahedronCell);
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHexahedronCell.hxx"
#endif

#endif
