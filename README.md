# CSCI 2270 Final Project
# VirtualLibrary

## Project Summary

VirtualLibrary is a simple command line C++ application that builds the user a virtual library from his or her collection of books. Books may be input either using the ISBN, which will query Google Books using curl and interpret the JSON-formatted output, or manually. Information about the books (title, author, genre, ISBN-10, and ISBN-13) are stored using a red-black tree. The user has the ability to retrieve and store library information from a local file formatted using JSON.

## How to Run

This program has only been tested at this time using Ubuntu 14.04 LTS, and the use of this operating system is recommended. Before proceeding, ensure that packages g++, libjson0, libjson0-dev, and libcurl4-gnutls-dev are installed.

To run this program, download the files LibraryMain.cpp, BookLibrary.cpp, and BookLibrary.h to a single directory. Navigate to the directory and execute the command:

g++ LibraryMain.cpp BookLibrary.cpp -std=c++11 -o VirtualLibrary -l json -l curl

Once compiled, the program may be launched from the command line by navigating to the directory and entering "./VirtualLibrary" to execute the file. The user is presented with a startup menu:

=====VIRTUAL LIBRARY=====
1. Create a new library
2. Open a library file
3. Rename a library file
4. Delete a library file
5. Exit

After a library is successfully created or opened from file, the main menu is then presented:

========MAIN MENU========
1. Enter a book from ISBN
2. Enter a book manually
3. Search for a book
4. Print all titles in library
5. Count books in the library
6. Save library to file
7. Exit

Further documentation, including expected inputs and outputs and their data types for each menu option, may be accessed in the repository file "METHOD_DOCUMENTATION.md".

## Dependencies

This program requires json-c (https://github.com/json-c/json-c) and libcurl (http://curl.haxx.se/libcurl/).

To install libcurl and json-c on Ubuntu, execute the following at the command line:

sudo apt-get install libcurl4-gnutls-dev libjson0 libjson0-dev

## System Requirements

This program has only been tested at this time using Ubuntu 14.04 LTS.

## Group Members

No other group members are currently assisting with the project.

## Contributors

Code to transfer data using LibCurl and output for further interpretation was adapted with minor changes from that provided in the article "A Beginners Guide to LibCurl" published by secdef9 at HackThisSite (https://www.hackthissite.org/articles/read/1078).

## Open Issues and Bugs

The following are known issues at this time:

1. When entering a book manually, the program does not check to see if ISBN-10 or ISBN-13 are formatted correctly.
2. Book searches are case sensitive (e.g. keyword searching for "harry potter" will not return the result "Harry Potter").
3. Book entry by ISBN does not utilize an API key for Google Books. The allowable number of ISBN queries may be restricted.
4. JSON interpretation is rudimentary and based upon directly analyzing string data rather than parsing.
