# Direct-mapped cache simulator

A small C++17 educational simulator for **separate instruction and data caches**. Given a trace of 32-bit byte addresses, it models direct-mapped tag/index lookup and reports hits, misses, and average access time. The optional Python/ttkbootstrap GUI supplies inputs to the same command-line executable.

**Scope:** This is a trace-driven timing model, not an instruction-set simulator or a hardware implementation. It does not store actual data, implement reads versus writes, model dirty bits, or simulate a hierarchy of caches. Its fixed main-memory miss penalty is 100 cycles; this is a teaching assumption, not a benchmark result.

## Quickstart

Requires a C++17 compiler, `make`, and Python 3 for the automated tests. On Linux/macOS:

```sh
git clone https://github.com/omarsaqr12/cache-simulator.git
cd cache-simulator
make
printf '4096\n4096\n64\n2\ntest_data/test1.txt\n' | ./cache_simulator
make test
```

The prompt order is instruction-cache size, data-cache size, block size (all in bytes), cache-access latency (cycles), and trace filename. Sizes must be **nonzero powers of two**; block size must fit both caches; latency must be 1–10 cycles. For Windows, use a C++17 compiler to build `src/main.cpp` and launch the resulting executable. The provided Makefile is intended for POSIX shells.

For the optional GUI, install Python with Tk support and run `make install-deps && make gui`. The GUI is a wrapper around the executable, **not** a second simulation engine. GUI runtime behavior has not been exercised as part of this review; its existing default block size of 12 is invalid, so change it to a power of two (for example 64) before running.

## Trace format

Each nonblank line contains **exactly 32 binary digits** followed immediately by `I` (instruction) or `D` (data). Leading/trailing whitespace is accepted; malformed records fail with an error and line number. Example:

```text
00000000000000000000000000000000I
00000000000000000000000000000001I
00000000000000000000000000000100D
```

See [`test_data/`](test_data/) for three input traces. Both caches start empty. For a byte address `a`, the index is `(a / block_size) mod number_of_lines`; the tag is the remaining high-order bits. On a hit the cost is the configured cache latency; on a miss it is that latency **plus 100 cycles**. AMAT is total modeled cycles divided by requests for that cache, or zero for an empty trace. Instruction and data cache statistics are separate.

## Repository map

| Path | Purpose |
| --- | --- |
| [`include/cache.h`](include/cache.h) | Header-only direct-mapped cache model and statistics |
| [`src/main.cpp`](src/main.cpp) | Validates CLI configuration, parses access traces, invokes the two caches |
| [`src/cache_logic.cpp`](src/cache_logic.cpp) | Compatibility entry point sharing the validated implementation |
| [`src/gui.py`](src/gui.py) | Optional GUI that launches the CLI |
| [`tests/`](tests/) | C++ known-answer checks and Python black-box CLI tests |
| [`SourceCodes/`](SourceCodes/) | Historical course-era source copies, **not** used by the Makefile |
| [`docs/project_report.pdf`](docs/project_report.pdf) | Original academic report; preserved, not used as proof of current behavior |

Run `make cache_logic` only if you need the historical second executable name. It now uses the same parser as `cache_simulator`.

## Verification and limitations

`make test` compiles the simulator, runs a C++ test of empty-cache statistics, hits, conflict misses, 32-bit address tags, and invalid configurations, then runs Python black-box tests of trace parsing and CLI output. The added tests passed locally with g++ and Python 3 when authored. That does **not** establish cross-platform or GUI behavior. No generated output or measured performance claim is included. See [`docs/AUDIT.md`](docs/AUDIT.md) for the baseline issues and review coverage.

This is an academic project. The historical source files and original report remain available for provenance. The repository does not include an open-source license; please ask the author before reusing code beyond what applicable law permits.
