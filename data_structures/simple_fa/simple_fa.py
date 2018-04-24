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

# Script that shows an example of simulating a finite automata in Python. This
# was originally part of a larger project, but has been stripped out for
# demonstration purposes, and a toy problem has been constructed to show off
# the functionality herein.

# For this example, we will parse tabular data which has been converted to an
# ordered list of strings. Consider a table with the columns
# STUDENT_ID,COURSE,GPA. Consider also that when a student has taken more than
# one course, the additional course is attached to the pertinent record by
# including a line of the format <empty string>,COURSE,GPA. For example, the
# input:

# jsmith,MATH101,3.0
# ,ENGL101,4.0

# Would imply a record attached to the student jsmith containing a score of 3.0
# for MATH101, and a score of 4.0 for ENGL101. An arbitrary number of scores
# might be attached to one record in this way.

# To simulate the original use case for this simulator, the input is read from
# standard in with one cell per line, so the example above would be input as
# follows:

# jsmith
# MATH101
# 3.0
#
# ENGL101
# 4.0

# A set of four sample files can be found in the same folder with this program.
# sample1.txt and sample2.txt show valid inputs, while sample3.txt and
# sample4.txt show invalid inputs.

# The automata in this program is made up of a table of transitions, such that
# the ith index in the table contains a list of transitions AWAY from that
# state.  Transitions are ordered from highest priority to lowest priority.
# Each transition contains a regex which the current token must match, and the
# highest priority token where the regex matches the current token defines the
# next state of the DFA.

import argparse
import sys
import re

args = None

# This is the table of states. The nth item in the table is state n, and
# contains a list of tuples, each of which describe a transition. A transition
# contains a regex, which if matched by the input token will cause it to be
# taken, the target state to transition to, and the field which we must have
# been looking at before the transition. If more than one transition could be
# taken, then the first one is taken.

# pattern, transition, field
transition_table = [
    [('[a-zA-Z0-9]',                1, "STUDENTID")],
    [('[A-Z]{4}[0-9]{3}',           2, "COURSE")],
    [('[0-9]{1,2}\.[0-9]{1,2}',     3, "GPA")],
    [('[a-zA-Z0-9]',                1, "STUDENTID"),
     ('',                           1, "NULL")],

]


def verbose(msg):
    if not args.verbose:
        return
    msg = "{}\n".format(msg)
    sys.stderr.write(msg)
    sys.stderr.flush()

def next_state(current_state, text):

    # consider all possible transitions away from the current state
    for transition_tup in transition_table[current_state]:

        # unpack the tuple into it's component parts
        pattern, transition, field = transition_tup

        # check if it matches the pattern
        r = re.compile(pattern)
        if r.match(text):
            verbose("{} --- ('{}' matched '{}' => {}) ---> {}"
                    .format(current_state, text, pattern,
                            field, transition))

            # extra debugging output for ERROR fields
            if field == "ERROR":
                verbose("error transition while matching " +
                        "'{}' which is {} from state {}"
                        .format(
                            text, ':'.join([hex(ord(x))[2:] for x in text]),
                            current_state))
                verbose ("other transitions were: {}"
                         .format(transition_table[current_state]))

            return (transition, field)

    # if execution reaches this point, the transition table is invalid
    sys.stderr.write("FATAL: invalid transition table\n")
    exit(1)


def visualize_transitions():
    from graphviz import Digraph

    table = transition_table
    dot = Digraph(comment='Transition Table')

    for node_num in range(len(table)):
        nodelabel = "({})\n".format(node_num)
        transitions = table[node_num]
        t_num = 0
        for t in transitions:
            pattern, target, field = t

            # render epsilon transitions
            if pattern == "":
                pattern = "epsilon"

            pattern = "{}: {}\n".format(t_num, pattern)

            dot.edge("{}".format(node_num),
                     "{}".format(target),
                     label = pattern)
            nodelabel += "{}: ".format(t_num) + field + "\n"
            t_num += 1


        dot.node("{}".format(node_num), label=nodelabel)
    dot.render("table.dot")

def extractEntries(f):

    err_trans = ('', 0, "ERROR")

    # Every transition must also support the error transition, which is an
    # epsilon-transition back to state zero. When this transition is taken, the
    # field is marked as 'ERROR', and extractEntries() induces a crash.
    for t in transition_table:
        t.append(err_trans)


    current_state = 0
    counter = 1
    field = None
    last_field = None
    records = []
    accumulator = {}

    for line in f:
        line = line.strip()

        # get the next state from the transition table
        last_field = field
        current_state, field = \
            next_state(current_state, line)

        # make sure the transition was valid
        if field == "ERROR":
            sys.stderr.write("FATAL: invalid transition while reading " +
                             "record {}\n".format(counter) )
            exit(1)

        # transition to parsing a new record
        if last_field == "GPA" and field == "STUDENTID":
            records.append(accumulator)
            verbose("Finished parsing a record: {}".format(accumulator))
            accumulator = {}

            counter += 1

        # insert the field into the accumulator
        if field != "NULL":
            if field not in accumulator:
                accumulator[field] = []
            accumulator[field].append(line)

    # make sure we catch the last record
    if len(accumulator) > 0:
        if field == "GPA":
            verbose("Finished parsing a record: {}".format(accumulator))
            records.append(accumulator)
        else:
            sys.stderr.write("FATAL: incomplete record\n")
            exit(1)


    print("- - - finished parsing, dump of records - - -")
    for rec in records:
        print(rec)


def main():

    parser = argparse.ArgumentParser()

    parser.add_argument("--input", "-i", default=sys.stdin, help="Specify " +
                        "file (default: standard input")

    parser.add_argument("--verbose", "-v", default=False, action="store_true",
                        help="Display verbose output")

    parser.add_argument("--visualize", "-s", default=False, action="store_true",
                        help="Visualize the FA in a file table.gv.pdf")

    global args
    args = parser.parse_args()

    if args.input is not sys.stdin:
        args.input = open(args.input, "r")

    extractEntries(args.input)

    if args.visualize:
        visualize_transitions()


if __name__ == "__main__":
    main()
