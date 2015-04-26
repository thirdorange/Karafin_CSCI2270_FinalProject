# CSCI 2270 Final Project
# VirtualLibrary
# Method Documentation

## Function prototype:
BookLibrary::BookLibrary()

Function description: This is the constructor for the BookLibrary class.

Pre-condition: None.
Post-condition: Counter for number of books is initialized, red-black tree is initialized, and a list of possible book categories has been generated.

## Function prototype:
BookLibrary::~BookLibrary(){

Function description: This is the destructor for the BookLibrary class. Calls the private method deleteTree to delete all nodes using a postorder transversal.

Pre-condition: None, other than that a class instance has been created. Contains handling for empty tree.
Post-condition: All nodes in the red-black tree have been deleted.

## Function prototype:
void BookLibrary::createLibrary()

Function description: This method captures the name of a new library when it is created, which is later used as the filename when the library is saved.

Pre-condition: None, other than that a class instance has been created.
Post-condition: Class variable libraryName has been set.

## Function prototype:
void BookLibrary::openLibrary()

Function description: This method allows the user to open a library from a previously saved .json file. If a matching filename is not found, the user has the option to either enter a new filename, create a new library, or exit the program. If a file is matched, the JSON-formatted data are converted to BookNode objects and added to the red-black tree.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If user selection results in a file being opened, a number of new BookNode objects equal to the number in the .json file are created and added to the red-black tree, which is rebalanced after addition of each object.

## Function prototype:
void BookLibrary::renameLibrary()

Function description: This method allows the user to rename the .json file for an existing library in the same directory as the executable file.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If the method is successful, the .json file has been renamed from its original name to one specified by the user.

## Function prototype:
void BookLibrary::deleteLibrary()

Function description: This method allows the user to delete a .json file for an existing library in the same directory as the executable file.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If the method is successful, the .json file has been deleted.

## Function prototype:
void BookLibrary::enterISBN()

Function description: This method allows the user to input either an ISBN-10 or ISBN-13 for a book to be added to the library. After the number is entered, libcurl submits the appropriate URL to Google Books and retrieves the output, which is then cast as a string. If there is a match, the JSON data are interpreted and a new BookNode is added.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If a match for the ISBN is found, a new BookNode object is created and initialized. insertNode() is then called to add the node to the red-black tree and rebalance as necessary. The function returns 1 if a node is added and 0 if no node is added, which determines whether changes to the tree have been made and the user will be prompted to save before exiting.

## Function prototype:
bool BookLibrary::enterManual()

Function description: This method allows the user to input either an ISBN-10 or ISBN-13 for a book to be added to the library. After the number is entered, libcurl submits the appropriate URL to Google Books and retrieves the output, which is then cast as a string. If there is a match, the JSON data are interpreted and a new BookNode is added.

Pre-condition: None, other than that a class instance has been created.
Post-condition: If a match for the ISBN is found, a new BookNode object is created and initialized. insertNode() is then called to add the node to the red-black tree and rebalance as necessary. The function returns 1, which flags that changes
to the tree have been made, and the user will be prompted to save before exiting.

## Function prototype:
void BookLibrary::searchBook()

Function description: This method allows the user to search the BookNode objects comprising the library. He or she is asked to choose between four different search options (title, title keyword, author, and category), the appropriate helper method is then executed, and any search results are printed to the console.

Pre-condition: The red-black tree must not be empty.
Post-condition: If found, search results are output to the console. The search option helper methods add nodes to a searchResult vector that is global within the class, so this vector is emptied for use in subsequent searches.

## Function prototype:
void BookLibrary::printLibrary()

Function description: Prints the contents of the library in alphabetical order by title using an inorder traversal. This is the public initial call of the method.

Pre-condition: The red-black tree must not be empty.
Post-condition: Library contents are printed to console.

## Function prototype:
void BookLibrary::countBooks()

Function description: Using a counter for the number of books that is incremented whenever a node is added to the tree, prints the number of books in the library.

Pre-condition: None, other than that a class instance has been created (the library may be empty).
Post-condition: The number of books in the library is printed to the console.

## Function prototype:
bool BookLibrary::saveLibrary()

Function description: Using the previously defined library name as the filename, an inorder traversal is used to output the contents of the library to a .json file, in alphabetical order by title and formatted using JSON.

Pre-condition: The red-black tree must not be empty. The filename will have previously been defined when the library was originally created or opened.
Post-condition: If the save is successful, a .json file in the same directory as the executable has been created. The function returns false if the save is successful, indicating that no changes have been made to the library that would require saving.

