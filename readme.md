
# File System with Authentication and Access Control Model

## Project Overview
This project implements a secure file system with user authentication, access control, and file operations. It utilizes the Bell-LaPadula model for access control, ensuring data confidentiality based on user clearance levels.

### Key Features
- **User Authentication**: Salted hashing-based authentication system for secure user credentials.
- **Access Control**: Implements the Bell-LaPadula model to enforce strict data access based on user clearance.
- **File Operations**: Provides secure file operations (create, read, write, append) based on user access levels.
- **Initialization Mode**: Allows administrators to initialize user credentials securely.

---

## Compilation Instructions

### 1. Ensure C++ Compiler Installed:
   - **Linux**: Install GCC using your package manager (`sudo apt-get install g++`).
   - **MacOS**: Use Homebrew to install GCC (`brew install gcc`).
   - **Windows**: Use MinGW, which can be installed via the MinGW installation manager.

### 2. Navigate to Your Project Directory:
   - Open the terminal (or command prompt on Windows).
   - Use `cd` to navigate to the directory containing the source files (`main.cpp`, `FileSystem.cpp`, `md5.cpp`, `FileSystem.h`, `md5.h`).

### 3. Compile the Source Files:
   Run the following command in your terminal:

   ```bash
   g++ -o FileSystem main.cpp FileSystem.cpp md5.cpp
   ```

### 4. Run the Compiled Program:
   - To initiate the hash/salt/shadow-based user/password creation system:
     ```bash
     ./FileSystem -i
     ```
   - To access the File System with existing credentials:
     ```bash
     ./FileSystem
     ```

---

## Reduction Overview

Reduction, or security reduction, involves steps to minimize potential vulnerabilities and ensure secure handling of sensitive operations. Below are the reduction techniques used in this project:

### 1. Salted Hashing for Passwords
   - **Implementation**: When a new user is created, a random salt is generated with `std::random_device`. The password is concatenated with this salt and hashed using MD5.
   - **Security Advantage**: Prevents attacks using precomputed hash dictionaries (rainbow tables) by making each password hash unique. Even identical passwords have distinct hashes due to differing salts.

### 2. Access Control Based on Clearance Levels
   - **Implementation**: Each file is assigned a classification level (0-3) based on confidentiality, and users have a corresponding clearance level.
   - **Security Advantage**: Restricts unauthorized access to sensitive information, enforcing a security model where users can only access files if their clearance level meets or exceeds the file’s classification.

### 3. Initialization and Authentication
   - **Implementation**: Ensures critical files (`salt.txt`, `shadow.txt`, `Files.store`) are initialized if missing. During login, the provided password is hashed and compared to stored values for validation.
   - **Security Advantage**: Prevents unauthorized access by verifying credentials and mitigates risks from missing or corrupt files.

### 4. Secure File Management
   - **Implementation**: Enforces clearance checks before allowing file operations, limiting access strictly to users with adequate clearance.
   - **Security Advantage**: Reduces the risk of data leaks and unauthorized file modifications, adhering to the principle of least privilege.

---

## Additional Information
This project demonstrates secure coding practices in a simulated file system, utilizing cryptographic functions and strict access controls. It highlights user authentication, access restriction based on security levels, and effective data management for a security-focused environment.
