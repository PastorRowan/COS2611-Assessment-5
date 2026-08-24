
/*
Name: Rowan
Surname: Van Zyl
Student Number: 25939831
Module: COS2611
Assignment: Assessment 5 (FPP)

Please see my github repo for more info: 
*/

enum ServiceType {
    Water,
    Electricity,
    Roads,
    Sanitation,
    PublicFacilities,
    Count
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

        ResponseTeam();

};


class Incident {

    public:

        enum Severity {
            Low,
            Medium,
            High
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

        Incident(
            IdGenerator idGeneratorP
        ): idGenerator(idGeneratorP);

};

const unsigned int SEED = 0;

int main() {

    const char[] MAIN_MENU = R"(
Programmer: Rowan Van Zyl
Student number: 25939831
========================================
 MUNICIPAL INCIDENT RESPONSE SYSTEM
========================================
)";

};
