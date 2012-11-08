/**
 *  \file TripletModifier.cpp   \brief A function on Particles.
 *
 *  WARNING This file was generated from NAMEDerivativeModifier.cc
 *  in tools/maintenance/container_templates/kernel
 *  by tools/maintenance/make-container.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/TripletDerivativeModifier.h"
#include "IMP/internal/utility.h"

IMP_BEGIN_NAMESPACE

TripletDerivativeModifier::TripletDerivativeModifier(std::string name):
  Object(name){
}

ModelObjectsTemp TripletDerivativeModifier
::get_inputs(Model *m,
             const ParticleIndexes &pis) const {
  return do_get_inputs(m, pis);
}
ModelObjectsTemp TripletDerivativeModifier
::get_outputs(Model *m,
              const ParticleIndexes &pis) const {
  return do_get_outputs(m, pis);
}

#if IMP_USE_DEPRECATED
ParticlesTemp
TripletDerivativeModifier::get_input_particles(Particle* p) const {
  return IMP::get_input_particles(get_inputs(p->get_model(),
                                        ParticleIndexes(1, p->get_index())));
}
ParticlesTemp
TripletDerivativeModifier::get_output_particles(Particle *p) const {
  return IMP::get_output_particles(get_outputs(p->get_model(),
                                        ParticleIndexes(1, p->get_index())));
}
ContainersTemp
TripletDerivativeModifier::get_input_containers(Particle *p) const {
  return IMP::get_input_containers(get_inputs(p->get_model(),
                                         ParticleIndexes(1, p->get_index())));
}
ContainersTemp
TripletDerivativeModifier::get_output_containers(Particle *p) const {
  return IMP::get_output_containers(get_outputs(p->get_model(),
                                          ParticleIndexes(1, p->get_index())));
}
#endif

IMP_END_NAMESPACE
