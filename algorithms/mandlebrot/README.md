
<!-- vim-markdown-toc GFM -->

* [Introduction](#introduction)
* [Usage](#usage)
* [Performance](#performance)
    * [Test Machines](#test-machines)
    * [Test Results](#test-results)

<!-- vim-markdown-toc -->

# Introduction

[](./assets/demo.png)

This directory contains a parallel (non-naive) implementation of a Mandelbrot
set generator in Python 3. This implementation demonstrates both a non-trivial
use case for Pythons `multiprocessing.Pool.map()`, as well as a relatively
straightforward implementation of a Mandelbrot set generator. For performance
reasons, this implementation of the set generator is implemented iteratively,
rather than recursively.

# Usage

Run `make demo` for a quick demo.

```
usage: mandelbrot.py [-h] [--recurse_depth RECURSE_DEPTH]
                     [--resolution RESOLUTION] [--verbose] [--display]
                     [--output OUTPUT]

Simple parallel mandlebrot set visualization. Requires Python 3.3 or better.

optional arguments:
  -h, --help            show this help message and exit
  --recurse_depth RECURSE_DEPTH, -d RECURSE_DEPTH
                        Maximum recursion depth before returning from
                        mandlebrot calculation. (default: 128)
  --resolution RESOLUTION, -n RESOLUTION
                        Image resolution in [cols]x[rows] format. (default:
                        640x480)
  --verbose, -v         Display every P_c calculation and point mapping
  --display, -p         Use matplotlib to display the image in a GUI window as
                        soon as computation is finished.
  --output OUTPUT, -o OUTPUT
                        Specify an output file to save the result to.
```

Note that this program is not truly rigorous - as the recursion depth is
necessarily finite, it is possible that some points marked as being members of
the Mandelbrot set will be false-positives (in the case where a given number
would diverge to infinity after some number of iterations greater than the
recursion depth). To improve the accuracy of the program, simply use a higher
recursion depth.

# Performance

## Test Machines

| Machine Number | CPU                 | Cores / Threads | RAM            | Number of NUMA Nodes | Number of Sockets |
|----------------|---------------------|-----------------|----------------|----------------------|-------------------|
| 1              | i7-2640M @ 2.80GHz  | 2C/4T           | 12GB @ 1333MHz | 1                    | 1                 |
| 2              | 2x E5520  @ 2.27GHz | 8C/16T          | 96Gb @ ? MHz   | 2                    | 2                 |

## Test Results

|Machine Number | Resolution | Recursion Depth | Threads used | Elapsed Seconds    |
|---------------|------------|-----------------|--------------|--------------------|
| 1             | 256x256    | 256             | 1            | 10.120744660000128 |
| 1             | 256x256    | 256             | 2            | 5.375709760000973  |
| 1             | 256x256    | 256             | 4            | 5.240381178999087  |
| 1             | 512x512    | 256             | 1            | 39.85520727000039  |
| 1             | 512x512    | 256             | 2            | 21.903679931001534 |
| 1             | 512x512    | 256             | 4            | 22.977132228999835 |
| 2             | 256x256    | 256             | 1            | 22.127644610009156 |
| 2             | 256x256    | 256             | 8            | 3.386901296908036  |
| 2             | 256x256    | 256             | 16           | 3.0181645220145583 |
| 2             | 512x512    | 256             | 1            | 88.4461338800611   |
| 2             | 512x512    | 256             | 8            | 13.539519277052023 |
| 2             | 512x512    | 256             | 16           | 12.352946505998261 |
| 2             | 1024x1024  | 256             | 16           | 49.16464102698956  |
