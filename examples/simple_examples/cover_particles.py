import IMP
import IMP.core
import IMP.modeller

m= IMP.Model()
prot= IMP.modeller.read_pdb('examples/simple_examples/single_protein.pdb', m)
res= IMP.core.get_by_type(prot, IMP.core.MolecularHierarchyDecorator.RESIDUE)
rc= IMP.core.ListSingletonContainer(res)
for p in res:
    IMP.core.XYZRDecorator.create(p)
mtr=IMP.core.MolecularHierarchyDecorator.get_traits()
pr= IMP.core.ChildrenParticleRefiner(mtr)
IMP.core.setup_covers(m, rc, pr)
m.evaluate(False)
