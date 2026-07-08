#pragma once
#include"ProjectHelp.h"
vector<PrimaryIndex> primaryIndexListDoctor;
vector<SecondaryIndex> secondaryIndexListDoctor;
class Doctor
{
public:
    int id;
    char name[50];
    char phone[12];
    char password[20];
    char username[20];
    char specialization[80];

    //display doctor data "except password"
    void displayDoctor()
    {
        cout << "Id            : " << id << endl;
        cout << "Name          : " << name << endl;
        cout << "Phone         : " << phone << endl;
        cout << "specialization: " << specialization << endl;
        cout << "Username      : " << username << endl;
        cout << "-------------------------------------" << endl;
    }

    void setData(bool isupdate = false)
    {
        if(!isupdate)
        {
            while (true)
            {
                cout << "Enter Doctor Id: ";
                cin >> id;
                if (id == 0 || id <= 0)
                {
                    cout << "ID can't be 0 or negative \n";
                    continue;
                }
                if (SearchInPrimaryIndexList(id, primaryIndexListDoctor) != -1)
                    cout << "Error: ID " << id << " already exists. Please enter a unique ID.\n";
                else
                    break;
            }
        }

        cout << "Enter Doctor Name: ";
        solveGetlineProblem();
        cin.getline(name, 50);

        cout << "Enter Doctor Phone: ";
        cin.getline(phone, 12);

        cout << "Enter Doctor Specialization: ";
        cin.getline(specialization, 80);

        cout << "Enter Doctor Username: ";
        cin.getline(username, 20);
       if(!isupdate){
        while (true)
        {
            cout << "Enter Doctor Password: ";
            solveGetlineProblem();
            cin.getline(password, 20);

            bool upper = 0, digit = 0, special = 0;

            for (int i = 0; password[i] != '\0'; i++)
            {
                if (isupper(password[i])) upper = 1;
                else if (isdigit(password[i])) digit = 1;
                else if (ispunct(password[i])) special = 1;
            }

            if (strlen(password) < 8)
            {
                cout << "Too short\n";
                continue;
            }
            if (!upper)
            {
                cout << "Need uppercase\n";
                continue;
            }
            if (!digit)
            {
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
}
    short calcLength()
    {
        short fixedLen = sizeof(int);
        short varLen = strlen(phone) + strlen(name) + strlen(password) + strlen(username) + strlen(specialization);
        short lengthIndicator = 6 * sizeof(short);

        return fixedLen + varLen + lengthIndicator;
    }

    void WriteDoctorInFile(fstream& file)
    {
    // recLen id len name len phone len spec len username len password
        short length, recLength = calcLength();

        file.write((char*)&recLength, sizeof(recLength));
        file.write((char*)&id, sizeof(id));

        length = strlen(name);
        file.write((char*)&length, sizeof(length));
        file.write(name, length);

        length = strlen(phone);
        file.write((char*)&length, sizeof(length));
        file.write(phone, length);

        length = strlen(specialization);
        file.write((char*)&length, sizeof(length));
        file.write(specialization, length);

        length = strlen(username);
        file.write((char*)&length, sizeof(length));
        file.write(username, length);

        length = strlen(password);
        file.write((char*)&length, sizeof(length));
        file.write(password, length);
    }

    void ReadDoctorFromFile(fstream& file)
    {
        // recLen id len name len phone len spec len username len password

        short length, recLength;

        file.read((char*)&recLength, sizeof(recLength));

        file.read((char*)&id, sizeof(id));

        file.read((char*)&length, sizeof(length));
        file.read(name, length); name[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(phone, length); phone[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(specialization, length); specialization[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(username, length); username[length] = '\0';

        file.read((char*)&length, sizeof(length));
        file.read(password, length); password[length] = '\0';
    }
};

void CreateDoctorAccount(fstream& file, Doctor& d, bool update = false)
{
    short neededSize = d.calcLength();
    bool found = false;
    int offset;


    file.seekg(0, ios::beg);
    short header;
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
            d.WriteDoctorInFile(file);

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
        d.WriteDoctorInFile(file);
    }


    PrimaryIndex pi = { d.id, offset };
    AddToPrimaryIndexList(pi, primaryIndexListDoctor);
    AddToSecondaryIndexList(d.name, d.id, secondaryIndexListDoctor);
    if(!update)
        cout << "=== Account created successfully at offset: " << offset << " ===\n";
}

void DisplayAllDoctors(fstream& file)
{
    Doctor d;
    if(primaryIndexListDoctor.size()==0){
        cout<<"No Doctors Yet !\n";
        return;
    }
    cout << "===== Doctors =====\n";
    for (int i = 0; i < primaryIndexListDoctor.size(); i++)
    {
        file.seekg(primaryIndexListDoctor.at(i).offset);
        d.ReadDoctorFromFile(file);
        cout << "=== Doctor " << i + 1 << " ===\n";
        d.displayDoctor();
    }
}

void DeleteDoctortById(fstream& file, int id)
{
    int offset = SearchInPrimaryIndexList(id, primaryIndexListDoctor);
    if (offset == -1)
    {
        cout << "Doctor not found !\n";
        return;
    }

    short oldHeader;
    file.seekg(0, ios::beg);
    file.read((char*)&oldHeader, sizeof(short));

    Doctor d;
    file.seekg(offset);
    d.ReadDoctorFromFile(file);
    d.displayDoctor();
    short recordSize = d.calcLength();

    file.seekp(offset);
    char mark = '*';
    file.write(&mark, 1);
    file.write((char*)&oldHeader, sizeof(short));
    file.write((char*)&recordSize, sizeof(short));

    file.seekp(0, ios::beg);
    short newHeader = (short)offset;
    file.write((char*)&newHeader, sizeof(short));


    DeleteFromPrimaryIndexList(id, primaryIndexListDoctor);
    DeleteFromSecondaryIndexList(d.name, d.id, secondaryIndexListDoctor);

    cout << "\n=== Doctor deleted Successfully and added to Avail List ===\n";
}

void FindDoctorById(fstream& file, int id)
{
    Doctor d;
    int offset = SearchInPrimaryIndexList(id, primaryIndexListDoctor);
    if (offset == -1)
    {
        cout << "Doctor not found !\n";
    }
    else
    {
        file.seekg(offset);
        d.ReadDoctorFromFile(file);
        d.displayDoctor();
    }
}

int DeleteDoctorByName(fstream &file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListDoctor);
    if (ids.size()==0)
        cout << "No Doctor with name: " << name << endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindDoctorById(file, ids[i]);
        }
        cout << "Enter id to delete: ";
        int id;
        cin >> id;
        DeleteDoctortById(file, id);
        return id;
    }
}

void UpdateDoctorInfoById(fstream& file, int id)
{
    file.seekg(0, ios::beg);
    short oldHeader;
    file.read((char*)&oldHeader, sizeof(short));

    Doctor d;
    short newSize, oldSize, padding;
    int offset = SearchInPrimaryIndexList(id, primaryIndexListDoctor);
    if (offset == -1)
        cout << "Doctor not found !\n";
    else
    {
        file.seekg(offset);
        d.ReadDoctorFromFile(file);
        cout << "=== Old data ===\n";
        d.displayDoctor();
        oldSize = d.calcLength();
        char oldName[50];
        strcpy_s(oldName, d.name);

        cout << "Edit Data::\n";
        d.setData(true);
        newSize = d.calcLength();



        if (oldSize >= newSize)
        {
            file.seekp(offset);
            d.WriteDoctorInFile(file);
            padding = oldSize - newSize;
            while (padding--)
            {
                file.put('#');
            }
            DeleteFromSecondaryIndexList(oldName, d.id, secondaryIndexListDoctor);
            AddToSecondaryIndexList(d.name, d.id, secondaryIndexListDoctor);
        }
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


            DeleteFromPrimaryIndexList(id, primaryIndexListDoctor);
            DeleteFromSecondaryIndexList(oldName, d.id, secondaryIndexListDoctor);

            CreateDoctorAccount(file, d, true);
        }
        cout << "=== Doctor Updated successfully ===\n";
    }
}

void updateDoctorByName(fstream& file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListDoctor);
    if (ids.size()==0)
        cout << "No Doctor with name: " << name << endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindDoctorById(file, ids[i]);
        }
        cout << "Enter id to update: ";
        int id; cin >> id;
        UpdateDoctorInfoById(file, id);
    }
}

void FindDoctorByName(fstream& file, const char* name)
{
    vector<int> ids = SearchInSecondaryIndexList(name, secondaryIndexListDoctor);
    if (ids.size()==0)
        cout << "No Doctor with name: " << name << endl;
    else
    {
        for (int i=0;i<ids.size();i++)
        {
            FindDoctorById(file, ids[i]);
        }
    }
}

void displayFirstFiveDoctor(fstream &file){
Doctor p;
if(primaryIndexListDoctor.size()==0){
    cout<<"No Doctor found !\n";
    return;
}
if(primaryIndexListDoctor.size()<5){
        cout<<"=== Only "<<primaryIndexListDoctor.size()<<" found ===\n";
    for(int i=0;i<primaryIndexListDoctor.size();i++){
        file.seekg(primaryIndexListDoctor[i].offset);
        p.ReadDoctorFromFile(file);
        cout<<"Doctor "<<i+1<<": \n";
        p.displayDoctor();
    }
}
else{
     for(int i=0;i<5;i++){
        file.seekg(primaryIndexListDoctor[i].offset);
        p.ReadDoctorFromFile(file);
        cout<<"Doctor "<<i+1<<": \n";
        p.displayDoctor();
    }
}
}
