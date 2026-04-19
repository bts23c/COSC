/*
 * bts67P1.cpp
 *
 * COSC 1020
 * Project #1 Code
 *
 * Due on: 2/6/2026
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
#include <iomanip>
#include <string>

using namespace std;

int main() {
    // CONSTANTS
    const double PRICE_ADULT_EVE = 34.99;
    const double PRICE_SENIOR_EVE = 30.99;
    const double PRICE_STUDENT_EVE = 24.99;
    
    const double PRICE_ADULT_MAT = 25.97;
    const double PRICE_SENIOR_MAT = 22.97;
    const double PRICE_STUDENT_MAT = 18.97;
    
    const int GROUP_DISCOUNT_THRESHOLD = 10;
    const double GROUP_DISCOUNT_RATE = 0.075;
    const double PROCESSING_FEE_PER_TICKET = 0.50;
    const int ACCESSIBLE_RATIO_LIMIT = 2;

    // VARIABLES
    char dayInput;
    char timeInput = ' '; 
    int numAdult;
    int numSenior;
    int numStudent;
    char isAccessibleInput;
    int numAccessible = 0;
    int totalTickets;
    double priceAdult;
    double priceSenior;
    double priceStudent;
    double grossCost;
    double discountAmount;
    double netCost;
    double processingFee;
    double totalCost;
    
    string tempInput; 
    string dayWord;
    string timeWord;

    // MAIN PROGRAM
    cout << "Welcome to the MCMT automated ticket purchase system!" << endl;
    cout << "Please enter the day you would like to attend (F, S, or U):" << endl;
    
    cin >> tempInput; 
    dayInput = tempInput[0]; 

    // Validate Day Input
    if (dayInput != 'F' && dayInput != 'f' && dayInput != 'S' && dayInput != 's' && dayInput != 'U' && dayInput != 'u') {
        cout << "Error: Invalid day selected." << endl;
        return 1;
    }

    // Determine Time and assign full word strings
    if (dayInput == 'S' || dayInput == 's') {
        dayWord = "Saturday";
        cout << "Enter time (M for Matinee, E for Evening):" << endl;
        cin >> tempInput;
        timeInput = tempInput[0];

        if (timeInput != 'M' && timeInput != 'm' && timeInput != 'E' && timeInput != 'e') {
            cout << "Error: Invalid time selected." << endl;
            return 1;
        }

        if (timeInput == 'E' || timeInput == 'e') {
            timeWord = "Evening";
        } else {
            timeWord = "Matinee";
        }
    } else {
        // Auto-assign day and time for Friday and Sunday
        if (dayInput == 'F' || dayInput == 'f') {
            dayWord = "Friday";
            timeWord = "Evening";
            timeInput = 'E';
        } else {
            dayWord = "Sunday";
            timeWord = "Matinee";
            timeInput = 'M';
        }
    }

    cout << "Enter numbers of adult, senior, and student tickets (separated by spaces):" << endl;
    cin >> numAdult >> numSenior >> numStudent;

    // Validate Ticket Counts
    if (numAdult < 0 || numSenior < 0 || numStudent < 0) {
        cout << "Error: Invalid ticket number." << endl;
        return 1;
    }

    totalTickets = numAdult + numSenior + numStudent;
    if (totalTickets == 0) {
        cout << "Error: Total tickets cannot be zero." << endl;
        return 1;
    }

    cout << "Do you require accessible seating? (Y/N):" << endl;
    cin >> tempInput;
    isAccessibleInput = tempInput[0];

    // Validate Accessible Seating
    if (isAccessibleInput == 'Y' || isAccessibleInput == 'y') {
        cout << "How many accessible seats are requested?" << endl;
        cin >> numAccessible;

        if (numAccessible < 0) {
            cout << "Error: Invalid number." << endl;
            return 1;
        }

        if (numAccessible > totalTickets / ACCESSIBLE_RATIO_LIMIT) {
            cout << "Error: Accessible seats cannot exceed 50% of order." << endl;
            return 1;
        }
    } else {
        if (isAccessibleInput == 'N' || isAccessibleInput == 'n') {
            numAccessible = 0;
        } else {
            cout << "Error: Invalid response." << endl;
            return 1;
        }
    }

    // Assign Prices using the new timeWord variable
    if (timeWord == "Evening") {
        priceAdult = PRICE_ADULT_EVE;
        priceSenior = PRICE_SENIOR_EVE;
        priceStudent = PRICE_STUDENT_EVE;
    } else {
        priceAdult = PRICE_ADULT_MAT;
        priceSenior = PRICE_SENIOR_MAT;
        priceStudent = PRICE_STUDENT_MAT;
    }

    grossCost = (numAdult * priceAdult) + (numSenior * priceSenior) + (numStudent * priceStudent);

    // Calculate Discount
    if (totalTickets > GROUP_DISCOUNT_THRESHOLD) {
        discountAmount = grossCost * GROUP_DISCOUNT_RATE;
    } else {
        discountAmount = 0.0;
    }

    netCost = grossCost - discountAmount;

    // Calculate Fees and Final Cost
    processingFee = totalTickets * PROCESSING_FEE_PER_TICKET;
    totalCost = netCost + processingFee;

    // Output Final Receipt
    cout << "**************************" << endl;
    cout << "Your order details:" << endl;
    cout << "***************" << endl;
    cout << "Performance Day: " << dayWord << endl;
    cout << "Performance Time: " << timeWord << endl;
    cout << "Adult Tickets: " << numAdult << endl;
    cout << "Senior Tickets: " << numSenior << endl;
    cout << "Student Tickets: " << numStudent << endl;
    cout << "Total Tickets: " << totalTickets << endl;
    cout << "Accessible Seats: " << numAccessible << endl;
    
    // Formatting the receipt numbers to match the assignment criteria
    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Ticket Price:" << "$" << right << setw(8) << grossCost << endl;
    cout << left << setw(20) << "Group Discount:" << "$" << right << setw(8) << discountAmount << endl;
    cout << left << setw(20) << "Net Ticket Price:" << "$" << right << setw(8) << netCost << endl;
    cout << left << setw(20) << "Processing Fees:" << "$" << right << setw(8) << processingFee << endl;
    cout << left << setw(20) << "Final Cost:" << "$" << right << setw(8) << totalCost << endl;

    cout << "Thank you for supporting MCMT – See you at the Theatre!" << endl;

    return 0;
}
