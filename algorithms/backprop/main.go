package main

// This program is based on Russel & Norvig 3/e, chapter 18
//
// It implements a very simple neural network with back propogation
//
// The NN takes as input a vector of floats, and produces a vector of floats.
//
// The length of the input vector must be the same as the size of the input
// layer, and the output must be the same as the size of the output layer.

import (
	"encoding/json"
	"fmt"
	"github.com/akamensky/argparse"
	"io/ioutil"
	"math/rand"
	"os"
)

type LayerSpec struct {
	// Size[k] = # of nodes in layer k
	Size []uint
}

func NewLayerSpec(sizes ...uint) LayerSpec {
	ls := LayerSpec{
		Size: make([]uint, len(sizes)),
	}

	for i, s := range sizes {
		ls.Size[i] = s
	}

	return ls
}

func (spec LayerSpec) TotalNeurons() uint {
	sum := uint(0)
	for _, s := range spec.Size {
		sum += s
	}
	return sum
}

func (spec LayerSpec) TotalLayers() uint {
	return uint(len(spec.Size))
}

// Return the layer number of a given neuron
func (spec LayerSpec) GetLayer(neuron uint) uint {
	s := uint(0)

	for l := uint(0); l < spec.TotalLayers(); l++ {
		nexts := s + spec.Size[l]
		if (neuron >= s) && (neuron < nexts) {
			return l
		}
		s = nexts
	}

	return spec.TotalLayers() // out of bounds
}

// Offset an index into a layer to an absolute neuron number.
//
// For example, if layer 0 had 10 neurons, and we called Offset(1, 3), the
// result would be 10+3=13.
func (spec LayerSpec) Offset(layer, index uint) uint {

	// accumulate up all the layer sizes until the one we're interested in
	s := uint(0)
	for l := uint(0); l < layer; l++ {
		s += spec.Size[l]
	}

	return s + index
}

// The inverse of Offset(), gets a relative index within a layer from an
// absolute index.
func (spec LayerSpec) UnOffset(index uint) uint {

	layer := spec.GetLayer(index)

	// accumulate all the layer sizes up until index
	s := uint(0)
	for l := uint(0); l < layer; l++ {
		s += spec.Size[l]
	}

	return index - layer
}

type NeuralNetwork struct {
	// Output[i] = a_i
	Output []float32

	// Weight[i][j] = w_i,j
	Weight [][]float32

	Bias []float32

	// Define the number of neurons in each layer
	Spec LayerSpec

	Alpha float32
}

func (nn *NeuralNetwork) ActivationFunction(neuron uint, inj float32) float32 {
	return nn.Bias[neuron] + inj
}

func NewNeuralNet(spec LayerSpec, alpha float32) *NeuralNetwork {
	nn := &NeuralNetwork{
		Output: make([]float32, spec.TotalNeurons()),
		Bias:   make([]float32, spec.TotalNeurons()),
		Weight: make([][]float32, spec.TotalNeurons()),
		Spec:   spec,
		Alpha:  alpha,
	}

	// allocate space for the weights matrix
	for neuron := uint(0); neuron < spec.TotalNeurons(); neuron++ {
		var prevLayerSize uint
		//
		// // for the inner arrays, we only allocate enough space for the
		// // weights applying to the previous layer's neurons
		// if spec.GetLayer(neuron) == 0 {
		//         // we assume that the first layer has it's size many
		//         // inputs.
		//         prevLayerSize = spec.Size[spec.GetLayer(neuron)]
		// } else {
		//         // otherwise we want to have enough space for the
		//         // previous layer
		//         prevLayerSize = spec.Size[spec.GetLayer(neuron)-1]
		//
		// }
		prevLayerSize = spec.TotalNeurons()
		nn.Weight[neuron] = make([]float32, prevLayerSize)

		// initialize to small random values
		for j := uint(0); j < prevLayerSize; j++ {
			// random float in 0 ... 1.0
			nn.Weight[neuron][j] = rand.Float32()
		}

		// also initialize the biases to small random values as well...
		nn.Bias[neuron] = 1 //rand.Float32()
	}

	return nn
}

// Train the neural network with one single example, given the expected input
// and expected output of that example.
func (nn *NeuralNetwork) Train(input, output []float32) error {

	// assert that input layer size matches input vector size
	if uint(len(input)) != nn.Spec.Size[0] {
		return fmt.Errorf("Input vector size %d =/= input layer size %d",
			len(input), nn.Spec.Size[0])
	}

	// and similarly for the output layer
	if uint(len(output)) != nn.Spec.Size[nn.Spec.TotalLayers()-1] {
		return fmt.Errorf("Input vector size %d =/= input layer size %d",
			len(input), nn.Spec.Size[0])
	}

	// forward pass ...

	// for each neuron in the input layer...
	for neuron := uint(0); neuron < nn.Spec.Size[0]; neuron++ {
		//a_i ← x_i
		nn.Output[neuron] = input[neuron]
	}

	// for each remaining layer...
	for l := uint(1); l < nn.Spec.TotalLayers(); l++ {

		// for each node j in layer l
		//
		// note that j is a relative offset, not an absolute neuron
		for j := uint(0); j < nn.Spec.Size[l]; j++ {
			inj := float32(0)

			// j as an absolute neuron
			jabs := nn.Spec.Offset(l, j)

			// for each input in the previous layer...
			for i := uint(0); i < nn.Spec.Size[l-1]; i++ {
				iabs := nn.Spec.Offset(l-1, i)
				inj += nn.Weight[iabs][jabs] * nn.Output[iabs]
			}

			//a_j ← g(Σi w+i,j a_i)
			nn.Output[jabs] = nn.ActivationFunction(jabs, inj)
		}
	}

	// backwards pas ...
	delta := make([]float32, nn.Spec.TotalNeurons())

	// for each neuron in the output layer ...
	for neuron := nn.Spec.Offset(nn.Spec.TotalLayers()-1, 0); neuron < nn.Spec.TotalNeurons(); neuron++ {
		/// TODO: should account for g'()

		// difference between expected value (y_j) and actual value (a_j)
		delta[neuron] = nn.Output[nn.Spec.UnOffset(neuron)] - nn.Output[neuron]
	}

	// for each remaining layer...
	for l := int(nn.Spec.TotalLayers()) - 2; l >= 0; l-- {
		for i := uint(0); i < nn.Spec.Size[l]; i++ {
			iabs := nn.Spec.Offset(uint(l), i)

			// TODO: should also account for g'() here
			delta[iabs] = 0
			for j := uint(0); j < nn.Spec.Size[l+1]; j++ {
				jabs := nn.Spec.Offset(uint(l+1), j)
				delta[iabs] += nn.Weight[iabs][jabs] * delta[i]
			}
		}
	}

	// update weights
	for i, _ := range nn.Weight {
		for j, _ := range nn.Weight[i] {
			jabs := nn.Spec.Offset(nn.Spec.GetLayer(uint(i)), uint(j))
			if int(jabs) >= len(delta) {
				// some of the values calculated in this way
				// will be out of bounds
				continue
			}

			nn.Weight[i][j] += nn.Alpha * nn.Output[i] * delta[jabs]
		}
	}

	// no error encountered
	return nil
}

// This is essentially just the forward pass part of the training function...
// that should probably be broken out into it's own abstracted function.
func (nn *NeuralNetwork) Predict(input []float32) ([]float32, error) {
	// assert that input layer size matches input vector size
	if uint(len(input)) != nn.Spec.Size[0] {
		return nil, fmt.Errorf("Input vector size %d =/= input layer size %d",
			len(input), nn.Spec.Size[0])
	}

	// for each neuron in the input layer...
	for neuron := uint(0); neuron < nn.Spec.Size[0]; neuron++ {
		//a_i ← x_i
		nn.Output[neuron] = input[neuron]
	}

	// for each remaining layer...
	for l := uint(1); l < nn.Spec.TotalLayers(); l++ {

		// for each node j in layer l
		//
		// note that j is a relative offset, not an absolute neuron
		for j := uint(0); j < nn.Spec.Size[l]; j++ {
			inj := float32(0)

			// j as an absolute neuron
			jabs := nn.Spec.Offset(l, j)

			// for each input in the previous layer...
			for i := uint(0); i < nn.Spec.Size[l-1]; i++ {
				iabs := nn.Spec.Offset(l-1, i)
				inj += nn.Weight[iabs][jabs] * nn.Output[iabs]
			}

			//a_j ← g(Σi w+i,j a_i)
			nn.Output[jabs] = nn.ActivationFunction(jabs, inj)
		}
	}

	outputs := make([]float32, 0)
	// for each neuron in the output layer ...
	for neuron := nn.Spec.Offset(nn.Spec.TotalLayers()-1, 0); neuron < nn.Spec.TotalNeurons(); neuron++ {
		outputs = append(outputs, nn.Output[neuron])
	}

	return outputs, nil

}

func main() {
	parser := argparse.NewParser("nn", "Simple neural network example.")

	i := parser.File("i", "input-file", os.O_RDONLY, 0755,
		&argparse.Options{Required: true, Help: "Input file to read NN spec from."})

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

	network := result["network"].(map[string]interface{})
	alpha := float32(network["alpha"].(float64))
	layers := make([]uint, 0)
	for _, v := range network["layers"].([]interface{}) {
		layers = append(layers, uint(v.(float64)))
	}

	spec := LayerSpec{Size: layers}
	nn := NewNeuralNet(spec, alpha)

	examples := result["examples"].([]interface{})
	fmt.Println(nn)
	for _, v := range examples {
		tup := v.([]interface{})
		rawinput := tup[0].([]interface{})
		rawoutput := tup[1].([]interface{})
		input := make([]float32, 0)
		output := make([]float32, 0)

		for _, inp := range rawinput {
			input = append(input, float32(inp.(float64)))
		}

		for _, out := range rawoutput {
			output = append(output, float32(out.(float64)))
		}

		fmt.Printf("Training with input=%v, output=%v\n", input, output)
		err := nn.Train(input, output)
		if err != nil {
			panic(err)
		}
	}
	fmt.Println(nn)

	inputs := result["inputs"].([]interface{})
	for _, v := range inputs {
		input := make([]float32, 0)
		for _, inp := range v.([]interface{}) {
			input = append(input, float32(inp.(float64)))
		}

		res, err := nn.Predict(input)
		if err != nil {
			panic(err)
		}
		fmt.Printf("Running with input=%v, result=%v\n", input, res)

	}

}
