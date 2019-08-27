package main

import (
	"math/rand"

	"github.com/faiface/pixel"
	"github.com/faiface/pixel/imdraw"
	"github.com/faiface/pixel/pixelgl"
	"golang.org/x/image/colornames"
)

func main() {
	pixelgl.Run(runSimulation)
}

const CELLS_PER_ROW = 64
const WINDOW_WIDTH = 640
const WINDOW_HEIGHT = 640
const CELL_SIZE = WINDOW_WIDTH / CELLS_PER_ROW

func runSimulation() {
	windowConfig := pixelgl.WindowConfig{
		Title:  "Game of Life",
		Bounds: pixel.R(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
		VSync:  true,
	}

	windowSurface, err := pixelgl.NewWindow(windowConfig)
	if err != nil {
		panic(err)
	}

	matrix := make([][]uint, CELLS_PER_ROW)

	for i := 0; i < CELLS_PER_ROW; i++ {
		matrix[i] = make([]uint, CELLS_PER_ROW)
	}

	initMatrix(matrix)

	for !windowSurface.Closed() {
		matrix = calculateTransitionMatrix(matrix)
		renderMatrix(matrix, windowSurface)
	}
}

func initMatrix(matrix [][]uint) {
	for x := 0; x < len(matrix); x++ {
		for y := 0; y < len(matrix[x]); y++ {
			matrix[x][y] = uint(rand.Intn(2))
		}
	}
}

func calculateTransitionMatrix(matrix [][]uint) [][]uint {
	nextMatrix := make([][]uint, CELLS_PER_ROW)

	for i := 0; i < CELLS_PER_ROW; i++ {
		nextMatrix[i] = make([]uint, CELLS_PER_ROW)
	}

	for x := 0; x < len(matrix); x++ {
		for y := 0; y < len(matrix[x]); y++ {
			nextMatrix[x][y] = matrix[x][y]

			var counter uint

			neighbours := [8][2]int{
				{x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1},
				{x - 1, y}, {x + 1, y},
				{x - 1, y + 1}, {x, y + 1}, {x + 1, y + 1},
			}

			for _, neighbour := range neighbours {
				if isValidCoordinate(len(matrix), len(matrix[x]), x, y) {
					counter += matrix[neighbour[0]][neighbour[1]]
				}
			}

			if matrix[x][y] == 1 && (counter < 2 || counter > 3) {
				nextMatrix[x][y] = 0
			}

			if matrix[x][y] == 0 && counter == 3 {
				nextMatrix[x][y] = 1
			}
		}
	}

	return nextMatrix
}

func isValidCoordinate(xlen int, ylen int, x int, y int) bool {
	return x > 0 && x < xlen-2 && y > 0 && y < ylen-2
}

func renderMatrix(matrix [][]uint, windowSurface *pixelgl.Window) {
	windowSurface.Clear(colornames.Black)
	for x := 0; x < len(matrix); x++ {
		for y := 0; y < len(matrix[x]); y++ {
			if matrix[x][y] == 1 {
				imd := imdraw.New(nil)

				imd.Color = pixel.RGB(0, 255, 0)
				imd.Push(pixel.V(float64(x*CELL_SIZE), float64(y*CELL_SIZE)))
				imd.Push(pixel.V(float64(x*CELL_SIZE+CELL_SIZE), float64(y*CELL_SIZE)))
				imd.Push(pixel.V(float64(x*CELL_SIZE+CELL_SIZE), float64(y*CELL_SIZE+CELL_SIZE)))
				imd.Push(pixel.V(float64(x*CELL_SIZE), float64(y*CELL_SIZE+CELL_SIZE)))
				imd.Polygon(0)

				imd.Draw(windowSurface)
			}
		}
	}
	windowSurface.Update()
}
