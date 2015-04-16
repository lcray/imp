/**
 *  \file BrownianDynamicsTAMD.cpp  \brief Simple Brownian dynamics optimizer
 *        with TAMD adjustments.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/atom/BrownianDynamicsTAMD.h>
#include <IMP/core/XYZ.h>
#include <IMP/algebra/Vector3D.h>
#include <IMP/algebra/utility.h>
#include <IMP/algebra/vector_generators.h>
#include <IMP/log.h>
#include <IMP/random.h>
#include <IMP/constants.h>
#include <IMP/atom/constants.h>
#include <IMP/warning_macros.h>
#include <IMP/internal/constants.h>
#include <IMP/internal/units.h>
#include <boost/random/normal_distribution.hpp>
#include <IMP/atom/Diffusion.h>
#include <IMP/atom/TAMDCentroid.h>
#include <IMP/atom/TAMDParticle.h>
#include <IMP/Configuration.h>
#include <IMP/algebra/LinearFit.h>
#include <IMP/thread_macros.h>

#include <IMP/core/ConjugateGradients.h>
#include <IMP/core/rigid_bodies.h>
#include <IMP/algebra/vector_generators.h>

#include <cmath>
#include <limits>

IMPATOM_BEGIN_NAMESPACE

// // DEBUG:
// static double Ek = 0.0; // kinetic energy (translational)
// static double M = 0.0; // total mass (estimated by 1/D)
// END DEBUG


namespace {

typedef boost::variate_generator<RandomNumberGenerator &,
                                 boost::normal_distribution<double> > RNG;
}


BrownianDynamicsTAMD::BrownianDynamicsTAMD(Model *m, std::string name,
                                   double wave_factor)
  : BrownianDynamics(m, name, wave_factor)
{}

/*
  radius
  if step is xi*radius^2/(3pi E) for some E=1
  then the motion per step should be sqrt(2kT/(pi E))

  T* is
 */

namespace {
/** get the force dispacement term in the Ermak-Mccammon equation
    for coordinate i of  particle pi in model m, with time step dt and ikT=1/kT
*/
inline double get_force_displacement_bdb(Model *m, ParticleIndex pi,
                        unsigned int i, double dt, double ikT) {
  Diffusion d(m, pi);
  double nforce(-d.get_derivative(i));
  // unit::Angstrom R(sampler_());
  double dd = d.get_diffusion_coefficient();
  if(TAMDParticle::get_is_setup(m, pi)){
    TAMDParticle tamd(m, pi);
    // rescale D = [kT] / [m*gamma] ; T = temperature, gamma = friction
    dd /= tamd.get_friction_scale_factor();
    // // DEBUG: next two lines even out so commented and kept just to
    // //        verify we got it right
    // dd *= tamd.get_temperature_scale_factor();
    // ikT /= tamd.get_temperature_scale_factor();
  }
  double force_term(nforce * dd * dt * ikT);
  /*if (force_term > unit::Angstrom(.5)) {
    std::cout << "Forces on " << _1->get_name() << " are "
    << force << " and " << nforce
    << " and " << force_term <<
    " vs " << dX[j] << ", " << sigma << std::endl;
    }
  std::cout << "Force " << i << " is " << force_term
            << "= " << nforce << "*" << dd << "*" << dt << "*" << ikT
            << std::endl;*/
  return force_term;
}
// radians
inline double get_torque_bdb(Model *m, ParticleIndex pi,
                         unsigned int i, double dt, double ikT) {
  RigidBodyDiffusion d(m, pi);
  core::RigidBody rb(m, pi);

  double cforce(rb.get_torque()[i]);
  // unit::Angstrom R(sampler_());
  double dr = d.get_rotational_diffusion_coefficient();
  // if(TAMDParticle::get_is_setup(m, pi)){
  //   TAMDParticle tamd(m, pi);
  //   dr /= tamd.get_friction_scale_factor();
  //   ikT /= tamd.get_temperature_scale_factor();
  // }
  double force_term = dr * cforce * dt * ikT;
  /*if (force_term > unit::Angstrom(.5)) {
    std::cout << "Forces on " << _1->get_name() << " are "
    << force << " and " << nforce
    << " and " << force_term <<
    " vs " << dX[j] << ", " << sigma << std::endl;
    }*/
  return -force_term;
}

  // returns the std-dev for the random displacement in the Ermak-Mccammon equation
inline double get_sigma_displacement_bdb(Model *m,
                                         ParticleIndex pi,
                                         double dtfs) {
  // TAMD: 6.0 since 2.0 for each dof so that l2 magnitude of sigma
  // is 2.0 * D * dtfs per dof
  // Daniel: 6.0 since we are picking radius rather than the coordinates
  double dd = Diffusion(m, pi).get_diffusion_coefficient();
  if(TAMDParticle::get_is_setup(m, pi)){
    // rescale D = [kT] / [m*gamma] ; T = temperature, gamma = friction
    TAMDParticle tamd(m, pi);
    dd *= tamd.get_temperature_scale_factor();
    dd /= tamd.get_friction_scale_factor();
  }
  return sqrt(6.0 * dd * dtfs);
}
inline double get_rotational_sigma_bdb(Model *m,
                                       ParticleIndex pi,
                                       double dtfs) {
  double dr = RigidBodyDiffusion(m, pi).get_rotational_diffusion_coefficient();
  // if(TAMDParticle::get_is_setup(m, pi)){
  //   TAMDParticle tamd(m, pi);
  //   dr /= tamd.get_friction_scale_factor();
  // }
  return sqrt(6.0 * dr * dtfs);
}
}

IMP_GCC_DISABLE_WARNING(-Wuninitialized)

namespace {
void check_dX_dbd(algebra::Vector3D &dX, double max_step) {
  for (unsigned int j = 0; j < 3; ++j) {
    // if (std::abs(dX[j]) > max_step) {
    /*std::cerr << "Truncating motion: " << dX[j] << " to " << max_step
      << std::endl;*/
    dX[j] = std::min(dX[j], max_step);
    dX[j] = std::max(dX[j], -max_step);
    //}
  }
}
}

void BrownianDynamicsTAMD::advance_coordinates_1(ParticleIndex pi,
                                             unsigned int i, double dt,
                                             double ikT) {
  Diffusion d(get_model(), pi);
  core::XYZ xd(get_model(), pi);
  algebra::Vector3D force(get_force_displacement_bdb(get_model(), pi, 0, dt, ikT),
                          get_force_displacement_bdb(get_model(), pi, 1, dt, ikT),
                          get_force_displacement_bdb(get_model(), pi, 2, dt, ikT));
  algebra::Vector3D dX = (force - get_force(i)) / 2.0;
  check_dX_dbd(dX, get_max_step());
  xd.set_coordinates(xd.get_coordinates() + dX);
}

void BrownianDynamicsTAMD::advance_coordinates_0(ParticleIndex pi,
                                             unsigned int i, double dtfs,
                                             double ikT) {
  Model* m = get_model();
  if(TAMDCentroid::get_is_setup(m, pi)){ // centroids do not move independently
    return; // TODO: a bit wasteful - probably worth it to just let them move
            // and reset in before_evaluate()
  }
  core::XYZ xd(m, pi);
  double sigma = get_sigma_displacement_bdb(get_model(), pi, dtfs);
  boost::normal_distribution<double> nd(0, sigma);
  RNG sampler(random_number_generator, nd);
  double r = sampler();
  algebra::Vector3D random_dX = r * algebra::get_random_vector_on_unit_sphere();
  algebra::Vector3D force_dX
    (get_force_displacement_bdb(get_model(), pi, 0, dtfs, ikT),
     get_force_displacement_bdb(get_model(), pi, 1, dtfs, ikT),
     get_force_displacement_bdb(get_model(), pi, 2, dtfs, ikT));
  if (get_is_srk()) {
    set_force(i, force_dX);
  }
  algebra::Vector3D dX = random_dX + force_dX;
  if (!get_is_srk()) {
    check_dX_dbd(dX, get_max_step());
  }

  // // DEBUG - get kinetic energy
  // algebra::Vector3D v = dX / dtfs;
  // double v2 = v.get_squared_magnitude();
  // Diffusion D(get_model(), pi);
  // // unit::Angstrom R(sampler_());
  // double DD = D.get_diffusion_coefficient();
  // double m = get_kt() / DD;  // for simplicity assume DD = kT / m
  // Ek += 0.5 * m * v2;
  // M += m;
  // std::cout << "Ek for particle " << pi << " is " << 0.5 * m * v2
  //           << std::endl;
  // // DEBUG - end kinetic energy

  xd.set_coordinates(xd.get_coordinates() + dX);
}

void BrownianDynamicsTAMD::advance_orientation_0(ParticleIndex pi,
                                             double dtfs, double ikT) {
  core::RigidBody rb(get_model(), pi);
  double sigma = get_rotational_sigma_bdb(get_model(), pi, dtfs);
  boost::normal_distribution<double> nd(0, sigma);
  RNG sampler(random_number_generator, nd);
  double angle = sampler();
  algebra::Transformation3D nt =
      rb.get_reference_frame().get_transformation_to();
  algebra::Vector3D axis = algebra::get_random_vector_on_unit_sphere();
  algebra::Rotation3D rrot = algebra::get_rotation_about_axis(axis, angle);
  nt = nt * rrot;
  algebra::Vector3D torque(get_torque_bdb(get_model(), pi, 0, dtfs, ikT),
                           get_torque_bdb(get_model(), pi, 1, dtfs, ikT),
                           get_torque_bdb(get_model(), pi, 2, dtfs, ikT));
  double tangle = torque.get_magnitude();
  if (tangle > 0) {
    algebra::Vector3D taxis = torque / tangle;
    algebra::Rotation3D frot = algebra::get_rotation_about_axis(taxis, tangle);
    nt = nt * frot;
  }
  rb.set_reference_frame_lazy(algebra::ReferenceFrame3D(nt));
  IMP_LOG_VERBOSE("Advancing rigid body "
                  << get_model()->get_particle(pi)->get_name() << " to " << nt
                  << std::endl);
}

void BrownianDynamicsTAMD::do_advance_chunk(double dtfs, double ikT,
                                     const ParticleIndexes &ps,
                                     unsigned int begin, unsigned int end) {
  IMP_LOG_TERSE("Advancing particles " << begin << " to " << end << std::endl);
  Model* m = get_model();
  for (unsigned int i = begin; i < end; ++i) {
    ParticleIndex pi = ps[i];
    if (RigidBodyDiffusion::get_is_setup(m, pi)) {
      // std::cout << "rb" << std::endl;
      advance_orientation_0(pi, dtfs, ikT);
    }
#if IMP_HAS_CHECKS >= IMP_INTERNAL
    else  {
      Particle *p = m->get_particle(ps[i]);
      IMP_INTERNAL_CHECK(!core::RigidBody::get_is_setup(p),
                         "A rigid body without rigid body diffusion info"
                         << " was found: " << p->get_name());
      IMP_INTERNAL_CHECK(!core::RigidMember::get_is_setup(p),
                         "A rigid member with diffusion info"
                         << " was found: " << p->get_name());
    }
#endif
    advance_coordinates_0(pi, i, dtfs, ikT);
  }
}



IMPATOM_END_NAMESPACE
