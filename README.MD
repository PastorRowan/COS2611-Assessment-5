
# Municipal Incident Response System

**COS2611 – Assessment 5 Final Practical Project**  
**Student:** Rowan Van Zyl  
**Student Number:** 25939831  

## Description

A C++ console application that assists a municipal operations centre in managing and responding to incidents across a small, realistic region in the Cape Peninsula, South Africa. The system models a fixed road network between 10 real locations, generates a set of incidents and response teams programmatically, and provides an interactive menu for prioritisation, routing, team assignment, and status tracking.

## Features

- **Hardcoded locations:** 10 real towns/suburbs in the Cape Peninsula:
  Fish Hoek, Noordhoek, Simon's Town, Glencairn, Kalk Bay, Muizenberg, Kommetjie, Scarborough, Hout Bay, Smit Winkel Bay.
- **Road network:** 18 undirected road connections with realistic travel times (4–31 minutes), all within the required 5–60 minute range.
- **Random operational data:** Uses a seed to generate:
  - 6 response teams with random capability, starting location, and status.
  - 10 incidents with random location, category, and severity.
- **Menu-driven interface:** 8 functional options plus exit.
- **Incident prioritisation:** Open incidents sorted by severity (High → Medium → Low) and then by ID descending (tie-breaker).
- **Route finding:** Uses Dijkstra’s algorithm to compute the shortest path (minimum travel time) between any two locations.
- **Team assignment:** Suggests suitable available teams based on matching capability; updates incident and team statuses consistently.
- **Summary display:** Counts of incidents and teams by status.

## Project Structure

```
project-root/
│
├── scripts/                        # Helper scripts for compilation and automated testing
│   ├── clean_run.bat               # Deletes previous executable, compiles, and runs the application
│   ├── compile.bat                 # Compiles the application using g++
│   ├── run.bat                     # Runs the compiled application
│   └── test.bat                    # Runs the automated application tests
│
├── .gitignore.cpp                  # Git ignore rules for project
├── 25939831_FPP.cpp                # Complete C++ application source code
├── COS2611-26-Y_Assessment 5.pdf   # COS2611 Assessment 5 assignment specification
├── LICENSE                         # Project licence
└── README.md                       # Project documentation
```

## Requirements

- C++20 compatible compiler (e.g., `g++`)
- No external libraries or frameworks.
- Runs as a console application on Windows, Linux, or macOS.

## Compilation

The application is provided as a single C++ source file and requires a C++20-compatible compiler such as `g++`.

For example the command I used to compile the app is:
```
g++ -std=c++20 25939831_FPP.cpp -o 25939831_FPP.exe
```

This command:
- Uses the **C++20** language standard.
- Compiles `25939831_FPP.cpp`.
- Produces the executable `25939831_FPP.exe`

After compilation, run the program with:

./25939831_FPP.exe

Alternatively, the provided batch script can be used to compile the application automatically:

scripts/compile.bat

This script will only work if you have the mingw toolchain installed with the corresponding g++ compiler and have g++ available on your system path.

## Usage

Run the executable with optional command-line arguments:

```
./25939831_FPP.exe [SEED] [NUMBER_OF_REPONSE_TEAMS] [NUMBER_OF_INCIDENTS]
```

- SEED - unsigned integer for random number generation (default 1).
To reproduce the exact scenario required by the assignment, use 2611.

- NUMBER_OF_REPONSE_TEAMS - number of response teams to generate (default 6).

- NUMBER_OF_INCIDENTS - number of incidents to generate (default 10).

#### Example:

```
./25939831_FPP 2611 6 10
```

If no arguments are given, the program uses the defaults listed above. Invalid arguments result in an error message.

Menu Options

| **Option** | **Description** |
|---|---|
| **1** | **Display Locations and Roads** – Shows the road network with travel times. |
| **2** | **Display Incidents** – Lists all incidents with their details. |
| **3** | **Prioritise Open Incidents** – Shows open incidents sorted by priority (severity). |
| **4** | **Display Response Teams** – Lists all teams with capability, location, and status. |
| **5** | **Investigate Incident** – Enter an incident ID to view its details and suitable available teams. |
| **6** | **Find Route** – Select two locations (by index) to get the shortest path and total travel time. |
| **7** | **Update Incident** – Change an incident’s status: assign/unassign team, resolve, or reopen. |
| **8** | **Display Summary** – Shows counts of open, assigned, resolved incidents and available, assigned, unavailable teams. |
| **0** | **Exit** – Terminates the program. |

## Testing

The application was tested using a combination of **manual testing** and an **automated batch test script**. Testing was performed against the complete compiled application to verify both individual menu options and the interaction between different parts of the system.

### Test Coverage

The following functionality was tested:

- **Program startup**
  - Verified that the application starts correctly with default arguments.
  - Verified that command-line arguments can be used to specify the random seed, number of response teams, and number of incidents.
  - Verified that invalid command-line arguments are rejected with an appropriate error message.

- **Locations and road network**
  - Verified that all 10 predefined locations are displayed correctly.
  - Verified that all predefined roads are displayed with their corresponding travel times.
  - Verified that road connections are treated as undirected connections.
  - Verified that the road network is connected.

- **Incident management**
  - Verified that incidents are generated with valid locations, categories, and severities.
  - Verified that all incidents initially have an Open status.
  - Verified that incidents can be investigated using their ID.
  - Verified that incidents can be assigned, unassigned, resolved, and reopened where appropriate.
  - Verified that invalid incident IDs are handled correctly.

- **Incident prioritisation**
  - Verified that only open incidents are included in the prioritised list.
  - Verified that incidents are ordered from highest to lowest severity.
  - Verified that incidents with equal severity are ordered by descending incident ID.

- **Response team management**
  - Verified that the configured number of response teams is generated.
  - Verified that teams receive valid locations, capabilities, and statuses.
  - Verified that at least three response teams are initially available.
  - Verified that suitable available teams are identified based on their service capability.
  - Verified that assigning and unassigning teams correctly updates both the team and incident status.

- **Route finding**
  - Verified that valid routes between locations produce a shortest path and total travel time.
  - Verified that Dijkstra's algorithm produces the minimum-travel-time route.
  - Verified that a location cannot be selected outside the valid location range.
  - Verified that invalid route selections are handled without terminating the application.

- **Summary**
  - Verified that the summary correctly counts incidents by status.
  - Verified that the summary correctly counts response teams by status.
  - Verified that the displayed counts change appropriately after incident and team status updates.

- **Menu and input handling**
  - Verified that all menu options can be selected.
  - Verified that invalid menu selections are rejected.
  - Verified that the application remains operational after invalid input.
  - Verified that the exit option terminates the application correctly.

### Automated Testing

A Windows batch script is included with the project to automate testing of the console application's menu functionality.

The script supplies predefined input to the executable through standard input (`stdin`). This allows repeatable sequences of menu selections and user input to be tested without manually entering every value.

The automated test script also includes delays where necessary to allow the application to process input before the next test input is supplied.

The test script can be found at:

```
scripts/test.bat
```

## Design Overview

- **Locations** are Defined using an `enum class LocationIndex` and parallel arrays are used to store location names and short names.
- **Road Network** is stored as an adjacency list as `std::vector<Road>`. Each `Road` contains the destination index and travel time (in minutes).
- **Incidents** and **Response Teams** are represented as classes with internal state structs.
Managers (`IncidentsManager`, `ResponseTeamManager`) handle storage, lookup, and updates.
- **Random generation** uses `std::mt19937` seeded from the command-line argument (or default).
- **Prioritisation** is implemented by overloading `operator<` for incidents (severity then ID) and sorting open incidents in reverse order.
- **Route finding** uses Dijkstra’s algorithm with a simple linear scan of unvisited nodes (adequate for less than 100)
- **Consistent state updates** are ensured by pairing incident and team operations inside manager methods.

## Data Sources

Real‑world geographical information (locations and road connections) was based on:

- Google Maps (road distances and travel times)

## Reflection

A detailed solution reflection, covering design decisions, challenges, and testing, is included as a comment block at the end of the source file (25939831_FPP.cpp).
