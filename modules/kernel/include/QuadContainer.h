/**
 *  \file QuadContainer.h    \brief A container for Quads.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_QUAD_CONTAINER_H
#define IMPKERNEL_QUAD_CONTAINER_H

#include "kernel_config.h"
#include "internal/IndexingIterator.h"
#include "Particle.h"
#include "container_base.h"
#include "VersionInfo.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "ParticleTuple.h"
#include "QuadScore.h"
#include "QuadModifier.h"
#include "QuadDerivativeModifier.h"
#include "macros.h"

IMP_BEGIN_NAMESPACE

class QuadContainer;
typedef std::pair<QuadContainer*,
                  QuadContainer*> QuadContainerPair;

//! A shared container for Quads
/** Stores a searchable shared collection of Quads.
    \ingroup restraints

    \implementationwithoutexample{QuadContainer, IMP_QUAD_CONTAINER}
 */
class IMPEXPORT QuadContainer : public Container
{
 protected:
  QuadContainer(){}
  QuadContainer(Model *m,
                     std::string name="QuadContainer %1%");
#ifndef IMP_DOXYGEN
  template <class S>
    double call_evaluate_index(const S *s,
                         const ParticleIndexQuad& a,
                         DerivativeAccumulator *da) const {
    return s->S::evaluate_index(get_model(), a, da);
  }
  double call_evaluate_index(const QuadScore *s,
                              const ParticleIndexQuad& a,
                              DerivativeAccumulator *da) const {
    return s->evaluate_index(get_model(), a, da);
  }
  template <class S>
    double call_evaluate_if_good_index(const S *s,
                                 const ParticleIndexQuad& a,
                                 DerivativeAccumulator *da,
                                 double max) const {
    return s->S::evaluate_if_good_index(get_model(), a, da, max);
  }
  double call_evaluate_if_good_index(const QuadScore *s,
                                      const ParticleIndexQuad& a,
                                      DerivativeAccumulator *da,
                                      double max) const {
    return s->evaluate_if_good_index(get_model(), a, da, max);
  }
  template <class S>
    void call_apply_index(const S *s,
                    const ParticleIndexQuad& a) const {
    s->S::apply_index(get_model(), a);
  }
  void call_apply(const QuadModifier *s,
                         const ParticleIndexQuad& a) const {
    s->apply_index(get_model(), a);
  }
  template <class S>
    void call_apply_index(const S *s,
                           const ParticleIndexQuad& a,
                           DerivativeAccumulator *&da) const {
    s->S::apply_index(get_model(), a, da);
  }
  void call_apply_index(const QuadDerivativeModifier *s,
                  const ParticleIndexQuad& a,
                  DerivativeAccumulator &da) const {
    s->apply_index(get_model(), a, da);
  }
#endif
public:
  typedef ParticleQuad ContainedType;
  typedef ParticleQuadsTemp ContainedTypes;
  /** \note This function may be linear. Be aware of the complexity
      bounds of your particular container.
   */
  virtual bool get_contains_particle_quad(const ParticleQuad& v) const =0;

  ParticleQuadsTemp get_particle_quads() const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes());
  }
#ifndef IMP_DOXGEN
  //! return the number of Quads in the container
  /** \note this isn't always constant time
   */
  virtual unsigned int get_number_of_particle_quads() const {
    return get_number();
  }

  virtual ParticleQuad get_particle_quad(unsigned int i) const {
    return get(i);
  }

#endif

  //! Apply a SingletonModifier to the contents
  virtual void apply(const QuadModifier *sm)=0;

  //! Apply a SingletonModifier to the contents
  virtual void apply(const QuadDerivativeModifier *sm,
                     DerivativeAccumulator &da)=0;

  //! Evaluate a score on the contents
  virtual double evaluate(const QuadScore *s,
                          DerivativeAccumulator *da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate_if_good(const QuadScore *s,
                                  DerivativeAccumulator *da,
                                  double max) const=0;

  /** Return true if the contents of the container changed since the last
      evaluate.
  */
  virtual bool get_contents_changed() const=0;

#ifndef IMP_DOXYGEN
  typedef ParticleQuad value_type;
  ParticleQuad get(unsigned int i) const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes()[i]);
  }
  ParticleQuadsTemp get() const {
    return IMP::internal::get_particle(get_model(), get_indexes());
  }
  bool get_contains(const ParticleQuad& v) const {
    return get_contains_particle_quad(v);
  }
  unsigned int get_number() const {return get_indexes().size();}
  virtual ParticleIndexQuads get_indexes() const=0;
  virtual ParticleIndexQuads get_all_possible_indexes() const=0;
  virtual Restraints create_decomposition(QuadScore *s) const=0;
#ifndef SWIG
  virtual bool get_provides_access() const {return false;}
  virtual const ParticleIndexQuads& get_access() const {
    IMP_THROW("Object not implemented properly.", IndexException);
  }
#endif
#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(QuadContainer);
};

IMP_OBJECTS(QuadContainer,QuadContainers);

IMP_END_NAMESPACE

#endif  /* IMPKERNEL_QUAD_CONTAINER_H */
