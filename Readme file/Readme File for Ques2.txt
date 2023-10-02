Readme File for question 2.
-> first  we Include Libraries: C++ standard libraries for input/output, file handling, string manipulation, data structures (like vectors and maps), time handling.

-> then we define the Class:
  - This part defines several classes and their member functions:
  - LibraryItem: Abstract base class representing a library item.
  - PhysicalItem: Derived from LibraryItem, representing physical items like books and magazines.
  - ElectronicItem: Derived from LibraryItem, representing electronic items.
  - Book, Magazine, and Journal: Derived from PhysicalItem, representing specific types of physical items (books, magazines, and journals).
  - LoanableItem: Derived from PhysicalItem, representing items that can be borrowedLoanableItem: Derived from PhysicalItem, representing              item that can be borrowed.

-> after that we define User Class:
   This class represents a user of the library. It has functions to borrow items, display borrowed items, and manage user information.

-> then we define File Reading Functions:
   Functions (readBooksCSV, readMagazinesCSV, readJournalsCSV) to read data from CSV files.

-> after that we define BookStore Class:
   Represents a store to purchase new books. It has a function to add a new book to the library.

-> in last we define Main Function:
   Initializes vectors to store books, magazines, and journals by reading data from CSV files.
Creates a User instance and a BookStore instance.
Presents a menu to the user with various options (borrowing, displaying borrowed items, registering a new user, purchasing a new book, and exiting).
Performs actions based on the user's choice.


References : Chat gpt.