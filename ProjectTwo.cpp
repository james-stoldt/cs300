/*
* Developer:			James Stoldt
* Company:				SNHU
* Date:					10/18/2024
* 
* Program Name:			ABCU Comp Sci Dept - Course Parser
* Program Description:	Parse a CSV containing college course records into a
*						unordered data structure. Sort vector by course ID and
*						print.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>

#include <algorithm>
#include <limits>

using namespace std;

//Data structure representing course information
struct Course
{
	//Fields to hold course data
	string courseID;
	string courseName;
	vector<string> prerequisites;

	//Print course information
	void print() const
	{
		cout << courseID << " - " << courseName << endl;
	}

	//Print course information with prerequisites
	void printWithPrereqs() const
	{
		this->print();
		cout << "\t" << "Prerequisites: ";

		int max = prerequisites.size() - 1;
		for (int i = 0; i <= max; i++)
		{
			cout << prerequisites.at(i) << (i != max ? ", " : "");
		}

		cout << endl;
	}

	//Overload < operator to be used with std::set insert comparison operation
	bool operator<(const Course& that) const
	{
		return this->courseID < that.courseID;
	}
};

//Parse CSV file into a hash map
//String key is course ID
//Value is course object
unordered_map<string, Course> parseCoursesCSV(const string& filePath)
{
	unordered_map<string, Course> courses;
	ifstream file(filePath);
	string line;

	if (!file.is_open())
	{
		cerr << "Could not open file: " << filePath << endl;
		return courses;
	}

	while (getline(file, line))
	{
		Course c;

		stringstream ss(line);
		string token;

		getline(ss, token, ',');
		c.courseID = token;

		getline(ss, token, ',');
		c.courseName = token;

		while (getline(ss, token, ','))
		{
			c.prerequisites.push_back(token);
		}

		courses[c.courseID] = c;
	}
	
	file.close();

	return courses;
}

// Course ID comparision function to pass to std::sort function
bool compareCourseByID(Course& a, Course& b)
{
	return a.courseID < b.courseID;
}

// Function to print all courses
void printAllCoursesABC(const unordered_map<string, Course>& courses)
{
	// Copy courses from map to vector
	vector<Course> courseList;

	for (const auto& entry : courses)
	{
		courseList.push_back(entry.second);	//pull the course object from the map entry
	}

	// Sort the courses by course ID
	sort(courseList.begin(), courseList.end(), compareCourseByID);

	// Print the sorted courses
	for (const Course& course : courseList)
	{
		course.print();
		cout << endl;
	}
}

//Display entry greeting with program name
void displayGreeting()
{
	cout << "--------------------------------" << endl;
	cout << "ABCU Computer Science Department" << endl;
	cout << "~Course Parser v1~" << endl;
	cout << endl;
	cout << "Copyright (c) 2024" << endl;
	cout << "--------------------------------" << endl;
	cout << endl;
}

//Input validation loop used by menu driver
int numericInputLoop()
{
	int input;

	while (true)
	{
		cout << "Menu Option: ";
		cin >> input;

		//Handle string input
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Input must be numeric." << endl;
		}
		else
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return input;
		}
	}
}

//Check if the hash map is loaded and return the result to the caller
bool checkMapLoaded(unordered_map<string, Course>* pMap)
{
	if (pMap->size() == 0)
	{
		cout << "No records loaded." << endl << endl;
		return false;
	}
	return true;
}

//Display menu function used by menu driver
void displayMenu()
{
	cout << "1. Load File" << endl;
	cout << "2. Print Course List Alphabetical" << endl;
	cout << "3. Display Course w/ Prerequisites" << endl;
	cout << "9. Exit" << endl;
}

//Main menu driver
void menuDriver()
{
	string filePath;
	unordered_map<string, Course> courses;

	while (true)
	{
		displayMenu();

		int input = numericInputLoop();
		string inputKey;

		switch (input)
		{
			case 1:
				cout << "Open file path: ";
				getline(cin, filePath);
				courses = parseCoursesCSV(filePath);
				cout << endl;
				break;

			case 2:
				cout << endl;
				if (!checkMapLoaded(&courses)) break;

				printAllCoursesABC(courses);
				break;

			case 3:
				cout << endl;
				if (!checkMapLoaded(&courses)) break;

				cout << "Enter course key: ";
				cin >> inputKey;
				transform(inputKey.begin(), inputKey.end(), inputKey.begin(), ::toupper);

				try
				{
					courses.at(inputKey).printWithPrereqs();
				}
				catch (const std::out_of_range&)
				{
					cout << "Key '" << inputKey << "' not found." << endl;
				}

				cout << endl;
				break;

			case 9: return;
			
			default:
				cout << "Input out of range." << endl;
				break;
		}
	}
}

int main(int argc, char* argv[])
{
	displayGreeting();
	menuDriver();

	return EXIT_SUCCESS;
}