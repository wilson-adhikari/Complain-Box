# 📨 Complaint Management System (Complain Box)

A secure and efficient **Complaint Management System** designed to handle the submission, tracking, and resolution of user complaints.  
The system supports **User** and **Admin** roles, enabling structured complaint handling with data encryption, analytics, and a clear workflow from submission to resolution.

---

## 🧩 Overview

This system allows users to **file complaints easily** while giving administrators **powerful tools** to view, manage, and resolve them.  
It includes features like **data encryption**, **search & filter**, **status tracking**, **backup system**, and **analytics dashboard** — all within a simple console-based interface.

---

## ⚙️ Core Features

| Category | Description |
|-----------|--------------|
| **User Panel** | Complaint submission, status viewing, and feedback management. |
| **Admin Panel** | Secure authentication, complaint viewing, filtering, resolution, and reporting. |
| **Data Security** | Encrypted file storage with backup and recovery system. |
| **Analytics** | Statistics, trend analysis, and resolution metrics. |
| **UI/UX** | Clean console interface with color-coded sections and alerts. |

---

## 🧠 System Architecture
complain-box/
│
├── main.cpp # Main program file
├── admin.cpp # Admin functions
├── user.cpp # User functions
├── complaint.h # Data structure and declarations
├── complaints_active.txt # Active complaint records (encrypted)
├── complaints_resolved.txt # Resolved complaint records
├── backup/ # Data backups
└── README.md # Project documentation

---

## 🧍‍♂️ USER MODULE

### 🎯 Purpose
Allows users to **submit, track, and review** complaints securely.  
Data is stored in encrypted format for privacy.

### 🧰 Functions & Purpose

| Function | Purpose | Features |
|-----------|----------|-----------|
| `userMenu()` | Main entry point for users. | Displays all user options. |
| `submitComplaint()` | Allows users to file a new complaint. | Takes complainant details, complaint type, and description. |
| `validateInput()` | Validates user inputs to prevent empty or invalid fields. | Ensures clean and structured data. |
| `viewMyComplaints()` | Shows user’s submitted complaints. | Displays status (Pending/In Progress/Resolved). |
| `searchMyComplaintByCode()` | Lets users find a specific complaint by unique code. | Fast retrieval using search code. |
| `editMyComplaint()` | Allows users to modify complaint details (if pending). | Edits before resolution process begins. |
| `cancelComplaint()` | Cancels a submitted complaint. | Removes it from the active complaint list. |

### 🧱 Data Collected from User

- Name  
- ID / Roll / Contact  
- Type (Student, Staff, Faculty, Stranger)  
- Complaint Type (Discipline, Maintenance, Harassment, etc.)  
- Description / Details  
- Date & Time  
- Priority Level (Normal / Urgent / Emergency)

### 🔒 Complaint Submission Example

Enter Name: Sarah Ahmed
Enter Type: Student
Enter Complaint Type: Maintenance
Enter Description: The lights in Lab 2 are not working.
Enter Priority: Normal
Complaint submitted successfully!
Complaint Code: CMP-0148


---

## 🛡️ ADMIN MODULE

### 🎯 Purpose
The admin side provides **complete complaint lifecycle management**, from authentication to final resolution and statistical analysis.

### 🧰 Functions & Purpose

#### Authentication & System Management
| Function | Purpose |
|-----------|----------|
| `adminLogin()` | Secure login with password masking and validation. |
| `run()` | Main control loop that loads complaints, displays menus, and routes functions. |
| `header()` | Displays stylized headers for consistent UI. |
| `clear()` / `pause()` | Improves readability and user control. |

#### Data Management & Encryption
| Function | Purpose |
|-----------|----------|
| `encrypt()` / `decrypt()` | Provides Caesar cipher–based encryption for data files. |
| `loadComplaints()` | Loads complaint records into memory. |
| `removeComplaintFromActive()` | Removes complaints after resolution or deletion. |
| `moveComplaintToResolved()` | Transfers a complaint from active to resolved list. |
| `updateComplaintStatus()` | Updates complaint status directly in the file. |
| `backupData()` | Creates timestamped backup copies for safety. |

#### Complaint Viewing & Filtering
| Function | Purpose |
|-----------|----------|
| `viewAllComplaints()` | Displays all active complaints with summary counts. |
| `viewResolvedComplaints()` | Shows archive of resolved complaints. |
| `viewEmergencyComplaints()` | Highlights urgent complaints for immediate action. |
| `searchComplaints()` | Multi-criteria search by name, type, code, or status. |
| `date_wise()` | Searches by date or date range. |

#### Complaint Resolution
| Function | Purpose |
|-----------|----------|
| `resolveComplaint()` | Marks complaints as resolved and transfers them. |
| `updateComplaintStatus()` | Updates statuses like *Pending → In Progress*. |
| `normalizeDate()` / `dateToSortable()` | Ensures consistent date format for sorting. |

#### Analytics & Reporting
| Function | Purpose |
|-----------|----------|
| `viewStatistics()` | Displays summary of complaint types, priorities, and resolution rates. |
| `buildTargetCountCache()` | Optimizes system by caching frequent offenders. |
| `colour_for()` | Assigns color codes based on severity and frequency. |

---

## 🧮 Key Features Summary

| Category | Highlights |
|-----------|-------------|
| **Security & Access Control** | Role-based access, masked passwords, encrypted data. |
| **Complaint Workflow** | End-to-end lifecycle (Submit → Track → Resolve → Archive). |
| **Data Handling** | Efficient I/O, file encryption, automated backups. |
| **UI/UX** | Color-coded console, clean menus, emergency alerts. |
| **Analytics** | Complaint trends, target frequency, and resolution metrics. |

---

## 🧱 Data Structure

### `struct Complaint`
Holds all data related to a complaint.

| Field | Description |
|--------|-------------|
| `code` | Unique complaint identifier. |
| `complainantName` | Name of the person filing the complaint. |
| `complainantType` | Role (Student/Staff/Faculty/Stranger). |
| `target` | Person or department the complaint is against. |
| `complaintType` | Category (Maintenance, Discipline, Harassment, etc.). |
| `priority` | Complaint urgency level. |
| `status` | Current state (Pending/In Progress/Resolved). |
| `description` | Details of the issue. |
| `date` | Submission date. |
| `repeatCount` | Number of similar past complaints against the same target. |

---

## 🧠 Data Encryption

The system uses a **Caesar Cipher**–based approach for file encryption and decryption:
- Each character is shifted by a fixed value before writing to file.
- Prevents direct reading or tampering with data files.

---

## 📊 Analytics Dashboard (Admin)

Displays:
- Total Complaints (Active, Resolved, Emergency)
- Complaint Distribution by Type
- Resolution Rate (%)
- Frequent Offenders (Repeat Targets)
- Average Handling Time

---

## 🔒 Security Measures

- Masked password input for admin login  
- Encrypted data files (Active & Resolved)  
- Backup system with timestamped archives  
- Role-based restricted access  
- Optional password-protected admin directory  

---

## 🧭 Workflow Summary

[User Submits Complaint]
↓
[Stored in Encrypted Active File]
↓
[Admin Reviews & Filters Complaint]
↓
[Admin Updates Status / Resolves Complaint]
↓
[Complaint Moves to Resolved Archive]
↓
[Analytics & Reports Updated]


---

## 🧰 Tools & Technologies

- **Language:** C++  
- **Paradigm:** Object-Oriented Programming (OOP)  
- **Core Concepts:** File Handling, Encryption, Data Structures  
- **Libraries Used:** `<iostream>`, `<fstream>`, `<vector>`, `<string>`, `<iomanip>`, `<ctime>`  

---

## ⚡ Setup & Execution

### 🪜 Steps to Run
1. Clone or download the project folder.  
2. Open the project in your IDE or terminal.  
3. Compile using:
4. Run the executable:


### 🔑 Default Admin Credentials
- **Username:** `admin`  
- **Password:** `12345`

*(Change in the source file for security before deployment.)*

---

## 📜 License

Released under the **MIT License**.  
You are free to use, modify, and distribute this software with proper attribution.

---

## 👨‍💻 Author

**Wilson Adhikari**  
🎓 CSE Engineer | 💻 Developer  
💡 “Every complaint is just an opportunity to build a better system.”
