# Neural Network with Back-Propagation

[Back to top-level README](../../README.md)

This program implements, with some minor deviations, the algorithm described in
*Artificial intelligence: a Modern Approach* third edition by Stuart Russel and
Peter Norvig (ISBN `978-0-13-604259-4`). The program is implemented entirely in
[`mlp.go`](./mlp.go), and is annotated directly with comments describing how it
relates to the *Back-Prop-Learning* algorithm described in Chapter 18 (Figure
18.24).

This code is not "production ready". It has not been written with performance
in mind, and it does not have the ability to save a model out to disk for later
execution. It is purely a learning tool.

## How To Run

```
$ go run mlp.go --help
usage: nn [-h|--help] -i|--input-file <file> [-e|--epochs <integer>]

          Simple neural network example.

Arguments:

  -h  --help        Print help information
  -i  --input-file  Input file to read NN spec from.
  -e  --epochs      Number of epochs to train the neural network for.. Default:
                    1000

```

Example:

```
$ go run mlp.go -e 50000 -i and.json
training network...
50000 / 50000 [-------------------------------------------------------------------------------------------------------] 100.00% 317140 p/s
Running with input=[0 0], result=[0.013674204588149027]
Running with input=[1 0], result=[0.9894154921863585]
Running with input=[0 1], result=[0.995382389326916]
Running with input=[1 1], result=[0.99519137557522]
```

## Input Format

For a representative example, see [`and.json`](./and.json).

Inputs are provided in JSON format, and describe the learning rate, network
structure, training examples, and inputs in a single file. The number of epochs
to run for is provided by a CLI argument.

## License

See [the license for this repository](../../LICENSE).
