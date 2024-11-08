#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>

using namespace std;

const int MAX_PASSENGERS = 200;
const int MAX_NAMELENGTH = 50;
const int MAX_NATIONALCODELENGTH = 12;
const int MAX_FLIGHTS = 100;

struct Passenger
{
    char name[MAX_NAMELENGTH];
    char lastName[MAX_NAMELENGTH];
    char nationalCode[MAX_NATIONALCODELENGTH];
    float weight;
    int baggageWeight;
    char seatRow;
    int seatCol;
};

struct Flight
{
    char flightNumber[100];
    char source[100];
    char destination[100];
    time_t takeOffTime;
    long long int duration;
    bool landing;
    int ROWS = 4;
    int COLUMS = 50;
    float planeWeight = 40;
    float fuelWeight = 10;
    float maxTakeoffWeight = 100;
    Passenger passengers[MAX_PASSENGERS];
    int countingPasseneger;
};

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
int formatString(char *buffer, size_t size, const char *format, const char *str)
{
    if (size == 0)
    {
        return 0;
    }

    size_t length = 0;
    while (*format && length < size - 1)
    {
        if (*format == '%' && *(format + 1) == 's')
        {
            format += 2;
            while (*str && length < size - 1)
            {
                buffer[length++] = *str++;
            }
        }
        else
        {
            buffer[length++] = *format++;
        }
    }

    buffer[length] = '\0';
    return length;
}

void readFlights(const char *input, Flight flights[], int &flightCount)
{
    ifstream infile(input);
    if (!infile)
    {
        cerr << "cant open your file " << input << endl;
        return;
    }

    infile >> flightCount;
    for (int i = 0; i < flightCount; i++)
    {
        infile >> flights[i].flightNumber >> flights[i].source >> flights[i].destination >> flights[i].takeOffTime >> flights[i].duration;
        flights[i].landing = (strcmp(flights[i].destination, "Tehran") == 0);
    }

    infile.close();
}

void readPassengers(const char *filename, Flight &flight)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "cant open your file " << filename << endl;
        return;
    }

    flight.countingPasseneger = 0;
    while (file >> flight.passengers[flight.countingPasseneger].name >> flight.passengers[flight.countingPasseneger].lastName >> flight.passengers[flight.countingPasseneger].nationalCode >> flight.passengers[flight.countingPasseneger].weight >> flight.passengers[flight.countingPasseneger].baggageWeight)
    {
        flight.countingPasseneger++;
    }

    file.close();
}

void assignSeats(Flight &flight)
{
    int seatIndex = 0;
    char rows[] = {'A', 'B', 'C', 'D'};
    int NumRows = flight.ROWS;
    int NumCols = flight.COLUMS;

    //  24 to 1
    for (int col = 24; col >= 0 && seatIndex < flight.countingPasseneger; col--)
    {
        for (int row = 0; row < NumRows && seatIndex < flight.countingPasseneger; row++)
        {
            flight.passengers[seatIndex].seatRow = rows[row];
            flight.passengers[seatIndex].seatCol = col + 1;
            seatIndex++;
        }
    }

    // 26 to 50
    for (int col = 25; col < NumCols && seatIndex < flight.countingPasseneger; col++)
    {
        for (int row = 0; row < NumRows && seatIndex < flight.countingPasseneger; row++)
        {
            flight.passengers[seatIndex].seatRow = rows[row];
            flight.passengers[seatIndex].seatCol = col + 1;
            seatIndex++;
        }
    }
}

bool checkWeight(const Flight &flight)
{
    float totalWeight = flight.planeWeight + flight.fuelWeight;
    for (int i = 0; i < flight.countingPasseneger; ++i)
    {
        totalWeight += flight.passengers[i].weight + flight.passengers[i].baggageWeight;
    }
    return totalWeight <= flight.maxTakeoffWeight;
}

void displayFlightCard(Flight &flight)
{
    if (checkWeight(flight))
    {
        assignSeats(flight);
        for (int i = 0; i < flight.countingPasseneger; ++i)
        {
            const Passenger &passen = flight.passengers[i];
            cout << "Seat " << passen.seatRow << passen.seatCol << " for " << passen.name << " " << passen.lastName << " " << passen.nationalCode << endl;
        }
    }
    else
    {
        cout << "Sorry your out of weight limit" << endl;
    }
    cout << endl;
}

void sortFlights(Flight flights[], int flightCount)
{
    for (int i = 0; i < flightCount - 1; i++)
    {
        for (int j = 0; j < flightCount - 1 - i; j++)
        {
            if (flights[j].takeOffTime > flights[j + 1].takeOffTime)
            {
                Flight temp = flights[j];
                flights[j] = flights[j + 1];
                flights[j + 1] = temp;
            }
        }
    }
}

void displayFlights(Flight flights[], int flightCount)
{
    time_t currentTime = time(0);
    currentTime = 100;

    cout << "coming Flights:" << endl;
    cout << endl;

    int count = 0;
    for (int i = 0; i < flightCount; ++i)
    {
        double TimeDiffenrent = difftime(flights[i].takeOffTime, currentTime);

        if (flights[i].landing && TimeDiffenrent > 0 && TimeDiffenrent <= 3 * 3600 && TimeDiffenrent > 3600)
        {
            cout << "Flight Number: " << flights[i].flightNumber << "  ";
            cout << "Source: " << flights[i].source << "  ";
            cout << "Destination: " << flights[i].destination << "  ";
            cout << "Take-off Time: " << ctime(&flights[i].takeOffTime);
            cout << "Duration: " << flights[i].duration << " minutes" << endl;
            cout << "=========================================================" << endl;
            count++;

            if (count >= 5)
            {
                break;
            }
        }
    }

    cout << "going Flights:" << endl;
    cout << endl;

    count = 0;
    for (int i = 0; i < flightCount; ++i)
    {
        double timeDifferent = difftime(flights[i].takeOffTime, currentTime);
        if (!flights[i].landing && timeDifferent > 0 && timeDifferent <= 3 * 3600)
        {
            cout << "Flight Number: " << flights[i].flightNumber << "  ";
            cout << "Source: " << flights[i].source << "  ";
            cout << "Destination: " << flights[i].destination << "  ";
            cout << "Takeoff Time: " << ctime(&flights[i].takeOffTime);
            cout << "Duration: " << flights[i].duration << " minutes" << endl;
            cout << "=========================================================" << endl;
            count++;

            if (count >= 5)
            {
                break;
            }
        }
    }
}

void searchPassenegerByNationalCode(Flight flights[], int flightCount, const char *nationalCode)
{
    for (int i = 0; i < flightCount; ++i)
    {
        for (int j = 0; j < flights[i].countingPasseneger; ++j)
        {
            if (strcmp(flights[i].passengers[j].nationalCode, nationalCode) == 0)
            {
                const Passenger &passeng = flights[i].passengers[j];
                cout << "Passenger " << passeng.name << " " << passeng.lastName << " is on Flight " << flights[i].flightNumber << endl;
                cout << "Seat: " << passeng.seatRow << passeng.seatCol << endl;
                cout << "Source: " << flights[i].source << endl;
                cout << "Destination: " << flights[i].destination << endl;
                cout << "Takeoff Time: " << ctime(&flights[i].takeOffTime);
                cout << "Duration: " << flights[i].duration << " minutes" << endl;
                return;
            }
        }
    }
    cout << "sorry your not in any flights" << endl;
}

int main()
{
    Flight flights[MAX_FLIGHTS];
    int countingFlight;
    readFlights("input.txt", flights, countingFlight);
    sortFlights(flights, countingFlight);

    while (true)
    {
        system("clear");
        displayFlights(flights, countingFlight);
        for (int i = 0; i < countingFlight; ++i)
        {
            char passengerFilename[100];
            formatString(passengerFilename, sizeof(passengerFilename), "%s.txt", flights[i].flightNumber);
            readPassengers(passengerFilename, flights[i]);
            cout << "Flight Number: " << flights[i].flightNumber << endl;
            cout << "Source: " << flights[i].source << endl;
            cout << "Destination: " << flights[i].destination << endl;
            cout << "Takeoff Time: " << ctime(&flights[i].takeOffTime);
            cout << "Duration: " << flights[i].duration << " minutes" << endl;
            cout << endl;

            readPassengers(passengerFilename, flights[i]);
            displayFlightCard(flights[i]);
        }

        char nationalCode[MAX_NATIONALCODELENGTH];
        cout << "Please enter your National Code to let us find yourseat for you darling";
        cin >> nationalCode;
        searchPassenegerByNationalCode(flights, countingFlight, nationalCode);

        sleep(30);
    }

    return 0;
}
