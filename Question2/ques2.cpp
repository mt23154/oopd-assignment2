#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <chrono>
#include <climits>



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

class LoanableItem : public PhysicalItem
{
private:
    bool isOnLoan;
    std::chrono::system_clock::time_point returnDate;

public:
    LoanableItem(const std::string &id, const std::string &loc, const std::string &duration)
        : PhysicalItem(id, loc, duration), isOnLoan(false) {}

    bool canBeBorrowed() const
    {
        return !isOnLoan;
    }

    void borrow()
    {
        isOnLoan = true;
        returnDate = std::chrono::system_clock::now() + std::chrono::hours(168); // 7 days later
    }

    void returnItem()
    {
        isOnLoan = false;
    }

    bool isOnLoanStatus() const
    {
        return isOnLoan;
    }

    std::chrono::system_clock::time_point getReturnDate() const
    {
        return returnDate;
    }

    virtual void displayInfo() const override
    {
        PhysicalItem::displayInfo();
        std::cout << "Item is " << (isOnLoan ? "on loan until " : "available for borrowing") << std::chrono::system_clock::to_time_t(returnDate) << "\n";
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

public:
    User(const std::string &name) : username(name) {}

    void borrowItem(const std::string &itemIdentifier, bool isLoanable = false)
    {
        if (isLoanable)
        {

            borrowedItems[itemIdentifier] = std::chrono::system_clock::now();
        }
        else
        {

            borrowedItems[itemIdentifier] = std::chrono::system_clock::now();
        }
    }

    void displayBorrowedItems(const std::vector<Book> &books, const std::vector<Magazine> &magazines, const std::vector<Journal> &journals) const
    {
        std::cout << "Borrowed Items for User " << username << ":\n";
        for (const auto &borrowedItem : borrowedItems)
        {
            const std::string &itemIdentifier = borrowedItem.first;

            std::cout << "Item Identifier: " << itemIdentifier << ", Borrowed Date: " << std::chrono::system_clock::to_time_t(borrowedItem.second) << ", Details:\n";

            bool found = false;

            for (const auto &book : books)
            {
                if (book.getIdentifier() == itemIdentifier)
                {
                    book.displayInfo();
                    found = true;
                    break;
                }
            }

            for (const auto &magazine : magazines)
            {
                if (magazine.getIdentifier() == itemIdentifier)
                {
                    magazine.displayInfo();
                    found = true;
                    break;
                }
            }

            for (const auto &journal : journals)
            {
                if (journal.getIdentifier() == itemIdentifier)
                {
                    journal.displayInfo();
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

    User user("Ajay");

    BookStore bookStore;

    int choice;
    do
    {
        std::cout << "Menu:\n";
        std::cout << "1. Borrow a regular item\n";
        std::cout << "2. Borrow an item on loan\n";
        std::cout << "3. Display borrowed items\n";
        std::cout << "4. Register a new user\n";
        std::cout << "5. Purchase a new book\n";
        std::cout << "6. Exit\n";
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

            bool itemFound = false;
            for (const auto &book : books)
            {
                if (book.getIdentifier() == itemIdentifier)
                {
                    user.borrowItem(book.getIdentifier());
                    std::cout << "Successfully borrowed a book.\n";
                    itemFound = true;
                    break;
                }
            }

            for (const auto &magazine : magazines)
            {
                if (magazine.getIdentifier() == itemIdentifier)
                {
                    user.borrowItem(magazine.getIdentifier());
                    std::cout << "Successfully borrowed a magazine.\n";
                    itemFound = true;
                    break;
                }
            }

            for (const auto &journal : journals)
            {
                if (journal.getIdentifier() == itemIdentifier)
                {
                    user.borrowItem(journal.getIdentifier());
                    std::cout << "Successfully borrowed a journal.\n";
                    itemFound = true;
                    break;
                }
            }

            if (!itemFound)
            {
                std::cout << "Item not found.\n";
            }
        }
        break;

        case 2:
        {
            std::string itemIdentifier;
            std::cin.ignore();
            std::cout << "Enter the item identifier to borrow on loan: ";
            std::getline(std::cin, itemIdentifier);

            user.borrowItem(itemIdentifier, true);
            std::cout << "Successfully borrowed the item on loan.\n";
        }
        break;

        case 3:
            user.displayBorrowedItems(books, magazines, journals);
            break;

        case 4:
        {
            std::string username;
            std::cout << "Enter username: ";
            std::cin.ignore();
            std::getline(std::cin, username);

            User newUser(username);

            std::cout << "User registered successfully.\n";
        }
        break;

        case 5:
            bookStore.purchaseNewBook(books);
            break;

        case 6:
            std::cout << "Exiting the program. Goodbye!\n";
            break;

        default:
            std::cout << "Invalid choice. Try again.\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    } while (choice != 6);

    return 0;
}
