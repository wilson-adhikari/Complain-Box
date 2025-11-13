#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include <cctype>
#include <map>
#include <sstream>
#include <conio.h>
#include <ctime>
using namespace std;

#define RED            "\033[31m"
#define GREEN          "\033[32m"
#define YELLOW         "\033[33m"
#define BLUE           "\033[34m"
#define MAGENTA        "\033[35m"
#define BRIGHT_MAGENTA "\033[95m"
#define CYAN           "\033[36m"
#define WHITE          "\033[37m"
#define BLACK          "\033[30m"
#define BG_MAGENTA     "\033[45m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN    "\033[106m"
#define BG_CYAN        "\033[46m"
#define BG_GREEN       "\033[42m"
#define BG_RED         "\033[41m"
#define BOLD           "\033[1m"
#define RESET          "\033[0m"


class AdminSystem {
    struct Complaint {
        string user_code, search_code;
        string i_identify_myself;
        string complain_against_for;
    
        string name, self_id, self_intake, self_section, self_dep;
        string faculty_dep;
        string staff_name, staff_designation, staff_office;
        string stranger_name, stranger_role;
    
        string against, against_id, against_intake, against_section, against_dep;
        string against_faculty_name, against_faculty_dep;
        string against_staff_name, against_staff_designation, against_staff_office;
        string against_stranger_name, against_stranger_role;
    
        string complain_type, date, describe, status, priority;
        int    repeatCount = 0;
        string colour;
        bool resolved = false;
    };
    vector<Complaint> complaints;
    vector<Complaint> resolvedComplaints;
    string currentUser;
    
    // Admin credentials
    map<string, string> adminCredentials = {
        {"admin", "admin123"},
        {"supervisor", "sup123"},
        {"moderator", "mod123"}
    };

    // Cache for frequently accessed data
    map<string, int> targetCountCache;
    bool cacheDirty = true;

    string encrypt(const string& s) { 
        string e = s; 
        for (char& c : e) c += 3; 
        return e; 
    }

    string decrypt(const string &data, int value = 3) {
        string result = data;
        for (char &c : result) c = c - value;
        return result;
    }

    string remove(string str) {
        while (!str.empty() && str[0] == ' ') str.erase(0, 1);
        return str;
    }

    string tolower_str(string s) {
        for (char &c : s) c = tolower(c);
        return s;
    }

    string value_after_colon(const string &line) {
        size_t p = line.find(':');
        if (p == string::npos) return "";
        return remove(line.substr(p + 1));
    }

    void clear() { system("cls"); }

    void pause() { 
        cout << YELLOW << "\nPress any key to continue..." << RESET; 
        _getch(); 
    }

    void header(const string& t) {
        clear();
        cout << BG_BRIGHT_MAGENTA << BOLD << WHITE << string(80, ' ') << RESET << "\n";
        cout << BG_BRIGHT_MAGENTA << BOLD << WHITE << setw(80) << (" " + t + " ") << RESET << "\n";
        cout << BG_BRIGHT_MAGENTA << BOLD << WHITE << string(80, ' ') << RESET << "\n\n";
    }

    // Function to normalize date format (ensure DD/MM/YYYY with leading zeros)
    string normalizeDate(const string& date) {
        string normalized = date;
        size_t firstSlash = normalized.find('/');
        size_t secondSlash = normalized.find('/', firstSlash + 1);
        
        if (firstSlash != string::npos && secondSlash != string::npos) {
            string day = normalized.substr(0, firstSlash);
            string month = normalized.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            string year = normalized.substr(secondSlash + 1);
            
            // Add leading zero to day if needed
            if (day.length() == 1) day = "0" + day;
            // Add leading zero to month if needed
            if (month.length() == 1) month = "0" + month;
            
            normalized = day + "/" + month + "/" + year;
        }
        return normalized;
    }

    // Function to convert date to sortable format (YYYYMMDD)
    string dateToSortable(const string& date) {
        string normalized = normalizeDate(date);
        size_t firstSlash = normalized.find('/');
        size_t secondSlash = normalized.find('/', firstSlash + 1);
        
        if (firstSlash != string::npos && secondSlash != string::npos) {
            string day = normalized.substr(0, firstSlash);
            string month = normalized.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            string year = normalized.substr(secondSlash + 1);
            
            return year + month + day; // YYYYMMDD format for proper sorting
        }
        return date; // Return original if format is invalid
    }

    // Optimized complaint loading with caching
    void loadComplaints(const string& filename, vector<Complaint>& target) {
        target.clear();
        ifstream in(filename);
        if (!in) return;

        Complaint c;
        string line;
        while (getline(in, line)) {
            if (line.empty()) {
                if (!c.name.empty()) {
                    c.resolved = (c.status == "Resolved");
                    target.push_back(c);
                }
                c = Complaint();
                continue;
            }
            
            string dec = decrypt(line);
            
            if (dec.rfind("USER_CODE: ", 0) == 0) c.user_code = value_after_colon(dec);
            else if (dec.rfind("SEARCH_CODE: ", 0) == 0) c.search_code = value_after_colon(dec);
            else if (dec.rfind("I am a : ", 0) == 0) c.i_identify_myself = value_after_colon(dec);
            else if (dec.rfind("Complain Against for : ", 0) == 0) c.complain_against_for = value_after_colon(dec);
            else if (dec.rfind("Name : ", 0) == 0) c.name = value_after_colon(dec);
            else if (dec.rfind("ID : ", 0) == 0) c.self_id = value_after_colon(dec);
            else if (dec.rfind("Intake : ", 0) == 0) c.self_intake = value_after_colon(dec);
            else if (dec.rfind("Section : ", 0) == 0) c.self_section = value_after_colon(dec);
            else if (dec.rfind("Department : ", 0) == 0) c.self_dep = value_after_colon(dec);
            else if (dec.rfind("Staff Designation/Job Title : ", 0) == 0) c.staff_designation = value_after_colon(dec);
            else if (dec.rfind("Staff Office/Department : ", 0) == 0) c.staff_office = value_after_colon(dec);
            else if (dec.rfind("Stranger Role : ", 0) == 0) c.stranger_role = value_after_colon(dec);
            else if (dec.rfind("Against : ", 0) == 0) c.against = value_after_colon(dec);
            else if (dec.rfind("Against ID : ", 0) == 0) c.against_id = value_after_colon(dec);
            else if (dec.rfind("Against Intake : ", 0) == 0) c.against_intake = value_after_colon(dec);
            else if (dec.rfind("Against Section : ", 0) == 0) c.against_section = value_after_colon(dec);
            else if (dec.rfind("Against Department : ", 0) == 0) c.against_dep = value_after_colon(dec);
            else if (dec.rfind("Against Faculty Name: ", 0) == 0) c.against_faculty_name = value_after_colon(dec);
            else if (dec.rfind("Against Faculty Department: ", 0) == 0) c.against_faculty_dep = value_after_colon(dec);
            else if (dec.rfind("Against Staff Name : ", 0) == 0) c.against_staff_name = value_after_colon(dec);
            else if (dec.rfind("Against Staff Designation : ", 0) == 0) c.against_staff_designation = value_after_colon(dec);
            else if (dec.rfind("Against Staff Office : ", 0) == 0) c.against_staff_office = value_after_colon(dec);
            else if (dec.rfind("Against Stranger Name : ", 0) == 0) c.against_stranger_name = value_after_colon(dec);
            else if (dec.rfind("Against Stranger Role : ", 0) == 0) c.against_stranger_role = value_after_colon(dec);
            else if (dec.rfind("Complain : ", 0) == 0) c.complain_type = value_after_colon(dec);
            else if (dec.rfind("Priority: ", 0) == 0) c.priority = value_after_colon(dec);
            else if (dec.rfind("Date: ", 0) == 0) c.date = value_after_colon(dec);
            else if (dec.rfind("Status: ", 0) == 0) c.status = value_after_colon(dec);
            else if (dec.rfind("Describe: ", 0) == 0) c.describe = value_after_colon(dec);
        }
        if (!c.name.empty()) {
            c.resolved = (c.status == "Resolved");
            target.push_back(c);
        }
        in.close();
        cacheDirty = true; // Mark cache as dirty when data changes
    }

    // Optimized target counting with caching
    void buildTargetCountCache() {
        if (!cacheDirty) return;
        
        targetCountCache.clear();
        for (const auto& complaint : complaints) {
            string target = (complaint.complain_against_for == "faculty") ? 
                           complaint.against_faculty_name : complaint.against;
            if (!target.empty()) {
                targetCountCache[target]++;
            }
        }
        cacheDirty = false;
    }

    int count_target(const string& target) {
        buildTargetCountCache();
        auto it = targetCountCache.find(target);
        return (it != targetCountCache.end()) ? it->second : 0;
    }

    // Enhanced color determination
    string colour_for(const string& complain_type, int repeatCount) {
        if (repeatCount > 5) return BG_RED;
        if (repeatCount > 3) return RED;
        
        static const map<string, string> typeColors = {
            {"Academic", BLUE},
            {"Administrative", YELLOW},
            {"Behavioral", RED},
            {"Discrimination", MAGENTA},
            {"Harassment", BRIGHT_MAGENTA},
            {"Other", CYAN}
        };
        
        auto it = typeColors.find(complain_type);
        return (it != typeColors.end()) ? it->second : WHITE;
    }

    // Enhanced print function with better formatting - FIXED VERSION
    void print_one(const Complaint& c, int idx) {
        cout << BG_CYAN << BOLD << "\n Complaint #" << idx << " ";
        cout << (c.status == "Resolved" ? GREEN "[RESOLVED]" : 
                c.status == "In Progress" ? YELLOW "[IN PROGRESS]" : 
                c.status == "Pending" ? MAGENTA "[PENDING]" : 
                RED "[UNKNOWN]") << RESET << "\n";
        
        // COMPLAINER INFO - Show all relevant information
        cout << RESET << "FROM: " << BOLD << c.i_identify_myself << RESET << " | " << MAGENTA << c.name << RESET << "\n";
        
        if (c.i_identify_myself == "student") {
            if (!c.self_id.empty()) cout << "  ID      : " << YELLOW << c.self_id << RESET << "\n";
            if (!c.self_intake.empty()) cout << "  Intake  : " << YELLOW << c.self_intake << RESET << "\n";
            if (!c.self_section.empty()) cout << "  Section : " << YELLOW << c.self_section << RESET << "\n";
            if (!c.self_dep.empty()) cout << "  Dept    : " << MAGENTA << c.self_dep << RESET << "\n";
        }
        else if (c.i_identify_myself == "faculty") {
            if (!c.self_dep.empty()) cout << "  Dept    : " << MAGENTA << c.self_dep << RESET << "\n";
            if (!c.staff_designation.empty()) cout << "  Designation : " << CYAN << c.staff_designation << RESET << "\n";
        }
        else if (c.i_identify_myself == "staff") {
            if (!c.staff_designation.empty()) cout << "  Designation : " << CYAN << c.staff_designation << RESET << "\n";
            if (!c.staff_office.empty()) cout << "  Office      : " << CYAN << c.staff_office << RESET << "\n";
        }
        else if (c.i_identify_myself == "stranger") {
            if (!c.stranger_role.empty()) cout << "  Role : " << CYAN << c.stranger_role << RESET << "\n";
        }

        // AGAINST INFO - Show all relevant information based on type
        cout << "AGAINST: " << BOLD << c.complain_against_for << RESET << "\n";
        
        if (c.complain_against_for == "faculty") {
            if (!c.against_faculty_name.empty()) cout << "  Faculty : " << c.colour << c.against_faculty_name << RESET << "\n";
            if (!c.against_faculty_dep.empty()) cout << "  Dept    : " << MAGENTA << c.against_faculty_dep << RESET << "\n";
        }
        else if (c.complain_against_for == "student") {
            if (!c.against.empty()) cout << "  Name    : " << c.colour << c.against << RESET << "\n";
            if (!c.against_id.empty()) cout << "  ID      : " << YELLOW << c.against_id << RESET << "\n";
            if (!c.against_intake.empty()) cout << "  Intake  : " << YELLOW << c.against_intake << RESET << "\n";
            if (!c.against_section.empty()) cout << "  Section : " << YELLOW << c.against_section << RESET << "\n";
            if (!c.against_dep.empty()) cout << "  Dept    : " << MAGENTA << c.against_dep << RESET << "\n";
        }
        else if (c.complain_against_for == "staff") {
            if (!c.against_staff_name.empty()) cout << "  Staff Name : " << c.colour << c.against_staff_name << RESET << "\n";
            if (!c.against_staff_designation.empty()) cout << "  Designation : " << CYAN << c.against_staff_designation << RESET << "\n";
            if (!c.against_staff_office.empty()) cout << "  Office      : " << CYAN << c.against_staff_office << RESET << "\n";
        }
        else if (c.complain_against_for == "stranger") {
            if (!c.against_stranger_name.empty()) cout << "  Name : " << c.colour << c.against_stranger_name << RESET << "\n";
            if (!c.against_stranger_role.empty()) cout << "  Role : " << CYAN << c.against_stranger_role << RESET << "\n";
        }
        else {
            if (!c.against.empty()) cout << "  Name : " << c.colour << c.against << RESET << "\n";
        }

        cout << "Issue    : " << RED << c.complain_type << RESET << " | Date: " << c.date << "\n";
        cout << "Priority : " << (c.priority == "Emergency" ? BG_RED : 
                                 c.priority == "High" ? RED : 
                                 c.priority == "Medium" ? YELLOW : GREEN) 
             << c.priority << RESET << " | Repeat: " << c.repeatCount << "\n";
        cout << "Status   : " << (c.status == "Resolved" ? GREEN : 
                                 c.status == "In Progress" ? YELLOW : 
                                 c.status == "Pending" ? MAGENTA : RED)
             << c.status << RESET << "\n";
        cout << "Desc     : " << c.describe << "\n";
        cout << "Search Code: " << CYAN << c.search_code << RESET << "\n\n";
    }

    void date_wise() {
    system("cls");
    cout << BG_GREEN << BOLD << setw(70) << " DATE-WISE SEARCH " << RESET << "\n\n";
    cout << CYAN << "Enter date (DD/MM/YYYY) or range (DD/MM/YYYY to DD/MM/YYYY):\n> " << RESET;
    string input;
    getline(cin >> ws, input);

    string start_str = input, end_str = input;
    size_t pos = input.find(" to ");
    if (pos != string::npos) {
        start_str = input.substr(0, pos);
        end_str = input.substr(pos + 4);
    }

    auto trim = [](string& s) {
        s.erase(0, s.find_first_not_of(" \t"));
        s.erase(s.find_last_not_of(" \t") + 1);
    };
    trim(start_str); trim(end_str);

    // FIXED: Improved date validation to handle both formats
    auto valid = [](const string& d) {
        // Check if it has slashes at correct positions
        size_t firstSlash = d.find('/');
        size_t secondSlash = d.find('/', firstSlash + 1);
        
        if (firstSlash == string::npos || secondSlash == string::npos) 
            return false;
        
        // Check if we have exactly 2 slashes
        if (d.find('/', secondSlash + 1) != string::npos) 
            return false;
        
        string day = d.substr(0, firstSlash);
        string month = d.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        string year = d.substr(secondSlash + 1);
        
        // Check if all parts are digits
        for (char c : day) if (!isdigit(c)) return false;
        for (char c : month) if (!isdigit(c)) return false;
        for (char c : year) if (!isdigit(c)) return false;
        
        // Check valid ranges
        int d_day = stoi(day);
        int d_month = stoi(month);
        int d_year = stoi(year);
        
        return (d_day >= 1 && d_day <= 31 && 
                d_month >= 1 && d_month <= 12 && 
                d_year >= 2000 && d_year <= 2100);
    };

    if (!valid(start_str) || !valid(end_str)) {
        cout << RED << "Invalid date format! Use D/M/YYYY or DD/MM/YYYY\n" << RESET;
        system("pause");
        return;
    }

    // FIXED: Improved date conversion to handle both formats
    auto to_num = [](const string& d) {
        size_t firstSlash = d.find('/');
        size_t secondSlash = d.find('/', firstSlash + 1);
        
        string day = d.substr(0, firstSlash);
        string month = d.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        string year = d.substr(secondSlash + 1);
        
        // Add leading zeros if needed
        if (day.length() == 1) day = "0" + day;
        if (month.length() == 1) month = "0" + month;
        
        return stoi(year) * 10000 + stoi(month) * 100 + stoi(day);
    };

    int start = to_num(start_str);
    int end = to_num(end_str);
    if (start > end) swap(start, end);

    vector<int> matches;
    
    // Search in active complaints
    for (int i = 0; i < complaints.size(); i++) {
        string d = complaints[i].date;
        trim(d);
        
        // Convert complaint date to numeric format for comparison
        size_t firstSlash = d.find('/');
        size_t secondSlash = d.find('/', firstSlash + 1);
        
        if (firstSlash != string::npos && secondSlash != string::npos) {
            string day = d.substr(0, firstSlash);
            string month = d.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            string year = d.substr(secondSlash + 1);
            
            // Add leading zeros if needed
            if (day.length() == 1) day = "0" + day;
            if (month.length() == 1) month = "0" + month;
            
            int num = stoi(year) * 10000 + stoi(month) * 100 + stoi(day);
            if (num >= start && num <= end)
                matches.push_back(i);
        }
    }

    // Also search in resolved complaints
    for (int i = 0; i < resolvedComplaints.size(); i++) {
        string d = resolvedComplaints[i].date;
        trim(d);
        
        size_t firstSlash = d.find('/');
        size_t secondSlash = d.find('/', firstSlash + 1);
        
        if (firstSlash != string::npos && secondSlash != string::npos) {
            string day = d.substr(0, firstSlash);
            string month = d.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            string year = d.substr(secondSlash + 1);
            
            // Add leading zeros if needed
            if (day.length() == 1) day = "0" + day;
            if (month.length() == 1) month = "0" + month;
            
            int num = stoi(year) * 10000 + stoi(month) * 100 + stoi(day);
            if (num >= start && num <= end)
                matches.push_back(complaints.size() + i); // Offset for resolved complaints
        }
    }

    if (matches.empty()) {
        cout << YELLOW << "No complaints found from " << start_str;
        if (start_str != end_str) cout << " to " << end_str;
        cout << ".\n" << RESET;
        system("pause");
        return;
    }

    cout << BG_CYAN << BOLD << " FOUND " << matches.size() << " COMPLAINT(S) " << RESET << "\n\n";
    for (int i = 0; i < matches.size(); i++) {
        int idx = matches[i];
        Complaint* currentComplaint;
        
        if (idx < complaints.size()) {
            currentComplaint = &complaints[idx];
        } else {
            currentComplaint = &resolvedComplaints[idx - complaints.size()];
        }
        
        string target = (currentComplaint->complain_against_for == "faculty") ? 
                       currentComplaint->against_faculty_name : currentComplaint->against;
        currentComplaint->repeatCount = count_target(target);
        currentComplaint->colour = colour_for(currentComplaint->complain_type, currentComplaint->repeatCount);
        print_one(*currentComplaint, i + 1);
    }
    system("pause");
}

    bool adminLogin() {
        header("ADMIN LOGIN");
        string username, password;
        
        cout << BOLD << "Username: " << RESET;
        getline(cin, username);
        cout << BOLD << "Password: " << RESET;
        
        // Password masking
        char ch;
        password = "";
        while ((ch = _getch()) != 13) {
            if (ch == 8 && !password.empty()) {
                cout << "\b \b";
                password.pop_back();
            } else if (ch != 8) {
                cout << '*';
                password += ch;
            }
        }
        cout << "\n";
        
        if (adminCredentials.find(username) != adminCredentials.end() && 
            adminCredentials[username] == password) {
            currentUser = username;
            cout << GREEN << "\nLogin successful! Welcome " << username << "!\n" << RESET;
            Sleep(1500);
            return true;
        } else {
            cout << RED << "\nInvalid username or password!\n" << RESET;
            Sleep(2000);
            return false;
        }
    }

    // SIMPLE MENU: Number-based selection
    int showMenu(const vector<string>& options, const string& title) {
        header(title);
        cout << BOLD << "Please select from the following options:\n\n" << RESET;
        
        for (int i = 0; i < options.size(); i++) {
            cout << " " << i + 1 << ". " << options[i] << "\n";
        }
        
        cout << "\n" << YELLOW << "Enter your choice (1-" << options.size() << "): " << RESET;
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice >= 1 && choice <= options.size()) {
            return choice - 1;
        } else {
            cout << RED << "Invalid choice! Please try again.\n" << RESET;
            pause();
            return -1;
        }
    }

    // FIXED displayComplaint function to show ALL information properly
    void displayComplaint(const Complaint& c, int index = 0) {
        if (index > 0) {
            cout << BG_BRIGHT_CYAN << BOLD;
            cout << "\n" << setw(48) << "Complaint #" << index << setw(80) << "\n";
            cout << setw(64) << "--------------------------" << setw(80) << "\n";
        }

        string status_color = (c.status == "Resolved") ? GREEN : 
                             (c.status == "In Progress") ? YELLOW : 
                             (c.status == "Pending") ? MAGENTA : RED;
        
        string priority_color = (c.priority == "Emergency") ? BG_RED :
                               (c.priority == "High") ? RED :
                               (c.priority == "Medium") ? YELLOW : GREEN;

        cout << BG_BRIGHT_CYAN << BOLD;
        cout << setw(50) << "Search Code : " << c.search_code << setw(80) << "\n";
        cout << setw(50) << "User Code : " << c.user_code << setw(80) << "\n";
        cout << setw(50) << "Date : " << c.date << setw(80) << "\n";
        cout << setw(50) << "Status : " << status_color << c.status << RESET << BG_BRIGHT_CYAN << setw(80) << "\n";
        cout << setw(50) << "Priority : " << priority_color << c.priority << RESET << BG_BRIGHT_CYAN << setw(80) << "\n";
        
        // COMPLAINER INFORMATION
        cout << setw(50) << "From : " << c.name << setw(80) << "\n";
        cout << setw(50) << "Identity : " << c.i_identify_myself << setw(80) << "\n";
        
        if (!c.self_id.empty()) cout << setw(50) << "ID : " << c.self_id << setw(80) << "\n";
        if (!c.self_intake.empty()) cout << setw(50) << "Intake : " << c.self_intake << setw(80) << "\n";
        if (!c.self_section.empty()) cout << setw(50) << "Section : " << c.self_section << setw(80) << "\n";
        if (!c.self_dep.empty()) cout << setw(50) << "Department : " << c.self_dep << setw(80) << "\n";
        if (!c.staff_designation.empty()) cout << setw(50) << "Staff Designation : " << c.staff_designation << setw(80) << "\n";
        if (!c.staff_office.empty()) cout << setw(50) << "Staff Office : " << c.staff_office << setw(80) << "\n";
        if (!c.stranger_role.empty()) cout << setw(50) << "Stranger Role : " << c.stranger_role << setw(80) << "\n";
        
        // AGAINST INFORMATION
        cout << setw(50) << "Against : " << c.complain_against_for << setw(80) << "\n";
        
        if (!c.against.empty()) cout << setw(50) << "Against Name : " << c.against << setw(80) << "\n";
        if (!c.against_id.empty()) cout << setw(50) << "Against ID : " << c.against_id << setw(80) << "\n";
        if (!c.against_intake.empty()) cout << setw(50) << "Against Intake : " << c.against_intake << setw(80) << "\n";
        if (!c.against_section.empty()) cout << setw(50) << "Against Section : " << c.against_section << setw(80) << "\n";
        if (!c.against_dep.empty()) cout << setw(50) << "Against Department : " << c.against_dep << setw(80) << "\n";
        if (!c.against_faculty_name.empty()) cout << setw(50) << "Against Faculty Name : " << c.against_faculty_name << setw(80) << "\n";
        if (!c.against_faculty_dep.empty()) cout << setw(50) << "Against Faculty Dept : " << c.against_faculty_dep << setw(80) << "\n";
        if (!c.against_staff_name.empty()) cout << setw(50) << "Against Staff Name : " << c.against_staff_name << setw(80) << "\n";
        if (!c.against_staff_designation.empty()) cout << setw(50) << "Against Staff Designation : " << c.against_staff_designation << setw(80) << "\n";
        if (!c.against_staff_office.empty()) cout << setw(50) << "Against Staff Office : " << c.against_staff_office << setw(80) << "\n";
        if (!c.against_stranger_name.empty()) cout << setw(50) << "Against Stranger Name : " << c.against_stranger_name << setw(80) << "\n";
        if (!c.against_stranger_role.empty()) cout << setw(50) << "Against Stranger Role : " << c.against_stranger_role << setw(80) << "\n";
        
        cout << setw(50) << "Complain Type : " << c.complain_type << setw(80) << "\n";
        cout << setw(50) << "Description : " << c.describe << setw(80) << "\n";
        
        if (index > 0) {
            cout << setw(64) << "--------------------------\n" << RESET << "\n";
        } else {
            cout << RESET << "\n";
        }
    }

    // Function to move complaint from active to resolved file
    void moveComplaintToResolved(const Complaint& complaint) {
        // First, remove the complaint from active file
        removeComplaintFromActive(complaint.search_code);
        
        // Then, add to resolved file
        ofstream resolved("resolved.txt", ios::app);
        auto e = [this](const string& s) { return encrypt(s); };
        
        resolved << e("USER_CODE: " + complaint.user_code) << "\n";
        resolved << e("SEARCH_CODE: " + complaint.search_code) << "\n";
        resolved << e("I am a : " + complaint.i_identify_myself) << "\n";
        resolved << e("Complain Against for : " + complaint.complain_against_for) << "\n";
        resolved << e("Name : " + complaint.name) << "\n";
        
        if (!complaint.self_id.empty()) resolved << e("ID : " + complaint.self_id) << "\n";
        if (!complaint.self_intake.empty()) resolved << e("Intake : " + complaint.self_intake) << "\n";
        if (!complaint.self_section.empty()) resolved << e("Section : " + complaint.self_section) << "\n";
        if (!complaint.self_dep.empty()) resolved << e("Department : " + complaint.self_dep) << "\n";
        if (!complaint.staff_designation.empty()) resolved << e("Staff Designation/Job Title : " + complaint.staff_designation) << "\n";
        if (!complaint.staff_office.empty()) resolved << e("Staff Office/Department : " + complaint.staff_office) << "\n";
        if (!complaint.stranger_role.empty()) resolved << e("Stranger Role : " + complaint.stranger_role) << "\n";
        
        if (!complaint.against.empty()) resolved << e("Against : " + complaint.against) << "\n";
        if (!complaint.against_id.empty()) resolved << e("Against ID : " + complaint.against_id) << "\n";
        if (!complaint.against_intake.empty()) resolved << e("Against Intake : " + complaint.against_intake) << "\n";
        if (!complaint.against_section.empty()) resolved << e("Against Section : " + complaint.against_section) << "\n";
        if (!complaint.against_dep.empty()) resolved << e("Against Department : " + complaint.against_dep) << "\n";
        if (!complaint.against_faculty_name.empty()) resolved << e("Against Faculty Name: " + complaint.against_faculty_name) << "\n";
        if (!complaint.against_faculty_dep.empty()) resolved << e("Against Faculty Department: " + complaint.against_faculty_dep) << "\n";
        if (!complaint.against_staff_name.empty()) resolved << e("Against Staff Name : " + complaint.against_staff_name) << "\n";
        if (!complaint.against_staff_designation.empty()) resolved << e("Against Staff Designation : " + complaint.against_staff_designation) << "\n";
        if (!complaint.against_staff_office.empty()) resolved << e("Against Staff Office : " + complaint.against_staff_office) << "\n";
        if (!complaint.against_stranger_name.empty()) resolved << e("Against Stranger Name : " + complaint.against_stranger_name) << "\n";
        if (!complaint.against_stranger_role.empty()) resolved << e("Against Stranger Role : " + complaint.against_stranger_role) << "\n";
        
        resolved << e("Complain : " + complaint.complain_type) << "\n";
        resolved << e("Priority: " + complaint.priority) << "\n";
        resolved << e("Date: " + complaint.date) << "\n";
        resolved << e("Status: Resolved") << "\n";
        resolved << e("Describe: " + complaint.describe) << "\n\n";
        
        resolved.close();
        
        cout << CYAN << "Complaint archived to resolved file.\n" << RESET;
        cacheDirty = true; // Mark cache as dirty
    }

    // Function to remove complaint from active file
    void removeComplaintFromActive(const string& searchCode) {
        ifstream in("complain.txt");
        ofstream out("temp.txt");
        string line;
        bool inTargetComplaint = false;
        bool removed = false;
        
        while (getline(in, line)) {
            string dec = decrypt(line);
            
            // Check if we're entering the target complaint
            if (dec.rfind("SEARCH_CODE: " + searchCode, 0) == 0) {
                inTargetComplaint = true;
                removed = true;
                // Skip this complaint by not writing its lines
                continue;
            }
            
            // Check if we're leaving the complaint (empty line)
            if (inTargetComplaint && line.empty()) {
                inTargetComplaint = false;
                continue; // Skip the empty line too
            }
            
            // Write non-target complaint lines
            if (!inTargetComplaint) {
                out << line << "\n";
            }
        }
        
        in.close();
        out.close();
        
        if (removed) {
            remove("complain.txt");
            rename("temp.txt", "complain.txt");
        } else {
            remove("temp.txt");
            cout << RED << "Warning: Could not find complaint in active file to remove!\n" << RESET;
        }
    }

    // Function to update complaint status without moving files
    void updateComplaintStatus(const Complaint& complaint, const string& newStatus) {
        ifstream in("complain.txt");
        ofstream out("temp.txt");
        string line;
        bool inTargetComplaint = false;
        bool updated = false;
        
        while (getline(in, line)) {
            string dec = decrypt(line);
            
            // Check if we're entering the target complaint
            if (dec.rfind("SEARCH_CODE: " + complaint.search_code, 0) == 0) {
                inTargetComplaint = true;
            }
            
            // Update status line
            if (inTargetComplaint && dec.rfind("Status: ", 0) == 0) {
                out << encrypt("Status: " + newStatus) << "\n";
                updated = true;
            } else {
                out << line << "\n";
            }
            
            // Check if we're leaving the complaint (empty line)
            if (inTargetComplaint && line.empty()) {
                inTargetComplaint = false;
            }
        }
        
        in.close();
        out.close();
        
        if (updated) {
            remove("complain.txt");
            rename("temp.txt", "complain.txt");
        } else {
            remove("temp.txt");
            cout << RED << "Warning: Could not update complaint status in file!\n" << RESET;
        }
        cacheDirty = true; // Mark cache as dirty
    }

public:
    void run() {
        if (!adminLogin()) {
            return;
        }

        while (true) {
            loadComplaints("complain.txt", complaints);
            loadComplaints("resolved.txt", resolvedComplaints);
            
            vector<string> options = {
                "View All Complaints",
                "Date-wise Search (Advanced)",
                "Search Complaints by Other Criteria",
                "Resolve Complaint",
                "View Emergency Complaints",
                "View Statistics & Analytics",
                "View Resolved Complaints",
                "Backup System Data",
                "Logout"
            };

            int choice = showMenu(options, "ADMIN DASHBOARD - Welcome " + currentUser);
            
            if (choice == -1) continue;

            switch (choice) {
                case 0: viewAllComplaints(); break;
                case 1: date_wise(); break;
                case 2: searchComplaints(); break;
                case 3: resolveComplaint(); break;
                case 4: viewEmergencyComplaints(); break;
                case 5: viewStatistics(); break;
                case 6: viewResolvedComplaints(); break;
                case 7: backupData(); break;
                case 8: 
                    cout << GREEN << "Logging out... Goodbye!\n" << RESET;
                    Sleep(1000);
                    return;
            }
        }
    }

private:
    void viewAllComplaints() {
        header("ALL ACTIVE COMPLAINTS");
        
        // Reload complaints to ensure we have the latest data
        loadComplaints("complain.txt", complaints);
        
        if (complaints.empty()) {
            cout << YELLOW << "No active complaints found.\n" << RESET;
            pause();
            return;
        }

        cout << BOLD << "Total Active Complaints: " << complaints.size() << "\n\n" << RESET;
        
        // Show summary statistics
        map<string, int> priorityCount, typeCount, statusCount;
        for (const auto& c : complaints) {
            priorityCount[c.priority]++;
            typeCount[c.complain_type]++;
            statusCount[c.status]++;
        }
        
        cout << CYAN << "Priority Summary:\n" << RESET;
        for (const auto& p : priorityCount) {
            string color = (p.first == "Emergency") ? BG_RED :
                          (p.first == "High") ? RED :
                          (p.first == "Medium") ? YELLOW : GREEN;
            cout << "  " << p.first << ": " << color << p.second << RESET << "\n";
        }
        
        cout << CYAN << "\nStatus Summary:\n" << RESET;
        for (const auto& s : statusCount) {
            string color = (s.first == "Resolved") ? GREEN :
                          (s.first == "In Progress") ? YELLOW :
                          (s.first == "Pending") ? MAGENTA : RED;
            cout << "  " << s.first << ": " << color << s.second << RESET << "\n";
        }
        
        cout << CYAN << "\nType Summary:\n" << RESET;
        for (const auto& t : typeCount) {
            cout << "  " << t.first << ": " << t.second << "\n";
        }
        cout << string(50, '=') << "\n\n";
        
        for (int i = 0; i < complaints.size(); i++) {
            displayComplaint(complaints[i], i + 1);
        }
        
        cout << BOLD << "\nEnd of active complaints list.\n" << RESET;
        pause();
    }

    void searchComplaints() {
        header("SEARCH COMPLAINTS BY OTHER CRITERIA");
        
        vector<string> searchOptions = {
            "By Search Code", "By User Code", "By Name", 
            "By Complaint Type", "By Priority", "By Status",
            "Return to Main Menu"
        };
        
        int searchTypeIndex = showMenu(searchOptions, "SEARCH BY CRITERIA");
        if (searchTypeIndex == -1) return;
        if (searchTypeIndex == 6) return; // Return to main menu
        
        string searchType = searchOptions[searchTypeIndex];
        string query;
        cout << BOLD << "Enter search term: " << RESET;
        getline(cin, query);
        
        vector<Complaint> results;
        
        // Search in both active and resolved complaints
        vector<Complaint> allComplaints = complaints;
        allComplaints.insert(allComplaints.end(), resolvedComplaints.begin(), resolvedComplaints.end());
        
        for (const auto& c : allComplaints) {
            bool match = false;
            string searchField;
            
            if (searchType == "By Search Code") searchField = c.search_code;
            else if (searchType == "By User Code") searchField = c.user_code;
            else if (searchType == "By Name") searchField = c.name;
            else if (searchType == "By Complaint Type") searchField = c.complain_type;
            else if (searchType == "By Priority") searchField = c.priority;
            else if (searchType == "By Status") searchField = c.status;
            
            if (tolower_str(searchField).find(tolower_str(query)) != string::npos) {
                results.push_back(c);
            }
        }
        
        if (results.empty()) {
            cout << YELLOW << "No complaints found matching your search.\n" << RESET;
        } else {
            cout << BOLD << "\nSearch Results (" << results.size() << " found):\n\n" << RESET;
            
            int activeCount = 0, resolvedCount = 0;
            for (const auto& c : results) {
                if (c.status == "Resolved") resolvedCount++;
                else activeCount++;
            }
            
            cout << "Active: " << activeCount << " | Resolved: " << resolvedCount << "\n";
            cout << string(50, '-') << "\n\n";
            
            for (int i = 0; i < results.size(); i++) {
                string type = (results[i].status == "Resolved") ? "RESOLVED" : "ACTIVE";
                cout << BG_CYAN << BOLD << "[" << type << "] Complaint #" << (i + 1) << RESET << "\n";
                displayComplaint(results[i]);
            }
        }
        pause();
    }

   void resolveComplaint() {
    header("RESOLVE COMPLAINT");
    
    // Reload to get current data
    loadComplaints("complain.txt", complaints);
    loadComplaints("resolved.txt", resolvedComplaints);
    
    if (complaints.empty()) {
        cout << YELLOW << "No active complaints to resolve.\n" << RESET;
        pause();
        return;
    }
    
    string searchCode;
    cout << BOLD << "Enter Search Code of complaint to resolve: " << RESET;
    getline(cin, searchCode);
    
    // Find the complaint
    Complaint* targetComplaint = nullptr;
    int complaintIndex = -1;
    for (int i = 0; i < complaints.size(); i++) {
        if (complaints[i].search_code == searchCode) {
            targetComplaint = &complaints[i];
            complaintIndex = i;
            break;
        }
    }
    
    if (targetComplaint) {
        cout << GREEN << "Complaint found!\n\n" << RESET;
        displayComplaint(*targetComplaint);
        
        vector<string> statusOptions = {"Resolved", "In Progress", "Pending", "Rejected"};
        int statusIndex = showMenu(statusOptions, "UPDATE STATUS TO");
        if (statusIndex == -1) return;
        
        string newStatus = statusOptions[statusIndex];
        
        if (newStatus == "Resolved") {
            // Move complaint from active to resolved
            moveComplaintToResolved(*targetComplaint);
            
            // RELOAD DATA AFTER CHANGES - THIS WAS MISSING
            loadComplaints("complain.txt", complaints);
            loadComplaints("resolved.txt", resolvedComplaints);
            
            cout << GREEN << "Complaint resolved and moved to resolved records!\n" << RESET;
            cout << CYAN << "The complaint will no longer appear in active complaints view.\n" << RESET;
        } else {
            // Just update the status in the active file
            updateComplaintStatus(*targetComplaint, newStatus);
            
            // RELOAD DATA AFTER CHANGES - THIS WAS MISSING
            loadComplaints("complain.txt", complaints);
            loadComplaints("resolved.txt", resolvedComplaints);
            
            cout << GREEN << "Complaint status updated to: " << newStatus << "\n" << RESET;
        }
        
        pause();
        return;
    }
    
    cout << RED << "No complaint found with that Search Code!\n" << RESET;
    pause();
}
    void viewEmergencyComplaints() {
        header("EMERGENCY COMPLAINTS");
        
        vector<Complaint> emergencies;
        for (const auto& c : complaints) {
            if (c.priority == "Emergency") {
                emergencies.push_back(c);
            }
        }
        
        if (emergencies.empty()) {
            cout << GREEN << "No emergency complaints at this time.\n" << RESET;
        } else {
            cout << BG_RED << WHITE << BOLD << "!!! " << emergencies.size() << " EMERGENCY COMPLAINTS NEED IMMEDIATE ATTENTION !!!" << RESET << "\n\n";
            
            for (int i = 0; i < emergencies.size(); i++) {
                cout << BG_RED << WHITE << BOLD << "EMERGENCY COMPLAINT #" << (i + 1) << RESET << "\n";
                displayComplaint(emergencies[i]);
                
                // Alert sound for emergencies
                Beep(1000, 300);
                Beep(1200, 300);
            }
        }
        pause();
    }

    void viewStatistics() {
        header("STATISTICS & ANALYTICS");
        
        map<string, int> typeStats, priorityStats, statusStats, identityStats, againstStats;
        int total = complaints.size() + resolvedComplaints.size();
        
        // Analyze active complaints
        for (const auto& c : complaints) {
            typeStats[c.complain_type]++;
            priorityStats[c.priority]++;
            statusStats[c.status]++;
            identityStats[c.i_identify_myself]++;
            againstStats[c.complain_against_for]++;
        }
        
        // Analyze resolved complaints
        for (const auto& c : resolvedComplaints) {
            typeStats[c.complain_type]++;
            identityStats[c.i_identify_myself]++;
            againstStats[c.complain_against_for]++;
        }
        
        cout << BOLD << "SYSTEM STATISTICS\n" << RESET;
        cout << string(50, '=') << "\n";
        cout << "Total Complaints (All Time): " << total << "\n";
        cout << "Active Complaints: " << complaints.size() << "\n";
        cout << "Resolved Complaints: " << resolvedComplaints.size() << "\n";
        cout << "Resolution Rate: " << (total > 0 ? (resolvedComplaints.size() * 100 / total) : 0) << "%\n\n";
        
        cout << BOLD << "COMPLAINT TYPES:\n" << RESET;
        for (const auto& stat : typeStats) {
            cout << "  " << stat.first << ": " << stat.second << "\n";
        }
        
        cout << BOLD << "\nPRIORITY DISTRIBUTION (Active):\n" << RESET;
        for (const auto& stat : priorityStats) {
            string color = (stat.first == "Emergency") ? BG_RED :
                          (stat.first == "High") ? RED :
                          (stat.first == "Medium") ? YELLOW : GREEN;
            cout << "  " << stat.first << ": " << color << stat.second << RESET << "\n";
        }
        
        cout << BOLD << "\nSTATUS DISTRIBUTION (Active):\n" << RESET;
        for (const auto& stat : statusStats) {
            string color = (stat.first == "Resolved") ? GREEN :
                          (stat.first == "In Progress") ? YELLOW :
                          (stat.first == "Pending") ? MAGENTA : RED;
            cout << "  " << stat.first << ": " << color << stat.second << RESET << "\n";
        }
        
        cout << BOLD << "\nCOMPLAINANT IDENTITY:\n" << RESET;
        for (const auto& stat : identityStats) {
            cout << "  " << stat.first << ": " << stat.second << "\n";
        }
        
        cout << BOLD << "\nCOMPLAINTS AGAINST:\n" << RESET;
        for (const auto& stat : againstStats) {
            cout << "  " << stat.first << ": " << stat.second << "\n";
        }
        
        pause();
    }

    void viewResolvedComplaints() {
        header("RESOLVED COMPLAINTS");
        
        if (resolvedComplaints.empty()) {
            cout << YELLOW << "No resolved complaints found.\n" << RESET;
            pause();
            return;
        }
        
        cout << BOLD << "Total Resolved Complaints: " << resolvedComplaints.size() << "\n\n" << RESET;
        
        for (int i = 0; i < resolvedComplaints.size(); i++) {
            displayComplaint(resolvedComplaints[i], i + 1);
        }
        
        pause();
    }

    void backupData() {
        header("BACKUP SYSTEM DATA");
        
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", ltm);
        
        string backup_cmd1 = "copy complain.txt backup_complain_" + string(timestamp) + ".txt >nul";
        string backup_cmd2 = "copy resolved.txt backup_resolved_" + string(timestamp) + ".txt >nul";
        
        system(backup_cmd1.c_str());
        system(backup_cmd2.c_str());
        
        cout << GREEN << "System data backed up successfully!\n" << RESET;
        cout << "Backup files created with timestamp: " << timestamp << "\n";
        pause();
    }
};

int main() {
    SetConsoleTitleA("Admin Complaint Management System");
    AdminSystem admin;
    admin.run();
    return 0;
}