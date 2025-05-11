#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

const vector<string> JOB_STATUSES = {
    "Applied",
    "Resume Review",
    "Technical Assignment",
    "HR Interview",
    "Technical Interview",
    "Final Interview",
    "Offer Received",
    "Offer Accepted",
    "Offer Declined",
    "Rejected"
};

struct Application {
    int id;
    string companyName;
    string jobTitle;
    string applicationDate;
    string status;
    string notes;

    Application(int i = 0, string cn = "", string jt = "", string ad = "", string st = "", string n = "")
        : id(i), companyName(cn), jobTitle(jt), applicationDate(ad), status(st), notes(n) {
    }
};

const string FILENAME = "applications.txt";
int nextId = 1;

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getStringInput(const string& prompt, bool allowNull = false) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty() || allowNull) {
            if (allowNull || input.find_first_not_of(' ') != string::npos) {
                break;
            }
        }
        if (!allowNull) {
            cout << "Error: Field cannot be empty. Please enter valid data.\n";
        }
    }
    return input;
}

int getIntInput(const string& prompt) {
    int input_val;
    while (true) {
        cout << prompt;
        cin >> input_val;
        if (cin.good()) {
            clearInputBuffer();
            return input_val;
        }
        else {
            cout << "Error: Not a number. Please enter valid data.\n";
            cin.clear();
            clearInputBuffer();
        }
    }
}

void loadApplications(vector<Application>& apps) {
    apps.clear();
    ifstream inFile(FILENAME);
    if (!inFile.is_open()) {
        nextId = 1;
        return;
    }

    string line;
    int maxId = 0;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string segment;
        Application app;
        vector<string> segments;

        while (getline(ss, segment, '|')) {
            segments.push_back(segment);
        }

        if (segments.size() == 6) {
            try {
                app.id = stoi(segments[0]);
                app.companyName = segments[1];
                app.jobTitle = segments[2];
                app.applicationDate = segments[3];
                app.status = segments[4];
                app.notes = segments[5];
                apps.push_back(app);
                if (app.id > maxId) {
                    maxId = app.id;
                }
            }
            catch (const invalid_argument& ia) {
                cerr << "Warning: Detected corrupted ID in file line: " << line << " (" << ia.what() << ")" << endl;
            }
            catch (const out_of_range& oor) {
                cerr << "Warning: Detected out-of-range ID in file line: " << line << " (" << oor.what() << ")" << endl;
            }
        }
        else {
            cerr << "Warning: Detected line with incorrect number of fields (" << segments.size() << "/6) in file: " << line << endl;
        }
    }

    if (apps.empty()) {
        nextId = 1;
    }
    else {
        nextId = maxId + 1;
    }
    inFile.close();
}

void saveApplications(const vector<Application>& apps) {
    ofstream outFile(FILENAME);
    if (!outFile.is_open()) {
        cout << "Critical Error: Failed to open file " << FILENAME << " for writing.\n";
        return;
    }

    for (const auto& app : apps) {
        outFile << app.id << "|"
            << app.companyName << "|"
            << app.jobTitle << "|"
            << app.applicationDate << "|"
            << app.status << "|"
            << app.notes << endl;
    }
    outFile.close();
}

string selectStatus(const string& currentStatus = "", bool allowNoChange = false) {
    cout << "Select application status:\n";
    for (size_t i = 0; i < JOB_STATUSES.size(); ++i) {
        cout << i + 1 << ". " << JOB_STATUSES[i] << "\n";
    }
    if (allowNoChange && !currentStatus.empty()) {
        cout << "0. Keep current (" << currentStatus << ")\n";
    }

    int choice;
    while (true) {
        choice = getIntInput("Your choice (number): ");
        if (allowNoChange && choice == 0) {
            return currentStatus;
        }
        if (choice >= 1 && choice <= static_cast<int>(JOB_STATUSES.size())) {
            return JOB_STATUSES[choice - 1];
        }
        else {
            cout << "Error: Invalid option! Please select a number from the list.\n";
        }
    }
}

void createApplication(vector<Application>& apps) {
    cout << "\n--- Add New Application ---\n";
    Application newApp;
    newApp.id = nextId++;

    newApp.companyName = getStringInput("Company Name: ");
    newApp.jobTitle = getStringInput("Job Title: ");
    newApp.applicationDate = getStringInput("Application Date (e.g., YYYY-MM-DD): ");

    cout << "\n--- Select Status ---\n";
    newApp.status = selectStatus();

    newApp.notes = getStringInput("Notes (can be left empty, press Enter): ", true);

    apps.push_back(newApp);
    saveApplications(apps);
    cout << "Application added successfully!\n";
}

void readApplications(const vector<Application>& apps) {
    cout << "\n--- List of All Applications ---\n";
    if (apps.empty()) {
        cout << "Application list is empty.\n";
        return;
    }
    for (const auto& app : apps) {
        cout << "-----------------------------\n";
        cout << "ID: " << app.id << "\n";
        cout << "Company: " << app.companyName << "\n";
        cout << "Job Title: " << app.jobTitle << "\n";
        cout << "Application Date: " << app.applicationDate << "\n";
        cout << "Status: " << app.status << "\n";
        cout << "Notes: " << (app.notes.empty() ? "None" : app.notes) << "\n";
    }
    cout << "-----------------------------\n";
}

void updateApplication(vector<Application>& apps) {
    cout << "\n--- Update Application ---\n";
    if (apps.empty()) {
        cout << "Application list is empty. Nothing to update.\n";
        return;
    }

    readApplications(apps);
    int idToUpdate = getIntInput("Enter ID of application to update: ");

    auto it = find_if(apps.begin(), apps.end(), [idToUpdate](const Application& app) {
        return app.id == idToUpdate;
        });

    if (it != apps.end()) {
        Application& appToUpdate = *it;
        cout << "Editing Application ID: " << appToUpdate.id << "\n";

        string tempInput;

        cout << "Current Company Name: " << appToUpdate.companyName << "\n";
        tempInput = getStringInput("New Company Name (Enter to keep '" + appToUpdate.companyName + "'): ", true);
        if (!tempInput.empty() || appToUpdate.companyName.empty()) {
            appToUpdate.companyName = tempInput;
        }

        cout << "Current Job Title: " << appToUpdate.jobTitle << "\n";
        tempInput = getStringInput("New Job Title (Enter to keep '" + appToUpdate.jobTitle + "'): ", true);
        if (!tempInput.empty() || appToUpdate.jobTitle.empty()) {
            appToUpdate.jobTitle = tempInput;
        }

        cout << "Current Application Date: " << appToUpdate.applicationDate << "\n";
        tempInput = getStringInput("New Application Date (Enter to keep '" + appToUpdate.applicationDate + "'): ", true);
        if (!tempInput.empty() || appToUpdate.applicationDate.empty()) {
            appToUpdate.applicationDate = tempInput;
        }

        cout << "\n--- Update Status --- \nCurrent Status: " << appToUpdate.status << "\n";
        appToUpdate.status = selectStatus(appToUpdate.status, true);

        cout << "Current Notes: " << (appToUpdate.notes.empty() ? "None" : appToUpdate.notes) << "\n";
        tempInput = getStringInput("New Notes (Enter to keep current): ", true);
        if (!tempInput.empty() || appToUpdate.notes.empty()) {
            appToUpdate.notes = tempInput;
        }

        saveApplications(apps);
        cout << "Application updated successfully!\n";
    }
    else {
        cout << "Error: Application with ID " << idToUpdate << " not found.\n";
    }
}

void deleteApplication(vector<Application>& apps) {
    cout << "\n--- Delete Application ---\n";
    if (apps.empty()) {
        cout << "Application list is empty. Nothing to delete.\n";
        return;
    }
    readApplications(apps);
    int idToDelete = getIntInput("Enter ID of application to delete: ");

    auto it = remove_if(apps.begin(), apps.end(), [idToDelete](const Application& app) {
        return app.id == idToDelete;
        });

    if (it != apps.end()) {
        apps.erase(it, apps.end());
        saveApplications(apps);
        cout << "Application deleted successfully!\n";
    }
    else {
        cout << "Error: Application with ID " << idToDelete << " not found.\n";
    }
}

void displayMenu() {
    cout << "\n===== Job Application Tracker =====\n";
    cout << "1. Add Application\n";
    cout << "2. View All Applications\n";
    cout << "3. Update Application\n";
    cout << "4. Delete Application\n";
    cout << "0. Exit\n";
    cout << "===============================\n";
}

int main() {
    vector<Application> applications;
    loadApplications(applications);

    int choice;
    do {
        displayMenu();
        choice = getIntInput("Select an action: ");

        switch (choice) {
        case 1:
            createApplication(applications);
            break;
        case 2:
            readApplications(applications);
            break;
        case 3:
            updateApplication(applications);
            break;
        case 4:
            deleteApplication(applications);
            break;
        case 0:
            cout << "Saving data and exiting program...\n";
            saveApplications(applications);
            break;
        default:
            cout << "Error: Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}