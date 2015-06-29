/**
 * \file IMP/kinematics/helpers.h
 * \brief
 *
 * \authors Dina Schneidman
 * Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPKINEMATICS_HELPERS_H
#define IMPKINEMATICS_HELPERS_H

#include "kinematics_config.h"

#include <IMP/atom/Atom.h>
#include <IMP/atom/Residue.h>

#include <boost/algorithm/string.hpp>

IMPKINEMATICS_BEGIN_NAMESPACE

IMPKINEMATICSEXPORT
IMP::atom::Atom find_atom(const IMP::ParticlesTemp& atoms, int input_index);

IMPKINEMATICSEXPORT
IMP::atom::Residue find_residue(const IMP::ParticlesTemp& residues,
                                int res_index, std::string chain);

IMPKINEMATICSEXPORT
void read_connect_chains_file(const std::string& file_name,
                              const IMP::ParticlesTemp& atoms,
                              std::vector<IMP::atom::Atoms>& connect_atoms);

IMPKINEMATICSEXPORT
void read_angle_file(const std::string& file_name,
                     const IMP::ParticlesTemp& residues,
                     const IMP::ParticlesTemp& atoms,
                     IMP::atom::Residues& flexible_residues,
                     std::vector<IMP::atom::Atoms>& dihedral_angles);

IMPKINEMATICSEXPORT
IMP::atom::Bond create_bond(IMP::atom::Atoms& as);

IMPKINEMATICSEXPORT
void add_missing_bonds(IMP::ParticlesTemp& atoms, IMP::ParticlesTemp& bonds);

IMPKINEMATICS_END_NAMESPACE

#endif /* IMPKINEMATICS_HELPERS_H */
