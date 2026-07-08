#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include<string>
#include <algorithm>
#include <cstring>
#include "Patient.h"
#include "Doctor.h"
#include"Appointment.h"
using namespace std;
void SkipPadding(fstream& file)
{
    while (file.peek() == '#')
        file.get();
}

class Administrator
{
private:
    string password;
    string username;
public:
    Administrator()
    {
        password = "user";
        username = "user";
    }
    string getPassword()
    {
        return password;
    }
    string getUsername()
    {
        return username;
    }
};
bool loginAsAdministrator()
{
    Administrator a;
    string user, pass;

    cout << "Enter Username:\n";
    solveGetlineProblem();
    getline(cin, user);

    cout << "Enter Password:\n";
    getline(cin, pass);

    if (a.getUsername() == user && a.getPassword() == pass)
    {
        return true;
    }
    else if (a.getUsername() == user && a.getPassword() != pass)
    {
        cout << "Wrong password!\n";
    }
    else if (a.getUsername() != user && a.getPassword() == pass)
    {
        cout << "Wrong username!\n";
    }
    else
    {
        cout << "Wrong username and password!\n";
    }
    return false;
}
bool loginAsPatient(fstream& file, Patient& current, int& pos)
{
    char uname[20], pass[20];

    cout << "Enter Username: ";
    solveGetlineProblem();
    cin.getline(uname, 20);

    cout << "Enter Password: ";
    solveGetlineProblem();
    cin.getline(pass, 20);

    // Skip the 2-byte header
    file.seekg(sizeof(short), ios::beg);

    while (true)
    {
        int currentPos = file.tellg();
        char firstChar;

        if (!file.read(&firstChar, 1)) break;

        // Handle Deleted Records (Avail List)
        if (firstChar == '*')
        {
            short next, size;
            file.read((char*)&next, sizeof(short));
            file.read((char*)&size, sizeof(short));
            // Jump to the start of the next record
            file.seekg(size - (sizeof(char) + 2 * sizeof(short)), ios::cur);
            continue;
        }

        // Move back 1 byte to read the full record
        file.seekg(-1, ios::cur);

        Patient temp;
        // Ensure your classes use a consistent naming convention for reading
        SkipPadding(file);
        temp.ReadPatientFromFile(file);

        if (strcmp(temp.username, uname) == 0)
        {
            if (strcmp(temp.password, pass) == 0)
            {
                current = temp;
                pos = currentPos;
                return true;
            }
            else
            {
                cout << "Invalid password!\n";
                return false;
            }
        }
    }
    cout << "Patient not found !\n";
    return false;
}
bool loginAsDoctor(fstream& file, Doctor& current, int& pos)
{
    char uname[20], pass[20];

    cout << "Enter Username: ";
    solveGetlineProblem();
    cin.getline(uname, 20);

    cout << "Enter Password: ";
    cin.getline(pass, 20);

    file.seekg(sizeof(short), ios::beg); // skip header فقط

    while (true) {
        int currentPos = file.tellg();
        char firstChar;

        if (!file.read(&firstChar, 1)) break;

        // Handle Deleted Records (Avail List)
        if (firstChar == '*')
        {
            short next, size;
            file.read((char*)&next, sizeof(short));
            file.read((char*)&size, sizeof(short));
            // Jump to the start of the next record
            file.seekg(size - (sizeof(char) + 2 * sizeof(short)), ios::cur);
            continue;
        }

        // Move back 1 byte to read the full record
        file.seekg(-1, ios::cur);

        Doctor temp;
        // Ensure your classes use a consistent naming convention for reading
        SkipPadding(file);
        temp.ReadDoctorFromFile(file);

        if (strcmp(temp.username, uname) == 0)
        {
            if (strcmp(temp.password, pass) == 0)
            {
                current = temp;
                pos = currentPos;
                return true;
            }
            else
            {
                cout << "Invalid password!\n";
                return false;
            }
        }
    }

    cout << "Doctor not found!\n";
    return false;
}



void InitializeFile(string filename)
{
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file)
    {
        ofstream out(filename, ios::binary);
        short header = -1;
        out.write((char*)&header, sizeof(short));
        out.close();
    }
}
void InitializeIndexFile(string filename)
{
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file)
    {
        ofstream out(filename, ios::binary);
        out.close();
    }
}


void mainMenu()
{
    system("cls");
    cout << "====================================================" << endl;
    cout << "             HOSPITAL MANAGEMENT SYSTEM             " << endl;
    cout << "====================================================" << endl;
    cout << " 1. Administrator" << endl;
    cout << " 2. Patient" << endl;
    cout << " 3. Doctor" << endl;
    cout << " 0. Exit" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Selection: ";
}
void administrator(fstream &fDoctor,fstream &fPatient,fstream &fAppointment)
{
    if (!loginAsAdministrator())
    {
        cout << "\nInvalid Admin Credentials!" << endl;
        system("pause");
        return;
    }
    int id;
    char name[20];
    int choice2, choice3;
    do
    {
        system("cls");
        cout << "---------------------- Admin Menu ----------------------" << endl;
        cout << " 1- View All Patients               7- Find Patient" << endl;
        cout << " 2- View All Doctors                8- Find Doctor" << endl;
        cout << " 3- View All Appointments           9- Update Patient" << endl;
        cout << " 4- Add Doctor                      10- Update Doctor" << endl;
        cout << " 5- Display First 5 Patients        11- Delete Patient " << endl;
        cout << " 6- Display First 5 Doctors         12- Delete Doctor  " << endl;
        cout<<"                        0- Logout\n";
        cout << "---------------------------------------------------------" << endl;
        cout << "Action: ";
        cin >> choice2;

        switch (choice2)
        {

        case 1:
            system("cls");
            DisplayAllPatients(fPatient);
            break;

        case 2:
            system("cls");
            fDoctor.clear();
            DisplayAllDoctors(fDoctor);
            break;

        case 3:
            system("cls");
            DisplayAllAppointment(fAppointment);
            break;

        case 4:
            system("cls");
            Doctor d;
            d.setData();
            CreateDoctorAccount(fDoctor, d);
            break;

        case 5:
            system("cls");
            displayFirstFivePatient(fPatient);
            break;

        case 6:
            system("cls");
            displayFirstFiveDoctor(fDoctor);
            break;

        case 7:
           {
            do
            {
                system("cls");
                cout << "------- Find Patient -------" << endl;
                cout << " 1- Find by name" << endl;
                cout << " 2- Find by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;
                cin.ignore();

                if (choice3 == 1)
                {
                    cout << "Enter Patient name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    FindPatientByName(fPatient, name);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Patient ID: ";
                    cin >> id;
                    FindPatientById(fPatient, id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        } break;

        case 8:
            {
            do
            {
                system("cls");
                cout << "------- Find Doctor -------" << endl;
                cout << " 1- Find by name" << endl;
                cout << " 2- Find by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;
                cin.ignore();

                if (choice3 == 1)
                {
                    cout << "Enter Doctor name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    FindDoctorByName(fDoctor, name);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Doctor ID: ";
                    cin >> id;
                    FindDoctorById(fDoctor, id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        } break;

        case 9:
            {
            do
            {
                system("cls");
                cout << "------- Update Patient -------" << endl;
                cout << " 1- Update by name" << endl;
                cout << " 2- Update by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;
                cin.ignore();

                if (choice3 == 1)
                {
                    cout << "Enter Patient name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    updatePatientByName(fPatient, name);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Patient ID: ";
                    cin >> id;
                    UpdatePatientInfoById(fPatient, id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        }
            break;

        case 10:
            {
            do
            {
                system("cls");
                cout << "------- Update Doctor -------" << endl;
                cout << " 1- Update by name" << endl;
                cout << " 2- Update by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;
                cin.ignore();

                if (choice3 == 1)
                {
                    cout << "Enter Doctor name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    updateDoctorByName(fDoctor, name);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Doctor ID: ";
                    cin >> id;
                    UpdateDoctorInfoById(fDoctor, id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        }

            break;

        case 11:
                    {
            do
            {
                system("cls");
                cout << "------- Delete Patient -------" << endl;
                cout << " 1- Delete by name" << endl;
                cout << " 2- Delete by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;

                if (choice3 == 1)
                {
                    cout << "Enter Patient name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    int x=DeletePatientByName(fPatient, name);
                    CancelAppointmentForPatientSimple(fAppointment,x);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Patient ID: ";
                    cin >> id;
                    DeletePatientById(fPatient, id);
                    CancelAppointmentForPatientSimple(fAppointment,id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        } break;
        case 12:
            {
            do
            {
                system("cls");
                cout << "------- Delete Doctor -------" << endl;
                cout << " 1- Delete by name" << endl;
                cout << " 2- Delete by id" << endl;
                cout << " 0- Back" << endl;
                cout << "Selection: ";
                cin >> choice3;
                cin.ignore();

                if (choice3 == 1)
                {
                    cout << "Enter Doctor name: ";
                    solveGetlineProblem();
                    cin.getline(name, 20);
                    int x=DeleteDoctorByName(fDoctor, name);
                    CancelAppointmentForDoctorSimple(fAppointment,x);
                    system("pause");
                }
                else if (choice3 == 2)
                {
                    cout << "Enter Doctor ID: ";
                    cin >> id;
                    DeleteDoctortById(fDoctor, id);
                    CancelAppointmentForDoctorSimple(fAppointment,id);
                    system("pause");
                }
            } while (choice3 != 0);
            break;
        } break;

}
        if (choice2 != 0 && choice2 != 3)
        {
            cout << endl;
            system("pause");
        }
        fDoctor.clear();
        fPatient.clear();
        fAppointment.clear();

    } while (choice2 != 0);
}
void patient(fstream& fDoctor, fstream& fPatient, fstream& fAppointment)
{
    int choice2, choice3;
    int id;
    char name[20];
    do
    {
        system("cls");
        cout << "---------------- Patient Portal ----------------" << endl;
        cout << " 1- Login" << endl;
        cout << " 2- Create Account" << endl;
        cout << " 0- Back" << endl;
        cout << "Selection: ";

        cin >> choice2;

        if (choice2 == 2)
        {
            Patient p;
            p.setData();
            CreatePatientAccount(fPatient, p);
            system("pause");
            continue;
        }
        else if (choice2 == 0)
        {
            break;
        }

        Patient current;
        int pos;

        if (!loginAsPatient(fPatient, current, pos))
        {
            system("pause");
            continue;
        }

        do
        {
            system("cls");
            cout << "------- Patient: " << current.name << " -------" << endl;
            cout << " 1- View My Profile" << endl;
            cout << " 2- Update My Profile" << endl;
            cout << " 3- Delete My Account" << endl;
            cout << " 4- Find Doctor" << endl;
            cout << " 5- Book Appointment" << endl;
            cout << " 6- Cancel Appointment" << endl;
            cout << " 7- View My Appointments" << endl;
            cout << " 0- Logout" << endl;
            cout << "Action: ";

            cin >> choice2;


            switch (choice2)
            {
            case 1:
                current.displayPatient();
                break;
            case 2:
                UpdatePatientInfoById(fPatient, current.id);
                break;
            case 3:
                DeletePatientById(fPatient, current.id);
                CancelAppointmentForPatientSimple(fAppointment,current.id);
                break;
            case 4:
                do
                {
                    system("cls");
                    cout << "------- Find Doctor -------" << endl;
                    cout << " 1- Find by name" << endl;
                    cout << " 2- Find by id" << endl;
                    cout << " 0- Back" << endl;
                    cout << "Selection: ";
                    cin >> choice3;
                    cin.ignore();

                    if (choice3 == 1)
                    {
                        cout << "Enter Doctor name: ";
                        solveGetlineProblem();
                        cin.getline(name, 20);
                        FindDoctorByName(fDoctor, name);
                        system("pause");
                    }
                    else if (choice3 == 2)
                    {
                        cout << "Enter Doctor ID: ";
                        cin >> id;
                        FindDoctorById(fDoctor, id);
                        system("pause");
                    }
                } while (choice3 != 0);
            case 5:
                BookAppointmentForPatient(fAppointment, fDoctor, current.id);
                break;
            case 6:
                CancelAppointmentForPatient(fAppointment, current.id);
                break;
            case 7:
                DisplayMyAppointmentForPatient(fAppointment, current.id);
                break;
            }

            if (choice2 != 0)
            {
                cout << endl;
                system("pause");
            }
            fDoctor.clear();
            fPatient.clear();
            fAppointment.clear();

        } while (choice2 != 0);

    } while (choice2 != 0);
}
void doctor(fstream& fDoctor, fstream& fAppointment)
{
    Doctor current;
    int pos;

    int choice2;

    if (!loginAsDoctor(fDoctor, current, pos))
    {
        system("pause");
        return;
    }

    do
    {
        system("cls");
        cout << "------- Doctor: " << current.name << " -------" << endl;
        cout << " 1- View My Profile" << endl;
        cout << " 2- Update My Profile" << endl;
        cout << " 3- View My Appointments" << endl;
        cout << " 4- Update My Appointment" << endl;
        cout << " 0- Logout" << endl;
        cout << "Action: ";

        cin >> choice2;
        cin.ignore();

        switch (choice2)
        {
        case 1:
            current.displayDoctor();
            break;
        case 2:
            UpdateDoctorInfoById(fDoctor, current.id);
            break;
        case 3:
            DisplayMyAppointmentForDoctor(fAppointment, current.id);
            break;
        case 4:
            UpdateAppointmentByDoctor(fAppointment, current.id);
            break;
        }

        if (choice2 != 0)
        {
            cout << endl;
            system("pause");
        }
        fDoctor.clear();
        fAppointment.clear();
    } while (choice2 != 0);
}
void startProgram()
{
    InitializeFile("Patient.txt");
    InitializeFile("Doctor.txt");
    InitializeFile("Appointment.txt");

    fstream fPatient;
    fstream fDoctor;
    fstream fAppointment;

    fPatient.open("Patient.txt", ios::in | ios::out | ios::binary);
    fDoctor.open("Doctor.txt", ios::in | ios::out | ios::binary);
    fAppointment.open("Appointment.txt", ios::in | ios::out | ios::binary);

    InitializeIndexFile("PrimaryIndexPatient.txt");
    InitializeIndexFile("PrimaryIndexDoctor.txt");
    InitializeIndexFile("PrimaryIndexAppointment.txt");

    InitializeIndexFile("SecondaryIndexPatient.txt");
    InitializeIndexFile("SecondaryIndexDoctor.txt");


    ReadPrimaryIndexFile("PrimaryIndexPatient.txt", primaryIndexListPatient);
    ReadPrimaryIndexFile("PrimaryIndexDoctor.txt", primaryIndexListDoctor);
    ReadPrimaryIndexFile("PrimaryIndexAppointment.txt", primaryIndexLisAppointment);

    ReadSecondaryIndexFile("SecondaryIndexPatient.txt", secondaryIndexListPatient);
    ReadSecondaryIndexFile("SecondaryIndexDoctor.txt", secondaryIndexListDoctor);

    int choice;

    do
    {
        mainMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            administrator(fDoctor, fPatient, fAppointment);
            break;
        case 2:
            patient(fDoctor, fPatient, fAppointment);
            break;
        case 3:
            doctor(fDoctor, fAppointment);
            break;
        case 0:
            cout << "\nExiting Program..." << endl;
            break;
        default:
            cout << "\nInvalid Selection!" << endl;
            system("pause");
            system("cls");
        }
        fDoctor.clear();
        fPatient.clear();
        fAppointment.clear();
    } while (choice != 0);



    WritePrimaryIndexFile("PrimaryIndexPatient.txt", primaryIndexListPatient);
    WritePrimaryIndexFile("PrimaryIndexDoctor.txt", primaryIndexListDoctor);
    WritePrimaryIndexFile("PrimaryIndexAppointment.txt", primaryIndexLisAppointment);

    WriteSecondaryIndexFile("SecondaryIndexPatient.txt", secondaryIndexListPatient);
    WriteSecondaryIndexFile("SecondaryIndexDoctor.txt", secondaryIndexListDoctor);
}
