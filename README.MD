
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

## Requirements

- C++17 compatible compiler (e.g., `g++`)
- No external libraries or frameworks.
- Runs as a console application on Windows, Linux, or macOS.

## Compilation

Compile the single source file using:

```
g++ -std=c++17 25939831_FPP.cpp -o municipal_incident_system
```

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
