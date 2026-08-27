
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
#include <random>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <regex>
#include <iomanip>
#include <sstream>

const unsigned int SEED = 0;
const unsigned int NUMBER_OF_REPONSE_TEAMS = 6;
const unsigned int NUMBER_OF_INCIDENTS = 10;

// Helpers

// Generates a random unsigned integer between min and max (inclusive)
template <class T = unsigned int>
T ran(const unsigned int min, const unsigned int max) {

    static std::mt19937 gen(SEED);

    std::uniform_int_distribution<unsigned int> dist(min, max);

    return static_cast<T>(dist(gen));

};

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
    RoadWork,
    Sanitation,
    PublicFacilities,
    ServiceTypeCount
};

constexpr const char* serviceTypeStringMap[ServiceTypeCount] = {
    "Water",
    "Electricity",
    "Road Works",
    "Sanitation",
    "Public Facilities"
};

std::string serviceTypeToString(ServiceType serviceType) {
    return serviceTypeStringMap[static_cast<unsigned int>(serviceType)];
};

enum LocationIndex {
    FISH_HOEK,
    NOORDHOEK,
    SIMONS_TOWN,
    GLENCAIRN,
    KALK_BAY,
    MUIZENBERG,
    KOMMETJIE,
    SCARBOROUGH,
    HOUT_BAY,
    SMIT_WINKEL_BAY,
    LocationIndexCount
};

constexpr const char* locationIndexNameMap[LocationIndexCount] = {
    "Fish Hoek",
    "Noordhoek",
    "Simon's Town",
    "Glencairn",
    "Kalk Bay",
    "Muizenberg",
    "Kommetjie",
    "Scarborough",
    "Hout Bay",
    "Smit Winkel Bay"
};

const std::string locationIndexToName(LocationIndex locationIndex) {
    return locationIndexNameMap[static_cast<unsigned int>(locationIndex)];
};

constexpr const char* locationIndexShortNameMap[LocationIndexCount] = {
    "FH",
    "N",
    "ST",
    "G",
    "KB",
    "M",
    "K",
    "S",
    "HB",
    "SWB"
};

const std::string locationIndexToShortName(LocationIndex locationIndex) {
    return locationIndexShortNameMap[static_cast<unsigned int>(locationIndex)];
};

struct Road {
    unsigned int to;
    unsigned int weight;
};

typedef std::vector<Road> Roads;

const Roads locationIndexRoadsMap[LocationIndexCount] = {
        {
            { .to = NOORDHOEK, .weight = 12 },
            { .to = GLENCAIRN, .weight = 7 },
            { .to = KALK_BAY, .weight = 4 },
            { .to = KOMMETJIE, .weight = 14 }
        },
        {
            { .to = FISH_HOEK, .weight =  12 },
            { .to = KOMMETJIE, .weight = 14 },
            { .to = GLENCAIRN, .weight = 14 },
            { .to = MUIZENBERG, .weight = 26 },
            { .to = HOUT_BAY, .weight = 18 }
        },
        {
            { .to = GLENCAIRN, .weight = 6 },
            { .to = SCARBOROUGH, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 14 }
        },
        {
            { .to = FISH_HOEK, .weight = 7 },
            { .to = NOORDHOEK, .weight = 14 },
            { .to = SIMONS_TOWN, .weight = 6 },
            { .to = KOMMETJIE, .weight = 18 },
            { .to = SCARBOROUGH, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 24 }
        },
        {
            { .to = FISH_HOEK, .weight = 4 },
            { .to = MUIZENBERG, .weight = 7 }
        },
        {
            { .to = NOORDHOEK, .weight = 26 },
            { .to = KALK_BAY, .weight = 7 },
            { .to = HOUT_BAY, .weight = 31 }
        },
        {
            { .to = FISH_HOEK, .weight = 14 },
            { .to = NOORDHOEK, .weight = 14 },
            { .to = GLENCAIRN, .weight = 18 },
            { .to = SCARBOROUGH, .weight = 14 }
        },
        {
            { .to = KOMMETJIE, .weight = 14 },
            { .to = GLENCAIRN, .weight = 18 },
            { .to = SIMONS_TOWN, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 18 }
        },
        {
            { .to = NOORDHOEK, .weight = 18 },
            { .to = MUIZENBERG, .weight = 31 }
        },
        {
            { .to = GLENCAIRN, .weight = 24 },
            { .to = SIMONS_TOWN, .weight = 14 },
            { .to = SCARBOROUGH, .weight = 18 }
        }
};

Roads locationIndexToRoads(LocationIndex locationIndex) {
    return locationIndexRoadsMap[static_cast<unsigned int>(locationIndex)];
};

class ResponseTeam {

    public:

        enum Status {
            Available,
            Assigned,
            Unavailable,
            StatusCount
        };

        static constexpr const char* statusStringMap[StatusCount] = {
            "Available",
            "Assigned",
            "Unavailable"
        };

        static std::string statusToString(Status status) {
            return statusStringMap[static_cast<int>(status)];
        };

        struct State {
            long long id;
            LocationIndex locationIndex;
            ServiceType capability;
            Status status;
        };

    private:

        State state;

    public:

        ResponseTeam(State stateP): state(stateP) {};

        const State& getState() const {
            return state;
        };

        std::string toString(
            const unsigned int idWidth = 5,
            const unsigned int locationWidth = 20,
            const unsigned int capabilityWidth = 15,
            const unsigned int statusWidth = 10
        ) const {

            std::ostringstream output;

            output
                << std::left
                << std::setw(idWidth) << state.id
                << std::setw(locationWidth) << locationIndexToName(state.locationIndex)
                << std::setw(capabilityWidth) << serviceTypeToString(state.capability)
                << std::setw(statusWidth) << statusToString(state.status)
            ;

            return output.str();
        };

};

typedef std::vector<ResponseTeam> ResponseTeams;

std::string responseTeamsToString(const ResponseTeams& responseTeams) {

    const unsigned int
        ID_WIDTH = 5,
        LOCATION_WIDTH = 20,
        CAPABILITY_WIDTH = 20,
        STATUS_WIDTH = 10
    ;

    std::string reponseTeamsString = "";

    for (const auto& responseTeam : responseTeams) {
        reponseTeamsString += responseTeam.toString(
            ID_WIDTH,
            LOCATION_WIDTH,
            CAPABILITY_WIDTH,
            STATUS_WIDTH
        ) + '\n';
    };

    std::ostringstream output;

    output
        << std::left
        << std::setw(ID_WIDTH) << "id"
        << std::setw(LOCATION_WIDTH) << "location"
        << std::setw(CAPABILITY_WIDTH) << "capability"
        << std::setw(STATUS_WIDTH) << "status" << std::endl
        << reponseTeamsString << std::endl
    ;

    return output.str();

};

class Incident {

    public:

        enum Severity {
            Low,
            Medium,
            High,
            SeverityCount
        };

        static constexpr const char* severityStringMap[SeverityCount] = {
            "Low",
            "Medium",
            "High"
        };

        static std::string severityToString(Severity severity) {
            return severityStringMap[static_cast<int>(severity)];
        };

        enum Status {
            Open,
            StatusCount
        };

        static constexpr const char* statusStringMap[StatusCount] = {
            "Open"
        };

        static std::string statusToString(Status status) {
            return statusStringMap[static_cast<int>(status)];
        };

        struct State {
            long long id;
            LocationIndex locationIndex;
            ServiceType category;
            Severity severity;
            Status status = Status::Open;
        };

    private:

        State state;

    public:

        Incident(State stateP): state(stateP) {};

        std::string toString(
            const unsigned int idWidth = 5,
            const unsigned int locationWidth = 20,
            const unsigned int categoryWidth = 15,
            const unsigned int severityWidth = 10,
            const unsigned int statusWidth = 10
        ) const {

            std::ostringstream output;

            output
                << std::left
                << std::setw(idWidth)  << state.id
                << std::setw(locationWidth) << locationIndexToName(state.locationIndex)
                << std::setw(categoryWidth) << serviceTypeToString(state.category)
                << std::setw(severityWidth) << severityToString(state.severity)
                << std::setw(statusWidth) << statusToString(state.status)
            ;

            return output.str();

        };

        const State& getState() const {
            return state;
        };

        friend bool operator<(const Incident& left, const Incident& right) {
            if (left.state.severity != right.state.severity) {
                return left.state.severity < right.state.severity;
            } else {
                return left.state.id > right.state.id;
            };
        };

};

typedef std::vector<Incident> Incidents;

std::string incidentsToString(const Incidents& incidents) {

    const unsigned int
        ID_WIDTH = 5,
        LOCATION_WIDTH = 20,
        CATEGORY_WIDTH = 20,
        SEVERITY_WIDTH = 10,
        STATUS_WIDTH = 10
    ;

    std::string incidentsString = "";

    for (const auto& incident : incidents) {
        incidentsString += incident.toString(
            ID_WIDTH,
            LOCATION_WIDTH,
            CATEGORY_WIDTH,
            SEVERITY_WIDTH,
            STATUS_WIDTH
        ) + '\n';
    };

    std::ostringstream output;

    output
        << std::left
        << std::setw(ID_WIDTH) << "id"
        << std::setw(LOCATION_WIDTH) << "location"
        << std::setw(CATEGORY_WIDTH) << "category"
        << std::setw(SEVERITY_WIDTH) << "severity"
        << std::setw(STATUS_WIDTH) << "status" << std::endl
        << incidentsString << std::endl
    ;

    return output.str();

};

struct Location {
    long long id;
    std::string name;
    std::string  shortName;
    Roads roads = {};
};

typedef std::vector<Location> Locations;

class RoadNetwork {

    private:

        Locations locations;

    public:

        RoadNetwork(Locations locationsP = {}): locations(locationsP) {};

        const Locations& getLocations() const {
            return locations;
        };

        void addLocation(const Location location) {
            locations.push_back(location);
        };

        void addRoads(
            const unsigned int from,
            const Roads roads,
            const bool areDirected = false
        ) {
            for (auto road : roads) {
                addRoad(from, road, areDirected);
            };
        };

        void addRoad(
            const unsigned int from,
            const Road road,
            const bool isDirected = false
        ) {

            const unsigned int to = road.to;

            Location& fromLocation = locations.at(from);
            Roads& fromRoads = fromLocation.roads;
            bool fromRoadAlreadyExists = false;
            for (auto& currentFromRoad : fromRoads) {
                if (currentFromRoad.to == to) {
                    currentFromRoad.weight = road.weight;
                    fromRoadAlreadyExists = true;
                    break;
                };
            };

            if (!fromRoadAlreadyExists) {
                fromRoads.push_back(road);
            };

            if (isDirected) {
                return;
            };

            Location& toLocation = locations.at(to);
            Roads& toRoads = toLocation.roads;
            bool toRoadAlreadyExists = false;
            for (auto& currentToRoad : toRoads) {
                if (currentToRoad.to == from) {
                    currentToRoad.weight = road.weight;
                    return;
                };
            };

            if (!toRoadAlreadyExists) {
                toRoads.push_back(road);
            };

        };

        bool isValid() const {
            for (const auto& currentLocation : locations) {
                const Roads& currentRoads = currentLocation.roads;
                for (const auto& currentRoad : currentRoads) {
                    if (currentRoad.to >= locations.size()) {
                        return false;
                    };
                };
            };
            return true;
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

int main() {
    /*
    enum LocationIndex {
        FISH_HOEK,
        NOORDHOEK,
        SIMONS_TOWN,
        GLENCAIRN,
        KALK_BAY,
        MUIZENBERG,
        KOMMETJIE,
        SCARBOROUGH,
        HOUT_BAY,
        SMIT_WINKEL_BAY,
        LocationIndexCount
    };
    */

    RoadNetwork roadNetwork;

    for (unsigned int i = 0; i < LocationIndexCount; ++i) {
        const LocationIndex currentLocationIndex = static_cast<LocationIndex>(i);
        roadNetwork.addLocation({
            .id = currentLocationIndex,
            .name = locationIndexToName(currentLocationIndex),
            .shortName = locationIndexToShortName(currentLocationIndex)
        });
    };

    for (unsigned int i = 0; i < LocationIndexCount; ++i) {
        const LocationIndex currentLocationIndex = static_cast<LocationIndex>(i);
        const Roads currentRoads = locationIndexToRoads(currentLocationIndex);
        roadNetwork.addRoads(currentLocationIndex, currentRoads);
    };

    IdGenerator reponseTeamIdGenerator;
    ResponseTeams responseTeams = {};
    responseTeams.reserve(NUMBER_OF_REPONSE_TEAMS);

    for (unsigned int i = 0; i < NUMBER_OF_REPONSE_TEAMS; ++i) {
        ResponseTeam randomResponseTeam({
            .id = reponseTeamIdGenerator.getNextId(),
            .locationIndex = ran<LocationIndex>(0, LocationIndexCount - 1),
            .capability = ran<ServiceType>(0, ServiceTypeCount - 1),
            .status = ran<ResponseTeam::Status>(0, ResponseTeam::StatusCount - 1)
        });
        responseTeams.push_back(randomResponseTeam);
    };

    IdGenerator incidentIdGenerator;
    Incidents incidents = {};
    incidents.reserve(NUMBER_OF_INCIDENTS);

    for (unsigned int i = 0; i < NUMBER_OF_INCIDENTS; ++i) {
        Incident randomIndicent({
            .id = incidentIdGenerator.getNextId(),
            .locationIndex = ran<LocationIndex>(0, LocationIndexCount - 1),
            .category = ran<ServiceType>(0, ServiceTypeCount - 1),
            .severity = ran<Incident::Severity>(0, Incident::SeverityCount - 1),
            .status = ran<Incident::Status>(0, Incident::StatusCount - 1)
        });
        incidents.push_back(randomIndicent);
    };

    bool exit = false;

    const std::string HEADER =
        "\n"
        "Programmer: Rowan Van Zyl\n"
        "Student number: 25939831\n";

    const std::string MAIN_MENU =
        HEADER +
        "========================================\n"
        " MUNICIPAL INCIDENT RESPONSE SYSTEM\n"
        "========================================\n"
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

    std::string input = "";

    char inputChar = ' ';

    do {

        clearScreen();

        std::cout << MAIN_MENU;

        input = getUserInput();

        inputChar = input.at(0);

        switch (inputChar) {

            case '1': {
                const std::string DISPLAY_ROAD_NETWORK_MENU =
                    HEADER +
                    "========================================\n"
                    " DISPLAY LOCATIONS AND ROADS MENU\n"
                    "========================================\n"
                    "\n" +
                    roadNetwork.toString()
                ;
                clearScreen();
                std::cout << DISPLAY_ROAD_NETWORK_MENU;
                waitForEnter();
                break;
            };
            case '2': {

                const std::string DISPLAY_INCIDENTS_MENU =
                    HEADER +
                    "========================================\n" +
                    " DISPLAY INCIDENTS MENU\n" +
                    "========================================\n" +
                    "\n" +
                    incidentsToString(incidents) + "\n"
                ;
                clearScreen();
                std::cout << DISPLAY_INCIDENTS_MENU;
                waitForEnter();
                break;
            };

            case '3': {

                Incidents prioritisedOpenIncidents = incidents;

                prioritisedOpenIncidents.erase(
                    std::remove_if(
                        prioritisedOpenIncidents.begin(),
                        prioritisedOpenIncidents.end(),
                        [](const Incident& incident) -> bool {
                            return incident.getState().status != Incident::Status::Open;
                        }
                    ),
                    prioritisedOpenIncidents.end()
                );

                std::sort(
                    prioritisedOpenIncidents.rbegin(),
                    prioritisedOpenIncidents.rend()
                );

                const std::string PRIORITISE_OPEN_INCIDENTS_MENU =
                    HEADER +
                    "========================================\n"
                    " PRIORITISE OPEN INCIDENTS MENU\n"
                    "========================================\n"
                    "\n" +
                    incidentsToString(prioritisedOpenIncidents) + "\n"
                ;
                clearScreen();
                std::cout << PRIORITISE_OPEN_INCIDENTS_MENU;
                waitForEnter();
                break;
            };

            case '4': {

                const std::string DISPLAY_RESPONSE_TEAMS_MENU =
                    HEADER +
                    "========================================\n"
                    " DISPLAY RESPONSE TEAMS MENU"
                    "========================================\n"
                    "\n" +
                    responseTeamsToString(responseTeams)
                ;

                clearScreen();
                std::cout << DISPLAY_RESPONSE_TEAMS_MENU;
                waitForEnter();
                break;
            };

            case '5': {

                const std::string INVESTIGATE_INCIDENT_MENU =
                    HEADER +
                    "========================================\n"
                    " INVESTIGATE INCIDENT MENU\n"
                    "========================================\n"
                    "\n"
                    "Enter incident id: "
                ;
                clearScreen();
                std::cout << INVESTIGATE_INCIDENT_MENU;

                const std::string inputIncidentIdString = getUserInput();

                long long inputIdLongLong = -1;

                try {
                    inputIdLongLong = std::stoll(inputIncidentIdString);
                } catch (const std::exception&) {
                    std::cout
                        << std::endl
                        << "Error: Incident id must be from 1 to " << NUMBER_OF_INCIDENTS << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const auto cit = std::find_if(
                    incidents.cbegin(),
                    incidents.cend(),
                    [ inputIdLongLong ](const auto& incident) -> bool {
                        return incident.getState().id == inputIdLongLong;
                    }
                );

                if (cit == incidents.cend()) {
                    std::cout
                        << std::endl
                        << "Incident with id '" << inputIncidentIdString << "' does not exist" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const Incident& incident = *cit;

                ResponseTeams suitableAvailableResponseTeams = {};

                std::copy_if(
                    responseTeams.cbegin(),
                    responseTeams.cend(),
                    std::back_inserter(suitableAvailableResponseTeams),
                    [ incident ](const auto& responseTeam) -> bool {
                        return (
                            incident.getState().category == responseTeam.getState().capability
                        ) && (
                            responseTeam.getState().status == ResponseTeam::Status::Available
                        );
                    }
                );

                std::cout
                    << std::endl
                    << incidentsToString({ incident })
                    << "Suitable available teams:" << std::endl
                    << responseTeamsToString(suitableAvailableResponseTeams)
                ;
                waitForEnter();

                break;

            };

            case '6': {

                std::string locationList = "";

                for (unsigned int i = 0; i < LocationIndexCount; ++i) {
                    LocationIndex locationIndex = static_cast<LocationIndex>(i);
                    locationList += std::to_string(locationIndex) + " - " + locationIndexToName(locationIndex) + "\n";
                };

                const std::string FIND_ROUTE_MENU =
                    "========================================\n"
                    " FIND ROUTE MENU\n"
                    "========================================\n"
                    "\n" +
                    locationList + "\n"
                    "Enter from index: "
                ;
                clearScreen();
                std::cout << FIND_ROUTE_MENU;

                const std::string fromIndexString = getUserInput();

                const bool isFromIndexStringValid = roadNetwork.isLocationIndexStringValid(fromIndexString);

                if (!isFromIndexStringValid) {
                    std::cout << "Error: from index is not one of the above displayed options" << std::endl;
                    waitForEnter();
                    break;
                };

                std::cout << "Enter to index: ";

                const std::string toIndexString = getUserInput();

                const bool isToIndexStringValid = roadNetwork.isLocationIndexStringValid(toIndexString);

                if (!isToIndexStringValid) {
                    std::cout << "Error: to index is not one of the above displayed options" << std::endl;
                    waitForEnter();
                    break;
                };

                const unsigned int fromIndex = std::stoul(fromIndexString);
                const unsigned int toIndex = std::stoul(toIndexString);

                const std::string shortestPathString = roadNetwork.shortestPathToString(fromIndex, toIndex);

                std::cout
                    << shortestPathString << std::endl
                    << std::endl
                ;

                waitForEnter();

                break;
            };

            case '7': {
                break;
            };

            case '8': {
                break;
            };

            case '0': {
                std::cout
                    << std::endl
                    << "Gracefully Exiting application..."
                    << std::endl
                ;
                exit = true;
                break;
            };

            default: {
                std::cout
                    << std::endl
                    << "Error: input is not a valid option (1, 2, 3, 4, 5, 6, 7, 8 or 0)" << std::endl
                ;
                waitForEnter();
                break;
            };

        };

    } while (!exit);

};
