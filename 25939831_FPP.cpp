
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

// Helpers

class RandomNumberGenerator {

    private:

        std::mt19937 twisterEngine;

    public:

        RandomNumberGenerator(
            const unsigned int seed = 0
        ) {
            seedTwister(seed);
        };

        void seedTwister(
            const unsigned int seed
        ) {
            twisterEngine.seed(seed); 
        };

        // Generates a random unsigned integer between min and max (inclusive)
        template <class T = unsigned int>
        T ran(const unsigned int min, const unsigned int max) {

            std::uniform_int_distribution<unsigned int> dist(min, max);

            return static_cast<T>(dist(twisterEngine));

        };

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

long long stringToLongLong(
    const std::string& str,
    bool* ok = nullptr
) {

    if (ok != nullptr) {
        *ok = true;
    };

    try {
        return std::stoll(str);
    } catch (const std::exception&) {
        if (ok != nullptr) {
            *ok = false;
        };
        return -1;
    };

};

unsigned long stringToUnsignedLong(
    const std::string& str,
    bool* ok = nullptr
) {

    if (ok != nullptr) {
        *ok = true;
    };

    try {
        return std::stoul(str);
    } catch (const std::exception&) {
        if (ok != nullptr) {
            *ok = false;
        };
        return 0;
    };

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

        void setId(const long long newId) {
            state.id = newId;
        };

        void setLocationIndex(const LocationIndex newLocationIndex) {
            state.locationIndex = newLocationIndex;
        };

        void setCapability(const ServiceType newCapability) {
            state.capability = newCapability;
        };

        void setStatus(const Status newStatus) {
            state.status = newStatus;
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

std::string responseTeamsToString(
    const ResponseTeams& responseTeams
) {
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

class ResponseTeamManager {

    private:

        ResponseTeams responseTeams;
        IdGenerator idGenerator;

        ResponseTeam* getResponseTeamPointerById(
            const long long teamId
        ) {
            auto it = std::find_if(
                responseTeams.begin(),
                responseTeams.end(),
                [ teamId ](const auto& responseTeam) -> bool {
                    return responseTeam.getState().id == teamId;
                }
            );

            if (it == responseTeams.end()) {
                return nullptr;
            };

            return &(*it);

        };

        ResponseTeam* getResponseTeamById(
            const long long teamId
        ) {
            return getResponseTeamPointerById(teamId);
        };

    public:

        ResponseTeamManager(
            ResponseTeams responseTeamsP = {}
        ):
            responseTeams(responseTeamsP),
            idGenerator() {
        };

        const ResponseTeams& getResponseTeams() {
            return responseTeams;
        };

        const ResponseTeam* getResponseTeamById(
            const long long teamId
        ) const {
            return const_cast<ResponseTeamManager*>(this)->getResponseTeamPointerById(teamId);
        };

        void addResponseTeam(
            ResponseTeam responseTeam
        ) {
            responseTeam.setId(idGenerator.getNextId());
            responseTeams.push_back(responseTeam);
        };

        void setResponseTeamStatusById(
            const long long teamId,
            const ResponseTeam::Status status
        ) {
            ResponseTeam* responseTeamPointer = getResponseTeamById(teamId);
            if (responseTeamPointer == nullptr) {
                return;
            };
            ResponseTeam& resposeTeam = *responseTeamPointer;
            resposeTeam.setStatus(status);
        };

        bool doesCapableAndAvailableResponseTeamExist(
            const ServiceType serviceType
        ) {
            for (const auto& responseTeam : responseTeams) {
                if ((
                    responseTeam.getState().status == ResponseTeam::Status::Available
                ) && (
                    responseTeam.getState().capability == serviceType
                )) {
                    return true;
                };
            };
            return false;
        };

        bool doesResponseTeamExist(
            const long long teamId
        ) {
            const ResponseTeam* responseTeamPointer = getResponseTeamById(teamId);
            return responseTeamPointer != nullptr;
        };

        bool isResponseTeamAvailable(
            const long long teamId
        ) {

            const ResponseTeam* responseTeamPointer = getResponseTeamById(teamId);

            if (responseTeamPointer == nullptr) {
                return false;
            };

            const ResponseTeam responseTeam = *responseTeamPointer;

            if (responseTeam.getState().status != ResponseTeam::Status::Available) {
                return false;
            };

            return true;

        };

        bool isResponseTeamCapable(
            const long long teamId,
            const ServiceType serviceType
        ) {

            const ResponseTeam* responseTeamPointer = getResponseTeamById(teamId);

            if (responseTeamPointer == nullptr) {
                return false;
            };

            const ResponseTeam responseTeam = *responseTeamPointer;

            if (responseTeam.getState().capability != serviceType) {
                return false;
            };

            return true;

        };

        unsigned int getResponseTeamsCount() const {
            return responseTeams.size();
        };

        unsigned int getResponseTeamsCountByCapability(
            const ServiceType serviceType
        ) const {
            return std::count_if(
                responseTeams.cbegin(),
                responseTeams.cend(),
                [ serviceType ](const auto& responseTeam) -> bool {
                    return responseTeam.getState().capability == serviceType;
                }
            );
        };

        unsigned int getResponseTeamsCountByStatus(
            const ResponseTeam::Status status
        ) const {
            return std::count_if(
                responseTeams.cbegin(),
                responseTeams.cend(),
                [ status ](const auto& responseTeam) -> bool {
                    return responseTeam.getState().status == status;
                }
            );
        };

        std::string toString() {
            return responseTeamsToString(responseTeams);
        };

        std::string suitableAndAvailableTeamsToString(
            const ServiceType serviceType
        ) {

            ResponseTeams suitableAndAvailableResponseTeams = {};

                std::copy_if(
                    responseTeams.cbegin(),
                    responseTeams.cend(),
                    std::back_inserter(suitableAndAvailableResponseTeams),
                    [ serviceType ](const auto& responseTeam) -> bool {
                        return (
                            serviceType == responseTeam.getState().capability
                        ) && (
                            responseTeam.getState().status == ResponseTeam::Status::Available
                        );
                    }
                );

            return responseTeamsToString(suitableAndAvailableResponseTeams);

        };

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

        static std::string severityToString(const Severity severity) {
            return severityStringMap[static_cast<unsigned int>(severity)];
        };

        enum Status {
            Open,
            Assigned,
            Resolved,
            StatusCount
        };

        static constexpr const char* statusStringMap[StatusCount] = {
            "Open",
            "Assigned",
            "Resolved"
        };

        static std::string statusToString(const Status status) {
            return statusStringMap[static_cast<unsigned int>(status)];
        };

        static const long long UNASSIGNED_ID = -1;

        static const long long UNASSIGNED_TEAM_ID = -1;

        static std::string assignedTeamIdToString(
            const long long assignedTeamId
        ) {
            if (assignedTeamId == UNASSIGNED_TEAM_ID) {
                return "None";
            };
            return std::to_string(assignedTeamId);
        };

        struct State {
            long long id = UNASSIGNED_ID;
            LocationIndex locationIndex;
            ServiceType category;
            Severity severity;
            Status status = Status::Open;
            long long assignedTeamId = UNASSIGNED_TEAM_ID;
        };

    private:

        State state;

    public:

        Incident(State stateP): state(stateP) {};

        const State& getState() const {
            return state;
        };

        void setId(const long long newId) {
            state.id = newId;
        };

        void setLocationIndex(const LocationIndex newLocationIndex) {
            state.locationIndex = newLocationIndex;
        };

        void setCategory(const ServiceType newCategory) {
            state.category = newCategory;
        };

        void setSeverity(const Severity newSeverity) {
            state.severity = newSeverity;
        };

        void setStatus(const Status newStatus) {
            state.status = newStatus;
        };

        void setAssignedTeamId(const long long newAssignedTeamId) {
            state.assignedTeamId =  newAssignedTeamId;
        };

        void unassignTeam() {
            state.assignedTeamId = UNASSIGNED_TEAM_ID;
        };

        bool isAssignedId() const {
            return state.id != UNASSIGNED_ID;
        };

        bool isAssignedTeam() const {
            return state.assignedTeamId != UNASSIGNED_TEAM_ID;
        };

        bool isStatus(const Status status) const {
            return state.status == status;
        };

        std::string toString(
            const unsigned int idWidth = 5,
            const unsigned int locationWidth = 20,
            const unsigned int categoryWidth = 15,
            const unsigned int severityWidth = 10,
            const unsigned int statusWidth = 10,
            const unsigned int teamIdWidth = 15
        ) const {

            std::ostringstream output;

            output
                << std::left
                << std::setw(idWidth)  << state.id
                << std::setw(locationWidth) << locationIndexToName(state.locationIndex)
                << std::setw(categoryWidth) << serviceTypeToString(state.category)
                << std::setw(severityWidth) << severityToString(state.severity)
                << std::setw(statusWidth) << statusToString(state.status)
                << std::setw(teamIdWidth) << assignedTeamIdToString(state.assignedTeamId)
            ;

            return output.str();

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

std::string incidentsToString(
    const Incidents& incidents
) {

    const unsigned int
        ID_WIDTH = 5,
        LOCATION_WIDTH = 20,
        CATEGORY_WIDTH = 20,
        SEVERITY_WIDTH = 10,
        STATUS_WIDTH = 10,
        TEAM_ID_WIDTH = 15
    ;

    std::string incidentsString = "";

    for (const auto& incident : incidents) {
        incidentsString += incident.toString(
            ID_WIDTH,
            LOCATION_WIDTH,
            CATEGORY_WIDTH,
            SEVERITY_WIDTH,
            STATUS_WIDTH,
            TEAM_ID_WIDTH
        ) + '\n';
    };

    std::ostringstream output;

    output
        << std::left
        << std::setw(ID_WIDTH) << "id"
        << std::setw(LOCATION_WIDTH) << "location"
        << std::setw(CATEGORY_WIDTH) << "category"
        << std::setw(SEVERITY_WIDTH) << "severity"
        << std::setw(STATUS_WIDTH) << "status"
        << std::setw(TEAM_ID_WIDTH) << "Assigned team" << std::endl
        << incidentsString << std::endl
    ;

    return output.str();

};

class IncidentsManager {

    private:

        Incidents incidents;
        IdGenerator idGenerator;

        Incident* getIncidentPointerById(
            const long long incidentId
        ) {
            auto it = std::find_if(
                incidents.begin(),
                incidents.end(),
                [ incidentId ](const auto& incident) -> bool {
                    return incident.getState().id == incidentId;
                }
            );

            if (it == incidents.end()) {
                return nullptr;
            };

            return &(*it);

        };

        Incident* getMutableIncidentById(
            const long long incidentId
        ) {
            return getIncidentPointerById(incidentId);
        };

    public:

        IncidentsManager(
            Incidents incidentsP = {}
        ):
            incidents(incidentsP),
            idGenerator() {
        };

        const Incidents& getIncidents() const {
            return incidents;
        };

        const Incident* getIncidentById(
            const long long incidentId
        ) const {
            return const_cast<IncidentsManager*>(this)->getIncidentPointerById(incidentId);
        };

        void addIncident(
            Incident incident
        ) {
            incident.setId(idGenerator.getNextId());
            incidents.push_back(incident);
        };

        unsigned int getIncidentsCount() const {
            return incidents.size();
        };

        unsigned int getIncidentsCountByStatus(
            const Incident::Status incidentStatus
        ) const {
            return std::count_if(
                incidents.cbegin(),
                incidents.cend(),
                [ incidentStatus ](const auto& incident) -> bool {
                    return incident.getState().status == incidentStatus;
                }
            );
        };

        unsigned int getIncidentsCountByCategory(
            const ServiceType serviceType
        ) const {
            return std::count_if(
                incidents.cbegin(),
                incidents.cend(),
                [ serviceType ](const auto& incident) -> bool {
                    return incident.getState().category == serviceType;
                }
            );
        };

        std::string toString() const {
            return incidentsToString(incidents);
        };

        std::string prioritisedIncidentsToString() const {

            Incidents prioritisedOpenIncidents = {};

            std::copy_if(
                incidents.cbegin(),
                incidents.cend(),
                std::back_inserter(prioritisedOpenIncidents),
                [](const Incident& incident) -> bool {
                    return incident.getState().status == Incident::Status::Open;
                }
            ),

            std::sort(
                prioritisedOpenIncidents.rbegin(),
                prioritisedOpenIncidents.rend()
            );

            return incidentsToString(prioritisedOpenIncidents);

        };

        void resolveIncident(
            const long long incidentId
        ) {
            Incident* incidentPointer = getMutableIncidentById(incidentId);
            if (incidentPointer == nullptr) {
                return;
            };
            Incident& incident = *incidentPointer;
            incident.setStatus(Incident::Status::Resolved);
            incident.unassignTeam();
        };

        void assignTeamToIncident(
            const long long incidentId,
            const long long assignedTeamId
        ) {
            Incident* incidentPointer = getMutableIncidentById(incidentId);
            if (incidentPointer == nullptr) {
                return;
            };
            Incident& incident = *incidentPointer;
            incident.setAssignedTeamId(assignedTeamId);
            incident.setStatus(Incident::Status::Assigned);
        };

        void unassignTeamFromIncident(
            const long long incidentId
        ) {
            Incident* incidentPointer = getMutableIncidentById(incidentId);
            if (incidentPointer == nullptr) {
                return;
            };
            Incident& incident = *incidentPointer;
            incident.unassignTeam();
            incident.setStatus(Incident::Status::Open);
        };

        void openIncident(
            const long long incidentId
        ) {
            Incident* incidentPointer = getMutableIncidentById(incidentId);
            if (incidentPointer == nullptr) {
                return;
            };
            Incident& incident = *incidentPointer;
            incident.setStatus(Incident::Status::Open);
            incident.unassignTeam();
        };

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

int main(int argc, const char* argv[]) {

    if (argc >= 5) {
        std::cout << "Error: too many arguements provided, only 3 additioanl arguements can be provided excluding the file name";
        return 1;
    };

    unsigned int args[argc - 1] = {
        1,
        6,
        10
    };

    for (unsigned int i = 1; i < argc; ++i) {
        const std::string currentArgString = argv[i];
        bool ok = false;
        const unsigned int currentArgUnsignedInt = stringToUnsignedLong(currentArgString, &ok);
        if (!ok) {
            std::cout << "Error: arg " << i << " is not a valid unsigned integer";
            return 1;
        };
        args[i - 1] = currentArgUnsignedInt;
    };

    const unsigned int SEED = args[0];
    const unsigned int NUMBER_OF_REPONSE_TEAMS = args[1];
    const unsigned int NUMBER_OF_INCIDENTS = args[2];

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

    RandomNumberGenerator randomNumberGenerator(SEED);

    ResponseTeamManager responseTeamManager;

    for (unsigned int i = 0; i < NUMBER_OF_REPONSE_TEAMS; ++i) {
        ResponseTeam randomResponseTeam({
            .locationIndex = randomNumberGenerator.ran<LocationIndex>(0, LocationIndexCount - 1),
            .capability = randomNumberGenerator.ran<ServiceType>(0, ServiceTypeCount - 1),
            .status = randomNumberGenerator.ran<ResponseTeam::Status>(0, ResponseTeam::Status::StatusCount - 1)
        });
        responseTeamManager.addResponseTeam(randomResponseTeam);
    };

    IncidentsManager incidentsManager;

    for (unsigned int i = 0; i < NUMBER_OF_INCIDENTS; ++i) {
        Incident randomIncident({
            .locationIndex = randomNumberGenerator.ran<LocationIndex>(0, LocationIndexCount - 1),
            .category = randomNumberGenerator.ran<ServiceType>(0, ServiceTypeCount - 1),
            .severity = randomNumberGenerator.ran<Incident::Severity>(0, Incident::SeverityCount - 1),
            .status = Incident::Status::Open
        });
        incidentsManager.addIncident(randomIncident);
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
                    incidentsManager.toString()
                ;
                clearScreen();
                std::cout << DISPLAY_INCIDENTS_MENU;
                waitForEnter();
                break;
            };

            case '3': {

                const std::string PRIORITISE_OPEN_INCIDENTS_MENU =
                    HEADER +
                    "========================================\n"
                    " PRIORITISE OPEN INCIDENTS MENU\n"
                    "========================================\n"
                    "\n" +
                    incidentsManager.prioritisedIncidentsToString() + "\n"
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
                    " DISPLAY RESPONSE TEAMS MENU\n"
                    "========================================\n"
                    "\n" +
                    responseTeamManager.toString()
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

                const std::string INPUT_INCIDENT_ID_STRING = getUserInput();

                bool ok = false;

                long long inputIdLongLong = stringToLongLong(INPUT_INCIDENT_ID_STRING, &ok);

                if (!ok) {
                    std::cout
                        << std::endl
                        << "Error: Incident id must be from 1 to " << NUMBER_OF_INCIDENTS << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const Incident* incidentPointer = incidentsManager.getIncidentById(inputIdLongLong);

                if (incidentPointer == nullptr) {
                    std::cout
                        << std::endl
                        << "Incident with id '" << INPUT_INCIDENT_ID_STRING << "' does not exist" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const Incident& incident = *incidentPointer;

                const ServiceType serviceType = incident.getState().category;

                std::cout
                    << std::endl
                    << "Incident info:" << std::endl
                    << incidentsToString({ incident })
                    << "Suitable available teams:" << std::endl
                    << responseTeamManager.suitableAndAvailableTeamsToString(serviceType)
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
                    std::cout
                        << std::endl
                        << "Error: from index is not one of the above displayed options" << std::endl
                        << std::endl
                    ;
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

                const std::string UPDATE_INCIDENT_MENU =
                    HEADER +
                    "========================================\n"
                    " UPDATE INCIDENT MENU\n"
                    "========================================\n"
                    "\n"
                    "Incidents:\n" +
                    incidentsManager.toString() +
                    "Enter incident id: "
                ;

                clearScreen();
                std::cout << UPDATE_INCIDENT_MENU;

                const std::string INPUT_INCIDENT_ID_STRING = getUserInput();

                bool ok = false;

                const long long INPUT_INCIDENT_ID_LONG_LONG = stringToLongLong(INPUT_INCIDENT_ID_STRING, &ok);

                if (!ok) {
                    std::cout
                        << std::endl
                        << "Error: Incident id must be from 1 to " << NUMBER_OF_INCIDENTS << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const Incident* incidentPointer = incidentsManager.getIncidentById(INPUT_INCIDENT_ID_LONG_LONG);

                if (incidentPointer == nullptr) {
                    std::cout
                        << std::endl
                        << "Incident with id '" << INPUT_INCIDENT_ID_STRING << "' does not exist" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                const Incident& incident = *incidentPointer;
                const long long incidentId = incident.getState().id;
                const LocationIndex incidentLocationIndex = incident.getState().locationIndex;
                const ServiceType serviceType = incident.getState().category;
                const Incident::Status incidentStatus = incident.getState().status;
                const long long assignedTeamId = incident.getState().assignedTeamId;

                std::string optionsText = "";

                switch (incidentStatus) {

                    case Incident::Status::Open: {
                        optionsText =
                            "1. Assign team\n"
                        ;
                        break;
                    };

                    case Incident::Status::Assigned: {
                        optionsText =
                            "1. Unassign team\n"
                            "2. Resolve incident\n"
                        ;
                        break;
                    };

                    case Incident::Status::Resolved: {
                        optionsText =
                            "1. Reopen incident\n"
                        ;
                        break;
                    };

                    default: {
                        std::cout
                            << std::endl
                            << "Error: Incident to be updated has invalid status:" << std::endl
                            << incidentsToString({ incident })
                        ;
                        waitForEnter();
                        exit = true;
                    };

                };

                optionsText += "0. Cancel";

                std::cout
                    << std::endl
                    << "Incident info:" << std::endl
                    << incidentsToString({ incident }) << std::endl
                    << "Options:" << std::endl
                    << optionsText << std::endl
                ;

                std::cout
                    << std::endl
                    << "Enter option: ";
                ;
                const std::string INPUT_OPTION = getUserInput();

                const char INPUT_OPTION_CHAR = INPUT_OPTION.at(0);

                switch (incidentStatus) {

                    case Incident::Status::Open: {
                        if (INPUT_OPTION_CHAR == '1') {
                            // assign team
                            try {

                                if (!responseTeamManager.doesCapableAndAvailableResponseTeamExist(serviceType)) {
                                    std::cout
                                        << std::endl
                                        << "No response teams with '" << serviceTypeToString(serviceType) << "' capability are available and exist" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                std::cout
                                    << "Available and suitable response teams:\n"
                                    << responseTeamManager.suitableAndAvailableTeamsToString(serviceType)
                                    << "Enter team id: "
                                ;
                                const std::string INPUT_TEAM_ID_STRING = getUserInput();

                                bool ok = false;

                                const long long INPUT_TEAM_ID_LONG_LONG = stringToLongLong(INPUT_TEAM_ID_STRING, &ok);

                                if (!ok) {
                                    std::cout
                                        << std::endl
                                        << "Error: Response team id must be from 1 to " << NUMBER_OF_REPONSE_TEAMS << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                if (!responseTeamManager.doesResponseTeamExist(INPUT_TEAM_ID_LONG_LONG)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' does not exist" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                if (!responseTeamManager.isResponseTeamCapable(INPUT_TEAM_ID_LONG_LONG, serviceType)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' is not capable of doing this job" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                if (!responseTeamManager.isResponseTeamAvailable(INPUT_TEAM_ID_LONG_LONG)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' is not available" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                incidentsManager.assignTeamToIncident(
                                    incidentId,
                                    INPUT_TEAM_ID_LONG_LONG
                                );

                                responseTeamManager.setResponseTeamStatusById(
                                    INPUT_TEAM_ID_LONG_LONG,
                                    ResponseTeam::Status::Assigned
                                );

                                std::cout
                                    << "Successfully assigned response team " << INPUT_TEAM_ID_STRING << " to incident " << incidentId << std::endl
                                    << std::endl
                                ;

                            } catch(const std::exception& e) {
                                std::cout
                                    << "Failed to assign response team to incident " << incidentId << std::endl
                                    << std::endl
                                ;
                            };

                        };
                        break;
                    };

                    case Incident::Status::Assigned: {
                        if (INPUT_OPTION_CHAR == '1') {
                            // unassign team
                            try {
                                incidentsManager.unassignTeamFromIncident(incidentId);
                                responseTeamManager.setResponseTeamStatusById(
                                    assignedTeamId,
                                    ResponseTeam::Status::Available
                                );
                                std::cout
                                    << "Successfully unassigned response team " << assignedTeamId << " from incident " << incidentId << std::endl
                                    << std::endl
                                ;
                            } catch(const std::exception& e) {
                                std::cout
                                    << std::endl
                                    << "Failed to unassign team to incident " << incidentId << std::endl
                                ;
                            };
                        } else if (INPUT_OPTION_CHAR == '2') {
                            // resolve
                            try {
                                incidentsManager.resolveIncident(incidentId);
                                responseTeamManager.setResponseTeamStatusById(
                                    assignedTeamId,
                                    ResponseTeam::Status::Available
                                );
                                std::cout
                                    << "Successfully resolved incident " << incidentId << std::endl
                                    << std::endl
                                ;
                            } catch(const std::exception& e) {
                                std::cout
                                    << std::endl
                                    << "Failed to resolve incident " << incidentId << std::endl
                                ;
                            };
                        };
                        break;
                    };

                    case Incident::Status::Resolved: {
                        if (INPUT_OPTION_CHAR == '1') {
                            // reopen
                            try {
                                incidentsManager.openIncident(incidentId);
                                std::cout
                                    << std::endl
                                    << "Successfully reopened incident " << incidentId << std::endl
                                ;
                            } catch(const std::exception& e) {
                                std::cout
                                    << std::endl
                                    << "Failed to reopen incident " << incidentId << std::endl
                                ;
                            };
                            incidentsManager.openIncident(incidentId);
                        };
                        break;
                    };

                    default: {
                        if (INPUT_OPTION_CHAR == '0') {
                            // cancel
                            std::cout
                                << std::endl
                                << "Cancelled incident update" << std::endl
                            ;
                        } else {
                            // Invalid input
                            std::cout
                                << std::endl
                                << "Input '" << INPUT_OPTION_CHAR << "' was not one of the above displayed options" << std::endl
                            ;
                        };
                        break;
                    };

                };

                waitForEnter();

                break;
            };

            case '8': {

                const std::string OPEN_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Open));
                const std::string ASSIGNED_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Assigned));
                const std::string RESOLVED_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Resolved));
                const std::string TOTAL_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCount());

                const std::string AVAILABLE_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Available));
                const std::string ASSIGNED_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Assigned));
                const std::string UNAVAILABLE_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Unavailable));
                const std::string RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCount());

                const std::string DISPLAY_SUMMARY_MENU =
                    HEADER +
                    "========================================\n"
                    " DISPLAY SUMMARY MENU\n"
                    "========================================\n"
                    "\n"
                    "----------------------------------------\n"
                    "Incidents info summary:\n"
                    " - Open:\t" + OPEN_INCIDENTS_COUNT_STRING + "\n" +
                    " - Assigned:\t" + ASSIGNED_INCIDENTS_COUNT_STRING + "\n" +
                    " - Resolved:\t" + RESOLVED_INCIDENTS_COUNT_STRING + "\n" +
                    "Total:\t" + TOTAL_INCIDENTS_COUNT_STRING + "\n" +
                    "\n"
                    "----------------------------------------\n"
                    "Response teams info summary:\n"
                    " - Available:\t" + AVAILABLE_RESPONSE_TEAMS_COUNT_STRING + "\n" +
                    " - Assigned:\t" + ASSIGNED_RESPONSE_TEAMS_COUNT_STRING + "\n" +
                    " - Unavailable:\t" + UNAVAILABLE_RESPONSE_TEAMS_COUNT_STRING + "\n" +
                    "Total:\t" + RESPONSE_TEAMS_COUNT_STRING + "\n" +
                    "\n"
                    "----------------------------------------\n"
                    "Team utilisation:\n"
                    " - Teams currently assigned " + ASSIGNED_RESPONSE_TEAMS_COUNT_STRING + " / " + RESPONSE_TEAMS_COUNT_STRING + "\n"
                    "\n"
                ;
                clearScreen();
                std::cout << DISPLAY_SUMMARY_MENU;
                waitForEnter();
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

/*
SOLUTION REFLECTION

I analysed the problem by first breaking it down into 3 distinct domains:
- The fixed geographical environment (locations and roads).
- the randomly generated operational data (incidents and response teams).
- The interactive management logic (menu options, updates, and queries).
- I treated the locations and roads as the static "backbone" of the system, because they do not change while the program runs.
The incidents and teams, however, needed to be updatable and centrally tracked so that status changes
(like assigning a team) would stay consistent across the whole application.

I decided to represent locations using an enumeration (LocationIndex) rather than strings or integers.
This gave me compile‑time safety and made it easy to map each location
to its name, short name,and list of outgoing roads using parallel arrays.
For the roads, I used an adjacency list stored as an array of vectors (Roads).
I chose this over an adjacency matrix because the network is relatively sparse (only 15–20 edges),
so a list saves memory and makes iterating over neighbours straightforward.
Each Road struct stores the destination index and the travel weight in minutes.

For incidents and response teams, I used classes (Incident and ResponseTeam) that each hold their state in a simple struct.
This made it easy to group related data while keeping the classes lightweight.
I then created two manager classes (IncidentsManager and ResponseTeamManager).
Each manager owns a std::vector of its respective objects and provides methodsfor common operations,
such as finding an item by ID, counting items by status/capability, and updating state.
The IdGenerator class provides a clean way to assign unique sequential IDs to both incidents and teams without mixing them.

For the route‑finding problem, I chose Dijkstra's algorithm because the road network has non‑negative travel times,
and we need the shortest path (minimum total time) between two locations.
I implemented Dijkstra using a simple linear search over the unvisited set rather than a priority queue.
This was a deliberate choice:
- With only 10 locations, the performance difference is negligible.
- The linear version is easier to read, debug, and verify.
The algorithm correctly reconstructs the full path by storing predecessors and handles the case where no path exists.

For incident prioritisation (Option 3), I decided that the most critical factor should be severity: High -> Medium -> Low.
When two incidents have the same severity, I prioritise the one with the larger ID (Acts as a tiebreaker),
I implemented this by overloading operator< for Incident and then sorting the open incidents in reverse order.

A significant challenge I encountered was maintaining consistency when assigning a team to an incident.
Specifically, when a team is assigned, the incident must move to "Assigned" and store the team's ID,
while the team itself must become "Assigned" so it is no longer listed as available.
Conversely, when an incident is resolved or unassigned, both states must revert correctly.
I solved this by centralising all state changes inside the manager methods (e.g., IncidentsManager::assignTeamToIncident)
and making sure that any update to an incident that affects a team is paired with an explicit call to the ResponseTeamManager.
This way, the program never gets into a state where one object thinks a team is available while the other thinks it is assigned.

Another challenge was validating user input,
especially when users enter non‑numeric values for IDs.
I created a helper function (stringToLongLong) that safely attempts conversion and returns a boolean flag to indicate success.
If the input is invalid, the program displays a clear error message and returns to the menu without crashing.
I also added checks to ensure that selected incident IDs and team IDs actually exist before performing any operations on them.

I tested my program by ...
*/
