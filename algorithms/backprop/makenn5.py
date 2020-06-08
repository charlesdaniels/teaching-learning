#!/usr/bin/env python3

import json
import random

j = {
    "network": {
        "layers": [1, 25, 10, 5, 1],
        "alpha": 0.1
    },
    "examples": [],
    "inputs": [
        [0.05],
        [0.15],
        [0.25],
        [0.35],
        [0.45],
        [0.55],
        [0.65],
        [0.75],
        [0.85],
    ]
}

num_examples = 2000

def f(x):
    #  return 3 * x * x + 5 * x - 7
    if x > 0.5:
        return 1
    else:
        return 0

for eno in range(num_examples):
    inp = random.uniform(0, 1)
    res = f(inp)
    j["examples"].append([[inp], [res]])

with open ("nn5.json", "w") as f:
    f.write(json.dumps(j, indent=4))

