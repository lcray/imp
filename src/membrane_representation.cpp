/**
 *  \file membrane_representation.cpp
 *  \brief Membrane stuff
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/algebra.h>
#include <IMP/core.h>
#include <IMP/atom.h>
#include <IMP/membrane.h>

using namespace IMP;

IMPMEMBRANE_BEGIN_NAMESPACE

core::TableRefiner* generate_TM(Model *m,
 atom::Hierarchy protein, Parameters *myparam)
{
int jseq;
unsigned nres;
double x,y,z;
double vol,rg,bb,ee;
algebra::Rotation3D rot;
algebra::Transformation3D tr;
atom::Hierarchy helixpdb;
Particles ps;
atom::ResidueType restype, check;
bool read_struct;
HelixData* TM=&(myparam->TM);
IMP_NEW(core::TableRefiner,tbr,());

for(int i=0;i<TM->num;++i){
 IMP_NEW(Particle,pm,(m));
 atom::Molecule tm=atom::Molecule::setup_particle(pm);
 tm->set_name(TM->name[i]);
 nres=TM->resid[i].second-TM->resid[i].first+1;
 core::XYZs atoms;
// pdb structure
 read_struct = (TM->struct_file[i].length()>0.0);
 if(read_struct){
  IMP_NEW(atom::CAlphaPDBSelector,sel,());
  helixpdb=atom::read_pdb(TM->struct_file[i],m,sel);
  ps=atom::get_leaves(helixpdb);
  if(ps.size()!=nres) IMP_FAILURE("Check number of residues in your pdb!");
 }
// cycle on the number of residues
 for(int j=0;j<nres;++j){
  // set up residue
  IMP_NEW(Particle,pr,(m));
  // residue index and type
  jseq=TM->resid[i].first+j;
  restype=atom::get_residue_type(TM->seq[jseq-1]);
  if(read_struct){
   check=atom::Residue(atom::Atom(ps[j]).get_parent()).get_residue_type();
   if(check != restype){
    std::cout << check << " " << restype << std::endl;
    IMP_FAILURE("Check sequence in your pdb!");
   }
  }
  atom::Residue r=atom::Residue::setup_particle(pr,restype,jseq);
  //set up atom
  IMP_NEW(Particle,pa,(m));
  atom::Atom a=atom::Atom::setup_particle(pa,atom::AT_CA);
  // calculate radius
  vol=atom::get_volume_from_residue_type(restype);
  if(myparam->RST.use_volume) rg=algebra::get_ball_radius_from_volume_3d(vol);
  else rg=2.273;
  // coordinates
  if(read_struct){
   core::XYZR aa=core::XYZR(ps[j]);
   x=aa.get_coordinates()[0];
   y=aa.get_coordinates()[1];
   z=aa.get_coordinates()[2];
  }else{
   x=2.3*cos(radians(100.0)*double(j));
   y=2.3*sin(radians(100.0)*double(j));
   z=1.51*(double(j)-double((nres-1))/2.0);
  }
  core::XYZR ad=core::XYZR::setup_particle
  (pa,algebra::Sphere3D(algebra::Vector3D(x,y,z),rg));
  r.add_child(a);
  tm.add_child(r);
  atoms.push_back(ad);
 }
 if(read_struct) atom::destroy(helixpdb);
 protein.add_child(tm);
 // create rigid body
 IMP_NEW(Particle,prb,(m));
 core::RigidBody rb=core::RigidBody::setup_particle(prb,atoms);
 rb->set_name(TM->name[i]);
 // TableRefiner
 tbr->add_particle(prb,atoms);

 // Check orientation of x-axis and topology
 bb = (core::RigidMember(atoms[0]).get_internal_coordinates())[0];
 ee = (core::RigidMember(atoms[nres-1]).get_internal_coordinates())[0];
 if ( TM->topo[i]*(ee-bb)<0.0 ){
  for(unsigned int k=0;k<atoms.size();++k){
   algebra::Vector3D coord=
   core::RigidMember(atoms[k]).get_internal_coordinates();
   rot=algebra::get_rotation_about_axis(algebra::Vector3D(0,0,1),IMP::PI);
   tr=algebra::Transformation3D(rot,algebra::Vector3D(0,0,0));
   core::RigidMember(atoms[k]).set_internal_coordinates
    (tr.get_transformed(coord));
  }
 }

// randomize initial positions
 double xx,yy;
 if(i==0){
  xx=0.0;
  yy=0.0;
 }else if(i==1){
  xx=rand()*(myparam->RST.diameter-5.0)/RAND_MAX+5.0;
  yy=0.0;
 }else{
  xx=rand()*2.0*myparam->RST.diameter/RAND_MAX-myparam->RST.diameter;
  yy=rand()*2.0*myparam->RST.diameter/RAND_MAX-myparam->RST.diameter;
 }
 rb.set_reference_frame(algebra::ReferenceFrame3D(algebra::Transformation3D
      (algebra::get_rotation_about_axis(algebra::Vector3D(0,1,0),-IMP::PI/2.0),
       algebra::Vector3D(xx,yy,0))));
 //initialize helix decorator
 bb = (core::RigidMember(atoms[0]).get_internal_coordinates())[0];
 ee = (core::RigidMember(atoms[nres-1]).get_internal_coordinates())[0];
 membrane::HelixDecorator d_rbs=
 membrane::HelixDecorator::setup_particle(prb,bb,ee);

}
return tbr.release();
}

IMPMEMBRANE_END_NAMESPACE
