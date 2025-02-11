# Conway's Game of Life

## Overview
This project implements Conway's Game of Life in C, utilizing 2D arrays, dynamic memory allocation, and file I/O. It allows for various configurations, performance measurement, and debugging using GDB and Valgrind.

## Features
- Supports toroidal (wrap-around) and non-toroidal grids.
- Displays the simulation at different speeds.
- Reads initial configurations from a file.
- Measures execution time of the simulation.

## Compilation & Execution
### Build
```sh
make
```

### Run
```sh
./gol <config_file> <wrap|nowrap> <show|hide> [slow|med|fast]
```
#### Examples
```sh
./gol file1.txt wrap hide
./gol file2.txt nowrap show fast
```

## Input File Format
```
<number of rows>
<number of columns>
<number of iterations>
<i> <j>  # Live cell coordinates
...
```
Example (glider pattern):
```
30
30
100
29 1
28 2
27 0
27 1
27 2
```

## Implementation Details
The project is organized into four source files:
- `gol.c`: Contains `main()`.
- `gol_cmd.c`: Parses command-line arguments.
- `gol_io.c`: Handles file I/O and board printing.
- `gol_sim.c`: Runs the simulation logic.

### Timing Execution
The `gettimeofday` function is used to measure the execution time of the simulation.

## Debugging
Use GDB and Valgrind to debug memory errors:
```sh
gdb ./gol
valgrind --leak-check=full ./gol file.txt wrap hide
```

## Code Style & Best Practices
- No global variables; use function parameters.
- Functions should ideally be ≤ 20 lines.
- Use `malloc()` for dynamic memory allocation.
- Handle file errors gracefully.
- Use `usleep()` and `system("clear")` for animations.

## Additional Notes
- `CTRL+C` terminates an infinite loop.
- Use `atoi()` to convert command-line arguments.
