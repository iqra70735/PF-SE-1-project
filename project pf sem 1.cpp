#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

void setColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void clearScreen() {
    system("cls");
}

void showHeader(string title) {
    setColor(3);
    cout << "\n" << string(60, '=') << endl;
    cout << "\t" << title << endl;
    cout << string(60, '=') << endl;
    setColor(7);
}

void pauseScreen() {
    cout << "\nPress any key to continue...";
    _getch();
}

struct Event {
    string name;
    string date;
    string location;
    string type;
    int totalSeats;
    int bookedSeats;
};

struct Guest {
    string name;
    string email;
    string cnic;
    string phone;
    string eventName;
};

struct Staff {
    string name;
    string email;
    string cnic;
    string role;
    string phone;
    string assignedEvent;
};

Event eventList[50];
Guest guestList[50];
Staff staffList[50];

int totalEvents = 0;
int totalGuests = 0;
int totalStaff = 0;

bool validateCNIC(string id) {
    if (id.length() != 13) return false;
    for (char c : id) if (!isdigit(c)) return false;
    return true;
}

bool validatePhone(string number) {
    if (number.length() != 11) return false;
    for (char c : number) if (!isdigit(c)) return false;
    return true;
}

bool validateEmail(string email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool validateDate(string date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }
    return true;
}

void displayEventDetails(const Event& e) {
    int available = e.totalSeats - e.bookedSeats;
    cout << left << setw(25) << e.name << setw(12) << e.date 
         << setw(20) << e.location << setw(15) << e.type
         << setw(10) << e.totalSeats << setw(15) << available << endl;
}

void displayGuestDetails(const Guest& g) {
    cout << left << setw(20) << g.name << setw(20) << g.email 
         << setw(18) << g.cnic << setw(15) << g.phone
         << setw(20) << g.eventName << endl;
}

void displayStaffDetails(const Staff& s) {
    cout << left << setw(20) << s.name << setw(20) << s.role 
         << setw(15) << s.phone << setw(25) << s.assignedEvent << endl;
}

void createNewEvent() {
    clearScreen();
    showHeader("ADD NEW EVENT");
    cin.ignore();
    
    Event newEvent;
    
    cout << "\nEvent Name: ";
    getline(cin, newEvent.name);
    
    while (true) {
        cout << "\nDate (DD/MM/YYYY): ";
        getline(cin, newEvent.date);
        if (validateDate(newEvent.date)) break;
        setColor(12);
        cout << "Invalid date format! Use DD/MM/YYYY.\n";
        setColor(7);
    }
    
    cout << "\nLocation: ";
    getline(cin, newEvent.location);
    
    cout << "\nEvent Type: ";
    getline(cin, newEvent.type);
    
    cout << "\nTotal Seats: ";
    while (!(cin >> newEvent.totalSeats) || newEvent.totalSeats <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter positive number: ";
    }
    
    newEvent.bookedSeats = 0;
    eventList[totalEvents++] = newEvent;
    
    setColor(10);
    cout << "\nEvent added!\n";
    setColor(7);
    pauseScreen();
}

void showAllEvents() {
    clearScreen();
    showHeader("EVENT LIST");
    
    if (totalEvents == 0) {
        setColor(12);
        cout << "\nNo events found!\n";
        setColor(7);
    } else {
        setColor(11);
        cout << left << setw(25) << "EVENT" << setw(12) << "DATE" << setw(20) << "LOCATION" 
             << setw(15) << "TYPE" << setw(10) << "SEATS" << setw(15) << "AVAILABLE" << endl;
        setColor(7);
        cout << string(97, '-') << endl;
        
        for (int i = 0; i < totalEvents; i++) {
            displayEventDetails(eventList[i]);
        }
    }
    pauseScreen();
}

void modifyEvent() {
    clearScreen();
    showHeader("UPDATE EVENT");
    
    if (totalEvents == 0) {
        setColor(12);
        cout << "\nNo events to update!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllEvents();
    
    int choice;
    cout << "\nSelect event to update (1-" << totalEvents << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalEvents) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    Event& e = eventList[choice-1];
    cin.ignore();
    
    cout << "\nCurrent Name: " << e.name;
    cout << "\nNew Name (enter to keep): ";
    string input;
    getline(cin, input);
    if (!input.empty()) e.name = input;
    
    while (true) {
        cout << "\nCurrent Date: " << e.date;
        cout << "\nNew Date (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validateDate(input)) {
            e.date = input;
            break;
        }
        setColor(12);
        cout << "Invalid date format!\n";
        setColor(7);
    }
    
    cout << "\nCurrent Location: " << e.location;
    cout << "\nNew Location (enter to keep): ";
    getline(cin, input);
    if (!input.empty()) e.location = input;
    
    cout << "\nCurrent Type: " << e.type;
    cout << "\nNew Type (enter to keep): ";
    getline(cin, input);
    if (!input.empty()) e.type = input;
    
    cout << "\nCurrent Seats: " << e.totalSeats;
    cout << "\nNew Seats (if you dont want to update give same digits again): ";
    int seats;
    while (!(cin >> seats) || seats < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter valid number: ";
    }
    cin.ignore();
    
    if (seats > 0) {
        if (seats >= e.bookedSeats) {
            e.totalSeats = seats;
        } else {
            setColor(12);
            cout << "Can't reduce below booked seats (" << e.bookedSeats << ")\n";
            setColor(7);
        }
    }
    
    setColor(10);
    cout << "\nEvent updated!\n";
    setColor(7);
    pauseScreen();
}

void removeEvent() {
    clearScreen();
    showHeader("REMOVE EVENT");
    
    if (totalEvents == 0) {
        setColor(12);
        cout << "\nNo events to remove!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllEvents();
    
    int choice;
    cout << "\nSelect event to remove (1-" << totalEvents << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalEvents) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cout << "\nConfirm deletion? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (toupper(confirm) != 'Y') {
        cout << "\nCancelled.\n";
        pauseScreen();
        return;
    }
    
    for (int i = choice-1; i < totalEvents-1; i++) {
        eventList[i] = eventList[i+1];
    }
    totalEvents--;
    
    setColor(10);
    cout << "\nEvent removed!\n";
    setColor(7);
    pauseScreen();
}

void registerNewGuest() {
    clearScreen();
    showHeader("GUEST REGISTRATION");
    
    if (totalEvents == 0) {
        setColor(12);
        cout << "\nNo events available!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cout << "\nAvailable Events:\n";
    for (int i = 0; i < totalEvents; i++) {
        int available = eventList[i].totalSeats - eventList[i].bookedSeats;
        cout << i+1 << ". " << eventList[i].name << " (" << available << " seats left)\n";
    }
    
    int choice;
    cout << "\nSelect event (1-" << totalEvents << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalEvents) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    if (eventList[choice-1].bookedSeats >= eventList[choice-1].totalSeats) {
        setColor(12);
        cout << "\nEvent is full!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cin.ignore();
    Guest newGuest;
    newGuest.eventName = eventList[choice-1].name;
    
    cout << "\nYour Name: ";
    getline(cin, newGuest.name);
    
    while (true) {
        cout << "\nYour Email: ";
        getline(cin, newGuest.email);
        if (validateEmail(newGuest.email)) break;
        setColor(12);
        cout << "Invalid email!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nYour CNIC (13 digits): ";
        getline(cin, newGuest.cnic);
        if (validateCNIC(newGuest.cnic)) break;
        setColor(12);
        cout << "Invalid CNIC!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nYour Phone (11 digits): ";
        getline(cin, newGuest.phone);
        if (validatePhone(newGuest.phone)) break;
        setColor(12);
        cout << "Invalid phone!\n";
        setColor(7);
    }
    
    guestList[totalGuests++] = newGuest;
    eventList[choice-1].bookedSeats++;
    
    setColor(10);
    cout << "\nRegistration successful!\n";
    setColor(7);
    pauseScreen();
}

void showAllGuests() {
    clearScreen();
    showHeader("GUEST LIST");
    
    if (totalGuests == 0) {
        setColor(12);
        cout << "\nNo guests found!\n";
        setColor(7);
    } else {
        setColor(11);
        cout << left << setw(20) << "NAME" << setw(20) << "EMAIL" << setw(18) << "CNIC"
             << setw(15) << "PHONE" << setw(20) << "EVENT" << endl;
        setColor(7);
        cout << string(93, '-') << endl;
        
        for (int i = 0; i < totalGuests; i++) {
            displayGuestDetails(guestList[i]);
        }
    }
    pauseScreen();
}

void updateGuestInfo() {
    clearScreen();
    showHeader("UPDATE GUEST");
    
    if (totalGuests == 0) {
        setColor(12);
        cout << "\nNo guests to update!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllGuests();
    
    int choice;
    cout << "\nSelect guest (1-" << totalGuests << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalGuests) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    Guest& g = guestList[choice-1];
    cin.ignore();
    
    cout << "\nCurrent Name: " << g.name;
    cout << "\nNew Name (enter to keep): ";
    string input;
    getline(cin, input);
    if (!input.empty()) g.name = input;
    
    while (true) {
        cout << "\nCurrent Email: " << g.email;
        cout << "\nNew Email (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validateEmail(input)) {
            g.email = input;
            break;
        }
        setColor(12);
        cout << "Invalid email!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nCurrent CNIC: " << g.cnic;
        cout << "\nNew CNIC (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validateCNIC(input)) {
            g.cnic = input;
            break;
        }
        setColor(12);
        cout << "Invalid CNIC!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nCurrent Phone: " << g.phone;
        cout << "\nNew Phone (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validatePhone(input)) {
            g.phone = input;
            break;
        }
        setColor(12);
        cout << "Invalid phone!\n";
        setColor(7);
    }
    
    setColor(10);
    cout << "\nGuest updated!\n";
    setColor(7);
    pauseScreen();
}

void removeGuest() {
    clearScreen();
    showHeader("REMOVE GUEST");
    
    if (totalGuests == 0) {
        setColor(12);
        cout << "\nNo guests to remove!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllGuests();
    
    int choice;
    cout << "\nSelect guest (1-" << totalGuests << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalGuests) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    string eventName = guestList[choice-1].eventName;
    for (int i = 0; i < totalEvents; i++) {
        if (eventList[i].name == eventName) {
            eventList[i].bookedSeats--;
            break;
        }
    }
    
    cout << "\nConfirm deletion? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (toupper(confirm) != 'Y') {
        cout << "\nCancelled.\n";
        pauseScreen();
        return;
    }
    
    for (int i = choice-1; i < totalGuests-1; i++) {
        guestList[i] = guestList[i+1];
    }
    totalGuests--;
    
    setColor(10);
    cout << "\nGuest removed!\n";
    setColor(7);
    pauseScreen();
}

void addNewStaff() {
    clearScreen();
    showHeader("ADD STAFF");
    cin.ignore();
    
    Staff newStaff;
    
    cout << "\nFull Name: ";
    getline(cin, newStaff.name);
    
    while (true) {
        cout << "\nEmail: ";
        getline(cin, newStaff.email);
        if (validateEmail(newStaff.email)) break;
        setColor(12);
        cout << "Invalid email!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nCNIC (13 digits): ";
        getline(cin, newStaff.cnic);
        if (validateCNIC(newStaff.cnic)) break;
        setColor(12);
        cout << "Invalid CNIC!\n";
        setColor(7);
    }
    
    cout << "\nRole: ";
    getline(cin, newStaff.role);
    
    while (true) {
        cout << "\nPhone (11 digits): ";
        getline(cin, newStaff.phone);
        if (validatePhone(newStaff.phone)) break;
        setColor(12);
        cout << "Invalid phone!\n";
        setColor(7);
    }
    
    newStaff.assignedEvent = "None";
    staffList[totalStaff++] = newStaff;
    
    setColor(10);
    cout << "\nStaff added!\n";
    setColor(7);
    pauseScreen();
}

void showAllStaff() {
    clearScreen();
    showHeader("STAFF LIST");
    
    if (totalStaff == 0) {
        setColor(12);
        cout << "\nNo staff found!\n";
        setColor(7);
    } else {
        setColor(11);
        cout << left << setw(20) << "NAME" << setw(20) << "ROLE" << setw(15) << "PHONE"
             << setw(25) << "ASSIGNED EVENT" << endl;
        setColor(7);
        cout << string(80, '-') << endl;
        
        for (int i = 0; i < totalStaff; i++) {
            displayStaffDetails(staffList[i]);
        }
    }
    pauseScreen();
}

void updateStaffInfo() {
    clearScreen();
    showHeader("UPDATE STAFF");
    
    if (totalStaff == 0) {
        setColor(12);
        cout << "\nNo staff to update!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllStaff();
    
    int choice;
    cout << "\nSelect staff (1-" << totalStaff << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalStaff) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    Staff& s = staffList[choice-1];
    cin.ignore();
    
    cout << "\nCurrent Name: " << s.name;
    cout << "\nNew Name (enter to keep): ";
    string input;
    getline(cin, input);
    if (!input.empty()) s.name = input;
    
    while (true) {
        cout << "\nCurrent Email: " << s.email;
        cout << "\nNew Email (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validateEmail(input)) {
            s.email = input;
            break;
        }
        setColor(12);
        cout << "Invalid email!\n";
        setColor(7);
    }
    
    while (true) {
        cout << "\nCurrent CNIC: " << s.cnic;
        cout << "\nNew CNIC (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validateCNIC(input)) {
            s.cnic = input;
            break;
        }
        setColor(12);
        cout << "Invalid CNIC!\n";
        setColor(7);
    }
    
    cout << "\nCurrent Role: " << s.role;
    cout << "\nNew Role (enter to keep): ";
    getline(cin, input);
    if (!input.empty()) s.role = input;
    
    while (true) {
        cout << "\nCurrent Phone: " << s.phone;
        cout << "\nNew Phone (enter to keep): ";
        getline(cin, input);
        if (input.empty()) break;
        if (validatePhone(input)) {
            s.phone = input;
            break;
        }
        setColor(12);
        cout << "Invalid phone!\n";
        setColor(7);
    }
    
    setColor(10);
    cout << "\nStaff updated!\n";
    setColor(7);
    pauseScreen();
}

void assignStaff() {
    clearScreen();
    showHeader("ASSIGN STAFF");
    
    if (totalStaff == 0 || totalEvents == 0) {
        setColor(12);
        if (totalStaff == 0) cout << "\nNo staff available!\n";
        if (totalEvents == 0) cout << "\nNo events available!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cout << "\nStaff Members:\n";
    for (int i = 0; i < totalStaff; i++) {
        cout << i+1 << ". " << staffList[i].name << " (" << staffList[i].role << ")\n";
    }
    
    int staffChoice;
    cout << "\nSelect staff (1-" << totalStaff << " or 0 to cancel): ";
    cin >> staffChoice;
    
    if (staffChoice == 0) return;
    if (staffChoice < 1 || staffChoice > totalStaff) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cout << "\nAvailable Events:\n";
    for (int i = 0; i < totalEvents; i++) {
        cout << i+1 << ". " << eventList[i].name << endl;
    }
    
    int eventChoice;
    cout << "\nSelect event (1-" << totalEvents << " or 0 to cancel): ";
    cin >> eventChoice;
    
    if (eventChoice == 0) return;
    if (eventChoice < 1 || eventChoice > totalEvents) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    staffList[staffChoice-1].assignedEvent = eventList[eventChoice-1].name;
    
    setColor(10);
    cout << "\nAssignment complete!\n";
    setColor(7);
    pauseScreen();
}

void removeStaff() {
    clearScreen();
    showHeader("REMOVE STAFF");
    
    if (totalStaff == 0) {
        setColor(12);
        cout << "\nNo staff to remove!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    showAllStaff();
    
    int choice;
    cout << "\nSelect staff (1-" << totalStaff << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalStaff) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    cout << "\nConfirm deletion? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (toupper(confirm) != 'Y') {
        cout << "\nCancelled.\n";
        pauseScreen();
        return;
    }
    
    for (int i = choice-1; i < totalStaff-1; i++) {
        staffList[i] = staffList[i+1];
    }
    totalStaff--;
    
    setColor(10);
    cout << "\nStaff removed!\n";
    setColor(7);
    pauseScreen();
}

void adminPanel() {
    string password;
    clearScreen();
    showHeader("ADMIN LOGIN");
    
    cout << "\nEnter password: ";
    cin >> password;
    
    if (password != "admin123") {
        setColor(12);
        cout << "\nAccess denied!\n";
        setColor(7);
        Sleep(1000);
        return;
    }
    
    while (true) {
        clearScreen();
        showHeader("ADMIN DASHBOARD");
        
        cout << "\n1. Manage Events";
        cout << "\n2. Manage Guests";
        cout << "\n3. Manage Staff";
        cout << "\n4. Logout";
        cout << "\n\nEnter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                while (true) {
                    clearScreen();
                    showHeader("EVENT MANAGEMENT");
                    
                    cout << "\n1. Add Event";
                    cout << "\n2. View Events";
                    cout << "\n3. Update Event";
                    cout << "\n4. Remove Event";
                    cout << "\n5. Back";
                    cout << "\n\nEnter choice: ";
                    
                    cin >> choice;
                    
                    switch (choice) {
                        case 1: createNewEvent(); break;
                        case 2: showAllEvents(); break;
                        case 3: modifyEvent(); break;
                        case 4: removeEvent(); break;
                        case 5: break;
                        default:
                            setColor(12);
                            cout << "\nInvalid option!\n";
                            setColor(7);
                            pauseScreen();
                    }
                    if (choice == 5) break;
                }
                break;
            }
                
            case 2: {
                while (true) {
                    clearScreen();
                    showHeader("GUEST MANAGEMENT");
                    
                    cout << "\n1. View Guests";
                    cout << "\n2. Update Guest";
                    cout << "\n3. Remove Guest";
                    cout << "\n4. Back";
                    cout << "\n\nEnter choice: ";
                    
                    cin >> choice;
                    
                    switch (choice) {
                        case 1: showAllGuests(); break;
                        case 2: updateGuestInfo(); break;
                        case 3: removeGuest(); break;
                        case 4: break;
                        default:
                            setColor(12);
                            cout << "\nInvalid option!\n";
                            setColor(7);
                            pauseScreen();
                    }
                    if (choice == 4) break;
                }
                break;
            }
                
            case 3: {
                while (true) {
                    clearScreen();
                    showHeader("STAFF MANAGEMENT");
                    
                    cout << "\n1. Add Staff";
                    cout << "\n2. View Staff";
                    cout << "\n3. Update Staff";
                    cout << "\n4. Assign Staff";
                    cout << "\n5. Remove Staff";
                    cout << "\n6. Back";
                    cout << "\n\nEnter choice: ";
                    
                    cin >> choice;
                    
                    switch (choice) {
                        case 1: addNewStaff(); break;
                        case 2: showAllStaff(); break;
                        case 3: updateStaffInfo(); break;
                        case 4: assignStaff(); break;
                        case 5: removeStaff(); break;
                        case 6: break;
                        default:
                            setColor(12);
                            cout << "\nInvalid option!\n";
                            setColor(7);
                            pauseScreen();
                    }
                    if (choice == 6) break;
                }
                break;
            }
                
            case 4:
                return;
                
            default:
                setColor(12);
                cout << "\nInvalid choice!\n";
                setColor(7);
                pauseScreen();
        }
    }
}

void changeGuestEvent(string email) {
    clearScreen();
    showHeader("CHANGE EVENT REGISTRATION");
    
    int guestIndex = -1;
    for (int i = 0; i < totalGuests; i++) {
        if (guestList[i].email == email) {
            guestIndex = i;
            break;
        }
    }
    
    if (guestIndex == -1) {
        setColor(12);
        cout << "\nNo registration found!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    string currentEvent = guestList[guestIndex].eventName;
    int currentEventIndex = -1;
    
    for (int i = 0; i < totalEvents; i++) {
        if (eventList[i].name == currentEvent) {
            currentEventIndex = i;
            break;
        }
    }
    
    cout << "\nCurrent Event: " << currentEvent << endl;
    cout << "\nAvailable Events:\n";
    for (int i = 0; i < totalEvents; i++) {
        int available = eventList[i].totalSeats - eventList[i].bookedSeats;
        cout << i+1 << ". " << eventList[i].name << " (" << available << " seats left)\n";
    }
    
    int choice;
    cout << "\nSelect new event (1-" << totalEvents << " or 0 to cancel): ";
    cin >> choice;
    
    if (choice == 0) return;
    if (choice < 1 || choice > totalEvents) {
        setColor(12);
        cout << "\nInvalid choice!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    if (eventList[choice-1].bookedSeats >= eventList[choice-1].totalSeats) {
        setColor(12);
        cout << "\nEvent is full!\n";
        setColor(7);
        pauseScreen();
        return;
    }
    
    if (currentEventIndex != -1) {
        eventList[currentEventIndex].bookedSeats--;
    }
    
    guestList[guestIndex].eventName = eventList[choice-1].name;
    eventList[choice-1].bookedSeats++;
    
    setColor(10);
    cout << "\nEvent changed successfully!\n";
    setColor(7);
    pauseScreen();
}

void guestPortal() {
    while (true) {
        clearScreen();
        showHeader("GUEST PORTAL");
        
        cout << "\n1. Register for Event";
        cout << "\n2. View My Registrations";
        cout << "\n3. Update My Information";
        cout << "\n4. Change My Event";
        cout << "\n5. Cancel Registration";
        cout << "\n6. Back";
        cout << "\n\nEnter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: 
                registerNewGuest();
                break;
                
            case 2: {
                clearScreen();
                showHeader("MY REGISTRATIONS");
                cin.ignore();
                
                string email;
                cout << "\nEnter your email: ";
                getline(cin, email);
                
                bool found = false;
                for (int i = 0; i < totalGuests; i++) {
                    if (guestList[i].email == email) {
                        if (!found) {
                            setColor(11);
                            cout << "\nYour Events:\n";
                            setColor(7);
                            found = true;
                        }
                        cout << "- " << guestList[i].eventName << endl;
                    }
                }
                
                if (!found) {
                    setColor(12);
                    cout << "\nNo registrations found!\n";
                    setColor(7);
                }
                pauseScreen();
                break;
            }
                
            case 3: {
                clearScreen();
                showHeader("UPDATE MY INFORMATION");
                cin.ignore();
                
                string email;
                cout << "\nEnter your email: ";
                getline(cin, email);
                
                bool found = false;
                for (int i = 0; i < totalGuests; i++) {
                    if (guestList[i].email == email) {
                        found = true;
                        Guest& g = guestList[i];
                        
                        cout << "\nCurrent Name: " << g.name;
                        cout << "\nNew Name (enter to keep): ";
                        string input;
                        getline(cin, input);
                        if (!input.empty()) g.name = input;
                        
                        while (true) {
                            cout << "\nCurrent Phone: " << g.phone;
                            cout << "\nNew Phone (enter to keep): ";
                            getline(cin, input);
                            if (input.empty()) break;
                            if (validatePhone(input)) {
                                g.phone = input;
                                break;
                            }
                            setColor(12);
                            cout << "Invalid phone!\n";
                            setColor(7);
                        }
                        
                        setColor(10);
                        cout << "\nInformation updated!\n";
                        setColor(7);
                        break;
                    }
                }
                
                if (!found) {
                    setColor(12);
                    cout << "\nNo registration found!\n";
                    setColor(7);
                }
                pauseScreen();
                break;
            }
                
            case 4: {
                cin.ignore();
                string email;
                cout << "\nEnter your email: ";
                getline(cin, email);
                changeGuestEvent(email);
                break;
            }
                
            case 5: {
                clearScreen();
                showHeader("CANCEL REGISTRATION");
                cin.ignore();
                
                string email;
                cout << "\nEnter your email: ";
                getline(cin, email);
                
                bool found = false;
                for (int i = 0; i < totalGuests; i++) {
                    if (guestList[i].email == email) {
                        found = true;
                        string eventName = guestList[i].eventName;
                        
                        for (int j = 0; j < totalEvents; j++) {
                            if (eventList[j].name == eventName) {
                                eventList[j].bookedSeats--;
                                break;
                            }
                        }
                        
                        for (int j = i; j < totalGuests-1; j++) {
                            guestList[j] = guestList[j+1];
                        }
                        totalGuests--;
                        
                        setColor(10);
                        cout << "\nRegistration cancelled!\n";
                        setColor(7);
                        break;
                    }
                }
                
                if (!found) {
                    setColor(12);
                    cout << "\nNo registration found!\n";
                    setColor(7);
                }
                pauseScreen();
                break;
            }
                
            case 6:
                return;
                
            default:
                setColor(12);
                cout << "\nInvalid option!\n";
                setColor(7);
                pauseScreen();
        }
    }
}

void staffPortal() {
    string password;
    clearScreen();
    showHeader("STAFF LOGIN");
    
    cout << "\nEnter password: ";
    cin >> password;
    
    if (password != "staff123") {
        setColor(12);
        cout << "\nAccess denied!\n";
        setColor(7);
        Sleep(1000);
        return;
    }
    
    while (true) {
        clearScreen();
        showHeader("STAFF DASHBOARD");
        
        cout << "\n1. View My Assignments";
        cout << "\n2. View All Events";
        cout << "\n3. Logout";
        cout << "\n\nEnter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                clearScreen();
                showHeader("MY ASSIGNMENTS");
                cin.ignore();
                
                string cnic;
                while (true) {
                    cout << "\nEnter your CNIC: ";
                    getline(cin, cnic);
                    if (validateCNIC(cnic)) break;
                    setColor(12);
                    cout << "Invalid CNIC!\n";
                    setColor(7);
                }
                
                bool found = false;
                for (int i = 0; i < totalStaff; i++) {
                    if (staffList[i].cnic == cnic) {
                        found = true;
                        cout << "\nName: " << staffList[i].name;
                        cout << "\nRole: " << staffList[i].role;
                        cout << "\nAssignment: " << staffList[i].assignedEvent << endl;
                        
                        if (staffList[i].assignedEvent != "None") {
                            for (int j = 0; j < totalEvents; j++) {
                                if (eventList[j].name == staffList[i].assignedEvent) {
                                    cout << "\nEvent Date: " << eventList[j].date;
                                    cout << "\nLocation: " << eventList[j].location;
                                    cout << "\nTotal Seats: " << eventList[j].totalSeats;
                                    cout << "\nBooked Seats: " << eventList[j].bookedSeats << endl;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                }
                
                if (!found) {
                    setColor(12);
                    cout << "\nStaff not found!\n";
                    setColor(7);
                }
                pauseScreen();
                break;
            }
                
            case 2:
                showAllEvents();
                break;
                
            case 3:
                return;
                
            default:
                setColor(12);
                cout << "\nInvalid option!\n";
                setColor(7);
                pauseScreen();
        }
    }
}

void mainMenu() {
    while (true) {
        clearScreen();
        setColor(6);
        cout << "\n" << string(60, '=') << endl;
        cout << "\tEVENT MANAGEMENT SYSTEM" << endl;
        cout << string(60, '=') << endl;
        setColor(11);
        
        cout << "\n1. Admin Login";
        cout << "\n2. Guest Portal";
        cout << "\n3. Staff Login";
        cout << "\n4. Exit";
        cout << "\n\nEnter choice: ";
        setColor(7);
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: 
                adminPanel(); 
                break;
                
            case 2: 
                guestPortal(); 
                break;
                
            case 3: 
                staffPortal(); 
                break;
                
            case 4:
                clearScreen();
                setColor(10);
                cout << "\nGoodbye!\n";
                setColor(7);
                exit(0);
                
            default:
                setColor(12);
                cout << "\nInvalid choice!\n";
                setColor(7);
                Sleep(500);
        }
    }
}

int main() {
    eventList[0] = {"Summer Wedding", "15/06/2023", "Grand Ballroom", "Wedding", 150, 0};
    eventList[1] = {"Rock Concert", "20/07/2023", "City Stadium", "Concert", 5000, 0};
    totalEvents = 2;
    
    staffList[0] = {"Ali Khan", "ali@example.com", "1234567890123", "Security", "03001234567", "Summer Wedding"};
    staffList[1] = {"Sara Ahmed", "sara@example.com", "9876543210987", "Catering", "03119876543", "None"};
    totalStaff = 2;
    
    mainMenu();
    return 0;
}