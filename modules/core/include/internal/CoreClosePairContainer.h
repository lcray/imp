/**
 *  \file core/internal/CoreClosePairContainer.h
 *  \brief Return all pairs from a SingletonContainer
 *
 *  Copyright 2007-2016 IMP Inventors. Close rights reserved.
 */

#ifndef IMPCORE_INTERNAL_CORE_CLOSE_PAIR_CONTAINER_H
#define IMPCORE_INTERNAL_CORE_CLOSE_PAIR_CONTAINER_H

#include <IMP/core/core_config.h>
#include "../ClosePairsFinder.h"
#include "MovedSingletonContainer.h"
#include "../PairRestraint.h"
#include <IMP/PairContainer.h>
#include <IMP/PairPredicate.h>
#include <IMP/generic.h>
#include <IMP/Pointer.h>
#include <IMP/SingletonContainer.h>
#include <IMP/internal/ContainerScoreState.h>
#include <IMP/internal/ListLikeContainer.h>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

class IMPCOREEXPORT CoreClosePairContainer
    : public IMP::internal::ListLikeContainer<PairContainer> {
  IMP::PointerMember<SingletonContainer> c_;
  IMP::PointerMember<ClosePairsFinder> cpf_;
  IMP::PointerMember<internal::MovedSingletonContainer> moved_;
  unsigned int moved_count_;
  bool first_call_;
  double distance_, slack_;
  unsigned int updates_, rebuilds_, partial_rebuilds_;
  typedef IMP::internal::ContainerScoreState<CoreClosePairContainer> SS;
  PointerMember<SS> score_state_;

  void initialize(SingletonContainer *c, double distance, double slack,
                  ClosePairsFinder *cpf);

  void check_duplicates_input() const;
  void check_list(bool include_slack) const;
  void do_first_call();
  void do_incremental();
  void do_rebuild();

 public:
  ModelObjectsTemp get_score_state_inputs() const;
  virtual ParticleIndexes get_all_possible_indexes() const IMP_OVERRIDE;
  virtual ParticleIndexPairs get_range_indexes() const IMP_OVERRIDE;
  virtual ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  void do_score_state_before_evaluate();
  void do_score_state_after_evaluate() {}

  CoreClosePairContainer(SingletonContainer *c, double distance,
                         ClosePairsFinder *cpf, double slack = 1,
                         std::string name = "CoreClosePairContainer%1%");

  IMP_LIST_ACTION(public, PairFilter, PairFilters, pair_filter, pair_filters,
                  PairPredicate *, PairPredicates,
  {
    set_has_dependencies(false);
    obj->set_was_used(true);
  },
  { set_has_dependencies(false); }, );

  void clear_caches() { first_call_ = true; }
  double get_slack() const { return slack_; }
  double get_distance() const { return distance_; }
  void update() { do_score_state_before_evaluate(); }
  SingletonContainer *get_singleton_container() const { return c_; }
  ClosePairsFinder *get_close_pairs_finder() const { return cpf_; }
  void set_slack(double d);
  Restraints create_decomposition(PairScore *ps) const {
    ParticleIndexPairs all = get_range_indexes();
    Restraints ret(all.size());
    for (unsigned int i = 0; i < all.size(); ++i) {
      ret[i] = new PairRestraint(get_model(), ps, all[i]);
    }
    return ret;
  }
  template <class PS>
  Restraints create_decomposition_t(PS *ps) const {
    ParticleIndexPairs all = get_range_indexes();
    Restraints ret(all.size());
    for (unsigned int i = 0; i < all.size(); ++i) {
      ret[i] = IMP::create_restraint(
          ps, IMP::internal::get_particle(get_model(), all[i]));
    }
    return ret;
  }
  unsigned int get_number_of_update_calls() const { return updates_; }
  unsigned int get_number_of_full_rebuilds() const { return rebuilds_; }
  unsigned int get_number_of_partial_rebuilds() const {
    return partial_rebuilds_;
  }

  IMP_OBJECT_METHODS(CoreClosePairContainer);
};

IMP_OBJECTS(CoreClosePairContainer, CoreClosePairContainers);

IMPCORE_END_INTERNAL_NAMESPACE

#endif /* IMPCORE_INTERNAL_CORE_CLOSE_PAIR_CONTAINER_H */
