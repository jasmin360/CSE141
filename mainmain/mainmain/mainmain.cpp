#define _CRT_SECURE_NO_WARNINGS

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
struct Transaction {
    double amount;
    string receiver_name; // will need user info
    string sender_name;   // will need user info
    int transaction_id;    // will need user info
    string currency = "$";
    string status;
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

struct Transaction {
    double amount;
    string receiver_name; // will need user info
    string sender_name;   // will need user info
    int transaction_id;    // will need user info
    string currency = "$";
    string status;
};

// (needs user2.name), checks data type and amount
double GetValidAmount(string receiver_name) {
    double amount;
    cout << "How much money would u like to send to " << receiver_name << endl; // receiver_name will be changed need info
    cin >> amount;
    while (true) {
        while (cin.fail()) { // error
            cin.clear();              // clear error input
            cin.ignore(1000, '\n');   // ignore invalid input
            cout << "Invalid Character,please enter a real number. " << endl;
            cin >> amount;
        }
        while (amount < 15) {
            cout << "please enter a number bigger than 15 u brookie" << endl;
            cin >> amount;
        }
        break; // exit the loop if the input is valid
    }
    return amount;
}

// checks if he has enough money
bool HasEnoughBalance(double balance, double amount) {
    if (balance >= amount) {
        return 1;
    }
    else {
        cout << "Not enough money to complete the transaction";
        return 0;
    }
}

double UpdateSenderBalance(double sender_balance, double amount_sent) {
    return sender_balance -= amount_sent;
}

double UpdateReceiverBalance(double receiver_balance, double amount_received) {
    return receiver_balance += amount_received;
}

int ProcessTransaction(double& sender_balance, double& receiver_balance) { // to use it in main function properly
    Transaction tx; // instance
    tx.sender_name = "User1"; //
    tx.receiver_name = "Omar"; //
    tx.transaction_id = 101; //
    tx.status = "pending"; //
    tx.amount = GetValidAmount(tx.receiver_name);
    char answer = 'Y';
    bool isBalanceSufficient = 0;
    double updated_sender_balance;
    double updated_receiver_balance;

    isBalanceSufficient = HasEnoughBalance(sender_balance, tx.amount);
    if (!isBalanceSufficient) {
        tx.status = "Cancelled";
        return 0;
    }

    cout << "Are u sure u want to send " << tx.amount << " to " << tx.receiver_name << " ? If yes type Y If not type N. (Default is : Y) " << endl;
    cin >> answer;
    cin.ignore(); // tx.currency picks up answer did this to fix it
    if (answer == 'N' || answer == 'n') {
        cout << "Transaction cancelled";
        tx.status = "Cancelled";
        return 0;
    }

    cout << "Please enter the currency. Type $ for dollars or type € for euros. (Default is : $) " << endl;
    cin >> tx.currency;
    if (tx.currency != "€" && tx.currency != "$") {
        tx.currency = "$"; // default value
    }

    updated_sender_balance = UpdateSenderBalance(sender_balance, tx.amount);
    updated_receiver_balance = UpdateReceiverBalance(receiver_balance, tx.amount);
    tx.status = "completed";

    cout << "\nTransaction Successful!" << endl;
    cout << "Transaction ID: " << tx.transaction_id << endl;
    cout << "Sender: " << tx.sender_name << endl;
    cout << "Receiver: " << tx.receiver_name << endl;
    cout << "Amount: " << tx.currency << tx.amount << endl;
    cout << "Sender's New Balance: " << updated_sender_balance << tx.currency << endl;
    cout << "Receiver's New Balance: " << updated_receiver_balance << tx.currency << endl;

    return 1; // returns success
}

int main() {
    double sender_balance = 10000;
    double receiver_balance = 0;
    ProcessTransaction(sender_balance, receiver_balance);
    return 0;
}


#include <iostream>
#include <iomanip> 
#include <string>
#include <cstring>
#include <vector>
#include <Windows.h>
using namespace std;


int horizontal = 100;
int vertical = 20;




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
    */

};
vector <user>userslist;


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
void signup();

void login() {

    // user is redirected here if login option is chosen
    user data, datamatch;
    int choice;
    cout << setw(horizontal / 2) << "Please enter your email\n";
    cin >> data.email;

    //verfies that email is linked to an account
    //if not linked, user is prompted to either sign up or renter email
    while (!is_there(userslist, data)) {
        cout << setw(horizontal / 2) << "This email is not linked to an account\n";
        cout << setw(horizontal / 2) << "If you wish to signup please press 1\nIf you wish to renter your email please press 2\n";
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






void clear();
void Display_login_interface();

// Function to set the console text and background color
void SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,
        (bgColor << 4) | textColor);
}


void centretext(string h) {
    cout << "||" << setw((horizontal - 2) / 2 + h.length() / 2) << h
        << setw((horizontal - 2) / 2 - h.length() / 2) << "||";
}

void setborder() {
    for (int i = 1; i <= horizontal; i++) {
        cout << "-";

    }
    cout << endl;
    for (int i = 1; i <= vertical; i++) {
        cout << "||" << setw(horizontal - 2) << "||";
        cout << endl;
    }
    for (int i = 1; i <= horizontal; i++) {
        cout << "-";
    }
}

void delay(double delay = 1) {

    clock_t start_time = clock();
    while ((clock() - start_time) / 1000 < delay);
}
string emailone;
string passwordone;
string emailtwo;
string passwordtwo;
void signup() {



    // user is redirected here if signup option is chosen
    user data;
    cout << setw(horizontal / 2) << "Please enter your email\n";
    cin >> data.email;

    //verfiy that this email does not exist
    //if already linked, user is prompted to either login or renter email
    while (is_there(userslist, data)) {
        int choice;
        cout << setw(horizontal / 2) << "This email is already linked to an account\n";
        cout << setw(horizontal / 2) << "If you wish to login please press 1\nIf you wish to renter your email please press 2\n";
        cin >> choice;
        cin.ignore();  // Clear input buffer

        //verify input is within range
        while (choice != 1 && choice != 2) {
            cout << setw(horizontal / 2) << "Invalid choice, please renter\n";
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
    cout << setw(horizontal / 2) << "Please enter your name\n";
    cin.ignore(); // Clearing input buffer
    getline(cin, data.name);
    cout << setw(horizontal / 2) << "Please enter your password\n";
    cin.ignore();
    getline(cin, data.password);
    cout << setw(horizontal / 2) << "Please enter phone number\n";
    cin >> data.contactinfo;
    cin.ignore();

    //adding id as size of set + 1 as the new user has not been inserted yet
    data.ID = userslist.size() + 1;
    userslist.push_back(data);

    //randomizing OTP
    cout << setw(horizontal / 2) << "This is your OTP\n";
    srand(time(0));
    int OTP, renter;
    //generates a 4 digit OTP
    OTP = 1000 + rand() % 9000;
    cout << OTP << setw(horizontal / 2) << "\nPlease renter the OTP\n";
    cin >> renter;

    //Verify the OTP has been rentered correctly
    //user has 3 tries to enter OTP, if still incorrect, OTP changes
    while (renter != OTP) {
        for (int i = 0; renter != OTP && i < 3; i++) {
            cout << setw(horizontal / 2) << "OTP does not match, please renter\n" << 3 - i << " tries left\n";
            cin >> renter;
        }
        if (renter == OTP) {
            break;
        }
        cout << setw(horizontal / 2) << "Your OTP has changed\n";
        cout << setw(horizontal / 2) << "This is your new OTP\n";
        srand(time(0));
        OTP = 1000 + rand() % 9000;
        cout << OTP << "\nPlease renter the OTP\n";
        cin >> renter;
    }


    cout << "Signup succesfull\nRedirecting to dashboard...\n";
    //redirection to dashboard

    delay(3.0);
    clear();
    Display_login_interface();
}

void clear() {

    system("cls");

}

void Display_login_interface() {
    int horizontal = 120;
    int vertical = 20;

    for (int i = 1; i <= horizontal; i++) {
        SetColor(14, 0);
        cout << "-";
    }
    cout << endl;
    for (int i = 1; i <= vertical; i++) {
        SetColor(14, 0);
        if (i == 2) {
            cout << "||" << setw((horizontal - 2) / 2 + 10);
            SetColor(15, 0);
            cout << "Welcome to Instapay";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 10) << "||";
        }
        else if (i == vertical / 2 - 1) {
            cout << "||" << setw((horizontal - 2) / 2 + 5);
            SetColor(15, 0);
            cout << "Sign up (1)";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 5) << "||";
        }
        else if (i == vertical / 2) {
            cout << "||" << setw((horizontal - 2) / 2 + 5);
            SetColor(15, 0);
            cout << "Sign in (2)";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 5) << "||";
        }
        else if (i == vertical - 2) {
            cout << "||" << setw((horizontal - 2) / 2 + 12);
            SetColor(12, 0);
            cout << "Please choose an option";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 12) << "||";
        }
        else {
            cout << "||" << setw(horizontal - 2) << "||";
        }
        cout << endl;
    }
    for (int i = 1; i <= horizontal; i++) {
        SetColor(14, 0);
        cout << "-";
    }
    int option;
    cin >> option;
    switch (option) {
    case 1:
        clear();
        signup();
        break;
    case 2:
        clear();
        login();
        break;
    }
}










int main()
{

    Display_login_interface();
    return 0;

}

