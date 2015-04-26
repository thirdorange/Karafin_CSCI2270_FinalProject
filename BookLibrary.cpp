#include <iostream>
#include <fstream>  //ofstream
#include <stdlib.h> //atoi
#include <string.h> //strcmp
#include <vector>
#include <json/json.h>

#include "BookLibrary.h"

#include "curl/curl.h"
#define URL_SIZE     256
#define BUFFER_SIZE  (10000000)

using namespace std;

/*PUBLIC METHODS*/

/*
Function prototype:
BookLibrary::BookLibrary()

Function description: This is the constructor for the BookLibrary class.

Pre-condition: None.
Post-condition: Counter for number of books is initialized, red-black tree is initialized, and a list of possible book categories
has been generated.
*/
BookLibrary::BookLibrary(){
    //Initialize book counter
    bookCounter = 0;
    //Initialize sentinel nil, and root
    nil = new BookNode;
    root = nil;
    root->parent = nil;
    nil->isRed = false;
    //Initialize book categories (must be even number for now)
    bookCategories.push_back("Adventure");
    bookCategories.push_back("Art");
    bookCategories.push_back("Biography");
    bookCategories.push_back("Children's");
    bookCategories.push_back("Cookbooks");
    bookCategories.push_back("Drama");
    bookCategories.push_back("Fantasy");
    bookCategories.push_back("History");
    bookCategories.push_back("Horror");
    bookCategories.push_back("Math");
    bookCategories.push_back("Mystery");
    bookCategories.push_back("Poetry");
    bookCategories.push_back("Reference");
    bookCategories.push_back("Romance");
    bookCategories.push_back("Sci-Fi");
    bookCategories.push_back("Science");
    bookCategories.push_back("Travel");
    bookCategories.push_back("Other");
}

/*
Function prototype:
BookLibrary::~BookLibrary(){

Function description: This is the destructor for the BookLibrary class. Calls the private method deleteTree to delete all nodes using a
postorder transversal.

Pre-condition: None, other than that a class instance has been created. Contains handling for empty tree.
Post-condition: All nodes in the red-black tree have been deleted.

*/
BookLibrary::~BookLibrary(){
    //dtor
    if(root != nil){
        deleteTree(root);
    }
    delete nil;
}

/*
Function prototype:
void BookLibrary::createLibrary()

Function description: This method captures the name of a new library when it is created, which is later used as the filename when the library is saved.

Pre-condition: None, other than that a class instance has been created.
Post-condition: Class variable libraryName has been set.
*/
void BookLibrary::createLibrary(){
    clearScreen();
    cout<<"CREATE A NEW LIBRARY"<<endl;
    cout<<"********************"<<endl;
    cout<<"Enter a name for your library."<<endl;
    getline(cin,libraryName);
    clearScreen();
}

/*
Function prototype:
void BookLibrary::openLibrary()

Function description: This method allows the user to open a library from a previously saved .json
file. If a matching filename is not found, the user has the option to either enter a new filename,
create a new library, or exit the program. If a file is matched, the JSON-formatted data are converted
to BookNode objects and added to the red-black tree.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If user selection results in a file being opened, a number of new BookNode objects equal
to the number in the .json file are created and added to the red-black tree, which is rebalanced after addition
of each object.
*/
void BookLibrary::openLibrary(){
    clearScreen();
    cout<<"OPEN AN EXISTING LIBRARY"<<endl;
    cout<<"************************"<<endl;
    cout<<"Enter the name of your library without extension."<<endl;
    getline(cin,libraryName);
    ifstream inFile;
    inFile.open(libraryName + ".json");
    //If file doesn't open, present user with other options
    if(inFile.fail()){
        int intUserInput = 0;
        while(intUserInput < 1 || intUserInput > 3){
            cout<<"Input file failed to open. What would you like to do?"<<endl;
            cout<<"1: Input a different library name"<<endl;
            cout<<"2: Create a new library"<<endl;
            cout<<"3: Exit"<<endl;
            cin>>intUserInput;
            cin.ignore(10000,'\n');
            if(intUserInput < 1 || intUserInput > 3){
                cout<<"Invalid Input"<<endl;
            }
        }
        if(intUserInput == 1){
            openLibrary();
        }
        else if(intUserInput == 2){
            createLibrary();
        }
        else{
            exit(0);
        }
    }
    //If file is opened, import JSON as new BookNode objects
    else{
        string strLine;
        while(!inFile.eof()){
            getline(inFile,strLine);
            //Ignore if first character is newline
            while(inFile.peek() == '\n'){
                inFile.get();
            }
            //Find book object in file
            int intVarLen = strLine.find("Book #");
            if(intVarLen != -1){
                //Initialize new book object
                BookNode *newnode = new BookNode;
                //Set book title
                getline(inFile,strLine);
                int intVarBegin = strLine.find(":") + 2;
                int intVarEnd = strLine.find(",") - 1;
                newnode->title = strLine.substr(intVarBegin,intVarEnd-intVarBegin);
                //Set book author
                getline(inFile,strLine);
                intVarBegin = strLine.find(":") + 2;
                intVarEnd = strLine.find(",") - 1;
                newnode->author = strLine.substr(intVarBegin,intVarEnd-intVarBegin);
                //Set book category
                getline(inFile,strLine);
                intVarBegin = strLine.find(":") + 2;
                intVarEnd = strLine.find(",") - 1;
                newnode->category = strLine.substr(intVarBegin,intVarEnd-intVarBegin);
                //Set book ISBN-10
                getline(inFile,strLine);
                if(strLine.find("\"\"") == -1){
                    intVarBegin = strLine.find(":") + 2;
                    newnode->ISBN10 = strLine.substr(intVarBegin,10);
                }
                else{
                    newnode->ISBN10 = "";
                }
                //Set book ISBN-13
                getline(inFile,strLine);
                if(strLine.find("\"\"") == -1){
                    intVarBegin = strLine.find(":") + 2;
                    newnode->ISBN13 = strLine.substr(intVarBegin,13);
                }
                else{
                    newnode->ISBN13 = "";
                }
                //Add the new node to the RB tree
                insertNode(newnode);
            }
        }
        inFile.close();
    }
}

/*
Function prototype:
void BookLibrary::renameLibrary()

Function description: This method allows the user to rename the .json file for an existing library in the same
directory as the executable file.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If the method is successful, the .json file has been renamed from its original name to one
specified by the user.
*/
void BookLibrary::renameLibrary(){
    clearScreen();
    cout<<"RENAME A LIBRARY FILE"<<endl;
    cout<<"************************"<<endl;
    cout<<"Enter the name of the existing library without extension."<<endl;
    getline(cin,libraryName);
    string newName;
    cout<<"Enter the new name of the library without extension."<<endl;
    getline(cin,newName);
    //Execute rename, print error if doesn't work
    if(rename((libraryName + ".json").c_str(),(newName + ".json").c_str()) != 0){
        cout<<"Error renaming file."<<endl;
    }
    else{
        cout<<"File successfully renamed."<<endl;
    }
}

/*
Function prototype:
void BookLibrary::deleteLibrary()

Function description: This method allows the user to delete a .json file for an existing library in the same
directory as the executable file.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If the method is successful, the .json file has been deleted.
*/
void BookLibrary::deleteLibrary(){
    clearScreen();
    cout<<"DELETE A LIBRARY FILE"<<endl;
    cout<<"************************"<<endl;
    cout<<"Enter the name of your library without extension."<<endl;
    getline(cin,libraryName);
    if(remove((libraryName + ".json").c_str()) != 0){
        cout<<"Error deleting file."<<endl;
    }
    else{
        cout<<"File successfully deleted."<<endl;
    }
}

/*
Function prototype:
void BookLibrary::enterISBN()

Function description: This method allows the user to input either an ISBN-10 or ISBN-13 for a book to be added
to the library. After the number is entered, libcurl submits the appropriate URL to Google Books and retrieves the
output, which is then cast as a string. If there is a match, the JSON data are interpreted and a new BookNode is added.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If a match for the ISBN is found, a new BookNode object is created and initialized. insertNode() is then
called to add the node to the red-black tree and rebalance as necessary. The function returns 1 if a node is added and 0 if
no node is added, which determines whether changes to the tree have been made and the user will be prompted to save before
exiting.
*/
bool BookLibrary::enterISBN(){
    clearScreen();
    bool newNodeMade = false;
    cout<<"ENTER BOOK BY ISBN"<<endl;
    cout<<"*****************"<<endl;
    cout<<"Enter an ISBN-10 or ISBN-13 number."<<endl;
    string inputISBN;
    getline(cin,inputISBN);
    string searchURL = "https://www.googleapis.com/books/v1/volumes?q=isbn:" + inputISBN;
    //Initialize curl and set options
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *myHandle;
    CURLcode result;
    struct BufferStruct output;
    output.buffer = NULL;
    output.size = 0;
    myHandle = curl_easy_init();
    curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, (void*)&output);
    curl_easy_setopt(myHandle, CURLOPT_URL, searchURL.c_str());
    //Execute curl command and clean up
    result = curl_easy_perform(myHandle);
    curl_easy_cleanup(myHandle);
    //Transfer data from the BufferStruct object to a string for further processing
    string strOutput = string(output.buffer);
    int intVarLen = strOutput.find("\"title\": \""); //Find title block, also makes sure a match is found
    if(intVarLen != -1){
        //Declare new node
        BookNode *newnode = new BookNode;
        //Set title
        strOutput.erase(0,intVarLen+10);
        intVarLen = strOutput.find("\",");
        newnode->title = strOutput.substr(0,intVarLen);
        //Find and set author (doesn't handle multiple authors)
        intVarLen = strOutput.find("\"authors\":");
        strOutput.erase(0,intVarLen+19);
        intVarLen = strOutput.find("\"");
        newnode->author = strOutput.substr(0,intVarLen);
        //Find and set ISBN-10 if exists
        intVarLen = strOutput.find("ISBN_10");
        if(intVarLen != -1){
            strOutput.erase(0,intVarLen+31);
            intVarLen = strOutput.find("\"");
            newnode->ISBN10 = strOutput.substr(0,intVarLen);
        }
        //Find and set ISBN-13 if exists
        intVarLen = strOutput.find("ISBN_13");
        if(intVarLen != -1){
            strOutput.erase(0,intVarLen+31);
            intVarLen = strOutput.find("\"");
            newnode->ISBN13 = strOutput.substr(0,intVarLen);
        }
        //Find and set category (doesn't handle multiple categories)
        intVarLen = strOutput.find("\"categories\":");
        strOutput.erase(0,intVarLen+22);
        intVarLen = strOutput.find("\"");
        bool categorymatch = false;
        for(unsigned int i = 0; i < bookCategories.size(); i++){
            if(strOutput.substr(0,intVarLen) == bookCategories[i]){
                categorymatch = true;
            }
        }
        if(categorymatch == true){
           newnode->category = strOutput.substr(0,intVarLen);
        }
        else{
            newnode->category = "Other";
        }
        newNodeMade = true;
        insertNode(newnode);
    }
    else{
        newNodeMade = false;
        cout<<"No matches found."<<endl;
    }

    if(output.buffer){
        free(output.buffer);
        output.buffer = 0;
        output.size = 0;
    }
    return newNodeMade;
}

/*
Function prototype:
bool BookLibrary::enterManual()

Function description: This method allows the user to input either an ISBN-10 or ISBN-13 for a book to be added
to the library. After the number is entered, libcurl submits the appropriate URL to Google Books and retrieves the
output, which is then cast as a string. If there is a match, the JSON data are interpreted and a new BookNode is added.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If a match for the ISBN is found, a new BookNode object is created and initialized. insertNode() is then
called to add the node to the red-black tree and rebalance as necessary. The function returns 1, which flags that changes
to the tree have been made, and the user will be prompted to save before exiting.
*/
bool BookLibrary::enterManual(){
    clearScreen();
    cout<<"MANUAL BOOK ENTRY"<<endl;
    cout<<"*****************"<<endl;
    //Create and initialize new BookNode
    BookNode *newnode = new BookNode;
    newnode->leftChild = nil;
    newnode->rightChild = nil;
    newnode->parent = nil;
    newnode->isRed = true;
    //Initialize variables for entry processing
    string yearEntry;
    int catEntry = 0;
    string ISBN10Entry;
    string ISBN13Entry;
    //Get input from user and set new BookNode variables
    cout<<"Enter the title:"<<endl;
    getline(cin,newnode->title);
    cout<<"Enter the author:"<<endl;
    getline(cin,newnode->author);
    while(catEntry < 1 || catEntry > 18){
        cout<<"Enter the category:"<<endl;
        for(unsigned int i = 0; i < bookCategories.size(); i++){
            cout<<"\t"<<i+1<<": "<<bookCategories[i];
            i++;
            cout<<"\t\t"<<i+1<<": "<<bookCategories[i]<<endl;

        }
        cin>>catEntry;
        cin.ignore(10000,'\n');
        if(catEntry < 1 || catEntry > 18){
            cout<<"Not a valid entry."<<endl;
        }
    }
    newnode->category = bookCategories[catEntry-1];
    cout<<"Enter the ISBN-10 if available, or 'None':"<<endl;
    getline(cin,ISBN10Entry);
    if(ISBN10Entry == "None"){
        newnode->ISBN10 = "";
    }
    else{
        newnode->ISBN10 = ISBN10Entry;
    }
    cout<<"Enter the ISBN-13 if available, or 'None':"<<endl;
    getline(cin,ISBN13Entry);
    if(ISBN13Entry == "None"){
        newnode->ISBN13 = "";
    }
    else{
        newnode->ISBN13 = ISBN13Entry;
    }
    insertNode(newnode);
    return 1;
}

/*
Function prototype:
void BookLibrary::searchBook()

Function description: This method allows the user to search the BookNode objects comprising the library. He or she is
asked to choose between four different search options (title, title keyword, author, and category), the appropriate
helper method is then executed, and any search results are printed to the console.

Pre-condition: The red-black tree must not be empty.
Post-condition: If found, search results are output to the console. The search option helper methods add nodes to
a searchResult vector that is global within the class, so this vector is emptied for use in subsequent searches.
*/
void BookLibrary::searchBook(){
    clearScreen();
    cout<<"SEARCH FOR A BOOK"<<endl;
    cout<<"*****************"<<endl;
    if(root == nil){
        cout<<"Your library is empty! Please enter a book before searching."<<endl<<endl;
    }
    else{
        int intUserInput = 0;
        while(intUserInput < 1 || intUserInput > 4){
            //Display main menu and prompt input
            cout<<"1. Search by title"<<endl;
            cout<<"2. Search by title keyword"<<endl;
            cout<<"3. Search by author"<<endl;
            cout<<"4. Search by category"<<endl;
            cin>>intUserInput;
            cin.ignore(10000,'\n');
            if(intUserInput < 1 || intUserInput > 4){
                cout<<"Invalid Input"<<endl;
            }
        }
        if(intUserInput == 1){
            string searchTitle;
            cout<<"Enter the exact title: "<<endl;
            getline(cin,searchTitle);
            searchBookTitle(root, searchTitle);
        }
        if(intUserInput == 2){
            string searchKeyword;
            cout<<"Enter a keyword from the title: "<<endl;
            getline(cin,searchKeyword);
            searchBookKeyword(root, searchKeyword);
        }
        if(intUserInput == 3){
            string searchAuthor;
            cout<<"Enter the author's last name: "<<endl;
            getline(cin,searchAuthor);
            searchBookAuthor(root, searchAuthor);
        }
        if(intUserInput == 4){
            string searchCategory;
            int catEntry;
            while(catEntry < 1 || catEntry > 18){
            cout<<"Enter a category: "<<endl;
            for(unsigned int i = 0; i < bookCategories.size(); i++){
                cout<<"\t"<<i+1<<": "<<bookCategories[i];
                i++;
                cout<<"\t\t"<<i+1<<": "<<bookCategories[i]<<endl;

            }
            cin>>catEntry;
            cin.ignore(10000,'\n');
            if(catEntry < 1 || catEntry > 18){
                cout<<"Not a valid entry."<<endl;
            }
            }
            searchCategory = bookCategories[catEntry-1];
            searchBookCategory(root, searchCategory);
        }
        if(searchResults.size() == 0){
            cout<<"No results found."<<endl;
        }
        else if(searchResults.size() != 0){
            cout<<"RESULTS"<<endl;
            cout<<"*******"<<endl;
            for(unsigned int i = 0; i < searchResults.size(); i++){
                cout<<"Result # "<<i+1<<endl;
                cout<<"\tTitle: "<<searchResults[i].title<<endl;
                cout<<"\tAuthor: "<<searchResults[i].author<<endl;
                cout<<"\tCategory: "<<searchResults[i].category<<endl;
                if(searchResults[i].ISBN10 != ""){
                    cout<<"\tISBN-10: "<<searchResults[i].ISBN10<<endl;
                }
                else{
                    cout<<"\tISBN-10 unknown."<<endl;
                }
                if(searchResults[i].ISBN13 != ""){
                    cout<<"\tISBN-13: "<<searchResults[i].ISBN13<<endl;
                }
                else{
                    cout<<"\tISBN-13 unknown."<<endl;
                }
            }
        }
        searchResults.clear();
    }
}

/*
Function prototype:
void BookLibrary::printLibrary()

Function description: Prints the contents of the library in alphabetical order by title using an inorder traversal. This
is the public initial call of the method.

Pre-condition: The red-black tree must not be empty.
Post-condition: Library contents are printed to console.
*/
void BookLibrary::printLibrary(){
    clearScreen();
    cout<<"PRINT LIBRARY"<<endl;
    cout<<"*************"<<endl;
    if(root == nil){
        cout<<"Your library is empty! Please enter a book before searching."<<endl<<endl;
    }
    else{
        BookNode *node = root;
        if(node->leftChild != nil){
            printLibrary(node->leftChild);
        }
        /*START INORDER OPS*/
        cout<<node->title<<" by "<<node->author<<" (Category:"<<node->category<<")"<<endl;
        /*END INORDER OPS*/
        if(node->rightChild != nil){
            printLibrary(node->rightChild);
        }
    }
}

/*
Function prototype:
void BookLibrary::countBooks()

Function description: Using a counter for the number of books that is incremented whenever a node
is added to the tree, prints the number of books in the library.

Pre-condition: None, other than that a class instance has been created (the library may be empty).
Post-condition: The number of books in the library is printed to the console.
*/
void BookLibrary::countBooks(){
    clearScreen();
    cout<<"COUNT BOOKS"<<endl;
    cout<<"***********"<<endl;
    if(bookCounter == 1){
        cout<<"There is "<<bookCounter<<" book in the library."<<endl;
    }
    else{
        cout<<"There are "<<bookCounter<<" books in the library."<<endl;
    }
}

/*
Function prototype:
bool BookLibrary::saveLibrary()

Function description: Using the previously defined library name as the filename, an inorder traversal is
used to output the contents of the library to a .json file, in alphabetical order by title and formatted using
JSON.

Pre-condition: The red-black tree must not be empty. The filename will have previously been defined when the
library was originally created or opened.
Post-condition: If the save is successful, a .json file in the same directory as the executable has been created. The
function returns false if the save is successful, indicating that no changes have been made to the library that would
require saving.
*/
bool BookLibrary::saveLibrary(){
    clearScreen();
    bool saveFailed = false;
    cout<<"SAVE LIBRARY"<<endl;
    cout<<"************"<<endl;
    if(root == nil){
        cout<<"Your library is empty! Please enter a book before searching."<<endl<<endl;
    }
    else{
        saveCounter = 1;
        BookNode *node = root;
        LibraryOutput = json_object_new_object(); //Pointer declared global within class
        /*Recursive call, left child*/
        if(node->leftChild != nil){
            saveLibrary(node->leftChild);
        }
        /*START INORDER*/
        json_object *jBook = json_object_new_object();
        //Add title
        json_object *jTitle = json_object_new_string(node->title.c_str());
        json_object_object_add(jBook,"Title",jTitle);
        //Add author
        json_object *jAuthor = json_object_new_string(node->author.c_str());
        json_object_object_add(jBook,"Author",jAuthor);
        //Add category
        json_object *jCat = json_object_new_string(node->category.c_str());
        json_object_object_add(jBook,"Category",jCat);
        //Add ISBN-10
        json_object *jISBN10 = json_object_new_string(node->ISBN10.c_str());
        json_object_object_add(jBook,"ISBN-10",jISBN10);
        //Add ISBN-13
        json_object *jISBN13 = json_object_new_string(node->ISBN13.c_str());
        json_object_object_add(jBook,"ISBN-13",jISBN13);
        //Add book to library object
        string booknumber = "Book #" + std::to_string(saveCounter);
        json_object_object_add(LibraryOutput,booknumber.c_str(),jBook);
        saveCounter++;
        /*END INORDER*/
        /*Recursive call, right child*/
        if(node->rightChild != nil){
            saveLibrary(node->rightChild);
        }
        /*POSTORDER*/
        string outputName = libraryName + ".json";
        ofstream outFile(outputName);
        if(outFile.is_open()){
            outFile << json_object_to_json_string_ext(LibraryOutput, JSON_C_TO_STRING_PRETTY);
            outFile.close();
            cout<<"Library saved to file."<<endl;
        }
        else{
            cout<<"Can't open file."<<endl;
            saveFailed = true;
        }
        //Reset variables
        saveCounter = 1;
        LibraryOutput = NULL;
    }
    return saveFailed;
}

/*Public method: Clears the console screen*/
void BookLibrary::clearScreen(){
    cout << "\033[2J\033[1;1H";
}

/*PRIVATE METHODS*/

/*
Function prototype:
void BookLibrary::searchBookTitle(BookNode, string)

Function description: Given a BookNode object and string, searches the subtree of the red-black tree to find
an exact title match of the string.

Pre-condition: The red-black tree must not be empty. Method is called from public method searchBook().
Post-condition: If a node with a matching title is found, it is added to a vector that is accessed from
searchBook() and output to the console.
*/
void BookLibrary::searchBookTitle(BookNode *node, string searchTitle){
    while(node != nil){
        if(strcmp(searchTitle.c_str(),node->title.c_str()) == 0){
            searchResults.push_back(*node);
            return;
        }
        else if(strcmp(searchTitle.c_str(),node->title.c_str()) < 0){
            node = node->leftChild;
        }
        else{
            node = node->rightChild;
        }
    }
}

/*
Function prototype:
void BookLibrary::searchBookKeyword(BookNode, string){

Function description: Given a BookNode object and string, searches the subtree of the red-black tree to find
a partial keyword match in the title of a book. Uses an inorder traversal, and all matches are returned.

Pre-condition: The red-black tree must not be empty. Method is called from public method searchBook().
Post-condition: All nodes with titles containing the input string are added to a vector that is accessed from
searchBook() and output to the console.
*/
void BookLibrary::searchBookKeyword(BookNode *node, string searchKeyword){
    if(node->leftChild != nil){
        searchBookKeyword(node->leftChild, searchKeyword);
    }
    /*START INORDER OPS*/
    /*Keyword found in title*/
    if(node->title.find(searchKeyword) != string::npos){
        searchResults.push_back(*node);
    }
    /*END INORDER OPS*/
    if(node->rightChild != nil){
        searchBookKeyword(node->rightChild, searchKeyword);
    }
}

/*
Function prototype:
void BookLibrary::searchBookAuthor(BookNode, string){

Function description: Given a BookNode object and string, searches the subtree of the red-black tree to find
books whose authors match the input string. Uses an inorder traversal, and all matches are returned.

Pre-condition: The red-black tree must not be empty. Method is called from public method searchBook().
Post-condition: All nodes with authors containing the input string are added to a vector that is accessed from
searchBook() and output to the console.
*/
void BookLibrary::searchBookAuthor(BookNode *node, string searchAuthor){
    if(node->leftChild != nil){
        searchBookAuthor(node->leftChild, searchAuthor);
    }
    /*START INORDER OPS*/
    /*Keyword found in title*/
    if(node->author.find(searchAuthor) != string::npos){
        searchResults.push_back(*node);
    }
    /*END INORDER OPS*/
    if(node->rightChild != nil){
        searchBookAuthor(node->rightChild, searchAuthor);
    }
}

/*
Function prototype:
void BookLibrary::searchBookCategory(BookNode, string){

Function description: Given a BookNode object and string, searches the subtree of the red-black tree to find
books whose categories match the input string. Uses an inorder traversal, and all matches are returned.

Pre-condition: The red-black tree must not be empty. Method is called from public method searchBook().
Post-condition: All nodes with categories matching the input string are added to a vector that is accessed from
searchBook() and output to the console.
*/
void BookLibrary::searchBookCategory(BookNode *node, string searchCategory){
    if(node->leftChild != nil){
        searchBookCategory(node->leftChild, searchCategory);
    }
    /*START INORDER OPS*/
    /*Keyword found in title*/
    if(node->category.find(searchCategory) != string::npos){
        searchResults.push_back(*node);
    }
    /*END INORDER OPS*/
    if(node->rightChild != nil){
        searchBookCategory(node->rightChild, searchCategory);
    }
}

/*
Function prototype:
void BookLibrary::insertNode(BookNode){

Function description: Adds a new node to the red-black tree using the RB-Insert algorithm from p. 315 of CLRS.

Pre-condition: None, other than that a class instance has been created.
Post-condition: A node has been added to the red-black tree and any rebalancing has been completed.
*/
void BookLibrary::insertNode(BookNode *newnode){
    bookCounter++;
    /*Add new book node to red-black tree, sorted by title*/
    BookNode *y = nil;
    BookNode *x = root;
    while(x != nil){
        //Set parent node to current node
        y = x;
        if(strcmp(newnode->title.c_str(),x->title.c_str()) < 0){
            x = x->leftChild; //If new node < current node, move to left child
        }
        else{
            x = x->rightChild; //If new node >= current node, move to right
        }
    }
    //Set parent/child to link new node to tree
    //If new node has no parent tree is empty; set as root
    newnode->parent = y;
    if(y == nil){
        root = newnode;
    }
    else if(strcmp(newnode->title.c_str(),y->title.c_str()) < 0){
        y->leftChild = newnode;
    }
    else{
        y->rightChild = newnode;
    }
    newnode->leftChild = nil;
    newnode->rightChild = nil;
    newnode->isRed = true;
    rbAddFixup(newnode);
}

/*
Function prototype:
void BookLibrary::rbAddFixup(BookNode){

Function description: After adding a node to the red-black tree, this function recolors nodes and performs
rotations as necessary to restore red-black properties. Uses the RB-Insert-Fixup algorithm from p. 316 of
CLRS.

Pre-condition: A node has been added to the red-black tree.
Post-condition: Nodes have been recolored, and rotations have been performed to restore red-black properties.
*/
void BookLibrary::rbAddFixup(BookNode *z){
    while(z->parent->parent != nil && z->parent->isRed == true){
        if(z->parent == z->parent->parent->leftChild){
            BookNode *y = z->parent->parent->rightChild;
            if(y->isRed == true){
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->rightChild){
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                rightRotate(z->parent->parent);
            }
        }
        else{
            BookNode *y = z->parent->parent->leftChild;
            if(y->isRed == true){
                z->parent->isRed = false;
                y->isRed = false;
                z->parent->parent->isRed = true;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->leftChild){
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->isRed = false;
                z->parent->parent->isRed = true;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->isRed = false;
}

/*
Function prototype:
void BookLibrary::leftRotate(BookNode)

Function description: After adding a node to the red-black tree, this function performs a left rotation
around a node as part of restoring red-black properties. Uses the Left-Rotate algorithm on p. 313 of CLRS.

Pre-condition: A node has been added to the red-black tree.
Post-condition: A left rotation around the input node has been performed as part of restoring red-black properties.
*/
void BookLibrary::leftRotate(BookNode *node){
    BookNode *y = node->rightChild;
    node->rightChild = y->leftChild;
    if(y->leftChild != nil){
        y->leftChild->parent = node;
    }
    y->parent = node->parent;
    if(node->parent == nil){
        root = y;
    }
    else if(node == node->parent->leftChild){
        node->parent->leftChild = y;
    }
    else{
        node->parent->rightChild = y;
    }
    y->leftChild = node;
    node->parent = y;
}

/*
Function prototype:
void BookLibrary::rightRotate(BookNode)

Function description: After adding a node to the red-black tree, this function performs a right rotation
around a node as part of restoring red-black properties. Uses a modified version of the Left-Rotate algorithm
on p. 313 of CLRS.

Pre-condition: A node has been added to the red-black tree.
Post-condition: A right rotation around the input node has been performed as part of restoring red-black properties.
*/
void BookLibrary::rightRotate(BookNode *node){
    BookNode *x = node->leftChild;
    node->leftChild = x->rightChild;
    if(x->rightChild != nil){
        x->rightChild->parent = node;
    }
    x->parent = node->parent;
    if(node->parent == nil){
        root = x;
    }
    else if(node == node->parent->leftChild){
        node->parent->leftChild = x;
    }
    else{
        node->parent->rightChild = x;
    }
    x->rightChild = node;
    node->parent = x;
}

/*
Function prototype:
void BookLibrary::deleteTree(BookNode)

Function description: Using a postorder transversal, all nodes except nil are deleted from the red-black tree.

Pre-condition: The red-black tree has more than one node (i.e. root does not equal nil).
Post-condition: All nodes other than nil have been deleted.
*/
void BookLibrary::deleteTree(BookNode *node){
    if(node->leftChild != nil){
        deleteTree(node->leftChild);
    }
    if(node->rightChild != nil){
        deleteTree(node->rightChild);
    }
    /*Postorder operations*/
    delete node;
}

/*Private: Recursive call of public method printLibrary()*/
void BookLibrary::printLibrary(BookNode *node){
    if(node->leftChild != nil){
        printLibrary(node->leftChild);
    }
    /*START INORDER OPS*/
    cout<<node->title<<endl;
    /*END INORDER OPS*/
    if(node->rightChild != nil){
        printLibrary(node->rightChild);
    }
}

/*Private: Recursive call of public method saveLibrary()*/
void BookLibrary::saveLibrary(BookNode *node){
    if(node->leftChild != nil){
        saveLibrary(node->leftChild);
    }
    /*START INORDER*/
    json_object *jBook = json_object_new_object();
    //Add title
    json_object *jTitle = json_object_new_string(node->title.c_str());
    json_object_object_add(jBook,"Title",jTitle);
    //Add author
    json_object *jAuthor = json_object_new_string(node->author.c_str());
    json_object_object_add(jBook,"Author",jAuthor);
    //Add category
    json_object *jCat = json_object_new_string(node->category.c_str());
    json_object_object_add(jBook,"Category",jCat);
    //Add ISBN-10
    json_object *jISBN10 = json_object_new_string(node->ISBN10.c_str());
    json_object_object_add(jBook,"ISBN-10",jISBN10);
    //Add ISBN-13
    json_object *jISBN13 = json_object_new_string(node->ISBN13.c_str());
    json_object_object_add(jBook,"ISBN-13",jISBN13);
    //Add book to library object
    string booknumber = "Book #" + std::to_string(saveCounter);
    json_object_object_add(LibraryOutput,booknumber.c_str(),jBook);
    saveCounter++;
    /*END INORDER*/
    if(node->rightChild != nil){
        saveLibrary(node->rightChild);
    }
}

/*Private: Helper function for enterISBN. Stores LibCurl data in struct BufferStruct object*/
/*Code taken from: https://www.hackthissite.org/articles/read/1078*/
size_t BookLibrary::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data){
    size_t realsize = size * nmemb;
    struct BufferStruct *mem = (struct BufferStruct*) data;
    mem->buffer = (char*)realloc(mem->buffer, mem->size + realsize + 1);
    if(mem->buffer){
        memcpy(&(mem->buffer[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->buffer[ mem->size ] = 0;
    }
    return realsize;
}