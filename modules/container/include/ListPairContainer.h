/**
 *  \file ListPairContainer.h    \brief Store a list of ParticlePairs
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCONTAINER_LIST_PAIR_CONTAINER_H
#define IMPCONTAINER_LIST_PAIR_CONTAINER_H

#include "config.h"
#include <IMP/core/internal/CoreListPairContainer.h>

IMPCONTAINER_BEGIN_NAMESPACE

//! Store a list of ParticlePairs
/** \note The indexes can change when particles are inserted
    as the list is maintained in sorted order.
 */
class IMPCONTAINEREXPORT ListPairContainer:
#if defined(IMP_DOXYGEN) || defined(SWIG)
public PairContainer
#else
public core::internal::CoreListPairContainer
#endif
{
  typedef core::internal::CoreListPairContainer P;
  // for the change versions
  ListPairContainer(bool);
public:
  //! construct and pass an initial set of particle_pairs
  ListPairContainer(const ParticlePairs &ps,
                         std::string name= "ListPairContainer %1%");

  ListPairContainer(std::string name= "ListPairContainer %1%");
  ListPairContainer(const char *name);

#if defined(IMP_DOXYGEN) || defined(SWIG)
 /** @name Methods to control the contained objects

     This container stores a list of ParticlePair objects. To manipulate
     the list use these methods.
  */
  /**@{*/
  void add_particle_pair(const ParticlePair& vt);
  void add_particle_pairs(const ParticlePairsTemp &c);
  void set_particle_pairs(ParticlePairsTemp c);
  IMP_NO_DOXYGEN(void add_particle_pairs(const ParticlePairs &c) {
      add_particle_pairs(static_cast<const ParticlePairsTemp&>(c));
    })
  IMP_NO_DOXYGEN(void set_particle_pairs(const ParticlePairs &c) {
      set_particle_pairs(static_cast<ParticlePairsTemp>(c));
    })
  void clear_particle_pairs();
  /**@}*/
  IMP_PAIR_CONTAINER(ListPairContainer, get_module_version_info());
#else
  static ListPairContainer *create_untracked_container() {
    ListPairContainer *lsc = new ListPairContainer(false);
    return lsc;
  }
  IMP_OBJECT(ListPairContainer, get_module_version_info());
#endif
};


IMPCONTAINER_END_NAMESPACE

#endif  /* IMPCONTAINER_LIST_PAIR_CONTAINER_H */
