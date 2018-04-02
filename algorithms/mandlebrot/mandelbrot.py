#!/usr/bin/env python3

#  Copyright (c) 2018, Charles Daniels
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  3. Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived from
#     this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.

import argparse
import numpy
import multiprocessing
import sys
import time


########10########20########30 ARGUMENT HANDING 50########60########70########80

def resolution2tuple(res):
    """resolution2tuple

    Convert resolution of the format [rows]x[cols] to a tuple (rows, cols).
    Crash the program if it is not valid.

    :param res:
    """

    res = res.strip()
    components = res.split("x")

    if len(components) != 2:
        sys.stderr.write("ERROR: invalid resolution: '{}'\n".format(res))
        exit(1)

    rows, cols = components

    try:
        rows = int(rows)
        cols = int(cols)
    except Exception as e:
        sys.stderr.write("ERROR: non-integer resolution components in")
        sys.stderr.write(" '{}' (caused exception '{}')\n".format(res, e))
        exit(1)

    return (rows, cols)


parser = argparse.ArgumentParser(description="Simple parallel mandelbrot " +
        "set visualization. Requires Python 3.3 or better.")

parser.add_argument("--recurse_depth", "-d", default=128, type=int,
                    help="Maximum recursion depth before returning from" +
                    " mandelbrot calculation. (default: 128)")

parser.add_argument("--resolution", "-n", default="640x480",
                    help="Image resolution in [cols]x[rows] format." +
                    " (default: 640x480)")

parser.add_argument("--verbose", "-v", default=False, action="store_true",
                    help="Display every P_c calculation and point mapping")

parser.add_argument("--display", "-p", default=False, action="store_true",
                    help="Use matplotlib to display the image in a GUI" +
                    " window as soon as computation is finished.")

parser.add_argument("--output", "-o", default=None,
                    help="Specify an output file to save the result to.")

parser.add_argument("--threads", "-t", default=multiprocessing.cpu_count(),
                    help="Specify number of threads. (default: number of " +
                    "cpu cores installed)", type=int)

args = parser.parse_args()

cols, rows = resolution2tuple(args.resolution)
min_axis = min(rows, cols)
max_axis = max(rows, cols)

########10########20########30# HELPER METHODS #50########60########70########80

def verbose(msg):
    """verbose

    Wrapper for stderr so we can toggle verpose messages on an off.

    :param msg:
    """
    if args.verbose:
        sys.stderr.write(msg)
        sys.stderr.write("\n")
        sys.stderr.flush()

def mapc(k):
    """mapc

    Map a coordinate k in the pixel-space of the output image to a coordinate
    in the complex plane. For example k = 128,128 with resolution = 128x128
    would cause an output of 2,2

    This function assume the "viewing window" will always be 4x4, centered
    about the origin (-2, -2 is a valid coordinate) and maps any arbitrary
    resolution into this space.

    :param k: tuple of coordinates in pixel-space (row, col)
    """

    row, col = k

    # this is the midpoint of the matrix data
    midx = cols / 2
    midy = rows / 2

    row_n = (row - midx) * (2 / rows) * 2
    col_n = (col - midy) * (2 / cols) * 2

    verbose("map {} -> {}".format(k, (row_n, col_n)))
    return (row_n, col_n)

def mandelbrot_step(z_real, z_imag, c_real, c_imag):
    """mandelbrot_step

    Compute P_c(z) = z^2 + c and return it as a tuple of (real, imag).

    :param z_real: real part of z
    :param z_imag: imaginary part of z
    :param c_real: real part of c
    :param c_imag: imaginary part of c
    """

    # (x + yi)^2 = (x^2 - y^2) + 2xyi
    real = z_real * z_real - z_imag * z_imag + c_real
    imag = 2 * z_real * z_imag + c_imag

    verbose("z = {} + {}i c = {} + {}i -> {} + {}i".format(z_real, z_imag, c_real, c_imag, real, imag))

    return(real, imag)

def mandelbrot(c):
    """mandelbrot

    Calculate the number of iterations until we are certain that the value
    real + imag * i will diverge to infinity (real^2 + imag^2 > 4). If
    maximum recursion depth is exceeded, return the number of iterations so
    far.

    This function is implemented iteratively for performance reasons.  Python
    has a relatively low max recursion depth, and very deep recursion will
    inflate the heap to absurd sizes anyway.

    This function returns a tuple of the calculated depth (# of iterations) and
    the original value of c it was given. The latter is included because it
    makes this function more continent to use with a map, and thus
    get free parallel computing multiprocessing.Pool.map.

    :param c: the value of c as a tuple of (imag, real) - it will be internally
    reversed to be (real, imag). This bizzare convention makes it more
    convenient to use this function with a parallel map with a numpy ndindex
    iterator.

    """

    # Note that we deliberately transpose c because the numpy matrix iterator
    # gives us values of c with the elements in the opposite order from what
    # we want.
    real, imag = mapc((c[1], c[0]))

    depth = 0
    z_real = 0
    z_imag =0
    c_real = real
    c_imag = imag
    dist = z_real * z_real + z_imag * z_imag

    while (depth < args.recurse_depth) and (dist < 4):
        # update the fake recursion depth counter
        depth += 1

        # remember that c stays the same, only z changes
        z_real, z_imag = mandelbrot_step(z_real, z_imag, c_real, c_imag)

        # updat ethe distance from the center
        dist = z_real * z_real + z_imag * z_imag

    # we've fallen off the end of the while loop
    return (depth, c)


########10########20####### PROGRAM IMPLEMENTATION #######60########70########80

# Create a matrix of size rows x cols to store floats in. This will be the
# output image.
A = numpy.ndarray(shape=(rows, cols), dtype=float)

# create a multiprocessing pool to distribute our function calls over
pool = multiprocessing.Pool(processes=args.threads)

# this will be an iterator that yields (x, y) tuples
pixiter = numpy.ndindex(A.shape)

# get the list of depths, iterate over it, and populate A
start_time = time.perf_counter()
for val in [elem for elem in pool.map(mandelbrot, pixiter)]:
    depth, c = val
    row, col = c
    A[row][col] = depth
end_time = time.perf_counter()
elapsed_time = end_time - start_time
print("elapsed time: {} seconds".format(elapsed_time))

# handle display / output
if args.display or args.output:

    # we don't import matplotlib until this point so that this program
    # can still be used for benchmarking without matplotlib installed

    import matplotlib.pyplot as plt
    plt.imshow(A)

    if args.output:
        plt.imsave(args.output, A)
    if args.display:
        plt.show()
