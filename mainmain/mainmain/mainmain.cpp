#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip> 
#include <string>
#include <cstring>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

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
    int logincounter = 0;

};
struct transaction {
    double amount;
    string receiver_name; // will need user info
    string sender_name;   // will need user info
    int transaction_id;    // will need user info
    string currency = "$";
    string status;
};
struct admin {
    int id;
    string name;
    string email;
    string role;
    string password;
    bool is_prime = 0; // 1-> prime admin can creat enew admins 0-> cannot create new admins

    // access level- i=0 : view profiles i = 1: suspend accounts i = 2: handle disputes
    // false = no permission, true = has permission
    bool permission[3] = { 0 };
};


//ADD GLOBAL VARIABLES HERE

admin ad1;
admin ad2;
admin ad3;

vector <user> userslist;
vector <admin> adminlist;

string FakeUserPass = "test"; // replace where this is used with user pass stored in user struct when full implementation

string emailone;
string passwordone;
string emailtwo;
string passwordtwo;

const int horizontal = 120, vertical = 20;


//ADD YOUR FUNCTIONS HERE



// function that validates choices; takes 3 parameters 1. choice to validate 2. lb : lower-bound 2. ub : upper-bound
int val_choices(int choice, int lb, int ub) {
    while (choice < lb || choice >ub) {
        cout << "Invalid input. PLease enter a number between 1 and 3";
        cin >> choice;
    }
    return choice;
}
void delay(double delay = 1) {

    clock_t start_time = clock();
    while ((clock() - start_time) / 1000 < delay);
}
void clear() {
    system("cls");
}

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

admin find(const vector <admin>& userslist, admin datamatch) {
    admin matchfound;
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

bool is_there(const vector <admin>& userslist, admin datamatch) {
    for (int i = 0; i < userslist.size(); i++) {
        if (userslist[i].email == datamatch.email) {
            return true;
        }
    }
    return false;
}

string stringifyotp(int x, int n=4) {
    string s;
    while (x) s.push_back('0' + x%10), x /= 10;
    while (s.size() < n) s.push_back('0');
    reverse(s.begin(), s.end());
    return s;
}

void gl(string &s) {
    getline(cin, s);
    while (s.empty() || s[0] == '\n') getline(cin, s);
}

//declaring login function before signup() since login() is called inside signup()
void login();
void signup();
void Dashboard();

void login() {

    // user is redirected here if login option is chosen
    user data, datamatch;
    string choice;
    cout << "Please enter your email ";
    gl(data.email);

    //verfies that email is linked to an account
    //if not linked, user is prompted to either sign up or renter email
    while (!is_there(userslist, data)) {
        cout << "This email is not linked to an account\n";
        cout << "If you wish to signup please press 1\nIf you wish to renter your email please press 2\n";
        gl(choice);

        //verify input is within range
        while (choice != "1" && choice != "2") {
            cout << "Invalid choice, please renter\n";
            gl(choice);
        }
        if (choice == "1") {

            //exits current function and redirects to signup
            return signup();
        }
        else if (choice == "2") {
            gl(data.email);
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
    datamatch.logincounter++;
    //redirection to dashboard
    delay(3.0);
    clear();
    Dashboard();
}

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

void transactionfile(transaction tx) {
    ofstream("temp.txt");
    fstream O; O.open("temp.txt");
    fstream H; H.open("history.txt");
    string s;
    while (getline(H, s)) {
        O << s << endl;
    }
    H.close();
    int _status = remove("history.txt");
    ofstream("history.txt",ios::app);
    H.open("history.txt");
    if (tx.status == "completed") H << "Transaction Successful!" << endl;
    else if (tx.status == "Cancelled") H << "Transaction Failed!" << endl;
    else H << "Transaction Pending!" << endl;
    H << "Transaction ID: " << tx.transaction_id << endl;
    H << "Sender: " << tx.sender_name << endl;
    H << "Receiver: " << tx.receiver_name << endl;
    H << "Amount: " << tx.currency << tx.amount << endl;
    time_t timern = time(0);
    H << "Time: " << ctime(&timern) << endl;
    H << "-------------------------------------" << endl;
    O.seekg(0);
    while (getline(O, s)) {
        H << s << endl;
    }
    O.close(); H.close();
    _status = remove("temp.txt");
}

int ProcessTransaction(double& sender_balance, double& receiver_balance) { // to use it in main function properly
    transaction tx; // instance
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

    cout << "Please enter the currency. Type $ for dollars or type € for euros." << endl;
    while (true) {
        cin >> tx.currency;
        if (tx.currency == "$" || tx.currency == "€") {
            break;  // Valid currency input, exit loop
        }
        cout << "Invalid input. Please enter $ or €: ";
    }
   
    updated_sender_balance = UpdateSenderBalance(sender_balance, tx.amount);
    updated_receiver_balance = UpdateReceiverBalance(receiver_balance, tx.amount);
    tx.status = "completed";
    transactionfile(tx);
    cout << "Sender's New Balance: " << tx.currency << updated_sender_balance  << endl;
    cout << "Receiver's New Balance: " << tx.currency << updated_receiver_balance << endl;
    
    return 1; // returns success
}
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

void adminlogin();
void signup() {
    // user is redirected here if signup option is chosen
    user data;
    cout << setw(horizontal / 2) << "Please enter your email\n";
    gl(data.email);

    //verfiy that this email does not exist
    //if already linked, user is prompted to either login or renter email
    while (is_there(userslist, data)) {
        string choice;
        cout << setw(horizontal / 2) << "This email is already linked to an account\n";
        cout << setw(horizontal / 2) << "If you wish to login please press 1\nIf you wish to renter your email please press 2\n";
        gl (choice);
        //verify input is within range
        while (choice != "1" && choice != "2") {
            cout << setw(horizontal / 2) << "Invalid choice, please renter\n";
            gl(choice);
        }
        if (choice == "1") {
            //exits current fujnction and redirects to login
            clear();
            return login();
        }
        else if (choice == "2") {
            gl(data.email);
        }
    }
    cout << setw(horizontal / 2) << "Please enter your name\n";
    gl(data.name);
    cout << setw(horizontal / 2) << "Please enter your password\n";
    gl(data.password);
    while (data.password.size() < 8) {
        cout << "Password must contain at least 8 characters";
        gl(data.password);
    }
    cout << "\"" << data.password << "\"\n";
    cout << setw(horizontal / 2) << "Please enter phone number in the format: +CCXXXXXXXXXX\n";
    gl(data.contactinfo);
    while (data.contactinfo.size() != 11) {
        cout << "your phone number must be 11 digits long";
        gl(data.contactinfo);
    }
    //adding id as size of set + 1 as the new user has not been inserted yet
    data.ID = userslist.size() + 1;
    userslist.push_back(data);

    //randomizing OTP
    cout << setw(horizontal / 2) << "This is your OTP\n";
    srand(time(0));
    string OTP, renter;
    //generates a 4 digit OTP
    OTP = stringifyotp(rand() % 10000);
    cout << OTP << setw(horizontal / 2) << "\nPlease renter the OTP\n";
    cin >> renter;

    //Verify the OTP has been rentered correctly
    //user has 3 tries to enter OTP, if still incorrect, OTP changes
    while (renter != OTP) {
        for (int i = 0; renter != OTP && i < 2; i++) {
            cout << setw(horizontal / 2) << "OTP does not match, please renter\n" << 2 - i << " tr" << (2-i!=1?"ies":"y") << " left\n";
            cin >> renter;
        }
        if (renter == OTP) {
            break;
        }
        cout << setw(horizontal / 2) << "Your OTP has changed\n";
        cout << setw(horizontal / 2) << "This is your new OTP\n";
        srand(time(0));
        OTP = stringifyotp(rand() % 10000);
        cout << OTP << "\nPlease renter the OTP\n";
        cin >> renter;
    }


    cout << "Signup succesfull\nRedirecting to dashboard...\n";
    //redirection to dashboard

    delay(3.0);
    clear();
    Display_login_interface();
}


void Display_login_interface() {
    for (int i = 1; i <= horizontal; i++) {
        SetColor(14, 0);
        cout << "-";  //Printing the upper horizontal lines along with setting its colors to yellow
    }
    cout << endl;
    for (int i = 1; i <= vertical; i++) {
        SetColor(14, 0);
        if (i == 2) {
            cout << "||" << setw((horizontal - 2) / 2 + 10); //Formula for each line tht contatins text
            SetColor(15, 0);
            cout << "Welcome to Instapay";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 10) << "||";  // Making the title
        }
        else if (i == vertical / 2 - 1) {
            cout << "||";
            cout << string(((horizontal - 2) / 2 - 6), ' ');
            SetColor(15, 5);
            cout << "Sign up (1)";
            SetColor(14, 0);
            cout << string(((horizontal - 2) / 2 - 7), ' ');
            cout << "||"; //Making the sign up button
        }
        else if (i == vertical / 2) {
            cout << "||";
            cout << string(((horizontal - 2) / 2 - 6), ' ');
            SetColor(15, 5);
            cout << "Sign in (2)";
            SetColor(14, 0);
            cout << string(((horizontal - 2) / 2 - 7), ' '); 
            cout << "||"; //Making the sign in button
        }
        else if (i == vertical / 2 + 1) {
            cout << "||";
            cout << string(((horizontal - 2) / 2 - 10), ' ');
            SetColor(15, 5);
            cout << "Sign in as admin (3)";
            SetColor(14, 0);
            cout << string(((horizontal - 2) / 2 - 12), ' ');
            cout << "||"; //Making the sgining in as admin button
        }
        else if (i == vertical - 2) {
            cout << "||" << setw((horizontal - 2) / 2 + 8);
            SetColor(10, 0);
            cout << "Choose an option";
            SetColor(14, 0);
            cout << setw((horizontal - 2) / 2 - 8) << "||"; // Choose an option comment
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
    case 3:
        clear();
        adminlogin();
        break;
    } //Making cases for each of the three options in the user interface
}

void Dashboard();
void Account();
void ChangeEmail();
void center(string& text, int boxwidth);
void ChangePass();
void ChangeContactInfo();

void center(string& text, int boxwidth = 50) {
	int numspc = (boxwidth - text.length()) / 2;

	cout << endl << string(numspc, ' ') << text << endl;

}

void Dashboard() { //function to output list of commands that can be used in dashboard
	string dashboard[] = {

		"   choose a number to navigate   ",
		"   user.name   ", //output name stored in user struct
		"   user.balance   ", //output balance stored in user struct
		"   Send Money (1)                 Account Management (2)   ",/*PLEASE DONT REMOVE THE BLANK OUTPUT!!!*/ "",// (1) redirects to money transfer page (2) redirects to account page
		"   Most recent transaction   ",
		"   (Function of most recent transaction)   ", //should output data for the most recent transaction (sent/recieved - value - date - status)
		"   Open Transaction History (3)   "

	};
	int n = 8;
	for (int i = 0; i < n; i++) {
		if (i != 1 && i != 2) {
			center(dashboard[i], 100);
		}
		else {
			cout << endl << dashboard[i] << endl;
		}
	}
	int dashboard_nav;
	cin >> dashboard_nav;
	while (dashboard_nav != 1 && dashboard_nav != 2 && dashboard_nav != 3) {
		cout << "invalid number please re-enter navigation numeber";
		cin >> dashboard_nav;
	}

	switch (dashboard_nav) {

	case 1:
		clear();
		cout << "1";
		break;//redirect to money transfer
	case 2:
		clear();
		Account();

	case 3:
		clear();
		cout << "3";
		break;//redirect to transaction history

	}
}

void Account() {

	int accountnav;

	string account[] = {
		"\tID:           ","user.id",
		"\tUsername:     ", "user.name",
		"\tEmail:        ", "user.email",
		"\tChange email (1)",
		"\tPassword:     ", string(FakeUserPass.length(), '*'),
		"\tChange password (2)",
		"\tContact Info: ","user.contactinfo",
		"\tChange contact info (3)",
		"\tLogout (4)",
		"\tBack to dashboard(5)"
	};

	int n = 15;
	for (int i = 0; i < n; i++) {
		if (i == 0 || i == 2 || i == 4 || i == 7 || i == 10) {
			cout << account[i];
		}
		else cout << account[i] << endl;
	}

	cin >> accountnav;
	cin.ignore();
	while (accountnav != 1 && accountnav != 2 && accountnav != 3 && accountnav != 4 && accountnav != 5) {
		cout << "invalid number please re-enter navigation number ";
		cin >> accountnav;
		cin.ignore();
	}

	switch (accountnav) {

	case 1:
		clear();
		ChangeEmail();
		break;
	case 2:
		clear();
		ChangePass();
		break;
	case 3:
		clear();
		ChangeContactInfo();
		break;
	case 4:
		clear();
		cout << endl << "logged out" << endl;     // send user back to login page and clear currently used user data
		break;
	case 5:
		clear();
		Dashboard();
		break;
	}

}

void ChangeEmail() { //change email function
	int fakeOTP;//will be used to compare with real OTP 
	string FakeUserEmail = "fakeemail";//will be replaced with email struct value
	string TempMail;
	string UserEmailVerify;

	bool masterbreaker = 0;

	cout << "please enter the OTP sent to your phone number \n";

	cin >> fakeOTP;
	cin.ignore();

	TempMail = FakeUserEmail;

	if (fakeOTP == 1234/*correct OTP*/) {

		cout << "Enter your new email:\n";





		while (masterbreaker == 0) {

			getline(cin, TempMail);

			if (TempMail.find('@') == string::npos) {//checks that email has @

				cout << "Email must contain @ please enter a valid email\n";
				continue;
			}
			else {

				if (TempMail.length() < 4) {

					cout << "email must be atleast 4 characters\nplease enter a valid email\n";
					continue;
				}
				else {
					bool continuefrominsideloop = 0;
					for (int i = 0; i < TempMail.length(); i++) {//check that email doesnt contain spaces

						if (TempMail[i] == ' ') {

							cout << "Email cant contain spaces please enter a valid email \n";
							continuefrominsideloop = 1;
							break;
						}
					}
					if (continuefrominsideloop == 1) {
						continue;
					}

				}

			}
			masterbreaker = 1;
		}
		cout << "\nValidation passed\n\n";


		cout << "Please re-enter your new email:\n";

		getline(cin, UserEmailVerify);

		int i = 0;

		while (TempMail.compare(UserEmailVerify) != 0) {

			if (i == 3) {
				cout << "\nEmail couldn't be changed\nReason: Max number of tries reached\nyou will be returned to account page\n";
				delay(3.0);
				break;
			}

			cout << "Emails dont match please re-enter your email:\n";
			cout << 3 - i << " tries left \n";

			getline(cin, UserEmailVerify);
			i++;
		}

		cout << "Enter the OTP that was sent to the new email\n";
		cin >> fakeOTP;
		cin.ignore();
		for (int i = 0; i < 5; i++) {
			if (i >= 3) {
				cout << "Email couldnt be changed\nReason: out of tries\n";
				delay(3.0);
				break;
			}
			if (fakeOTP == 1234) {

				FakeUserEmail = TempMail;
				cout << "Email successfully changed\nYou will be returned to account page\n";
				delay(3.0);
				break;
			}
			else {

				cout << "OTP is incorrect " << 3 - i << " tries left\n";
				cin >> fakeOTP;

			}

		}
	}
	else {
		cout << "\nEmail couldn't be changed\nReason: OTP is incorrect\nyou will be returned to account page\n";
		delay(3.0);
	}

	clear();

	Account();

}

void ChangePass() {

	bool masterbreaker = 0;

	string PassVerify;
	string userpasstemp = FakeUserPass;

	cout << "Please enter current password: ";

	getline(cin, PassVerify);

	int i = 0;

	while (i < 3) {

		if (userpasstemp.compare(PassVerify) != 0) {//compare original password with the password entered

			cout << "Password is incorrect re-enter your password: " << 3 - i << " tries left\n";

			getline(cin, PassVerify);

			i++;

		}
		else {

			break;

		}
	}
	if (i != 3) {
		while (true) {

			cout << "Enter your new password: \n";

			getline(cin, userpasstemp);

			cout << "Re-enter your password: \n";

			getline(cin, PassVerify);

			if (userpasstemp.compare(PassVerify) != 0) {

				cout << "Passwords dont match\n";

			}
			else {

				if (PassVerify.empty()) {

					cout << "Password cannot be empty\n";

				}
				else {

					break;
				}
			}
		}
	}

	FakeUserPass = userpasstemp;

	cout << "Password changed successfully\nReturning to account page";

	delay(3.0);

	clear();

	Account();

}

void ChangeContactInfo() {

	bool masterbreaker = 0;

	string Userdotcontactinfo;
	string PhoneNumberVerify;

	int FakeOTP;//will be replaced with real OTP

	cout << "Please enter an OTP sent to your Email: \n";
	cin >> FakeOTP;
	cin.ignore();
	if (FakeOTP == 1234) {//entered OTP compare to real OTP

		cout << "Please enter your new Phone number in the format: +CCXXXXXXXXXX\n";

		cin >> PhoneNumberVerify;
		cin.ignore();

		PhoneNumberVerify[0] = '+';

		while (masterbreaker == 0) {
			if (PhoneNumberVerify.length() != 13) { //checks if entered number is 13 character long ex: +20XXXXXXXXXX

				cout << "phone number must have 13 digits\nPlease re-enter your new phone number in the format: +CCXXXXXXXXXX\n";

				cin >> PhoneNumberVerify;
				cin.ignore();

				PhoneNumberVerify[0] = '+';


				for (int i = 1; i < 13; i++) {//first character will be always '+' so i starts with the second character ⁂int i = 1;

					if (PhoneNumberVerify[i] < '0' || PhoneNumberVerify[i] > '9') {//check if the character entered in PhoneNumberVerify is a digit

						cout << "Phone number cant contain characters other than [0-9]\nPlease re-enter your phone number in the format: +CCXXXXXXXXXX\n";
						cin >> PhoneNumberVerify;
						cin.ignore();
						PhoneNumberVerify[0] = '+';

						i = 0;

					}
				}
			}
			else {
				break;

			}
		}
		if (masterbreaker == 0) {


			cout << "Please enter the OTP sent to the new phone number:\n";

			cin >> FakeOTP;

			if (FakeOTP == 1234) {//checks if OTP entered is same as real OTP

				Userdotcontactinfo = PhoneNumberVerify;

				cout << "Phone number changed successfully!!\nReturning to account page";

				delay(4.0);

			}
			else {
				for (int i = 0; i < 5; i++) {
					if (i == 3) {
						cout << "failed to change phone number\nReason:out of tries\nReturning to account page";
						delay(4.0);
						break;
					}
					cout << "OTP is incorrect please re-enter " << 3 - i << " tries left\n";
					cin >> FakeOTP;
					if (FakeOTP == 1234) {

						Userdotcontactinfo = PhoneNumberVerify;

						cout << "Phone number changed successfully!! \nReturning to account page";

						delay(4.0);

						break;
					}
				}
			}
		}



	}
	else {

		cout << "failed to change phone number\nReason: failed to enter OTP\nReturning to account page";
		delay(4.0);

	}
	clear();
	Account();
}

//function that should only work when prime admin to add new admins
admin AdminInfo(admin reg) {
    cout << "enter Admin's ID";
    cin >> reg.id;
    cout << "enter Admin's name";
    cin.ignore();  // clear input buffer
    getline(cin, reg.name);
    cout << "enter Admin's email";
    cin >> reg.email;
    cout << "enter Admin's role"; // it will just appear in the dashboard (Doesnt affect code)

    int choice; // Prime Admin choices
    cout << "enter Admin's permissions";
    // permission 1
    cout << "can they view profiles\n1. yes\n2. no : ";
    cin >> choice;
    choice = val_choices(choice, 1, 2);
    switch (choice) {
    case 1:
        reg.permission[0] = 1; // store permission in array
        break;
    default:
        reg.permission[0] = 0;
    }

    // permission 2
    cout << "can they suspend accounts\n1. yes\n2. no : ";
    cin >> choice;
    choice = val_choices(choice, 1, 2);
    switch (choice) {
    case 1:
        reg.permission[1] = 1;
        break;
    default:
        reg.permission[1] = 0;
    }

    // permission 3
    cout << "can they handle disputes\n1. yes\n2. no : ";
    cin >> choice;
    choice = val_choices(choice, 1, 2);
    switch (choice) {
    case 1:
        reg.permission[2] = 1;
        break;
    default:
        reg.permission[2] = 0;
    }

    return reg;
}

void adminlogin() {
    admin data, datamatch;
    string choice;
    cout << "Please enter your email ";
    gl(data.email);
    while (!is_there(adminlist, data)) {
        cout << "This email is not linked to an account\n";
        cout << "Please contact a prime admin to add your account or press 1 to re-enter your email\n";
        gl(choice);

        //verify input is within range
        while (choice != "1") {
            cout << "Invalid choice, please renter\n";
            gl(choice);
        }
        if (choice == "1") {
            gl(data.email);
            
        }
   
    }
    //password checking
    cout << "Account located successfully\nPlease enter your password\n";
    string password;
    gl(password);
    datamatch = find(adminlist, data);
    while (datamatch.password != password) {
        cout << "Incorrect password, please renter your password\n";
        gl(password);
    }
    cout << "Password confirmed\nRedirecting to dashboard...\n";
    //redirection to admin dashboard
}

void displayhistory() {
    fstream H; H.open("history.txt");
    string s;
    while (getline(H, s)) cout << s << endl;
    return;
}

int main()
{
    // initialized prime admins
    ad1.id = 0; ad1.name = "omar"; ad1.email = "omar@gmail.com"; ad1.is_prime = 1; ad1.permission[0] = 1; ad1.permission[1] = 1; ad1.permission[2] = 1; ad1.role = "manager";
    ad2.id = 0; ad2.name = "jasmin"; ad2.email = "jasmin@gmail.com"; ad2.is_prime = 1; ad2.permission[0] = 1; ad2.permission[1] = 1; ad2.permission[2] = 1; ad2.role = "moderator";
    ad3.id = 0; ad2.name = "jasmin"; ad3.email = "jasmin@gmail.com"; ad3.is_prime = 1; ad3.permission[0] = 1; ad3.permission[1] = 1; ad3.permission[2] = 1; ad3.role = "branch manager";
    adminlist.push_back(ad1); adminlist.push_back(ad2); adminlist.push_back(ad3);

    while (true) {
        Display_login_interface();
        clear();
    }
    /*Account();*/
    return 0;
}