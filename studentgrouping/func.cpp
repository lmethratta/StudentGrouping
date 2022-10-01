#include "func.hpp"

// Prompts the user for their desired size of group and determines
// if it is a valid size before storing it in the given variable
inline void get_size(int &size) {
  cout << "Please enter your desired size of the groups (Between 2 and 6):"
       << endl;
  bool valid = false;
  // run while loop until a valid size is entered
  while (!valid) {
    cin >> size;
    // if the size is between 2 and 6 (inclusive) end the while loop
    if (size >= 2 && size <= 6) {
      valid = true;
    // else prompt the user for another size
    } else {
      cout << endl << "Invalid number: Must be between 2 and 6" << endl;
    }
  }
  cout << endl;
}

// reads the data from the csv file into a map whose keys are
// student usernames and whose values are the students' survey answers
inline void read_csv(map<string, vector<string>> &teamf) {
  string line;
  string line2;
  string data;

  ifstream infile("TeamFormation.csv");

  if (!infile) {
    cout << "can't open the file" << endl;
    exit(EXIT_FAILURE);
  }

  while (getline(infile, line, '\n')) {
    stringstream ss(line);
    getline(ss, data, ',');
    while (getline(ss, line2, ',')) {
      teamf[data].push_back(line2);
    }
  }
}

// determines if the given student is already a member of a group
inline bool in_group(map<string, vector<string>> groups, string student) {
  bool is_member = false;
  // iterates through every member of every group and only returns true
  // if a member is found whose username is the same as the student's
  for (auto group: groups) {
    for (auto member: group.second) {
      if (member == student) {
        is_member = true;
      }
    }
  }
  return is_member;
}

// determines if either the given student doesn't want to work with someone
// in the given group or if someone in the given group doesn't want to 
// work with the student
inline bool dont_want(string student, vector<string> group,
                      map<string, vector<string>> teamf) {
  bool contains = false;
  // iterates through each member of the group and returns false if either
  // they are on the student's don't want list or if the student is on
  // their don't want list
  for (auto member : group) {
    if (teamf[student][5].find(member) != string::npos || teamf[member][5].find(student) != string::npos) {
      contains = true;
    }
  }
  return !contains;
}

// determines if the given student is advanced at a subject that someone
// in the given group is already advanced at
inline bool skill_balance(string student, vector<string> group,
                          map<string, vector<string>> teamf) {
  bool st_prog = false;
  bool st_db = false;
  bool st_proj = false;
  bool mem_prog = false;
  bool mem_db = false;
  bool mem_proj = false;
  bool contains = false;

  // sets booleans equal to true if their associated student is advanced
  // at their associated subject
  if (teamf[student][1] == "5") {
    st_prog = true;
  }

  if (teamf[student][2] == "5") {
    st_db = true;
  }

  if (teamf[student][3] == "5") {
    st_proj = true;
  }

  for (auto member : group) {
    if (teamf[member][1] == "5") {
      mem_prog = true;
    }

    if (teamf[member][2] == "5") {
      mem_db = true;
    }

    if (teamf[member][3] == "5") {
      mem_proj = true;
    }
  }

  // returns false if there is a match between what the student is advanced
  // at and what the group members are advanced at
  if ((st_prog && mem_prog) || (st_db && mem_db) || (st_proj && mem_proj)) {
    contains = true;
  }

  return !contains;
}

// determines if the given student wants to be in a group with someone in
// the given group or vice versa
inline bool want(string student, vector<string> group,
                 map<string, vector<string>> teamf) {
  bool contains = false;
  // iterates through every member of the group and returns true if 
  // the student appears on a member's want list or vice versa
  for (auto member : group) {
    if (teamf[student][4].find(member) != string::npos || teamf[member][4].find(student) != string::npos) {
      contains = true;
    }
  }
  return contains;
}

// Assigns the students to groups
inline void assign_groups(map<string, vector<string>> teamf,
                          map<string, vector<string>> &groups, int size) {
  // Calculates the number of groups based on the groups sizes
  int num_groups = (29 / size) + 1;

  // Iterates through all the students and assigns them a group if they:
  // don't have anyone in the group listed as don't want
  // are not advanced in a subject someone in the group is already advanced at
  // have someone in the group listed as someone they want
  // are not already in a group
  // and if the group is not full
  for (auto student : teamf) {
    for (int i = 1; i <= num_groups; i++) {
      string groupno = "Group " + to_string(i);
      if (dont_want(student.first, groups[groupno], teamf) &&
          skill_balance(student.first, groups[groupno], teamf) &&
          want(student.first, groups[groupno], teamf) &&
          groups[groupno].size() < size && !in_group(groups, student.first)) {
        groups[groupno].push_back(student.first);
      }
    }
  }

  // same as previous loop, but remove the requirement of having someone
  // they want in the group
  for (auto student : teamf) {
    for (int i = 1; i <= num_groups; i++) {
      string groupno = "Group " + to_string(i);
      if (dont_want(student.first, groups[groupno], teamf) &&
          skill_balance(student.first, groups[groupno], teamf) &&
          groups[groupno].size() < size && !in_group(groups, student.first)) {
        groups[groupno].push_back(student.first);
      }
    }
  }

  // same as the previous loop, but replace balancing the skill with having
  // somebody they want in the group
  for (auto student : teamf) {
    for (int i = 1; i <= num_groups; i++) {
      string groupno = "Group " + to_string(i);
      if (dont_want(student.first, groups[groupno], teamf) &&
          want(student.first, groups[groupno], teamf) &&
          groups[groupno].size() < size && !in_group(groups, student.first)) {
        groups[groupno].push_back(student.first);
      }
    }
  }

  // same as previous loop, but remove the requirement of having someone
  // they want in the group
  for (auto student : teamf) {
    for (int i = 1; i <= num_groups; i++) {
      string groupno = "Group " + to_string(i);
      if (dont_want(student.first, groups[groupno], teamf) &&
          groups[groupno].size() < size && !in_group(groups, student.first)) {
        groups[groupno].push_back(student.first);
      }
    }
  }

  // Place any leftover students in any group that isn't full
  for (auto student : teamf) {
    for (int i = 1; i <= num_groups; i++) {
      string groupno = "Group " + to_string(i);
      if (groups[groupno].size() < size && !in_group(groups, student.first)) {
        groups[groupno].push_back(student.first);
      }
    }
  }
}

// Get the scores of each group for the three subjects, as well as their
// total scores, and add the scores to the ends of the group lists
inline void get_score(map<string, vector<string>> &groups, map<string, vector<string>> teamf) {
  // iterate through every group and add each accumulated score
  // to the group vectors
  for (auto group: groups) {
    // accumulator variables:
    int prog = 0;
    int db = 0;
    int proj = 0;
    int total = 0;
    // iterate through every member of every group to get their scores
    // and add them to the accumulator variables
    for (auto member: group.second) {
      prog += stoi(teamf[member][1]);
      db += stoi(teamf[member][2]);
      proj += stoi(teamf[member][3]);
      total += stoi(teamf[member][1]);
      total += stoi(teamf[member][2]);
      total += stoi(teamf[member][3]);
    }
    groups[group.first].push_back(to_string(prog));
    groups[group.first].push_back(to_string(db));
    groups[group.first].push_back(to_string(proj));
    groups[group.first].push_back(to_string(total));
  }
}

// Outputs the group members and scores to a text file
inline void output_txt(map<string, vector<string>> groups) {
  string line;

  for (auto group: groups) {
    line += group.first + ": ";
    for (auto member: group.second) {
      line += member + " ";
    }

    line += "\n";
  
    // opens file
    ofstream outfile("groups.txt");
    if (!outfile) {
      cout << "can't open the file" << endl;
      exit(EXIT_FAILURE);
    }

    outfile << line << endl;
  }
}

// Allows the user to search for a student by username
// Prints out the information on that student's group
inline void search(map<string, vector<string>> groups) {
  string yn;
  string username;
  bool search = true;
  bool exists = false;
  // Prompts the user for if they want to search for a student
  cout << "Do you want to search for a student? (y/n)" << endl;
  // while loop that runs as long as the user wants to search for a student
  while (search) {
    cin >> yn;
    cout << endl;
    // if the user wants to search for a student...
    if (yn == "y") {
      // Enter the student's username
      cout << "Please enter the username of the student you want to search for:" << endl;
      // while loop that runs as long as the given username belongs to a 
      // student in a group
      while (!exists) {
        cin >> username;
        // Prompt the user for a different username if there is no student
        // that has that username in a group
        if (!in_group(groups, username)) {
          cout << "Student not found, please try again:" << endl;
        }
        // end loop if the given student is in a group
        else {
          exists = true;
        }
      }
      bool in_here = false;
      // iterates through all of the members of all of the groups and
      // sets in_here = true if there is a member whose username matches
      // the one we're searching for
      for (auto group: groups) {
        for (auto member: group.second) {
          if (member == username) {
            in_here = true;
          }
        }
        // if the student was found, print out their group name and details
        if (in_here == true) {
          cout << group.first + ": ";
          for (auto mem: group.second) {
            cout << mem + " ";
          }
          // reset in_here = false for the next potential loop
          in_here = false;
        }
        cout << endl;
      }
      // reset exists = false for the next potential loop
      exists = false;
      // Prompts the user for if they want to search for another student
      cout << "Do you want to search for another student? (y/n)" << endl;
    }
    // end loop if the user does not want to search for a student
    else if (yn == "n") {
      search = false;
    }
    // if the input is not 'y' or 'n', prompt the user to enter a valid
    // input
    else {
      cout << "Invalid input, please enter 'y' or 'n':" << endl;
    }
  }
}

// Function that runs all the necessary function for the program to run
// Called in main.cpp
inline void make_groups() {
  // variables to store the student information, groups, and team size
  map <string, vector<string>> teamf;
  map <string, vector<string>> groups;
  int size;

  // Prompts user to enter size for groups between 2 and 6 (inclusive)
  get_size(size);

  // Reads data from csv file and stores student information in teamf map
  read_csv(teamf);

  // Assigns the students to groups
  assign_groups(teamf, groups, size);

  // Adds the scores in the three subjects and the total score to the groups map
  get_score(groups, teamf);

  // Outputs the groups information to a text file
  output_txt(groups);

  // Search for a student in a group
  search(groups);
}