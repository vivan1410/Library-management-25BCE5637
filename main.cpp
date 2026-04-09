#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

std::string getBrowserInput() {
#ifdef __EMSCRIPTEN__
    EM_ASM({
        window.inputReady = false;
        var inputRow = document.getElementById("terminal-input-row");
        if (inputRow) {
            inputRow.style.display = "flex";
            var inputField = document.getElementById("term-input");
            if(inputField) inputField.focus();
        }
        var wrapper = document.getElementById("output-wrapper");
        if (wrapper) wrapper.scrollTop = wrapper.scrollHeight;
    });

    while (true) {
        int ready = EM_ASM_INT({ return window.inputReady ? 1 : 0; });
        if (ready) break;
        emscripten_sleep(50);
    }

    char* result = (char*)EM_ASM_INT({
        var str = window.inputValue || "";
        window.inputValue = "";
        var inputRow = document.getElementById("terminal-input-row");
        if(inputRow) inputRow.style.display = "none";
        var inputField = document.getElementById("term-input");
        if(inputField) inputField.value = "";
        var lengthBytes = lengthBytesUTF8(str) + 1;
        var ptr = _malloc(lengthBytes);
        stringToUTF8(str, ptr, lengthBytes);
        return ptr;
    });

    std::string res(result);
    free(result);
    return res;
#else
    std::string s;
    std::getline(std::cin, s);
    return s;
#endif
}

class Book {
private:
    string BookID;
    string Title;
    string Author;
    string Status;
    string IssuedToRegNo;

public:
    Book() : Status("Available"), IssuedToRegNo("") {}

    Book(string id, string title, string author, string status = "Available", string regNo = "")
        : BookID(id), Title(title), Author(author), Status(status), IssuedToRegNo(regNo) {}

    string getBookID() const { return BookID; }
    string getTitle() const { return Title; }
    string getAuthor() const { return Author; }
    string getStatus() const { return Status; }
    string getIssuedToRegNo() const { return IssuedToRegNo; }

    bool issueBook(const string& regNo) {
        if (Status == "Available") {
            Status = "Issued";
            IssuedToRegNo = regNo;
            return true;
        }
        return false;
    }

    bool returnBook() {
        if (Status == "Issued") {
            Status = "Available";
            IssuedToRegNo = "";
            return true;
        }
        return false;
    }

    string serialize() const {
        return BookID + "|" + Title + "|" + Author + "|" + Status + "|" + IssuedToRegNo;
    }

    static Book deserialize(const string& line) {
        stringstream ss(line);
        string id, title, author, status, regno;
        getline(ss, id, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, status, '|');
        getline(ss, regno);
        return Book(id, title, author, status, regno);
    }

    void display() const {
        cout << "ID: " << BookID << " | Title: " << Title << " | Author: " << Author
             << " | Status: " << Status;
        if (Status == "Issued") {
            cout << " (Issued to: " << IssuedToRegNo << ")";
        }
        cout << "\n";
    }
};

class Library {
private:
    vector<Book> books;
    string dataFile;

    bool bookExists(const string& id) const {
        for (const auto& book : books) {
            if (book.getBookID() == id) return true;
        }
        return false;
    }

    static string toLower(const string& str) {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

public:
    Library(string file) : dataFile(file) { loadFromFile(); }

    ~Library() { saveToFile(); }

    void loadFromFile() {
        ifstream file(dataFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (!line.empty()) books.push_back(Book::deserialize(line));
            }
            file.close();
        }
    }

    void saveToFile() const {
        ofstream file(dataFile);
        if (file.is_open()) {
            for (const auto& book : books) file << book.serialize() << "\n";
            file.close();
        } else {
            cout << "Error opening file for saving data.\n";
        }
    }

    void addBook(const string& id, const string& title, const string& author) {
        if (id.empty() || title.empty()) {
            cout << "Error: Book ID and Title cannot be empty.\n";
            return;
        }
        if (bookExists(id)) {
            cout << "Error: A book with ID '" << id << "' already exists.\n";
            return;
        }
        books.push_back(Book(id, title, author));
        cout << "Book added successfully.\n";
    }

    void issueBook(const string& id, const string& regNo) {
        for (auto& book : books) {
            if (book.getBookID() == id) {
                if (book.issueBook(regNo)) {
                    cout << "Success: Book issued to " << regNo << ".\n";
                } else {
                    cout << "Error: Book is already issued.\n";
                }
                return;
            }
        }
        cout << "Error: Book ID '" << id << "' not found.\n";
    }

    void returnBook(const string& id) {
        for (auto& book : books) {
            if (book.getBookID() == id) {
                if (book.returnBook()) {
                    cout << "Success: Book returned.\n";
                } else {
                    cout << "Error: Book is not currently issued.\n";
                }
                return;
            }
        }
        cout << "Error: Book ID '" << id << "' not found.\n";
    }

    void searchBook(const string& query) const {
        bool found = false;
        string lowerQuery = toLower(query);
        for (const auto& book : books) {
            string lowerID = toLower(book.getBookID());
            string lowerTitle = toLower(book.getTitle());
            if (lowerID == lowerQuery || lowerTitle.find(lowerQuery) != string::npos) {
                book.display();
                found = true;
            }
        }
        if (!found) cout << "No matching books found for query: " << query << "\n";
    }

    void showIssuedBooks() const {
        bool found = false;
        for (const auto& book : books) {
            if (book.getStatus() == "Issued") {
                book.display();
                found = true;
            }
        }
        if (!found) cout << "No books are currently issued.\n";
    }

    void showCount() const {
        int available = 0, issued = 0;
        for (const auto& book : books) {
            if (book.getStatus() == "Available") available++;
            else issued++;
        }
        cout << "--- Library Status ---\n";
        cout << "Available Books : " << available << "\n";
        cout << "Issued Books    : " << issued << "\n";
        cout << "Total Books     : " << books.size() << "\n";
        cout << "----------------------\n";
    }
};

void printMenu() {
    cout << "\n============================================\n";
    cout << "  Library Management - Issue/Return with Rules\n";
    cout << "============================================\n";
    cout << "1. Add Book\n";
    cout << "2. Issue Book\n";
    cout << "3. Return Book\n";
    cout << "4. Search by BookID or Title\n";
    cout << "5. Show all issued books\n";
    cout << "6. Show count of available vs issued books\n";
    cout << "7. Exit\n";
    cout << "Enter your choice:\n";
}

int main() {
    Library lib("library_data.txt");
    int choice;

    while (true) {
        printMenu();
        string inputStr = getBrowserInput();
        stringstream ss(inputStr);

        if (!(ss >> choice)) {
            cout << "Error: Invalid input. Please enter a number between 1 and 7.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string id, title, author;
                cout << "Enter Book ID:\n";
                id = getBrowserInput();
                cout << "Enter Title:\n";
                title = getBrowserInput();
                cout << "Enter Author:\n";
                author = getBrowserInput();
                lib.addBook(id, title, author);
                break;
            }
            case 2: {
                string id, regNo;
                cout << "Enter Book ID to issue:\n";
                id = getBrowserInput();
                cout << "Enter Student RegNo:\n";
                regNo = getBrowserInput();
                lib.issueBook(id, regNo);
                break;
            }
            case 3: {
                string id;
                cout << "Enter Book ID to return:\n";
                id = getBrowserInput();
                lib.returnBook(id);
                break;
            }
            case 4: {
                string query;
                cout << "Enter Book ID or Title Keyword to search:\n";
                query = getBrowserInput();
                lib.searchBook(query);
                break;
            }
            case 5:
                lib.showIssuedBooks();
                break;
            case 6:
                lib.showCount();
                break;
            case 7:
                cout << "Saving data to file & exiting the system. Goodbye!\n";
                return 0;
            default:
                cout << "Error: Invalid choice. Try again.\n";
        }
    }
    return 0;
}