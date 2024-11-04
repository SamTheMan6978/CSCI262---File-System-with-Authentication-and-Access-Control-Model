#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>
#include <map>

// Constants
// SALT_LENGTH defines the length of the salt used in password hashing. 
// This is an essential part of the hash/salt/shadow-based user/password creation system.
const int SALT_LENGTH = 8;

// File paths used for storing user information and file system data.
const std::string SALT_FILE = "salt.txt";          // File for storing user salts.
const std::string SHADOW_FILE = "shadow.txt";      // File for storing hashed passwords and clearance levels.
const std::string STORE_FILE = "Files.store";      // File for storing the internal file system records.
const std::string MD5_TEST_STRING = "This is a test"; // Test string used to verify the MD5 hashing function.

// Structure to represent a file in the system
// Each file has an owner, a classification level (0-3), and its content.

struct FileEntry {
    std::string owner;         // The owner of the file.
    int classification;        // The classification level of the file (0-3).
    std::string content;       // The content of the file.
};

// Function Prototypes

// Utility Functions
// These functions support the core operations of the file system, including generating salts, hashing passwords, and initializing the system.

std::string generateSalt();                               // Generates a random salt of SALT_LENGTH digits.
bool checkPasswordRequirements(const std::string& password); // Validates that the password meets security requirements.
void initializeSystem();                                  // Initializes system files if they do not exist.
void loadFileStore(std::map<std::string, FileEntry>& fileStore); // Loads the file store from STORE_FILE into memory.
void saveFileStore(const std::map<std::string, FileEntry>& fileStore); // Saves the file store to STORE_FILE.

// User Management Functions
// Functions related to managing users, including creating new users, checking for existing users, and authenticating users.

bool userExists(const std::string& username);             // Checks if a user already exists in the system.
bool createUser(const std::string& username, const std::string& password, int clearance); // Creates a new user with a salt and hashed password.
bool authenticateUser(const std::string& username, const std::string& password, int& clearance); // Authenticates a user based on their username and password.

// File Management Functions
// Functions for managing files within the system, including creating, reading, writing, and listing files.

bool createFile(const std::string& filename, const std::string& owner, int classification, std::map<std::string, FileEntry>& fileStore); // Creates a new file in the system.
bool appendToFile(const std::string& filename, const std::string& content, int userClearance, std::map<std::string, FileEntry>& fileStore); // Appends content to an existing file.
std::string readFile(const std::string& filename, int userClearance, const std::map<std::string, FileEntry>& fileStore); // Reads content from a file.
bool writeFile(const std::string& filename, const std::string& content, int userClearance, std::map<std::string, FileEntry>& fileStore); // Writes content to a file.
void listFiles(const std::map<std::string, FileEntry>& fileStore); // Lists all files in the file store, showing their owners and classification levels.

// Menu Handling
// Functions for displaying and handling user interactions with the system's menu.

void displayMenu();                                       // Displays the user menu with available options.
void handleMenuOption(char option, int userClearance, std::map<std::string, FileEntry>& fileStore); // Handles the user's menu selection, enforcing access control.

// Helper Functions (newly added based on refactoring)
// Additional functions to aid in file validation and error handling.

void displayError(const std::string& message);            // Displays an error message to the user.
bool validateFileExists(const std::string& filename, const std::map<std::string, FileEntry>& fileStore); // Validates that a file exists before allowing operations on it.
bool validateFileDoesNotExist(const std::string& filename, const std::map<std::string, FileEntry>& fileStore); // Validates that a new file does not overwrite an existing file.

#endif // FILESYSTEM_H
