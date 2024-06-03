# Warehouse Simulation Project

## Overview

This project was given to us as a second year assignment in the course Systems Programming, written by [Idan Saltzman](https://github.com/idsa0) and [Tomer Kandel](https://github.com/teramisuskandel).

This project is a Warehouse Simulation designed to manage customers, volunteers, and orders. The simulation includes handling various types of volunteers and customers, processing orders, and maintaining warehouse operations. This project adheres to specific implementation guidelines and requirements to ensure correct functionality and performance.

## Project Structure

The project is structured as follows:
- `src/`: Contains all the `.cpp` source files.
- `include/`: Contains all the header `.h` files.
- `bin/`: Intended for the compiled binaries (empty upon submission).
- `makefile`: Used to compile the project and create an executable named `warehouse`.

## Installation Instructions

1. **Compile the Project**:
   Use the provided `makefile` to compile the project. Run the command `make` in the project directory.
   
   This command compiles the `.cpp` files into the `bin/` folder and creates an executable named `warehouse`.

2. **Run the Simulation**:
   Execute the program with different input files using the  command `bin/warehouse input_file.txt`.

## Usage Instructions

### Input File Format
The input file should contain the initial program data in the following order:
1. **Customers**:
   ```bash
   customer <customer_name> <customer_type> <customer_distance> <max_orders>
   ```
   Example:
   ```bash
   customer Maya soldier 4 2
   customer David civilian 3 1
   ```

2. **Volunteers**:
   ```bash
   volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step> <volunteer_maxOrders>
   ```
   
   Example:
   ```bash
   volunteer Noya collector 2
   volunteer Ibrahim limited_collector 3 2
   volunteer Din limited_driver 13 4 2
   volunteer Limor driver 8 3
   ```

### Actions
The simulation supports various actions such as creating orders, simulating steps, and printing status. The syntax for these actions is defined within the project documentation.
