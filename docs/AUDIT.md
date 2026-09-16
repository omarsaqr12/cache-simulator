# Repository audit and evidence (2026-09-16)

Baseline `main`: `c11e6ef2b6c724e4e351fc8e3d79b8d790339cb1`. Scope: only `omarsaqr12/cache-simulator`.

## Calibration

This is a trace-driven, direct-mapped cache timing simulator. The central correctness questions are binary-address parsing, power-of-two index/tag mapping, hit/eviction transitions, and defined performance counters. A successful build or screenshots alone are insufficient. The CLI and cache model are sources of truth; the PDF report is historical material.

## Confirmed baseline defects

- `include/cache.h`: `totalCycles` was never initialized; `computeAMAT()` read its indeterminate value. Empty traces also divided by zero.
- `src/main.cpp` and `src/cache_logic.cpp`: `substr(0, size()-2)` silently removed the final address bit; the alternatives also disagreed on 30-bit versus 32-bit addresses. Invalid/short records could throw without a useful line number.
- Configuration accepted zero or non-power-of-two block sizes and did not validate input extraction; `log2`/division/indexing then had invalid assumptions.
- `Makefile`'s old `test` target only piped examples through the CLI without asserting expected results, and used `echo -e` in a shell-dependent way.
- `SourceCodes/` contains historical copies and should not be mistaken for the compiled sources; the duplicate `CacheLogic.cpp` blob matches the previous `src/cache_logic.cpp`.
- `src/gui.py` uses invalid default block size `12`, and GUI execution was not verified.
- The former README described write-through behavior despite no write/access-type modeling, advertised a nonexistent screenshot-like real-time capability, and contained a placeholder clone URL. No license file is present.

## Changes and evidence

- Initialize counters and make zero-request rates deterministic; validate constructor inputs and use explicit 32-bit unsigned tags with defined shifts.
- Parse strict 32-bit binary records; fail with line numbers and nonzero exit codes; preserve trace ordering.
- Route the secondary executable through the same validated parser while preserving `SourceCodes/` untouched.
- Add C++ assertions and black-box CLI regression tests; make `make test` assert outcomes.
- Correct README scope, quickstart, formula, source map and limitations. No feature expansion or new license.

Locally, `make test` compiled with `g++ -std=c++17 -Wall -Wextra -Wpedantic -O2` and passed the C++ known-answer assertions plus 3 Python unittest cases. A separately built compatibility executable accepted the existing `test_data/test1.txt` and produced instruction AMAT 27.00 and data AMAT 35.33 cycles under the README configuration. The GUI was not run. A fresh remote checkout and GitHub Actions checks have not been independently confirmed.

## File coverage

Reviewed all tracked text-source categories: README, Makefile, `.gitignore`, requirements, current and historical C++ source copies, GUI, and example trace/documentation files. `src/cache_logic.cpp` and `SourceCodes/CacheLogic.cpp` had the same baseline blob. The binary PDF `docs/project_report.pdf` could not be fetched through the available UTF-8-only GitHub connector; its text and figures are **not** claimed reviewed. The historical source copies are preserved rather than deleted. Security review was limited to visible tracked text; it is not a secret-scanning attestation.

## Remaining risks

GUI behavior, Windows compilation, unusually large allocations, actual PDF claims, and the historical source copies remain unverified. The simulator deliberately does not model writes, replacement choices beyond direct mapping, multi-level caches, coherence, or actual CPU instructions. Do not infer hardware conformance or production readiness from these tests.
