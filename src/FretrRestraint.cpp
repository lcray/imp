/**
 *  \file FretrRestraint.cpp \brief FRET_R restraint
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/membrane/FretrRestraint.h>
#include <IMP/container/ListSingletonContainer.h>
#include <IMP/container/CloseBipartitePairContainer.h>
#include <IMP/core/XYZ.h>
#include <IMP/Particle.h>

IMPMEMBRANE_BEGIN_NAMESPACE

FretrRestraint::FretrRestraint(Particles pd, Particles pa,
                               double R0, double Sd, double Sa,
                               double gamma, double Ida,
                               double fretr, double kappa,
                               std::string name):
  Restraint("FRET_R Restraint " + name) {

 pd_ = pd;
 pa_ = pa;
 R0_ = R0;
 Sd_ = Sd;
 Sa_ = Sa;
 gamma_ = gamma;
 Ida_ = Ida;
 fretr_ = fretr;
 kappa_ = kappa;
}

double
FretrRestraint::unprotected_evaluate(DerivativeAccumulator *da) const
{
// check if derivatives are requested
 IMP_USAGE_CHECK(!da, "Derivatives not available");

 double sumFi=0.0;

 for(unsigned i=0;i<pd_.size();++i){
  double Fi=0.0;
  for(unsigned j=0;j<pa_.size();++j){
   double R=core::get_distance(core::XYZ(pd_[i]),core::XYZ(pa_[j]));
   Fi+=pow(R0_/R,6);
  }
  sumFi+=1.0/(1.0+Fi);
 }

 double fretr = 1.0 + gamma_ * ( (double)(pd_.size()) - sumFi ) /
                ( Sd_ * Ida_ * sumFi + Sa_ * (double)(pa_.size()) );

 double score = 0.5 * kappa_ * pow( fretr - fretr_ , 2);

 return score;
}

ParticlesTemp FretrRestraint::get_input_particles() const {
  ParticlesTemp ret;
  ret.insert(ret.end(), pd_.begin(), pd_.end());
  ret.insert(ret.end(), pa_.begin(), pa_.end());
  return ret;
}

ContainersTemp FretrRestraint::get_input_containers() const {
  return ContainersTemp();
}

void FretrRestraint::do_show(std::ostream &) const
{
}

IMPMEMBRANE_END_NAMESPACE
