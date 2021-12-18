#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdlib.h> // use rand()
#include<ctime> //use time functions

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

void InitializeCompany(Company&); //function 0
bool CompanyIsEmpty(Company); //function 00
Company ParseInputFile(std::string); // function 1
Employee* SearchForEmployee(Company, int); //function 2
void AddEmployee(Company&); //function 3
void DeleteEmployee(Company&, int); //function 4
void AddMeeting(Company&); //function 5
void CancelMeeting(Company&, Appointment*); //function 6
void DeleteAllMeetings(Company&, int); //function 7
Time ProposeMeetingSchedule(Employee*, Time); //function 8; return type: array of time(timeslots),,, the argument is the minimum duration
void UpdateOutputFile(std::string, Company); //function 9


void InitializeCompany(Company& com)
{
	com.Head = NULL;
	com.Tail = NULL;
}

void InitializeCalendar(Appointment*& ap)
{
	ap = NULL;
}

void InitializeAttendees(Attendee*& at, Employee* self)
{
	at = NULL;

	Attendee* tmp = new Attendee;
	if (tmp == NULL)
		exit(1);

	tmp->self = self;

	tmp->next = at;
	at = tmp;
}

bool CompanyIsEmpty(Company com)
{
	return(com.Head == NULL || com.Tail == NULL);
}

bool CalendarIsEmpty(Appointment* ap)
{
	return(ap == NULL);
}

bool AttendeesIsEmpty(Attendee* at)
{
	return(at == NULL);
}

Company ParseInputFile(std::string filename)
{
	Company com;
	std::string line; //the lines from the file will be read into this variable
	InitializeCompany(com);

	std::ifstream inFile;
	inFile.open(filename + ".txt");
	if (!inFile) //if input file does not exist
	{
		std::ofstream firstTime(filename + ".txt");
		firstTime.close();
	}

	Employee* cur;

	std::string id, firstname, lastname, email, meetingTitle, meetingDay, meetingHour, meetingMinute, meetingSecond, meetingDuration, meetingAttendeeID; //these strings are used to temporarily store components from the input file

	//start by populating the the head of the company
	if (CompanyIsEmpty(com))
	{
		//create temporary employee
		cur = new Employee;
		if (cur == NULL)
			exit(1);
		cur->next = NULL;
		cur->previous = NULL;

		getline(inFile, line);
		std::stringstream ss(line);

		std::getline(ss, id, ',');
		std::getline(ss, firstname, ',');
		std::getline(ss, lastname, ',');
		std::getline(ss, email, '#');
		//now convert the strings into the convenient types and place them to their employee
		cur->UniqueID = stoi(id); //std::stoi takes a string as parameter and returns it as int
		strcpy(cur->FirstName, firstname.c_str()); //strcpy converts string into an array of char
		strcpy(cur->LastName, lastname.c_str());
		strcpy(cur->EmailAddress, email.c_str());

		InitializeCalendar(cur->Calendar);
		while (getline(ss, meetingTitle, '|') && meetingTitle != "mmm")
		{
			Appointment* tmpCal = new Appointment;
			if (tmpCal == NULL)
				exit(1);
			tmpCal->next = NULL;

			getline(ss, meetingDay, ',');
			getline(ss, meetingHour, ',');
			getline(ss, meetingMinute, ',');
			getline(ss, meetingSecond, ',');
			getline(ss, meetingDuration, '!');

			strcpy(tmpCal->Title, meetingTitle.c_str());
			tmpCal->StartingTime.Day = stoi(meetingDay);
			tmpCal->StartingTime.Hour = stoi(meetingHour);
			tmpCal->StartingTime.Minute = stoi(meetingMinute);
			tmpCal->StartingTime.Second = stoi(meetingSecond);
			tmpCal->Duration = stoi(meetingDuration);

			InitializeAttendees(tmpCal->ListOfAttendees, cur);

			while (getline(ss, meetingAttendeeID, ',') && stoi(meetingAttendeeID) != 0)
			{

				Attendee* tmpAt = new Attendee;
				if (tmpAt == NULL)
					exit(1);
				Employee* tmpEm = new Employee;
				if (tmpEm == NULL)
					exit(1);
				tmpEm->UniqueID = stoi(meetingAttendeeID);
				tmpAt->self = tmpEm;
				tmpAt->next = tmpCal->ListOfAttendees;
				tmpCal->ListOfAttendees = tmpAt;
			}
			tmpCal->next = cur->Calendar;
			cur->Calendar = tmpCal;

		}

		com.Head = cur;
		com.Tail = com.Head;
	} //now we have our first employee in the company

	//then continue the rest of the list
	while (getline(inFile, line))
	{
		cur = new Employee;
		if (cur == NULL)
			exit(1);
		cur->next = NULL;
		cur->previous = NULL;

		std::stringstream ss(line);
		std::getline(ss, id, ',');
		std::getline(ss, firstname, ',');
		std::getline(ss, lastname, ',');
		std::getline(ss, email, '#');
		//now convert the strings into the convenient types and place them to their employee
		cur->UniqueID = stoi(id); //std::stoi takes a string as parameter and returns it as int
		strcpy(cur->FirstName, firstname.c_str()); //strcpy converts string into an array of char
		strcpy(cur->LastName, lastname.c_str());
		strcpy(cur->EmailAddress, email.c_str());

		InitializeCalendar(cur->Calendar);
		while (getline(ss, meetingTitle, '|') && meetingTitle != "mmm")
		{
			Appointment* tmpCal = new Appointment;
			if (tmpCal == NULL)
				exit(1);
			tmpCal->next = NULL;

			getline(ss, meetingDay, ',');
			getline(ss, meetingHour, ',');
			getline(ss, meetingMinute, ',');
			getline(ss, meetingSecond, ',');
			getline(ss, meetingDuration, '!');
			strcpy(tmpCal->Title, meetingTitle.c_str());
			tmpCal->StartingTime.Day = stoi(meetingDay);
			tmpCal->StartingTime.Hour = stoi(meetingHour);
			tmpCal->StartingTime.Minute = stoi(meetingMinute);
			tmpCal->StartingTime.Second = stoi(meetingSecond);
			tmpCal->Duration = stoi(meetingDuration);

			InitializeAttendees(tmpCal->ListOfAttendees, cur);

			while (getline(ss, meetingAttendeeID, ',') && stoi(meetingAttendeeID) != 0)
			{
				Attendee* tmpAt = new Attendee;
				if (tmpAt == NULL)
					exit(1);
				Employee* tmpEm = new Employee;
				if (tmpEm == NULL)
					exit(1);
				tmpEm->UniqueID = stoi(meetingAttendeeID);
				tmpAt->self = tmpEm;
				tmpAt->next = tmpCal->ListOfAttendees;
				tmpCal->ListOfAttendees = tmpAt;
			}
			tmpCal->next = cur->Calendar;
			cur->Calendar = tmpCal;
		}

		cur->previous = com.Tail;
		com.Tail->next = cur;
		com.Tail = cur;
	}

	inFile.close();
	return com;
}

Employee* SearchForEmployee(Company c, int id)
{
	if (CompanyIsEmpty(c))
		return NULL;

	Employee* current = c.Head;
	while (current != NULL)
	{
		if (current->UniqueID == id)
		{
			return current;
		}

		current = current->next;
	}
	return NULL;
}

//verify unique ID
bool IDcheck(Company c, int newID)
{
	Employee* cur = c.Head;

	while(cur!=NULL)
	{
		if (cur->UniqueID == newID)
			return false;
		cur = cur->next;
	}

	return true;
}

int inputID(Company c)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

	srand((unsigned int)time(NULL)); //get more random results

	int id = (1900 + ltm->tm_year) * 100000 + rand() % 100000; //the id starts with the year the employee was added, followed by 5 random digits

	while (IDcheck(c, id) == false) //ensure given random id doesnt already exist
	{
		id = rand();
		return id;
	}
	return id;
}

void InputAttendees(Company c, Attendee* &head) //the attendees to add to a meeting must be already created in the company
{
	char x;
	do
	{
		Employee* e = new Employee;
		if (e == NULL)
			exit(1);
		e->next = NULL;

		std::cout << "Enter the id of the attendee: ";
		std::cin >> e->UniqueID;

		if (SearchForEmployee(c, e->UniqueID) == NULL)
		{
			std::cout << "Employee not found\n";
			return;
		}

		Attendee* at = new Attendee;
		if (at == NULL)
			exit(1);
		at->next = NULL;

		InitializeAttendees(at, e);

		at->next = head;
		head = at;

		std::cout << "Add another attendee?\n";
		std::cin >> x;

	} while (x == 'y' || x == 'Y');

}

void AddMeeting(Company& c)
{
	Appointment* newAp = new Appointment;
	if (newAp == NULL)
		exit(1);
	newAp->next = NULL;
	newAp->ListOfAttendees = NULL;

	std::cout << "Enter the title of this meeting: ";
	std::cin >> newAp->Title;
	std::cout << "Enter the day of the meeting: ";
	std::cin >> newAp->StartingTime.Day;
	std::cout << "Enter the startimg hour: ";
	std::cin >> newAp->StartingTime.Hour;
	std::cout << "Enter the starting minute: ";
	std::cin >> newAp->StartingTime.Minute;
	std::cout << "Enter the starting second: ";
	std::cin >> newAp->StartingTime.Second;
	std::cout << "enter the duration of this meeting: ";
	std::cin >> newAp->Duration;

	InputAttendees(c, newAp->ListOfAttendees);

	// add the meeting to all the attendees' calendars

	for (Attendee* at = newAp->ListOfAttendees; at != NULL; at = at->next)
	{
		for (Employee* e = c.Head; e != NULL; e = e->next)
		{
			if (at->self->UniqueID == e->UniqueID)
			{
				newAp->next = e->Calendar;
				e->Calendar = newAp;
			}
		}
	}

}


void AddEmployee(Company& com)
{
	Employee* emp = new Employee;
	if (emp == NULL)
	{
		std::cout << "Fault in creation\n";
		exit(1);
	}

	//a random ID will be given
	emp->UniqueID = inputID(com);
	
	std::cout << "input the employee first name: ";
	std::cin >> emp->FirstName;
	std::cout << "input the employee last name: ";
	std::cin >> emp->LastName;
	std::cout << "input the employee email: ";
	std::cin >> emp->EmailAddress;
	emp->Calendar = NULL;

	emp->next = com.Head;
	com.Head->previous = emp;
	com.Head = emp;
}

void DeleteAttendee(Company & com, Employee* del)
{
	for (Employee* e = com.Head; e != NULL; e = e->next)
	{
		if (e->Calendar == NULL) // if employee will not attend any meeting
			continue;

		for (Appointment* ap = e->Calendar; ap != NULL; ap = ap->next)
		{
			if (ap->ListOfAttendees == NULL) //if meeting does not yet have any attendees
				continue;

			Attendee* tmp, * prev, *cur;
			if (del->UniqueID == ap->ListOfAttendees->self->UniqueID)
			{
				tmp = ap->ListOfAttendees->next;
				delete ap->ListOfAttendees;
				ap->ListOfAttendees = tmp;
				break;
				
			}
			//if attendee is not at head
			prev = NULL;
			cur = ap->ListOfAttendees;
			while (cur != NULL && cur->self->UniqueID != del->UniqueID)		
			{
				prev = cur;
				cur = cur->next;
			}

			if (cur != NULL)
			{
				//delete
				prev->next = cur->next;
				delete cur;
			}
			
		}
	}
}

void DeleteEmployee(Company& com, int id)
{
	if (CompanyIsEmpty(com))
	{
		std::cout << "Company is empty";
		return;
	}	

	Employee* toBeDeleted = new Employee;
	if (toBeDeleted == NULL)
		exit(1);
	toBeDeleted = NULL;

	toBeDeleted = SearchForEmployee(com, id);
	if (toBeDeleted == NULL)
	{
		std::cout << "ID does not belong to any existing employee\n";
		return;
	}
	
	//now delete this employee from all appointments
	DeleteAttendee(com, toBeDeleted);

	Employee *cur;

	//delete head
	if (toBeDeleted == com.Head)
	{
		cur = com.Head->next;
		delete com.Head;
		com.Head = cur;
		com.Head->previous = NULL;
		return;
	}

	//delete tail
	if (toBeDeleted == com.Tail)
	{
		cur = com.Tail->previous;
		delete com.Tail;
		com.Tail = cur;
		com.Tail->next = NULL;
		return;
	}

	//delete other than head or tail
	Employee* prev = toBeDeleted->previous;
	prev->next = toBeDeleted->next;
	toBeDeleted->next->previous = toBeDeleted->previous;

	delete toBeDeleted;
}

void CancelMeeting(Company& com, std::string delTitle)
{
	for (Employee* e = com.Head; e != NULL ; e = e->next)
	{
		if (e->Calendar == NULL) //if the employee does not have any meeting
			continue;

		Appointment* tmp, * prev, * cur;
		if (delTitle == std::string(e->Calendar->Title))
		{
			tmp = e->Calendar->next;
			delete e->Calendar;
			e->Calendar = tmp;
		}
		else
			
			{
				//if app is not at head
				prev = NULL;
				cur = e->Calendar;
				while (cur != NULL && std::string(cur->Title) != delTitle)
				{
					prev = cur;
					cur = cur->next;
				}

				if (cur != NULL)
				{
					//delete
					prev->next = cur->next;
					delete cur;
				}
			}	
	}
}

void DeleteAllMeetings(Company &com, int dayLim)
{
	for (Employee* e = com.Head; e != NULL; e = e->next)
	{
		if (e->Calendar == NULL) //if the employee does not have any meeting
			continue;

			if (e->Calendar->StartingTime.Day <= dayLim)
				CancelMeeting(com, std::string(e->Calendar->Title));
	}
}

//only used to print date in appropriate format
std::string DateFormat(Appointment ap)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);


	const int MAXDAYS = 365;
	const int MAXDAYSleap = 366;

	//the day of 365 or 366 in which each month ends
	const int JAN = 31;
	int FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC;

	std::string date;
	int  month{}, day{};

	if ((1900 + ltm->tm_year) % 4 != 0)
	{
		FEB = 59;
		MAR = 90;
		APR = 120;
		MAY = 151;
		JUN = 181;
		JUL = 212;
		AUG = 243;
		SEP = 273;
		OCT = 304;
		NOV = 334;
		DEC = 365;
	}
	else
	{
		FEB = 60;
		MAR = 91;
		APR = 121;
		MAY = 152;
		JUN = 182;
		JUL = 213;
		AUG = 244;
		SEP = 274;
		OCT = 305;
		NOV = 335;
		DEC = 366;
	}


	if (ap.StartingTime.Day >= 1 && ap.StartingTime.Day <= JAN)
	{
		month = 1;
		day = ap.StartingTime.Day;
	}
	if (ap.StartingTime.Day >= JAN + 1 && ap.StartingTime.Day <= FEB)
	{
		month = 2;
		day = ap.StartingTime.Day - JAN;
	}
	if (ap.StartingTime.Day >= FEB + 1 && ap.StartingTime.Day <= MAR)
	{
		month = 3;
		day = ap.StartingTime.Day - FEB;
	}
	if (ap.StartingTime.Day >= MAR + 1 && ap.StartingTime.Day <= APR)
	{
		month = 4;
		day = ap.StartingTime.Day - MAR;
	}
	if (ap.StartingTime.Day >= APR + 1 && ap.StartingTime.Day <= MAY)
	{
		month = 5;
		day = ap.StartingTime.Day - APR;
	}
	if (ap.StartingTime.Day >= MAY + 1 && ap.StartingTime.Day <= JUN)
	{
		month = 6;
		day = ap.StartingTime.Day - MAY;
	}
	if (ap.StartingTime.Day >= JUN + 1 && ap.StartingTime.Day <= JUL)
	{
		month = 7;
		day = ap.StartingTime.Day - JUN;
	}
	if (ap.StartingTime.Day >= JUL + 1 && ap.StartingTime.Day <= AUG)
	{
		month = 8;
		day = ap.StartingTime.Day - JUL;
	}
	if (ap.StartingTime.Day >= AUG + 1 && ap.StartingTime.Day <= SEP)
	{
		month = 9;
		day = ap.StartingTime.Day - AUG;
	}
	if (ap.StartingTime.Day >= SEP + 1 && ap.StartingTime.Day <= OCT)
	{
		month = 10;
		day = ap.StartingTime.Day - SEP;
	}
	if (ap.StartingTime.Day >= OCT + 1 && ap.StartingTime.Day <= NOV)
	{
		month = 11;
		day = ap.StartingTime.Day - OCT;
	}
	if (ap.StartingTime.Day >= NOV + 1 && ap.StartingTime.Day <= DEC)
	{
		month = 12;
		day = ap.StartingTime.Day - NOV;
	}

	date = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(1900 + ltm->tm_year) + " at ";
	if (ap.StartingTime.Hour < 10)
		date += "0" + std::to_string(ap.StartingTime.Hour) + ":";
	else
		date += std::to_string(ap.StartingTime.Hour) + ":";
	if (ap.StartingTime.Minute < 10)
		date += "0" + std::to_string(ap.StartingTime.Minute) + ":";
	else
		date += std::to_string(ap.StartingTime.Minute) + ":";
	if (ap.StartingTime.Second < 10)
		date += "0" + std::to_string(ap.StartingTime.Second);
	else
		date += std::to_string(ap.StartingTime.Second);

	return date;
}

std::string DurationFormat(Appointment ap)
{
	int hour, minute, second;
	std::string dur;

	hour = ap.Duration / 3600;
	minute = (ap.Duration - hour * 3600) / 60;
	second = ap.Duration - hour * 3600 - minute * 60;

	if (hour < 10)
		dur += "0" + std::to_string(hour) + ":";
	else
		dur += std::to_string(hour) + ":";
	if (minute < 10)
		dur += "0" + std::to_string(minute) + ":";
	else
		dur += std::to_string(minute) + ":";
	if (second < 10)
		dur += "0" + std::to_string(second);
	else
		dur += std::to_string(second);

	return dur;
}

void PrintEmployeeDetails(Company myCompany)
{
	for (Employee* counter = myCompany.Head; counter != NULL; counter = counter->next)
	{
		std::cout << counter->UniqueID << std::endl;
		std::cout << counter->FirstName << std::endl;
		std::cout << counter->LastName << std::endl;
		std::cout << counter->EmailAddress << std::endl;
		std::cout << "Will attend the following meetings:\n";
		std::cout << "\n--\n";
		for (Appointment* j = counter->Calendar; j != NULL; j = j->next)
		{
			std::cout << j->Title << std::endl;
			std::cout << "Meeting date: " << DateFormat(*j) << "(local time) ";
			std::cout << "Duration: " << DurationFormat(*j) << std::endl;
;			std::cout << "Meeting Attendees:\n";
			for (Attendee* i = j->ListOfAttendees; i != NULL; i = i->next)
			{
				std::cout << i->self->UniqueID << " ";
			}
			std::cout << "\n--\n";
		}


		std::cout << "----------------------------------\n";
	}
}

int main()
{
	std::string filename = "Input";
	Company myCompany;

	myCompany = ParseInputFile(filename);

	PrintEmployeeDetails(myCompany);

	int id;
	std::cout << "Enter an id: ";
	std::cin >> id;
	if (SearchForEmployee(myCompany, id) == NULL)
	{
		if (CompanyIsEmpty(myCompany))
			std::cout << "The company is empty\n";
		else
			std::cout << "Employee not found\n";
	}
	else
	{
		Employee* current = SearchForEmployee(myCompany, id);
		std::cout << current->EmailAddress;
	}
	
	std::cout << "Do you want to add an employee\n";
	AddEmployee(myCompany);
	//std::cout << "Employee added\n";

	PrintEmployeeDetails(myCompany);

	AddMeeting(myCompany);
	std::cout << "Meeting added.............\n";
	PrintEmployeeDetails(myCompany);

	//std::cout << "enter id to delete employee\n";
	//int f;
	//std::cin >> f;

	//DeleteEmployee(myCompany, f);
	//PrintEmployeeDetails(myCompany);
	std::cout << "Enter a meeting to cancel: \n";
	std::string a;
	std::cin >> a;
	CancelMeeting(myCompany, a);
	PrintEmployeeDetails(myCompany);
	std::cout << "Enter the day ";
	int day;
	std::cin >> day;
	DeleteAllMeetings(myCompany, day);
	PrintEmployeeDetails(myCompany);

	return 0;
}