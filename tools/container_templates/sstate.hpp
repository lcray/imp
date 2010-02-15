/**
 *  \file GroupnameConstraint.h
 *  \brief Use a GroupnameModifier applied to a Classnames to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_GROUPNAME_CONSTRAINT_H
#define IMPCORE_GROUPNAME_CONSTRAINT_H

#include "config.h"
#include <IMP/GroupnameModifier.h>
#include <IMP/Constraint.h>
#include <IMP/Particle.h>

IMP_BEGIN_NAMESPACE
// for swig
class GroupnameModifier;
IMP_END_NAMESPACE

IMPCORE_BEGIN_NAMESPACE
//! Apply a GroupnameFunction to a Groupname
/** The score state is passed up to two GroupnameModifiers, one to
    apply before evaluation and the other after. The one after
    should take a DerivativeAccumulator as its last argument for
    GroupnameModifier::apply() and will only be called if
    the score was computed with derivatives.

    \see GroupnamesConstraint
 */
class IMPCOREEXPORT GroupnameConstraint : public Constraint
{
  IMP::internal::OwnerPointer<GroupnameModifier> f_;
  IMP::internal::OwnerPointer<GroupnameModifier> af_;
  StoreValue v_;
public:
  /** before and after are the modifiers to apply before and after
      evaluate.
   */
  GroupnameConstraint(GroupnameModifier *before,
                      GroupnameModifier *after, PassValue vt,
                      std::string name="GroupnameConstraint %1%");

  //! Apply this modifier to all the elements after an evaluate
  void set_after_evaluate_modifier(GroupnameModifier* f) {
    af_=f;
  }

  //! Apply this modifier to all the elements before an evaluate
  void set_before_evaluate_modifier(GroupnameModifier* f) {
    f_=f;
  }

  IMP_CONSTRAINT(GroupnameConstraint)
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_GROUPNAME_CONSTRAINT_H */
