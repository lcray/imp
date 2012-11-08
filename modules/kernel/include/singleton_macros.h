/**
 *  \file IMP/singleton_macros.h
 *  \brief Macros for various classes.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_SINGLETON_MACROS_H
#define IMPKERNEL_SINGLETON_MACROS_H

#include "internal/TupleRestraint.h"
#include "internal/functors.h"
#include "container_macros.h"
#include "input_output_macros.h"
#include <algorithm>


#define IMP_SINGLETON_SCORE_BASE(Name)                                 \
  IMP_IMPLEMENT_INLINE(double evaluate_indexes(Model *m,                \
                                 const ParticleIndexes &ps, \
                                               DerivativeAccumulator *da) \
                       const, {                                         \
                         IMP::internal::ScoreAccumulator<Name> sa(m,    \
                                                                  this, \
                                                                  da);  \
                         sa(ps);                                        \
                         return sa.get_score();                         \
                       });                                              \
  IMP_IMPLEMENT_INLINE(double                                           \
                       evaluate_if_good_indexes(Model *m,               \
                                 const ParticleIndexes &ps, \
                                                DerivativeAccumulator *da, \
                                                double max) const, {    \
                         IMP::internal::ScoreAccumulatorIfGood<Name> sa(m, \
                                                                        this, \
                                                                        max, \
                                                                        da); \
                         sa(ps);                                        \
                         return sa.get_score();                         \
                       });                                              \
  IMP_OBJECT(Name)



//! Declare the functions needed for a SingletonScore
/** In addition to the methods done by IMP_INTERACTON, it declares
    - IMP::SingletonScore::evaluate(IMP::Particle*,
    IMP::DerivativeAccumulator*)
    - IMP::SingletonScore::get_input_particles()
    - IMP::SingletonScore::get_output_particles()

    See IMP_SIMPLE_SINGLETON_SCORE() for a way of providing an
    implementation of that method.
*/
#define IMP_SINGLETON_SCORE(Name)                                      \
  IMP_IMPLEMENT(double evaluate(Particle* p,\
                                DerivativeAccumulator *da) const);      \
  IMP_IMPLEMENT_INLINE(double evaluate_index(Model *m,                  \
                                ParticleIndex p,           \
                                     DerivativeAccumulator *da) const, { \
    return evaluate(IMP::internal::get_particle(m,p), da);              \
                                    });                                 \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_index(Model *m,          \
                          ParticleIndex p,                       \
                          DerivativeAccumulator *da,                    \
                                                     double max) const, { \
    IMP_UNUSED(max);                                                    \
    return evaluate_index(m, p, da);                                    \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_SINGLETON_SCORE_BASE(Name)

//! Declare the functions needed for a SingletonScore
/** In addition to the methods declared and defined by IMP_SINGLETON_SCORE,
    the macro provides an implementation of
    - IMP::SingletonScore::get_input_particles()
    - IMP::SingletonScore::get_input_containers()
    which assume that only the passed particle serves as input to the
    score.
*/
#define IMP_SIMPLE_SINGLETON_SCORE(Name)                               \
  IMP_IMPLEMENT(double evaluate(Particle* p,    \
                                DerivativeAccumulator *da) const);      \
  IMP_IMPLEMENT_INLINE(double evaluate(Model *m,                        \
                                  ParticleIndex p,  \
                                       DerivativeAccumulator *da) const, { \
    return evaluate(IMP::internal::get_particle(m,p), da);              \
                       });                                              \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_index(Model *m,          \
                          ParticleIndex p,                       \
                          DerivativeAccumulator *da,                    \
                                                     double max) const, { \
    IMP_UNUSED(max);                                                    \
    return evaluate_index(m, p, da);                                    \
                       });                                              \
  IMP_IMPLEMENT_INLINE(ModelObjectsTemp                                 \
                       do_get_inputs(Model *m,                          \
                                     const ParticleIndexes &pis) const, { \
                         ModelObjectsTemp ret;                          \
                         ret+=IMP::get_particles(m, pis);               \
                         return ret;                                    \
                       });                                              \
  IMP_IMPLEMENT_INLINE(Restraints create_current_decomposition          \
  (Particle* vt) const, {                                      \
 return  IMP::internal::create_score_current_decomposition(this, vt); \
                       });                                        \
  IMP_SINGLETON_SCORE_BASE(Name)



//! Declare the functions needed for a complex SingletonScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::SingletonScore::evaluate()
    - IMP::SingletonScore::get_input_particles()
    - IMP::SingletonScore::get_output_particles()
    - IMP::SingletonScore::evaluate_if_good
*/
#define IMP_COMPOSITE_SINGLETON_SCORE(Name)                            \
  IMP_IMPLEMENT_INLINE(double evaluate(Particle* p,     \
                                       DerivativeAccumulator *da) const, { \
    return evaluate_index(IMP::internal::get_model(p),                  \
                  IMP::internal::get_index(p), da);                     \
                       });                                              \
  IMP_IMPLEMENT(double evaluate_index(Model *m, ParticleIndex p,\
                                      DerivativeAccumulator *da) const); \
  IMP_IMPLEMENT(double evaluate_if_good_index(Model *m,                 \
                          ParticleIndex p,                       \
                          DerivativeAccumulator *da,                    \
                                              double max) const);       \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_SINGLETON_SCORE_BASE(Name)

//! Declare the functions needed for a complex SingletonScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::SingletonScore::evaluate()
    - IMP::SingletonScore::get_input_particles()
    - IMP::SingletonScore::get_output_particles()
    - IMP::SingletonScore::evaluate_if_good
*/
#define IMP_INDEX_SINGLETON_SCORE(Name)                                \
  IMP_IMPLEMENT_INLINE(double evaluate(Particle* p,\
                                        DerivativeAccumulator *da) const, { \
    return evaluate_index(IMP::internal::get_model(p),                  \
                  IMP::internal::get_index(p),                          \
                  da);                                                  \
                        });                                             \
  IMP_IMPLEMENT(double evaluate_index(Model *m, ParticleIndex p,\
                                      DerivativeAccumulator *da) const); \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_index(Model *m,         \
                          ParticleIndex p,                      \
                          DerivativeAccumulator *da,                    \
                                                      double max) const, { \
    IMP_UNUSED(max);                                                    \
    return evaluate_index(m, p, da);                                    \
                        });                                             \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_SINGLETON_SCORE_BASE(Name)



//! Declare the functions needed for a SingletonPredicate
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonPredicate::get_value()
    - IMP::SingletonPredicate::get_input_particles()
    - IMP::SingletonPredicate::get_output_particles()
*/
#define IMP_SINGLETON_PREDICATE(Name)                                   \
  IMP_IMPLEMENT(int get_value(Particle* a) const);   \
  IMP_IMPLEMENT_INLINE(Ints get_value(const                             \
                              ParticlesTemp &o) const, {   \
    Ints ret(o.size());                                                 \
    for (unsigned int i=0; i< o.size(); ++i) {                          \
      ret[i]+= Name::get_value(o[i]);                                   \
    }                                                                   \
    return ret;                                                         \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(int get_value_index(Model *m,                    \
                                           ParticleIndex vt)\
                       const, {                                         \
        return Name::get_value(IMP::internal::get_particle(m, vt)); \
                       });                                              \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                     const ParticleIndexes &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_OBJECT(Name)


//! Declare the functions needed for a SingletonPredicate
/** In addition to the methods done by IMP_OBJECT, it defines
    - IMP::SingletonPredicate::get_value_index() based on the return_value
    parameter
    - IMP::SingletonPredicate::do_get_inputs() based on the return_inputs
    parameter
*/
#define IMP_INDEX_SINGLETON_PREDICATE(Name, return_value, return_inputs) \
  IMP_IMPLEMENT_INLINE(int get_value(Particle* a) const, {    \
    return get_value_index(IMP::internal::get_model(a),                 \
                     IMP::internal::get_index(a));                      \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(Ints get_value(const                             \
                                      ParticlesTemp &o) const, {   \
    Ints ret(o.size());                                                 \
    for (unsigned int i=0; i< o.size(); ++i) {                          \
      ret[i]+= Name::get_value(o[i]);                                   \
    }                                                                   \
    return ret;                                                         \
                       })                                               \
  IMP_IMPLEMENT_INLINE(int get_value_index(Model *m,                    \
                                           ParticleIndex pi)\
                       const, {                                         \
                         return_value;                                  \
                       })                                               \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                const ParticleIndexes &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_IMPLEMENT_INLINE_NO_SWIG(void remove_if_equal(Model *m,           \
                                            ParticleIndexes& ps,        \
                                            int value) const, {         \
      ps.erase(std::remove_if(ps.begin(), ps.end(),                     \
                              IMP::internal::PredicateEquals<Name, true>(this, \
                                                              m, value)), \
               ps.end());                                               \
                       });                                              \
  IMP_IMPLEMENT_INLINE_NO_SWIG(void remove_if_not_equal(Model *m,       \
                                            ParticleIndexes& ps,        \
                                            int value) const, {         \
      ps.erase(std::remove_if(ps.begin(), ps.end(),                     \
                          IMP::internal::PredicateEquals<Name, false>(this, \
                                                                 m, value)), \
               ps.end());                                               \
                       });                                              \
  IMP_IMPLEMENT_INLINE(ModelObjectsTemp                                 \
  do_get_inputs(Model *m,                                               \
                const ParticleIndexes &pi) const, {                     \
    return_inputs;                                                      \
                       });                                              \
  IMP_OBJECT_INLINE(Name,IMP_UNUSED(out),)


//! Declare the functions needed for a SingletonModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonModifier::apply(IMP::Particle*)
    - IMP::SingletonModifier::get_input_particles()
    - IMP::SingletonModifier::get_output_particles()
*/
#define IMP_SINGLETON_MODIFIER(Name)                                   \
  IMP_IMPLEMENT(void apply(Particle* a) const); \
  IMP_IMPLEMENT_INLINE(void apply_index(Model *m, \
                                        ParticleIndex a) const, {\
    return Name::apply(IMP::internal::get_particle(m,a));               \
    })                                                                  \
  IMP_BACKWARDS_MACRO_INPUTS;                                                 \
  IMP_BACKWARDS_MACRO_OUTPUTS;                                                \
  IMP_OBJECT(Name)

//! Declare the functions needed for a SingletonModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonDerivativeModifier::apply()
    - IMP::SingletonDerivativeModifier::get_input_particles()
    - IMP::SingletonDerivativeModifier::get_output_particles()
*/
#define IMP_SINGLETON_DERIVATIVE_MODIFIER(Name)                        \
  IMP_IMPLEMENT(void apply(Particle* a,\
                           DerivativeAccumulator&da) const);            \
  IMP_IMPLEMENT_INLINE(void apply_index(Model *m,                       \
                                        ParticleIndex a,  \
                                        DerivativeAccumulator&da) const, { \
    return Name::apply(IMP::internal::get_particle(m,a), da);           \
                       });                                              \
  IMP_IMPLEMENT_INLINE(void apply_indexes(Model *m,\
                                          const ParticleIndexes &ps,    \
                                          DerivativeAccumulator&da) const, { \
                         IMP::internal::ModifierDerivativeApplier<Name>(m, \
                                                                        this,\
                                                                        da) \
                           (ps);                                        \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_BACKWARDS_MACRO_OUTPUTS;                                          \
  IMP_OBJECT(Name)


//! Declare the functions needed for a SingletonModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonModifier::apply(IMP::Particle*)
    - IMP::SingletonModifier::get_input_particles()
    - IMP::SingletonModifier::get_output_particles()
*/
#define IMP_INDEX_SINGLETON_MODIFIER(Name)                     \
  IMP_IMPLEMENT_INLINE(void apply(Particle* a) const, {  \
    apply_index(IMP::internal::get_model(a),                            \
                IMP::internal::get_index(a));                           \
    });                                                                 \
  IMP_IMPLEMENT(void apply_index(Model *m,                              \
                        ParticleIndex a) const);               \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_BACKWARDS_MACRO_OUTPUTS;                                          \
  IMP_OBJECT(Name)

//! Declare the functions needed for a SingletonModifier
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonDerivativeModifier::apply()
    - IMP::SingletonDerivativeModifier::get_input_particles()
    - IMP::SingletonDerivativeModifier::get_output_particles()
*/
#define IMP_INDEX_SINGLETON_DERIVATIVE_MODIFIER(Name)                        \
  IMP_IMPLEMENT_INLINE(void apply(Particle* a,\
                                  DerivativeAccumulator&da) const, {    \
    apply_index(IMP::internal::get_model(a),                            \
                IMP::internal::get_index(a), da);                       \
                       });                                              \
  IMP_IMPLEMENT(void apply_index(Model *m, ParticleIndex a,\
                                 DerivativeAccumulator&da) const);      \
  IMP_IMPLEMENT_INLINE(void apply_indexes(Model *m,\
                                          const ParticleIndexes &ps,    \
                                          DerivativeAccumulator&da) const, { \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      Name::apply_index(m, ps[i], da);                                  \
    }                                                                   \
                       });                                              \
  IMP_BACKWARDS_MACRO_INPUTS;                                           \
  IMP_BACKWARDS_MACRO_OUTPUTS;                                          \
  IMP_OBJECT(Name)




#ifndef IMP_DOXYGEN
#define IMP_IMPLEMENT_SINGLETON_CONTAINER(Name)                        \
  void apply(const SingletonModifier *sm) const {                       \
    for_each(IMP::internal::ModifierApplier<SingletonModifier>          \
             (get_model(), sm));                                        \
  }                                                                     \
  void apply(const SingletonDerivativeModifier *sm,                     \
             DerivativeAccumulator &da) const {                         \
    for_each(IMP::internal                                              \
             ::ModifierDerivativeApplier<SingletonDerivativeModifier>   \
             (get_model(), sm, da));                                    \
  }                                                                     \
  double evaluate(const SingletonScore *s,                              \
                  DerivativeAccumulator *da) const {                    \
    return for_each(IMP::internal::ScoreAccumulator<SingletonScore>     \
                    (get_model(), s, da)).get_score();                  \
  }                                                                     \
  double evaluate_if_good(const SingletonScore *s,                      \
                          DerivativeAccumulator *da, double max) const { \
    return for_each(IMP::internal::ScoreAccumulatorIfGood<SingletonScore> \
                    (get_model(), s, max, da)).get_score();             \
  }                                                                     \
  ParticlesTemp get_all_possible_particles() const;                     \
  IMP_OBJECT(Name)
#endif





//! Declare the needed functions for a SingletonContainer
/** In addition to the methods of IMP_OBJECT, it declares
    - IMP::SingletonContainer::get_number_of_particle_particles()
    - IMP::SingletonContainer::get_particle_particle()
    - IMP::SingletonContainer::apply()
    - IMP::SingletonContainer::evaluate()
    - IMP::Interaction::get_input_objects()

    You need to define a template method with the signature
\code
template <class Functor>
Functor for_each(Functor f);
\endcode
    that applied the functor to each thing in the container.
*/
#define IMP_SINGLETON_CONTAINER(Name)                                  \
  IMP_IMPLEMENT(bool get_is_changed() const);                           \
  IMP_IMPLEMENT(ParticleIndexes get_indexes() const);                   \
  IMP_IMPLEMENT(ParticleIndexes get_range_indexes() const);      \
  IMP_IMPLEMENT(void do_before_evaluate());                             \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles() const);             \
  IMP_IMPLEMENT(ContainersTemp get_input_containers() const);           \
  ModelObjectsTemp do_get_inputs() const {                              \
    ModelObjects ret;                                                   \
    ret+=get_input_containers();                                        \
    ret+=get_input_particles();                                         \
    return ret;                                                         \
  }                                                                     \
  IMP_IMPLEMENT_SINGLETON_CONTAINER(Name)


//! Declare the needed functions for an active SingletonContainer
/** In addition to the methods of IMP_SINGLETON_CONTAINER(), it declares
    - IMP::ScoreState::get_input_particles()
    - IMP::ScoreState::get_input_containers()
    - IMP::ScoreState::do_before_evaluate()
*/
#define IMP_ACTIVE_SINGLETON_CONTAINER(Name)                           \
  IMP_SINGLETON_CONTAINER(name)

/** These macros avoid various inefficiencies.

    The macros take the name of the sequence and the operation to
    peform. The item in the sequence is called _1, it's index is _2.
    Use it like
    \code
    IMP_FOREACH_PARTICLE(sc, std::cout << "Item " << _2
    << " is " << _1->get_name() << std::endl);
    \endcode
*/
#define IMP_FOREACH_SINGLETON(sequence, operation) do {                \
    IMP::ParticlesTemp imp_all=sequence->get();   \
  for (unsigned int _2=0;                                               \
       _2 != imp_all.size();                                            \
       ++_2) {                                                          \
    IMP::Particle* _1= imp_all[_2];               \
    bool imp_foreach_break=false;                                       \
    operation                                                           \
      if (imp_foreach_break) break;                                     \
  }                                                                     \
  } while (false)



/** These macros avoid various inefficiencies.

    The macros take the name of the sequence and the operation to
    peform. The item in the sequence is called _1, it's index is _2.
    Use it like
    \code
    IMP_FOREACH_PARTICLE(sc, std::cout << "Item " << _2
                         << " is _1->get_name() << std::endl);
    \endcode
*/
#define IMP_FOREACH_SINGLETON_INDEX(sequence, operation)               \
  IMP_CONTAINER_FOREACH(SingletonContainer, sequence, operation)

#endif  /* IMPKERNEL_SINGLETON_MACROS_H */
