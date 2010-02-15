/**
 *  \file PairsOptimizerState.cpp
 *  \brief Use a PairModifier applied to a PairContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/PairsOptimizerState.h"
#include <utility>

IMPCONTAINER_BEGIN_NAMESPACE

PairsOptimizerState
::PairsOptimizerState(PairContainer *c,
                           PairModifier *gm,
                           std::string name):
  OptimizerState(name),
  c_(c)
{
  f_=gm;
}


void PairsOptimizerState::update()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin PairsOptimizerState::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  IMP_CHECK_OBJECT(c_);
  ParticlePairsTemp set =c_->get_particle_pairs();
  f_->apply(set);

  IMP_LOG(TERSE, "End PairsOptimizerState::update" << std::endl);
}



void PairsOptimizerState::show(std::ostream &out) const {
  out << "PairsOptimizerState base" << std::endl;
}

IMPCONTAINER_END_NAMESPACE
