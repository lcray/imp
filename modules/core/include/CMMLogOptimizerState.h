/**
 *  \file CMMLogOptimizerState.h
 *  \brief A state which writes a series of CMM files.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPCORE_CMM_LOG_OPTIMIZER_STATE_H
#define IMPCORE_CMM_LOG_OPTIMIZER_STATE_H

#include "config.h"
#include "internal/version_info.h"

#include <IMP/base_types.h>
#include <IMP/OptimizerState.h>

#include <iostream>

IMPCORE_BEGIN_NAMESPACE

//! A state that writes a series of CMM files.
/** The State writes a series of files generated from a
    printf-style format string. These files contain spheres
    and points for a set of particles. The particles must have
    x,y,z coordinates (as defined by XYZDecorator) and
    optionally have a radius.

    Documentation for the CMM file format can be found at
    http://www.cgl.ucsf.edu/chimera/docs/ContributedSoftware/volumepathtracer/volumepathtracer.html#markerfiles

    \deprecated Use display::LogOptimizerState instead
 */
class IMPCOREEXPORT CMMLogOptimizerState : public OptimizerState
{
 public:
  CMMLogOptimizerState(std::string filename, const Particles &pis=Particles());
  virtual ~CMMLogOptimizerState() {}

  IMP_OPTIMIZER_STATE(internal::version_info)

  //! Set the number of update calls to skip between writing files
  /** The first update call always writes a file.
   */
  void set_skip_steps(unsigned int i) {
    skip_steps_=i;
  }

  //! The float key to use for the radius
  /** Particles without such an attribute are drawn as fixed sized markers.
   */
  void set_radius(FloatKey k) {
    radius_=k;
  }
  //! The three color components
  /** Color values should be between 0 and 255. They will be snapped if needed.
   */
  void set_color(IntKey r, IntKey g, IntKey b) {
    r_=r; g_=g; b_=b;
  }
  void set_name(StringKey name) {
    name_=name;
  }
  //! Set the particles to use.
  void set_particles(const Particles &pis) {
    pis_=pis;
  }

  //! force it to write the next file
  void write_next_file();

  void write(std::string name) const;

  //! A helper function to just write a list of particles to a file
  static void write(const Particles &pis, const std::string &marker_set_name,
                    FloatKey radius_key, IntKey r_key, IntKey g_key,
                    IntKey b_key, StringKey name_key, std::ostream &out);

protected:
  Particles pis_;
  std::string filename_;
  int file_number_;
  int call_number_;
  int skip_steps_;
  FloatKey radius_;
  IntKey r_, g_, b_;
  StringKey name_;
};

IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_CMM_LOG_OPTIMIZER_STATE_H */
