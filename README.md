# Network Routing Simulation

## Overview

This project is a **Network Routing Simulation** implemented in C. It processes routing information from multiple input files and simulates network pathfinding using structured data storage and efficient processing techniques. The program models a simplified network with nodes and links, dynamically updating routing decisions based on provided data.

## Features

- **Data Parsing**: Reads network topology and routing information from structured input files.
- **Dynamic Memory Management**: Uses efficient allocation and deallocation techniques to manage node and link structures.
- **Routing Simulation**: Implements routing logic based on shortest path principles and link-cost calculations.
- **Modular Design**: Functions are structured across multiple source files for maintainability and scalability.

## Technical Concepts Applied

### 1. **File Handling & Data Parsing**

- Uses file I/O (`fopen`, `fgets`, `fscanf`) to read network information from `routeinfo1.txt`, `rerouteinfo.txt`, `nodeinfo1.txt`, and `linkinfo1.txt`.
- Tokenizes and processes each line to extract relevant routing data.

### 2. **Graph Representation of the Network**

- Nodes represent network devices identified by names (e.g., `nodeA`, `nodeB`).
- Links between nodes are represented with weights (costs) that define the path efficiency.
- Data is stored in **linked lists** or **adjacency matrices** to model the network topology efficiently.

### 3. **Routing Logic**

- The program reads routing data and applies decision-making algorithms to determine optimal paths.
- Each routing entry consists of a source, a destination, an intermediate node, and a cost.
- The simulation can dynamically update the network when reroute data is provided.

### 4. **Memory Management**

- **Dynamic Allocation**: Structures such as nodes and links are allocated dynamically using `malloc`.
- **Deallocation**: Implements proper memory cleanup with `free()` to avoid memory leaks.

## Files

- `main.c` - The main driver of the program, handling input/output and invoking functions.
- `a2_functions.c` - Implements core logic for processing and managing routing data.
- `a2_functions.h` - Header file containing function prototypes and data structures.
- `routeinfo1.txt` - Contains initial routing data (source, destination, next hop, cost).
- `rerouteinfo.txt` - Defines alternative routes for rerouting scenarios.
- `nodeinfo1.txt` - Provides mappings of node names to IP addresses.
- `linkinfo1.txt` - Details network link costs and bandwidth.

## How It Works

1. **Reading Input Files**

   - The program starts by reading `nodeinfo1.txt` to store node details.
   - It then reads `linkinfo1.txt` to build the network graph.
   - Routing information is loaded from `routeinfo1.txt` and processed into a routing table.

2. **Processing Routes & Reroutes**

   - Each routing entry is stored and validated against available links.
   - If `rerouteinfo.txt` is provided, the program updates the routing table accordingly.
   - The simulation then processes routing decisions based on shortest-path calculations.

3. **Generating Output**

   - The final routing table is printed, displaying source, destination, next hop, and cost.
   - The program ensures optimized routing paths are used based on provided data.

## Requirements

- GCC Compiler
- A Linux/Mac terminal or MinGW for Windows

## Compilation & Execution

To compile and run the program, use the following steps:

```sh
# To compile and run the project, use the following command in the terminal:
gcc main.c a2_functions.c -o network_simulator

# Then, run the compiled program (pre-compiled executable main.exe is available):
./network_simulator
```

## Author

**Mithushan Ravichandramohan**

## License

This project is released under the MIT License.

---

