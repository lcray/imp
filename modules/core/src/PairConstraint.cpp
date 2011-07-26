/**
 *  \file PairConstraint.cpp
 *  \brief Use a PairModifier applied to a PairContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/PairConstraint.h"
#include "IMP/internal/container_helpers.h"

IMPCORE_BEGIN_NAMESPACE

PairConstraint::PairConstraint(PairModifier *before,
                                PairDerivativeModifier *after,
                                         const ParticlePair& vt,
                                         std::string name):
  Constraint(name), v_(IMP::internal::get_index(vt)){
  if (before) f_=before;
  if (after) af_=after;
}


void PairConstraint::do_update_attributes()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin PairsConstraint::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  f_->apply(get_model(), v_);
  IMP_LOG(TERSE, "End PairsConstraint::update" << std::endl);
}

void PairConstraint::do_update_derivatives(DerivativeAccumulator *da)
{
  IMP_OBJECT_LOG;
  if (!af_) return;
  IMP_LOG(TERSE, "Begin PairsConstraint::after_evaluate" << std::endl);
  IMP_CHECK_OBJECT(af_);
  af_->apply(get_model(),v_, *da);
  IMP_LOG(TERSE, "End PairsConstraint::after_evaluate" << std::endl);
}

ContainersTemp PairConstraint::get_input_containers() const {
  return ContainersTemp();
}

ContainersTemp PairConstraint::get_output_containers() const {
  return ContainersTemp();
}


ParticlesTemp PairConstraint::get_input_particles() const {
  ParticlesTemp ret;
  ParticlePair vi= IMP::internal::get_particle(get_model(), v_);
  if (f_) {
    ret= IMP::internal::get_input_particles(f_.get(), vi);
    ParticlesTemp o= IMP::internal::get_output_particles(f_.get(), vi);
    ret.insert(ret.end(), o.begin(), o.end());
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= IMP::internal::get_input_particles(af_.get(), vi);
        std::sort(ret.begin(), ret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(ret.begin(), ret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == ret.size(),
                        "The particles written by "
                        << " the after modifier in " << get_name() << " must "
                        << "be a subset of those read by the before "
                        << "modifier. Before: " << Particles(ret)
                        << " and after " << Particles(oret));
      }
    }
  } else {
    ret=IMP::internal::get_output_particles(af_.get(), vi);
  }
  return ret;
}

ParticlesTemp PairConstraint::get_output_particles() const {
  ParticlesTemp ret;
  ParticlePair vi= IMP::internal::get_particle(get_model(), v_);
  if (f_) {
    ret= IMP::internal::get_output_particles(f_.get(), vi);
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= IMP::internal::get_input_particles(af_.get(), vi);
        ParticlesTemp iret=IMP::internal::get_input_particles(f_.get(), vi);
        iret.insert(iret.end(), ret.begin(), ret.end());
        std::sort(iret.begin(), iret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(iret.begin(), iret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == iret.size(), "The particles read by "
                      << " the after modifier in " << get_name() << " must "
                        << "be a subset of those written by the before"
                        << " modifier.");
      }
    }
  } else {
    ret= IMP::internal::get_input_particles(af_.get(), vi);
  }
  return ret;
}

void PairConstraint::do_show(std::ostream &out) const {
  out << "on " << v_ << std::endl;
  if (f_) out << "before " << *f_ << std::endl;
  if (af_) out << "after " << *af_ << std::endl;
}

IMPCORE_END_NAMESPACE
