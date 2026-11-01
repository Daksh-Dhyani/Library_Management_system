# Library_Management_system

An automated academic resource circulation and inventory management console application implemented in modern C++ (C++17). The project models real-world library operations—such as multi-tier catalog management, dynamic loan windows, member quota enforcement, and overdue fine calculations—using the core principles of Object-Oriented Programming (OOP) alongside the C++ Standard Template Library (STL) and Resource Acquisition Is Initialization (RAII) memory safety.

---

## 📌 Project Overview

Traditional library circulation systems that rely on manual registers or flat procedural scripts frequently encounter operational issues: untracked concurrent loans, inconsistent borrowing limits, manual calculation errors in assessing late tariffs, and unhandled runtime exceptions[cite: 1, 3].

This system automates circulation desk workflows using a structured, type-safe OOP architecture[cite: 1, 3]:
* **Dual-Category Catalog:** Manages **Books** and serialized **Magazines** with distinct circulation rules.
* **Tiered Member Roles:** Distinguishes between **Students** and **Faculty**, enforcing specific borrowing limits and fine subsidies.
* **Automated Due Dates & Dynamic Fines:** Calculates loan periods and progressive late fees automatically via runtime polymorphism.
* **Defensive Exception Handling:** Isolates operational failures using a custom exception hierarchy derived from `std::exception`.
* **Memory Safety & RAII:** Uses modern STL containers (`std::vector`) and smart pointers (`std::unique_ptr`) to eliminate raw pointer leaks and object slicing.

---

## 🏛️ System Architecture & ASCII UML Class Diagram

```text
               +----------------------------------------------------+
               |              <<Interface>> Borrowable              |
               +----------------------------------------------------+
               | + isAvailable() : bool = 0                         |
               | + borrow() : void = 0                              |
               | + giveBack() : void = 0                            |
               +----------------------------------------------------+
                                         ^
                                         | (implements)
               +----------------------------------------------------+
               |           <<Abstract Base>> LibraryItem            |
               +----------------------------------------------------+
               | - id: string, title: string, available: bool       |
               +----------------------------------------------------+
               | + getLoanPeriodDays() : int = 0                    |
               | + calculateFine(daysLate: long) : double = 0       |
               | + getType() : string = 0, getDetails() : string = 0|
               +----------------------------------------------------+
                         ^                               ^
                         |                               |
       +-------------------------------+   +-------------------------------+
       |             Book              |   |           Magazine            |
       +-------------------------------+   +-------------------------------+
       | - author: string, isbn: string|   | - issueNumber: int            |
       | + getLoanPeriodDays() = 14    |   | + getLoanPeriodDays() = 7     |
       | + calculateFine() = late * 2.0|   | + calculateFine() = late * 1.0|
       +-------------------------------+   +-------------------------------+

               +----------------------------------------------------+
               |              <<Abstract Base>> Member              |
               +----------------------------------------------------+
               | # id: string, name: string                         |
               | # activeLoans: vector<Transaction>                 |
               +----------------------------------------------------+
               | + getMaxItems() : int = 0                          |
               | + getFineDiscount() : double = 0                   |
               +----------------------------------------------------+
                         ^                               ^
                         |                               |
       +-------------------------------+   +-------------------------------+
       |            Student            |   |            Faculty            |
       +-------------------------------+   +-------------------------------+
       | + getMaxItems() = 3           |   | + getMaxItems() = 5           |
       | + getFineDiscount() = 0.0 (0%)|   | + getFineDiscount() = 0.5(50%)|
       +-------------------------------+   +-------------------------------+
