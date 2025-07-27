# posn

This repository contains solutions to various competitive programming problems, implemented in C++, C#, and Python. Each folder corresponds to a specific problem or topic, with implementations in one or more languages.

---

## C++ Programs

### Day1_ORBS
- **File:** `Day1_ORBS/orbs.cpp`
- **Description:** Solution to the "ORBS" problem, likely involving operations on orbs with weights and selection logic.

### Day1_Route
- **Files:** `Day1_Route/route.cpp`, `Day1_Route/route.h`, `Day1_Route/grader.cpp`, `Day1_Route/test_route.cpp`, `Day1_Route/test_grader.cpp`
- **Description:** Implements a route assignment algorithm. Includes a grader and test files for validation.

### Day1_Sculpture
- **File:** `Day1_Sculpture/sculpture.cpp`
- **Description:** Solves the "Sculpture" problem, likely involving dynamic programming and segment trees for optimal arrangement.

### Day2_Bit_String
- **File:** `Day2_Bit_String/bit_string.cpp`
- **Description:** Handles bit string manipulations and queries, possibly using dynamic programming.

### Day2_Lover
- **File:** `Day2_Lover/lover.cpp`
- **Description:** Solution for the "Lover" problem, involving prefix sums and dynamic programming for segment partitioning.

### Day2_Tour
- **File:** `Day2_Tour/tour.cpp`
- **Description:** Solves the "Tour" problem, involving graph traversal and path calculations.

---

## C# Programs

### Day1_ORBS/OrbsApp
- **File:** `Day1_ORBS/OrbsApp/Program.cs`
- **Description:** C# implementation of the ORBS problem, using collections and LINQ for efficient operations.

### Day1_Route/RouteApp
- **File:** `Day1_Route/RouteApp/Program.cs`
- **Description:** C# version of the route assignment problem, with input/output handling and route logic.

### Day1_Sculpture/SculptureApp
- **File:** `Day1_Sculpture/SculptureApp/Program.cs`
- **Description:** C# solution for the Sculpture problem, using segment trees and dynamic programming.

### Day2_Bit_String/BitStringApp
- **File:** `Day2_Bit_String/BitStringApp/Program.cs`
- **Description:** C# implementation for bit string queries and dynamic programming.

### Day2_Lover/LoverApp
- **File:** `Day2_Lover/LoverApp/Program.cs`
- **Description:** C# solution for the Lover problem, using prefix sums and dynamic programming.

### Day2_Tour/TourApp
- **File:** `Day2_Tour/TourApp/Program.cs`
- **Description:** C# implementation of the Tour problem, involving graph traversal and path comparison.

---

## Python Programs

### Day2_Tour
- **File:** `Day2_Tour/tour_v2.py`
- **Description:** Python implementation of the Tour problem, using threading for fast input and graph traversal logic.

---

## Additional Information

- Each folder may contain a `Readme/` subfolder with problem statements or additional documentation.
- The `.vscode/` folder contains configuration for building and debugging in Visual Studio Code.

---

## Setting Up C++ on Windows

You can set up a C++ development environment using [Chocolatey](https://chocolatey.org/):

1. **Install Chocolatey** (if not already installed):  
   Open PowerShell as Administrator and run:
   ```
   Set-ExecutionPolicy Bypass -Scope Process -Force; `
     [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; `
     iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
   ```

2. **Install a C++ compiler and build tools:**  
   ```
   choco install mingw
   choco install cmake
   ```

3. **(Optional) Install Visual Studio Code:**  
   ```
   choco install vscode
   ```

4. **Add MinGW to your PATH:**  
   Chocolatey usually adds it automatically, but if not, add `C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin` to your system PATH.

5. **Verify installation:**  
   Open a new terminal and run:
   ```
   g++ --version
   cmake --version
   ```

Now you can build and run C++ programs from the terminal or within VS Code.

---

## Setting Up C++ Extensions in Visual Studio Code

To enhance your C++ development experience in Visual Studio Code, follow these steps:

1. **Install the C/C++ Extension:**
   - Open Visual Studio Code.
   - Go to the Extensions view by clicking the square icon on the sidebar or pressing `Ctrl+Shift+X`.
   - Search for `C/C++` by Microsoft.
   - Click **Install**.

2. **(Optional) Install Additional Helpful Extensions:**
   - **CMake Tools** (for CMake project integration)
   - **Code Runner** (to quickly run code snippets)
   - **Better C++ Syntax** (for improved syntax highlighting)

3. **Configure IntelliSense and Build Tasks:**
   - After installing, open a C++ file. The extension will prompt you to install recommended tools if needed.
   - You can configure build tasks by pressing `Ctrl+Shift+B` and selecting a build configuration or creating a new one.

4. **Debugging:**
   - Set breakpoints and press `F5` to start debugging.
   - Make sure your compiler (e.g., MinGW) is in your system PATH.

For more details, see the [official documentation](https://code.visualstudio.com/docs/languages/cpp).

---