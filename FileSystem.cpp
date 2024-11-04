#include "FileSystem.h"
#include "md5.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>

// Utility Functions

// Generates a random salt of the specified length
// This function is essential for the hash/salt/shadow based user/password creation system.
std::string generateSalt() {
    std::string salt;
    std::random_device rd;  // Cryptographically secure random number generator
    std::uniform_int_distribution<int> dist(0, 9);  // Distribution to generate numbers between 0 and 9

    for (int i = 0; i < SALT_LENGTH; ++i) {
        salt += std::to_string(dist(rd));  // Generate a random digit (0-9)
    }
    return salt;
}

// Checks if the password meets certain security requirements
// This function ensures that the password meets a minimum length requirement, as per the assignment specifications.
bool checkPasswordRequirements(const std::string& password) {
    if (password.length() < 8) {
        std::cout << "Password must be at least 8 characters long.\n";
        return false;
    }
    // Additional checks could include numbers, special characters, etc.
    return true;
}

// Initializes the system by creating empty salt and shadow files if they do not exist
// This function is required for setting up the file system for the first time, ensuring the necessary files are in place.
void initializeSystem() {
    // Check if salt.txt exists; if not, create it
    std::ifstream saltCheck(SALT_FILE);
    if (!saltCheck) {
        std::ofstream saltFile(SALT_FILE);
        saltFile.close();
        std::cout << "Created " << SALT_FILE << std::endl;
    }
    else {
        saltCheck.close();
    }

    // Check if shadow.txt exists; if not, create it
    std::ifstream shadowCheck(SHADOW_FILE);
    if (!shadowCheck) {
        std::ofstream shadowFile(SHADOW_FILE);
        shadowFile.close();
        std::cout << "Created " << SHADOW_FILE << std::endl;
    }
    else {
        shadowCheck.close();
    }

    // Check if Files.store exists; if not, create it
    std::ifstream storeCheck(STORE_FILE);
    if (!storeCheck) {
        std::ofstream storeFile(STORE_FILE);
        storeFile.close();
        std::cout << "Created " << STORE_FILE << std::endl;
    }
    else {
        storeCheck.close();
    }
}

// Loads the file store from the store file into the provided map
// This function reads the Files.store file into memory, allowing access control to be enforced on the files.
void loadFileStore(std::map<std::string, FileEntry>& fileStore) {
    std::ifstream storeFile(STORE_FILE);
    if (storeFile) {
        std::string filename, owner;
        int classification;
        std::string content;

        while (storeFile >> filename >> owner >> classification) {
            storeFile.ignore();  // Ignore the newline character
            std::getline(storeFile, content);
            fileStore[filename] = { owner, classification, content };
        }
    }
    storeFile.close();
}

// Saves the file store to the store file from the provided map
// This function ensures that the current state of the file system is saved to Files.store.
void saveFileStore(const std::map<std::string, FileEntry>& fileStore) {
    std::ofstream storeFile(STORE_FILE);
    for (const auto& entry : fileStore) {
        storeFile << entry.first << " " << entry.second.owner << " " << entry.second.classification << "\n";
        storeFile << entry.second.content << "\n";
    }
    storeFile.close();
}

// User Management Functions

// Checks if a username already exists in the system
// This function verifies whether a user is already registered by checking the salt file.
bool userExists(const std::string& username) {
    std::ifstream saltFile(SALT_FILE);
    std::string line;
    while (std::getline(saltFile, line)) {
        if (line.find(username + ":") == 0) {
            return true;
        }
    }
    return false;
}

// Creates a new user with a salt and hashed password
// This function adds a new user to the system, storing their salt and hashed password, and assigns a security clearance.
bool createUser(const std::string& username, const std::string& password, int clearance) {
    if (userExists(username)) {
        std::cout << "Username already exists.\n";
        return false;
    }

    std::string salt = generateSalt();  // Generate a random salt for the user
    std::string passSaltHash = md5(password + salt);  // Hash the password combined with the salt

    // Append the new user's information to the salt and shadow files
    std::ofstream saltFile(SALT_FILE, std::ios::app);
    saltFile << username << ":" << salt << "\n";
    saltFile.close();

    std::ofstream shadowFile(SHADOW_FILE, std::ios::app);
    shadowFile << username << ":" << passSaltHash << ":" << clearance << "\n";
    shadowFile.close();

    return true;
}

// Authenticates a user based on their username and password
// This function verifies the user's identity by comparing the hashed password against the stored hash in shadow.txt.
bool authenticateUser(const std::string& username, const std::string& password, int& clearance) {
    // Check if the salt file exists
    std::ifstream saltFile(SALT_FILE);
    if (!saltFile) {
        std::cout << "Error: Salt file (" << SALT_FILE << ") not found. Authentication cannot proceed.\n";
        return false;
    }

    // Check if the shadow file exists
    std::ifstream shadowFile(SHADOW_FILE);
    if (!shadowFile) {
        std::cout << "Error: Shadow file (" << SHADOW_FILE << ") not found. Authentication cannot proceed.\n";
        saltFile.close();
        return false;
    }

    // Check if the username exists in the salt file
    if (!userExists(username)) {
        std::cout << "Username not found.\n";
        saltFile.close();
        shadowFile.close();
        return false;
    }

    // Extract the salt for the given username
    std::string line, salt;
    while (std::getline(saltFile, line)) {
        if (line.find(username + ":") == 0) {
            salt = line.substr(username.length() + 1);  // Extract the salt from the line
            break;
        }
    }
    saltFile.close();

    // Display that the user was found and salt was retrieved
    std::cout << username << " found in " << SALT_FILE << std::endl;
    std::cout << "salt retrieved: " << salt << std::endl;

    // Combine the entered password with the extracted salt and hash it
    std::cout << "hashing ..." << std::endl;
    std::string passSaltHash = md5(password + salt);
    std::cout << "hash value: " << passSaltHash << std::endl;

    // Authenticate the user by comparing the computed hash with the stored hash
    while (std::getline(shadowFile, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedHash;
        int storedClearance;

        std::getline(ss, storedUsername, ':');  // Extract username from the line
        std::getline(ss, storedHash, ':');      // Extract hash from the line
        ss >> storedClearance;                  // Extract clearance level

        if (storedUsername == username && storedHash == passSaltHash) {
            clearance = storedClearance;
            std::cout << "Authentication for user " << username << " complete.\n";
            std::cout << "The clearance for " << username << " is " << clearance << ".\n";
            shadowFile.close();
            return true;
        }
    }
    shadowFile.close();

    std::cout << "Authentication failed.\n";
    return false;
}

// File Management Functions

// Creates a new file in the system with the owner's classification level
// This function allows users to create files, ensuring that the file's classification level matches the user's clearance.
bool createFile(const std::string& filename, const std::string& owner, int classification, std::map<std::string, FileEntry>& fileStore) {
    if (fileStore.find(filename) != fileStore.end()) {
        std::cout << "File already exists.\n";
        return false;
    }

    fileStore[filename] = { owner, classification, "" };
    std::cout << "File " << filename << " created successfully with classification level " << classification << ".\n";
    return true;
}

// Appends content to an existing file, enforcing clearance checks
// This function allows users to append content to files if their clearance level is sufficient.
bool appendToFile(const std::string& filename, const std::string& content, int userClearance, std::map<std::string, FileEntry>& fileStore) {
    auto it = fileStore.find(filename);
    if (it == fileStore.end()) {
        std::cout << "File not found.\n";
        return false;
    }

    if (userClearance < it->second.classification) {
        std::cout << "Access denied: Your clearance level (" << userClearance << ") is too low to append to this file.\n";
        return false;
    }

    it->second.content += content;
    std::cout << "Content appended to " << filename << " successfully.\n";
    return true;
}

// Reads content from an existing file, enforcing clearance checks
// This function allows users to read files if their clearance level is sufficient.
std::string readFile(const std::string& filename, int userClearance, const std::map<std::string, FileEntry>& fileStore) {
    auto it = fileStore.find(filename);
    if (it == fileStore.end()) {
        return "File not found.";
    }

    if (userClearance < it->second.classification) {
        return "Access denied: Your clearance level (" + std::to_string(userClearance) + ") is too low to read this file.";
    }

    return it->second.content;
}

// Writes content to an existing file, enforcing clearance checks
// This function allows users to overwrite file content if their clearance level is sufficient.
bool writeFile(const std::string& filename, const std::string& content, int userClearance, std::map<std::string, FileEntry>& fileStore) {
    auto it = fileStore.find(filename);
    if (it == fileStore.end()) {
        std::cout << "File not found.\n";
        return false;
    }

    if (userClearance < it->second.classification) {
        std::cout << "Access denied: Your clearance level (" << userClearance << ") is too low to write to this file.\n";
        return false;
    }

    it->second.content = content;
    std::cout << "Content written to " << filename << " successfully.\n";
    return true;
}

// Lists all files in the system, displaying their classification levels and owners
// This function provides a list of all files and their associated metadata.
void listFiles(const std::map<std::string, FileEntry>& fileStore) {
    for (const auto& entry : fileStore) {
        std::cout << entry.first << " owned by " << entry.second.owner << " classified at level "
            << entry.second.classification << "\n";
    }
}

// Menu Handling (slightly modified to reflect access control)

// Displays the user menu with available options
// This function shows the user what actions they can take in the file system.
void displayMenu() {
    std::cout << "Options: (C)reate, (A)ppend, (R)ead, (W)rite, (L)ist, (S)ave or (E)xit.\n";
}

// Displays an error message to the user
// This function standardizes the way errors are shown to the user.
void displayError(const std::string& message) {
    std::cout << "Error: " << message << "\n";
}

// Validates that a file exists before allowing operations on it
// This function checks whether the file exists in the file store.
bool validateFileExists(const std::string& filename, const std::map<std::string, FileEntry>& fileStore) {
    if (fileStore.find(filename) == fileStore.end()) {
        displayError("File '" + filename + "' does not exist.");
        return false;
    }
    return true;
}

// Validates that a file does not already exist before creating it
// This function ensures that new files do not overwrite existing ones.
bool validateFileDoesNotExist(const std::string& filename, const std::map<std::string, FileEntry>& fileStore) {
    if (fileStore.find(filename) != fileStore.end()) {
        displayError("File '" + filename + "' already exists.");
        return false;
    }
    return true;
}

// Handles the user's menu selection, enforcing access control
// This function processes the user's chosen action, ensuring that access controls are respected.
void handleMenuOption(char option, int userClearance, std::map<std::string, FileEntry>& fileStore) {
    std::string filename, content;

    switch (std::tolower(option)) {
    case 'c':
        // Create file
        std::cout << "Enter filename: ";
        std::cin >> filename;

        if (validateFileDoesNotExist(filename, fileStore)) {
            createFile(filename, "CurrentUser", userClearance, fileStore);
        }
        break;

    case 'a':
        // Append to file
        std::cout << "Enter filename: ";
        std::cin >> filename;

        if (validateFileExists(filename, fileStore)) {
            std::cout << "Enter content to append: ";
            std::cin.ignore();
            std::getline(std::cin, content);
            appendToFile(filename, content, userClearance, fileStore);
        }
        break;

    case 'r':
        // Read file
        std::cout << "Enter filename: ";
        std::cin >> filename;

        if (validateFileExists(filename, fileStore)) {
            std::cout << readFile(filename, userClearance, fileStore) << std::endl;
        }
        break;

    case 'w':
        // Write to file
        std::cout << "Enter filename: ";
        std::cin >> filename;

        if (validateFileExists(filename, fileStore)) {
            std::cout << "Enter content to write: ";
            std::cin.ignore();
            std::getline(std::cin, content);
            writeFile(filename, content, userClearance, fileStore);
        }
        break;

    case 'l':
        // List files
        if (fileStore.empty()) {
            std::cout << "No files available.\n";
        }
        else {
            listFiles(fileStore);
        }
        break;

    case 's':
        // Save file store
        saveFileStore(fileStore);
        std::cout << "File store saved successfully.\n";
        break;

    case 'e':
        // Exit system
        char confirm;
        std::cout << "Shut down the FileSystem? (Y)es or (N)o: ";
        std::cin >> confirm;
        confirm = std::tolower(confirm);

        if (confirm == 'y') {
            std::exit(0);
        }
        else if (confirm == 'n') {
            std::cout << "Shutdown canceled. Returning to the menu...\n";
        }
        else {
            displayError("Invalid input. Returning to the menu...");
        }
        break;

    default:
        // Invalid option
        displayError("Invalid option.");
        break;
    }
}