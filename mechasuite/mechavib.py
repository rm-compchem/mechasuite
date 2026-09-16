"""Command-line interface for vibrational rotational-character analysis."""

import argparse
import json
import os

from mechasuite.vibrational import analyze_vibrations


def _write_report(path, result):
    with open(path, "w", encoding="utf-8") as report:
        report.write("# Mode Frequency(cm-1) Total_Rotation(%)\n")
        for mode, (frequency, rotation) in enumerate(zip(
                result.frequencies, result.total_rotational_percentages), 1):
            report.write(f"{mode:6d} {frequency:16.4f} {rotation:20.6f}\n")


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="Calculate vibrational rotational character from ORCA or VASP output."
    )
    parser.add_argument("output", help="ORCA job.out or VASP OUTCAR")
    parser.add_argument("-x", "--xyz", help="ORCA XYZ geometry; defaults to job.xyz")
    parser.add_argument("-p", "--poscar", default="POSCAR", help="VASP POSCAR")
    parser.add_argument(
        "--format", choices=["auto", "orca", "vasp"], default="auto",
        help="Input format (default: auto)",
    )
    parser.add_argument(
        "-c", "--cutoff", type=float, default=1.2,
        help="Covalent-radius bond multiplier (default: 1.2)",
    )
    parser.add_argument(
        "-o", "--output-json", default="rotational_character.json",
        help="Percentage-only JSON output",
    )
    parser.add_argument("--report", help="Optional human-readable table output")
    args = parser.parse_args(argv)

    result = analyze_vibrations(
        args.output,
        xyz_path=args.xyz,
        poscar_path=args.poscar,
        format=args.format,
        cutoff=args.cutoff,
    )
    payload = {
        "source": os.path.abspath(args.output),
        "frequency_unit": "cm-1",
        "frequencies": result.frequencies,
        "rotational_percentages": result.total_rotational_percentages,
        "dihedrals": [list(dihedral) for dihedral in result.dihedrals],
    }
    with open(args.output_json, "w", encoding="utf-8") as output:
        json.dump(payload, output, indent=2)
    if args.report:
        _write_report(args.report, result)

    print(f"Analyzed {len(result.frequencies)} normal modes")
    print(f"Found {len(result.dihedrals)} unique bonded dihedrals")
    print(f"Wrote {args.output_json}")
    if args.report:
        print(f"Wrote {args.report}")
    return 0


if __name__ == "__main__":
    main()