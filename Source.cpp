#include <iostream>
#include <fstream>
#include<sstream>
#include<string>
#include<cstring>

using namespace std;

struct Appointment;
struct Time
{
	int Day, Hour, Minute, Second;
};
struct Employee
{
	int UniqueID;
	char FirstName[30], LastName[30], EmailAddress[50];
	Appointment* Calendar;
	Employee* next;
	Employee* previous;
};
struct Company
{
	Employee* Head, * Tail;
};
struct Attendee
{
	Employee* self;
	Attendee* next;
};
struct Appointment
{
	char Title[50];
	Time StartingTime;
	int Duration;
	Attendee* ListOfAttendees;
	Appointment* next;
};

void InitializeCompany(Company &); //function 0
bool CompanyIsEmpty(Company); //function 00
Company ParseInputFile(string); // function 1
Employee SearchForEmployee(); //function 2
void AddEmployee(Company &, Employee); //function 3
void DeleteEmployee(Company &, Employee); //function 4
void AddMeeting(Company&, Appointment *); //function 5
void CancelMeeting(Company&, Appointment *); //function 6
void DeleteAllMeetings(Appointment*&, Time); //function 7
Time ProposeMeetingSchedule(Employee*, Time); //function 8; return type: array of time(timeslots),,, the argument is the minimum duration
void UpdateOutputFile(string, Company); //function 9

void InitializeCompany(Company &com)
{
	com.Head = NULL;
	com.Tail = NULL;
}

bool CompanyIsEmpty(Company com)
{
	return(com.Head == NULL || com.Tail == NULL);
}

Company ParseInputFile(string filename)
{
	Company com;
	string line; //the lines from the file will be read into this variable
	InitializeCompany(com); 

	ifstream inFile;
	inFile.open(filename + ".txt");
	if (!inFile) //if input file does not exist
	{
		ofstream firstTime(filename + ".txt");
		firstTime.close();
	}

	Employee* cur;
	string id, firstname, lastname, email, meetingTitle, meetingDay, meetingHour, meetingMinute, meetingSecond, meetingDuration, meetingAttendeeID; //these strings are used to temporarily store components from the input file

	//start by populating the the head of the company
	if (CompanyIsEmpty(com))
	{
		cur = new Employee;
		cur->next = NULL;
		cur->previous = NULL;

		getline(inFile, line);
		stringstream ss(line);

		getline(ss, id, ',');
		getline(ss, firstname, ',');
		getline(ss, lastname, ',');
		getline(ss, email, '#');

		//now convert the strings into the convenient types and place them to their employee
		cur->UniqueID = stoi(id); //std::stoi takes a string as parameter and returns it as int
		strcpy(cur->FirstName, firstname.c_str()); //strcpy converts string into an array of char
		strcpy(cur->LastName, lastname.c_str());
		strcpy(cur->EmailAddress, email.c_str());

		com.Head = cur;
		com.Tail = com.Head;
	} //now we have our first employee in the company

	//then continue the rest of the list
	while (getline(inFile, line))
	{
		cur = new Employee;
		cur->next = NULL;
		cur->previous = NULL;
		stringstream ss(line);
		getline(ss, id, ',');
		getline(ss, firstname, ',');
		getline(ss, lastname, ',');
		getline(ss, email, '#');

		//now convert the strings into the convenient types and place them to their employee
		cur->UniqueID = stoi(id); //std::stoi takes a string as parameter and returns it as int
		strcpy(cur->FirstName, firstname.c_str()); //strcpy converts string into an array of char
		strcpy(cur->LastName, lastname.c_str());
		strcpy(cur->EmailAddress, email.c_str());

		cur->previous = com.Tail;
		com.Tail->next = cur;
		com.Tail = cur;
	}
	
	inFile.close();
	return com;
}

int main()
{
	string filename = "Input";
	Company myCompany;

	myCompany = ParseInputFile(filename);
	for (Employee* counter = myCompany.Head; counter != NULL; counter = counter->next)
	{
		cout << counter->UniqueID <<endl;
		cout << counter->FirstName << endl;
		cout << counter->LastName << endl;
		cout << counter->EmailAddress << endl;
		cout << "----------------------------------\n";
	}

	return 0;
}