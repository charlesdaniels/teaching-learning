package main

// Copyright 2020 Charles Daniels
//
// This program implements a simple Perlin noise generator, and a few utility
// methods.
//
// It is loosely based on the information described here
// https://adrianb.io/2014/08/09/perlinnoise.html

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"math"
	"math/rand"
	"os"
	"time"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/plotutil"
	"gonum.org/v1/plot/vg"
)

// Implementation of smoothstep as described here:
//
// https://en.wikipedia.org/wiki/Smoothstep
func Smoothstep(x float64) float64 {
	if x <= 0 {
		return 0
	}

	if 1 <= x {
		return 1
	}

	return 3*math.Pow(x, 2) - 2*math.Pow(x, 3)
}

// generate a plot of the smoothstep function
func PlotSmoothstep(outfile string) {
	p, err := plot.New()
	if err != nil {
		panic(err)
	}

	npoints := 500

	points := make(plotter.XYs, npoints)
	for i := range points {
		points[i].X = float64(i) / float64(npoints)
		points[i].Y = Smoothstep(points[i].X)
	}

	p.Title.Text = "Smoothstep"
	p.X.Label.Text = "X"
	p.Y.Label.Text = "Y"

	err = plotutil.AddLinePoints(p, "Smoothstep", points)
	if err != nil {
		panic(err)
	}

	err = p.Save(6*vg.Inch, 4*vg.Inch, outfile)
	if err != nil {
		panic(err)
	}
}

type Vec2 struct {
	X float64
	Y float64
}

func (u Vec2) Dot(v Vec2) float64 {
	return u.X*v.X + u.Y*v.Y
}

func (u Vec2) Magnitude() float64 {
	return math.Sqrt(math.Pow(u.X, 2) + math.Pow(u.Y, 2))
}

func (u Vec2) Unit() Vec2 {
	mag := u.Magnitude()
	return Vec2{u.X / mag, u.Y / mag}
}

func RandomVec2() Vec2 {
	return Vec2{rand.Float64(), rand.Float64()}
}

func RandomUnit() Vec2 {
	return RandomVec2().Unit()
}

func (u Vec2) Distance(v Vec2) Vec2 {
	// this is the same thing as subtraction, it calculates a vector
	// which points from the tip of u to the tip of v
	return Vec2{u.X - v.X, u.Y - v.Y}
}

// 2-D row-major grid of vectors in R2
type Grid struct {
	Elements []Vec2
	Rows     int
	Cols     int
}

func MakeGrid(rows, cols int) *Grid {
	return &Grid{
		Elements: make([]Vec2, (rows+1)*(cols+1)),
		Rows:     rows,
		Cols:     cols,
	}
}

func (g *Grid) At(row, col int) Vec2 {
	return g.Elements[row*g.Cols+col]
}

func (g *Grid) Set(row, col int, v Vec2) {
	g.Elements[row*g.Cols+col] = v
}

// make a grid of randomized gradient vectors
func MakeGradient(rows, cols int) *Grid {
	g := MakeGrid(rows, cols)
	for i := range g.Elements {
		g.Elements[i] = RandomUnit()
	}

	return g
}

// Return the perlin noise at the given point, assuming that the given gradient
// vector grid has the given width and height. In words, the gradient vector
// grid is mapped onto a space of size width by height.
func perlin(x, y, width, height float64, g *Grid) float64 {

	// project x, y into the coordinate space of the grid
	gridX := float64(g.Cols) * x / width
	gridY := float64(g.Rows) * y / height

	// calculate the gradient vectors for the four adjacent spaces on
	// the gradient grid
	gnw := g.At(int(math.Floor(gridY)), int(math.Floor(gridX)))
	gne := g.At(int(math.Floor(gridY)), int(math.Floor(gridX)+1))
	gsw := g.At(int(math.Floor(gridY)+1), int(math.Floor(gridX)))
	gse := g.At(int(math.Floor(gridY)+1), int(math.Floor(gridX)+1))

	// calculate the distance vectors for the four adjacent spaces on the
	// gradient grid
	dnw := Vec2{gridX, gridY}.Distance(Vec2{math.Floor(gridY), math.Floor(gridX)})
	dne := Vec2{gridX, gridY}.Distance(Vec2{math.Floor(gridY), math.Ceil(gridX)})
	dsw := Vec2{gridX, gridY}.Distance(Vec2{math.Ceil(gridY), math.Floor(gridX)})
	dse := Vec2{gridX, gridY}.Distance(Vec2{math.Ceil(gridY), math.Ceil(gridX)})

	// compute a weighted average of all of the other values
	perlin := gnw.Dot(dnw)*Smoothstep(dnw.Magnitude()) +
		gne.Dot(dne)*Smoothstep(dne.Magnitude()) +
		gsw.Dot(dsw)*Smoothstep(dsw.Magnitude()) +
		gse.Dot(dse)*Smoothstep(dse.Magnitude())

	return perlin
}

func PlotPerlin(gridRows, gridCols, width, height float64, outfile string) {
	img := image.NewRGBA(image.Rectangle{image.Point{0, 0}, image.Point{int(width), int(height)}})

	g := MakeGradient(int(gridRows), int(gridCols))
	for x := 0; x < int(width)-1; x++ {
		for y := 0; y < int(height); y++ {
			p := perlin(float64(x), float64(y), width, height, g)

			// map perlin noise to greyscale
			c := color.RGBA{uint8(p * 255), uint8(p * 255), uint8(p * 255), 255}
			img.Set(x, y, c)
		}
	}

	f, _ := os.Create(outfile)
	png.Encode(f, img)

}

func main() {
	fmt.Println("")
	rand.Seed(time.Now().UTC().UnixNano())

	PlotSmoothstep("smoothstep.pdf")

	PlotPerlin(1, 1, 1920, 1080, "perlin.png")

}
