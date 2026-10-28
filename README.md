
# 📚 Library Management System using OOP in C++

A console-based **Library Management System** developed in **C++** as a mini project for the Object-Oriented Programming (OOP) subject. The project demonstrates how OOP concepts can be used to manage library resources, members, book issuing, returns, and late fines.

## 📌 Project Overview

The Library Management System automates basic library operations through a menu-driven console application. It manages books and magazines, registers sample student and faculty members, tracks active loans, calculates late-return fines, and maintains transaction history during program execution.

This project is developed to demonstrate the practical implementation of Object-Oriented Programming concepts in C++.

## ✨ Features

- **Item Management:** Maintains a catalogue of books and magazines.
- **Member Management:** Supports Student and Faculty member types.
- **Issue Items:** Issues available library items to registered members.
- **Return Items:** Processes returns and calculates late fines.
- **Due Date Calculation:** Automatically calculates due dates according to item type.
- **Search Functionality:** Searches items using title or author/issue details.
- **Borrowing Limits:** Applies different borrowing limits for students and faculty.
- **Fine Calculation:** Calculates fines based on late days and member discounts.
- **Transaction History:** Displays issue and return records.
- **Exception Handling:** Handles invalid items, members, unavailable items, and borrowing limit violations.
- **Demo Mode:** Provides predefined scenarios for testing and demonstration.

## 🛠️ Technologies Used

- **Language:** C++
- **Standard:** C++17
- **Compiler:** GCC (g++)
- **Interface:** Console / Command Line
- **Libraries:** C++ Standard Library (STL)
- **IDE:** Visual Studio Code

## 🧠 OOP Concepts Implemented

This project demonstrates the following Object-Oriented Programming concepts:

| Concept | Implementation |
|---|---|
| Encapsulation | Data members are hidden inside classes |
| Abstraction | Abstract classes define common behaviour |
| Inheritance | Book and Magazine inherit from LibraryItem; Student and Faculty inherit from Member |
| Polymorphism | Virtual functions implement type-specific rules |
| Interface | Borrowable defines borrowing operations |
| Constructors | Initialize objects and their properties |
| Exception Handling | Custom exceptions handle errors |
| STL | Vector and unique_ptr manage collections and ownership |

### Class Design

- **Borrowable:** Abstract interface for borrowable items.
- **LibraryItem:** Abstract base class for library catalogue items.
- **Book:** Represents a book with author and ISBN information.
- **Magazine:** Represents a magazine with an issue number.
- **Member:** Abstract base class for library members.
- **Student:** Represents a student member.
- **Faculty:** Represents a faculty member.
- **Transaction:** Stores issue date, due date, item and member references.
- **Date:** Handles date calculations.
- **Library:** Controls catalogue operations, lending, returns, search and history.

## 📋 Library Rules

### Item Rules

| Item Type | Loan Period | Fine Per Late Day |
|---|---:|---:|
| Book | 14 days | ₹2 |
| Magazine | 7 days | ₹1 |

### Member Rules

| Member Type | Maximum Items | Fine Discount |
|---|---:|---:|
| Student | 3 | 0% |
| Faculty | 5 | 50% |

### Fine Calculation

**Fine = Days Late × Fine Per Day × (1 − Member Discount)**

A return on or before the due date has no fine.

Example:
- A student returns a book 6 days late: ₹12 fine.
- A faculty member returns the same book 6 days late: ₹6 fine.

## ⚙️ Installation and Setup

### Prerequisites

1. Install Visual Studio Code or another C++ editor.
2. Install a C++ compiler such as GCC (g++).
3. Ensure that `g++` is configured in your system PATH.

### Clone the Repository

```bash
git clone YOUR_REPOSITORY_URL
```

Navigate to the project folder:

```bash
cd LibraryManagementSystem
```

### Compile the Program

```bash
g++ -std=c++17 LibraryManagementSystem.cpp -o library
```

### Run the Application

**Interactive Menu:**

```bash
./library
```

On Windows:

```powershell
.\library.exe
```

**Demo Mode:**

```bash
./library demo
```

On Windows:

```powershell
.\library.exe demo
```

## 🖥️ Application Menu

The interactive menu provides the following operations:

1. View All Items
2. View Members
3. Issue Item
4. Return Item
5. Search
6. Transaction History
0. Exit

The program uses predefined sample items and members for demonstration.

## 🧪 Sample Data

### Books

- B101 – Java: The Complete Reference
- B102 – Head First Java
- B103 – Python Crash Course
- B104 – Data Science from Scratch

### Magazines

- M201 – Digit
- M202 – Data Quest

### Members

- S01 – Daksh (Student)
- S02 – Khushi (Student)
- F01 – Dr. Sharma (Faculty)

## 📊 Testing and Validation

The demo mode demonstrates the following scenarios:

- Displaying library items and members.
- Successfully issuing items.
- Rejecting an already-issued item.
- Handling unknown item IDs.
- Handling unknown member IDs.
- Enforcing borrowing limits.
- Searching catalogue items.
- Processing returns and calculating fines.
- Applying faculty fine discounts.
- Handling returns of items not borrowed by a member.
- Displaying transaction history and final item status.

## 📁 Project Structure

```text
LibraryManagementSystem/
│
├── LibraryManagementSystem.cpp
└── README.md
```

## ⚠️ Limitations

- The project uses predefined sample items and members.
- There is no member registration interface.
- There is no authentication system.
- Data is maintained in memory during execution.
- Transaction history is not permanently stored.
- There is no database integration.
- The application is console-based.

**Note:** The current implementation does not provide persistent storage between application runs.

## 🚀 Future Enhancements

- Add book and member registration.
- Implement permanent data storage using file handling or MySQL.
- Introduce login and administrator authentication.
- Add member-wise borrowing history.
- Add due-date reminders.
- Implement a graphical user interface.
- Generate library reports and statistics.

## 🎯 Learning Outcomes

Through this project, I learned how to:

- Apply OOP concepts to a real-world problem.
- Design class hierarchies using inheritance.
- Implement runtime polymorphism using virtual functions.
- Use abstract classes and interfaces.
- Handle exceptions using try-catch.
- Manage objects using STL containers and smart pointers.
- Implement date calculations and fine logic.

## 👨‍💻 Author

**Daksh**  
BCA (Data Science), 2nd Year  
Chandigarh University

## 🎓 Academic Project

**Subject:** Object-Oriented Programming (C++)  
**Project:** Library Management System  
**Project Type:** Mini Project

---

⭐ If you find this project useful, feel free to explore, learn from it, and improve it.
