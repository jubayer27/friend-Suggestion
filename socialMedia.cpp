#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
using namespace std;

struct User
{
    string userName;
    string password;
    vector<string> friends;
    User *next;

    User(string userName, string password) : userName(userName), password(password), next(nullptr) {}
};

class SocialMedia
{
private:
    User *head;
    User *currentUser;

public:
    SocialMedia() : head(nullptr), currentUser(nullptr) {}
    void addUser(string username, const string password, bool saveToFile = true)
    {
        User *newUser = new User(username, password);
        if (!head)
        {
            head = newUser;
        }
        else
        {
            User *temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newUser;
        }
        if (saveToFile)
        {
            saveUsersToFile("user.txt"); // Save only if specified
        }
    }
    void addFriend(string friendName)
    {
        if (!currentUser)
        {
            cout << "Please login first!" << endl;
            return;
        }
        if (currentUser->userName == friendName)
        {
            cout << "You can't add yourself as a friend!" << endl;
        }
        if (currentUser->friends.size() >= 10)
        {
            cout << "You can't add more than 10 friends!" << endl;
        }
        if (find(currentUser->friends.begin(), currentUser->friends.end(), friendName) != currentUser->friends.end())
        {
            cout << "Friend already added!" << endl;
        }
        if (currentUser->friends.size() < 10)
        {
            currentUser->friends.push_back(friendName);
            cout << "Friend added successfully!" << endl;
            saveUsersToFile("user.txt"); // Save changes to file
        }
    }
    void login(string userName, string password)
    {
        User *temp = head;
        while (temp)
        {
            if (temp->userName == userName && temp->password == password)
            {
                currentUser = temp;
                cout << "Login successful!" << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Invalid username or password!" << endl;
    }
    void logout()
    {
        currentUser = nullptr;
        cout << "Logged out successfully!" << endl;
    }

    void suggestFriends()
    {
        if (!currentUser)
        {
            cout << "Please login first!" << endl;
            return;
        }
        unordered_map<string, int> mFri;
        set<string> added_friends(currentUser->friends.begin(), currentUser->friends.end());
        User *temp = head;
        while (temp)
        {
            if (temp->userName != currentUser->userName && !added_friends.count(temp->userName))
            {
                int mutualNumber = 0;
                for (string friends : temp->friends)
                {
                    if (added_friends.count(friends))
                    {
                        mutualNumber++;
                    }
                }
                if (mutualNumber > 0)
                {
                    mFri[temp->userName] = mutualNumber;
                }
            }
            temp = temp->next;
        }
        if (mFri.empty())
        {
            cout << "No mutual friends to be suggested ..!" << endl;
        }
        else
        {
            cout << "--------------------------------------------------" << endl;
            cout << "We have suggeted friend for you: " << endl;
            for (auto fri : mFri)
            {
                cout << fri.first << " (Added with " << fri.second << " person mutule to you)" << endl;
            }
            cout << "--------------------------------------------------" << endl;

            cout << "Enter username to add in your friend list OR write 'exit' to close adding." << endl;
            string newFriendName;
            while (cin >> newFriendName && newFriendName != "exit")
            {
                if (mFri.find(newFriendName) != mFri.end())
                {
                    addFriend(newFriendName);
                }
                else
                {
                    cout << "Username is not in the mutual list" << endl;
                }
            }
            cout << "--------------------------------------------------" << endl;
        }
    }
    string trim(const string &str)
    {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");
        return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
    }
    void getUser(const string &fileName)
    {

        ifstream infile(fileName);
        if (!infile.is_open())
        {
            cout << "Failed to open " << fileName << " file ..!" << endl;
            return;
        }

        string data;
        while (getline(infile, data))
        {

            size_t pos = data.find(',');
            if (pos == string::npos)
                continue;

            string username = data.substr(0, pos);
            username = trim(username);
            data = data.substr(pos + 1);

            pos = data.find(',');
            string password = data.substr(0, pos);
            password = trim(password);
            data = data.substr(pos + 1);

            addUser(username, password, false);

            User *user = head;
            while (user && user->userName != username)
            {
                user = user->next;
            }

            if (user)
            {
                while ((pos = data.find(',')) != string::npos)
                {
                    user->friends.push_back(data.substr(0, pos));
                    data = data.substr(pos + 1);
                }
                if (!data.empty())
                    user->friends.push_back(data);
            }
        }
        infile.close();
    }

    void saveUsersToFile(const string &filename)
    {
        ofstream outfile(filename);
        if (!outfile.is_open())
        {
            cout << "Could not open file for writing: " << filename << endl;
            return;
        }

        User *temp = head;
        while (temp)
        {
            outfile << temp->userName << ',' << temp->password;
            for (const string &friendName : temp->friends)
            {
                outfile << ',' << friendName;
            }
            outfile << endl;
            temp = temp->next;
        }
        outfile.close();
    }
    void displayAllUsers()
    {
        if (!head)
        {
            cout << "No users found!" << endl;
            return;
        }

        User *temp = head;
        int count = 0;

        cout << "All Users:\n";
        while (temp)
        {
            cout << ++count << ". " << temp->userName;

            // Add spacing logic for three lines
            if (count % 3 == 0 || !temp->next)
            {
                cout << endl;
            }
            else
            {
                cout << " | ";
            }

            temp = temp->next;
        }
    }
};

int main()
{
    SocialMedia socialMedia;

    // Load existing users from file
    socialMedia.getUser("user.txt");

    string pass;
    string id;
    int choice;
    int choice2;
    string newFriendName;
    while (true)
    {

        // choose from menue
        cout << "**************************************************" << endl;
        cout << "** 1- Login                                     **" << endl;
        cout << "** 2- Register                                  **" << endl;
        cout << "** 3- Exit                                      **" << endl;
        cout << "**************************************************" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            cout << "Enter your username: ";
            cin >> id;
            cout << "Enter your password: ";
            cin >> pass;
            socialMedia.login(id, pass);
            while (true)
            {

                cout << "**************************************************" << endl;
                cout << "** 1- Add friend                                **" << endl;
                cout << "** 2- Suggest friend                            **" << endl;
                cout << "** 3- Logout                                    **" << endl;
                cout << "**************************************************" << endl;
                cout << "Enter your choice: ";
                cin >> choice2;
                if (choice2 == 1)
                {
                    cout << "**************************************************" << endl;
                    cout << "Registered user list: " << endl;
                    socialMedia.displayAllUsers();
                    cout << "--------------------------------------------------" << endl
                         << endl;
                    cout << "Enter your friend username: ";
                    cin >> id;
                    socialMedia.addFriend(id);
                    cout << "**************************************************" << endl;
                }
                else if (choice2 == 2)
                {
                    cout << "**************************************************" << endl;
                    socialMedia.suggestFriends();
                    cout << "**************************************************" << endl;
                }
                else if (choice2 == 3)
                {
                    socialMedia.logout();
                    cout << "**************************************************" << endl;
                    break;
                }
                else
                {
                    cout << "Invalid choice!" << endl;
                }
            }
        }
        else if (choice == 2)
        {
            cout << "**************************************************" << endl;
            cout << "Set your username: ";
            cin >> id;
            cout << "Set your password: ";
            cin >> pass;
            socialMedia.addUser(id, pass);
            cout << "**************************************************" << endl;
        }
        else if (choice == 3)
        {
            cout << "**************************************************" << endl;
            cout << "Goodbye!" << endl;
            cout << "**************************************************" << endl;
            break;
            ;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }
}