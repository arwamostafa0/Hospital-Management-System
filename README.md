# Hospital Management System (C++) 🏥

A robust, terminal-based Hospital Management System developed in **C++**. This project focuses on efficient data management using **Binary File Handling** and advanced **Indexing techniques** to ensure high performance and data persistence.

## 🌟 Key Features

### 👤 Multi-User Access Levels
The system provides tailored experiences for different roles:
* **Administrator:** Full control over managing doctors, patients, and system reports.
* **Doctor:** Manage profiles and view scheduled appointments.
* **Patient:** Manage personal profiles, search for doctors, and book/cancel appointments.

### ⚙️ Core Functionalities
* **Secure Authentication:** Role-based login system.
* **Doctor Management:** Add, Update, Search (by Name/ID), and Delete doctor records.
* **Patient Management:** Manage profiles and medical history.
* **Appointment System:** Link patients with doctors, schedule slots, and update status.
* **Advanced Search:** Fast retrieval using Primary and Secondary indexing.

## 🛠️ Technical Architecture

### 🗄️ Data Persistence & Indexing
The system is designed to handle data like a real database:
-   **Binary Storage:** All records are stored in binary files for efficiency.
-   **Primary Index:** Used for fast access via unique IDs (e.g., Doctor ID).
-   **Secondary Index:** Enables searching by non-unique fields (e.g., Names).
-   **Memory Management:** Indexes are loaded into memory during program startup and saved back to files upon exit.

### 🏗️ Built With
- **Language:** C++
- **Build System:** CMake
- **Concepts:** Object-Oriented Programming (OOP), File Streams (fstream), Data Structures, and Indexing.

## 📊 System Flow
1.  **Startup:** Load Primary and Secondary indexes from files into memory.
2.  **Authentication:** User chooses a role (Admin/Doctor/Patient) and logs in.
3.  **Operations:** User performs actions through a clean, menu-driven interface.
4.  **Shutdown:** All changes are written back to files to ensure data is never lost.

## 📂 Project Structure
- `main.cpp`: Entry point & Main Menu.
- `Doctor.h/cpp`: Doctor entity and management logic.
- `Patient.h/cpp`: Patient entity and personal records.
- `Appointment.h/cpp`: Scheduling logic and record linking.
- `Global.h`: Constants and utility functions for indexing.
- `CMakeLists.txt`: Build configuration.

## 💻 Installation & Usage
1. Clone the repo:
   ```bash
   git clone [https://github.com/arwamostafa0/Hospital-Management-System.git](https://github.com/arwamostafa0/Hospital-Management-System.git)
