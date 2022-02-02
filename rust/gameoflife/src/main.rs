use std::{thread, time::Duration};
use rand::Rng;

// function to compute the next generation
fn gol(grid: &Vec<Vec<i8>>) -> Vec<Vec<i8>> {

    // get the number of rows
    let n = grid.len();

    // get the number of columns
    let m = grid[0].len();

    // create an empty grid to compute the future generation
    let mut future: Vec<Vec<i8>> = vec![vec![0; n]; m];

    // iterate through each and every cell
    for i in 0..n {
        for j in 0..m {

            // the current state of the cell (alive / dead)
            let cell_state = grid[i][j];

            // variable to track the number of alive neighbors
            let mut live_neighbors = 0;

            // iterate through every neighbors including the current cell
            for x in -1i8..=1 {
                for y in -1i8..=1 {

                    // position of one of the neighbors (new_x, new_y)
                    let new_x = (i as i8) + x;
                    let new_y = (j as i8) + y;

                    // make sure the position is within the bounds of the grid
                    if new_x > 0 && new_y > 0 && new_x < n as i8 && new_y < m as i8 {
                        live_neighbors += grid[new_x as usize][new_y as usize];
                    }
                }
            }

            // substract the state of the current cell to get the number of alive neighbors
            live_neighbors -= cell_state;

            // applying the rules of game of life to get the future generation
            if cell_state == 1 && live_neighbors < 2 {
                future[i][j] = 0;
            } else if cell_state == 1 && live_neighbors > 3 {
                future[i][j] = 0;
            } else if cell_state == 0 && live_neighbors == 3 {
                future[i][j] = 1;
            } else {
                future[i][j] = cell_state;
            }
        }
    }

    // return the future generation
    future
}

// main function
fn main() {
    let mut rng = rand::thread_rng();

    // set the number of rows and columns of the grid
    let (rows, cols) = (24, 24);

    // create the grid
    let mut grid: Vec<Vec<i8>> = vec![vec![0; cols]; rows];

    // set the initial state of the grid (blinker)
    grid[1][2] = 1;
    grid[2][2] = 1;
    grid[3][2] = 1;

    for y in 0..grid.len() {
        for x in 0..grid[y].len() {
            grid[y][x] = rng.gen_range(0..2);
        }
    }

    // compute and print the next generation
    loop {
        grid = gol(&grid);
        grid.iter().for_each(|i| {
            println!("{:?}", i);
        });

        thread::sleep(Duration::from_millis(50));
    };
}
