

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <limits>

using namespace std;

// --- Global Constants for Pricing ---
const double ADULT_EVE_PRICE = 34.99;
const double ADULT_MAT_PRICE = 25.97;
const double SENIOR_EVE_PRICE = 30.99;
const double SENIOR_MAT_PRICE = 22.97;
const double STUDENT_EVE_PRICE = 24.99;
const double STUDENT_MAT_PRICE = 18.97;

const int GROUP_DISCOUNT_THRESHOLD = 10;
const double GROUP_DISCOUNT_RATE = 0.075;
const double PROCESSING_FEE = 0.50;

// --- Required Function Prototypes ---
char displayMenu();

bool uploadFile(string filename, vector<string>& productionTitles,
                vector<char>& performanceDays, vector<char>& performanceTimes,
                vector<int>& adultCounts, vector<int>& seniorCounts,
                vector<int>& studentCounts,
                vector<char>& accessibilities, vector<int>& accessibilityCounts,
                vector<string>& contactDetails);

void allDetails(const vector<string>& productionTitles,
                const vector<char>& performanceDays, const vector<char>& performanceTimes,
                const vector<int>& adultCounts, const vector<int>& seniorCounts,
                const vector<int>& studentCounts,
                const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                const vector<string>& contactDetails);

void summaryByDay(const vector<string>& productionTitles,
                  const vector<char>& performanceDays, const vector<char>& performanceTimes,
                  const vector<int>& adultCounts, const vector<int>& seniorCounts,
                  const vector<int>& studentCounts,
                  const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                  const vector<string>& contactDetails);

void summaryByTitle(const vector<string>& productionTitles,
                    const vector<char>& performanceDays, const vector<char>& performanceTimes,
                    const vector<int>& adultCounts, const vector<int>& seniorCounts,
                    const vector<int>& studentCounts,
                    const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                    const vector<string>& contactDetails);

// --- Helper Function Prototype ---
double calculateCost(char time, int adult, int senior, int student);

// --- Main Program ---
int main() {
    // Parallel vectors for storing order data, strictly local to main()
    vector<string> productionTitles;
    vector<char> performanceDays;
    vector<char> performanceTimes;
    vector<int> adultCounts;
    vector<int> seniorCounts;
    vector<int> studentCounts;
    vector<char> accessibilities;
    vector<int> accessibilityCounts;
    vector<string> contactDetails;

    char choice;
    do {
        choice = displayMenu();

        switch (choice) {
            case 'U': {
                string filename;
                cout << "\nEnter the ticket data filename to upload: ";
                getline(cin, filename);
                uploadFile(filename, productionTitles, performanceDays, performanceTimes,
                           adultCounts, seniorCounts, studentCounts,
                           accessibilities, accessibilityCounts, contactDetails);
                break;
            }
            case 'A':
                if (productionTitles.empty()) {
                    cout << "\nError: No ticket data is currently loaded.\n";
                } else {
                    allDetails(productionTitles, performanceDays, performanceTimes,
                               adultCounts, seniorCounts, studentCounts,
                               accessibilities, accessibilityCounts, contactDetails);
                }
                break;
            case 'D':
                if (productionTitles.empty()) {
                    cout << "\nError: No ticket data is currently loaded.\n";
                } else {
                    summaryByDay(productionTitles, performanceDays, performanceTimes,
                                 adultCounts, seniorCounts, studentCounts,
                                 accessibilities, accessibilityCounts, contactDetails);
                }
                break;
            case 'T':
                if (productionTitles.empty()) {
                    cout << "\nError: No ticket data is currently loaded.\n";
                } else {
                    summaryByTitle(productionTitles, performanceDays, performanceTimes,
                                   adultCounts, seniorCounts, studentCounts,
                                   accessibilities, accessibilityCounts, contactDetails);
                }
                break;
            case 'C':
                // Clear all parallel vectors
                productionTitles.clear();
                performanceDays.clear();
                performanceTimes.clear();
                adultCounts.clear();
                seniorCounts.clear();
                studentCounts.clear();
                accessibilities.clear();
                accessibilityCounts.clear();
                contactDetails.clear();
                cout << "\nAll loaded data has been successfully cleared.\n";
                break;
            case 'Q':
                cout << "\nExiting MCMT Program. Goodbye!\n";
                break;
            default:
                cout << "\nError: Invalid menu choice. Please try again.\n";
        }
    } while (choice != 'Q');

    return 0;
}

// --- Function Implementations ---

char displayMenu() {
    cout << "\n=== MCMT Box Office Menu ===\n";
    cout << "(U) Upload a production ticket data file\n";
    cout << "(A) Display All loaded data details\n";
    cout << "(D) Display a summary by performance Day and time\n";
    cout << "(T) Display a summary by performance Title\n";
    cout << "(C) Clear all loaded data\n";
    cout << "(Q) Quit the program\n";
    cout << "Enter your choice: ";
    
    char choice;
    cin >> choice;
    // Clear the rest of the line to ignore extra characters
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    return toupper(choice); // Standardize input to uppercase
}

bool uploadFile(string filename, vector<string>& productionTitles,
                vector<char>& performanceDays, vector<char>& performanceTimes,
                vector<int>& adultCounts, vector<int>& seniorCounts,
                vector<int>& studentCounts,
                vector<char>& accessibilities, vector<int>& accessibilityCounts,
                vector<string>& contactDetails) {
    
    ifstream inFile(filename);
    if (!inFile) {
        cout << "\nError: Could not open file '" << filename << "'.\n";
        return false;
    }

    // Determine production name from filename
    string title = "Other";
    if (filename.find("Curtains", 0) != string::npos) title = "Curtains";
    else if (filename.find("Oklahoma", 0) != string::npos) title = "Oklahoma";
    else if (filename.find("Seussical", 0) != string::npos) title = "Seussical";
    else if (filename.find("Urinetown", 0) != string::npos) title = "Urinetown";

    string header;
    getline(inFile, header); // Read and ignore the first line (column headers)

    int validCount = 0;
    int invalidCount = 0;

    cout << "\nProcessing file: " << filename << "\n";
    cout << left << setw(15) << "Production" << setw(5) << "Day" << setw(6) << "Time"
         << setw(6) << "Adult" << setw(8) << "Senior" << setw(9) << "Student"
         << setw(8) << "Access" << setw(12) << "Acc. Tix" << "Status/Cost\n";
    cout << string(80, '-') << "\n";

    char day, time, access;
    int adult, senior, student, accessCount;
    string contact;

    // Read data line by line
    while (inFile >> day >> time >> adult >> senior >> student >> access >> accessCount) {
        getline(inFile >> ws, contact); // Get rest of line, ignoring leading whitespace

        char uDay = toupper(day);
        char uTime = toupper(time);
        char uAccess = toupper(access);

        // --- Data Validation Rules ---
        bool isValid = true;
        string errorMsg = "Invalid: ";

        if (uDay != 'F' && uDay != 'S' && uDay != 'U') {
            isValid = false; errorMsg += "Day ";
        }
        if (uTime != 'M' && uTime != 'E') {
            isValid = false; errorMsg += "Time ";
        }
        if (isValid && uDay == 'F' && uTime != 'E') {
            isValid = false; errorMsg += "Fri->Must be Eve ";
        }
        if (isValid && uDay == 'U' && uTime != 'M') {
            isValid = false; errorMsg += "Sun->Must be Mat ";
        }
        if (adult < 0 || senior < 0 || student < 0 || (adult + senior + student) <= 0) {
            isValid = false; errorMsg += "Ticket Counts ";
        }
        if (uAccess != 'Y' && uAccess != 'N') {
            isValid = false; errorMsg += "Accessibility Flag ";
        }
        if (uAccess == 'N' && accessCount != 0) {
            isValid = false; errorMsg += "Acc. Tix must be 0 ";
        }
        int totalTickets = adult + senior + student;
        if (uAccess == 'Y' && (accessCount < 0 || accessCount > totalTickets / 2)) {
            isValid = false; errorMsg += "Acc. Tix count out of bounds ";
        }

        // Output row
        cout << left << setw(15) << title << setw(5) << day << setw(6) << time
             << setw(6) << adult << setw(8) << senior << setw(9) << student
             << setw(8) << access << setw(12) << accessCount;

        if (isValid) {
            // Append to vectors
            productionTitles.push_back(title);
            performanceDays.push_back(uDay);
            performanceTimes.push_back(uTime);
            adultCounts.push_back(adult);
            seniorCounts.push_back(senior);
            studentCounts.push_back(student);
            accessibilities.push_back(uAccess);
            accessibilityCounts.push_back(accessCount);
            contactDetails.push_back(contact);

            double cost = calculateCost(uTime, adult, senior, student);
            cout << "$" << fixed << setprecision(2) << cost << "\n";
            validCount++;
        } else {
            cout << errorMsg << "\n";
            invalidCount++;
        }
    }

    inFile.close();
    cout << "\nUpload complete. Valid orders: " << validCount << " | Invalid orders: " << invalidCount << "\n";
    
    return validCount > 0;
}

void allDetails(const vector<string>& productionTitles,
                const vector<char>& performanceDays, const vector<char>& performanceTimes,
                const vector<int>& adultCounts, const vector<int>& seniorCounts,
                const vector<int>& studentCounts,
                const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                const vector<string>& contactDetails) {
    
    cout << "\n--- All Loaded Ticket Orders ---\n";
    cout << left << setw(15) << "Production" << setw(5) << "Day" << setw(6) << "Time"
         << setw(6) << "Adult" << setw(8) << "Senior" << setw(9) << "Student"
         << setw(8) << "Access" << setw(12) << "Acc. Tix" << "Total Cost\n";
    cout << string(80, '-') << "\n";

    for (size_t i = 0; i < productionTitles.size(); i++) {
        double cost = calculateCost(performanceTimes[i], adultCounts[i], seniorCounts[i], studentCounts[i]);
        cout << left << setw(15) << productionTitles[i] << setw(5) << performanceDays[i] << setw(6) << performanceTimes[i]
             << setw(6) << adultCounts[i] << setw(8) << seniorCounts[i] << setw(9) << studentCounts[i]
             << setw(8) << accessibilities[i] << setw(12) << accessibilityCounts[i]
             << "$" << fixed << setprecision(2) << cost << "\n";
    }
}

void summaryByDay(const vector<string>& productionTitles,
                  const vector<char>& performanceDays, const vector<char>& performanceTimes,
                  const vector<int>& adultCounts, const vector<int>& seniorCounts,
                  const vector<int>& studentCounts,
                  const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                  const vector<string>& contactDetails) {
    
    // Arrays for accumulation (Indices: 0=Fri, 1=Sat Mat, 2=Sat Eve, 3=Sun)
    int tAdult[4] = {0}, tSenior[4] = {0}, tStudent[4] = {0}, tAccess[4] = {0};
    double tSales[4] = {0.0};
    string categories[] = {"Friday", "Saturday Matinee", "Saturday Evening", "Sunday"};

    for (size_t i = 0; i < productionTitles.size(); i++) {
        int idx = -1;
        if (performanceDays[i] == 'F') idx = 0;
        else if (performanceDays[i] == 'S' && performanceTimes[i] == 'M') idx = 1;
        else if (performanceDays[i] == 'S' && performanceTimes[i] == 'E') idx = 2;
        else if (performanceDays[i] == 'U') idx = 3;

        if (idx != -1) {
            tAdult[idx] += adultCounts[i];
            tSenior[idx] += seniorCounts[i];
            tStudent[idx] += studentCounts[i];
            tAccess[idx] += accessibilityCounts[i];
            tSales[idx] += calculateCost(performanceTimes[i], adultCounts[i], seniorCounts[i], studentCounts[i]);
        }
    }

    cout << "\n--- Summary By Performance Day ---\n";
    cout << left << setw(20) << "Day & Time" << setw(8) << "Adult" << setw(8) << "Senior" 
         << setw(9) << "Student" << setw(10) << "Acc. Tix" << "Total Sales\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < 4; i++) {
        cout << left << setw(20) << categories[i] << setw(8) << tAdult[i] << setw(8) << tSenior[i] 
             << setw(9) << tStudent[i] << setw(10) << tAccess[i] 
             << "$" << fixed << setprecision(2) << tSales[i] << "\n";
    }
}

void summaryByTitle(const vector<string>& productionTitles,
                    const vector<char>& performanceDays, const vector<char>& performanceTimes,
                    const vector<int>& adultCounts, const vector<int>& seniorCounts,
                    const vector<int>& studentCounts,
                    const vector<char>& accessibilities, const vector<int>& accessibilityCounts,
                    const vector<string>& contactDetails) {
    
    // Arrays for accumulation (Indices: 0=Curtains, 1=Oklahoma, 2=Seussical, 3=Urinetown, 4=Other)
    int tAdult[5] = {0}, tSenior[5] = {0}, tStudent[5] = {0}, tAccess[5] = {0};
    double tSales[5] = {0.0};
    string categories[] = {"Curtains", "Oklahoma", "Seussical", "Urinetown", "Other"};

    for (size_t i = 0; i < productionTitles.size(); i++) {
        int idx = 4; // Default to Other
        if (productionTitles[i] == "Curtains") idx = 0;
        else if (productionTitles[i] == "Oklahoma") idx = 1;
        else if (productionTitles[i] == "Seussical") idx = 2;
        else if (productionTitles[i] == "Urinetown") idx = 3;

        tAdult[idx] += adultCounts[i];
        tSenior[idx] += seniorCounts[i];
        tStudent[idx] += studentCounts[i];
        tAccess[idx] += accessibilityCounts[i];
        tSales[idx] += calculateCost(performanceTimes[i], adultCounts[i], seniorCounts[i], studentCounts[i]);
    }

    cout << "\n--- Summary By Production Title ---\n";
    cout << left << setw(15) << "Production" << setw(8) << "Adult" << setw(8) << "Senior" 
         << setw(9) << "Student" << setw(10) << "Acc. Tix" << "Total Sales\n";
    cout << string(65, '-') << "\n";

    for (int i = 0; i < 5; i++) {
        cout << left << setw(15) << categories[i] << setw(8) << tAdult[i] << setw(8) << tSenior[i] 
             << setw(9) << tStudent[i] << setw(10) << tAccess[i] 
             << "$" << fixed << setprecision(2) << tSales[i] << "\n";
    }
}

// Helper Function: Calculates the cost of a single valid order
double calculateCost(char time, int adult, int senior, int student) {
    double ticketTotal = 0.0;
    
    // Apply base prices based on Matinee (M) or Evening (E)
    if (time == 'E') {
        ticketTotal += (adult * ADULT_EVE_PRICE) + (senior * SENIOR_EVE_PRICE) + (student * STUDENT_EVE_PRICE);
    } else {
        ticketTotal += (adult * ADULT_MAT_PRICE) + (senior * SENIOR_MAT_PRICE) + (student * STUDENT_MAT_PRICE);
    }

    int totalTickets = adult + senior + student;
    
    // Apply group discount if applicable
    if (totalTickets > GROUP_DISCOUNT_THRESHOLD) {
        ticketTotal -= (ticketTotal * GROUP_DISCOUNT_RATE);
    }

    // Processing fee is strictly flat per ticket and not discounted
    ticketTotal += (totalTickets * PROCESSING_FEE);

    return ticketTotal;
}
