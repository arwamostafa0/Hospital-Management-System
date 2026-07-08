#pragma once
#include"ProjectHelp.h"

using namespace std;
vector<PrimaryIndex> primaryIndexListPatient;
vector<SecondaryIndex> secondaryIndexListPatient;
class Patient
{
public:
    int id;
    int age;
    char name[50];
    char password[20];
    char username[20];
    char address[80];
    char feeling[200];

    //display information "except password"
    void displayPatient()
    {
        cout << "Id          : " << id << endl;
        cout << "Name        : " << name << endl;
        cout << "Age         : " << age << endl;
        cout << "Address     : " << address << endl;
        cout << "Username    : " << username << endl;
        cout << "Your Feeling: " << feeling << endl;
        cout << "-------------------------------------" << endl;
    }

    void setData(bool isupdate = false)
    {
        //make sure id not 0 or negative
        if (!isupdate)
        {
            while (true)
            {
                cout << "Enter Patient Id: ";
                cin >> id;
                if (id == 0 || id <= 0)
                {
                    cout << "ID can't be 0 or negative \n";
                    continue;
                }
                if (SearchInPrimaryIndexList(id, primaryIndexListPatient) != -1)
                    cout << "Error: ID " << id << " already exists. Please enter a unique ID.\n";
                else
                    break;
            }
        }

        cout << "Enter Patient Name: ";
        solveGetlineProblem();
        cin.getline(name, 50);

        cout << "Enter Patient Age: ";
        cin >> age;

        cout << "Enter Patient Address: ";
        solveGetlineProblem();
        cin.getline(address, 80);

        cout << "Enter Patient Username: ";
        solveGetlineProblem();
        cin.getline(username, 20);
        //make sure password contain at least 1 special character, 1 capital letter, 1 number and be larger than 8 characters
        if(!isupdate){
        while (true)
        {
            cout << "Enter Patient Password: ";
            solveGetlineProblem();
            cin.getline(password, 20);

            bool upper = 0, digit = 0, special = 0;
            for (int i = 0; password[i] != '\0'; i++)
            {
                if (isupper(password[i])) upper = 1;
                else if (isdigit(password[i])) digit = 1;
                else if (ispunct(password[i])) special = 1;
            }
            //not all cases will be covered it will be so long "16 case"
            if (strlen(password) < 8)
            {
                cout << "Password is too short\n";
                continue;
            }
            if (!upper)
            {
                cout << "Need uppercase\n";
                continue;
            }
            if (!digit) {
                cout << "Need digit\n";
                continue;
            }
            if (!special)
            {
                cout << "Need special char\n";
                continue;
            }
            break;
        }
    }
        //feeling
        cout << "Describe what you are feeling: ";
        solveGetlineProblem();
        cin.getline(feeling, 200);
    }

    //calculating length of record
    // recLen id len name age len adress len username len password len feeling
    short calcLength()
    {
        short fixedLen = 2 * sizeof(int);
        short varLen = strlen(name) + strlen(password) + strlen(username) + strlen(address) + strlen(feeling);
        short lengthIndicator = 6 * sizeof(short);
        return fixedLen + varLen + lengthIndicator;
    }

    //writing in patient file
    void WritePatientInFile(fstream& file)
    {
        // recLen id len name age len adress len username len password len feeling
        short length, recLength = calcLength();

        file.write((char*)&recLength, sizeof(recLength));

        file.write((char*)&id, sizeof(id));

        length = strlen(name);
        file.write((char*)&length, sizeof(length));
        file.write(name, length);

        file.write((char*)&age, sizeof(age));

        length = strlen(address);
        file.write((char*)&length, sizeof(length));
        file.write(address, length);

        length = strlen(username);
        file.write((char*)&length, sizeof(length));
        file.write(username, length);

        length = strlen(password);
        file.write((char*)&length, sizeof(length));
        file.write(password, length);

        length = strlen(feeling);
        file.write((char*)&length, sizeof(length));
        file.write(feeling, length);
    }

    //reading from patient file
    void ReadPatientFromFile(fstream& file)
    {
        // recLen id len name age len adress len username len password len feeling
        short length, recLength;

        file.read((char*)&recLength, sizeof(recLength));

        file.read((char*)&id, sizeof(id));

        file.read((char*)&length, sizeof(length));
        file.read(name, length); name[length] = '\0';

        file.read((char*)&age, sizeof(age));

        file.read((char*)&length, sizeof(length));
        file.read(address, length); address[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(username, length); username[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(password, length); password[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(feeling, length); feeling[length] = '\0';
    }
};
void DisplayAllPatients(fstream& file)
{
    Patient p;
    if(primaryIndexListPatient.size()==0){
        cout<<"No Patient Yet !\n";
        return;
    }
    cout << "===== Patients =====\n";
    for (int i = 0; i < primaryIndexListPatient.size(); i++)
    {
        file.seekg(primaryIndexListPatient.at(i).offset);
        p.ReadPatientFromFile(file);
        cout << "=== Patient " << i + 1 << " ===\n";
        p.displayPatient();
    }
}
void CreatePatientAccount(fstream& file, Patient p, bool update = false)
{
    short neededSize = p.calcLength();
    short header;
    bool found = false;
    int offset;

    file.seekg(0, ios::beg);
    file.read((char*)&header, sizeof(short));

    int curr = header;
    int prev = -1;

    while (curr != -1)
    {
        file.seekg(curr);
        char mark;
        short next, availableSize;

        file.read(&mark, 1);

        file.read((char*)&next, sizeof(short));
        file.read((char*)&availableSize, sizeof(short));

        if (availableSize >= neededSize)
        {
            found = true;
            offset = curr;


            if (prev == -1)
            {
                file.seekp(0, ios::beg);
                file.write((char*)&next, sizeof(short));
            }
            else
            {
                file.seekp(prev + 1);
                file.write((char*)&next, sizeof(short));
            }


            file.seekp(offset);
            p.WritePatientInFile(file);


            short padding = availableSize - neededSize;
            file.seekp(offset + neededSize);
            while (padding--) file.put('#');
            break;
        }
        prev = curr;
        curr = next;
    }


    if (!found)
    {
        file.seekp(0, ios::end);
        offset = file.tellp();
        p.WritePatientInFile(file);
    }


    PrimaryIndex pi = { p.id, offset };
    AddToPrimaryIndexList(pi, primaryIndexListPatient);
    AddToSecondaryIndexList(p.name, p.id, secondaryIndexListPatient);
    if(!update)
        cout << "=== Account created successfully at offset: " << offset << " ===\n";

}

void DeletePatientById(fstream& file, int id)
{
    int offset = SearchInPrimaryIndexList(id, primaryIndexListPatient);
    if (offset == -1)
    {
        cout << "Patient not found !\n";
        return;
    }

    short oldHeader;
    file.seekg(0, ios::beg);
    file.read((char*)&oldHeader, sizeof(short));

    Patient p;
    file.seekg(offset);
    p.ReadPatientFromFile(file);
    p.displayPatient();
    short recordSize = p.calcLength();

    file.seekp(offset);
    char mark = '*';
    file.write(&mark, 1);
    file.write((char*)&oldHeader, sizeof(short));
    file.write((char*)&recordSize, sizeof(short));

    file.seekp(0, ios::beg);
    short newHeader = (short)offset;
    file.write((char*)&newHeader, sizeof(short));


    DeleteFromPrimaryIndexList(id, primaryIndexListPatient);
    DeleteFromSecondaryIndexList(p.name, p.id, secondaryIndexListPatient);

    cout << "\n=== Patient deleted Successfully and added to Avail List ===\n";
}
void FindPatientById(fstream& file, int id)
{
    Patient p;
    int offset = SearchInPrimaryIndexList(id, primaryIndexListPatient);
    if (offset == -1)
        cout << "Patient not found!\n";
    else
    {
        file.seekg(offset);
        p.ReadPatientFromFile(file);
        p.displayPatient();
    }

}

int DeletePatientByName(fstream&file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListPatient);
    if (ids.size()==0)
        cout << "No Patient with name: " << name<< endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindPatientById(file, ids[i]);
        }
        cout << "Enter id to delete: ";
        int id;
        cin >> id;
        DeletePatientById(file, id);
        return id;
    }
}
void UpdatePatientInfoById(fstream& file, int id)
{
    file.seekg(0, ios::beg);
    short oldHeader;
    file.read((char*)&oldHeader, sizeof(short));

    Patient p;
    short newSize, oldSize, padding;
    int offset = SearchInPrimaryIndexList(id, primaryIndexListPatient);
    if (offset == -1)
        cout << "Patient not found !\n";
    else
    {
        file.seekg(offset);
        p.ReadPatientFromFile(file);
        cout << "=== Old data ===\n";
        p.displayPatient();
        oldSize = p.calcLength();
        char oldName[50];
        strcpy_s(oldName, p.name);


        cout << "Edit Data::\n";
        p.setData(true);
        newSize = p.calcLength();

        if (oldSize >= newSize)
        {
            file.seekp(offset);
            p.WritePatientInFile(file);
            padding = oldSize - newSize;
            while (padding--)
            {
                file.put('#');
            }
            DeleteFromSecondaryIndexList(oldName, p.id, secondaryIndexListPatient);
            AddToSecondaryIndexList(p.name, p.id, secondaryIndexListPatient);
        }
        //if the size is bigger than the old data size
        else
        {
            file.seekp(offset);
            char mark = '*';
            file.write(&mark, 1);
            file.write((char*)&oldHeader, sizeof(short));
            file.write((char*)&oldSize, sizeof(short));

            file.seekp(0, ios::beg);
            short newHeader = (short)offset;
            file.write((char*)&newHeader, sizeof(short));

            DeleteFromPrimaryIndexList(id, primaryIndexListPatient);
            DeleteFromSecondaryIndexList(oldName, p.id, secondaryIndexListPatient);

            CreatePatientAccount(file, p, true);
        }
        cout << "=== Patient Updated successfully ===\n";
    }
}
void updatePatientByName(fstream& file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListPatient);
    if (ids.size()==0)
        cout << "No Patient with name: " << name << endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindPatientById(file, ids[i]);
        }
        cout << "Enter id to update: ";
        int id; cin >> id;
        UpdatePatientInfoById(file, id);
    }
}

void FindPatientByName(fstream& file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListPatient);
    if (ids.size()==0)
        cout << "No Patient with name: " << name << endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindPatientById(file, ids[i]);
        }
    }
}

void displayFirstFivePatient(fstream &file){
Patient p;
if(primaryIndexListPatient.size()==0){
    cout<<"No Patient found !\n";
    return;
}
if(primaryIndexListPatient.size()<5){
        cout<<"=== Only "<<primaryIndexListPatient.size()<<" found ===\n";
    for(int i=0;i<primaryIndexListPatient.size();i++){
        file.seekg(primaryIndexListPatient[i].offset);
        p.ReadPatientFromFile(file);
        cout<<"Patient "<<i+1<<": \n";
        p.displayPatient();
    }
}
else{
     for(int i=0;i<5;i++){
        file.seekg(primaryIndexListPatient[i].offset);
        p.ReadPatientFromFile(file);
        cout<<"Patient "<<i+1<<": \n";
        p.displayPatient();
    }
}
}
