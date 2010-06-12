/**
 *  \file BoxSweepClosePairsFinder.cpp
 *  \brief Test all pairs.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/base_types.h>
#include "IMP/core/BoxSweepClosePairsFinder.h"
#include "IMP/core/XYZR.h"

#ifdef IMP_USE_CGAL

/* compile the CGAL code with NDEBUG since it doesn't have the
   same level of control over errors as IMP
*/
#ifndef NDEBUG
#define NDEBUG
#endif

#include <CGAL/box_intersection_d.h>
#include <vector>
#include <IMP/macros.h>


IMPCORE_BEGIN_NAMESPACE


namespace {
struct NBLBbox
{
  XYZ d_;
  typedef Float NT;
  typedef void * ID;
  Float r_;
  NBLBbox(){}
  NBLBbox(Particle *p,
                  Float r): d_(p),
                            r_(r){}
  static unsigned int dimension() {return 3;}
  void *id() const {return d_.get_particle();}
  NT min_coord(unsigned int i) const {
    return d_.get_coordinate(i)-r_;
  }
  NT max_coord(unsigned int i) const {
    return d_.get_coordinate(i)+r_;
  }
  // make it so I can reused the callback provide by NBLSS
  operator Particle*() const {return d_.get_particle();}
};

static void copy_particles_to_boxes(const SingletonContainer *ps,
                                    Float distance,
                                    std::vector<NBLBbox> &boxes)
{
  boxes.resize(ps->get_number_of_particles());
  IMP_FOREACH_SINGLETON(ps, {
      Float r= distance/2.0;
      r+= _1->get_value(XYZR::get_default_radius_key());
      IMP_INTERNAL_CHECK(_2 < boxes.size(), "Off the end");
      boxes[_2]=NBLBbox(_1, r);
    });
}

struct AddToList {
  ParticlePairsTemp &out_;
  AddToList(ParticlePairsTemp &out): out_(out){}
  void operator()(const NBLBbox &a, const NBLBbox &b) {
    if (get_squared_distance(XYZ(a).get_coordinates(),
                         XYZ(b).get_coordinates())
        < square(a.r_ + b.r_)) {
      out_.push_back(ParticlePair(a,b));
    }
  }
};

}


BoxSweepClosePairsFinder::BoxSweepClosePairsFinder():
  ClosePairsFinder("BoxSweepCPF") {}

ParticlePairsTemp BoxSweepClosePairsFinder
::get_close_pairs(IMP_RESTRICT SingletonContainer *ca,
                  IMP_RESTRICT SingletonContainer *cb) const {
  set_was_used(true);
  std::vector<NBLBbox> boxes0, boxes1;
  copy_particles_to_boxes(ca, get_distance(), boxes0);
  copy_particles_to_boxes(cb, get_distance(), boxes1);

  ParticlePairsTemp out;

  CGAL::box_intersection_d( boxes0.begin(), boxes0.end(),
                            boxes1.begin(), boxes1.end(), AddToList(out));
  return out;
}

ParticlePairsTemp BoxSweepClosePairsFinder
::get_close_pairs(IMP_RESTRICT SingletonContainer *c) const {
  set_was_used(true);
  ParticlePairsTemp out;
  std::vector<NBLBbox> boxes;
  copy_particles_to_boxes(c, get_distance(), boxes);

  CGAL::box_self_intersection_d( boxes.begin(), boxes.end(), AddToList(out));
  return out;
}

void BoxSweepClosePairsFinder::do_show(std::ostream &out) const {
  out << "distance " << get_distance() << std::endl;
}


ParticlesTemp
BoxSweepClosePairsFinder::get_input_particles(SingletonContainer *sc) const {
  ParticlesTemp ret= sc->get_particles();
  return ret;
}

ParticlesTemp
BoxSweepClosePairsFinder::get_input_particles(SingletonContainer *a,
                                             SingletonContainer *b) const {
  ParticlesTemp ret0= a->get_particles();
  ParticlesTemp ret1= b->get_particles();
  ret0.insert(ret0.end(), ret1.begin(), ret1.end());
  return ret0;
}

ContainersTemp
BoxSweepClosePairsFinder::get_input_containers(SingletonContainer *sc) const {
  ContainersTemp ret(1,sc);
  return ret;
}

ContainersTemp
BoxSweepClosePairsFinder::get_input_containers(SingletonContainer *a,
                                              SingletonContainer *b) const {
  ContainersTemp ret(2);
  ret[0]= a;
  ret[1]= b;
  return ret;
}

IMPCORE_END_NAMESPACE
#endif /* IMP_USE_CGAL */
