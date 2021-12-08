The main objective of this project is to create a time management system for the employees within a company.
More precisely, you need to help an employee plan a meeting by checking the availability of the other
employees involved in this meeting and identifying the timeslots where all the meeting attendees are available.
For this purpose, we will define the Company as a doubly linked list of Employees. Each Employee is identified by
a Unique ID, a First Name, a Last Name, and an Email Address. Your program should automatically generate and
attribute a unique ID to each newly created employee. On top of this, each Employee has a calendar which
consists in a singly linked list that stores all the employee’s meetings/appointments. A meeting/appointment is
defined by its title, its starting time, its duration (integer number expressed in seconds), and its list of attendees.
The starting time of a meeting can be represented using a structure composed of 4 integer numbers denoting
the seconds [0 ... 59], the minutes [0 ... 59], the hours [0 ... 23], and the day of the year [1 ... 365].

Data Structures to be used in the project:

struct Employee {
int UniqueID;
char FirstName[30], LastName[30], EmailAddress[50];
Appointment *Calendar;
Employee *next;
Employee *previous;
};

struct Company {
Employee *Head, *Tail;
};

struct Time {
int Day, Hour, Minute, Second;
};

struct Appointment {
char Title[50];
Time StartingTime;
int Duration;
Attendee *ListOfAttendees;
Appointment *next;
};

struct Attendee {
Employee *self;
Attendee *next;
};

For this purpose, you will make use of the data structures defined above where:

• The structure Company is a doubly linked list of all the employees

• The Employee is a data structure characterized by a ‘Unique ID’, a ‘First Name’, a ‘Last Name’, and an
‘Email Address’. It contains two pointers ‘next’ and ‘previous’ that are required for the proper operation
of the doubly linked list Company. It also contains a singly linked list ‘Calendar’ that lists all the
Appointments of this Employee.

• The nodes in this latter singly linked list ‘Calendar’ are of type Appointment. An Appointment is a data
structure characterized by a ‘Title’, a ‘Starting Time’, and a ‘Duration’. It contains the pointer ‘next’
required for the proper operation of the singly linked list. It also contains a singly linked list ‘List of
Attendees’ that lists all the Employees that are expected to attend that particular Appointment.

• Finally, the ‘List of Attendees’ is a singly linked list of nodes of type Attendee. An Attendee is a data
structure composed of the pointer ‘next’ necessary for the proper operation of the singly linked list and
the pointer ‘self’ that points to a node of the doubly linked list Company.

Your task is to write a program and test it on an input text file containing already some Employees with
some scheduled appointments. You can choose the format of this input text file according to your needs.

Among others, this program should contain the following functions:

1. A function that parses the input text file and returns the doubly linked list of all the employees in your
input text file. This function should pay particular attention to correctly build the ‘Calendar’ and the ‘List
of Attendees’ for each scheduled appointment.

2. A function that searches for a particular Employee given by its characteristics in the doubly linked list
Company. This function should return NULL if the Employee could not be found.

3. A function that adds a new Employee given by its characteristics to the doubly linked list Company.

4. A function that deletes an Employee given by its characteristics from the doubly linked list Company.
This function should also take care of deleting this individual from the appointments that he was
supposed to attend.

5. A function that adds a new meeting to the calendar of all the employees that will attend a particular
appointment.

6. A function that cancels an already scheduled meeting and deletes it from the calendar of all the
employees that were planning to attend it.

7. A function that deletes all the meetings that are scheduled before a given day.

8. A function that takes as parameter a list of employees and a minimum duration and will propose some
timeslots that can be used to organize a meeting depending on the common availabilities of all the
attendees that are longer than the minimum duration of the meeting.

9. A function that takes as argument the doubly linked list Company and write an output file containing the
updated list of employees in the company along with the updated list of meetings that they are
attending. The output file has the same format as the input file.

10. A main function that interacts with the user and allows him to make use of all the previous functions.
