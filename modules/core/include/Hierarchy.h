/**
 *  \file IMP/core/Hierarchy.h     \brief Decorator for helping deal with
 *                                        a hierarchy.
 *
 *  Copyright 2007-2014 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPCORE_HIERARCHY_H
#define IMPCORE_HIERARCHY_H

#include <IMP/core/core_config.h>
#include "internal/hierarchy_helpers.h"

#include <IMP/SingletonModifier.h>
#include <IMP/kernel/Particle.h>
#include <IMP/kernel/Model.h>
#include <IMP/Decorator.h>
#include <IMP/decorator_macros.h>
#include <IMP/kernel/internal/utility.h>
#include <IMP/base/Pointer.h>

#include <boost/tuple/tuple.hpp>

#include <limits>
#include <vector>
#include <deque>

IMPCORE_BEGIN_NAMESPACE

/** \defgroup hierarchy Hierarchies of particles
    These functions and classes aid in manipulating particles representing
    molecules at multiple levels.
 */
#ifndef SWIG
class Hierarchy;
#endif

//! Define the type for a type of hierarchy
/** The hierarchy class is identified by the passed string so two
    hierarchies created with the same initialization string will be
    the same.

    This example shows how to make and use a custom hierarchy:
    \include custom_hierarchy.py
    \see Hierarchy
*/
class IMPCOREEXPORT HierarchyTraits {
  kernel::ParticleIndexesKey children_;
  kernel::ParticleIndexKey parent_;

 public:
  HierarchyTraits() {}
  //! Create a HierarchyTraits with the given name
  HierarchyTraits(std::string name);
  kernel::ParticleIndexesKey get_children_key() const { return children_; }
  kernel::ParticleIndexKey get_parent_key() const { return parent_; }
  bool operator==(const HierarchyTraits &o) const {
    return parent_ == o.parent_;
  }
  IMP_SHOWABLE_INLINE(HierarchyTraits, out << parent_);
};

IMP_VALUES(HierarchyTraits, HierarchyTraitsList);

class Hierarchy;

#ifndef IMP_DOXYGEN
typedef IMP::base::Vector<Hierarchy> GenericHierarchies;
#endif

//! A decorator for helping deal with a hierarchy.
/**
    See HierarchyTraits for an example of how to define a custom hierarchy
    and Hierarchy for a hierarchy for molecules.
    \ingroup hierarchy
    \see HierarchyTraits
 */
class IMPCOREEXPORT Hierarchy : public Decorator {
  static void do_setup_particle(kernel::Model *, kernel::ParticleIndex,
                                HierarchyTraits) {}
  static void do_setup_particle(kernel::Model *m, kernel::ParticleIndex pi,
                                const kernel::ParticleIndexes &children,
                                HierarchyTraits traits) {
    for (unsigned int i = 0; i < children.size(); ++i) {
      m->add_attribute(traits.get_parent_key(), children[i], pi);
    }
    m->add_attribute(traits.get_children_key(), pi, children);
  }
  static void do_setup_particle(kernel::Model *m, kernel::ParticleIndex pi,
                                const kernel::ParticlesTemp &children,
                                HierarchyTraits traits) {
    do_setup_particle(m, pi, kernel::get_indexes(children), traits);
  }

 public:
  IMP_DECORATOR_WITH_TRAITS_METHODS(Hierarchy, Decorator, HierarchyTraits,
                                    traits, get_default_traits());
  IMP_DECORATOR_TRAITS_SETUP_0(Hierarchy);
  /** Setup the particle and add children. */
  IMP_DECORATOR_TRAITS_SETUP_1(Hierarchy, kernel::ParticleIndexes, children);
  IMP_DECORATOR_TRAITS_SETUP_1(Hierarchy, kernel::ParticlesTemp, children);

  /** Check if the particle has the needed attributes for a
   cast to succeed */
  static bool get_is_setup(kernel::Model *, kernel::ParticleIndex,
                           HierarchyTraits = Hierarchy::get_default_traits()) {
    return true;
  }
  /** \return the parent particle, or Hierarchy()
      if it has no parent.
   */
  Hierarchy get_parent() const {
    if (get_model()->get_has_attribute(get_decorator_traits().get_parent_key(),
                                       get_particle_index())) {
      kernel::ParticleIndex VALUE = get_model()->get_attribute(
          get_decorator_traits().get_parent_key(), get_particle_index());
      return Hierarchy(get_model(), VALUE, get_decorator_traits());
    } else {
      return Hierarchy();
    }
  }

  unsigned int get_number_of_children() const {
    if (get_model()->get_has_attribute(
            get_decorator_traits().get_children_key(), get_particle_index())) {
      return get_model()
          ->get_attribute(get_decorator_traits().get_children_key(),
                          get_particle_index())
          .size();
    } else {
      return 0;
    }
  }
  Hierarchy get_child(unsigned int i) const {
    IMP_USAGE_CHECK(i < get_number_of_children(), "Invalid child requested");
    return Hierarchy(get_model(), get_model()->get_attribute(
                                      get_decorator_traits().get_children_key(),
                                      get_particle_index())[i],
                     get_decorator_traits());
  }
  kernel::ParticleIndex get_child_index(unsigned int i) const {
    IMP_USAGE_CHECK(i < get_number_of_children(), "Invalid child requested");
    return get_model()->get_attribute(get_decorator_traits().get_children_key(),
                                      get_particle_index())[i];
  }
  kernel::ParticleIndexes get_children_indexes() const {
    if (get_model()->get_has_attribute(
            get_decorator_traits().get_children_key(), get_particle_index())) {
      return get_model()->get_attribute(
          get_decorator_traits().get_children_key(), get_particle_index());
    } else {
      return kernel::ParticleIndexes();
    }
  }
  GenericHierarchies get_children() const {
    GenericHierarchies ret(get_number_of_children());
    for (unsigned int i = 0; i < ret.size(); ++i) {
      ret[i] = get_child(i);
    }
    return ret;
  }
  void remove_child(unsigned int i) {
    IMP_USAGE_CHECK(i < get_number_of_children(), "Invalid child requested");
    Hierarchy c = get_child(i);
    kernel::ParticleIndexes &pis = get_model()->access_attribute(
        get_decorator_traits().get_children_key(), get_particle_index());
    pis.erase(pis.begin() + i);
    get_model()->remove_attribute(get_decorator_traits().get_parent_key(),
                                  c.get_particle_index());
  }
  void remove_child(Hierarchy h) { remove_child(h.get_child_index()); }
  void clear_children() {
    kernel::ParticleIndexes &pis = get_model()->access_attribute(
        get_decorator_traits().get_children_key(), get_particle_index());
    for (unsigned int i = 0; i < pis.size(); ++i) {
      get_model()->remove_attribute(get_decorator_traits().get_parent_key(),
                                    pis[i]);
    }
    get_model()->remove_attribute(get_decorator_traits().get_children_key(),
                                  get_particle_index());
  }
  void add_child(Hierarchy h) const {
    if (get_model()->get_has_attribute(
            get_decorator_traits().get_children_key(), get_particle_index())) {
      get_model()
          ->access_attribute(get_decorator_traits().get_children_key(),
                             get_particle_index())
          .push_back(h.get_particle_index());
    } else {
      get_model()->add_attribute(
          get_decorator_traits().get_children_key(), get_particle_index(),
          kernel::ParticleIndexes(1, h.get_particle_index()));
    }
    get_model()->add_attribute(get_decorator_traits().get_parent_key(),
                               h.get_particle_index(), get_particle_index());
  }
  void add_child_at(Hierarchy h, unsigned int pos) {
    IMP_USAGE_CHECK(get_number_of_children() >= pos, "Invalid position");
    if (get_model()->get_has_attribute(
            get_decorator_traits().get_children_key(), get_particle_index())) {
      kernel::ParticleIndexes &pis = get_model()->access_attribute(
          get_decorator_traits().get_children_key(), get_particle_index());
      pis.insert(pis.begin() + pos, h.get_particle_index());
    } else {
      get_model()->add_attribute(
          get_decorator_traits().get_children_key(), get_particle_index(),
          kernel::ParticleIndexes(1, h.get_particle_index()));
    }
    get_model()->add_attribute(get_decorator_traits().get_parent_key(),
                               h.get_particle_index(), get_particle_index());
  }
  /** Return i such that `get_parent().get_child(i) == this` */
  int get_child_index() const;
  //! Get the default hierarchy traits
  static const HierarchyTraits &get_default_traits();

  HierarchyTraits get_traits() { return get_decorator_traits(); }
};

//! A visitor for traversal of a hierarchy
/** This works from both C++ and Python
    \ingroup hierarchy
    \ingroup decorators
    \see Hierarchy
 */
class IMPCOREEXPORT HierarchyVisitor {
 public:
  HierarchyVisitor() {}
  //! Return true if the traversal should visit this node's children
  virtual bool operator()(Hierarchy p) = 0;
  virtual ~HierarchyVisitor() {}
};

//! A visitor which applies a modifier to each kernel::Particle in a hierarchy
/** This works from both C++ and Python
    \ingroup hierarchy
    \ingroup decorators
    \see SingletonModifier
    \see Hierarchy
 */
class IMPCOREEXPORT ModifierVisitor : public HierarchyVisitor {
  IMP::base::PointerMember<SingletonModifier> sm_;

 public:
  ModifierVisitor(SingletonModifier *sm) : sm_(sm) {}
  virtual bool operator()(Hierarchy p) {
    sm_->apply_index(p.get_model(), p.get_particle_index());
    return true;
  }
  virtual ~ModifierVisitor() {}
};

#if !defined(SWIG) && !defined(IMP_DOXYGEN)
namespace internal {
template <class H, class F, class Out, bool Slice = false>
struct Gather {
  //! initialize with the function and the container
  Gather(F f, Out out) : f_(f), out_(out) {}
  bool operator()(H p) {
    if (f_(p)) {
      *out_ = p;
      ++out_;
      if (Slice) return false;
    }
    return true;
  }
  //! Return the container
  Out get_out() const { return out_; }

 private:
  F f_;
  Out out_;
};
}
#endif

//! Apply the visitor to each particle, breadth first.
/** \param[in] d The Hierarchy for the tree in question
    \param[in] f The visitor to be applied. This is passed by reference.
    A branch of the traversal stops when f returns false.
    \ingroup hierarchy
    See Hierarchy
 */
template <class HD, class F>
inline F visit_breadth_first(HD d, F f) {
  std::deque<HD> stack;
  stack.push_back(d);
  // d.show(std::cerr);
  do {
    HD cur = stack.front();
    stack.pop_front();
    if (f(cur)) {
      // std::cerr << "Visiting particle " << cur.get_particle() << std::endl;
      for (int i = cur.get_number_of_children() - 1; i >= 0; --i) {
        stack.push_back(cur.get_child(i));
      }
    }
  } while (!stack.empty());
  return f;
}

//! Apply functor F to each particle, traversing the hierarchy depth first.
/** See visit_breadth_first() for documentation.
    \ingroup hierarchy
    See Hierarchy
 */
template <class HD, class F>
inline F visit_depth_first(HD d, F &f) {
  base::Vector<HD> stack;
  stack.push_back(d);
  // d.show(std::cerr);
  do {
    HD cur = stack.back();
    stack.pop_back();
    if (f(cur)) {
      // std::cerr << "Visiting particle " << cur.get_particle() << std::endl;
      for (int i = cur.get_number_of_children() - 1; i >= 0; --i) {
        stack.push_back(cur.get_child(i));
      }
    }
  } while (!stack.empty());
  return f;
}

//! Apply functor F to each particle, traversing the hierarchy breadth first.
/** This method allows data to be associated with each visited node.
    The data of the parent is passed to each invocation of the child.

    \param[in] d The Hierarchy for the tree in question
    \param[in] f The functor to be applied
    F must define a type Data which is returned by each call.
    The result of the parent call is passed as the second argument
    to the operator() of the child. e.g.
    struct DepthVisitor {
      typedef int result_type;
      result_type operator()(kernel::Particle *p, int i) const
      {
        if (p==nullptr) return 0;
        else return i+1;
      }
    };
    \param[in] i The data to be used for d (since it has no relevant parent)

    \return A copy of the functor passed in. Use this if you care about
           the functor state.

    \ingroup hierarchy
    See Hierarchy
 */
template <class HD, class F>
inline F visit_breadth_first_with_data(HD d, F f, typename F::result_type i) {
  typedef std::pair<typename F::result_type, HD> DP;
  std::deque<DP> stack;
  stack.push_back(DP(i, d));
  // d.show(std::cerr);
  do {
    DP cur = stack.front();
    stack.pop_front();
    typename F::result_type r = f(cur.second.get_particle(), cur.first);
    // std::cerr << "Visiting particle " << cur.get_particle() << std::endl;
    for (int i = cur.second.get_number_of_children() - 1; i >= 0; --i) {
      stack.push_back(std::make_pair(r, cur.second.get_child(i)));
    }
  } while (!stack.empty());
  return f;
}

//! Apply functor F to each particle, traversing the hierarchy depth first.
/** See visit_breadth_first() for documentation.
    \ingroup hierarchy
    See Hierarchy
 */
template <class HD, class F>
inline F visit_depth_first_with_data(HD d, F f, typename F::result_type i) {
  typedef std::pair<typename F::result_type, HD> DP;
  base::Vector<DP> stack;
  stack.push_back(DP(i, d));
  // d.show(std::cerr);
  do {
    DP cur = stack.back();
    stack.pop_back();
    typename F::result_type r = f(cur.second, cur.first);
    // std::cerr << "Visiting particle " << cur.get_particle() << std::endl;
    for (int i = cur.second.get_number_of_children() - 1; i >= 0; --i) {
      stack.push_back(DP(r, cur.second.get_child(i)));
    }
  } while (!stack.empty());
  return f;
}

//! Print the hierarchy using a given decorator as to display each node
/** The last argument limits how deep will be printed out.
    \ingroup hierarchy
    See Hierarchy
 */
template <class ND>
inline std::ostream &show(Hierarchy h, std::ostream &out = std::cout) {
  IMP_PRINT_TREE(out, Hierarchy, h, n.get_number_of_children(), n.get_child,
                 ND(n).show(out));
  return out;
}

//! A simple functor to count the number of particles in a hierarchy.
/** This is a good example of a simple HierarchyVisitor.
    \ingroup hierarchy
    \see Hierarchy
 */
struct HierarchyCounter : public HierarchyVisitor {
  HierarchyCounter() : ct_(0) {}

  //! Increment the counter
  bool operator()(Hierarchy) {
    ++ct_;
    return true;
  }
  //! Return how many nodes have been visited
  unsigned int get_count() const { return ct_; }
  IMP_SHOWABLE_INLINE(HierarchyCounter, out << get_count());

 private:
  unsigned int ct_;
};

IMP_VALUES(HierarchyCounter, HierarchyCounters);

//! Gather all the particles in the hierarchy which meet some criteria
/** \ingroup hierarchy
    See Hierarchy
 */
template <class H, class Out, class F>
inline Out gather(H h, F f, Out out) {
  internal::Gather<H, F, Out> gather(f, out);
  visit_depth_first(h, gather);
  return gather.get_out();
}

//! Gather all the particles in the hierarchy which meet some criteria
/** Descent in the tree terminates when a node is gathered so that
    none of its children are explored.

    \ingroup hierarchy
    See Hierarchy
 */
template <class H, class Out, class F>
inline Out gather_slice(H h, F f, Out out) {
  internal::Gather<H, F, Out, true> gather(f, out);
  visit_depth_first(h, gather);
  return gather.get_out();
}

//! Gather all the particles in the hierarchy which match on an attribute
/** \ingroup hierarchy
    See Hierarchy
 */
template <class H, class Out, class K, class V>
inline Out gather_by_attribute(H h, K k, V v, Out out) {
  internal::Gather<H, internal::MatchAttribute<K, V>, Out> gather(
      internal::MatchAttribute<K, V>(k, v), out);
  visit_depth_first(h, gather);
  return gather.get_out();
}

//! Gather all the particles in the hierarchy which match on two attributes
/** \ingroup hierarchy
    See Hierarchy
 */
template <class H, class Out, class K0, class V0, class K1, class V1>
inline Out gather_by_attributes(H h, K0 k0, V0 v0, K1 k1, V1 v1, Out out) {
  internal::Gather<H, internal::MatchAttributes<K0, V0, K1, V1>, Out> gather(
      internal::MatchAttributes<K0, V0, K1, V1>(k0, v0, k1, v1), out);
  visit_depth_first(h, gather);
  return gather.get_out();
}

//! Find the first node which matches some criteria
/** \ingroup hierarchy
    See Hierarchy
 */
template <class HD, class F>
inline HD find_breadth_first(HD h, F f) {
  if (f(h.get_particle())) return h;
  base::Vector<HD> stack;
  stack.push_back(h);
  // d.show(std::cerr);
  do {
    HD cur = stack.back();
    stack.pop_back();

    for (int i = cur.get_number_of_children() - 1; i >= 0; --i) {
      HD hd = cur.get_child(i);
      if (f(hd.get_particle())) {
        return hd;
      } else {
        stack.push_back(hd);
      }
    }
  } while (!stack.empty());
  return HD();
}

//! Get all the leaves of the bit of hierarchy
/** The leaves are returned in the obvious order
    (first child before second child).

    See Hierarchy
 */
IMPCOREEXPORT GenericHierarchies get_leaves(Hierarchy mhd);

//! Get all the non-leaves of the bit of hierarchy
/**     See Hierarchy
 */
IMPCOREEXPORT GenericHierarchies get_internal(Hierarchy mhd);

//! Get all the particles in the subtree
/**     See Hierarchy
 */
IMPCOREEXPORT GenericHierarchies get_all_descendants(Hierarchy mhd);

//! Return the root of the hierarchy
/** See Hierarchy */
inline Hierarchy get_root(Hierarchy h) {
  while (h.get_parent()) {
    h = h.get_parent();
  }
  return h;
}

IMPCORE_END_NAMESPACE

#endif /* IMPCORE_HIERARCHY_H */
