/**
 *  \file spb_restraint.cpp
 *  \brief SPB Restraints
 *
 *  Copyright 2011 IMP Inventors. All rights reserved.
 *
 */
#include <IMP/core.h>
#include <IMP/atom.h>
#include <IMP/algebra.h>
#include <IMP/container.h>
#include <IMP/membrane.h>
#include <string>
using namespace IMP;

IMPMEMBRANE_BEGIN_NAMESPACE

core::DistancePairScore*
 get_pair_score(FloatRange dist, double kappa)
{
 IMP_NEW(core::HarmonicWell,hw,(dist,kappa));
 IMP_NEW(core::DistancePairScore,ps,(hw));
 return ps.release();
}

core::SphereDistancePairScore*
 get_sphere_pair_score(FloatRange dist, double kappa)
{
 IMP_NEW(core::HarmonicWell,hw,(dist,kappa));
 IMP_NEW(core::SphereDistancePairScore,ps,(hw));
 return ps.release();
}

core::SphereDistancePairScore*
 get_sphere_pair_score(double dist, double kappa)
{
 IMP_NEW(core::HarmonicUpperBound,hu,(dist,kappa));
 IMP_NEW(core::SphereDistancePairScore,ps,(hu));
 return ps.release();
}

void add_SPBexcluded_volume (Model *m,atom::Hierarchies& hs,double kappa)
{
 IMP_NEW(container::ListSingletonContainer,lsc,(m));
 for(unsigned int i=0;i<hs.size();++i){
  lsc->add_particles(atom::get_leaves(hs[i]));
 }
 IMP_NEW(core::ExcludedVolumeRestraint,evr,(lsc,kappa));
 evr->set_name("Excluded Volume");
 m->add_restraint(evr);
}

void add_internal_restraint(Model *m,std::string name,
atom::Molecule protein_a,atom::Molecule protein_b,double kappa,double dist)
{
 core::SphereDistancePairScore*
 ps=get_sphere_pair_score(dist,kappa);
 atom::Selection sa=atom::Selection(protein_a);
 atom::Selection sb=atom::Selection(protein_b);
 sa.set_terminus(atom::Selection::C);
 sb.set_terminus(atom::Selection::N);
 Particle*  pa=sa.get_selected_particles()[0];
 Particle*  pb=sb.get_selected_particles()[0];
 IMP_NEW(core::PairRestraint,r,(ps,ParticlePair(pa, pb)));
 r->set_name("IR " + name);
 m->add_restraint(r);
}

FloatRange get_range_from_fret_class(std::string r_class)
{
 FloatRange range;
 if (r_class=="High")   {range=FloatRange(0.0,  41.0);}
 if (r_class=="Mod")    {range=FloatRange(41.0, 55.5);}
 if (r_class=="Low")    {range=FloatRange(55.5, 66.0);}
 if (r_class=="Lowest") {range=FloatRange(66.0, 70.0);}
 if (r_class=="None")   {range=FloatRange(70.0, 100000.0);}
 return range;
}

FloatRange get_range_from_fret_value(double r_value)
{
 std::string r_class;
 if (r_value >= 2.0)                   {r_class="High";}
 if (r_value >= 1.5 && r_value < 2.0)  {r_class="Mod";}
 if (r_value >= 1.2 && r_value < 1.5)  {r_class="Low";}
 if (r_value >= 1.05 && r_value < 1.2) {r_class="Lowest";}
 if (r_value <  1.05)                  {r_class="None";}
 return get_range_from_fret_class(r_class);
}

void do_bipartite_mindist(Model *m,Particles p1,Particles p2,
 core::SphereDistancePairScore* dps,bool filter)
{
 IMP_NEW(container::ListPairContainer,lpc,(m));
 for(unsigned int i=0;i<p1.size();++i){
  for(unsigned int j=0;j<p2.size();++j){
   bool samep=(atom::Hierarchy(p1[i]).get_parent() ==
               atom::Hierarchy(p2[j]).get_parent());
   if(filter && samep){
    continue;
   } else {
    lpc->add_particle_pair(ParticlePair(p1[i],p2[j]));
   }
  }
 }
 if(lpc->get_number_of_particle_pairs()==0) {return;}
 IMP_NEW(container::MinimumPairRestraint,mpr,(dps,lpc,1));
 m->add_restraint(mpr);
}
/*
void do_bipartite_mindist(Model *m,Particles p1,Particles p2,
 core::SphereDistancePairScore* dps,bool filter)
{
 IMP_NEW(container::ListSingletonContainer,lpc1,(p1));
 IMP_NEW(container::ListSingletonContainer,lpc2,(p2));
 IMP_NEW(container::CloseBipartitePairContainer,cbpc,(lpc1,lpc2,200.0));
 if(filter){
  IMP_NEW(atom::SameResiduePairFilter,f,());
  cbpc->add_pair_filter(f);
 }
 IMP_NEW(container::MinimumPairRestraint,mpr,(dps,cbpc,1));
 m->add_restraint(mpr);
}
*/
void do_allpairs_mindist(Model *m,Particles ps,
 core::SphereDistancePairScore* dps,bool filter)
{
 IMP_NEW(container::ListPairContainer,lpc,(m));
 for(unsigned int i=0;i<ps.size()-1;++i){
  for(unsigned int j=i+1;j<ps.size();++j){
   bool samep=(atom::Hierarchy(ps[i]).get_parent() ==
               atom::Hierarchy(ps[j]).get_parent());
   if(filter && samep){
    continue;
   } else {
    lpc->add_particle_pair(ParticlePair(ps[i],ps[j]));
   }
  }
 }
 if(lpc->get_number_of_particle_pairs()==0) {return;}
 IMP_NEW(container::MinimumPairRestraint,mpr,(dps,lpc,1));
 m->add_restraint(mpr);
}
/*
void do_allpairs_mindist(Model *m,Particles ps,
 core::SphereDistancePairScore* dps,bool filter)
{
 IMP_NEW(container::ListSingletonContainer,lpc,(ps));
 IMP_NEW(container::ClosePairContainer,cpc,(lpc,200.0));
 if(filter){
  IMP_NEW(atom::SameResiduePairFilter,f,());
  cpc->add_pair_filter(f);
 }
 IMP_NEW(container::MinimumPairRestraint,mpr,(dps,cpc,1));
 m->add_restraint(mpr);
}
*/
void add_fret_restraint (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b, std::string residues_b,
 double r_value, double kappa, bool use_GFP)
{
 atom::Selection sa=atom::Selection(ha);
 if(use_GFP){
  protein_a=protein_a+"-"+residues_a+"-GFP";
  sa.set_residue_index(65);
 } else {
  if(residues_a=="C") {sa.set_terminus(atom::Selection::C);}
  if(residues_a=="N") {sa.set_terminus(atom::Selection::N);}
 }
 sa.set_molecule(protein_a);
 atom::Selection sb=atom::Selection(hb);
 if(use_GFP){
  protein_b=protein_b+"-"+residues_b+"-GFP";
  sb.set_residue_index(65);
 } else {
  if(residues_b=="C") {sb.set_terminus(atom::Selection::C);}
  if(residues_b=="N") {sb.set_terminus(atom::Selection::N);}
 }
 sb.set_molecule(protein_b);
 Particles p1=sa.get_selected_particles();
 Particles p2=sb.get_selected_particles();
 if(p1.size()==0 || p2.size()==0) {return;}
 FloatRange range=get_range_from_fret_value(r_value);
 core::SphereDistancePairScore* sps=get_sphere_pair_score(range,kappa);
 do_bipartite_mindist(m,p1,p2,sps,false);
}

void add_y2h_restraint (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, IntRange residues_a,
 atom::Hierarchies& hb, std::string protein_b, IntRange residues_b,
 double kappa)
{
 atom::Selection sa=atom::Selection(ha);
 sa.set_molecule(protein_a);
 Ints r_a;
 for(int i=residues_a.first;i<=residues_a.second;++i) r_a.push_back(i);
 sa.set_residue_indexes(r_a);
 atom::Selection sb=atom::Selection(hb);
 sb.set_molecule(protein_b);
 Ints r_b;
 for(int i=residues_b.first;i<=residues_b.second;++i) r_b.push_back(i);
 sb.set_residue_indexes(r_b);
 Particles p1=sa.get_selected_particles();
 Particles p2=sb.get_selected_particles();
 if(p1.size()==0 || p2.size()==0) {return;}
 core::SphereDistancePairScore* sps=get_sphere_pair_score(0.0,kappa);
 do_bipartite_mindist(m,p1,p2,sps);
}

void add_y2h_restraint (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b, std::string residues_b,
 double kappa)
{
 atom::Selection sa=atom::Selection(ha);
 sa.set_molecule(protein_a);
 if(residues_a=="C") {sa.set_terminus(atom::Selection::C);}
 if(residues_a=="N") {sa.set_terminus(atom::Selection::N);}
 atom::Selection sb=atom::Selection(hb);
 sb.set_molecule(protein_b);
 if(residues_b=="C") {sb.set_terminus(atom::Selection::C);}
 if(residues_b=="N") {sb.set_terminus(atom::Selection::N);}
 Particles p1=sa.get_selected_particles();
 Particles p2=sb.get_selected_particles();
 if(p1.size()==0 || p2.size()==0) {return;}
 core::SphereDistancePairScore* sps=get_sphere_pair_score(0.0,kappa);
 do_bipartite_mindist(m,p1,p2,sps);
}

void add_y2h_restraint (Model *m,
 const atom::Hierarchy& ha, std::string protein_a,    IntRange residues_a,
 atom::Hierarchies& hb, std::string protein_b, std::string residues_b,
 double kappa)
{
 atom::Selection sa=atom::Selection(ha);
 sa.set_molecule(protein_a);
 Ints r_a;
 for(int i=residues_a.first;i<=residues_a.second;++i) r_a.push_back(i);
 sa.set_residue_indexes(r_a);
 atom::Selection sb=atom::Selection(hb);
 sb.set_molecule(protein_b);
 if(residues_b=="C") {sb.set_terminus(atom::Selection::C);}
 if(residues_b=="N") {sb.set_terminus(atom::Selection::N);}
 Particles p1=sa.get_selected_particles();
 Particles p2=sb.get_selected_particles();
 if(p1.size()==0 || p2.size()==0) {return;}
 core::SphereDistancePairScore* sps=get_sphere_pair_score(0.0,kappa);
 do_bipartite_mindist(m,p1,p2,sps);
}

void add_y2h_restraint (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b,    IntRange residues_b,
 double kappa)
{
 atom::Selection sa=atom::Selection(ha);
 sa.set_molecule(protein_a);
 if(residues_a=="C") {sa.set_terminus(atom::Selection::C);}
 if(residues_a=="N") {sa.set_terminus(atom::Selection::N);}
 atom::Selection sb=atom::Selection(hb);
 sb.set_molecule(protein_b);
 Ints r_b;
 for(int i=residues_b.first;i<=residues_b.second;++i) r_b.push_back(i);
 sb.set_residue_indexes(r_b);
 Particles p1=sa.get_selected_particles();
 Particles p2=sb.get_selected_particles();
 if(p1.size()==0 || p2.size()==0) {return;}
 core::SphereDistancePairScore* sps=get_sphere_pair_score(0.0,kappa);
 do_bipartite_mindist(m,p1,p2,sps);
}

void add_link (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b, IntRange residues_b,
 double kappa)
{
 atom::Hierarchies hs=ha.get_children();
 std::vector<unsigned int> index_a,index_b;
 for(unsigned int i=0;i<hs.size();++i){
  if(hs[i]->get_name()==protein_a) {index_a.push_back(i);}
  if(hs[i]->get_name()==protein_b) {index_b.push_back(i);}
 }
 if(index_a.size()!=index_b.size() || index_a.size()==0){return;}
 for(unsigned int i=0;i<index_a.size();++i){
  atom::Hierarchies hhb;
  for(unsigned int j=0;j<hb.size();++j){
   hhb.push_back(hb[j].get_children()[index_b[i]]);
  }
  add_y2h_restraint(m,hs[index_a[i]],protein_a,residues_a,
   hhb,protein_b,residues_b,kappa);
 }
}

void add_link (Model *m,
 const atom::Hierarchy& ha, std::string protein_a, std::string residues_a,
 atom::Hierarchies& hb, std::string protein_b, std::string residues_b,
 double kappa)
{
 atom::Hierarchies hs=ha.get_children();
 std::vector<unsigned int> index_a,index_b;
 for(unsigned int i=0;i<hs.size();++i){
  if(hs[i]->get_name()==protein_a) {index_a.push_back(i);}
  if(hs[i]->get_name()==protein_b) {index_b.push_back(i);}
 }
 if(index_a.size()!=index_b.size() || index_a.size()==0){return;}
 for(unsigned int i=0;i<index_a.size();++i){
  atom::Hierarchies hhb;
  for(unsigned int j=0;j<hb.size();++j){
   hhb.push_back(hb[j].get_children()[index_b[i]]);
  }
  add_y2h_restraint(m,hs[index_a[i]],protein_a,residues_a,
   hhb,protein_b,residues_b,kappa);
 }
}

void add_symmetry_restraint (Model *m,
 atom::Hierarchies& hs,algebra::Transformation3Ds transformations)
{
 Particles ps0=atom::get_leaves(hs[0]);
 for(unsigned int i=1;i<transformations.size();++i){
  IMP_NEW(core::TransformationSymmetry,sm,(transformations[i]));
  Particles ps1=atom::get_leaves(hs[i]);
  for(unsigned int j=0;j<ps1.size();++j){
   core::Reference::setup_particle(ps1[j],ps0[j]);
  }
  IMP_NEW(container::ListSingletonContainer,lc,(ps1));
  IMP_NEW(container::SingletonsConstraint,c,(sm,NULL,lc));
  m->add_score_state(c);
 }
}

void add_layer_restraint(Model *m, container::ListSingletonContainer *lsc,
 FloatRange range, double kappa)
{
 IMP_NEW(core::HarmonicWell,hw,(range,kappa));
 IMP_NEW(core::AttributeSingletonScore,asc,(hw,FloatKey("z")));
 IMP_NEW(container::SingletonsRestraint,sr,(asc, lsc));
 m->add_restraint(sr);
}

void add_tilt (Model *m, const atom::Hierarchy& h,
 std::string name, double tilt, double kappa)
{
 std::list<core::RigidBody> rbs;
 std::list<core::RigidBody>::iterator iit;

 atom::Selection s=atom::Selection(h);
 s.set_molecule(name);
 Particles ps=s.get_selected_particles();
 for(unsigned int i=0;i<ps.size();++i){
  if(core::RigidMember::particle_is_instance(ps[i])){
   rbs.push_back(core::RigidMember(ps[i]).get_rigid_body());
  }
 }
 rbs.unique();
 for (iit = rbs.begin(); iit != rbs.end(); iit++){
  add_tilt_restraint(m,*iit,FloatRange(0.0,tilt),kappa);
 }
}

IMPMEMBRANE_END_NAMESPACE
