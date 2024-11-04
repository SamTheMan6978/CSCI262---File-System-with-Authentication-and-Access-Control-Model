Compilation Instructions and Reduction Overview

Compilation Instructions:

1. Ensure C++ Compiler Installed:
   - Install using your package manager (`sudo apt-get install g++` for Debian-based systems, or `brew install gcc` for MacOS).
   - On Windows, use MinGW, You can install it via the MinGW installation manager.

2. Navigate to Your Project Directory:
   - Open the terminal (or command prompt on Windows).
   - Use `cd` command to navigate to the directory containing the source files (`main.cpp`, `FileSystem.cpp`, `md5.cpp`, `FileSystem.h`, `md5.h`).

3. Compile the Source Files:
   - Run the following command in your terminal:

     g++ -o FileSystem main.cpp FileSystem.cpp md5.cpp


4. Run the Compiled Program:

     ./FileSystem -i (for initating the hash/salt/shadow based user/password creation system)
     ./FileSystem (use the credentials to access the FileSystem)


Reduction in the Program:

Reduction, or Security reduction refers to the steps taken to ensure that sensitive operations, such as password handling and file access, are implemented in a secure manner to minimize potential vulnerabilities. Here's how reduction was performed in the code:

1. Salted Hashing for Passwords:
   - When a new user is created, their password is not stored directly. Instead, a random salt is generated using a cryptographically secure random number generator (`std::random_device`).
   - The password is concatenated with this salt and then hashed using the MD5 algorithm. This hash is stored instead of the raw password.
   - Security Advantage: This approach prevents attacks the adversary might use precomputed hash dictionaries (rainbow tables) to reverse-engineer passwords. Even if two users have the same password, their stored hashes will differ due to different salts.

2. Access Control Based on Clearance Levels:
   - Each file in the system has a classification level (0-3), and each user has a corresponding clearance level.
   - The system ensures that users can only read, write, or append to files if their clearance level is equal to or higher than the file's classification level.
   - Security Advantage: This prevents unauthorized access to sensitive information, enforcing strict access control based on predefined clearance levels.

3. Initialization and Authentication:
   - The system initializes critical files (`salt.txt`, `shadow.txt`, `Files.store`) if they do not exist, ensuring a clean state for the application.
   - During user authentication, the system checks whether the stored hash of the provided password matches the one stored in the shadow file, ensuring that only users with correct credentials can log in.
   - Security Advantage: This setup mitigates risks associated with missing or corrupt files, and ensures that only authenticated users can access the system.

4. Secure File Management:
   - The file management functions enforce clearance checks before allowing any operations on files. Users cannot perform operations on files classified above their clearance level.
   - Security Advantage: This limits the potential for data leakage and unauthorized modification of files, adhering to the principle of least privilege.

