/**
 *  \file SingletonScore.h    \brief Define SingletonScore.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMP_SINGLETON_SCORE_H
#define IMP_SINGLETON_SCORE_H

#include "kernel_config.h"
#include "base_types.h"
#include "ParticleTuple.h"
#include "DerivativeAccumulator.h"
#include "internal/container_helpers.h"

IMP_BEGIN_NAMESPACE

//! Abstract score function
/** SingletonScores will evaluate the score and derivatives
    for the passed particles. Use in conjunction with various
    restraints such as IMP::core::SingletonsRestraint or
    IMP::core::SingletonRestraint.

    Implementers should check out IMP_SINGLETON_SCORE().
*/
class IMPEXPORT SingletonScore : public Object
{
 public:
  typedef Particle* Argument;
  SingletonScore(std::string name="SingletonScore %1%");
  //! Compute the score and the derivative if needed.
  virtual double evaluate(Particle* vt,
                          DerivativeAccumulator *da) const =0;

  /** Implementations
      for these are provided by the IMP_SINGLETON_SCORE()
      macro.
  */
  virtual double evaluate(const ParticlesTemp &o,
                          DerivativeAccumulator *da) const {
    double ret=0;
    for (unsigned int i=0; i< o.size(); ++i) {
      ret+= evaluate(o[i], da);
    }
    return ret;
  }



  //! Compute the score and the derivative if needed.
  virtual double evaluate(Model *m, ParticleIndex vt,
                          DerivativeAccumulator *da) const {
    return evaluate(internal::get_particle(m, vt), da);
  }

  /** Implementations
      for these are provided by the IMP_SINGLETON_SCORE()
      macro.
  */
  virtual double evaluate(Model *m,
                          const ParticleIndexes &o,
                          DerivativeAccumulator *da) const {
    double ret=0;
    for (unsigned int i=0; i< o.size(); ++i) {
      ret+= evaluate(m, o[i], da);
    }
    return ret;
  }


  //! Compute the score and the derivative if needed.
  virtual double evaluate_if_good(Model *m, ParticleIndex vt,
                                  DerivativeAccumulator *da,
                                  double max) const {
    IMP_UNUSED(max);
    return evaluate(m, vt, da);
  }

  /** Implementations
      for these are provided by the IMP_SINGLETON_SCORE()
      macro.
  */
  virtual double evaluate_if_good(Model *m,
                                  const ParticleIndexes &o,
                                  DerivativeAccumulator *da,
                                  double max) const {
    double ret=0;
    for (unsigned int i=0; i< o.size(); ++i) {
      double cur= evaluate(m, o[i], da);
      max-=cur;
      ret+=cur;
      if (max<0) break;
    }
    return ret;
  }

  /** Get the set of particles read when applied to the arguments. */
  virtual ParticlesTemp
    get_input_particles(Particle *p) const =0;

  /** Get the set of input containers when this modifier is applied to
      the arguments. */
  virtual ContainersTemp
    get_input_containers(Particle *p) const =0;

  /** Decompose this pair score acting on the pair into a set of
      restraints. The scoring function and derivatives should
      be equal to the current score. The defualt implementation
      just returns this object bound to the pair.*/
  Restraints get_instant_decomposition(Particle* vt) const;

  IMP_REF_COUNTED_DESTRUCTOR(SingletonScore);
};

IMP_OBJECTS(SingletonScore,SingletonScores);


/** A SingletonScoreRestraint is a restraint where the score (and
    derivative values) can be decomposed into an application
    of a SingletonScore onto a Particle*.
*/
class IMPEXPORT SingletonScoreRestraint: public Restraint {
public:
  SingletonScoreRestraint(std::string name);
  virtual SingletonScore *get_score() const =0;
  virtual Particle* get_argument() const=0;
};

IMP_OBJECTS(SingletonScoreRestraint, SingletonScoreRestraints);

/** A SingletonsScoreRestraint is a restraint where the score (and
    derivative values) can be decomposed into a series of applications
    of a SingletonScore onto a Particle*.
*/
class IMPEXPORT SingletonsScoreRestraint: public Restraint {
public:
  SingletonsScoreRestraint(std::string name);
  virtual SingletonScore *get_score() const =0;
  virtual ParticlesTemp get_arguments() const=0;
};

IMP_OBJECTS(SingletonsScoreRestraint, SingletonsScoreRestraints);

IMP_END_NAMESPACE

#endif  /* IMP_SINGLETON_SCORE_H */
