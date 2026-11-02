## Measurement Methodology

All timings use `clock()` from `<time.h>`, which returns CPU time, converted to seconds with `CLOCKS_PER_SEC`. `baseline_loop.c` (100,000,000 iterations) and `comparison_algorithms.c` (50,000 elements) were each run three times without modifying the source. In `instrumentation_lab.c`, each phase is wrapped by a `start` and an `end` reading, and `TOTAL` wraps the three phases. The `printf` calls are placed after all measurements, so printing is not counted.

## Observed Performance Differences

**Baseline loop.** The three runs gave 0.203582 s, 0.199169 s and 0.200834 s, an average of 0.201195 s. The spread between the fastest and the slowest run is 0.004413 s, about 2.2 % of the average, so identical code does not give identical times.

**Algorithm comparison.** The naive algorithm took 1.258383 s, 1.240744 s and 1.325694 s (average 1.274940 s). The single-pass algorithm took 0.000043 s, 0.000072 s and 0.000042 s (average 0.000052 s). The naive version is about 24,000 times slower, because it contains two nested loops over 50,000 elements, while the single-pass version has one loop.

**Instrumented phases.**

| Phase | Time (s) | Share of TOTAL |
|---|---|---|
| BUILD_DATA | 0.000262 | about 39.8 % |
| PROCESS | 0.000287 | about 43.6 % |
| REDUCE | 0.000105 | about 16.0 % |
| TOTAL | 0.000658 | 100 % |

`PROCESS`, which performs a division and a modulo per element, is the slowest phase. `REDUCE`, which only multiplies and adds, is the fastest. The phases add up to 0.000654 s, and the 0.000004 s gap to `TOTAL` comes from the intermediate `clock()` calls.

## Relation Between Runtime and Energy Consumption

Energy is approximately the average power multiplied by the execution time. If power stays roughly constant, the naive algorithm (1.274940 s on average) would consume far more energy than the single-pass one (0.000052 s). This is an assumption based on that relation, because no energy was measured in these experiments. The measurements only show where CPU time is spent.

## Limitations of the Experiment

- **No real energy measurement**: only execution time was recorded.
- **Few runs**: three runs are not enough for reliable statistics. The single-pass times vary from 0.000042 s to 0.000072 s, a spread of about 57 % of the average.
- **Very short durations**: the instrumented program runs in 0.000658 s, close to the timer resolution. The 0.000025 s difference between `PROCESS` and `BUILD_DATA` is not reliable.
- **CPU time versus real time**: `clock()` measures CPU time, which differs from wall-clock time.
- **Different results**: the naive and single-pass algorithms returned 625025000 and 25000, so the comparison covers execution time only.

## Practical Engineering Takeaway

The choice of algorithm has the largest effect: removing the nested loop reduced the time from about 1.27 s to about 0.00005 s, far more than any difference between instructions. Per-phase instrumentation then shows where to focus: `PROCESS` and `BUILD_DATA` account for about 83 % of the instrumented total. Measurements must be repeated before drawing conclusions, since even the baseline loop varied by about 2 % between runs. Finally, a lower CPU time suggests a lower energy cost, but only a dedicated energy measurement could confirm it.
