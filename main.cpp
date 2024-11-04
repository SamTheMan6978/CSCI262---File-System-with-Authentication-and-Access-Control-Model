#include "FileSystem.h" // Include the file system header, which contains declarations for file and user management functions
#include "md5.h" // Include the MD5 header, which provides functions for hashing strings
#include <iostream> // Include standard input-output stream
#include <map> // Include the map container to manage the file system's in-memory structure

// Function declarations
bool initializeMode(int argc, char* argv[]); // Function to handle system initialization mode
bool loginUser(int& userClearance); // Function to handle user login and authentication
void handleProgramLoop(int userClearance, std::map<std::string, FileEntry>& fileStore); // Function to handle the main program loop

int main(int argc, char* argv[]) {
    // Display the MD5 hash of a test string to verify the MD5 function
    std::cout << "MD5 (\"" << MD5_TEST_STRING << "\") = " << md5(MD5_TEST_STRING) << std::endl;

    // Initialize the file store in memory
    std::map<std::string, FileEntry> fileStore; // Create a map to hold the files in memory
    loadFileStore(fileStore);  // Load existing file store from disk if available

    // Handle initialization mode if the flag is present
    if (initializeMode(argc, argv)) {
        return 0; // Exit after initialization if in initialization mode
    }

    // Handle user login
    int userClearance = 0; // Variable to store user's clearance level
    if (!loginUser(userClearance)) { // Attempt to login
        return 1; // Exit if login fails
    }

    // Handle the main program loop after successful login
    handleProgramLoop(userClearance, fileStore); // Pass user clearance and file store to the main loop

    return 0; // Exit the program
}

// Function to handle initialization mode
bool initializeMode(int argc, char* argv[]) {
    // Check if the "-i" flag is passed to initialize the system
    if (argc > 1 && std::string(argv[1]) == "-i") {
        initializeSystem();  // Ensure all necessary files are created if they don't exist
        std::string username, password, confirmPassword; // Variables to store user input
        int clearance; // Variable to store user clearance level

        // Prompt for a new username
        std::cout << "Enter new username: ";
        std::cin >> username;

        // Check if the username already exists
        if (userExists(username)) {
            std::cout << "Error: Username already exists.\n"; // Notify if username is taken
            return true; // Exit the initialization mode
        }

        // Prompt for a valid password that meets the requirements
        do {
            // Inner loop to ensure the password meets the security requirements
            do {
                std::cout << "Enter password: "; // Prompt for the password
                std::cin >> password; // Capture the password input
            } while (!checkPasswordRequirements(password)); // Repeat until the password meets the requirements

            // Prompt for password confirmation
            std::cout << "Confirm password: ";
            std::cin >> confirmPassword; // Capture the confirmation password

            // Notify the user if the passwords don't match
            if (password != confirmPassword) {
                std::cout << "Error: Passwords do not match.\n";
            }
        } while (password != confirmPassword); // Repeat until the passwords match

        // Prompt for the user's clearance level
        std::cout << "Enter clearance level (0, 1, 2, or 3): ";
        std::cin >> clearance;

        // Attempt to create the new user with the provided details
        if (createUser(username, password, clearance)) {
            std::cout << "User " << username << " created successfully.\n"; // Success message
        }
        else {
            std::cout << "Failed to create user.\n"; // Failure message
            return true; // Exit the initialization mode
        }
        return true;  // Exit after successful user creation in initialization mode
    }
    return false; // Return false if not in initialization mode
}

// Function to handle user login
bool loginUser(int& userClearance) {
    std::string username, password; // Variables to store user credentials

    // Prompt for username
    std::cout << "Username: ";
    std::cin >> username;

    // Prompt for password
    std::cout << "Password: ";
    std::cin >> password;

    // Attempt to authenticate the user
    if (!authenticateUser(username, password, userClearance)) {
        std::cout << "Authentication failed. Exiting...\n"; // Notify on failure
        return false; // Return false on failure
    }
    return true; // Return true on success
}

// Function to handle the main program loop
void handleProgramLoop(int userClearance, std::map<std::string, FileEntry>& fileStore) {
    char option; // Variable to store user's menu choice
    std::string input; // Variable to store user's input

    do {
        displayMenu(); // Display the menu options
        std::cout << "Choose an option: "; // Prompt for user input
        std::cin >> input;

        // Extract the first character and convert it to lowercase for the menu option
        option = std::tolower(input[0]);

        handleMenuOption(option, userClearance, fileStore); // Process the user's menu choice

    } while (true); // Continue the loop indefinitely
}
