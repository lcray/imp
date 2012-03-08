/**
 *  \file singleton_macros.h    \brief Macros for various classes.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_SINGLETON_MACROS_H
#define IMPKERNEL_SINGLETON_MACROS_H

#define IMP_SINGLETON_SCORE_BASE(Name)                                 \
  IMP_IMPLEMENT_INLINE(double evaluate(const ParticlesTemp &ps,    \
                                       DerivativeAccumulator *da) const, { \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      ret+=Name::evaluate(ps[i], da);                                   \
    }                                                                   \
    return ret;                                                         \
                       });                                              \
  IMP_IMPLEMENT_INLINE(double evaluate_indexes(Model *m,                \
                                              const ParticleIndexes &ps, \
                                              DerivativeAccumulator *da)\
    const, {                                                            \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      ret+=Name::evaluate_index(m, ps[i], da);                          \
    }                                                                   \
    return ret;                                                         \
                       });                                              \
  IMP_IMPLEMENT_INLINE(double evaluate_if_good_indexes(Model *m, \
                                     const ParticleIndexes &ps, \
                          DerivativeAccumulator *da,                    \
                                                      double max) const, { \
    double ret=0;                                                       \
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      double cur=Name::evaluate_if_good_index(m, ps[i], da, max);       \
      max-=cur;                                                         \
      ret+=cur;                                                         \
      if (max <0) break;                                                \
    }                                                                   \
    return ret;                                                         \
                      });                                               \
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
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*p) const) ;  \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle *) const) ; \
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
  IMP_IMPLEMENT_INLINE(ParticlesTemp get_input_particles(Particle*p) const, { \
    return ParticlesTemp(1,p);                                          \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(ContainersTemp get_input_containers(Particle *) const, \
  {                                                                     \
    return ContainersTemp();                                            \
  });                                                                   \
  IMP_IMPLEMENT_INLINE(Restraints create_current_decomposition\
  (Particle* vt) const, {                                      \
    return Restraints(1, create_restraint(this, vt));             \
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
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle *p) const);  \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle *p) const); \
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
  IMP_SINGLETON_SCORE_BASE(Name)

//! Declare the functions needed for a complex SingletonScore
/** In addition to the methods done by IMP_OBJECT(), it declares
    - IMP::SingletonScore::evaluate()
    - IMP::SingletonScore::get_input_particles()
    - IMP::SingletonScore::get_output_particles()
    - IMP::SingletonScore::evaluate_if_good
*/
#define IMP_INDEX_SINGLETON_SCORE(Name)                                \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle *p) const);  \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle *p) const); \
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
    return Name::get_value(internal::get_particle(m, vt));              \
                       });                                              \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                     const ParticleIndexes &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*) const);    \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle*) const);  \
  IMP_OBJECT(Name)


//! Declare the functions needed for a SingletonPredicate
/** In addition to the methods done by IMP_OBJECT, it declares
    - IMP::SingletonPredicate::get_value_index()
    - IMP::SingletonPredicate::get_input_particles()
    - IMP::SingletonPredicate::get_output_particles()
*/
#define IMP_INDEX_SINGLETON_PREDICATE(Name, gv)                        \
  IMP_IMPLEMENT_INLINE(int get_value(Particle* a) const, { \
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
    gv;                                                                 \
                       })                                               \
  IMP_IMPLEMENT_INLINE(Ints get_value_index(Model *m,                   \
                                const ParticleIndexes &o) const, { \
   Ints ret(o.size());                                                  \
   for (unsigned int i=0; i< o.size(); ++i) {                           \
     ret[i]+= Name::get_value_index(m, o[i]);                           \
   }                                                                    \
   return ret;                                                          \
                       });                                              \
  IMP_IMPLEMENT_INLINE(ParticlesTemp get_input_particles(Particle*p) const, { \
   return ParticlesTemp(1, p);                                          \
    });                                                                 \
  IMP_IMPLEMENT_INLINE(ContainersTemp get_input_containers(Particle*) const, { \
   return ContainersTemp();                                             \
    });                                                                 \
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
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*) const);    \
  IMP_IMPLEMENT(ParticlesTemp get_output_particles(Particle*) const);   \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle*) const);  \
  IMP_IMPLEMENT(ContainersTemp get_output_containers(Particle*) const); \
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
    for (unsigned int i=0; i< ps.size(); ++i) {                         \
      Name::apply_index(m, ps[i], da);                                  \
    }                                                                   \
                       });                                              \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*) const);    \
  IMP_IMPLEMENT(ParticlesTemp get_output_particles(Particle*) const);   \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle*) const);  \
  IMP_IMPLEMENT(ContainersTemp get_output_containers(Particle*) const); \
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
  IMP_IMPLEMENT(void apply_index(Model *m,\
                                 ParticleIndex a) const);   \
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*) const);    \
  IMP_IMPLEMENT(ParticlesTemp get_output_particles(Particle*) const);   \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle*) const);  \
  IMP_IMPLEMENT(ContainersTemp get_output_containers(Particle*) const); \
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
  IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle*) const);    \
  IMP_IMPLEMENT(ParticlesTemp get_output_particles(Particle*) const);   \
  IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle*) const);  \
  IMP_IMPLEMENT(ContainersTemp get_output_containers(Particle*) const); \
  IMP_OBJECT(Name)




#ifndef IMP_DOXYGEN
#define IMP_IMPLEMENT_SINGLETON_CONTAINER(Name)                        \
  void apply(const SingletonModifier *sm) const {                       \
    template_apply(sm);                                                 \
  }                                                                     \
  void apply(const SingletonDerivativeModifier *sm,                     \
             DerivativeAccumulator &da) const {                         \
    template_apply(sm, da);                                             \
  }                                                                     \
  double evaluate(const SingletonScore *s,                              \
                  DerivativeAccumulator *da) const {                    \
    return template_evaluate(s, da);                                    \
  }                                                                     \
  double evaluate_if_good(const SingletonScore *s,                      \
                          DerivativeAccumulator *da, double max) const { \
    return template_evaluate_if_good(s, da, max);                       \
  }                                                                     \
  ParticlesTemp get_contained_particles() const;                        \
  IMP_OBJECT(Name)
#endif



/** Implement the needed template functions for a container.
    \param[in] Name the name
    \param[in] LOOP do the loop. There should be a line IMP_OPERATION and
    the current item should be in a variable named item at that time.
 */
#define IMP_IMPLEMENT_SINGLETON_CONTAINER_OPERATIONS(Name, LOOP)       \
  public:                                                               \
  IMP_IMPLEMENTATION_TEMPLATE_1(class SM,                               \
  void template_apply(const SM *sm,                                     \
                      DerivativeAccumulator &da) const, {               \
                       LOOP(IMP::internal::call_apply_index(get_model(),\
                                                        sm, item, da)); \
                                });                                     \
  IMP_IMPLEMENTATION_TEMPLATE_1(class SM,                               \
    void template_apply(const SM *sm) const, {                          \
                      LOOP(IMP::internal::call_apply_index(get_model(), \
                                                        sm, item));     \
                                });                                     \
  IMP_IMPLEMENTATION_TEMPLATE_1(class SS,                               \
  double template_evaluate(const SS *s,                                 \
                           DerivativeAccumulator *da) const, {          \
    double ret=0;                                                       \
    LOOP({                                                              \
        double cur=IMP::internal::call_evaluate_index(get_model(),      \
                                                      s, item, da);     \
      ret+=cur;                                                         \
      });                                                               \
    return ret;                                                         \
                                });                                     \
  IMP_IMPLEMENTATION_TEMPLATE_1(class SS,                               \
  double template_evaluate_if_good(const SS *s,                         \
                                   DerivativeAccumulator *da,           \
                                   double max) const, {                 \
    double ret=0;                                                       \
    LOOP({                                                              \
        double cur=IMP::internal::call_evaluate_if_good_index(get_model(), \
                                               s, item, da, max);       \
      ret+=cur;                                                         \
      max-=cur;                                                         \
      if (max < 0) return ret;                                          \
      });                                                               \
    return ret;                                                         \
                                });


//! Declare the needed functions for a SingletonContainer
/** In addition to the methods of IMP_OBJECT, it declares
    - IMP::SingletonContainer::get_contains_particle_particle()
    - IMP::SingletonContainer::get_number_of_particle_particles()
    - IMP::SingletonContainer::get_particle_particle()
    - IMP::SingletonContainer::apply()
    - IMP::SingletonContainer::evaluate()
    - IMP::Interaction::get_input_objects()
*/
#define IMP_SINGLETON_CONTAINER(Name)                      \
  IMP_IMPLEMENT(bool get_contents_changed() const);                     \
  IMP_IMPLEMENT(bool get_contains_particle(Particle* p)\
                const);                                       \
  IMP_IMPLEMENT(ParticleIndexes get_indexes() const);                   \
  IMP_IMPLEMENT(ParticleIndexes get_all_possible_indexes() const);      \
  IMP_IMPLEMENTATION_TEMPLATE_1(class S,                                \
    Restraints create_decomposition_t(S *s) const, {                    \
    ParticleIndexes all= get_all_possible_indexes();            \
    Restraints ret(all.size());                                         \
    for (unsigned int i=0; i< all.size(); ++i) {                        \
      ret[i]=  IMP::create_restraint(s,                           \
                                              IMP::internal::           \
                                              get_particle(get_model(), \
                                                           all[i]));    \
    }                                                                   \
    return ret;                                                         \
                                });                                     \
  IMP_IMPLEMENT_INLINE(Restraints                                       \
  create_decomposition(SingletonScore *s) const, {                      \
    ParticleIndexes all= get_all_possible_indexes();                    \
    Restraints ret(all.size());                                         \
    for (unsigned int i=0; i< all.size(); ++i) {                        \
      ret[i]= new IMP::SingletonRestraint(s,                            \
            IMP::internal::get_particle(get_model(), all[i]));          \
    }                                                                   \
    return ret;                                                         \
                       });                                              \
  IMP_IMPLEMENT_SINGLETON_CONTAINER(Name)


#ifndef SWIG
//! Declare the needed functions for a SingletonFilter
/** In addition to the methods done by all the macros, it declares
    - IMP::SingletonFilter::get_contains()
    - IMP::SingletonFilter::get_input_particles()
    - IMP::SingletonFilter::get_input_containers()
*/
#define IMP_SINGLETON_FILTER(Name)                                     \
public:                                                                 \
 IMP_IMPLEMENT(bool get_contains(Particle* p) const); \
 IMP_IMPLEMENT_INLINE(bool get_contains(Model *m,                       \
                                        ParticleIndex p)\
                      const, {                                          \
   return get_contains(IMP::internal::get_particle(m,p));               \
                      });                                               \
 IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle* t) const);   \
 IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle* t) const); \
 IMP_IMPLEMENT_INLINE(void filter_in_place(Model *m,\
                                   ParticleIndexes &ps) const, { \
   ps.erase(std::remove_if(ps.begin(), ps.end(),                        \
                           IMP::internal::GetContainsIndex<Name>(this,  \
                                                                 m)),   \
            ps.end());                                                  \
                      });                                               \
 IMP_IMPLEMENT_INLINE(void filter_in_place(ParticlesTemp &ps) const, { \
   ps.erase(std::remove_if(ps.begin(), ps.end(),                        \
                           IMP::internal::GetContains<Name>(this)),   \
            ps.end());                                                  \
   });                                                                  \
 IMP_OBJECT(Name)
#else
#define IMP_SINGLETON_FILTER(Name)                                     \
  bool get_contains(Particle* p) const;                    \
  bool get_contains(Model *m,ParticleIndex p) const;           \
  ParticlesTemp get_input_particles(Particle*t) const;                  \
  ContainersTemp get_input_containers(Particle*t) const;                \
  IMP_OBJECT(Name)
#endif


#ifndef SWIG
//! Declare the needed functions for a SingletonFilter
/** In addition to the methods done by all the macros, it declares
    - IMP::SingletonFilter::get_contains() (model, index variant)
    - IMP::SingletonFilter::get_input_particles()
    - IMP::SingletonFilter::get_input_containers()
*/
#define IMP_INDEX_SINGLETON_FILTER(Name)                               \
public:                                                                 \
IMP_IMPLEMENT_INLINE(bool get_contains(Particle* p) const,{\
    return get_contains(IMP::internal::get_model(p),                    \
                        IMP::internal::get_index(p));                   \
  });                                                                   \
IMP_IMPLEMENT(bool get_contains(Model *m,                               \
                                        ParticleIndex p)\
              const);                                                   \
 IMP_IMPLEMENT(ParticlesTemp get_input_particles(Particle* t) const);   \
 IMP_IMPLEMENT(ContainersTemp get_input_containers(Particle* t) const); \
 IMP_IMPLEMENT_INLINE(void filter_in_place(Model *m,\
                                   ParticleIndexes &ps) const, { \
   ps.erase(std::remove_if(ps.begin(), ps.end(),                        \
                           IMP::internal::GetContainsIndex<Name>(this,  \
                                                                 m)),   \
            ps.end());                                                  \
                      });                                               \
 IMP_IMPLEMENT_INLINE(void filter_in_place(ParticlesTemp &ps) const, { \
   ps.erase(std::remove_if(ps.begin(), ps.end(),                        \
                           IMP::internal::GetContains<Name>(this)),   \
            ps.end());                                                  \
   });                                                                  \
 IMP_OBJECT(Name)
#else
#define IMP_INDEX_SINGLETON_FILTER(Name)                     \
  bool get_contains(Particle* p) const;                    \
  bool get_contains(Model *m,ParticleIndex p) const;           \
  ParticlesTemp get_input_particles(Particle*t) const;                  \
  ContainersTemp get_input_containers(Particle*t) const;                \
  IMP_OBJECT(Name)
#endif


/** These macros avoid various inefficiencies.

    The macros take the name of the sequence and the operation to
    peform. The item in the sequence is called _1, it's index is _2.
    Use it like
    \code
    IMP_FOREACH_PARTICLE(sc, std::cout << "Item " << _2
                         << " is _1->get_name() << std::endl);
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
  do {                                                                  \
    if (sequence->get_provides_access()) {                              \
      const ParticleIndexes &imp_foreach_access                         \
        =sequence->get_access();                                        \
      for (unsigned int _2=0; _2< imp_foreach_access.size(); ++_2) {    \
        IMP::ParticleIndex _1= imp_foreach_access[_2];          \
        bool imp_foreach_break=false;                                   \
        operation                                                       \
          if (imp_foreach_break) { break;}                              \
      }                                                                 \
    } else {                                                            \
      ParticleIndexes imp_foreach_indexes              \
        =sequence->get_indexes();                                       \
      for (unsigned int _2=0;                                           \
           _2 != imp_foreach_indexes.size();                            \
           ++_2) {                                                      \
        IMP::ParticleIndex _1= imp_foreach_indexes[_2];            \
        bool imp_foreach_break=false;                                   \
        operation                                                       \
          if (imp_foreach_break) break;                                 \
      }                                                                 \
    }                                                                   \
  } while (false)

#endif  /* IMPKERNEL_SINGLETON_MACROS_H */
