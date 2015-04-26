#include <iostream>
#include "BookLibrary.h"

using namespace std;

//First menu (contains library file creation and management tools)
bool firstMenu(BookLibrary *MyLibrary){
    int intUserInput = 0;
    while(intUserInput < 1 || intUserInput > 5){
        //Display main menu and prompt input
        cout<<"=====VIRTUAL LIBRARY====="<<endl;
        cout<<"1. Create a new library"<<endl;
        cout<<"2. Open a library file"<<endl;
        cout<<"3. Rename a library file"<<endl;
        cout<<"4. Delete a library file"<<endl;
        cout<<"5. Exit"<<endl;
        cin>>intUserInput;
        cin.ignore(10000,'\n');
        if(intUserInput < 1 || intUserInput > 5){
            cout<<"Invalid Input"<<endl;
        }
    }
    //#1: Create a new library
    if(intUserInput == 1){
        MyLibrary->createLibrary();
        MyLibrary->clearScreen();
        return 0;
    }
    //#2: Open a library file
    else if(intUserInput == 2){
        MyLibrary->openLibrary();
        return 0;
    }
    //#3: Rename an existing library
    else if(intUserInput == 3){
        MyLibrary->renameLibrary();
        firstMenu(MyLibrary);
        return 0;
    }
    //#4: Delete an existing library
    else if(intUserInput == 4){
        MyLibrary->deleteLibrary();
        firstMenu(MyLibrary);
        return 0;
    }
    //#4: Quit
    else{
        return 1;
    }
    return 1;
}

//Second menu (contains tools to view, modify and save library)
bool secondMenu(BookLibrary *MyLibrary, bool &unsavedChanges){
    int intUserInput = 0;
    while(intUserInput < 1 || intUserInput > 7){
        //Display main menu and prompt input
        cout<<"======MAIN MENU======"<<endl;
        cout<<"1. Enter a book from ISBN"<<endl;
        cout<<"2. Enter a book manually"<<endl;
        cout<<"3. Search for a book"<<endl;
        cout<<"4. Print all titles in library"<<endl;
        cout<<"5. Count books in the library"<<endl;
        cout<<"6. Save library to file"<<endl;
        cout<<"7. Exit"<<endl;
        cin>>intUserInput;
        cin.ignore(10000,'\n');
        if(intUserInput < 1 || intUserInput > 7){
            cout<<"Invalid Input"<<endl;
        }
    }
    //#1: Enter a book from ISBN
    if(intUserInput == 1){
        unsavedChanges = MyLibrary->enterISBN();
        return 0;
    }
    //#2: Enter a book manually
    else if(intUserInput == 2){
        unsavedChanges = MyLibrary->enterManual();
        return 0;
    }
    //#3: Search for a book
    else if(intUserInput == 3){
        MyLibrary->searchBook();
        return 0;
    }
    //#4: Print information for the entire library
    else if(intUserInput == 4){
        MyLibrary->printLibrary();
        return 0;
    }
    //#5: Delete the current library
    else if(intUserInput == 5){
        MyLibrary->countBooks();
        return 0;
    }

    //#6: Save library to File
    else if(intUserInput == 6){
        unsavedChanges = MyLibrary->saveLibrary();
        return 0;
    }
    //#7: Exit
    else{
        if(unsavedChanges == true){
            int intSaveInput = 0;
            while(intSaveInput < 1 || intSaveInput > 2){
                cout<<"You have unsaved changes. Would you like to save?"<<endl;
                cout<<"Enter 1 for yes or 2 for no."<<endl;
                cin>>intSaveInput;
                cin.ignore(10000,'\n');
                if(intSaveInput < 1 || intSaveInput > 2){
                    cout<<"Invalid Input"<<endl;
                }
            }
            if(intSaveInput == 1){
                MyLibrary->saveLibrary();
            }
        }
        return 1;
    }
    return 1;
}


int main(){
    BookLibrary *MyLibrary = new BookLibrary();
    bool quit;
    bool unsavedChanges = 0;
    MyLibrary->clearScreen();
    //Launch first menu (contains library file creation and management tools)
    quit = firstMenu(MyLibrary);
    //Launch second menu (contains tools to view, modify and save library)
    while(quit == false){
        quit = secondMenu(MyLibrary,unsavedChanges);
    }
    delete MyLibrary;
    return 0;
}