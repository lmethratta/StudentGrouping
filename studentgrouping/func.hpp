#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <numeric>

using namespace std;

// Prompts the user for their desired size of group and determines
// if it is a valid size before storing it in the given variable
void get_size(int &size);

// reads the data from the csv file into a map whose keys are
// student usernames and whose values are the students' survey answers
void read_csv(map<string, vector<string>> &teamf);

// determines if the given student is already a member of a group
bool in_group(map<string, vector<string>> groups, string student);

// determines if either the given student doesn't want to work with someone
// in the given group or if someone in the given group doesn't want to 
// work with the student
bool dont_want(string student, vector<string> group, map<string, vector<string>> teamf);

// determines if the given student is advanced at a subject that someone
// in the given group is already advanced at
bool skill_balance(string student, vector<string> group, map<string, vector<string>> teamf);

// determines if the given student wants to be in a group with someone in
// the given group or vice versa
bool want(string student, vector<string> group, map<string, vector<string>> teamf);

// Assigns the students to groups
void assign_groups(map<string, vector<string>> teamf, map<string, vector<string>> &groups, int size);

// Get the scores of each group for the three subjects, as well as their
// total scores, and add the scores to the ends of the group lists
void get_score(map <string, vector<string>> &groups, map<string, vector<string>> teamf);

// Outputs the group members and scores to a text file
void output_txt(map<string, vector<string>> groups);

// Allows the user to search for a student by username
// Prints out the information on that student's group
void search(map<string, vector<string>> groups);

// Function that runs all the necessary function for the program to run
// Called in main.cpp
void make_groups();
