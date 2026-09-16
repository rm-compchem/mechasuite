import unittest
import json
import os
from types import SimpleNamespace
from tempfile import TemporaryDirectory

import numpy as np

from mechasuite.vibrational import (
    NormalModeData,
    analyze_normal_modes,
    align_rotational_percentages,
    apply_rotational_percentages,
    find_bonded_dihedrals,
    read_vasp_normal_modes,
)
from mechasuite.mechavib import main as mechavib_main


class VibrationalAnalysisTests(unittest.TestCase):
    def test_zero_motion_produces_zero_rotation(self):
        data = NormalModeData(
            frequencies=[100.0],
            modes=np.zeros((1, 4, 3)),
            equilibrium_geometry=np.array([
                [0.0, 0.0, 0.0],
                [1.4, 0.0, 0.0],
                [2.8, 0.0, 0.0],
                [4.2, 0.0, 0.0],
            ]),
            elements=["C", "C", "C", "C"],
        )

        result = analyze_normal_modes(data, dihedrals=[(0, 1, 2, 3)])

        self.assertEqual(result.total_rotational_percentages, [0.0])
        self.assertEqual(result.frequencies, [100.0])

    def test_bonded_paths_are_unique_under_reversal(self):
        geometry = np.array([
            [0.0, 0.0, 0.0],
            [1.4, 0.0, 0.0],
            [2.8, 0.0, 0.0],
            [4.2, 0.0, 0.0],
        ])

        paths = find_bonded_dihedrals(geometry, ["C"] * 4)

        self.assertEqual(paths, [(0, 1, 2, 3)])

    def test_persistence_requires_frequency_alignment(self):
        item = SimpleNamespace(freqs=SimpleNamespace(vibs=[100.0]))

        with self.assertRaises(ValueError):
            apply_rotational_percentages(item, [10.0, 20.0])

        apply_rotational_percentages(item, [10.0])
        self.assertEqual(item.freqs.rotation, [10.0])

    def test_alignment_fills_excluded_zero_modes(self):
        aligned = align_rotational_percentages(
            [100.0, 0.0, 0.0],
            [100.0],
            [25.0],
        )

        self.assertEqual(aligned, [25.0, 0.0, 0.0])

    def test_alignment_rejects_unmatched_nonzero_modes(self):
        with self.assertRaises(ValueError):
            align_rotational_percentages([100.0, 50.0], [100.0], [25.0])

    def test_alignment_handles_close_frequency_pair(self):
        aligned = align_rotational_percentages(
            [1655.98, 1655.49],
            [1655.49, 1655.98],
            [10.0, 20.0],
        )

        self.assertEqual(aligned, [20.0, 10.0])

    def test_vasp_expands_frozen_atoms(self):
        poscar = """C4
1.0
10 0 0
0 10 0
0 0 10
C
4
Selective dynamics
Cartesian
0 0 0 T T T
1.4 0 0 F F F
2.8 0 0 T T T
4.2 0 0 T T T
"""
        outcar = """ Eigenvectors and eigenvalues of the dynamical matrix
  1 f 1.000 THz 33.356 cm-1
  1 C 0 0 0 1 2 3
  2 C 0 0 0 4 5 6
  3 C 0 0 0 7 8 9
  4 C 0 0 0 10 11 12
"""
        with TemporaryDirectory() as directory:
            poscar_path = os.path.join(directory, "POSCAR")
            outcar_path = os.path.join(directory, "OUTCAR")
            with open(poscar_path, "w") as poscar_file:
                poscar_file.write(poscar)
            with open(outcar_path, "w") as outcar_file:
                outcar_file.write(outcar)

            result = read_vasp_normal_modes(outcar_path, poscar_path)

        self.assertEqual(result.frequencies, [33.356])
        np.testing.assert_array_equal(
            result.modes[0],
            np.array([[1, 2, 3], [0, 0, 0], [7, 8, 9], [10, 11, 12]], dtype=float),
        )

    def test_cli_writes_percentages_without_modes(self):
        xyz = """4
chain
C 0.0 0.0 0.0
C 1.4 0.0 0.0
C 2.8 0.0 0.0
C 4.2 0.0 0.0
"""
        normal_modes = """VIBRATIONAL FREQUENCIES
 6: 100.0 cm-1
NORMAL MODES
 These modes are the Cartesian displacements
 6
 1 0.0
 2 0.0
 3 0.0
 4 0.0
 5 0.0
 6 0.0
 7 0.0
 8 0.0
 9 0.0
 10 0.0
 11 0.0
 12 0.0
IR SPECTRUM
"""
        with TemporaryDirectory() as directory:
            xyz_path = os.path.join(directory, "job.xyz")
            output_path = os.path.join(directory, "job.out")
            json_path = os.path.join(directory, "rotation.json")
            with open(xyz_path, "w") as xyz_file:
                xyz_file.write(xyz)
            with open(output_path, "w") as output_file:
                output_file.write(normal_modes)

            exit_code = mechavib_main([
                str(output_path), "-x", str(xyz_path), "--format", "orca",
                "-o", str(json_path),
            ])
            with open(json_path) as json_file:
                payload = json.load(json_file)

        self.assertEqual(exit_code, 0)
        self.assertEqual(payload["frequencies"], [100.0])
        self.assertEqual(payload["rotational_percentages"], [0.0])
        self.assertNotIn("modes", payload)


if __name__ == "__main__":
    unittest.main()