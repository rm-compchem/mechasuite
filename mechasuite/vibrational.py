"""Vibrational mode analysis for rotational character.

Normal modes and geometry-derived quantities in this module are transient.
Only total rotational percentages are copied to Freq.rotation.
"""

import math
import re

import numpy as np


COVALENT_RADII = {
    "H": 0.31, "HE": 0.28, "LI": 1.28, "BE": 0.96, "B": 0.84,
    "C": 0.76, "N": 0.71, "O": 0.66, "F": 0.57, "NE": 0.58,
    "NA": 1.66, "MG": 1.41, "AL": 1.21, "SI": 1.11, "P": 1.07,
    "S": 1.05, "CL": 1.02, "AR": 1.06, "K": 2.03, "CA": 1.76,
    "SC": 1.70, "TI": 1.60, "V": 1.53, "CR": 1.39, "MN": 1.39,
    "FE": 1.32, "CO": 1.26, "NI": 1.24, "CU": 1.32, "ZN": 1.22,
    "BR": 1.20, "PD": 1.39, "AG": 1.45, "PT": 1.36, "AU": 1.36,
}

ATOMIC_MASSES = {
    "H": 1.008, "C": 12.011, "N": 14.007, "O": 15.999,
    "F": 18.998, "P": 30.974, "S": 32.06, "CL": 35.45,
    "CU": 63.546, "ZN": 65.38, "BR": 79.904, "PD": 106.42,
    "AG": 107.868, "PT": 195.084, "AU": 196.967,
}


class NormalModeData:
    """Parsed normal modes. This object is not saved in a mechanism file."""

    def __init__(self, frequencies, modes, equilibrium_geometry, elements):
        self.frequencies = frequencies
        self.modes = modes
        self.equilibrium_geometry = equilibrium_geometry
        self.elements = elements


class RotationalAnalysis:
    """Analysis output aligned with the frequencies."""

    def __init__(self, frequencies, total_rotational_percentages,
                 dihedrals, dihedral_percentages):
        self.frequencies = frequencies
        self.total_rotational_percentages = total_rotational_percentages
        self.dihedrals = dihedrals
        self.dihedral_percentages = dihedral_percentages


def read_xyz(path):
    xyz_path = str(path)
    with open(xyz_path, encoding="utf-8", errors="replace") as xyz_file:
        lines = [line.strip() for line in xyz_file if line.strip()]
    if len(lines) < 2:
        raise ValueError(f"Could not parse XYZ geometry from {xyz_path}")

    try:
        atom_count = int(lines[0])
    except ValueError as exc:
        raise ValueError(f"Invalid XYZ atom count in {xyz_path}") from exc

    elements = []
    coordinates = []
    for line in lines[2:2 + atom_count]:
        fields = line.split()
        if len(fields) < 4:
            raise ValueError(f"Invalid XYZ atom line in {xyz_path}: {line}")
        elements.append(fields[0])
        coordinates.append([float(value) for value in fields[1:4]])

    if len(coordinates) != atom_count:
        raise ValueError(
            f"Expected {atom_count} atoms in {xyz_path}, found {len(coordinates)}"
        )
    return elements, np.asarray(coordinates, dtype=float)


def read_orca_normal_modes(output_path, xyz_path):
    """Read ORCA frequencies and Cartesian normal modes without writing files."""
    output = str(output_path)
    with open(output, encoding="utf-8", errors="replace") as output_file:
        text = output_file.read()
    frequency_pattern = re.compile(
        r"^\s*(\d+)\s*:\s*([-+]?(?:\d+\.\d*|\.\d+|\d+)(?:[Ee][-+]?\d+)?)\s*cm(?:\*\*)?-1",
        re.MULTILINE,
    )
    frequency_entries = [
        (int(match.group(1)), float(match.group(2)))
        for match in frequency_pattern.finditer(text)
        if int(match.group(1)) >= 6 and abs(float(match.group(2))) > 1e-10
    ]
    if not frequency_entries:
        raise ValueError(f"No non-zero vibrational frequencies found in {output}")

    selected_modes = {mode_id for mode_id, _ in frequency_entries}
    frequencies = [value for _, value in frequency_entries]
    start = text.find("NORMAL MODES")
    if start == -1:
        raise ValueError(f"Could not find NORMAL MODES in {output}")
    end = text.find("IR SPECTRUM", start)
    block = text[start:] if end == -1 else text[start:end]

    mode_values = {mode_id: [] for mode_id in selected_modes}
    columns = None
    in_table = False
    for line in block.splitlines():
        if "These modes are the Cartesian displacements" in line:
            in_table = True
            continue
        if not in_table or not line.strip():
            continue
        tokens = line.split()
        if tokens and all(token.isdigit() for token in tokens) and 1 <= len(tokens) <= 6:
            columns = [int(token) for token in tokens]
            continue
        if columns is None or len(tokens) != len(columns) + 1:
            continue
        try:
            values = [float(token) for token in tokens[1:]]
        except ValueError:
            continue
        for mode_id, value in zip(columns, values):
            if mode_id in mode_values:
                mode_values[mode_id].append(value)

    elements, geometry = read_xyz(xyz_path)
    expected_values = 3 * len(elements)
    modes = []
    for mode_id in sorted(selected_modes):
        values = mode_values[mode_id]
        if len(values) != expected_values:
            raise ValueError(
                f"Mode {mode_id} has {len(values)} Cartesian values; "
                f"expected {expected_values}"
            )
        modes.append(np.asarray(values, dtype=float).reshape(len(elements), 3))

    return NormalModeData(
        frequencies=frequencies,
        modes=np.asarray(modes, dtype=float),
        equilibrium_geometry=geometry,
        elements=elements,
    )


def read_poscar_geometry(path):
    """Read POSCAR elements, Cartesian coordinates, and active atom indices."""
    poscar = str(path)
    with open(poscar, encoding="utf-8", errors="replace") as poscar_file:
        lines = [line.strip() for line in poscar_file if line.strip()]
    if len(lines) < 8:
        raise ValueError(f"Could not parse POSCAR geometry from {poscar}")

    try:
        scale = float(lines[1].split()[0])
        lattice = np.asarray([[float(value) for value in lines[index].split()[:3]]
                              for index in range(2, 5)]) * scale
    except (ValueError, IndexError) as exc:
        raise ValueError(f"Invalid POSCAR lattice in {poscar}") from exc

    element_index = 5
    element_names = lines[element_index].split()
    if all(token.isdigit() for token in element_names):
        element_names = ["X"] * len(element_names)
    else:
        element_index += 1
    try:
        counts = [int(value) for value in lines[element_index].split()]
    except ValueError as exc:
        raise ValueError(f"Invalid POSCAR atom counts in {poscar}") from exc
    if len(element_names) != len(counts):
        raise ValueError(f"POSCAR element and count columns differ in {poscar}")

    elements = [element for element, count in zip(element_names, counts) for _ in range(count)]
    coordinate_index = element_index + 1
    selective = lines[coordinate_index].lower().startswith("s")
    if selective:
        coordinate_index += 1
    direct = lines[coordinate_index].lower().startswith("d")
    coordinate_index += 1

    coordinates = []
    active_atoms = set()
    for atom_index in range(sum(counts)):
        fields = lines[coordinate_index + atom_index].split()
        if len(fields) < 3:
            raise ValueError(f"Invalid POSCAR coordinate line in {poscar}")
        coordinate = np.asarray([float(value) for value in fields[:3]], dtype=float)
        if direct:
            coordinate = coordinate @ lattice
        coordinates.append(coordinate)
        if not selective or any(flag.upper() == "T" for flag in fields[3:6]):
            active_atoms.add(atom_index)

    return elements, np.asarray(coordinates), active_atoms


def read_vasp_normal_modes(output_path, poscar_path="POSCAR"):
    """Read VASP frequencies and eigenvectors, expanding frozen atoms."""
    elements, geometry, active_atoms = read_poscar_geometry(poscar_path)
    output = str(output_path)
    frequencies = []
    modes = []
    current_mode = None
    current_atom_index = 0
    inside_modes = False
    frequency_pattern = re.compile(r"([-+]?\d+(?:\.\d*)?|[-+]?\.\d+)\s*cm-1")

    with open(output, encoding="utf-8", errors="replace") as output_file:
        output_lines = output_file.readlines()

    for line in output_lines:
        if "Eigenvectors and eigenvalues" in line:
            if inside_modes:
                break
            inside_modes = True
            continue
        if not inside_modes:
            continue
        if "THz" in line and "cm-1" in line:
            if current_mode is not None:
                modes.append(current_mode)
            match = frequency_pattern.search(line)
            if match is None:
                continue
            frequency = float(match.group(1))
            if "f/i" in line.lower():
                frequency = -abs(frequency)
            frequencies.append(frequency)
            current_mode = []
            current_atom_index = 0
            continue
        if current_mode is None:
            continue
        fields = line.split()
        if len(fields) < 6:
            continue
        try:
            values = [float(value) for value in fields[-6:]]
        except ValueError:
            continue
        if active_atoms == set(range(len(elements))) or current_atom_index in active_atoms:
            current_mode.append(values[-3:])
        current_atom_index += 1

    if current_mode is not None:
        modes.append(current_mode)
    if len(frequencies) != len(modes):
        raise ValueError(
            f"Found {len(frequencies)} VASP frequencies but {len(modes)} mode blocks"
        )

    full_modes = []
    active_list = sorted(active_atoms)
    for mode in modes:
        values = np.asarray(mode, dtype=float)
        if len(values) == len(elements):
            full_modes.append(values)
            continue
        if len(values) != len(active_list):
            raise ValueError(
                f"VASP mode has {len(values)} vectors; expected "
                f"{len(elements)} or {len(active_list)}"
            )
        full = np.zeros((len(elements), 3), dtype=float)
        full[active_list] = values
        full_modes.append(full)

    return NormalModeData(
        frequencies=frequencies,
        modes=np.asarray(full_modes, dtype=float),
        equilibrium_geometry=geometry,
        elements=elements,
    )


def _dihedral_angle(points):
    b0 = points[1] - points[0]
    b1 = points[2] - points[1]
    b2 = points[3] - points[2]
    b1_length = np.linalg.norm(b1)
    if b1_length == 0:
        raise ValueError("Cannot calculate a dihedral with a zero-length central bond")
    n1 = np.cross(b0, b1)
    n2 = np.cross(b1, b2)
    m1 = np.cross(n1, b1)
    return math.degrees(math.atan2(np.dot(m1, n2) / b1_length, np.dot(n1, n2)))


def find_bonded_dihedrals(coordinates, elements, tolerance=1.2):
    """Return unique bonded A-B-C-D paths using covalent radii."""
    coordinates = np.asarray(coordinates, dtype=float)
    adjacency = {index: [] for index in range(len(coordinates))}
    for first in range(len(coordinates)):
        for second in range(first + 1, len(coordinates)):
            first_element = elements[first].upper()
            second_element = elements[second].upper()
            if first_element not in COVALENT_RADII or second_element not in COVALENT_RADII:
                continue
            cutoff = (COVALENT_RADII[first_element] + COVALENT_RADII[second_element]) * tolerance
            if np.linalg.norm(coordinates[first] - coordinates[second]) <= cutoff:
                adjacency[first].append(second)
                adjacency[second].append(first)

    result = []
    seen = set()
    for second in range(len(coordinates)):
        for third in adjacency[second]:
            if second >= third:
                continue
            for first in adjacency[second]:
                if first == third:
                    continue
                for fourth in adjacency[third]:
                    if fourth in (second, first):
                        continue
                    path = (first, second, third, fourth) if first < fourth else (fourth, third, second, first)
                    if path not in seen:
                        seen.add(path)
                        _dihedral_angle(coordinates[list(path)])
                        result.append(path)
    return result


def analyze_mode(geometry, mode, dihedrals, elements, step=0.01):
    """Return total rotational KED percentage and per-dihedral percentages."""
    geometry = np.asarray(geometry, dtype=float)
    mode = np.asarray(mode, dtype=float)
    if geometry.shape != mode.shape or geometry.ndim != 2 or geometry.shape[1] != 3:
        raise ValueError("Geometry and mode must both have shape (number_of_atoms, 3)")
    if len(elements) != len(geometry):
        raise ValueError("Element and coordinate counts do not match")

    masses = np.asarray([ATOMIC_MASSES.get(element.upper(), 1.0) for element in elements])
    displacement = step * mode
    atom_ked = masses * np.sum(displacement * displacement, axis=1)
    total_ked = float(np.sum(atom_ked))
    if total_ked == 0.0:
        return 0.0, {dihedral: 0.0 for dihedral in dihedrals}

    axis_atom_ked: dict[tuple[int, int], dict[int, float]] = {}
    dihedral_ked = {}
    for first, second, third, fourth in dihedrals:
        axis = tuple(sorted((second, third)))
        axis_atom_ked.setdefault(axis, {})
        axis_vector = geometry[third] - geometry[second]
        reverse_axis = -axis_vector
        first_radius = geometry[first] - geometry[second]
        fourth_radius = geometry[fourth] - geometry[third]
        tangent_first = np.cross(axis_vector, first_radius)
        tangent_fourth = np.cross(reverse_axis, fourth_radius)

        contributions = []
        for atom, tangent in ((first, tangent_first), (fourth, tangent_fourth)):
            tangent_length = np.linalg.norm(tangent)
            if tangent_length <= 1e-12:
                contribution = 0.0
            else:
                tangent_unit = tangent / tangent_length
                projected = float(np.dot(displacement[atom], tangent_unit))
                contribution = min(float(masses[atom] * projected * projected), float(atom_ked[atom]))
            contributions.append(contribution)
            axis_atom_ked[axis][atom] = max(axis_atom_ked[axis].get(atom, 0.0), contribution)
        dihedral_ked[(first, second, third, fourth)] = sum(contributions)

    total_rotational_ked = min(
        total_ked,
        sum(sum(atom_values.values()) for atom_values in axis_atom_ked.values()),
    )
    total_percentage = 100.0 * total_rotational_ked / total_ked
    dihedral_percentages = {
        dihedral: 100.0 * value / total_ked
        for dihedral, value in dihedral_ked.items()
    }
    return total_percentage, dihedral_percentages


def analyze_normal_modes(data, dihedrals=None, cutoff=1.2):
    if len(data.frequencies) != len(data.modes):
        raise ValueError("Frequency and normal-mode counts do not match")
    if dihedrals is None:
        selected_dihedrals = find_bonded_dihedrals(
            data.equilibrium_geometry, data.elements, cutoff
        )
    else:
        selected_dihedrals = list(dihedrals)
    totals = []
    per_dihedral = []
    for mode in data.modes:
        total, contributions = analyze_mode(
            data.equilibrium_geometry, mode, selected_dihedrals, data.elements
        )
        totals.append(total)
        per_dihedral.append(contributions)
    return RotationalAnalysis(
        frequencies=list(data.frequencies),
        total_rotational_percentages=totals,
        dihedrals=selected_dihedrals,
        dihedral_percentages=per_dihedral,
    )


def analyze_vibrations(output_path, xyz_path=None, format="auto",
                       poscar_path="POSCAR", dihedrals=None, cutoff=1.2):
    """Parse and analyze a vibrational calculation without persisting modes."""
    output = str(output_path)
    selected_format = format.lower()
    if selected_format == "auto":
        with open(output, encoding="utf-8", errors="replace") as output_file:
            text = output_file.read()
        if "NORMAL MODES" in text and "VIBRATIONAL FREQUENCIES" in text:
            selected_format = "orca"
        else:
            selected_format = "vasp"
    if selected_format == "orca":
        if xyz_path is None:
            xyz_path = output.with_name("job.xyz")
        data = read_orca_normal_modes(output, xyz_path)
    elif selected_format == "vasp":
        data = read_vasp_normal_modes(output, poscar_path)
    else:
        raise ValueError(f"Unsupported vibrational format {format!r}")
    return analyze_normal_modes(data, dihedrals=dihedrals, cutoff=cutoff)


def align_rotational_percentages(frequencies, analyzed_frequencies, percentages,
                                 match_tolerance=0.5,
                                 zero_frequency_tolerance=0.5):
    """Align analyzed percentages, assigning zero to excluded near-zero modes."""
    if len(analyzed_frequencies) != len(percentages):
        raise ValueError(
            "Analyzed frequency and rotational-percentage counts differ "
            f"({len(analyzed_frequencies)} != {len(percentages)})"
        )

    remaining = list(zip(analyzed_frequencies, percentages))
    aligned = []
    for frequency in frequencies:
        if remaining:
            index = min(
                range(len(remaining)),
                key=lambda candidate: abs(remaining[candidate][0] - frequency),
            )
            source_frequency, percentage = remaining[index]
        else:
            index = None
            source_frequency = None
            percentage = None

        if index is not None and abs(source_frequency - frequency) <= match_tolerance:
            remaining.pop(index)
            aligned.append(float(percentage))
        elif not remaining and abs(frequency) <= zero_frequency_tolerance:
            aligned.append(0.0)
        elif index is not None and abs(frequency) <= zero_frequency_tolerance:
            aligned.append(0.0)
        else:
            raise ValueError(
                f"Could not uniquely match rotational character for {frequency} cm-1"
            )

    if remaining:
        raise ValueError(
            f"{len(remaining)} analyzed frequencies could not be aligned with the item"
        )
    return aligned


def apply_rotational_percentages(item, percentages):
    """Persist only mode-aligned percentages on a MechaSuite ``Itm`` object."""
    frequencies = item.freqs.vibs
    if len(frequencies) != len(percentages):
        raise ValueError(
            "Cannot store rotational character: frequency and percentage counts "
            f"differ ({len(frequencies)} != {len(percentages)})"
        )
    item.freqs.rotation = [float(value) for value in percentages]