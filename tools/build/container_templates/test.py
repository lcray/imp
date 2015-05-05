from __future__ import print_function
import IMP
import IMP.test
import IMP.core
import IMP.container
import math


# This file is generated by the make-container script

class ClassnameContainerTest(IMP.test.TestCase):

    """Tests for ClassnameContainer related objects"""

    def create_particle(self, m):
        p = m.add_particle("p")
        d = IMP.core.XYZ.setup_particle(m, p)
        d.set_coordinates(IMP.algebra.get_random_vector_in(
            IMP.algebra.get_unit_bounding_box_3d()))
        m.add_attribute(IMP.FloatKey("thekey"), p, d.get_x())
        return p

    def create_particle_pair(self, m):
        p0 = m.add_particle("p0")
        p1 = m.add_particle("p1")
        d0 = IMP.core.XYZ.setup_particle(m, p0)
        d1 = IMP.core.XYZ.setup_particle(m, p1)
        d0.set_coordinates(IMP.algebra.get_random_vector_in(
            IMP.algebra.get_unit_bounding_box_3d()))
        d1.set_coordinates(IMP.algebra.get_random_vector_in(
            IMP.algebra.get_unit_bounding_box_3d()))
        return (p0, p1)

    def create_singleton_score(self):
        uf = IMP.core.Linear(0, 1)
        return IMP.core.AttributeSingletonScore(uf, IMP.FloatKey("thekey"))

    def create_pair_score(self):
        uf = IMP.core.Linear(0, 1)
        return IMP.core.DistancePairScore(uf)

    def create_singleton_restraint(self, m, ps, p):
        return IMP.core.SingletonRestraint(m, ps, p)

    def create_pair_restraint(self, m, ps, p):
        return IMP.core.PairRestraint(m, ps, (p[0], p[1]))

    def test_restraint(self):
        """Test the ClassnamesRestraint"""
        m = IMP.Model()
        gs = self.create_CLASSFUNCTIONNAME_score()
        c = IMP.container.ListClassnameContainer(m)
        f = 0
        for i in range(0, 10):
            p = self.create_FUNCTIONNAME(m)
            f = f + gs.evaluate_index(m, p, None)
            c.add(p)
        r = IMP.container.ClassnamesRestraint(gs, c)
        r.set_was_used(True)
        self.assertAlmostEqual(r.evaluate(False), f, delta=.1 * f)
        self.assertAlmostEqual(r.evaluate(False), f, delta=.1 * f)
        p = self.create_FUNCTIONNAME(m)
        f = f + gs.evaluate_index(m, p, None)
        c.add(p)
        self.assertAlmostEqual(r.evaluate(False), f, delta=.1 * f)
        self.assertAlmostEqual(r.evaluate(False), f, delta=.1 * f)

    def test_srestraint(self):
        """Test the ClassnameRestraint"""
        m = IMP.Model()
        gs = self.create_CLASSFUNCTIONNAME_score()
        p = self.create_FUNCTIONNAME(m)
        f = gs.evaluate_index(m, p, None)
        r = self.create_CLASSFUNCTIONNAME_restraint(m, gs, p)
        r.set_was_used(True)
        self.assertAlmostEqual(r.evaluate(False), f, delta=.1 * f)

    def test_min_restraint(self):
        """Test the MinimumClassnameRestraint"""
        m = IMP.Model()
        c = IMP.container.ListClassnameContainer(m)
        self.assertEqual(c.get_ref_count(), 1)
        for i in range(0, 10):
            c.add(self.create_FUNCTIONNAME(m))
        d = self.create_CLASSFUNCTIONNAME_score()
        self.assertEqual(d.get_ref_count(), 1)
        r = IMP.container.MinimumClassnameRestraint(d, c)
        self.assertEqual(d.get_ref_count(), 2)
        self.assertEqual(c.get_ref_count(), 2)
        r.set_n(4)
        f = r.evaluate(False)
        print(f)
        ms = []
        indexes = c.get_indexes()
        for i in range(0, 10):
            ps = indexes[i]
            cm = d.evaluate_index(m, ps, None)
            ms.append(cm)
        print(ms)
        ms.sort()
        mt = 0
        for i in range(0, 4):
            mt = mt + ms[i]
        print(mt)
        self.assertAlmostEqual(mt, f, delta=.1 * f)

    def test_min_score(self):
        """Test the MinimumClassnameScore"""
        m = IMP.Model()
        s = []
        for i in range(0, 5):
            s.append(IMP._ConstClassnameScore(i))
        ps = IMP.container.MinimumClassnameScore(s, 3)
        p = self.create_FUNCTIONNAME(m)
        ps.set_was_used(True)
        v = ps.evaluate_index(m, p, None)
        self.assertEqual(v, 3)

    def test_set(self):
        """Testing ClassnameContainerSet"""
        IMP.set_log_level(IMP.VERBOSE)
        m = IMP.Model()
        print("hi")
        c = IMP.container.ClassnameContainerSet(m)
        ls = []
        cs = []
        for i in range(0, 3):
            l = IMP.container.ListClassnameContainer(m)
            c.add_CLASSFUNCTIONNAME_container(l)
            for j in range(0, 3):
                t = self.create_FUNCTIONNAME(m)
                l.add(t)
                cs.append(t)
        ret = c.get_contents()
        for pi in cs:
            self.assertIn(pi, ret)
        ret.sort()
        # print ret
        cs.sort()
        # rint cs
        self.assertEqual(len(ret), len(cs))
        for r, c in zip(ret, cs):
            self.assertEqual(r, c)

if __name__ == '__main__':
    IMP.test.main()
