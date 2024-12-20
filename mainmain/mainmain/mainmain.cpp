

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;

//ADD STRUCTS HERE



struct user {
    int ID;
    string name;
    string email;
    string password;

    //contactinfo is declared as a string to protect left hand side zeros (phone numbers)
    string contactinfo;

    //vector of pairs: where each pair's first value is the linked bank account ID and the second value is balance of said acc
    vector<pair<int, double>> linkedbankacc_balance;

    /*
    //may be added later on in development of project
            //vector of pairs: where each pair's first value is the linked bank account ID
            //and the second pair's first value is the balance of the account and the second value is the currency symbol
            vector<pair<int,pair<double, char>>> linkedbankacc_balance;
            double accbalance;
    */

};



//ADD GLOBAL VARIABLES HERE

vector <user>userslist;



//ADD YOUR FUNCTIONS HERE



//this function DOES NOT deal with cases where there is no data match
//user emails have been proved to exist in userslist using is_there in all instances where this function is used below
user find(const vector <user>& userslist, user datamatch) {
    user matchfound;
    for (int i = 0; i < userslist.size(); i++) {
        if (userslist[i].email == datamatch.email) {
            matchfound = userslist[i];
            break;
        }
    }
    return matchfound;
}




//this function returns a boolean value to describe weather or not a certain email is linked to a user
bool is_there(const vector <user>& userslist, user datamatch) {
    for (int i = 0; i < userslist.size(); i++) {
        if (userslist[i].email == datamatch.email) {
            return true;
        }
    }
    return false;
}




//declaring login function before signup() since login() is called inside signup()
void login();




void signup() {

    // user is redirected here if signup option is chosen
    user data;
    cout << "Please enter your email\n";
    cin >> data.email;

    //verfiy that this email does not exist
    //if already linked, user is prompted to either login or renter email
    while (is_there(userslist, data)) {
        int choice;
        cout << "This email is already linked to an account\n";
        cout << "If you wish to login please press 1\nIf you wish to renter your email please press 2\n";
        cin >> choice;
        cin.ignore();  // Clear input buffer

        //verify input is within range
        while (choice != 1 && choice != 2) {
            cout << "Invalid choice, please renter\n";
            cin >> choice;
            cin.ignore();  // Clear input buffer
        }
        if (choice == 1) {

            //exits current fujnction and redirects to login
            return login();
        }
        else if (choice == 2) {
            cin >> data.email;
        }
    }
    cout << "Please enter your name\n";
    cin.ignore(); // Clearing input buffer
    getline(cin, data.name);
    cout << "Please enter your password\n";
    cin.ignore();
    getline(cin, data.password);
    cout << "Please enter phone number\n";
    cin >> data.contactinfo;
    cin.ignore();

    //adding id as size of set + 1 as the new user has not been inserted yet
    data.ID = userslist.size() + 1;
    userslist.push_back(data);

    //randomizing OTP
    cout << "This is your OTP\n";
    srand(time(0));
    int OTP, renter;
    //generates a 4 digit OTP
    OTP = 1000 + rand() % 9000;
    cout << OTP << "\n Please renter the OTP\n";
    cin >> renter;

    //Verify the OTP has been rentered correctly
    //user has 3 tries to enter OTP, if still incorrect, OTP changes
    while (renter != OTP) {
        for (int i = 0; renter != OTP && i < 3; i++) {
            cout << "OTP does not match, please renter\n" << 3 - i << " tries left\n";
            cin >> renter;
        }
        if (renter == OTP) {
            break;
        }
        cout << "Your OTP has changed\n";
        cout << "This is your new OTP\n";
        srand(time(0));
        OTP = 1000 + rand() % 9000;
        cout << OTP << "\n Please renter the OTP\n";
        cin >> renter;
    }


    cout << "Signup succesfull\nRedirecting to dashboard...\n";
    //redirection to dashboard
}




void login() {

    // user is redirected here if login option is chosen
    user data, datamatch;
    int choice;
    cout << "Please enter your email";
    cin >> data.email;

    //verfies that email is linked to an account
    //if not linked, user is prompted to either sign up or renter email
    while (!is_there(userslist, data)) {
        cout << "This email is not linked to an account\n";
        cout << "If you wish to signup please press 1\nIf you wish to renter your email please press 2\n";
        cin >> choice;

        //verify input is within range
        while (choice != 1 && choice != 2) {
            cout << "Invalid choice, please renter\n";
            cin >> choice;
        }
        if (choice == 1) {

            //exits current function and redirects to signup
            return signup();
        }
        else if (choice == 2) {
            cin >> data.email;
        }
    }

    //password checking
    cout << "Account located successfully\nPlease enter your password\n";
    string password;
    cin >> password;
    datamatch = find(userslist, data);
    while (datamatch.password != password) {
        cout << "Incorrect password, please renter your password\n";
        cin >> password;
    }
    cout << "Password confirmed\nRedirecting to dashboard...\n";
    //redirection to dashboard
}







int main()
{

    //REMOVE THIS PART when eyad adds his user interface using jasmin's signup&login functions
                        /*int choice;
                        cout << "Press 1 to signup\nPress 2 to login\n";
                        cin >> choice;
                        //verify input is within range
                        while (choice != 1 && choice != 2) {
                            cout << "Invalid choice, please renter\n";
                            cin >> choice;
                        }
                        if (choice == 1) {
                            signup();
                        }
                        else if (choice == 2) {
                            login();
                        }*/


}

