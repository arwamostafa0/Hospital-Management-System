#pragma once
#include"ProjectHelp.h"
#include"Doctor.h"
#include"Patient.h"
vector<PrimaryIndex>primaryIndexLisAppointment;
char* SearchInSecondaryIndexListForName(const int& id, vector<SecondaryIndex>& SecondaryIndexList)
{
    for (int i = 0; i < SecondaryIndexList.size(); i++)
    {
        if (SecondaryIndexList[i].id == id)
            return SecondaryIndexList[i].name;
    }
}

//appointment
class Appointment{
public:
    int id;
    int patientId;
    int doctorId;
    char date[11];
    char time[6];
    Appointment(){}

    void WriteAppointmentInFile(fstream& file){
    file.write((char*)&id,sizeof(id));
    file.write((char*)&patientId,sizeof(patientId));
    file.write((char*)&doctorId,sizeof(doctorId));
    file.write(date,11);
    file.write(time,6);
    }

    void ReadAppointmentFromFile(fstream& file){
    file.read((char*)&id,sizeof(id));
    file.read((char*)&patientId,sizeof(patientId));
    file.read((char*)&doctorId,sizeof(doctorId));
    file.read(date,11);
    date[11]='\0';
    file.read(time,6);
    time[6]='\0';
    }

    void DisplayData(){
    cout<<"Appointment ID: "<<id<<endl;
    char* name=SearchInSecondaryIndexListForName(patientId,secondaryIndexListPatient);
    cout<<"Patient: "<<name<<endl;
    name=SearchInSecondaryIndexListForName(doctorId,secondaryIndexListDoctor);
    cout<<"Doctor: "<<name<<endl;
    cout<<"Date: "<<date<<endl;
    cout<<"Time: "<<time<<endl;
    cout<<"-------------------------------------"<<endl;
    }
};

//book appointment by patient
void BookAppointmentForPatient(fstream &fileOfAppointment,fstream &fileOfDoctor,int &patientid){
    Appointment a;
    while (true)
            {
                cout << "Enter Appointment Id: ";
                cin >> a.id;
                if ( a.id == 0 ||  a.id <= 0)
                {
                    cout << "ID can't be 0 or negative \n";
                    continue;
                }
                if (SearchInPrimaryIndexList( a.id, primaryIndexLisAppointment) != -1)
                    cout << "Error: ID " <<  a.id << " already exists. Please enter a unique ID.\n";
                else
                    break;
            }
    a.patientId=patientid;
    while(true){
    cout<<"Doctor ID: ";
    cin>>a.doctorId;
    int offset=SearchInPrimaryIndexList(a.doctorId,primaryIndexListDoctor);
    if(offset==-1){
        cout<<"Doctor not found please try again !\n";
        continue;
    }
    else{
        Doctor d;
        fileOfDoctor.seekg(offset);
        d.ReadDoctorFromFile(fileOfDoctor);
        cout<<"-------------------------------------"<<endl;
        d.displayDoctor();
        break;
     }
    }
    cout<<"Date (dd/mm/yyyy): ";
    solveGetlineProblem();
    cin.getline(a.date,11);
    cout<<"Time (hh:mm): ";
    cin.getline(a.time,6);
    // Reclaming Spaces
   short header;
    bool found = false;
    int offset;

    fileOfAppointment.clear();
    fileOfAppointment.seekg(0, ios::beg);
    fileOfAppointment.read((char*)&header, sizeof(short));

    if (header != -1) {
        found = true;
        offset = header;
        fileOfAppointment.seekg(offset + 1);
        short next;
        fileOfAppointment.read((char*)&next, sizeof(short));
        fileOfAppointment.seekp(0, ios::beg);
        fileOfAppointment.write((char*)&next, sizeof(short));
        fileOfAppointment.seekp(offset);
        a.WriteAppointmentInFile(fileOfAppointment);
    }

    if (!found) {
        fileOfAppointment.clear();
        fileOfAppointment.seekp(0, ios::end);
        offset = fileOfAppointment.tellp();
        a.WriteAppointmentInFile(fileOfAppointment);
    }

    PrimaryIndex pi = {a.id, offset};
    AddToPrimaryIndexList(pi, primaryIndexLisAppointment);
    cout << "=== Appointment Booked Successfully at offset: " << offset << " ===\n";
}

//cancel appointment by patient
void CancelAppointmentForPatient(fstream &fileOfAppointment,int &patientid){
    Appointment a;
    cout<<"Appointment ID: ";
    cin>>a.id;
    a.patientId=patientid;
    int offset=SearchInPrimaryIndexList(a.id,primaryIndexLisAppointment);
    if(offset==-1){
    cout<<"Appointment not found !\n";
    }
    else{
    fileOfAppointment.seekg(offset);
    a.ReadAppointmentFromFile(fileOfAppointment);
    if(a.patientId!=patientid){
        cout<<"Not Your Appointment !\n";
        return;
    }
    a.DisplayData();
    DeleteFromPrimaryIndexList(a.id,primaryIndexLisAppointment);

  short oldHeader;
    fileOfAppointment.clear();
    fileOfAppointment.seekg(0, ios::beg);
    fileOfAppointment.read((char*)&oldHeader, sizeof(short));

    short recordSize = sizeof(Appointment);

    fileOfAppointment.seekp(offset);
    char mark = '*';
    fileOfAppointment.write(&mark, 1);
    fileOfAppointment.write((char*)&oldHeader, sizeof(short));
    fileOfAppointment.write((char*)&recordSize, sizeof(short));

    fileOfAppointment.seekp(0, ios::beg);
    short newHeader = (short)offset;
    fileOfAppointment.write((char*)&newHeader, sizeof(short));

    DeleteFromPrimaryIndexList(a.id, primaryIndexLisAppointment);
    fileOfAppointment.flush();

    cout<<"=== Appointment Canceled Successfully ===\n";
 }
}

//display appointment for patient
void DisplayMyAppointmentForPatient(fstream &fileOfAppointment,int &patientid){
Appointment a;
int number=0;
    if(primaryIndexLisAppointment.size()==0){
        cout<<"No Appointments available !\n";
        return;
    }
for(int i=0;i<primaryIndexLisAppointment.size();i++){
    fileOfAppointment.seekg(primaryIndexLisAppointment[i].offset);
    a.ReadAppointmentFromFile(fileOfAppointment);
    if(a.patientId!=patientid)
        continue;
    a.DisplayData();
    number++;
}
if(number==0){
    cout<<"No Appointments available !\n";
}
}

//display appointment for doctor
void DisplayMyAppointmentForDoctor(fstream &fileOfAppointment,int &doctorid){
Appointment a;
int number=0;
    if(primaryIndexLisAppointment.size()==0){
        cout<<"No Appointments available !\n";
        return;
    }
for(int i=0;i<primaryIndexLisAppointment.size();i++){
    fileOfAppointment.seekg(primaryIndexLisAppointment[i].offset);
    a.ReadAppointmentFromFile(fileOfAppointment);
    if(a.doctorId!=doctorid)
        continue;
    a.DisplayData();
    number++;
}
if(number==0){
    cout<<"No Appointments available !\n";
}
}

//display all appointments
void DisplayAllAppointment(fstream &fileOfAppointment){
Appointment a;
    if(primaryIndexLisAppointment.size()==0){
        cout<<"No Appointments available !\n";
        return;
    }
for(int i=0;i<primaryIndexLisAppointment.size();i++){
    fileOfAppointment.seekg(primaryIndexLisAppointment[i].offset);
    a.ReadAppointmentFromFile(fileOfAppointment);
    a.DisplayData();
}
}

//update date and time by doctor
void UpdateAppointmentByDoctor(fstream &fileOfAppointment,int &doctorid)
{
     Appointment a;
    cout<<"Appointment ID: ";
    cin>>a.id;
    a.doctorId=doctorid;
    int offset=SearchInPrimaryIndexList(a.id,primaryIndexLisAppointment);
    if(offset==-1){
    cout<<"Appointment not found !\n";
    }
    else{
    fileOfAppointment.seekg(offset);
    a.ReadAppointmentFromFile(fileOfAppointment);
    if(a.doctorId!=doctorid){
        cout<<"Not Your Appointment !\n";
        return;
    }
    cout<<"=== Old date & Time ===\n";
    a.DisplayData();
    cout<<"=== New date & Time ===\n"<<endl;
    cout<<"Date (dd/mm/yyyy): ";
    solveGetlineProblem();
    cin.getline(a.date,11);
    cout<<"Time (hh:mm): ";
    cin.getline(a.time,6);
    fileOfAppointment.seekp(offset);
    a.WriteAppointmentInFile(fileOfAppointment);
    cout<<"=== Appointment Updated Successfully ===\n";
 }
}
void CancelAppointmentForPatientSimple(fstream &fileOfAppointment, int &patientid){
    Appointment a;
    int x = primaryIndexLisAppointment.size();
    for(int i = x - 1; i >= 0; i--){
        int currentOffset = primaryIndexLisAppointment[i].offset;
        int currentId = primaryIndexLisAppointment[i].id;

        fileOfAppointment.seekg(currentOffset);
        a.ReadAppointmentFromFile(fileOfAppointment);

        if(a.patientId == patientid){
            DeleteFromPrimaryIndexList(currentId, primaryIndexLisAppointment);
            short oldHeader;
            fileOfAppointment.clear();
            fileOfAppointment.seekg(0, ios::beg);
            fileOfAppointment.read((char*)&oldHeader, sizeof(short));

            fileOfAppointment.seekp(currentOffset);
            char mark = '*';
            fileOfAppointment.write(&mark, 1);
            fileOfAppointment.write((char*)&oldHeader, sizeof(short));

            fileOfAppointment.seekp(0, ios::beg);
            short newHeader = (short)currentOffset;
            fileOfAppointment.write((char*)&newHeader, sizeof(short));
            fileOfAppointment.flush();
        }
    }
}

void CancelAppointmentForDoctorSimple(fstream &fileOfAppointment, int &Doctorid){
    Appointment a;
    int x = primaryIndexLisAppointment.size();
    for(int i = x - 1; i >= 0; i--){
        int currentOffset = primaryIndexLisAppointment[i].offset;
        int currentId = primaryIndexLisAppointment[i].id;
        fileOfAppointment.seekg(currentOffset);
        a.ReadAppointmentFromFile(fileOfAppointment);
        if(a.doctorId == Doctorid){
            DeleteFromPrimaryIndexList(currentId, primaryIndexLisAppointment);
            short oldHeader;
            fileOfAppointment.clear();
            fileOfAppointment.seekg(0, ios::beg);
            fileOfAppointment.read((char*)&oldHeader, sizeof(short));

            fileOfAppointment.seekp(currentOffset);
            char mark = '*';
            fileOfAppointment.write(&mark, 1);
            fileOfAppointment.write((char*)&oldHeader, sizeof(short));

            fileOfAppointment.seekp(0, ios::beg);
            short newHeader = (short)currentOffset;
            fileOfAppointment.write((char*)&newHeader, sizeof(short));
            fileOfAppointment.flush();
        }
    }
}
