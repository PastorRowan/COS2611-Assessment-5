
/*
Name: Rowan
Surname: Van Zyl
Student Number: 25939831
Module: COS2611
Assignment: Assessment 5 (FPP)

Please see my github repository for the best marking experience: https://github.com/PastorRowan/COS2611-Assessment-5.git
*/

#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <sstream>

// Helper classes and functions

/**
 * Random number generator using Mersenne Twister engine.
 * 
 * Provides a seeded pseudorandom number generator with a convenient template method
 * to produce uniformly distributed integers in a closed interval [min, max].
 */
class RandomNumberGenerator {

    private:

        // Mersenne Twister pseudorandom engine
        std::mt19937 twisterEngine;

    public:

        /**
         * Construct with an optional seed.
         * seed - Initial seed for the engine (default 0).
         */
        RandomNumberGenerator(
            const unsigned int seed = 0
        ) {
            seedTwister(seed);
        };

        /**
         * Reseed the random engine.
         * seed - New seed value.
         */
        void seedTwister(
            const unsigned int seed
        ) {
            twisterEngine.seed(seed); 
        };

        /**
         * Generates a random integer in [min, max] inclusive.
         * 
         * T - The type to cast the result to (default unsigned int).
         * min - Lower bound (inclusive).
         * max - Upper bound (inclusive).
         * Returns a random value of type T.
         */
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

/**
 * Wait for the user to press Enter before continuing.
 */
void waitForEnter() {

    std::cout << "Press enter to continue..." << std::endl;

    std::string dummyStr;
    std::getline(std::cin, dummyStr);

};

/**
 * Convert a string to a long long, with error indication.
 * str - The string to convert.
 * ok - If not null, set to true on success, false on failure.
 * Returns converted long long, or -1 if conversion fails.
 */
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

/**
 * Convert a string to an unsigned long, with error indication.
 * str - The string to convert.
 * ok - If not null, set to true on success, false on failure.
 * Returns converted unsigned long, or 0 if conversion fails.
 */
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

/**
 * Simple sequential ID generator.
 * 
 * Provides unique IDs for incidents and response teams.
 */
class IdGenerator {

    private:

        // Last ID issued.
        long long currentId = 0;

        // Sets currentId to 
        void setCurrentId(long long newCurrentId) {
            currentId = newCurrentId;
        };

    public:

        IdGenerator() {};

        // Returns the last issued id
        long long getCurrentId() const {
            return currentId;
        };

        long long getNextId() {
            currentId++;
            return currentId;
        };

};

// Enumerations and Mapping for Service Types, Locations, Roads

// Service/incident categories and response team capabilities.
enum ServiceType {
    Water,
    Electricity,
    RoadWork,
    Sanitation,
    PublicFacilities,
    // Number of service types
    ServiceTypeCount
};

// String representation for each ServiceType.
constexpr const char* serviceTypeStringMap[ServiceTypeCount] = {
    "Water",
    "Electricity",
    "Road Works",
    "Sanitation",
    "Public Facilities"
};

// Convert ServiceType to its display string
std::string serviceTypeToString(ServiceType serviceType) {
    return serviceTypeStringMap[static_cast<unsigned int>(serviceType)];
};

/**
 * LOCATION INDICES & ROAD NETWORK
 *
 * These locations represent real towns and suburbs in the Cape Peninsula,
 * South Africa. The road network is used for:
 *   - Displaying the municipal service area.
 *   - Finding the shortest driving route between any two locations.
 *   - Assigning response teams to incidents based on proximity.
 *
 * Sources:
 *   - Google Maps
 *   - Local knowledge of the Cape Peninsula road network
 *
 * ORIGINAL DISTANCES (RESEARCH DATA)
 *
 * The following distances (in kilometres) were obtained from real-world
 * road maps. These are the raw measurements used as the basis for the
 * travel times in the system.
 *
 * Note: The road network is undirected; each connection is shown once,
 * but the system stores both directions implicitly.
 *
 * +---------------------+---------------------+--------------+
 * | From                | To                  | Distance (km)|
 * +---------------------+---------------------+--------------+
 * | Fish Hoek           | Noordhoek           | 9            |
 * | Fish Hoek           | Glencairn           | 5            |
 * | Fish Hoek           | Kalk Bay            | 4            |
 * | Fish Hoek           | Kommetjie           | 10           |
 * | Noordhoek           | Kommetjie           | 10           |
 * | Noordhoek           | Glencairn           | 10           |
 * | Noordhoek           | Muizenberg          | 19           |
 * | Noordhoek           | Hout Bay            | 13           |
 * | Simon's Town        | Glencairn           | 4            |
 * | Simon's Town        | Scarborough         | 13           |
 * | Simon's Town        | Smit Winkel Bay     | 10           |
 * | Glencairn           | Kommetjie           | 13           |
 * | Glencairn           | Scarborough         | 13           |
 * | Glencairn           | Smit Winkel Bay     | 18           |
 * | Kalk Bay            | Muizenberg          | 5            |
 * | Muizenberg          | Hout Bay            | 23           |
 * | Kommetjie           | Scarborough         | 10           |
 * | Scarborough         | Smit Winkel Bay     | 13           |
 * | Smit Winkel Bay     | Cape Point          | 120          |
 * +---------------------+---------------------+--------------+
 *
 * CALCULATION OF TRAVEL TIMES (USED IN THE SYSTEM)
 *
 * Each distance was converted to a travel time (in minutes) assuming an
 * average driving speed of 45 km/h (typical for suburban roads).
 *
 *    Formula:
 *        time (minutes) = (distance (km) / 45 km/h) × 60
 *        = distance × 4/3   (since 60/45 = 4/3)
 *
 *    Examples:
 *      - Fish Hoek → Kalk Bay:  3 km  → 3 × 4/3 = 4 minutes
 *      - Noordhoek → Hout Bay: 13 km → 13 × 4/3 ≈ 17.33 → rounded to 18 minutes
 *      (Rounding is applied up to the nearest upper minute for integer weights.)
 *
 * The resulting times are stored as integer weights in the adjacency list
 * used by the RoadNetwork class.
 *
 * For reference, the full conversion table (from the original distances)
 * is shown below with both the raw distance and the derived travel time.
 *
 * +---------------------+---------------------+--------------+---------------+
 * | From                | To                  | Distance (km)| Time (minutes)|
 * +---------------------+---------------------+--------------+---------------+
 * | Fish Hoek           | Noordhoek           | 9            | 12            |
 * | Fish Hoek           | Glencairn           | 5            | 7             |
 * | Fish Hoek           | Kalk Bay            | 4            | 6             |
 * | Fish Hoek           | Kommetjie           | 10           | 14            |
 * | Noordhoek           | Kommetjie           | 10           | 14            |
 * | Noordhoek           | Glencairn           | 10           | 14            |
 * | Noordhoek           | Muizenberg          | 19           | 26            |
 * | Noordhoek           | Hout Bay            | 13           | 18            |
 * | Simon's Town        | Glencairn           | 4            | 6             |
 * | Simon's Town        | Scarborough         | 13           | 18            |
 * | Simon's Town        | Smit Winkel Bay     | 10           | 14            |
 * | Glencairn           | Kommetjie           | 13           | 18            |
 * | Glencairn           | Scarborough         | 13           | 18            |
 * | Glencairn           | Smit Winkel Bay     | 18           | 24            |
 * | Kalk Bay            | Muizenberg          | 5            | 7             |
 * | Muizenberg          | Hout Bay            | 23           | 31            |
 * | Kommetjie           | Scarborough         | 10           | 14            |
 * | Scarborough         | Smit Winkel Bay     | 13           | 18            |
 * | Smit Winkel Bay     | Cape Point          | 120          | 160           |
 * +---------------------+---------------------+--------------+---------------+
 *
 * LOCATION INDEX ENUMERATION
 *
 * Each location is assigned a unique index for efficient array-based lookup.
 * These indices are used throughout the program to identify locations without
 * string comparisons.
 *
 * Note: The order of this enumeration must match the locationNameMap and the
 * roadNetwork adjacency lists below.
 */
enum LocationIndex {
    FishHoek,
    Noordhoek,
    SimonsTown,
    Glencairn,
    KalkBay,
    Muizenberg,
    Kommetjie,
    Scarborough,
    HOUT_BAY,
    SMIT_WINKEL_BAY,
    // Number of locations.
    LocationIndexCount
};

// Full names for each LocationIndex.
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

// Convert LocationIndex to full name.
const std::string locationIndexToName(LocationIndex locationIndex) {
    return locationIndexNameMap[static_cast<unsigned int>(locationIndex)];
};

/**
 * A road connection between two locations.
 * 
 * Stored as a directed edge from a source location to a destination.
 */
struct Road {

    // Destination LocationIndex.
    unsigned int to;

    // Travel time in minutes (5–60).
    unsigned int weight;

};

// List of roads from a single location.
typedef std::vector<Road> Roads;

/**
 * Hard-coded road network as an adjacency list.
 * 
 * Each index corresponds to a LocationIndex and contains a vector of
 * outgoing roads. The network is undirected (both directions exist
 * implicitly by symmetric entries, though we store both directions).
 * Travel times are in minutes and based on realistic driving estimates.
 */
const Roads locationIndexRoadsMap[LocationIndexCount] = {

        // Fish Hoek
        {
            { .to = Noordhoek, .weight = 12 },
            { .to = Glencairn, .weight = 7 },
            { .to = KalkBay, .weight = 6 },
            { .to = Kommetjie, .weight = 14 }
        },

        // Noordhoek
        {
            { .to = FishHoek, .weight =  12 },
            { .to = Kommetjie, .weight = 14 },
            { .to = Glencairn, .weight = 14 },
            { .to = Muizenberg, .weight = 26 },
            { .to = HOUT_BAY, .weight = 18 }
        },

        // Simon's Town
        {
            { .to = Glencairn, .weight = 6 },
            { .to = Scarborough, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 14 }
        },

        // Glencairn
        {
            { .to = FishHoek, .weight = 7 },
            { .to = Noordhoek, .weight = 14 },
            { .to = SimonsTown, .weight = 6 },
            { .to = Kommetjie, .weight = 18 },
            { .to = Scarborough, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 24 }
        },

        // Kalk Bay
        {
            { .to = FishHoek, .weight = 6 },
            { .to = Muizenberg, .weight = 7 }
        },

        // Muizenberg
        {
            { .to = Noordhoek, .weight = 26 },
            { .to = KalkBay, .weight = 7 },
            { .to = HOUT_BAY, .weight = 31 }
        },

        // Kommetjie
        {
            { .to = FishHoek, .weight = 14 },
            { .to = Noordhoek, .weight = 14 },
            { .to = Glencairn, .weight = 18 },
            { .to = Scarborough, .weight = 14 }
        },

        // Scarborough
        {
            { .to = Kommetjie, .weight = 14 },
            { .to = Glencairn, .weight = 18 },
            { .to = SimonsTown, .weight = 18 },
            { .to = SMIT_WINKEL_BAY, .weight = 18 }
        },

        // Hout Bay
        {
            { .to = Noordhoek, .weight = 18 },
            { .to = Muizenberg, .weight = 31 }
        },

        // Smit Winkel Bay
        {
            { .to = Glencairn, .weight = 24 },
            { .to = SimonsTown, .weight = 14 },
            { .to = Scarborough, .weight = 18 }
        }

};

// Retrieve the roads for a given LocationIndex.
Roads locationIndexToRoads(LocationIndex locationIndex) {
    return locationIndexRoadsMap[static_cast<unsigned int>(locationIndex)];
};

// Response Team Class and Manager

/**
 * Represents a municipal response team.
 * 
 * Each team has an ID, location, capability (ServiceType), and status.
 */
class ResponseTeam {

    public:

        // Possible statuses of a response team.
        enum Status {
            Available,
            Assigned,
            Unavailable,
            StatusCount
        };

        /**
         * Maps each Status value to its corresponding string representation.
         *
         * The array index corresponds to the integer value of a Status enum.
         */
        static constexpr const char* statusStringMap[StatusCount] = {
            "Available",
            "Assigned",
            "Unavailable"
        };

        /**
         * Converts a Status value to its string representation.
         *
         * status - The Status value to convert.
         * Returns a string containing the name of the specified status.
         */
        static std::string statusToString(Status status) {
            return statusStringMap[static_cast<int>(status)];
        };

        /**
         * Stores the complete state of a response team.
         *
         * This structure provides a convenient way to group the properties
         * that describe a response team and allows the state to be easily
         * accessed and copied.
         */
        struct State {

            // The unique identifier of the response team.
            long long id = -1;

            // The current location of the response team.
            LocationIndex locationIndex = LocationIndex::LocationIndexCount;

            // The municipal service capability of the team.
            ServiceType capability = ServiceType::ServiceTypeCount;

            // The current operational status of the team.
            Status status = Status::StatusCount;

        };

    private:

        // Stores the current state of the response team.
        State state;

    public:

       /**
         * Constructs a ResponseTeam from an initial state.
         *
         * stateP - The initial state of the response team.
         */
        ResponseTeam(State stateP): state(stateP) {};

        /**
         * Returns the current state of the response team.
         *
         * Returns A constant reference to the team's current State.
         */
        const State& getState() const {
            return state;
        };

       /**
         * Changes the response team's ID.
         *
         * newId - The new unique identifier for the response team.
         */
        void setId(const long long newId) {
            state.id = newId;
        };

        /**
         * Changes the response team's current location.
         *
         * newLocationIndex - The new location index of the response team.
         */
        void setLocationIndex(const LocationIndex newLocationIndex) {
            state.locationIndex = newLocationIndex;
        };

        /**
         * Changes the service capability of the response team.
         *
         * newCapability - The new ServiceType capability of the team.
         */
        void setCapability(const ServiceType newCapability) {
            state.capability = newCapability;
        };

        /**
         * Changes the operational status of the response team.
         *
         * newStatus The new Status of the response team.
         */
        void setStatus(const Status newStatus) {
            state.status = newStatus;
        };

        /**
         * Converts the response team's state into a formatted string.
         *
         * The returned string contains the team's ID, location, capability,
         * and status. Each field is left-aligned and padded to the specified
         * width, making the result suitable for displaying in a table.
         *
         * idWidth - The display width allocated to the team ID.
         * locationWidth - The display width allocated to the location.
         * capabilityWidth - The display width allocated to the capability.
         * statusWidth - The display width allocated to the status.
         *
         * Returns a formatted string containing the response team's information.
         */
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

/**
 * ResponseTeams
 * A collection of municipal response teams.
 *
 * This type is an alias for a vector containing ResponseTeam objects.
 */
typedef std::vector<ResponseTeam> ResponseTeams;

/**
 * Converts a collection of response teams into a formatted table.
 *
 * Each response team is converted to a formatted string using its
 * ResponseTeam::toString() method. A header row containing the ID,
 * location, capability, and status fields is added to the beginning
 * of the resulting table.
 *
 * responseTeams - The collection of response teams to format.
 *
 * Retruns a formatted string containing the response teams in tabular form.
 */
std::string responseTeamsToString(
    const ResponseTeams& responseTeams
) {

    // Column widths used when formatting the response team table.
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

/**
 * ResponseTeamManager
 * Manages a collection of municipal response teams.
 *
 * ResponseTeamManager provides functionality for adding, searching,
 * updating, validating, counting, and displaying response teams.
 *
 * Each response team is assigned a unique ID when it is added to the
 * manager. The manager also provides methods for determining whether
 * teams exist, whether they are available, and whether they possess
 * the required service capability.
 */
class ResponseTeamManager {

    private:

        // Stores all response teams managed by this object.
        ResponseTeams responseTeams;

        // Generates unique IDs for response teams.
        IdGenerator idGenerator;

        /**
         * Finds a response team by its ID and returns a pointer to it.
         *
         * This method searches the collection of response teams for a team
         * whose ID matches the supplied ID.
         *
         * teamId - The ID of the response team to find.
         *
         * Returns a pointer to the matching ResponseTeam, or nullptr if
         * no response team with the specified ID exists.
         *
         */
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

        /**
         * Finds a response team by its ID.
         *
         * This non-const overload returns a pointer that can be used to
         * modify the matching response team.
         *
         * teamId - The ID of the response team to find.
         *
         * Returns a pointer to the matching ResponseTeam, or nullptr if
         * no matching response team exists.
         */
        ResponseTeam* getResponseTeamById(
            const long long teamId
        ) {
            return getResponseTeamPointerById(teamId);
        };

    public:

        /**
         * Constructs a ResponseTeamManager.
         *
         * responseTeamsP - An optional initial collection of response teams.
         * Defaults to an empty collection.
         */
        ResponseTeamManager(
            ResponseTeams responseTeamsP = {}
        ):
            responseTeams(responseTeamsP),
            idGenerator() {
        };

        // Returns A constant reference to the collection of response teams.
        const ResponseTeams& getResponseTeams() {
            return responseTeams;
        };

        /**
         * Finds a response team by its ID.
         *
         * teamId - The ID of the response team to find.
         *
         * Returns a constant pointer to the matching ResponseTeam, or nullptr if
         * no response team with the specified ID exists.
         */
        const ResponseTeam* getResponseTeamById(
            const long long teamId
        ) const {
            return const_cast<ResponseTeamManager*>(this)->getResponseTeamPointerById(teamId);
        };

        /**
         * Adds a response team to the manager.
         *
         * A new unique ID is generated and assigned to the response team
         * before it is added to the collection.
         *
         * responseTeam - The response team to add.
         */
        void addResponseTeam(
            ResponseTeam responseTeam
        ) {
            responseTeam.setId(idGenerator.getNextId());
            responseTeams.push_back(responseTeam);
        };

        /**
         * Sets the status of a response team using its ID.
         *
         * If no response team with the specified ID exists, no changes
         * are made.
         *
         * teamId - The ID of the response team whose status should be changed.
         * status - The new status to assign to the response team.
         */
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

        /**
         * Determines whether a suitable and available response team exists.
         *
         * A suitable team must both:
         * - Have the specified service capability.
         * - Have an Available status.
         *
         * serviceType - The required service capability.
         *
         * Returns true if at least one suitable and available response team
         * exists; otherwise false.
         */
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

        /**
         * Determines whether a response team with a specified ID exists.
         *
         * teamId - The ID of the response team to search for.
         *
         * Returns true if the response team exists; otherwise false.
         */
        bool doesResponseTeamExist(
            const long long teamId
        ) {
            const ResponseTeam* responseTeamPointer = getResponseTeamById(teamId);
            return responseTeamPointer != nullptr;
        };

       /**
         * Determines whether a response team is currently available.
         *
         * teamId - The ID of the response team to check.
         *
         * Retruns true if the response team exists and has an Available status;
         * otherwise false.
         */
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

        /**
         * Determines whether a response team has a specified capability.
         *
         * teamId - The ID of the response team to check.
         * serviceType - The service capability to check for.
         *
         * Retruns true if the response team exists and has the specified
         * capability; otherwise false.
         */
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

        // Returns The number of response teams currently managed.
        unsigned int getResponseTeamsCount() const {
            return responseTeams.size();
        };

        /**
         * Counts response teams with a specified capability.
         *
         * serviceType - The service capability to count.
         *
         * Returns the number of response teams possessing the specified capability.
         */
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

        /**
         * Counts response teams with a specified status.
         *
         * status - The response team status to count.
         *
         * Returns the number of response teams currently having the specified status.
         */
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

        /**
         * Converts all managed response teams into a formatted table.
         *
         * Returns a formatted string containing all response teams.
         */
        std::string toString() {
            return responseTeamsToString(responseTeams);
        };

        /**
         * Returns all available response teams with a specified capability.
         *
         * This method creates a collection containing only response teams
         * that are both available and capable of providing the requested
         * service.
         *
         * serviceType - The required service capability.
         *
         * Returns A formatted string containing all suitable and available response teams.
         */
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

/**
 * Incident
 * Represents a municipal incident requiring a response.
 *
 * An incident contains information about the location, service category,
 * severity, current status, and response team assigned to it.
 *
 * Incidents can be assigned to response teams and can progress through
 * different statuses as they are handled.
 */
class Incident {

    public:

        // Represents the severity level of an incident.
        enum Severity {
            Low,
            Medium,
            High,
            // Stores the number of valid severity levels.
            SeverityCount
        };

        // Maps each Severity value to its corresponding string.
        static constexpr const char* severityStringMap[SeverityCount] = {
            "Low",
            "Medium",
            "High"
        };

        /**
         * Converts a Severity value to its string representation.
         *
         * severity - The severity value to convert.
         *
         * Returns a string containing the name of the specified severity.
         */
        static std::string severityToString(const Severity severity) {
            return severityStringMap[static_cast<unsigned int>(severity)];
        };

        // Represents the current status of an incident.
        enum Status {
            Open,
            Assigned,
            Resolved,
            // Stores the number of valid incident statuses.
            StatusCount
        };

        // Maps each Status value to its corresponding string.
        static constexpr const char* statusStringMap[StatusCount] = {
            "Open",
            "Assigned",
            "Resolved"
        };

        /**
         * Converts a Status value to its string representation.
         *
         * status - The status value to convert.
         *
         * Returns a string containing the name of the specified status.
         */
        static std::string statusToString(const Status status) {
            return statusStringMap[static_cast<unsigned int>(status)];
        };

        // Represents an unassigned incident ID.
        static const long long UNASSIGNED_ID = -1;

        // Represents an unassigned response team ID.
        static const long long UNASSIGNED_TEAM_ID = -1;

        /**
         * Converts an assigned team ID to a display string.
         *
         * If the supplied ID is equal to UNASSIGNED_TEAM_ID, the string
         * "None" is returned. Otherwise, the numeric team ID is converted
         * to a string.
         *
         * assignedTeamId - The ID of the response team assigned to the incident.
         *
         * Retruns "None" if no team is assigned; otherwise, the team ID as a string.
         */
        static std::string assignedTeamIdToString(
            const long long assignedTeamId
        ) {
            if (assignedTeamId == UNASSIGNED_TEAM_ID) {
                return "None";
            };
            return std::to_string(assignedTeamId);
        };

        /**
         * State
         * Stores the complete state of an incident.
         *
         * This structure groups all properties that describe an incident,
         * including its ID, location, service category, severity, status,
         * and assigned response team.
         *
         * By default, an incident has an unassigned ID, an Open status,
         * and no assigned response team.
         */
        struct State {

            // The unique identifier of the incident.
            long long id = UNASSIGNED_ID;

            // The location where the incident occurred.
            LocationIndex locationIndex = LocationIndex::LocationIndexCount;

            // The municipal service category associated with the incident.
            ServiceType category = ServiceType::ServiceTypeCount;

            // The severity level of the incident.
            Severity severity = Severity::SeverityCount;

            // The current status of the incident, defaults to Status::Open.
            Status status = Status::Open;

            // The ID of the response team assigned to the incident, defaults to UNASSIGNED_TEAM_ID when no team is assigned.
            long long assignedTeamId = UNASSIGNED_TEAM_ID;
        };

    private:

        // Stores the current state of the incident.
        State state;

    public:

        /**
         * Constructs an Incident from an initial state.
         *
         * stateP - The initial state of the incident.
         */
        Incident(State stateP): state(stateP) {};

        /**
         * Returns the current state of the incident.
         *
         * Retruns A constant reference to the incident's current State.
         */
        const State& getState() const {
            return state;
        };

        /**
         * Changes the incident's ID.
         *
         * newId - The new ID to assign to the incident.
         */
        void setId(const long long newId) {
            state.id = newId;
        };

        /**
         * Changes the location of the incident.
         *
         * newLocationIndex - The new location index of the incident.
         */
        void setLocationIndex(const LocationIndex newLocationIndex) {
            state.locationIndex = newLocationIndex;
        };

        /**
         * Changes the service category of the incident.
         *
         * newCategory - The new service category of the incident.
         */
        void setCategory(const ServiceType newCategory) {
            state.category = newCategory;
        };

        /**
         * Changes the severity of the incident.
         *
         * newSeverity The new severity level of the incident.
         */
        void setSeverity(const Severity newSeverity) {
            state.severity = newSeverity;
        };

        /**
         * Changes the status of the incident.
         *
         * newStatus - The new status of the incident.
         */
        void setStatus(const Status newStatus) {
            state.status = newStatus;
        };

        /**
         * Assigns a response team to the incident.
         *
         * newAssignedTeamId - The ID of the response team to assign.
         */
        void setAssignedTeamId(const long long newAssignedTeamId) {
            state.assignedTeamId =  newAssignedTeamId;
        };

        /**
         * Removes the response team assigned to the incident.
         *
         * The assigned team ID is reset to UNASSIGNED_TEAM_ID.
         */
        void unassignTeam() {
            state.assignedTeamId = UNASSIGNED_TEAM_ID;
        };

        /**
         * Determines whether the incident has been assigned an ID.
         *
         * Returns true if the incident has a valid assigned ID;
         * otherwise false.
         */
        bool isAssignedId() const {
            return state.id != UNASSIGNED_ID;
        };

        /**
         * Determines whether a response team is assigned to the incident.
         *
         * true if a response team is assigned;
         * otherwise false.
         */
        bool isAssignedTeam() const {
            return state.assignedTeamId != UNASSIGNED_TEAM_ID;
        };

        /**
         * Determines whether the incident has a specified status.
         *
         * status - The status to compare against the incident's current status.
         *
         * Returns true if the incident has the specified status; otherwise false.
         */
        bool isStatus(const Status status) const {
            return state.status == status;
        };

        /**
         * Converts the incident into a formatted string.
         *
         * The returned string contains the incident's ID, location,
         * service category, severity, status, and assigned response
         * team ID. Each field is left-aligned and padded to the
         * specified width, making the result suitable for displaying
         * incidents in a table.
         *
         * idWidth The display width allocated to the incident ID.
         * locationWidth - The display width allocated to the location.
         * categoryWidth - The display width allocated to the service category.
         * severityWidth - The display width allocated to the severity.
         * statusWidth - The display width allocated to the status.
         * teamIdWidth - The display width allocated to the assigned response team ID.
         *
         * Returns A formatted string containing the incident's information.
         */
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

        /**
         * Compares two incidents for ordering.
         *
         * Incidents are ordered primarily by severity. If two incidents
         * have the same severity, they are ordered by descending incident
         * ID, meaning the incident with the larger ID is considered smaller
         * for the purposes of the comparison.
         *
         * This ordering allows incidents to be prioritised by severity
         * while using the ID as a secondary ordering criterion.
         *
         * left - The left-hand incident in the comparison.
         * right - The right-hand incident in the comparison.
         *
         * Returns true if left should appear before right according
         * to the defined ordering; otherwise false.
         */
        friend bool operator<(const Incident& left, const Incident& right) {
            if (left.state.severity != right.state.severity) {
                return left.state.severity < right.state.severity;
            } else {
                return left.state.id > right.state.id;
            };
        };

};

/**
 * Incidents
 * A collection of municipal incidents.
 *
 * This type is an alias for a vector containing Incident objects.
 */
typedef std::vector<Incident> Incidents;

/**
 * Converts a collection of incidents into a formatted table.
 *
 * Each incident is converted to a formatted string using its
 * Incident::toString() method. A header row containing the incident ID,
 * location, category, severity, status, and assigned team ID is added
 * to the beginning of the resulting table.
 *
 * incidents - The collection of incidents to format.
 *
 * Returns a formatted string containing the incidents in tabular form.
 */
std::string incidentsToString(
    const Incidents& incidents
) {

    // Column widths used when formatting the incident table.
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

/**
 * IncidentsManager
 * Manages a collection of municipal incidents.
 *
 * IncidentsManager provides functionality for adding, searching,
 * counting, displaying, prioritising, assigning, unassigning, opening,
 * and resolving incidents.
 *
 * Each incident is assigned a unique ID when it is added to the manager.
 * The manager also maintains the state of each incident, including its
 * current status and any response team assigned to it.
 */
class IncidentsManager {

    private:

        // Stores all incidents managed by this object.
        Incidents incidents;

        // Generates unique IDs for incidents.
        IdGenerator idGenerator;

        /**
         * Finds an incident by its ID and returns a modifiable pointer.
         *
         * This method searches the collection of incidents for an incident
         * whose ID matches the supplied ID.
         *
         * incidentId The ID of the incident to find.
         *
         * Returns a pointer to the matching Incident, or nullptr if
         * no incident with the specified ID exists.
         */
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

        /**
         * Finds an incident by its ID for modification.
         *
         * This method provides access to the mutable Incident object
         * associated with the specified ID.
         *
         * incidentId - The ID of the incident to find.
         *
         * Returns a pointer to the matching Incident, or nullptr if no
         * matching incident exists.
         */
        Incident* getMutableIncidentById(
            const long long incidentId
        ) {
            return getIncidentPointerById(incidentId);
        };

    public:

        /**
         * Constructs an IncidentsManager.
         *
         * incidentsP - An optional initial collection of incidents.
         * Defaults to an empty collection.
         */
        IncidentsManager(
            Incidents incidentsP = {}
        ):
            incidents(incidentsP),
            idGenerator() {
        };

        /**
         * Returns all incidents managed by this object.
         *
         * Returns a constant reference to the collection of incidents.
         */
        const Incidents& getIncidents() const {
            return incidents;
        };

        /**
         * Finds an incident by its ID.
         *
         * incidentId - The ID of the incident to find.
         *
         * Returns a pointer to the matching Incident, or nullptr if no
         * incident with the specified ID exists.
         */
        const Incident* getIncidentById(
            const long long incidentId
        ) const {
            return const_cast<IncidentsManager*>(this)->getIncidentPointerById(incidentId);
        };

        /**
         * Adds an incident to the manager.
         *
         * A new unique ID is generated and assigned to the incident
         * before it is added to the collection.
         *
         * incident - The incident to add.
         */
        void addIncident(
            Incident incident
        ) {
            incident.setId(idGenerator.getNextId());
            incidents.push_back(incident);
        };

        // Returns the number of incidents currently managed.
        unsigned int getIncidentsCount() const {
            return incidents.size();
        };

        /**
         * Counts incidents with a specified status.
         *
         * incidentStatus - The incident status to count.
         *
         * Returns the number of incidents currently having the specified status.
         */
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

        /**
         * Counts incidents belonging to a specified service category.
         *
         * serviceType - The service category to count.
         *
         * Returns the number of incidents belonging to the specified service category.
         */
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

        /**
         * Converts all managed incidents into a formatted table.
         *
         * Returns a formatted string containing all managed incidents.
         */
        std::string toString() const {
            return incidentsToString(incidents);
        };

        /**
         * Returns A formatted string containing the prioritised open incidents.
         *
         * This method creates a separate collection containing only
         * incidents whose status is Incident::Status::Open. The incidents
         * are then sorted according to Incident::operator<.
         */
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

        /**
         * Resolves an incident.
         *
         * The incident's status is changed to Incident::Status::Resolved
         * and any response team assigned to the incident is removed.
         *
         * If no incident with the specified ID exists, no changes are made.
         *
         * incidentId - The ID of the incident to resolve.
         */
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

        /**
         * Assigns a response team to an incident.
         *
         * The incident's assigned team ID is updated and its status is
         * changed to Incident::Status::Assigned.
         *
         * If no incident with the specified ID exists, no changes are made.
         *
         * incidentId - The ID of the incident to assign.
         * assignedTeamId - The ID of the response team to assign.
         */
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

        /**
         * Removes the response team assigned to an incident.
         *
         * The incident's assigned team is removed and its status is changed
         * back to Incident::Status::Open.
         *
         * If no incident with the specified ID exists, no changes are made.
         *
         * incidentId - The ID of the incident from which the team should be unassigned.
         */
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

        /**
         * Reopens an incident.
         *
         * The incident's status is changed to Incident::Status::Open and
         * any response team currently assigned to it is removed.
         *
         * If no incident with the specified ID exists, no changes are made.
         *
         * incidentId - The ID of the incident to open.
         */
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

/**
 * Location
 * Represents a location within the municipal road network.
 *
 * A location represents a node in the road network and contains identifying
 * information as well as the roads that connect it to other locations.
 */
struct Location {

    // The unique identifier of the location.
    long long id = -1;

    // The full name of the location.
    std::string name = "name not assigned";

    // The roads connecting this location to other locations.
    Roads roads = {};

};

/**
 * Locations
 * A collection of locations in the road network.
 *
 * This type is an alias for a vector containing Location objects.
 */
typedef std::vector<Location> Locations;

/**
 * RoadNetwork
 * Represents and manages a municipal road network.
 *
 * The road network consists of locations connected by roads. Each location
 * acts as a node in the network, while each road represents an edge connecting
 * one location to another.
 *
 * The class provides functionality for:
 * - Adding locations.
 * - Adding roads between locations.
 * - Validating the network.
 * - Validating location indices supplied as strings.
 * - Displaying the complete road network.
 * - Finding the shortest route between two locations.
 *
 * The shortest route functionality uses Dijkstra's shortest-path algorithm
 * to determine the minimum-distance route between two locations.
 */
class RoadNetwork {

    private:

        // Stores all locations belonging to the road network.
        Locations locations;

        /**
         * Adds a road to a location's list of outgoing roads.
         *
         * If a road to the specified destination already exists, its weight
         * is updated instead of adding a duplicate road.
         *
         * from - The index of the location from which the road originates.
         * road - The road to add.
         */
        void addRoadInternal(
            const unsigned int from,
            const Road road
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
        };

    public:

        /**
         * Constructs a RoadNetwork.
         *
         * locationsP - An optional initial collection of locations.
         * Defaults to an empty collection.
         */
        RoadNetwork(Locations locationsP = {}): locations(locationsP) {};

        /**
         * Constructs a RoadNetwork.
         *
         * locationsP An optional initial collection of locations.
         * Defaults to an empty collection.
         */
        const Locations& getLocations() const {
            return locations;
        };

        /**
         * Adds a location to the road network.
         *
         * location - The location to add.
         */
        void addLocation(const Location location) {
            locations.push_back(location);
        };

        /**
         * Adds multiple roads originating from a specified location.
         *
         * Each supplied road is added using addRoad().
         *
         * from - The index of the location from which the roads originate.
         * roads - The collection of roads to add.
         * areDirected - Determines whether the roads are directed.
         *
         * If areDirected is false, each road is intended to have a corresponding reverse connection.
         */
        void addRoads(
            const unsigned int from,
            const Roads roads,
            const bool areDirected = false
        ) {
            for (auto road : roads) {
                addRoad(from, road, areDirected);
            };
        };

        /**
         * Adds a road to the road network.
         *
         * The road is first added from the specified source location.
         * If the road is not directed, a reverse connection is also added.
         *
         * from - The index of the source location.
         * road - The road to add.
         * isDirected - Determines whether the road is directed.
         */
        void addRoad(
            const unsigned int from,
            const Road road,
            const bool isDirected = false
        ) {

            // Add the road from the source location to the destination.
            addRoadInternal(from, road);

            // If the road is directed, no reverse road is required.
            if (isDirected) {
                return;
            };

            // For an undirected road, add the reverse connection.
            const Road reverseRoad = {
                from,
                road.weight
            };

            addRoadInternal(road.to, reverseRoad);

        };

        /**
         * Determines whether the road network is valid.
         *
         * A valid network must have every road reference a destination
         * location whose index exists within the locations collection.
         *
         * Returns true if every road references a valid location;
         * otherwise false.
         */
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

        /**
         * Determines whether a string represents a valid location index.
         *
         * The supplied string must contain between one and three digits and
         * must represent an index that exists within the locations collection.
         *
         * locationIndex - The location index represented as a string.
         *
         * Returns true if the string represents a valid location index;
         * otherwise false.
         */
        bool isLocationIndexStringValid (
            const std::string& locationIndex
        ) const {

            bool ok = false;

            // Convert park index string into an index for comparison
            const unsigned long locationIndexAsUnsignedLong = stringToUnsignedLong(locationIndex, &ok);

            if (!ok) {
                return false;
            };

            // If park index is greater than the size of national parks then return false
            if (locationIndexAsUnsignedLong >= locations.size()) {
                return false;
            };

            // Else return true
            return true;
        };

        /**
         * Converts the road network into a human-readable string.
         *
         * Each location is displayed together with every road originating
         * from that location. Each road displays its destination and weight.
         *
         * Returns a formatted string containing the complete road network.
         */
        std::string toString() const {

            // Build a human readable representation of the Conservation Network
            std::string output = "Road Network:\n\n";

            // Loop over all nodes in the adjacency list
            for (const auto& currentLocation : locations) {

                // Get current node data
                const std::string& currentLocationName = currentLocation.name;
                const Roads& currentLocationRoads = currentLocation.roads;

                output += currentLocationName + ":\n";

                // Loop over the current node's edges
                for (const auto& currentRoad : currentLocationRoads) {

                    // Get current edge data;
                    const int& currentNeighborIndex = currentRoad.to;
                    const int& currentRoadWeight = currentRoad.weight;

                    // Get current neighbor data
                    const Location& currentNeighbor = locations.at(currentNeighborIndex);
                    const std::string& currentNeighborName = currentNeighbor.name;

                    // append edge data to console
                    output += "\t-> " + currentNeighborName + " (" + std::to_string(currentRoadWeight) + " km)\n";

                };

                output += '\n';

            };

            return output;
        };

        /**
         * Finds and formats the shortest route between two locations.
         *
         * This method uses Dijkstra's shortest-path algorithm to determine
         * the minimum-distance route from the starting location to the
         * destination location.
         *
         * The method maintains a distance value for each location and a
         * predecessor value that is used to reconstruct the shortest path
         * once the algorithm has completed.
         *
         * from - The index of the starting location.
         * to - The index of the destination location.
         *
         * Returns a formatted string describing the shortest route and its total distance.
         * If either index is invalid, an error message is returned.
         * If no route exists, a message indicating that no path exists is returned.
         */
        std::string shortestPathToString(
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

            output += "The shortest route from " + fromLocation.name + " to " + toLocation.name + "\n\n";

            // Report when no path exists between the selected parks
            if (path.empty()) {
                output += "No path exists between " + fromLocation.name + " and " + toLocation.name;
                return output;
            };

            // Append each location in the path from start to destination
            for (unsigned int i = path.size() - 1; i < path.size(); --i) {

                const unsigned int& currentLocationIndex = path.at(i);
                const Location& currentLocation = locations.at(currentLocationIndex);
                const std::string& currentLocationName = currentLocation.name;

                output += currentLocationName + (i == 0 ? "" : " -> ");

            };

            // Show the total length of the shortest path
            output += "\n\n" + std::string("Time to get there: ") + std::to_string(dist.at(to)) + "m";

            return output;

        };

};

/**
 * Entry point of the Municipal Incident Response System.
 *
 * The main function initialises the road network, response teams, and
 * municipal incidents before presenting the main menu to the user.
 *
 * The program accepts up to three optional command-line arguments:
 * 1. The random-number generator seed.
 * 2. The number of response teams to generate.
 * 3. The number of incidents to generate.
 *
 * If no command-line arguments are supplied, the default values are used:
 * - Seed: 1
 * - Response teams: 6
 * - Incidents: 10
 *
 * Response teams and incidents are generated using the supplied seed,
 * allowing the generated data to be reproduced when the same seed and
 * generation parameters are used.
 *
 * argc - The number of command-line arguments, including the program name.
 * argv - An array containing the command-line arguments.
 *
 * Returns 0 if the program terminates successfully.
 * Returns 1 if invalid or excessive command-line arguments are supplied.
 */
int main(int argc, const char* argv[]) {

    /**
     * Validate the maximum number of command-line arguments.
     *
     * The program name is included in argc, so a maximum argc value of 4
     * allows three additional arguments.
     */
    if (argc >= 5) {
        std::cout << "Error: too many arguements provided, only 3 additioanl arguements can be provided excluding the file name";
        return 1;
    };

    const unsigned int MAX_NUMBER_OF_ARGS = 3;

    /**
     * Stores the configurable program arguments.
     *
     * Default values are:
     * - args[0]: Random-number generator seed.
     * - args[1]: Number of response teams.
     * - args[2]: Number of incidents.
     *
     * Command-line arguments replace these defaults when provided.
     */
    unsigned int args[MAX_NUMBER_OF_ARGS] = {
        1,
        6,
        10
    };

    /**
     * Process each supplied command-line argument.
     *
     * Each argument is converted from a string to an unsigned integer.
     * If an argument cannot be converted, the program displays an error
     * message and terminates.
     */
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

    // Extract the program configuration values.
    const unsigned int SEED = args[0];
    const unsigned int NUMBER_OF_REPONSE_TEAMS = args[1];
    const unsigned int NUMBER_OF_INCIDENTS = args[2];

    // Create the municipal road network.
    RoadNetwork roadNetwork;

    /**
     * Populate the road network with all predefined locations.
     *
     * Each LocationIndex is converted into a Location object containing
     * its ID, full name, and short name.
     */
    for (unsigned int i = 0; i < LocationIndexCount; ++i) {
        const LocationIndex currentLocationIndex = static_cast<LocationIndex>(i);
        roadNetwork.addLocation({
            .id = currentLocationIndex,
            .name = locationIndexToName(currentLocationIndex)
        });
    };

    /**
     * Populate the road network with its predefined roads.
     *
     * The roads associated with each location are retrieved and added
     * to the road network. Roads are added as undirected connections
     * unless otherwise specified by the RoadNetwork implementation.
     */
    for (unsigned int i = 0; i < LocationIndexCount; ++i) {
        const LocationIndex currentLocationIndex = static_cast<LocationIndex>(i);
        const Roads currentRoads = locationIndexToRoads(currentLocationIndex);
        roadNetwork.addRoads(currentLocationIndex, currentRoads);
    };

    // Create the random-number generator.
    RandomNumberGenerator randomNumberGenerator(SEED);

    // Create the response team manager.
    ResponseTeamManager responseTeamManager;

    /**
     * Generate the configured number of response teams.
     *
     * Each response team is randomly assigned:
     * - A location.
     * - A service capability.
     * - An operational status.
     *
     * The generated team is then added to the ResponseTeamManager,
     * which assigns it a unique ID.
     */
    for (unsigned int i = 0; i < NUMBER_OF_REPONSE_TEAMS; ++i) {
        ResponseTeam randomResponseTeam({
            .locationIndex = randomNumberGenerator.ran<LocationIndex>(0, LocationIndexCount - 1),
            .capability = randomNumberGenerator.ran<ServiceType>(0, ServiceTypeCount - 1),
            .status = randomNumberGenerator.ran<ResponseTeam::Status>(0, ResponseTeam::Status::StatusCount - 1)
        });
        responseTeamManager.addResponseTeam(randomResponseTeam);
    };

    // Ensure that at least three response teams are initially available.
    unsigned int currentResponseTeamId = 1;
    while (
        responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Available) <= 2 &&
        currentResponseTeamId <= NUMBER_OF_REPONSE_TEAMS
    ) {
        responseTeamManager.setResponseTeamStatusById(
            currentResponseTeamId,
            ResponseTeam::Status::Available
        );
        ++currentResponseTeamId;
    };

    // Create the incident manager.
    IncidentsManager incidentsManager;

    /**
     * Generate the configured number of municipal incidents.
     *
     * Each incident is randomly assigned:
     * - A location.
     * - A service category.
     * - A severity level.
     *
     * All generated incidents initially have an Open status and are
     * added to the IncidentsManager, which assigns each incident a
     * unique ID.
     */
    for (unsigned int i = 0; i < NUMBER_OF_INCIDENTS; ++i) {
        Incident randomIncident({
            .locationIndex = randomNumberGenerator.ran<LocationIndex>(0, LocationIndexCount - 1),
            .category = randomNumberGenerator.ran<ServiceType>(0, ServiceTypeCount - 1),
            .severity = randomNumberGenerator.ran<Incident::Severity>(0, Incident::SeverityCount - 1),
            .status = Incident::Status::Open
        });
        incidentsManager.addIncident(randomIncident);
    };

    /**
     * Controls whether the main program loop should terminate.
     *
     * The variable is initially false and is expected to be set to true
     * when the user selects an option that sets exit to true
     */
    bool exit = false;

    // Header displayed at the beginning of every menu
    const std::string HEADER =
        "\n"
        "Programmer: Rowan Van Zyl\n"
        "Student number: 25939831\n";

    // Main menu displayed to the user.
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

    // Stores the user's current menu input.
    std::string input = "";

    // Stores the first character of the user's menu selection.
    char inputChar = ' ';

    /**
     * Main application loop.
     *
     * Displays the main menu, obtains the user's selection, and executes
     * the corresponding functionality. The loop continues until the user
     * selects the Exit option or a fatal error occurs.
     */
    do {

        clearScreen();

        std::cout << MAIN_MENU;

        input = getUserInput();

        if (input.size() == 1) {
            inputChar = input.at(0);
        } else {
            inputChar = ' ';
        };

        /**
         * Process the selected main-menu option.
         *
         * Each case corresponds to an option displayed in MAIN_MENU.
         */
        switch (inputChar) {

            /**
             * Display the locations and roads in the road network.
             *
             * Builds and displays a formatted representation of the complete
             * road network, including locations and the roads connecting them.
             */
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

            /**
             * Display all municipal incidents.
             *
             * Displays the current state of every incident managed by the
             * IncidentsManager.
             */
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

            /**
             * Display open incidents in prioritised order.
             *
             * Retrieves all open incidents and displays them according to
             * their priority, with higher-severity incidents prioritised.
             */
            case '3': {

                const std::string PRIORITISE_OPEN_INCIDENTS_MENU =
                    HEADER +
                    "========================================\n"
                    " PRIORITISE OPEN INCIDENTS MENU\n"
                    "========================================\n"
                    "\n" +
                    incidentsManager.prioritisedIncidentsToString()
                ;

                clearScreen();
                std::cout << PRIORITISE_OPEN_INCIDENTS_MENU;
                waitForEnter();
                break;
            };

            /**
             * Display all municipal response teams.
             *
             * Displays the current location, capability, and status of every
             * response team managed by the ResponseTeamManager.
             */
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

            /**
             * Investigate a specific municipal incident.
             *
             * Prompts the user for an incident ID and displays the selected
             * incident together with all available response teams that have
             * the capability required by the incident.
             *
             * The supplied incident ID is validated before the incident is
             * accessed.
             */
            case '5': {

                const std::string INVESTIGATE_INCIDENT_MENU =
                    HEADER +
                    "========================================\n"
                    " INVESTIGATE INCIDENT MENU\n"
                    "========================================\n"
                    "\n" +
                    incidentsManager.toString() +
                    "Enter incident id: "
                ;

                clearScreen();
                std::cout << INVESTIGATE_INCIDENT_MENU;

                // Obtain the incident ID supplied by the user.
                const std::string INPUT_INCIDENT_ID_STRING = getUserInput();

                bool ok = false;

                long long inputIdLongLong = stringToLongLong(INPUT_INCIDENT_ID_STRING, &ok);

                // Validate that the incident ID is a valid number.
                if (!ok) {
                    std::cout
                        << std::endl
                        << "Error: Incident id must be from 1 to " << NUMBER_OF_INCIDENTS << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                // Locate the requested incident.
                const Incident* incidentPointer = incidentsManager.getIncidentById(inputIdLongLong);

                // Handle an incident ID that does not exist.
                if (incidentPointer == nullptr) {
                    std::cout
                        << std::endl
                        << "Incident with id '" << INPUT_INCIDENT_ID_STRING << "' does not exist" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                // Obtain the selected incident and its required service capability.
                const Incident& incident = *incidentPointer;
                const ServiceType serviceType = incident.getState().category;

                /**
                 * Display the incident information and suitable teams.
                 *
                 * Only response teams that are both available and capable
                 * of handling the incident's service category are displayed.
                 */
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

            /**
             * Display the incident information and suitable teams.
             *
             * Only response teams that are both available and capable
             * of handling the incident's service category are displayed.
             */
            case '6': {

                // Build a list of all valid location indices.
                std::string locationList = "";
                for (unsigned int i = 0; i < LocationIndexCount; ++i) {
                    LocationIndex locationIndex = static_cast<LocationIndex>(i);
                    locationList += std::to_string(locationIndex) + " - " + locationIndexToName(locationIndex) + "\n";
                };

                // Display the available locations and request the starting location.
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

                // Obtain the starting location index.
                const std::string fromIndexString = getUserInput();

                // Validate the starting location index.
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

                // Obtain the destination location index.
                const std::string toIndexString = getUserInput();

                // Validate the destination location index.
                const bool isToIndexStringValid = roadNetwork.isLocationIndexStringValid(toIndexString);
                if (!isToIndexStringValid) {
                    std::cout << "Error: to index is not one of the above displayed options" << std::endl;
                    waitForEnter();
                    break;
                };

                // Convert the validated location indices to integers.
                const unsigned int fromIndex = std::stoul(fromIndexString);
                const unsigned int toIndex = std::stoul(toIndexString);

                // Calculate the shortest route between the locations.
                const std::string shortestPathString = roadNetwork.shortestPathToString(fromIndex, toIndex);

                // Display the calculated route.
                std::cout
                    << std::endl
                    << shortestPathString << std::endl
                    << std::endl
                ;

                waitForEnter();

                break;
            };

            /**
             * Update the state of an existing incident.
             *
             * Prompts the user for an incident ID and displays the actions
             * available for the incident based on its current status.
             *
             * Open incidents can have a response team assigned.
             * Assigned incidents can have their team unassigned or can be
             * resolved.
             * Resolved incidents can be reopened.
             */
            case '7': {

                // Display all incidents and request an incident ID.
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

                // Obtain the incident ID.
                const std::string INPUT_INCIDENT_ID_STRING = getUserInput();

                // validate the incident ID.
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

                // Locate the incident to be updated.
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

                // Extract the current state of the selected incident.
                const Incident& incident = *incidentPointer;
                const long long incidentId = incident.getState().id;
                const LocationIndex incidentLocationIndex = incident.getState().locationIndex;
                const ServiceType serviceType = incident.getState().category;
                const Incident::Status incidentStatus = incident.getState().status;
                const long long assignedTeamId = incident.getState().assignedTeamId;

                // Stores the update options available for the incident's current status.
                std::string optionsText = "";

                // Determine which operations are valid for the incident's current status.
                switch (incidentStatus) {

                    // Open incidents may have a response team assigned.
                    case Incident::Status::Open: {
                        optionsText =
                            "1. Assign team\n"
                        ;
                        break;
                    };

                    // Assigned incidents may have their team unassigned or may be resolved.
                    case Incident::Status::Assigned: {
                        optionsText =
                            "1. Unassign team\n"
                            "2. Resolve incident\n"
                        ;
                        break;
                    };

                    // Resolved incidents may be reopened.
                    case Incident::Status::Resolved: {
                        optionsText =
                            "1. Reopen incident\n"
                        ;
                        break;
                    };

                    // Handle an invalid incident status.
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

                // Add the cancellation option to the update menu.
                optionsText += "0. Cancel";

                // Display the incident and available update options.
                std::cout
                    << std::endl
                    << "Incident info:" << std::endl
                    << incidentsToString({ incident }) << std::endl
                    << "Options:" << std::endl
                    << optionsText << std::endl
                    << std::endl
                    << "Enter option: ";
                ;

                // Obtain the user's selected update operation.
                const std::string INPUT_OPTION = getUserInput();

                // Validate input
                if (INPUT_OPTION.size() >= 2) {
                    std::cout
                        << std::endl
                        << "Error: input option must be a single character" << std::endl
                        << std::endl
                    ;
                    waitForEnter();
                    break;
                };

                // Extract selected update operation from input
                const char INPUT_OPTION_CHAR = INPUT_OPTION.at(0);

                // Execute the operation appropriate to the incident's current status.
                switch (incidentStatus) {

                    /**
                     * Handle operations for an open incident.
                     *
                     * Option 1 assigns a suitable and available response
                     * team to the incident.
                     */
                    case Incident::Status::Open: {
                        if (INPUT_OPTION_CHAR == '1') {
                            try {

                                // Verify that at least one suitable
                                if (!responseTeamManager.doesCapableAndAvailableResponseTeamExist(serviceType)) {
                                    std::cout
                                        << std::endl
                                        << "No response teams with '" << serviceTypeToString(serviceType) << "' capability are available and exist" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                // Display suitable available teams and request the team ID.
                                std::cout
                                    << "Available and suitable response teams:\n"
                                    << responseTeamManager.suitableAndAvailableTeamsToString(serviceType)
                                    << "Enter team id: "
                                ;
                                const std::string INPUT_TEAM_ID_STRING = getUserInput();

                                bool ok = false;

                                const long long INPUT_TEAM_ID_LONG_LONG = stringToLongLong(INPUT_TEAM_ID_STRING, &ok);

                                // Validate the response team ID.
                                if (!ok) {
                                    std::cout
                                        << std::endl
                                        << "Error: Response team id must be from 1 to " << NUMBER_OF_REPONSE_TEAMS << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                // Verify that the selected team exists.
                                if (!responseTeamManager.doesResponseTeamExist(INPUT_TEAM_ID_LONG_LONG)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' does not exist" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                // Verify that the selected team has the capability required by the incident.
                                if (!responseTeamManager.isResponseTeamCapable(INPUT_TEAM_ID_LONG_LONG, serviceType)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' is not capable of doing this job" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                // Verify that the selected team is currently available.
                                if (!responseTeamManager.isResponseTeamAvailable(INPUT_TEAM_ID_LONG_LONG)) {
                                    std::cout
                                        << std::endl
                                        << "Response team with id '" << INPUT_TEAM_ID_STRING << "' is not available" << std::endl
                                        << std::endl
                                    ;
                                    break;
                                };

                                // Assign the selected response team to the incident.
                                incidentsManager.assignTeamToIncident(
                                    incidentId,
                                    INPUT_TEAM_ID_LONG_LONG
                                );

                                // Mark the response team as assigned.
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

                    /**
                     * Handle operations for an assigned incident.
                     *
                     * Option 1 unassigns the current response team.
                     * Option 2 resolves the incident and makes the assigned
                     * response team available again.
                     */
                    case Incident::Status::Assigned: {
                        if (INPUT_OPTION_CHAR == '1') {
                            // Unassign the response team from the incident
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
                            // Resolve the incident and release its team.
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

                    /**
                     * Handle operations for a resolved incident.
                     *
                     * Option 1 reopens the incident and returns it to the
                     * Open state.
                     */
                    case Incident::Status::Resolved: {
                        if (INPUT_OPTION_CHAR == '1') {
                            // Reopen the resolved incident.
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
                        };
                        break;
                    };

                    /**
                     * Handle cancellation and invalid input.
                     *
                     * Selecting 0 cancels the update. Any other value that
                     * does not correspond to an available option is reported
                     * as invalid input.
                     */
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

            /**
             * Display a summary of the current system state.
             *
             * Displays the number of incidents in each status, the total
             * number of incidents, the number of response teams in each
             * status, the total number of response teams, and the current
             * response-team utilisation.
             */
            case '8': {

                // Calculate incident counts by status.
                const std::string OPEN_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Open));
                const std::string ASSIGNED_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Assigned));
                const std::string RESOLVED_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCountByStatus(Incident::Status::Resolved));
                const std::string TOTAL_INCIDENTS_COUNT_STRING = std::to_string(incidentsManager.getIncidentsCount());

                // Calculate response-team counts by status.
                const std::string AVAILABLE_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Available));
                const std::string ASSIGNED_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Assigned));
                const std::string UNAVAILABLE_RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCountByStatus(ResponseTeam::Status::Unavailable));
                const std::string RESPONSE_TEAMS_COUNT_STRING = std::to_string(responseTeamManager.getResponseTeamsCount());

                /**
                 * Construct and display the system summary.
                 *
                 * The summary provides an overview of incident status,
                 * response-team status, and team utilisation.
                 */
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

            /**
             * Exit the Municipal Incident Response System.
             *
             * Sets the exit flag to true, causing the main application
             * loop to terminate after the current iteration.
             */
            case '0': {
                std::cout
                    << std::endl
                    << "Gracefully Exiting application..."
                    << std::endl
                ;
                exit = true;
                break;
            };

            /**
             * Handle invalid main-menu input.
             *
             * Displays an error message when the user enters a value that
             * does not correspond to one of the available main-menu options.
             */
            default: {
                std::cout
                    << std::endl
                    << "Error: input '" << input << "' is not a valid option (1, 2, 3, 4, 5, 6, 7, 8 or 0)" << std::endl
                ;
                waitForEnter();
                break;
            };

        };

    // Continue displaying the main menu until the user exits.
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
This gave me compile-time safety and made it easy to map each location
to its name and list of outgoing roads using parallel arrays.
For the roads, I used an adjacency list stored as an array of vectors (Roads).
I chose this over an adjacency matrix because the network is relatively sparse (only 15-20 edges),
so a list saves memory and makes iterating over neighbours straightforward.
Each Road struct stores the destination index and the travel weight in minutes.

For incidents and response teams, I used classes (Incident and ResponseTeam) that each hold their state in a simple struct.
This made it easy to group related data while keeping the classes lightweight.
I then created two manager classes (IncidentsManager and ResponseTeamManager).
Each manager owns a std::vector of its respective objects and provides methodsfor common operations,
such as finding an item by ID, counting items by status/capability, and updating state.
The IdGenerator class provides a clean way to assign unique sequential IDs to both incidents and teams without mixing them.

For the route-finding problem, I chose Dijkstra's algorithm because the road network has non-negative travel times,
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
especially when users enter non-numeric values for IDs.
I created a helper function (stringToLongLong) that safely attempts conversion and returns a boolean flag to indicate success.
If the input is invalid, the program displays a clear error message and returns to the menu without crashing.
I also added checks to ensure that selected incident IDs and team IDs actually exist before performing any operations on them.

I tested my program by 
I tested my program by combining manual testing with an automated Windows batch test script.
Manual testing was used during development to verify individual functions and menu options,
while the automated test script was used to repeatedly test complete user interaction sequences against the compiled application.
The testing covered normal and invalid input for all menu options, including displaying the locations and roads, displaying
incidents and response teams, prioritising incidents, investigating incidents, finding routes, updating incident statuses,
and displaying the summary. I also specifically tested state transitions involving incident and response team assignment to
ensure that both objects were updated consistently. For the route-finding functionality, I tested routes between
multiple locations and compared the resulting paths and travel times against the expected shortest routes through
the road network. I also tested invalid location selections and ensured that invalid input did not cause the application
to terminate unexpectedly. The automated test script supplies predefined input to the application through standard
input (stdin), allowing repeatable tests to be performed without manually entering each value. This was particularly
useful for testing sequences of related operations, such as assigning a team to an incident and then verifying that the
resulting status changes were reflected throughout the system. I also tested the application using the assignment's
required configuration of seed 2611, 6 response teams, and 10 incidents. Because the random-number generator uses a
fixed seed, the same configuration produces the same operational data on every run, making the results reproducible and
allowing the automated tests to operate against a predictable scenario. Overall, the testing confirmed that the application's
core functionality operates correctly, including data generation, incident prioritisation, shortest-path routing,
team assignment, status management, input validation, and summary calculations. The combination of manual and automated
testing also helped identify and correct issues with input handling and maintaining consistent state between incidents and response teams.
*/
