
/*
Name: Rowan
Surname: Van Zyl
Student Number: 25939831
Module: COS2611
Assignment: Assessment 5 (FPP)

Please see my github repo for more info: https://github.com/PastorRowan/COS2611-Assessment-5.git
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <regex>

// Helpers

/*
clearScreen

Clears the terminal screen on Windows, Linux and MacOs
*/
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
};

/*
getUserInput

Waits for the user to complete a line of input into the console

Returns the complete line as a string
*/
std::string getUserInput(
    bool returnNullWhenEmpty = true
) {

    std::string userInputString = "";

    std::getline(
        std::cin,
        userInputString,
        '\n'
    );

    if (returnNullWhenEmpty && userInputString.empty()) {
        userInputString = '\0';
    };

    return userInputString;

};

void waitForEnter() {

    // Prompt
    std::cout << "Press enter to continue..." << std::endl;

    std::string dummyStr = "";
    std::getline(
        std::cin,
        dummyStr,
        '\n'
    );

};

class IdGenerator {

    private:

        long long currentId = 0;

        void setCurrentId(long long newCurrentId) {
            currentId = newCurrentId;
        };

    public:

        IdGenerator() {};

        long long getCurrentId() const {
            return currentId;
        };

        long long getNextId() {
            currentId++;
            return currentId;
        };

};

enum ServiceType {
    Water,
    Electricity,
    Roads,
    Sanitation,
    PublicFacilities,
    Count
};

class ResponseTeam {

    public:

        enum Status {
            Available,
            Assigned,
            Unavailable,
            Count
        };

    private:

        long long id;

        ServiceType capability;

        Status status;

    public:

        ResponseTeam() {};

};


class Incident {

    public:

        enum Severity {
            Low,
            Medium,
            High,
            Count
        };

        enum Status {
            Open,
            Count
        };

    private:

        long long id;
        // std::string location;
        ServiceType category;
        int severity;

    public:

        Incident() {};

};

struct Road {
    const unsigned int to;
    const unsigned int weight;
};

typedef std::vector<Road> Roads;

struct Location {
    const long long id;
    const std::string name;
    const std::string  shortName;
    const Roads roads;
};

typedef std::vector<Location> Locations;

class RoadNetwork {

    private:

        const Locations locations;

    public:

        RoadNetwork(
            const Locations locationsP
        ): locations(locationsP) {};

        const Locations& getLocations() const {
            return locations;
        };

        bool isLocationIndexStringValid (
            const std::string& locationIndex
        ) const;

        std::string toString() const;

        std::string locationsToString() const;

        /*
        std::string BFSPathToString(
            const int& start
        ) const;
        */

        std::string shortestPathToString(
            const unsigned int& from,
            const unsigned int& to
        ) const;

};

bool RoadNetwork::isLocationIndexStringValid (
    const std::string& locationIndex
) const {

    // Regular expression that only matches strings containing numbers in the range of 0-999
    static const std::regex onlyDigitsRegEx("^[0-9]{1,3}$");

    // If regex does not match then return false
    if (!std::regex_match(locationIndex, onlyDigitsRegEx)) {
        return false;
    };

    // Convert park index string into an index for comparison
    const int locationIndexAsInt = std::stoul(locationIndex);

    // If park index is greater than the size of national parks then return false
    if (locationIndexAsInt >= locations.size()) {
        return false;
    };

    // Else return true
    return true;

};

std::string RoadNetwork::toString() const {

    // Build a human readable representation of the Conservation Network
    std::string output = "Road Network:\n\n";

    // Loop over all nodes in the adjacency list
    for (const auto& currentLocation : locations) {

        // Get current node data
        const std::string& currentLocationName = currentLocation.name;
        const Roads& currentLocationRoads = currentLocation.roads;

        // Loop over the current node's edges
        for (const auto& currentRoad : currentLocationRoads) {

            // Get current edge data;
            const int& currentNeighborIndex = currentRoad.to;
            const int& currentRoadWeight = currentRoad.weight;

            // Get current neighbor data
            const Location& currentNeighbor = locations.at(currentNeighborIndex);
            const std::string& currentNeighborName = currentNeighbor.name;

            // append edge data to console
            output += currentLocationName + " -> " + currentNeighborName + " (" + std::to_string(currentRoadWeight) + " km)\n";

        };

        output += '\n';

    };

    return output;

};

std::string RoadNetwork::locationsToString() const {

    // Build a human readable representation of the adjacency list
    std::string output = "";

    // Create the header row containing the short names of all locations
    for (const auto& currentLocation : locations) {
        const std::string& currentLocationShortName = currentLocation.shortName;
        // Add park's short name to column
        output += '\t' + currentLocationShortName;
    };

    // Move console cursor to the 1st row of the table
    output += '\n';

    // Create one park row at a time by iterating over the locations of the adajency matrix
    for (const auto& currentLocation : locations) {

        const std::string& currentLocationShortName = currentLocation.shortName;
        const Roads& currentRoads = currentLocation.roads;

        // output the current nodes short name and move console cursor one tab space (next column in table)
        output += currentLocationShortName + '\t';

        // Create a row entry of the current park by checking whether an edge exists from it
        // to every other park
        for (unsigned int j = 0; j < locations.size(); ++j) {

            const unsigned int& currentOtherLocationIndex = j;

            int weight = 0;

            // Search the current park's edge list for a connection to the destination location
            for (const auto& road : currentRoads) {

                // If edge from current park to column park exists
                // then set the weight to the edges weight and break the loop
                if (road.to == currentOtherLocationIndex) {
                    weight = road.weight;
                    break;
                };

            };

            // Enter edge weight entry then move the cursor one tab space (next column in table)
            output += std::to_string(weight) + '\t';

        };

        // Move console cursor to next row of the table
        output += '\n';

    };

    return output;

};

/*
shortestPathToString

Returns a string representing the shortest path from one park to another

Uses Dijkstra's algorithm

Parameters:
from - index of the starting location in the adjacency list
to - index of the destination location in the adjacency list
*/
std::string RoadNetwork::shortestPathToString(
    const unsigned int& from,
    const unsigned int& to
) const {

    // Ensure the starting park index is valid
    if (from >= locations.size()) {
        std::string errorMsg = "from index '" + std::to_string(from) + "' is not from 0 to '" + std::to_string(locations.size() - 1) + "' (inclusive)";
        return errorMsg;
    };

    // Ensure the starting park index is valid
    if (to >= locations.size()) {
        std::string errorMsg = "to index '" + std::to_string(to) + "' is not from 0 to '" + std::to_string(locations.size() - 1) + "' (inclusive)";
        return errorMsg;
    };

    // Value representing an infinite distance
    static const unsigned int INF = UINT_MAX;

    // Sentinel value indicating that a park has no predecessor
    static const unsigned int NULL_PREV = UINT_MAX;

    // List of parks that have not yet been processed
    std::vector<unsigned int> queue;
    queue.reserve(locations.size());

    // Stores the shortest known distance from the start park to each park
    std::vector<unsigned int> dist(locations.size(), INF);

    // Stores the predecessor of each park in the shortest path
    std::vector<unsigned int> prev(locations.size(), NULL_PREV);

    // Initially place every park into the unvisited queue
    for (unsigned int i = 0; i < locations.size(); ++i) {
        const unsigned int& currentLocationIndex = i;
        queue.push_back(currentLocationIndex);
    };

    // The distance from the start location to itself is zero
    dist.at(from) = 0;

    // Continue until every location has been processed
    while (!queue.empty()) {

        // Find the unvisited location with the smallest known distance
        unsigned int closestLocationIndex = queue.front();
        for (unsigned int i = 0; i < queue.size(); ++i) {
            const unsigned int& currentLocationIndex = queue.at(i);
            const unsigned int& currentDistance = dist.at(currentLocationIndex);
            if (dist.at(currentLocationIndex) < dist.at(closestLocationIndex)) {
                closestLocationIndex = currentLocationIndex;
            };
        };

        // Remove the selected park from the unvisited queue
        auto it = std::find(queue.begin(), queue.end(), closestLocationIndex);
        if (it != queue.end()) {
            queue.erase(it);
        };

        // Skip if distance is INF (no path to this location)
        if (dist.at(closestLocationIndex) == INF) {
            continue;
        };

        const Location& closestLocation = locations.at(closestLocationIndex);
        const Roads& roads = closestLocation.roads;

        // Attempt to improve the shortest known distance to each neighbouring park
        for (unsigned int i = 0; i < roads.size(); ++i) {

            const unsigned int& currentRoadIndex = i;
            const Road& currentRoad= roads.at(currentRoadIndex);

            const unsigned int& currentNeighborIndex = currentRoad.to;
            const unsigned int& distanceFromCurrentLocationToNeighbor = currentRoad.weight;

            // Distance to the neighbour if travelling through the current park
            const unsigned int alternativeDistance = dist.at(closestLocationIndex) + distanceFromCurrentLocationToNeighbor;

            // Update the neighbour's distance and predecessor if a shorter route was found
            if (alternativeDistance < dist.at(currentNeighborIndex)) {
                dist.at(currentNeighborIndex) = alternativeDistance;
                prev.at(currentNeighborIndex) = closestLocationIndex;
            };
        };

    };

    // Reconstruct the shortest path by following predecessor links backwards
    std::vector<unsigned int> path = {};
    path.reserve(locations.size());

    unsigned int current = to;

    if (prev.at(current) != NULL_PREV || current == from) {
        while (current != from) {
            path.push_back(current);
            current = prev.at(current);
        };
        // Add the starting park
        path.push_back(from);
    };

    // Build a human readable representation of the shortest path
    std::string output = "";

    const Location& fromLocation = locations.at(from);
    const Location& toLocation = locations.at(to);

    output += "shortest route from " + fromLocation.name + " to " + toLocation.name + "\n\n";

    // Report when no path exists between the selected parks
    if (path.empty()) {
        output += "No path exists between " + fromLocation.name + " and " + toLocation.name;
        return output;
    };

    // Append each park in the path from start to destination
    for (unsigned int i = path.size() - 1; i < path.size(); --i) {

        const unsigned int& currentLocationIndex = path.at(i);
        const Location& currentLocation = locations.at(currentLocationIndex);
        const std::string& currentLocationName = currentLocation.name;

        output += currentLocationName + (i == 0 ? "" : " -> ");

    };

    // Show the total length of the shortest path
    output += "\n\n" + std::string("Total distance: ") + std::to_string(dist.at(to)) + "km";

    return output;

};

const unsigned int SEED = 0;

int main() {

    enum LocationIndex {
        FISH_HOEK,
        NOORDHOEK,
        SIMONS_TOWN,
        GLENCAIRN,
        KALK_BAY,
        MUIZENBERG,
        KOMMETJIE,
        SCARBOROUGH,
        CASTLE_ROCK,
        CAPE_POINT,
        Count
    };

    RoadNetwork roadNetwork({
        {
            .name = "Fish Hoek",
            .shortName = "FH",
            .roads = {
                { .to = NOORDHOEK, .weight = 120 },
                { .to = SIMONS_TOWN, .weight = 120 }
            }
        },
        {
            .name = "Noordhoek",
            .shortName = "N",
            .roads = {
                { .to = FISH_HOEK, .weight =  120 },
                { .to = KOMMETJIE, .weight = 120 },
                { .to = GLENCAIRN, .weight = 120 }
            }
        },
        {
            .name = "Simon's Town",
            .shortName = "ST",
            .roads = {
                { .to = GLENCAIRN, .weight = 120 },
                { .to = SCARBOROUGH, .weight = 120 },
                { .to = CASTLE_ROCK, .weight = 120 },
                { .to = MUIZENBERG, .weight = 120 }
            }
        },
        {
            .name = "Glencairn",
            .shortName = "G",
            .roads = {
                { .to = FISH_HOEK, .weight = 120 },
                { .to = SIMONS_TOWN, .weight = 120 },
                { .to = KOMMETJIE, .weight = 120 },
                { .to = CASTLE_ROCK, .weight = 120 },
                { .to = CAPE_POINT, .weight = 120 }
            }
        },
        {
            .name = "Kalk Bay",
            .shortName = "KB",
            .roads = {
                { .to = FISH_HOEK, .weight = 120 },
                { .to = MUIZENBERG, .weight = 120 }
            }
        },
        {
            .name = "Muizenberg",
            .shortName = "M",
            .roads = {
                { .to = KALK_BAY, .weight = 120 },
                { .to = NOORDHOEK, .weight = 120 }
            }
        },
        {
            .name = "Kommetjie",
            .shortName = "K",
            .roads = {
                { .to = FISH_HOEK, .weight = 120 },
                { .to = NOORDHOEK, .weight = 120 },
                { .to = SCARBOROUGH, .weight = 120 }
            }
        },
        {
            .name = "Scarborough",
            .shortName = "S",
            .roads = {
                { .to = KOMMETJIE, .weight = 120 },
                { .to = GLENCAIRN, .weight = 120 },
                { .to = SIMONS_TOWN, .weight = 120 },
                { .to = CAPE_POINT, .weight = 120 }
            }
        },
        // SCARBOROUGH
        {
            .name = "Castle Rock",
            .shortName = "CR",
            .roads = {
                { .to = SIMONS_TOWN, .weight = 120 },
                { .to = SCARBOROUGH, .weight = 120 },
                { .to = CAPE_POINT, .weight = 120 }
            }
        },
        {
            .name = "Cape Point",
            .shortName = "CP",
            .roads = {
                { .to = CASTLE_ROCK, .weight = 120 },
                { .to = SCARBOROUGH, .weight = 120 }
            }
        }
    });

    bool exit = false;

    const std::string HEADER =
        "\n"
        "Programmer: Rowan Van Zyl\n"
        "Student number: 25939831\n";

    const std::string MAIN_MENU_TITLE = 
        "========================================\n"
        " MUNICIPAL INCIDENT RESPONSE SYSTEM\n"
        "========================================\n"
    ;

    const std::string MAIN_MENU_OPTIONS =
        "\n"
        "1. Display Locations and Roads\n"
        "2. Display Incidents\n"
        "3. Prioritise Open Incidents\n"
        "4. Display Response Teams\n"
        "5. Investigate Incident\n"
        "6. Find Route\n"
        "7. Update Incident\n"
        "8. Display Summary\n"
        "0. Exit\n"
        "\n"
        "Enter your choice: "
    ;

    const std::string MAIN_MENU =
        MAIN_MENU_TITLE +
        MAIN_MENU_OPTIONS
    ;

    std::string input = "";

    char inputChar = ' ';

    do {

        clearScreen();

        std::cout
            << HEADER
            << MAIN_MENU
        ;

        input = getUserInput();

        inputChar = input.at(0);

        switch (inputChar) {

            case '1':
                break;

            case '2':
                break;

            case '3':
                break;

            case '4':
                break;

            case '5':
                break;

            case '6':
                break;

            case '7':
                break;

            case '8':
                break;

            case '0':
                std::cout
                    << std::endl
                    << "Gracefully Exiting application..."
                    << std::endl
                ;
                exit = true;
                break;

            default:
                std::cout
                    << std::endl
                    << "Error: input is not a valid option (1, 2, 3, 4, 5, 6, 7, 8 or 0)" << std::endl
                ;
                waitForEnter();
                break;

        };

    } while (!exit);

};
