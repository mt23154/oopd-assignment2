#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <chrono>
#include <climits>
#include <iomanip>
class LibraryItem
{
protected:
    std::string identifier;

public:
    LibraryItem(const std::string &id) : identifier(id) {}
    virtual void displayInfo() const = 0;
    virtual std::string getIdentifier() const
    {
        return identifier;
    }
};

class PhysicalItem : public LibraryItem
{
protected:
    std::string location;
    std::string returnDuration;

public:
    PhysicalItem(const std::string &id, const std::string &loc, const std::string &duration)
        : LibraryItem(id), location(loc), returnDuration(duration) {}
    virtual void displayInfo() const override
    {
        std::cout << "Identifier: " << identifier << ", Location: " << location << ", Return Duration: " << returnDuration << "\n";
    }

    std::string getIdentifier() const override
    {
        return identifier;
    }
};

class ElectronicItem : public LibraryItem
{
protected:
    std::string accessLink;

public:
    ElectronicItem(const std::string &id, const std::string &link)
        : LibraryItem(id), accessLink(link) {}
    virtual void displayInfo() const override
    {
        std::cout << "Identifier: " << identifier << ", Access Link: " << accessLink << "\n";
    }

    std::string getIdentifier() const override
    {
        return identifier;
    }
};

class Book : public PhysicalItem
{
private:
    int count;
    std::string isbn;
    std::string authors;
    std::string title;

public:
    Book(const std::string &id, const std::string &loc, const std::string &duration,
         int cnt, const std::string &isbnVal, const std::string &auth, const std::string &titleVal)
        : PhysicalItem(id, loc, duration), count(cnt), isbn(isbnVal), authors(auth), title(titleVal) {}

    virtual void displayInfo() const override
    {
        PhysicalItem::displayInfo();
        std::cout << "Type: Book, Count: " << count << " ISBN: " << isbn << " Authors: " << authors << " Title: " << title << "\n";
    }

    std::string getIdentifier() const override
    {
        return identifier;
    }
};

class Magazine : public PhysicalItem
{
private:
    std::string publication;

public:
    Magazine(const std::string &id, const std::string &loc, const std::string &duration, const std::string &pub)
        : PhysicalItem(id, loc, duration), publication(pub) {}

    virtual void displayInfo() const override
    {
        std::cout << "Identifier: " << getPublication() << ", Location: " << getLocation() << ", Return Duration: " << getReturnDuration() << "\n";
    }

    std::string getPublication() const
    {
        return publication;
    }

    std::string getLocation() const
    {
        return location;
    }

    std::string getReturnDuration() const
    {
        return returnDuration;
    }
};

class Journal : public PhysicalItem
{
private:
    std::string journalName;

public:
    Journal(const std::string &id, const std::string &loc, const std::string &duration, const std::string &name)
        : PhysicalItem(id, loc, duration), journalName(name) {}

    virtual void displayInfo() const override
    {
        std::cout << "Identifier: " << journalName << ", Location: " << location << ", Return Duration: " << returnDuration << "\n";
    }

    std::string getIdentifier() const override
    {
        return identifier;
    }
};

class User
{
private:
    std::string username;
    std::map<std::string, std::chrono::system_clock::time_point> borrowedItems;
    bool isStudent;  // Added to store user type

public:
    User(const std::string &name) : username(name), isStudent(true) {}  // Default to student

    void setUserType(bool student) {
        isStudent = student;
    }

    void borrowItem(const std::string &itemIdentifier)
    {
        auto currentTime = std::chrono::system_clock::now();

        // Calculate the return duration based on the user type
        std::chrono::duration<int> returnDuration;
        if (isStudent)
            returnDuration = std::chrono::hours(24 * 30);  // Student: 30 days
        else
            returnDuration = std::chrono::hours(24 * 30 * 6);  // Faculty: 6 months

        auto returnTime = currentTime + returnDuration;

        borrowedItems[itemIdentifier] = returnTime;
    }

  std::string formatTime(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void displayBorrowedItems(const std::vector<Book> &books, const std::vector<Magazine> &magazines, const std::vector<Journal> &journals) const
{
    std::cout << "Borrowed Items for User " << username << ":\n";

    if (borrowedItems.empty()) {
        std::cout << "No items currently borrowed.\n";
        return;
    }

    for (const auto &borrowedItem : borrowedItems)
    {
        const std::string &itemIdentifier = borrowedItem.first;
        auto borrowedDate = std::chrono::system_clock::to_time_t(borrowedItem.second);

        std::chrono::system_clock::time_point returnTime = borrowedItem.second;
        if (isStudent)
            returnTime += std::chrono::hours(24 * 30 * 1);  // Adjusted return duration for students
        else
            returnTime += std::chrono::hours(24 * 30 * 6);  // Adjusted return duration for faculty

        auto returnDate = std::chrono::system_clock::to_time_t(returnTime);

        bool found = false;

        // Check if the item is a book
        for (const auto &book : books)
        {
            if (book.getIdentifier() == itemIdentifier)
            {
                std::cout << "Borrowed from Books:\n";
                book.displayInfo();
                std::cout << "Borrowed Date: " << std::ctime(&borrowedDate);
                std::cout << "Return Date: " << std::ctime(&returnDate);
                found = true;
                break;
            }
        }

        // Check if the item is a magazine
        for (const auto &magazine : magazines)
        {
            if (magazine.getIdentifier() == itemIdentifier)
            {
                std::cout << "Borrowed from Magazines:\n";
                magazine.displayInfo();
                std::cout << "Borrowed Date: " << std::ctime(&borrowedDate);
                std::cout << "Return Date: " << std::ctime(&returnDate);
                found = true;
                break;
            }
        }

        // Check if the item is a journal
        for (const auto &journal : journals)
        {
            if (journal.getIdentifier() == itemIdentifier)
            {
                std::cout << "Borrowed from Journals:\n";
                journal.displayInfo();
                std::cout << "Borrowed Date: " << std::ctime(&borrowedDate);
                std::cout << "Return Date: " << std::ctime(&returnDate);
                found = true;
                break;
            }
        }

        if (!found)
            std::cout << "Item not found.\n";
    }
}


};


void readBooksCSV(const std::string &filename, std::vector<Book> &books)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;

        std::istringstream iss(line);
        std::string token;

        int count;
        std::string isbn, authors, title;

        if (std::getline(iss, token, ','))
        {
            try
            {
                count = std::stoi(token);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Invalid count at line " << lineNum << ": " << token << "\n";
                continue;
            }
        }
        else
        {
            std::cerr << "Invalid line format at line " << lineNum << "\n";
            continue;
        }

        if (std::getline(iss, isbn, ','))
        {
            if (!isbn.empty() && isbn.front() == '"' && isbn.back() == '"')
                isbn = isbn.substr(1, isbn.length() - 2);
        }

        if (std::getline(iss, authors, ','))
        {
            if (!authors.empty() && authors.front() == '"' && authors.back() == '"')
                authors = authors.substr(1, authors.length() - 2);
        }

        if (std::getline(iss, title, ','))
        {
            if (!title.empty() && title.front() == '"' && title.back() == '"')
                title = title.substr(1, title.length() - 2);
        }

        std::string loc = "Unknown location";
        std::string duration = "Unknown duration";
        books.emplace_back(isbn, loc, duration, count, isbn, authors, title);
    }

    file.close();
}

void readMagazinesCSV(const std::string &filename, std::vector<Magazine> &magazines)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string publication = line;

        std::string loc = "Unknown location";
        std::string duration = "Unknown duration";

        magazines.emplace_back(publication, loc, duration, publication);
    }

    file.close();
}

void readJournalsCSV(const std::string &filename, std::vector<Journal> &journals)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string journalName = line;

        std::string loc = "Unknown location";
        std::string duration = "Unknown duration";

        journals.emplace_back(journalName, loc, duration, journalName);
    }

    file.close();
}

class BookStore
{
public:
    void purchaseNewBook(std::vector<Book> &books)
    {
        std::string isbn, authors, title, location, returnDuration;
        int count;

        std::cout << "Enter ISBN: ";
        std::cin >> isbn;

        std::cout << "Enter authors: ";
        std::cin.ignore();
        std::getline(std::cin, authors);

        std::cout << "Enter title: ";
        std::getline(std::cin, title);

        std::cout << "Enter location: ";
        std::getline(std::cin, location);

        std::cout << "Enter return duration: ";
        std::getline(std::cin, returnDuration);

        std::cout << "Enter count: ";
        std::cin >> count;

        Book newBook(isbn, location, returnDuration, count, isbn, authors, title);
        books.push_back(newBook);

        std::cout << "Book purchased and added to the library.\n";
    }
};

int main()
{
    std::vector<Book> books;
    readBooksCSV("books.csv", books);

    std::vector<Magazine> magazines;
    readMagazinesCSV("magazines.csv", magazines);

    std::vector<Journal> journals;
    readJournalsCSV("journals.csv", journals);

    std::string username;
    std::cout << "Enter username: ";
    std::cin.ignore();
    std::getline(std::cin, username);

    int userType;  // 1 for student, 2 for faculty
    std::cout << "Enter user type (1 for student, 2 for faculty): ";
    std::cin >> userType;

    bool isStudent = (userType == 1);

    User user(username);

    BookStore bookStore;

    int choice;
    do
    {
        std::cout << "Menu:\n";
        std::cout << "1. Borrow an item\n";
        std::cout << "2. Display borrowed items\n";
        std::cout << "3. Register a new user\n";
        std::cout << "4. Purchase a new book\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
{
    std::string itemIdentifier;
    std::cin.ignore();
    std::cout << "Enter the item identifier to borrow: ";
    std::getline(std::cin, itemIdentifier);

    // Call borrowItem with the correct parameters
    user.borrowItem(itemIdentifier);  // Pass only the item identifier, not isStudent
    std::cout << "Successfully borrowed the item.\n";
    break;
}
       case 2:
    user.displayBorrowedItems(books, magazines, journals);
    break;

        case 3:
            std::cout << "User registered successfully.\n";
            break;
        case 4:
            bookStore.purchaseNewBook(books);
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    } while (choice != 5);

    return 0;
}

