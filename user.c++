#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <random>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

#define RED "\033[31m"
#define BLACK "\033[30m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define BG_CYAN "\033[46m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"

class ComplainBox {
private:
    struct Complaint {
        string user_code, search_code;
        string i_am, against;
        string name, id, intake, section, dept;
        string staff_designation, staff_office, stranger_role;
        string against_name, against_id, against_intake, against_section, against_dept;
        string against_faculty_name, against_faculty_dept;
        string against_staff_name, against_staff_designation, against_staff_office;
        string against_stranger_name, against_stranger_role;
        string complain_type, priority, date, describe;
        string status;
        vector<string> lines;
    };

    vector<Complaint> complaints;
    const string FILE = "complain.txt";

    string encrypt(const string& s) { 
        string e = s; 
        for (char& c : e) c += 3; 
        return e; 
    }
    
    string decrypt(const string& s) { 
        string d = s; 
        for (char& c : d) c -= 3; 
        return d; 
    }

    string generate_code() {
        static mt19937 gen(random_device{}());
        uniform_int_distribution<> dis(100000, 999999);
        return to_string(dis(gen));
    }

    void clear() { system("cls"); }

    void pause() { 
        cout << YELLOW << "\nPress any key to continue..." << RESET; 
        _getch(); 
    }

    void header(const string& t) {
        clear();
        cout << BG_CYAN << BOLD << WHITE << string(80, ' ') << RESET << "\n";
        cout << BG_CYAN << BOLD << WHITE << setw(80) << (" " + t + " ") << RESET << "\n";
        cout << BG_CYAN << BOLD << WHITE << string(80, ' ') << RESET << "\n\n";
    }

    string get_date() {
        time_t t = time(0);
        tm* now = localtime(&t);
        char buf[20];
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", now);
        return string(buf);
    }

    void load() {
        complaints.clear();
        ifstream in(FILE);
        if (!in) return;

        Complaint c;
        string line;
        while (getline(in, line)) {
            if (line.empty()) {
                if (!c.user_code.empty()) complaints.push_back(c);
                c = Complaint();
                continue;
            }
            c.lines.push_back(line);
            string d = decrypt(line);
            
            // Parse all fields
            if (d.rfind("USER_CODE: ", 0) == 0) c.user_code = d.substr(11);
            else if (d.rfind("SEARCH_CODE: ", 0) == 0) c.search_code = d.substr(13);
            else if (d.rfind("I am a : ", 0) == 0) c.i_am = d.substr(9);
            else if (d.rfind("Complain Against for : ", 0) == 0) c.against = d.substr(23);
            else if (d.rfind("Name : ", 0) == 0) c.name = d.substr(7);
            else if (d.rfind("ID : ", 0) == 0) c.id = d.substr(5);
            else if (d.rfind("Intake : ", 0) == 0) c.intake = d.substr(9);
            else if (d.rfind("Section : ", 0) == 0) c.section = d.substr(10);
            else if (d.rfind("Department : ", 0) == 0) c.dept = d.substr(13);
            else if (d.rfind("Staff Designation/Job Title : ", 0) == 0) c.staff_designation = d.substr(30);
            else if (d.rfind("Staff Office/Department : ", 0) == 0) c.staff_office = d.substr(26);
            else if (d.rfind("Stranger Role : ", 0) == 0) c.stranger_role = d.substr(16);
            else if (d.rfind("Against : ", 0) == 0) c.against_name = d.substr(10);
            else if (d.rfind("Against ID : ", 0) == 0) c.against_id = d.substr(13);
            else if (d.rfind("Against Intake : ", 0) == 0) c.against_intake = d.substr(17);
            else if (d.rfind("Against Section : ", 0) == 0) c.against_section = d.substr(18);
            else if (d.rfind("Against Department : ", 0) == 0) c.against_dept = d.substr(21);
            else if (d.rfind("Against Faculty Name: ", 0) == 0) c.against_faculty_name = d.substr(22);
            else if (d.rfind("Against Faculty Department: ", 0) == 0) c.against_faculty_dept = d.substr(28);
            else if (d.rfind("Against Staff Name : ", 0) == 0) c.against_staff_name = d.substr(21);
            else if (d.rfind("Against Staff Designation : ", 0) == 0) c.against_staff_designation = d.substr(29);
            else if (d.rfind("Against Staff Office : ", 0) == 0) c.against_staff_office = d.substr(23);
            else if (d.rfind("Against Stranger Name : ", 0) == 0) c.against_stranger_name = d.substr(24);
            else if (d.rfind("Against Stranger Role : ", 0) == 0) c.against_stranger_role = d.substr(24);
            else if (d.rfind("Complain : ", 0) == 0) c.complain_type = d.substr(11);
            else if (d.rfind("Priority: ", 0) == 0) c.priority = d.substr(10);
            else if (d.rfind("Date: ", 0) == 0) c.date = d.substr(6);
            else if (d.rfind("Describe: ", 0) == 0) c.describe = d.substr(10);
            else if (d.rfind("Status: ", 0) == 0) c.status = d.substr(8);
        }
        if (!c.user_code.empty()) complaints.push_back(c);
        in.close();
    }

    void save() {
        ofstream out(FILE);
        for (const auto& c : complaints) {
            for (const auto& l : c.lines) {
                out << l << "\n";
            }
            out << "\n";
        }
        out.close();
    }

    void backup() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", ltm);
        
        string backup_cmd = "copy " + FILE + " backup_" + string(timestamp) + ".txt >nul";
        system(backup_cmd.c_str());
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

    void edit_complaint(Complaint& c) {
        while (true) {
            vector<string> editOptions = {
                "Edit Description",
                "Edit Complaint Type", 
                "Edit Priority Level",
                "Edit Against Information",
                "Finish Editing"
            };
            
            int choice = showMenu(editOptions, "EDIT COMPLAINT - " + c.user_code);
            
            if (choice == -1) continue;
            if (choice == 4) break; // Finish Editing
            
            switch (choice) {
                case 0: // Edit Description
                    cout << BOLD << "Current Description: " << RESET << c.describe << "\n";
                    cout << BOLD << "New Description: " << RESET;
                    getline(cin, c.describe);
                    break;
                    
                case 1: // Edit Complaint Type
                    {
                        vector<string> complaintTypes = {
                            "Academic Issue", "Behavior Issue", "Harassment", 
                            "Bullying", "Discrimination", "Facilities Issue",
                            "Administrative Issue", "Other"
                        };
                        int typeIndex = showMenu(complaintTypes, "SELECT COMPLAINT TYPE");
                        if (typeIndex != -1) {
                            c.complain_type = complaintTypes[typeIndex];
                        }
                    }
                    break;
                    
                case 2: // Edit Priority
                    {
                        vector<string> priorities = {"Low", "Medium", "High", "Emergency"};
                        int priorityIndex = showMenu(priorities, "SELECT PRIORITY LEVEL");
                        if (priorityIndex != -1) {
                            c.priority = priorities[priorityIndex];
                        }
                    }
                    break;
                    
                case 3: // Edit Against Information
                    edit_against_info(c);
                    break;
            }
            
            // Update the file after each edit
            update_complaint_in_file(c);
            cout << GREEN << "Field updated successfully!\n" << RESET;
            pause();
        }
        
        cout << GREEN << "Complaint editing completed!\n" << RESET;
        pause();
    }

    void edit_against_info(Complaint& c) {
        header("EDIT AGAINST INFORMATION");
        
        if (c.against == "Student") {
            cout << BOLD << "Current Against Student Name: " << RESET << c.against_name << "\n";
            cout << BOLD << "New Against Student Name: " << RESET;
            getline(cin, c.against_name);
            
            cout << BOLD << "Current Against Student ID: " << RESET << c.against_id << "\n";
            cout << BOLD << "New Against Student ID: " << RESET;
            getline(cin, c.against_id);
        }
        else if (c.against == "Faculty Member") {
            cout << BOLD << "Current Against Faculty Name: " << RESET << c.against_faculty_name << "\n";
            cout << BOLD << "New Against Faculty Name: " << RESET;
            getline(cin, c.against_faculty_name);
        }
        else if (c.against == "Staff") {
            cout << BOLD << "Current Against Staff Name: " << RESET << c.against_staff_name << "\n";
            cout << BOLD << "New Against Staff Name: " << RESET;
            getline(cin, c.against_staff_name);
        }
        else if (c.against == "Other") {
            cout << BOLD << "Current Against Person Name: " << RESET << c.against_stranger_name << "\n";
            cout << BOLD << "New Against Person Name: " << RESET;
            getline(cin, c.against_stranger_name);
        }
    }

    void update_complaint_in_file(const Complaint& updatedComplaint) {
        ifstream in(FILE);
        vector<string> allLines;
        string line;
        
        while (getline(in, line)) {
            allLines.push_back(line);
        }
        in.close();
        
        ofstream out(FILE);
        bool inTargetComplaint = false;
        bool complaintUpdated = false;
        
        for (size_t i = 0; i < allLines.size(); i++) {
            string dec = decrypt(allLines[i]);
            
            if (dec.rfind("USER_CODE: " + updatedComplaint.user_code, 0) == 0) {
                inTargetComplaint = true;
                complaintUpdated = true;
                
                auto e = [this](const string& s) { return encrypt(s); };
                out << e("USER_CODE: " + updatedComplaint.user_code) << "\n";
                out << e("SEARCH_CODE: " + updatedComplaint.search_code) << "\n";
                out << e("I am a : " + updatedComplaint.i_am) << "\n";
                out << e("Complain Against for : " + updatedComplaint.against) << "\n";
                out << e("Name : " + updatedComplaint.name) << "\n";
                
                if (!updatedComplaint.id.empty()) out << e("ID : " + updatedComplaint.id) << "\n";
                if (!updatedComplaint.intake.empty()) out << e("Intake : " + updatedComplaint.intake) << "\n";
                if (!updatedComplaint.section.empty()) out << e("Section : " + updatedComplaint.section) << "\n";
                if (!updatedComplaint.dept.empty()) out << e("Department : " + updatedComplaint.dept) << "\n";
                if (!updatedComplaint.staff_designation.empty()) out << e("Staff Designation/Job Title : " + updatedComplaint.staff_designation) << "\n";
                if (!updatedComplaint.staff_office.empty()) out << e("Staff Office/Department : " + updatedComplaint.staff_office) << "\n";
                if (!updatedComplaint.stranger_role.empty()) out << e("Stranger Role : " + updatedComplaint.stranger_role) << "\n";
                
                if (!updatedComplaint.against_name.empty()) out << e("Against : " + updatedComplaint.against_name) << "\n";
                if (!updatedComplaint.against_id.empty()) out << e("Against ID : " + updatedComplaint.against_id) << "\n";
                if (!updatedComplaint.against_intake.empty()) out << e("Against Intake : " + updatedComplaint.against_intake) << "\n";
                if (!updatedComplaint.against_section.empty()) out << e("Against Section : " + updatedComplaint.against_section) << "\n";
                if (!updatedComplaint.against_dept.empty()) out << e("Against Department : " + updatedComplaint.against_dept) << "\n";
                if (!updatedComplaint.against_faculty_name.empty()) out << e("Against Faculty Name: " + updatedComplaint.against_faculty_name) << "\n";
                if (!updatedComplaint.against_faculty_dept.empty()) out << e("Against Faculty Department: " + updatedComplaint.against_faculty_dept) << "\n";
                if (!updatedComplaint.against_staff_name.empty()) out << e("Against Staff Name : " + updatedComplaint.against_staff_name) << "\n";
                if (!updatedComplaint.against_staff_designation.empty()) out << e("Against Staff Designation : " + updatedComplaint.against_staff_designation) << "\n";
                if (!updatedComplaint.against_staff_office.empty()) out << e("Against Staff Office : " + updatedComplaint.against_staff_office) << "\n";
                if (!updatedComplaint.against_stranger_name.empty()) out << e("Against Stranger Name : " + updatedComplaint.against_stranger_name) << "\n";
                if (!updatedComplaint.against_stranger_role.empty()) out << e("Against Stranger Role : " + updatedComplaint.against_stranger_role) << "\n";
                
                out << e("Complain : " + updatedComplaint.complain_type) << "\n";
                out << e("Priority: " + updatedComplaint.priority) << "\n";
                out << e("Date: " + updatedComplaint.date) << "\n";
                out << e("Status: " + updatedComplaint.status) << "\n";
                out << e("Describe: " + updatedComplaint.describe) << "\n\n";
                
                while (i < allLines.size() && !allLines[i].empty()) {
                    i++;
                }
                continue;
            }
            
            if (inTargetComplaint && allLines[i].empty()) {
                inTargetComplaint = false;
                continue;
            }
            
            if (!inTargetComplaint) {
                out << allLines[i] << "\n";
            }
        }
        
        out.close();
        load();
    }

public:
    void run() {
        load();
        while (true) {
            vector<string> options = {
                "Submit New Complaint",
                "Edit My Complaint by USER CODE", 
                "Delete My Complaint by USER CODE",
                "View My Complaints",
                "Check Complaint Status",
                "Exit System"
            };
            
            int choice = showMenu(options, "COMPLAINT MANAGEMENT SYSTEM - USER PORTAL");
            
            if (choice == -1) continue;
            
            switch (choice) {
                case 0: submit_new(); backup(); break;
                case 1: edit_by_code(); break;
                case 2: delete_by_code(); break;
                case 3: view_my(); break;
                case 4: check_status(); break;
                case 5: 
                    save();
                    cout << GREEN << "Thank you for using the Complaint System!\n" << RESET;
                    return;
            }
        }
    }

private:
    void submit_new() {
        header("SUBMIT NEW COMPLAINT");
        Complaint c;
        c.user_code = generate_code();
        c.search_code = generate_code();
        c.date = get_date();
        c.status = "Pending";

        // Select identity using menu
        vector<string> identities = {"Student", "Faculty Member", "Staff", "Other"};
        int identityIndex = showMenu(identities, "SELECT YOUR IDENTITY");
        if (identityIndex == -1) return;
        c.i_am = identities[identityIndex];

        cout << "\n" << BOLD << "Your Name: " << RESET;
        getline(cin, c.name);

        // Select complaint against using menu
        vector<string> againstOptions = {"Student", "Faculty Member", "Staff", "Department", "Other"};
        int againstIndex = showMenu(againstOptions, "COMPLAINT AGAINST");
        if (againstIndex == -1) return;
        c.against = againstOptions[againstIndex];

        // Collect identity-specific information
        if (c.i_am == "Student") {
            cout << BOLD << "Student ID: " << RESET;
            getline(cin, c.id);
            cout << BOLD << "Intake: " << RESET;
            getline(cin, c.intake);
            cout << BOLD << "Section: " << RESET;
            getline(cin, c.section);
            cout << BOLD << "Department: " << RESET;
            getline(cin, c.dept);
        } else if (c.i_am == "Faculty Member" || c.i_am == "Staff") {
            cout << BOLD << "Designation: " << RESET;
            getline(cin, c.staff_designation);
            cout << BOLD << "Office/Department: " << RESET;
            getline(cin, c.staff_office);
        } else if (c.i_am == "Other") {
            cout << BOLD << "Your Role: " << RESET;
            getline(cin, c.stranger_role);
        }

        // Collect against-specific information
        if (c.against == "Student") {
            cout << BOLD << "Against Student Name: " << RESET;
            getline(cin, c.against_name);
            cout << BOLD << "Against Student ID: " << RESET;
            getline(cin, c.against_id);
            cout << BOLD << "Against Student Intake: " << RESET;
            getline(cin, c.against_intake);
            cout << BOLD << "Against Student Section: " << RESET;
            getline(cin, c.against_section);
            cout << BOLD << "Against Student Department: " << RESET;
            getline(cin, c.against_dept);
        } else if (c.against == "Faculty Member") {
            cout << BOLD << "Against Faculty Name: " << RESET;
            getline(cin, c.against_faculty_name);
            cout << BOLD << "Against Faculty Department: " << RESET;
            getline(cin, c.against_faculty_dept);
        } else if (c.against == "Staff") {
            cout << BOLD << "Against Staff Name: " << RESET;
            getline(cin, c.against_staff_name);
            cout << BOLD << "Against Staff Designation: " << RESET;
            getline(cin, c.against_staff_designation);
            cout << BOLD << "Against Staff Office: " << RESET;
            getline(cin, c.against_staff_office);
        } else if (c.against == "Other") {
            cout << BOLD << "Against Person Name: " << RESET;
            getline(cin, c.against_stranger_name);
            cout << BOLD << "Against Person Role: " << RESET;
            getline(cin, c.against_stranger_role);
        }

        // Select complaint type from menu
        vector<string> complaintTypes = {
            "Academic Issue", "Behavior Issue", "Harassment", 
            "Bullying", "Discrimination", "Facilities Issue",
            "Administrative Issue", "Other"
        };
        int typeIndex = showMenu(complaintTypes, "COMPLAINT TYPE");
        if (typeIndex == -1) return;
        c.complain_type = complaintTypes[typeIndex];

        // Select priority from menu
        vector<string> priorities = {"Low", "Medium", "High", "Emergency"};
        int priorityIndex = showMenu(priorities, "PRIORITY LEVEL");
        if (priorityIndex == -1) return;
        c.priority = priorities[priorityIndex];

        cout << BOLD << "Describe your complaint:\n" << RESET;
        getline(cin, c.describe);

        // Save complaint
        ofstream out(FILE, ios::app);
        auto e = [this](const string& s) { return encrypt(s); };
        
        out << e("USER_CODE: " + c.user_code) << "\n";
        out << e("SEARCH_CODE: " + c.search_code) << "\n";
        out << e("I am a : " + c.i_am) << "\n";
        out << e("Complain Against for : " + c.against) << "\n";
        out << e("Name : " + c.name) << "\n";
        
        if (!c.id.empty()) out << e("ID : " + c.id) << "\n";
        if (!c.intake.empty()) out << e("Intake : " + c.intake) << "\n";
        if (!c.section.empty()) out << e("Section : " + c.section) << "\n";
        if (!c.dept.empty()) out << e("Department : " + c.dept) << "\n";
        if (!c.staff_designation.empty()) out << e("Staff Designation/Job Title : " + c.staff_designation) << "\n";
        if (!c.staff_office.empty()) out << e("Staff Office/Department : " + c.staff_office) << "\n";
        if (!c.stranger_role.empty()) out << e("Stranger Role : " + c.stranger_role) << "\n";
        
        if (!c.against_name.empty()) out << e("Against : " + c.against_name) << "\n";
        if (!c.against_id.empty()) out << e("Against ID : " + c.against_id) << "\n";
        if (!c.against_intake.empty()) out << e("Against Intake : " + c.against_intake) << "\n";
        if (!c.against_section.empty()) out << e("Against Section : " + c.against_section) << "\n";
        if (!c.against_dept.empty()) out << e("Against Department : " + c.against_dept) << "\n";
        if (!c.against_faculty_name.empty()) out << e("Against Faculty Name: " + c.against_faculty_name) << "\n";
        if (!c.against_faculty_dept.empty()) out << e("Against Faculty Department: " + c.against_faculty_dept) << "\n";
        if (!c.against_staff_name.empty()) out << e("Against Staff Name : " + c.against_staff_name) << "\n";
        if (!c.against_staff_designation.empty()) out << e("Against Staff Designation : " + c.against_staff_designation) << "\n";
        if (!c.against_staff_office.empty()) out << e("Against Staff Office : " + c.against_staff_office) << "\n";
        if (!c.against_stranger_name.empty()) out << e("Against Stranger Name : " + c.against_stranger_name) << "\n";
        if (!c.against_stranger_role.empty()) out << e("Against Stranger Role : " + c.against_stranger_role) << "\n";
        
        out << e("Complain : " + c.complain_type) << "\n";
        out << e("Priority: " + c.priority) << "\n";
        out << e("Date: " + c.date) << "\n";
        out << e("Status: " + c.status) << "\n";
        out << e("Describe: " + c.describe) << "\n\n";
        out.close();

        header("COMPLAINT SUBMITTED SUCCESSFULLY!");
        cout << GREEN << BOLD << "YOUR USER CODE: " << c.user_code << RESET << "\n";
        cout << CYAN << BOLD << "SEARCH CODE (For Admin): " << c.search_code << RESET << "\n";
        cout << "\n" << YELLOW << "IMPORTANT: Save your USER CODE to edit/delete/view your complaint later!\n" << RESET;
        
        if (c.priority == "Emergency") {
            cout << BG_RED << WHITE << BOLD << "!!! EMERGENCY ALERT NOTIFICATION SENT TO ADMIN !!!" << RESET << "\n";
            for (int i = 0; i < 3; i++) {
                Beep(1000, 500);
                Sleep(200);
            }
        }
        
        pause();
        load();
    }

    void edit_by_code() {
        header("EDIT MY COMPLAINT");
        string code = ask_code("USER CODE");
        
        for (auto& c : complaints) {
            if (c.user_code == code) {
                cout << GREEN << "Complaint found!\n" << RESET;
                edit_complaint(c);
                return;
            }
        }
        cout << RED << "No complaint found with that USER CODE!\n" << RESET;
        pause();
    }

    void delete_by_code() {
        header("DELETE MY COMPLAINT");
        string code = ask_code("USER CODE");
        
        auto it = remove_if(complaints.begin(), complaints.end(),
            [code](const Complaint& x) { return x.user_code == code; });
            
        if (it != complaints.end()) {
            complaints.erase(it, complaints.end());
            save();
            load();
            cout << GREEN << "Complaint deleted successfully!\n" << RESET;
        } else {
            cout << RED << "No complaint found with that USER CODE!\n" << RESET;
        }
        pause();
    }

    void view_my() {
        header("MY COMPLAINTS");
        string code = ask_code("YOUR USER CODE");
        bool found = false;
        
        for (const auto& c : complaints) {
            if (c.user_code == code) {
                if (!found) {
                    cout << BOLD << CYAN << "Your Complaints:\n" << RESET;
                    cout << string(60, '-') << "\n";
                    found = true;
                }
                
                string status_color = (c.status == "Resolved") ? GREEN : 
                                     (c.status == "In Progress") ? YELLOW : RED;
                string priority_color = (c.priority == "Emergency") ? BG_RED :
                                       (c.priority == "High") ? RED :
                                       (c.priority == "Medium") ? YELLOW : GREEN;
                
                cout << BOLD << "Search Code: " << c.search_code << RESET << "\n";
                cout << "Date: " << c.date << "\n";
                cout << "Type: " << c.complain_type << "\n";
                cout << "Priority: " << priority_color << c.priority << RESET << "\n";
                cout << "Status: " << status_color << c.status << RESET << "\n";
                cout << "Description: " << c.describe << "\n";
                cout << string(60, '-') << "\n\n";
            }
        }
        
        if (!found) {
            cout << YELLOW << "No complaints found with your USER CODE.\n" << RESET;
        }
        pause();
    }

    void check_status() {
        header("CHECK COMPLAINT STATUS");
        string code = ask_code("USER CODE");
        bool found = false;
        
        for (const auto& c : complaints) {
            if (c.user_code == code) {
                cout << "\n" << BOLD << "Complaint Status:\n" << RESET;
                cout << string(40, '=') << "\n";
                cout << "Search Code: " << c.search_code << "\n";
                cout << "Date: " << c.date << "\n";
                cout << "Type: " << c.complain_type << "\n";
                
                string priority_color = (c.priority == "Emergency") ? BG_RED :
                                       (c.priority == "High") ? RED :
                                       (c.priority == "Medium") ? YELLOW : GREEN;
                cout << "Priority: " << priority_color << c.priority << RESET << "\n";
                
                string status_color = (c.status == "Resolved") ? GREEN : 
                                     (c.status == "In Progress") ? YELLOW : RED;
                cout << "Status: " << status_color << c.status << RESET << "\n";
                cout << "Description: " << c.describe << "\n";
                cout << string(40, '=') << "\n";
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << RED << "No complaint found with that USER CODE!\n" << RESET;
        }
        pause();
    }

    string ask_code(const string& type) {
        cout << YELLOW << "Enter " << type << ": " << RESET;
        string code; 
        cin >> code; 
        cin.ignore();
        return code;
    }
};

int main() {
    SetConsoleTitleA("User Complaint Management System");
    ComplainBox system;
    system.run();
    return 0;
}