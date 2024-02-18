#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const string FILE_NAME = "userdata.txt"; // File to store user data

//Function Prototypes
bool isUsernameExists(const string& username);
void registerUser();
void forgotPassword();
void loggedInMenu(const string& username);
void loginUser();
void displayMenu();

int main() {
    displayMenu();
    return 0;
}

/// Function to check if a username already exists in the file
bool isUsernameExists(const string& username) {
    ifstream file(FILE_NAME);
    string line;
    while (getline(file, line)) {
        if (line == username) {
            file.close();
            return true; // Username found
        }
    }
    file.close();
    return false; // Username not found
}

// Function to register a new user
void registerUser() {
    ofstream file(FILE_NAME, ios::app);
    string username, password, securityQs, answer;

    cout << "\nEnter username: ";
    cin >> username;

    if (isUsernameExists(username)) {
        cout << "Username already exists. Please choose a different username or login.\n";
        file.close();
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    cin.ignore(); // Clear input buffer
    cout << "\nSelect a security question:\n";
    cout << "1. What is your pet's name?\n2. What city were you born in?\n3. What is the name of your favorite childhood friend?\n";
    cout << "4. What is the name of your favorite teacher?\n5. What is the name of the street you grew up on?\n";
    int choice;
    cout << "\nEnter your question number: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer

    switch (choice) {
        case 1:
            securityQs = "What is your pet's name?";
            break;
        case 2:
            securityQs = "What city were you born in?";
            break;
        case 3:
            securityQs = "What is the name of your favorite childhood friend?";
            break;
        case 4:
            securityQs = "What is the name of your favorite teacher?";
            break;
        case 5:
            securityQs = "What is the name of the street you grew up on?";
            break;
        default:
            cout << "Invalid choice.\n";
            file.close();
            return;
    }

    cout << "Enter your answer: ";
    getline(cin, answer);

    // Store user data in the file
    file << username << "\n";
    file << password << "\n";
    file << securityQs << "\n";
    file << answer << "\n";
    file.close();

    cout << "\nRegistration successful!\n";
}

// Function to handle forgot password functionality
void forgotPassword() {
    ifstream file(FILE_NAME);
    string username, line, storedSecurityQs, storedAnswer, storedPassword, newPassword;

    if (!file.is_open()) {
        cout << "\nNo data stored. Kindly register first.\n";
        file.close();
        return;
    }

    cout << "\nEnter your username: ";
    cin >> username;

    bool found = false;
    ofstream updatedFile("temp.txt"); // Create a temporary file to update

    while (getline(file, line)) {
        if (line == username) {
            found = true;
            updatedFile << line << "\n"; // Write username
            getline(file, storedPassword); // Get stored password
            updatedFile << storedPassword << "\n"; // Write stored password
            getline(file, storedSecurityQs); // Get stored security question
            updatedFile << storedSecurityQs << "\n"; // Write stored security question
            getline(file, storedAnswer); // Get stored answer
            updatedFile << storedAnswer << "\n"; // Write stored answer
            cout << "\nSecurity Question: " << storedSecurityQs << "\n";
            cout << "Enter your answer: ";
            cin.ignore(); // Clear input buffer
            string userAnswer;
            getline(cin, userAnswer); // Get user's answer

            if (userAnswer != storedAnswer) {
                cout << "\nSecurity answer doesn't match our records. Password update failed.\n";
                updatedFile.close();
                remove("temp.txt"); // Delete the temporary file
                file.close();
                return;
            } else {
                cout << "\nEnter new password: ";
                cin >> newPassword;
                updatedFile << newPassword << "\n"; // Write new password
                cout << "\nPassword updated successfully!\n";
            }
        } else {
            updatedFile << line << "\n"; // Write other user data as it is
        }
    }

    file.close();
    updatedFile.close();

    if (!found) {
        cout << "\nUsername not found. Please register.\n";
        remove("temp.txt"); // Delete the temporary file
        return;
    }

    remove(FILE_NAME.c_str()); // Delete old file
    rename("temp.txt", FILE_NAME.c_str()); // Rename temp file to userdata.txt
}

// Function to update password for the logged-in user
void updatePassword(const string& username, const string& newPassword) {
    ifstream file(FILE_NAME);
    ofstream tempFile("temp.txt");

    string line;
    bool userFound = false;
    int linesProcessed = 0;

    while (getline(file, line)) {
        if (line == username) {
            userFound = true;
            linesProcessed++;
            tempFile << line << "\n"; // Write username
            getline(file, line); // Skip old password
            tempFile << newPassword << "\n"; // Write new password
        } else {
            tempFile << line << "\n"; // Write other user data as is
        }
    }

    if (!userFound) {
        cout << "User not found!\n";
    }

    file.close();
    tempFile.close();

    remove(FILE_NAME.c_str()); // Delete old file
    rename("temp.txt", FILE_NAME.c_str()); // Rename temp file to userdata.txt

    cout << "Password updated!\n"; // Confirmation message after password update
}

// Function to display the logged-in user menu
void loggedInMenu(const string& username) {
    int choice;
    do {
        cout << "\nLogged in as: " << username << "\n";
        cout << "1. Update Password\n";
        cout << "2. Logout to Main Menu\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string newPassword;
                cout << "\nEnter new password: ";
                cin >> newPassword;
                updatePassword(username, newPassword); // Update password
                break;
            }
            case 2:
                cout << "\nLogging out...\n";
                // No need to update anything for logout
                break;
            case 3:
                cout << "\nExiting...\n";
                exit(0);
            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
    } while (choice != 2);
}

// Function to handle login
void loginUser() {
    ifstream file(FILE_NAME);
    string username, password, line, storedPassword;
    bool found = false;

    if (!file.is_open()) {
        cout << "No data stored. Kindly register first.\n";
        file.close();
        return;
    }

    cout << "\nEnter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    while (getline(file, line)) {
        if (line == username) {
            getline(file, storedPassword); // Get stored password
            if (storedPassword == password) {
                found = true;
                break;
            }
        }
    }

    file.close();

    if (found) {
        cout << "Login successful!\n";
        loggedInMenu(username); // Proceed to logged-in user menu
    } else {
        cout << "\nIncorrect username/password.\n";
        cout << "Forgot Password? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            forgotPassword(); // Handle forgot password scenario
        }
    }
}

// Function to display the main menu
void displayMenu() {
    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                cout << "\nExiting...\n\n";
                exit(0);
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    } while (choice != 3);
}