#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Structure to store billing information
struct BillingInfo {
    string modeOfPayment;   // Payment method (CASH or CARD)
    string email;           // User's email address
    string contactNumber;   // User's contact number
};

// Structure to store user information, including billing
struct User {
    string name;            // User's full name
    string username;        // Username for login
    int subscriptionType;   // Subscription type (1 = Basic, 2 = Standard, 3 = Premium)
    BillingInfo billing;    // Billing information for the user
};

// Class to manage user data, including file I/O and in-memory storage
class UserDatabase {
private:
    string filename;
    vector<User> users;  // Vector to store users in memory

public:
    UserDatabase(const string& file) : filename(file) {
        // Check if the file exists or can be opened
        if (!loadUsers()) {
            cout << "Error: Unable to open or read the file: " << filename << endl;
            // Optionally, create the file here or handle the error more gracefully
        }
    }

    // Load users from file into the vector
    bool loadUsers() {
        ifstream userFile(filename);
        if (!userFile) {
            // If the file does not exist, return false
            return false;
        }

        User user;
        while (getline(userFile, user.name)) {
            getline(userFile, user.username);
            userFile >> user.subscriptionType;
            userFile.ignore(); // Ignore newline after subscription type
            getline(userFile, user.billing.modeOfPayment);
            getline(userFile, user.billing.email);
            getline(userFile, user.billing.contactNumber);
            users.push_back(user);
        }
        return true;
    }

    // Save all users from the vector to the file
    void saveAllUsers() {
        ofstream userFile(filename);
        for (const auto& user : users) {
            userFile << user.name << endl;
            userFile << user.username << endl;
            userFile << user.subscriptionType << endl;
            userFile << user.billing.modeOfPayment << endl;
            userFile << user.billing.email << endl;
            userFile << user.billing.contactNumber << endl;
        }
    }

    // Save a new user to the file and vector
    void saveUser(const User& user) {
        users.push_back(user);  // Add user to the in-memory vector
        ofstream userFile(filename, ios::app);  // Append new user to file
        userFile << user.name << endl;
        userFile << user.username << endl;
        userFile << user.subscriptionType << endl;
        userFile << user.billing.modeOfPayment << endl;
        userFile << user.billing.email << endl;
        userFile << user.billing.contactNumber << endl;
    }

    // Get the list of users
    const vector<User>& getUsers() const {
        return users;
    }

    // Find a user by name and username
    const User* findUser(const string& name, const string& username) const {
        for (const auto& user : users) {
            if (user.name == name && user.username == username) {
                return &user;
            }
        }
        return nullptr;
    }

    // Get the count of users
    int getUserCount() const {
        return users.size();
    }
};

// Function to register a new user
void registerUser(UserDatabase& db) {
    User newUser;
    cout << "--- REGISTER USER ---" << endl;
    cout << "Enter Customer's Name: "; cin.ignore(); getline(cin, newUser.name);
    cout << "Select a Username: "; cin >> newUser.username;

    int subChoice;
    cout << "--- Subscription Type ---" << endl;
    cout << "[1] Basic\n[2] Standard\n[3] Premium\nChoose: ";
    cin >> subChoice;
    cin.ignore();

    switch (subChoice) {
    case 1: newUser.subscriptionType = 1; break;
    case 2: newUser.subscriptionType = 2; break;
    case 3: newUser.subscriptionType = 3; break;
    default:
        cout << "Invalid choice, setting to Basic by default." << endl;
        newUser.subscriptionType = 1;
    }

    cout << "--- Billing Information ---" << endl;
    cout << "Mode of Payment (CASH/CARD): "; getline(cin, newUser.billing.modeOfPayment);
    cout << "Email Address: "; getline(cin, newUser.billing.email);
    cout << "Contact Number: "; getline(cin, newUser.billing.contactNumber);

    db.saveUser(newUser);   // Save new user to the database
    cout << "Registration complete!" << endl;
}

// Function to log in a user
bool loginUser(UserDatabase& db, string& loggedInUsername) {
    string inputName, inputUsername;
    cout << "--- LOGIN/VERIFY USER ---" << endl;
    cout << "Enter Customer's Name: "; cin.ignore(); getline(cin, inputName);
    cout << "Enter Username: "; cin >> inputUsername;

    const User* user = db.findUser(inputName, inputUsername);
    if (user) {
        loggedInUsername = inputUsername;
        return true;
    }

    cout << "User not found. Please try again." << endl;
    return false;
}

// Function to print payslip information for a user
void printPayslip(const User& user) {
    cout << "--- PRINT PAYSLIP ---" << endl;
    cout << "Username: " << user.username << endl;
    cout << "Customer's Name: " << user.name << endl;
    cout << "--- Subscription Type ---" << endl;
    switch (user.subscriptionType) {
    case 1: cout << "Type: Basic" << endl; break;
    case 2: cout << "Type: Standard" << endl; break;
    case 3: cout << "Type: Premium" << endl; break;
    default: cout << "Type: Unknown" << endl;
    }
    cout << "--- Billing Information ---" << endl;
    cout << "Mode of Payment: " << user.billing.modeOfPayment << endl;
    cout << "Email Address: " << user.billing.email << endl;
    cout << "Contact Number: " << user.billing.contactNumber << endl;
    cout << "--- LOG OUT USER ---" << endl;
}

// Function to display admin analytics (requires PIN)
void viewAnalytics(UserDatabase& db) {
    string adminPIN = "1234";
    string enteredPIN;
    cout << "Enter Admin PIN to view Analytics: ";
    cin >> enteredPIN;

    if (enteredPIN == adminPIN) {
        cout << "--- ADMIN'S DATABASE/ANALYTICS ---" << endl;
        cout << "Active Users: " << db.getUserCount() << endl;
        for (const auto& user : db.getUsers()) {
            cout << "Name: " << user.name << ", Username: " << user.username << endl;
            cout << "Subscription Type: " << user.subscriptionType << endl;
            cout << "Billing Email: " << user.billing.email << ", Contact: " << user.billing.contactNumber << endl;
        }
    }
    else {
        cout << "Invalid PIN. Access Denied." << endl;
    }
}

int main() {
    UserDatabase db("users.txt");  // Initialize UserDatabase with filename
    int choice;
    string loggedInUsername;

    while (true) {
        cout << "--- USER'S GYM MEMBERSHIP ---" << endl;
        cout << "[1] Register User\n[2] Login User\n[3] Analytics\n[0] Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser(db);
            break;
        case 2:
            if (loginUser(db, loggedInUsername)) {
                const User* user = db.findUser(loggedInUsername, loggedInUsername);
                if (user) printPayslip(*user);
            }
            break;
        case 3:
            viewAnalytics(db);
            break;
        case 0:
            cout << "Exiting the program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}