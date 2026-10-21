#!/usr/bin/env python3
"""Measure protocol-v1 UE captures without reproducing the engine simulation."""

import argparse
import csv
import hashlib
import json
import math
from pathlib import Path


def crossing(rows, channel, initial, final, fraction):
    """First crossing, interpolated between samples; None means not reached."""
    if initial == final:
        return None
    previous_progress = 0.0
    previous_time = float(rows[0]["time_s"])
    for row in rows[1:]:
        progress = (float(row[channel]) - initial) / (final - initial)
        time = float(row["time_s"])
        if progress >= fraction:
            return previous_time + (time - previous_time) * (
                fraction - previous_progress
            ) / (progress - previous_progress)
        previous_progress, previous_time = progress, time
    return None


def summarize(path):
    with path.open(encoding="utf-8-sig", newline="") as handle:
        rows = list(csv.DictReader(handle))
    scenario, rate = path.stem.rsplit("_", 1)
    hz = int(rate.removesuffix("Hz"))
    if len(rows) < 2 or float(rows[0]["time_s"]) != 0:
        raise ValueError("Missing initial state: " + str(path))
    previous_time = -1.0
    for index, row in enumerate(rows):
        if not all(math.isfinite(float(value)) for value in row.values()):
            raise ValueError("Non-finite capture: " + str(path))
        time = float(row["time_s"])
        if time <= previous_time or abs(time - index / hz) > 1e-7:
            raise ValueError("Unexpected sample timing: " + str(path))
        previous_time = time

    # Endpoints come from the protocol, never from the truncated last sample.
    heat = 3500.0 / 6000.0
    idle_torque = 50.0 + 900.0 * 0.005
    lift_spool = idle_torque / (idle_torque + 400.0) - 0.001
    endpoints = {
        "boost_rise": {"spool": 1.0, "legacy_pressure": 1.0},
        "throttle_lift": {"spool": lift_spool, "legacy_pressure": 0.0},
        "heat_rise": {"legacy_heat": heat},
        "heat_cool": {"legacy_heat": 0.0},
        "fuel_decay": {"legacy_fuel_buffer": 0.0},
    }.get(scenario, {})
    summaries = []
    for channel in ("spool", "legacy_pressure", "legacy_heat", "legacy_fuel_buffer", "effective_torque", "backfire_intensity", "rpm"):
        values = [float(row[channel]) for row in rows]
        initial, last = values[0], values[-1]
        final = endpoints.get(channel)
        record = {
            "scenario": scenario, "hz": hz, "channel": channel,
            "initial": initial, "last": last, "minimum": min(values),
            "maximum": max(values), "reference_final": final,
            "final_residual": None if final is None else last - final,
        }
        for label, fraction in (("t10_s", 0.1), ("t63_s", 1 - math.exp(-1)), ("t90_s", 0.9), ("t95_s", 0.95)):
            record[label] = None if final is None else crossing(rows, channel, initial, final, fraction)
        summaries.append(record)
    return summaries


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("capture_dir", type=Path)
    parser.add_argument("--output-dir", type=Path, required=True)
    args = parser.parse_args()
    captures = sorted(args.capture_dir.glob("*_?*Hz.csv"))
    if len(captures) != 30:
        parser.error("Protocol v1 expects 30 scenario/rate captures, got %d" % len(captures))
    summaries = [record for path in captures for record in summarize(path)]
    expected_scenarios = {"boost_rise", "throttle_lift", "antilag_lift", "spark_cut", "heat_rise", "heat_cool", "fuel_decay", "backfire_impulse", "free_rev", "rev_limiter"}
    actual = {(record["scenario"], record["hz"]) for record in summaries}
    if actual != {(scenario, hz) for scenario in expected_scenarios for hz in (60, 120, 240)}:
        parser.error("Capture names/rates do not match protocol v1")
    args.output_dir.mkdir(parents=True, exist_ok=True)
    with (args.output_dir / "summary.csv").open("w", encoding="utf-8", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(summaries[0]))
        writer.writeheader()
        writer.writerows(summaries)
    root = Path(__file__).resolve().parent.parent
    sources = [
        "Source/KinetiForge/Private/VehicleEngineComponent.cpp",
        "Source/KinetiForge/Public/VehicleDrivetrainStructs.h",
        "Source/KinetiForge/Private/Tests/VehicleEngineLegacyTests.cpp",
    ]
    provenance = {
        "protocol": "EngineLegacyBaseline/v1",
        "note": "Source hashes describe files at analysis time; retain the UE run log and build revision with captures.",
        "sources_sha256": {name: hashlib.sha256((root / name).read_bytes()).hexdigest() for name in sources},
        "captures_sha256": {path.name: hashlib.sha256(path.read_bytes()).hexdigest() for path in captures},
        "missing_metric": "Empty CSV field means no reference endpoint or threshold not reached; see protocol.",
    }
    (args.output_dir / "provenance.json").write_text(json.dumps(provenance, indent=2) + "\n", encoding="utf-8")
    print("Measured %d captures; wrote %s" % (len(captures), args.output_dir / "summary.csv"))
    for row in summaries:
        if row["t95_s"] is not None:
            print("{scenario:16s} {hz:3d} Hz {channel:20s} t95={t95_s:.6f} s".format(**row))


if __name__ == "__main__":
    main()
