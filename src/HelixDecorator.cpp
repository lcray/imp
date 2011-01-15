/**
 *  \file membrane/HelixDecorator.cpp
 *  \brief Add a name to a particle.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include <IMP/membrane/HelixDecorator.h>

IMPMEMBRANE_BEGIN_NAMESPACE

FloatKey HelixDecorator::get_helix_key(unsigned int i) {
    IMP_USAGE_CHECK(i <2, "Out of range");
    static FloatKeys helix_keys;
    if(helix_keys.empty()) {
     helix_keys.push_back(FloatKey("helix begin"));
     helix_keys.push_back(FloatKey("helix end"));
    }
    return helix_keys[i];
  }

void HelixDecorator::show(std::ostream &out) const {
  out << get_begin(), get_end();
}

IMPMEMBRANE_END_NAMESPACE
