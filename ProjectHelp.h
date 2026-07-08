#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include<string>
#include <algorithm>
#include <cstring>
using namespace std;

void solveGetlineProblem()
{
    if (cin.peek() == '\n')
        cin.ignore(10, '\n');
}
struct PrimaryIndex
{
    int id, offset;
    //sorting of index operator overloading
    bool operator < (const PrimaryIndex& pi)
    {
        return id < pi.id;
    }
};
void WritePrimaryIndexFile(string filename, vector<PrimaryIndex>& primaryIndexList)
{
    ofstream outFile(filename, ios::binary | ios::out | ios::trunc);
    for (int i = 0; i < primaryIndexList.size(); i++)
    {
        PrimaryIndex pi = primaryIndexList.at(i);
        outFile.write((char*)&pi, sizeof(PrimaryIndex));
    }
    cout << filename << " Primary Index saved successfully.\n";
    outFile.close();
}
void ReadPrimaryIndexFile(string filename, vector<PrimaryIndex>& primaryIndexList)
{
    ifstream inFile(filename, ios::binary | ios::in);
    if (!inFile)return;
    PrimaryIndex pi;
    while (true)
    {
        inFile.read((char*)&pi, sizeof(pi));
        if (inFile.eof()) break;
        primaryIndexList.push_back(pi);
    }
    cout << filename << " Primary Index loaded successfully.\n";
    inFile.close();
}
void AddToPrimaryIndexList(PrimaryIndex& pi, vector<PrimaryIndex>& primaryIndexList)
{
    primaryIndexList.push_back(pi);
    sort(primaryIndexList.begin(), primaryIndexList.end()); //sorting id
}
void DeleteFromPrimaryIndexList(const int& id, vector<PrimaryIndex>& primaryIndexList)
{
    //binary search for better time complexity
    int low = 0;
    int high = primaryIndexList.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        PrimaryIndex pi = primaryIndexList[mid];

        if (pi.id == id)
        {
            primaryIndexList.erase(primaryIndexList.begin() + mid);
            return;
        }
        else if (pi.id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
}
int SearchInPrimaryIndexList(const int& id, vector<PrimaryIndex>& primaryIndexList)
{
    //binary search for better time complexity
    int low = 0;
    int high = primaryIndexList.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        PrimaryIndex pi = primaryIndexList[mid];

        if (pi.id == id)
            return pi.offset;
        else if (pi.id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}



struct SecondaryIndex
{
    int id;
    char name[50];
};

void WriteSecondaryIndexFile(string filename,vector<SecondaryIndex>& secondaryIndexList)
{
    ofstream outFile(filename, ios::binary | ios::out | ios::trunc);
    for (int i = 0; i < secondaryIndexList.size(); i++)
    {
        SecondaryIndex si = secondaryIndexList[i];
        outFile.write((char*)&si, sizeof(si));
    }
    cout << filename << " Secondary Index saved successfully.\n";
    outFile.close();
}

void ReadSecondaryIndexFile(string filename, vector<SecondaryIndex>& secondaryIndexList)
{
    ifstream inFile(filename, ios::binary | ios::in);
    if (!inFile)return;
    SecondaryIndex si;
    while (true)
    {
        inFile.read((char*)&si, sizeof(si));
        if (inFile.eof()) break;
        secondaryIndexList.push_back(si);
    }
    cout << filename << " Secondary Index loaded successfully.\n";
    inFile.close();
}

void AddToSecondaryIndexList(const char* name, int id, vector<SecondaryIndex>& secondaryIndexList)
{
    SecondaryIndex si;
    strcpy_s(si.name, name);
    si.id = id;
    secondaryIndexList.push_back(si);
}

void DeleteFromSecondaryIndexList(const char* name, int id, vector<SecondaryIndex>& secondaryIndexList)
{
    for (int i = 0; i < secondaryIndexList.size(); i++)
    {

        if (strcmp(secondaryIndexList[i].name, name) == 0 && secondaryIndexList[i].id == id)
        {
            secondaryIndexList.erase(secondaryIndexList.begin() + i);
            break;
        }
    }
}

vector<int> SearchInSecondaryIndexList(const char* name, vector<SecondaryIndex>& secondaryIndexList)
{
    vector<int> ids;
    for (int i = 0; i < secondaryIndexList.size(); i++)
    {
        SecondaryIndex& si = secondaryIndexList[i];
        if (strcmp(si.name, name) == 0)
        {
            ids.push_back(si.id);
        }
    }
    return ids;
}
