// Library Management System - OOP Mini Project (C++)
// Concepts: Encapsulation, Abstraction, Inheritance, Polymorphism,
//           Interface (pure abstract class), Custom Exceptions, STL containers
//
// Compile      : g++ -std=c++17 LibraryManagementSystem.cpp -o library
// Run demo     : ./library demo
// Run menu mode: ./library

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ---------------------------------------------------------------
// Helper: simple Date class (no external libraries needed)
// ---------------------------------------------------------------
class Date {
    int year, month, day;

    // days since 1970-01-01
    static long toDays(int y, int m, int d) {
        y -= (m <= 2);
        long era = (y >= 0 ? y : y - 399) / 400;
        long yoe = y - era * 400;
        long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
        long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return era * 146097 + doe - 719468;
    }

    static Date fromDays(long z) {
        z += 719468;
        long era = (z >= 0 ? z : z - 146096) / 146097;
        long doe = z - era * 146097;
        long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        long y = yoe + era * 400;
        long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        long mp = (5 * doy + 2) / 153;
        int d = (int)(doy - (153 * mp + 2) / 5 + 1);
        int m = (int)(mp < 10 ? mp + 3 : mp - 9);
        return Date((int)(y + (m <= 2)), m, d);
    }

public:
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    static Date today() {
        time_t t = time(nullptr);
        tm* lt = localtime(&t);
        return Date(lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
    }

    Date plusDays(int n) const { return fromDays(toDays(year, month, day) + n); }

    long daysBetween(const Date& other) const {   // other - this
        return toDays(other.year, other.month, other.day) - toDays(year, month, day);
    }

    string toString() const {
        char buf[16];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);
        return string(buf);
    }
};

// ---------------------------------------------------------------
// 1. INTERFACE  (pure abstract class: "something that can be borrowed")
// ---------------------------------------------------------------
class Borrowable {
public:
    virtual bool isAvailable() const = 0;
    virtual void borrow() = 0;
    virtual void giveBack() = 0;
    virtual ~Borrowable() {}
};

// ---------------------------------------------------------------
// 2. CUSTOM EXCEPTIONS
// ---------------------------------------------------------------
class LibraryException : public exception {
    string message;
public:
    explicit LibraryException(const string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ItemNotFoundException : public LibraryException {
public:
    explicit ItemNotFoundException(const string& id)
        : LibraryException("No item found with ID: " + id) {}
};

class MemberNotFoundException : public LibraryException {
public:
    explicit MemberNotFoundException(const string& id)
        : LibraryException("No member found with ID: " + id) {}
};

class ItemNotAvailableException : public LibraryException {
public:
    explicit ItemNotAvailableException(const string& title)
        : LibraryException("'" + title + "' is already issued to someone else.") {}
};

class BorrowLimitExceededException : public LibraryException {
public:
    BorrowLimitExceededException(const string& name, int limit)
        : LibraryException(name + " has reached the borrowing limit of " +
                           to_string(limit) + " items.") {}
};

// ---------------------------------------------------------------
// 3. ABSTRACT CLASS + INHERITANCE  (library items)
// ---------------------------------------------------------------
class LibraryItem : public Borrowable {
    string id;            // private data -> ENCAPSULATION
    string title;
    bool available;

public:
    LibraryItem(const string& id, const string& title)
        : id(id), title(title), available(true) {}

    string getId() const    { return id; }
    string getTitle() const { return title; }

    bool isAvailable() const override { return available; }
    void borrow() override            { available = false; }
    void giveBack() override          { available = true; }

    // Each item type decides its own rules -> POLYMORPHISM
    virtual int getLoanPeriodDays() const = 0;
    virtual double calculateFine(long daysLate) const = 0;
    virtual string getType() const = 0;
    virtual string getDetails() const = 0;

    string toString() const {
        char buf[256];
        snprintf(buf, sizeof(buf), "%-6s %-10s %-34s %-22s %s",
                 id.c_str(), getType().c_str(), title.c_str(),
                 getDetails().c_str(), available ? "Available" : "Issued");
        return string(buf);
    }
};

class Book : public LibraryItem {
    string author;
    string isbn;
public:
    Book(const string& id, const string& title, const string& author, const string& isbn)
        : LibraryItem(id, title), author(author), isbn(isbn) {}

    int getLoanPeriodDays() const override          { return 14; }
    double calculateFine(long late) const override  { return late * 2.0; }   // Rs. 2 per day
    string getType() const override                 { return "Book"; }
    string getDetails() const override              { return author; }
    string getIsbn() const                          { return isbn; }
};

class Magazine : public LibraryItem {
    int issueNumber;
public:
    Magazine(const string& id, const string& title, int issueNumber)
        : LibraryItem(id, title), issueNumber(issueNumber) {}

    int getLoanPeriodDays() const override          { return 7; }
    double calculateFine(long late) const override  { return late * 1.0; }   // Rs. 1 per day
    string getType() const override                 { return "Magazine"; }
    string getDetails() const override              { return "Issue #" + to_string(issueNumber); }
};

// ---------------------------------------------------------------
// 4. TRANSACTION  (one issue record)
// ---------------------------------------------------------------
class Member;   // forward declaration

class Transaction {
    LibraryItem* item;
    Member* member;
    Date issueDate;
    Date dueDate;
public:
    Transaction(LibraryItem* item, Member* member, const Date& issueDate)
        : item(item), member(member), issueDate(issueDate),
          dueDate(issueDate.plusDays(item->getLoanPeriodDays())) {}

    LibraryItem* getItem() const { return item; }
    Member* getMember() const    { return member; }
    Date getIssueDate() const    { return issueDate; }
    Date getDueDate() const      { return dueDate; }
};

// ---------------------------------------------------------------
// 5. ABSTRACT CLASS + INHERITANCE  (members)
// ---------------------------------------------------------------
class Member {
    string id;
    string name;
    vector<Transaction> activeLoans;

public:
    Member(const string& id, const string& name) : id(id), name(name) {}
    virtual ~Member() {}

    string getId() const   { return id; }
    string getName() const { return name; }
    vector<Transaction>& getActiveLoans() { return activeLoans; }

    virtual int getMaxItems() const = 0;
    virtual double getFineDiscount() const = 0;   // 0.0 = none, 0.5 = 50 %
    virtual string getMemberType() const = 0;

    string toString() const {
        char buf[256];
        snprintf(buf, sizeof(buf), "%-6s %-18s %-9s Borrowed: %d/%d",
                 id.c_str(), name.c_str(), getMemberType().c_str(),
                 (int)activeLoans.size(), getMaxItems());
        return string(buf);
    }
};

class Student : public Member {
public:
    Student(const string& id, const string& name) : Member(id, name) {}
    int getMaxItems() const override         { return 3; }
    double getFineDiscount() const override  { return 0.0; }
    string getMemberType() const override    { return "Student"; }
};

class Faculty : public Member {
public:
    Faculty(const string& id, const string& name) : Member(id, name) {}
    int getMaxItems() const override         { return 5; }
    double getFineDiscount() const override  { return 0.5; }
    string getMemberType() const override    { return "Faculty"; }
};

// ---------------------------------------------------------------
// 6. LIBRARY  (core controller class)
// ---------------------------------------------------------------
class Library {
    vector<unique_ptr<LibraryItem>> items;
    vector<unique_ptr<Member>> members;
    vector<string> history;

    LibraryItem* findItem(const string& id) {
        for (auto& i : items)
            if (i->getId() == id) return i.get();
        throw ItemNotFoundException(id);
    }

    Member* findMember(const string& id) {
        for (auto& m : members)
            if (m->getId() == id) return m.get();
        throw MemberNotFoundException(id);
    }

    static string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
        return s;
    }

public:
    void addItem(unique_ptr<LibraryItem> item)   { items.push_back(move(item)); }
    void addMember(unique_ptr<Member> member)    { members.push_back(move(member)); }

    void issueItem(const string& itemId, const string& memberId, const Date& date) {
        LibraryItem* item = findItem(itemId);
        Member* member = findMember(memberId);

        if (!item->isAvailable())
            throw ItemNotAvailableException(item->getTitle());
        if ((int)member->getActiveLoans().size() >= member->getMaxItems())
            throw BorrowLimitExceededException(member->getName(), member->getMaxItems());

        item->borrow();
        Transaction t(item, member, date);
        member->getActiveLoans().push_back(t);

        char buf[400];
        snprintf(buf, sizeof(buf), "ISSUED   : '%s' -> %s | Issue: %s | Due: %s",
                 item->getTitle().c_str(), member->getName().c_str(),
                 date.toString().c_str(), t.getDueDate().toString().c_str());
        history.push_back(buf);
        cout << buf << endl;
    }

    double returnItem(const string& itemId, const string& memberId, const Date& date) {
        LibraryItem* item = findItem(itemId);
        Member* member = findMember(memberId);

        vector<Transaction>& loans = member->getActiveLoans();
        int index = -1;
        for (int k = 0; k < (int)loans.size(); k++) {
            if (loans[k].getItem()->getId() == itemId) { index = k; break; }
        }
        if (index == -1) {
            cout << "ERROR    : " << member->getName() << " has not borrowed '"
                 << item->getTitle() << "'." << endl;
            return 0;
        }

        long daysLate = max(0L, loans[index].getDueDate().daysBetween(date));
        double fine = item->calculateFine(daysLate) * (1 - member->getFineDiscount());

        item->giveBack();
        loans.erase(loans.begin() + index);

        char buf[400];
        snprintf(buf, sizeof(buf),
                 "RETURNED : '%s' <- %s | Returned: %s | Days late: %ld | Fine: Rs. %.2f",
                 item->getTitle().c_str(), member->getName().c_str(),
                 date.toString().c_str(), daysLate, fine);
        history.push_back(buf);
        cout << buf << endl;
        return fine;
    }

    void search(const string& keyword) {
        cout << "Search results for \"" << keyword << "\":" << endl;
        bool any = false;
        string key = toLower(keyword);
        for (auto& i : items) {
            if (toLower(i->getTitle()).find(key) != string::npos ||
                toLower(i->getDetails()).find(key) != string::npos) {
                cout << "  " << i->toString() << endl;
                any = true;
            }
        }
        if (!any) cout << "  (no matching items)" << endl;
    }

    void showAllItems() {
        char buf[200];
        snprintf(buf, sizeof(buf), "%-6s %-10s %-34s %-22s %s",
                 "ID", "Type", "Title", "Author/Issue", "Status");
        cout << buf << endl;
        cout << string(88, '-') << endl;
        for (auto& i : items) cout << i->toString() << endl;
    }

    void showAllMembers() {
        for (auto& m : members) cout << m->toString() << endl;
    }

    void showHistory() {
        if (history.empty()) cout << "No transactions yet." << endl;
        for (auto& h : history) cout << h << endl;
    }
};

// ---------------------------------------------------------------
// 7. MAIN  (demo mode + interactive menu)
// ---------------------------------------------------------------
Library createSampleLibrary() {
    Library lib;
    lib.addItem(make_unique<Book>("B101", "Java: The Complete Reference", "Herbert Schildt", "978-1260440232"));
    lib.addItem(make_unique<Book>("B102", "Head First Java", "Kathy Sierra", "978-1491910771"));
    lib.addItem(make_unique<Book>("B103", "Python Crash Course", "Eric Matthes", "978-1718502703"));
    lib.addItem(make_unique<Book>("B104", "Data Science from Scratch", "Joel Grus", "978-1492041139"));
    lib.addItem(make_unique<Magazine>("M201", "Digit", 202));
    lib.addItem(make_unique<Magazine>("M202", "Data Quest", 87));
    lib.addMember(make_unique<Student>("S01", "Daksh"));
    lib.addMember(make_unique<Student>("S02", "Khushi"));
    lib.addMember(make_unique<Faculty>("F01", "Dr. Sharma"));
    return lib;
}

void runDemo() {
    Library lib = createSampleLibrary();

    cout << "=== 1. ALL ITEMS ===" << endl;
    lib.showAllItems();

    cout << "\n=== 2. MEMBERS ===" << endl;
    lib.showAllMembers();

    cout << "\n=== 3. ISSUING ITEMS ===" << endl;
    Date d(2026, 10, 1);
    try {
        lib.issueItem("B101", "S01", d);
        lib.issueItem("M201", "S01", d);
        lib.issueItem("B102", "F01", d);
    } catch (const exception& e) {
        cout << "ERROR    : " << e.what() << endl;
    }

    cout << "\n=== 4. EXCEPTION HANDLING TESTS ===" << endl;
    try { lib.issueItem("B101", "S02", d); }
    catch (const exception& e) { cout << "ERROR    : " << e.what() << endl; }

    try { lib.issueItem("B999", "S02", d); }
    catch (const exception& e) { cout << "ERROR    : " << e.what() << endl; }

    try { lib.issueItem("B103", "S99", d); }
    catch (const exception& e) { cout << "ERROR    : " << e.what() << endl; }

    try {
        lib.issueItem("B103", "S01", d);   // 3rd item for Daksh (limit = 3, ok)
        lib.issueItem("B104", "S01", d);   // 4th item -> limit exceeded
    } catch (const exception& e) { cout << "ERROR    : " << e.what() << endl; }

    cout << "\n=== 5. SEARCH ===" << endl;
    lib.search("java");

    cout << "\n=== 6. RETURNS AND FINE CALCULATION (polymorphism) ===" << endl;
    try {
        // Book, 14 days allowed, 6 days late, Student pays full fine
        lib.returnItem("B101", "S01", Date(2026, 10, 21));
        // Magazine, 7 days allowed, 5 days late, Student pays full fine
        lib.returnItem("M201", "S01", Date(2026, 10, 13));
        // Book, Faculty, 6 days late -> 50 % discount
        lib.returnItem("B102", "F01", Date(2026, 10, 21));
        // On-time return, no fine
        lib.returnItem("B103", "S01", Date(2026, 10, 10));
        // Returning something never borrowed
        lib.returnItem("B104", "S02", Date(2026, 10, 10));
    } catch (const exception& e) {
        cout << "ERROR    : " << e.what() << endl;
    }

    cout << "\n=== 7. TRANSACTION HISTORY ===" << endl;
    lib.showHistory();

    cout << "\n=== 8. FINAL STATUS ===" << endl;
    lib.showAllItems();
}

void runMenu() {
    Library lib = createSampleLibrary();
    bool running = true;
    string choice, itemId, memberId, keyword;

    while (running) {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====" << endl;
        cout << "1. View all items" << endl;
        cout << "2. View members" << endl;
        cout << "3. Issue item" << endl;
        cout << "4. Return item" << endl;
        cout << "5. Search" << endl;
        cout << "6. Transaction history" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose an option: ";
        if (!getline(cin, choice)) break;

        try {
            if (choice == "1")      lib.showAllItems();
            else if (choice == "2") lib.showAllMembers();
            else if (choice == "3") {
                cout << "Item ID: ";   getline(cin, itemId);
                cout << "Member ID: "; getline(cin, memberId);
                lib.issueItem(itemId, memberId, Date::today());
            }
            else if (choice == "4") {
                cout << "Item ID: ";   getline(cin, itemId);
                cout << "Member ID: "; getline(cin, memberId);
                lib.returnItem(itemId, memberId, Date::today());
            }
            else if (choice == "5") {
                cout << "Keyword: "; getline(cin, keyword);
                lib.search(keyword);
            }
            else if (choice == "6") lib.showHistory();
            else if (choice == "0") { running = false; cout << "Goodbye!" << endl; }
            else cout << "Invalid option, try again." << endl;
        } catch (const exception& e) {
            cout << "ERROR: " << e.what() << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "demo") runDemo();
    else runMenu();
    return 0;
}