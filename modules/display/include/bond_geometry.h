/**
 *  \file bond_geometry.h
 *  \brief Represent an XYZRDecorator particle with a sphere
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPDISPLAY_BOND_GEOMETRY_H
#define IMPDISPLAY_BOND_GEOMETRY_H

#include "config.h"
#include "macros.h"

#include "internal/version_info.h"
#include <IMP/PairContainer.h>
#include <IMP/core/bond_decorators.h>
#include <IMP/display/geometry.h>

IMPDISPLAY_BEGIN_NAMESPACE

//! Represent a bond with segment
/**
 */
class IMPDISPLAYEXPORT BondGeometry: public Geometry
{
  core::BondDecorator d_;
public:
  //! Get the individual particles from the passed SingletonContainer
  BondGeometry(core::BondDecorator d);

  virtual ~BondGeometry();

  virtual Float get_size() const;

  std::string get_name() const {
    return d_.get_particle()->get_name();
  }
  /* GCC was barfing on the macro, even though doing -E and putting the
     contents in was fine */
  virtual unsigned int get_dimension() const;
  virtual algebra::Vector3D get_vertex(unsigned int i) const;
  virtual VersionInfo get_version_info() const
  {return internal::version_info;}
  virtual void show(std::ostream &out=std::cout) const;

  //IMP_GEOMETRY(internal::version_info)
};

class IMPDISPLAYEXPORT BondGeometryExtractor: public GeometryExtractor {
 public:
  BondGeometryExtractor();
  virtual ~BondGeometryExtractor();
  IMP_GEOMETRY_EXTRACTOR(internal::version_info);
};


IMPDISPLAY_END_NAMESPACE

#endif  /* IMPDISPLAY_BOND_GEOMETRY_H */
