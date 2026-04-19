/*
 * bts67P4.cpp
 *
 * COSC 1020
 * Project #4 Code
 *
 * Due on: April 8
 * Author: bts67
 *
 * In accordance with class policies and Georgetown's Honor Code,
 * I certify that, with the exception of the class resources and those
 * items noted below, I have neither given nor received any assistance
 * on this project.
 *
 * Note that you may use without citation any help from our TAs,
 * professors, or any code taken from the course textbook.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

// I am defining my TicketOrder class to encapsulate all data and behaviors of a single order.
class TicketOrder {
    friend ostream& operator<<(ostream &os, const TicketOrder &rhs);
    friend istream& operator>>(istream &in, TicketOrder &rhs);
private:
    string productionTitle;
    char performanceDay, performanceTime;
    int adultCount, seniorCount, studentCount;
    char accessibility;
    int accessibleCount;
    string contactDetails;
public:
    TicketOrder();
    TicketOrder(string productionTitle, char performanceDay, char performanceTime,
                int adultCount, int seniorCount, int studentCount,
                char accessibility, int accessibleCount, string contactDetails);
    TicketOrder(const TicketOrder& otherOrder);
    TicketOrder& operator=(const TicketOrder& otherOrder);
    
    // Member functions to calculate costs using my P2/P3 rules
    double getTicketCost() const;
    double getDiscount() const;
    double getProcessingFee() const;
    double getTotalCost() const;
    
    // Getters and Setters
    void setProductionTitle(string title) { productionTitle = title; }
    string getProductionTitle() const { return productionTitle; }
    
    char getPerformanceDay() const { return performanceDay; }
    char getPerformanceTime() const { return performanceTime; }
    
    string getPerformanceDayName() const;
    string getPerformanceTimeName() const;
};

// Global function prototypes to drive the menu options
char displayMenu();
bool uploadFile(string filename, vector<TicketOrder>& ticketOrders);
void allDetails(const vector<TicketOrder>& ticketOrders);
void summaryByDay(const vector<TicketOrder>& ticketOrders);
void summaryByTitle(const vector<TicketOrder>& ticketOrders);

int main() {
    // I am using a single vector to replace the parallel vectors from Project 3
    vector<TicketOrder> ticketOrders;
    char menuChoice;
    
    do {
        menuChoice = displayMenu();
        
        switch(tolower(menuChoice)) {
            case 'u': {
                string filename;
                cout << "Enter the ticket data filename: ";
                cin >> filename;
                if (!uploadFile(filename, ticketOrders)) {
                    cout << "Error: Could not open the file." << endl;
                }
                break;
            }
            case 'a':
                if (ticketOrders.empty()) {
                    cout << "Error: No data currently loaded. Please upload a file first." << endl;
                } else {
                    allDetails(ticketOrders);
                }
                break;
            case 'd':
                if (ticketOrders.empty()) {
                    cout << "Error: No data currently loaded." << endl;
                } else {
                    summaryByDay(ticketOrders);
                }
                break;
            case 't':
                if (ticketOrders.empty()) {
                    cout << "Error: No data currently loaded." << endl;
                } else {
                    summaryByTitle(ticketOrders);
                }
                break;
            case 'c':
                ticketOrders.clear();
                cout << "All loaded data has been cleared." << endl;
                break;
            case 'q':
                cout << "Exiting the program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid selection. Please try again." << endl;
        }
        cout << endl;
    } while (tolower(menuChoice) != 'q');
    
    return 0;
}

// ---------------------------------------------------------
// TicketOrder Class Implementations
// ---------------------------------------------------------

// My default constructor initializes an invalid order by design.
TicketOrder::TicketOrder() {
    productionTitle = "";
    performanceDay = 'f';
    performanceTime = 'e';
    adultCount = 0;
    seniorCount = 0;
    studentCount = 0;
    accessibility = 'n';
    accessibleCount = 0;
    contactDetails = "";
}

// My parameterized constructor sets all member variables based on provided inputs.
TicketOrder::TicketOrder(string title, char pDay, char pTime, int aCount, int senCount, 
                         int stuCount, char access, int accessCount, string contact) {
    productionTitle = title;
    performanceDay = pDay;
    performanceTime = pTime;
    adultCount = aCount;
    seniorCount = senCount;
    studentCount = stuCount;
    accessibility = access;
    accessibleCount = accessCount;
    contactDetails = contact;
}

// My copy constructor copies data from another TicketOrder object.
TicketOrder::TicketOrder(const TicketOrder& otherOrder) {
    productionTitle = otherOrder.productionTitle;
    performanceDay = otherOrder.performanceDay;
    performanceTime = otherOrder.performanceTime;
    adultCount = otherOrder.adultCount;
    seniorCount = otherOrder.seniorCount;
    studentCount = otherOrder.studentCount;
    accessibility = otherOrder.accessibility;
    accessibleCount = otherOrder.accessibleCount;
    contactDetails = otherOrder.contactDetails;
}

// My assignment operator safely assigns state and returns a reference to itself.
TicketOrder& TicketOrder::operator=(const TicketOrder& otherOrder) {
    if (this != &otherOrder) {
        productionTitle = otherOrder.productionTitle;
        performanceDay = otherOrder.performanceDay;
        performanceTime = otherOrder.performanceTime;
        adultCount = otherOrder.adultCount;
        seniorCount = otherOrder.seniorCount;
        studentCount = otherOrder.studentCount;
        accessibility = otherOrder.accessibility;
        accessibleCount = otherOrder.accessibleCount;
        contactDetails = otherOrder.contactDetails;
    }
    return *this;
}

// ---------------------------------------------------------
// Cost Calculations (Integrated from P2/P3 Rules)
// ---------------------------------------------------------

double TicketOrder::getTicketCost() const {
    double cost = 0.0;
    char lTime = tolower(performanceTime);
    
    // I am applying base prices based on Evening (e) or Matinee (m)
    if (lTime == 'e') {
        cost = (adultCount * 34.99) + (seniorCount * 30.99) + (studentCount * 24.99);
    } else {
        cost = (adultCount * 25.97) + (seniorCount * 22.97) + (studentCount * 18.97);
    }
    
    return cost;
}

double TicketOrder::getDiscount() const {
    int totalTickets = adultCount + seniorCount + studentCount;
    
    // I am applying a 7.5% group discount if the order has more than 10 tickets
    if (totalTickets > 10) {
        return getTicketCost() * 0.075;
    }
    
    return 0.0;
}

double TicketOrder::getProcessingFee() const {
    int totalTickets = adultCount + seniorCount + studentCount;
    
    // The processing fee is a flat $0.50 per ticket
    return totalTickets * 0.50;
}

// I use my other member functions to compute the final combined total.
double TicketOrder::getTotalCost() const {
    return getTicketCost() - getDiscount() + getProcessingFee();
}

string TicketOrder::getPerformanceDayName() const {
    char lowerDay = tolower(performanceDay);
    if (lowerDay == 'f') return "Friday";
    if (lowerDay == 's') return "Saturday";
    if (lowerDay == 'u') return "Sunday";
    return "Unknown Day";
}

string TicketOrder::getPerformanceTimeName() const {
    char lowerTime = tolower(performanceTime);
    if (lowerTime == 'm') return "Matinee";
    if (lowerTime == 'e') return "Evening";
    return "Unknown Time";
}

// I am overloading the extraction operator to easily read a file line into my object.
istream& operator>>(istream &in, TicketOrder &rhs) {
    in >> rhs.performanceDay >> rhs.performanceTime >> rhs.adultCount 
       >> rhs.seniorCount >> rhs.studentCount >> rhs.accessibility >> rhs.accessibleCount;
    // Clearing the whitespace before reading the rest of the line as contact details
    getline(in >> ws, rhs.contactDetails);
    return in;
}

// I am overloading the insertion operator to neatly print my order details.
ostream& operator<<(ostream &os, const TicketOrder &rhs) {
    os << left << setw(15) << rhs.productionTitle 
       << setw(10) << rhs.getPerformanceDayName() 
       << setw(10) << rhs.getPerformanceTimeName()
       << "Tickets: A:" << rhs.adultCount << " S:" << rhs.seniorCount << " St:" << rhs.studentCount
       << right << setw(10) << " Total: $" << fixed << setprecision(2) << rhs.getTotalCost()
       << "  | " << rhs.contactDetails;
    return os;
}


// ---------------------------------------------------------
// Global Functions Implementations
// ---------------------------------------------------------

char displayMenu() {
    char choice;
    cout << "-------------------------------------------" << endl;
    cout << "(U) Upload a production ticket data file" << endl;
    cout << "(A) Display All loaded data details" << endl;
    cout << "(D) Display a summary by performance Day and time" << endl;
    cout << "(T) Display a summary by performance Title" << endl;
    cout << "(C) Clear all loaded data" << endl;
    cout << "(Q) Quit the program" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

bool uploadFile(string filename, vector<TicketOrder>& ticketOrders) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        return false;
    }

    // Determining the production title based on the filename string.
    string title = "Other";
    // I am checking for both capitalized and lowercase versions just to be safe
    if (filename.find("Curtains") != string::npos || filename.find("curtains") != string::npos) {
        title = "Curtains";
    } else if (filename.find("Oklahoma") != string::npos || filename.find("oklahoma") != string::npos) {
        title = "Oklahoma";
    } else if (filename.find("Seussical") != string::npos || filename.find("seussical") != string::npos) {
        title = "Seussical";
    } else if (filename.find("Urinetown") != string::npos || filename.find("urinetown") != string::npos) {
        title = "Urinetown";
    }

    string headerLine;
    // I am skipping the first line containing the column headers
    getline(ifs, headerLine); 

    TicketOrder nextOrder;
    // Reading data until EOF. I'm relying on the overloaded >> operator to handle extraction.
    while (ifs >> nextOrder) {
        nextOrder.setProductionTitle(title);
        ticketOrders.push_back(nextOrder);
    }

    ifs.close();
    cout << "File uploaded and processed successfully." << endl;
    return true;
}

void allDetails(const vector<TicketOrder>& ticketOrders) {
    cout << "\n--- All Ticket Order Details ---" << endl;
    // I iterate through my vector and use the overloaded << operator to print each object.
    for (size_t i = 0; i < ticketOrders.size(); i++) {
        cout << ticketOrders[i] << endl;
    }
}

void summaryByDay(const vector<TicketOrder>& ticketOrders) {
    double fridayTotal = 0, saturdayTotal = 0, sundayTotal = 0;
    
    // I extract the total cost for each order and accumulate it by performance day.
    for (size_t i = 0; i < ticketOrders.size(); i++) {
        char day = tolower(ticketOrders[i].getPerformanceDay());
        double cost = ticketOrders[i].getTotalCost();
        
        if (day == 'f') fridayTotal += cost;
        else if (day == 's') saturdayTotal += cost;
        else if (day == 'u') sundayTotal += cost;
    }

    cout << "\n--- Summary By Performance Day ---" << endl;
    cout << left << setw(15) << "Friday:" << "$" << fixed << setprecision(2) << fridayTotal << endl;
    cout << left << setw(15) << "Saturday:" << "$" << fixed << setprecision(2) << saturdayTotal << endl;
    cout << left << setw(15) << "Sunday:" << "$" << fixed << setprecision(2) << sundayTotal << endl;
}

void summaryByTitle(const vector<TicketOrder>& ticketOrders) {
    double curtainsTotal = 0, oklahomaTotal = 0, seussicalTotal = 0, urinetownTotal = 0, otherTotal = 0;
    
    // I extract the total cost and distribute it based on the production title.
    for (size_t i = 0; i < ticketOrders.size(); i++) {
        string title = ticketOrders[i].getProductionTitle();
        double cost = ticketOrders[i].getTotalCost();
        
        if (title == "Curtains") curtainsTotal += cost;
        else if (title == "Oklahoma") oklahomaTotal += cost;
        else if (title == "Seussical") seussicalTotal += cost;
        else if (title == "Urinetown") urinetownTotal += cost;
        else otherTotal += cost;
    }

    cout << "\n--- Summary By Performance Title ---" << endl;
    cout << left << setw(15) << "Curtains:" << "$" << fixed << setprecision(2) << curtainsTotal << endl;
    cout << left << setw(15) << "Oklahoma:" << "$" << fixed << setprecision(2) << oklahomaTotal << endl;
    cout << left << setw(15) << "Seussical:" << "$" << fixed << setprecision(2) << seussicalTotal << endl;
    cout << left << setw(15) << "Urinetown:" << "$" << fixed << setprecision(2) << urinetownTotal << endl;
    cout << left << setw(15) << "Other:" << "$" << fixed << setprecision(2) << otherTotal << endl;
}
