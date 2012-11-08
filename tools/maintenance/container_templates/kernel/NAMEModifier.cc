/**
 *  \file CLASSNAMEModifier.cpp   \brief A function on Particles.
 *
 *  BLURB
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/CLASSNAMEModifier.h"
#include "IMP/internal/utility.h"
#include "IMP/ModelObject.h"

IMP_BEGIN_NAMESPACE

CLASSNAMEModifier::CLASSNAMEModifier(std::string name):
  Object(name){
}

IMP_INPUTS_DEF(CLASSNAMEModifier);
IMP_OUTPUTS_DEF(CLASSNAMEModifier);

IMP_END_NAMESPACE
