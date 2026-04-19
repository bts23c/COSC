
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <vector>

using namespace std;

// Named constants for prices and fees to avoid "magic numbers"
const double ADULT_EVE = 34.99;
const double ADULT_MAT = 25.97;
const double SENIOR_EVE = 30.99;
const double SENIOR_MAT = 22.97;
const double STUDENT_EVE = 24.99;
const double STUDENT_MAT = 18.97;
const double FEE = 0.50;
const double GROUP_DISCOUNT = 0.075;
const int GROUP_THRESHOLD = 10;

// Struct to keep track of category totals
struct CategoryTotals {
    int adult = 0;
    int senior = 0;
    int student = 0;
    int acc = 0;
    double cost = 0.0;
};

int main() {
    // Greet the user and prompt for the file name
    cout << "Welcome to the MCMT bulk ticket processing system!" << endl;
    cout << "Please enter the path of the data file to be processed: ";
    string filePath;
    cin >> filePath;

    // Attempt to open the file
    ifstream inFile(filePath);
    if (!inFile) {
        cout << "***ERROR*** The file path you entered cannot be opened." << endl;
        cout << "The program will now exit." << endl;
        return 1;
    }

    // Read and discard the first line (column headings)
    string headerLine;
    getline(inFile, headerLine);

    // Track overall record counts
    int validRecords = 0;
    int invalidRecords = 0;

    // Track totals per performance time
    CategoryTotals friEve, satMat, satEve, sunMat;

    // Table Header
    cout << "\n" << left 
         << setw(6) << "Day" 
         << setw(6) << "Time" 
         << setw(10) << "Adult Tix" 
         << setw(10) << "Sen. Tix" 
         << setw(10) << "Stud. Tix" 
         << setw(12) << "Needs Acc?" 
         << setw(10) << "Acc. Tix" 
         << "Total Cost" << endl;
    cout << string(75, '-') << endl;

    // Variables to hold line data
    char day, timeChar, accChar;
    int adult, senior, student, accNum;
    string contactInfo;

    // Process each line
    while (inFile >> day >> timeChar >> adult >> senior >> student >> accChar >> accNum) {
        // Read the rest of the line as contact info
        getline(inFile, contactInfo);
        if (!contactInfo.empty() && contactInfo[0] == ' ') {
            contactInfo.erase(0, 1); // Remove leading space
        }

        // Output raw data read
        cout << left 
             << setw(6) << day 
             << setw(6) << timeChar 
             << setw(10) << adult 
             << setw(10) << senior 
             << setw(10) << student 
             << setw(12) << accChar 
             << setw(10) << accNum;

        // Data Validation Flags & Error Tracking
        vector<string> errors;
        char lDay = tolower(day);
        char lTime = tolower(timeChar);
        char lAcc = tolower(accChar);
        int totalTix = adult + senior + student;

        if (lDay != 'f' && lDay != 's' && lDay != 'u') {
            errors.push_back("Invalid performance day.");
        }
        if (lTime != 'm' && lTime != 'e') {
            errors.push_back("Invalid performance time.");
        }
        if (lDay == 'f' && lTime != 'e') {
            errors.push_back("There is no matinee performance on Fridays.");
        }
        if (lDay == 'u' && lTime != 'm') {
            errors.push_back("There is no evening performance on Sundays.");
        }
        if (adult < 0 || senior < 0 || student < 0 || totalTix <= 0) {
            errors.push_back("The total number of tickets is not positive.");
        }
        if (lAcc != 'y' && lAcc != 'n') {
            errors.push_back("Invalid accessibility character.");
        }
        if (lAcc == 'y') {
            if (accNum < 0 || accNum > (totalTix / 2.0)) {
                errors.push_back("Accessible tickets must be between zero and 50% of the total.");
            }
        } else if (lAcc == 'n') {
            if (accNum != 0) {
                errors.push_back("Number of accessible tickets must be exactly zero if not requested.");
            }
        }

        // Process based on validity
        if (errors.empty()) {
            validRecords++;

            // Calculate costs
            double ticketSubtotal = 0.0;
            if (lTime == 'e') {
                ticketSubtotal = (adult * ADULT_EVE) + (senior * SENIOR_EVE) + (student * STUDENT_EVE);
            } else {
                ticketSubtotal = (adult * ADULT_MAT) + (senior * SENIOR_MAT) + (student * STUDENT_MAT);
            }

            // Apply 7.5% group discount if applicable
            if (totalTix > GROUP_THRESHOLD) {
                ticketSubtotal -= (ticketSubtotal * GROUP_DISCOUNT);
            }

            // Add flat $0.50 processing fee per ticket
            double totalCost = ticketSubtotal + (totalTix * FEE);

            // Print cost
            cout << "$ " << fixed << setprecision(2) << totalCost << endl;

            // Update respective running totals
            CategoryTotals* targetCategory = nullptr;
            if (lDay == 'f') targetCategory = &friEve;
            else if (lDay == 'u') targetCategory = &sunMat;
            else if (lDay == 's' && lTime == 'm') targetCategory = &satMat;
            else if (lDay == 's' && lTime == 'e') targetCategory = &satEve;

            if (targetCategory != nullptr) {
                targetCategory->adult += adult;
                targetCategory->senior += senior;
                targetCategory->student += student;
                targetCategory->acc += accNum;
                targetCategory->cost += totalCost;
            }

        } else {
            invalidRecords++;
            cout << endl; // Leave cost column blank
            // Print all errors associated with the line
            for (const string& errorMsg : errors) {
                cout << " ***ERROR*** " << errorMsg << endl;
            }
            cout << " To fix this order, contact: " << contactInfo << endl;
        }
    }

    inFile.close();

    // Summary Statistics Output
    cout << "\n" << validRecords + invalidRecords << " records processed: " 
         << validRecords << " valid; " << invalidRecords << " invalid\n" << endl;

    // Summary Table
    cout << left 
         << setw(20) << "Performance" 
         << setw(10) << "Adult" 
         << setw(10) << "Sen." 
         << setw(10) << "Stud." 
         << setw(10) << "Acc." 
         << "Tot. Cost" << endl;
    cout << string(70, '-') << endl;

    // Helper lambda to print summary rows cleanly
    auto printSummaryRow = [](string name, CategoryTotals ct) {
        cout << left 
             << setw(20) << name 
             << setw(10) << ct.adult 
             << setw(10) << ct.senior 
             << setw(10) << ct.student 
             << setw(10) << ct.acc 
             << "$ " << fixed << setprecision(2) << ct.cost << endl;
    };

    printSummaryRow("Friday Evening", friEve);
    printSummaryRow("Saturday Matinee", satMat);
    printSummaryRow("Saturday Evening", satEve);
    printSummaryRow("Sunday Matinee", sunMat);

    // Program termination message
    cout << "\nThank you for supporting MCMT - see you at the theatre!" << endl;

    return 0;
}
