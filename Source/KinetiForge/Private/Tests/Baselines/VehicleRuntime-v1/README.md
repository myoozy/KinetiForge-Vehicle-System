# Vehicle runtime regression snapshots

These read-only CSVs were captured on UE 5.3.2 from the reference code at
`e1d67bf`, with temporary output hooks, before removing the legacy vehicle and
input backends. The six source comparison tests passed during capture. Four
unrelated lifecycle migrations initially failed and were corrected against the
real fixed-runtime path before the final regression run.

Normal tests only read these files. There is no baseline regeneration flag or
executable legacy backend. A future intentional behavior change must review any
new expectations separately; never regenerate them from a failing implementation
and treat the result as validation.

The files contain no header row. Columns are zero-based:

| File | Rows | Columns |
| --- | ---: | --- |
| independent.csv | 36 | engine RPM, axle angular velocity; then left/right wheel angular velocity, suspension length, static sprung mass, chassis-local hub Z |
| solid.csv | 18 | axle angular velocity, left wheel angular velocity, right wheel angular velocity |
| static-contact.csv | 24 | engine RPM, axle angular velocity, chassis Z, chassis vertical velocity; then left/right contact flag, signed normal load, hub X/Y/Z, strut length, wheel angular velocity, impact Z |
| dynamic-contact.csv | 12 | left normal load, right normal load, contact count, summed normal load, same-phase platform impulse X/Y/Z, accumulated platform velocity change Y, platform velocity Y, chassis Z, chassis velocity Z |
| input-compatibility.csv | 18 | five smoothed axes (throttle, brake, clutch, steering, handbrake), five final axes, throttle/brake switch flag |
| input-assist.csv | 3 | same eleven input columns; automatic clutch, EV clutch, auto-hold cases |

Independent and solid-axle tolerances remain 0.01. Static-contact checks retain
0.02 for RPM/axle velocity, 0.15 for chassis motion, 1 for signed load, 0.05 for
wheel velocity/geometry/travel, and exact contact flags. Impact Z also uses 0.05.
Dynamic checks retain per-wheel load tolerance 1, loaded same-phase impulse
component tolerance max(1, 10% of expected impulse magnitude), and accumulated
platform response tolerance max(0.1, 10% of expected response). The remaining
dynamic columns are context recorded during capture, not additional assertions.
Input tolerance remains 1e-5 with exact switch flags.

The redundant old component-vs-hybrid airborne test was retired. Numeric scene
regressions, manual callback checks, ownership positive controls, component
recovery, worker/frozen lanes, 120/1000 Hz asynchronous exit, original Blueprint
assets and contact reaction checks exercise the sole production runtime.

The original capture audit is retained on the development branch; it is not required to run these regression tests.
