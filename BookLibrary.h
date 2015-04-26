#include <iostream>
#include <vector>
#include <json/json.h>

#ifndef BOOKLIBRARY_H
#define BOOKLIBRARY_H

using namespace std;

struct BookNode{
    /*Red-Black Variables*/
    BookNode *leftChild;
    BookNode *rightChild;
    BookNode *parent;
    bool isRed;

    /*Other Variables*/
    string title;
    string author;
    string category;
    string ISBN10;
    string ISBN13;

};

struct BufferStruct{
    char* buffer;
    size_t size;
};

class BookLibrary
{
    private:
        /*Private Variables*/
        BookNode *root;
        BookNode *nil;
        vector<BookNode> searchResults;
        string libraryName;
        vector<string> bookCategories;
        int bookCounter;
        int saveCounter;
        json_object *LibraryOutput;

        /*Private Methods*/
        void insertNode(BookNode*); //Insert a new node into RB tree
        void rbAddFixup(BookNode*); //Call after insert to restore RB properties to tree
        void leftRotate(BookNode*); //Rotates RB tree left around a node
        void rightRotate(BookNode*); //Rotates RB tree right around a node
        void searchBookTitle(BookNode*,string); //Search for all books matching title
        void searchBookKeyword(BookNode*,string); //Search for all books including title keyword
        void searchBookAuthor(BookNode*,string); //Search for a book by author's last name
        void searchBookCategory(BookNode*,string); //Search for a book given a list of categories
        void deleteTree(BookNode*);
        void printLibrary(BookNode*); //Print all titles in the library, recursive call
        void saveLibrary(BookNode*); //Save library to file, recursive call

        static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);

    public:

        /*Public Methods*/
        BookLibrary();
        virtual ~BookLibrary();
        void createLibrary();
        void openLibrary();
        void renameLibrary();
        void deleteLibrary();
        bool enterISBN();
        bool enterManual();
        void searchBook();
        void printLibrary(); //Print all titles in the library, initial call
        void countBooks();
        bool saveLibrary(); //Save library to a file, initial call
        void clearScreen(); //Clears the screen

};

#endif // BOOKLIBRARY_H