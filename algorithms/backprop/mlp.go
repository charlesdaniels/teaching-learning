package main

// This file implements a simple neural network based on the one from Russel &
// Norvig 3/e, Chapter 18 "Learning From Examples"

import (
	"encoding/json"
	"fmt"
	"github.com/akamensky/argparse"
	"github.com/cheggaaa/pb/v3"
	"io/ioutil"
	"math"
	"math/rand"
	"os"
)

type Layer struct {

	// Previous layer, nil for input layer
	Prev *Layer

	// Next layer, nil for output layer
	Next *Layer

	// Weight[j * Prev.TotalNeurons() + i] = weight for neuron j in THIS
	// layer coming in from the output of neuron i in the PREVIOUS layer.
	Weight []float64

	// Outputs for this layer
	Output []float64

	// The output after activation -- this is separate because we need the
	// pre-activation outputs for certain computations
	Activation []float64

	Delta []float64

	Bias []float64
}

func (l *Layer) TotalNeurons() int {
	return len(l.Output)
}

// Retrieve the weight associated with the link from prevNeuron in the previous
// layer, to thisNeuron in this layer.
func (l *Layer) GetWeight(thisNeuron, prevNeuron int) float64 {

	// input layer
	if l.Prev == nil {
		return 1.0
	} else {
		return l.Weight[thisNeuron*l.Prev.TotalNeurons()+prevNeuron]
	}
}

func (l *Layer) SetWeight(thisNeuron, prevNeuron int, newWeight float64) {
	if l.Prev == nil {
		return
	}
	l.Weight[thisNeuron*l.Prev.TotalNeurons()+prevNeuron] = newWeight
}

func NewLayer(size int, prev, next *Layer) *Layer {
	l := &Layer{
		Prev:       prev,
		Next:       next,
		Delta:      make([]float64, size),
		Output:     make([]float64, size),
		Activation: make([]float64, size),
		Bias:       make([]float64, size),
	}

	if prev != nil {
		l.Weight = make([]float64, size*(prev.TotalNeurons()))
		for i, _ := range l.Weight {
			l.Weight[i] = rand.Float64()
		}
	} else {
		// This is the inputlayer, so there are no weights
		l.Weight = nil
	}

	for i, _ := range l.Bias {
		l.Bias[i] = rand.Float64()
	}

	return l
}

type MLP struct {
	Layer []*Layer

	// Learning rate
	Alpha float64
}

func (nn *MLP) InputLayer() *Layer {
	return nn.Layer[0]
}

func (nn *MLP) OutputLayer() *Layer {
	return nn.Layer[len(nn.Layer)-1]
}

func NewMLP(alpha float64, layerSizes ...int) *MLP {
	nn := &MLP{
		Layer: make([]*Layer, len(layerSizes)),
		Alpha: alpha,
	}

	// generate the layers and their links back to the previous layers
	for i, v := range layerSizes {
		if i == 0 {
			nn.Layer[i] = NewLayer(v, nil, nil)
		} else {
			nn.Layer[i] = NewLayer(v, nn.Layer[i-1], nil)
		}

	}

	// generate links to next layers
	for i, _ := range layerSizes {
		if i < len(layerSizes)-1 {
			nn.Layer[i].Next = nn.Layer[i+1]
		}
	}

	return nn

}

func Sigmoid(x float64) float64 {
	return 1.0 / (1.0 + math.Exp(-1.0*x))
}

func Logit(x float64) float64 {
	return math.Log(x / (1 - x))
}

func ActivationFunction(inj float64) float64 {
	return Sigmoid(inj)
}

func ActivationDeriv(inj float64) float64 {
	return Sigmoid(inj) * (1 - Sigmoid(inj))
}

func (nn *MLP) ForwardPass(input []float64) error {
	// The input must be the same size as the input layer, for obvious
	// reasons.
	if len(input) != nn.InputLayer().TotalNeurons() {
		return fmt.Errorf("Input vector size %d =/= input layer size %d",
			len(input), nn.Layer[0].TotalNeurons())
	}

	// copy input data into input layer outputs
	for i := 0; i < nn.InputLayer().TotalNeurons(); i++ {
		nn.InputLayer().Activation[i] = input[i]

		// in_j is not used for the input layer
		nn.InputLayer().Output[i] = 0
	}

	// consider remaining layers
	for l := 1; l < len(nn.Layer); l++ {
		layer := nn.Layer[l]
		for j := 0; j < layer.TotalNeurons(); j++ {
			// in_j ← ∑i w_i,j ai
			sum := 0.0 // sum is in_j
			for i := 0; i < layer.Prev.TotalNeurons(); i++ {
				// layer.Prev.Activation[j] is a_i
				// layer.GetWeight(j, i) w_i,j
				sum += layer.Prev.Activation[i] * layer.GetWeight(j, i)
			}

			// explicitly account for the bias, which is handled
			// implicitly in the Russel & Norvig text as weight
			sum += layer.Bias[j]

			// a_j ← g(in_j)
			layer.Activation[j] = ActivationFunction(sum)

			// We also save in_j because we need it later for
			// computing the Δ values
			layer.Output[j] = sum
		}
	}

	return nil
}

// "output" refers to the expected output data
func (nn *MLP) BackwardPass(output []float64) error {
	if len(output) != nn.OutputLayer().TotalNeurons() {
		return fmt.Errorf("Output vector size %d =/= output layer size %d",
			len(output), nn.OutputLayer().TotalNeurons())
	}

	// calculate deltas for output layer
	for j := 0; j < nn.OutputLayer().TotalNeurons(); j++ {
		// Δ[j] ← g'(in_j) × (y_j - a_j)
		//
		// nn.OutputLayer().Delta[j] is Δ[j]
		//
		// g' is ActivationDeriv()
		//
		// in_j is layer.Output[j]
		//
		// a_j is layer.activation[j]
		//
		// y_j is output[j]
		//
		//    Notice that we have already asserted that the output
		//    layer and the output vector are the same size, so it is
		//    safe to assume we won't go out of bounds in output
		nn.OutputLayer().Delta[j] =
			ActivationDeriv(nn.OutputLayer().Output[j]) * (output[j] - nn.OutputLayer().Activation[j])
	}

	// and for remaining layers
	for l := len(nn.Layer) - 2; l >= 0; l-- {
		layer := nn.Layer[l]
		for i := 0; i < layer.TotalNeurons(); i++ {
			// Δ[i] ← g'(in_i) ∑j w_i,j Δ[j]
			//
			// layer.Delta[i] is Δ[i]
			//
			// layer.Next.GetWeight(j, i) w_i,j
			//
			//    The indices appear reversed because of how
			//    GetWeight() is written, j is the destination
			//    neuron on layer.Next, i is the neuron in this
			//    current layer.
			//
			// layer.Next.Delta[j] is Δ[j]
			//
			// layer.Output[i] is in_i
			layer.Delta[i] = 0
			for j := 0; j < layer.Next.TotalNeurons(); j++ {
				layer.Delta[i] += layer.Next.GetWeight(j, i) * layer.Next.Delta[j]
			}
			layer.Delta[i] *= ActivationDeriv(layer.Output[i])
		}
	}

	return nil
}

func (nn *MLP) UpdateWeights() {
	for l, layer := range nn.Layer {
		for i := 0; i < layer.TotalNeurons(); i++ {
			if l == 0 {
				// skip input layer, weights are always 1
				continue
			}
			for j := 0; j < layer.Prev.TotalNeurons(); j++ {
				// w_i,j ← w_i,j + α × a_i × Δ[j]
				//
				// layer.Activation[i] is a_i
				//
				// layer.Prev.Delta[j] is Δ[j]
				//
				// nn.Alpha is α
				layer.SetWeight(i, j,
					layer.GetWeight(i, j)+nn.Alpha*layer.Activation[i]*layer.Prev.Delta[j])

				// we also update the bias at this point,
				// keeping in mind that the a_i for the bias is
				// implied to be 1
				layer.Bias[i] += nn.Alpha * layer.Delta[i]
			}
		}
	}
}

func (nn *MLP) Train(input, output []float64) error {
	err := nn.ForwardPass(input)
	if err != nil {
		return err
	}

	err = nn.BackwardPass(output)
	if err != nil {
		return err
	}

	nn.UpdateWeights()

	return nil
}

func (nn *MLP) Predict(input []float64) ([]float64, error) {
	err := nn.ForwardPass(input)
	if err != nil {
		return nil, err
	}

	return nn.OutputLayer().Activation, nil
}

func main() {
	parser := argparse.NewParser("nn", "Simple neural network example.")

	i := parser.File("i", "input-file", os.O_RDONLY, 0755,
		&argparse.Options{Required: true, Help: "Input file to read NN spec from."})

	e := parser.Int("e", "epochs",
		&argparse.Options{Required: false, Default: 1000, Help: "Number of epochs to train the neural network for."})

	err := parser.Parse(os.Args)
	if err != nil {
		fmt.Print(parser.Usage(err))
		return
	}

	defer i.Close()

	jsondata, err := ioutil.ReadAll(i)
	if err != nil {
		panic(err)
	}

	var result map[string]interface{}
	err = json.Unmarshal(jsondata, &result)
	if err != nil {
		panic(err)
	}

	// We load the NN from the JSON file. The error handling here is
	// not very good, but I didn't feel like writing all the structs
	// to directly map onto the JSON.
	//
	// This is and example of the basic structure we want:
	//
	// {
	//     "_comment": "Neural network implementation of AND gate",
	//     "network": {
	//         "layers": [2, 5, 5, 1],
	//         "alpha": 0.1
	//     },
	//     "examples": [
	//         [[0.0, 0.0], [0.0]],
	//         [[0.0, 1.0], [1.0]],
	//         [[1.0, 0.0], [1.0]],
	//         [[1.0, 1.0], [1.0]]
	//     ],
	//     "inputs": [
	//         [0, 0],
	//         [1, 0],
	//         [0, 1],
	//         [1, 1]
	//     ]
	// }
	//
	// The "_comment" field can simply be ignored. Note that the input
	// and output vectors have to match the length of the laers specified.

	network := result["network"].(map[string]interface{})

	// Parse enough to create the network...
	alpha := network["alpha"].(float64)
	layers := make([]int, 0)
	for _, v := range network["layers"].([]interface{}) {
		layers = append(layers, int(v.(float64)))
	}

	nn := NewMLP(alpha, layers...)

	// Now we start on the examples, training as we go.
	examples := result["examples"].([]interface{})
	fmt.Printf("training network... \n")
	bar := pb.StartNew(*e)
	for epoch := 0; epoch < *e; epoch++ {
		bar.Increment()

		// rand.Perm(len(examples)) will give us the shuffled indices
		// of the examples array.
		for _, idx := range rand.Perm(len(examples)) {

			// Pull out the input and output vectors.
			v := examples[idx]
			tup := v.([]interface{})
			rawinput := tup[0].([]interface{})
			rawoutput := tup[1].([]interface{})
			input := make([]float64, 0)
			output := make([]float64, 0)

			// Copy and typecast in one go from the input to the
			// output vectors.
			for _, inp := range rawinput {
				input = append(input, inp.(float64))
			}

			for _, out := range rawoutput {
				output = append(output, out.(float64))
			}

			// Pass the example into the network.
			err := nn.Train(input, output)
			if err != nil {
				panic(err)
			}
		}
	}
	bar.Finish()

	// Finally, consider the inputs we'd like to compute results for.
	// These don't have expected values, since in principle they are cases
	// where we want the model to extrapolate for us.
	inputs := result["inputs"].([]interface{})
	for _, v := range inputs {

		// This works the same way as for the examples.
		input := make([]float64, 0)
		for _, inp := range v.([]interface{}) {
			input = append(input, inp.(float64))
		}

		res, err := nn.Predict(input)
		if err != nil {
			panic(err)
		}
		fmt.Printf("Running with input=%v, result=%v\n", input, res)

	}

}
