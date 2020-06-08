#!/usr/bin/env python3

import json
import random

j = {
    "network": {
        "layers": [2, 5, 1],
        "alpha": 0.1
    },
    "examples": [],
    "inputs": [
        [0.05, 0.0025],
        [0.15, 0.0225]
    ]
}

num_examples = 500

def f(x):
    return 3 * x * x + 5 * x - 7

for eno in range(num_examples):
    inp = random.uniform(0, 1)
    res = f(inp)
    j["examples"].append([[inp, inp*inp], [res]])

with open ("nn2.json", "w") as f:
    f.write(json.dumps(j, indent=4))
