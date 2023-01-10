#include <bits/stdc++.h>
#include <string.h>
#include "parser.h"
#include "fat32.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

// helper functions

void openImage(char **image);
uint32_t CalculateOffset(uint32_t sector);
void lsHandler(char *arg1, char *arg2);
void cdHandler(uint32_t cluster, string arg);
bool checkAttr(FatFileEntry entry);
uint32_t getCluster(string dirname);
void cdForLs(uint32_t cluster, string name);
void lsHandlerForL(char *arg1, char *arg2);
string creationMonth(uint16_t creationDate);
int creationDay(uint16_t creationDate);
int creationHour(uint16_t creationTime);
int creationMinute(uint16_t creationTime);
unsigned char lfn_checksum(const unsigned char *pFCBName);
uint32_t getClusterLs(string dirname);
void mkdirHandler();
void catHandler(uint32_t cluster);
uint16_t time_constructor(tm* now);
uint16_t date_constructor(tm* now);
int findEmptyCluster();
unsigned char shortEntryCreator(FatFile83 &msdos, int special_file, int fileOrDir, int cluster);
int calculate_iteration(int length);
void writeToImage(int offsetToAdd, vector<FatFileEntry> &entries);
uint32_t getClusterForFile(string dirname);
uint32_t getNextCluster(uint32_t cluster);
vector<FatFileEntry> lfnEntryCreator(unsigned char checksum, string filename);
uint32_t bytesPerCluster();



// global variables

BPB_struct BPB;
FILE *filePointer;
FatFileEntry directory;
uint32_t currentDirectory;
uint32_t root;
vector<string> cdArgs;
vector<string> lsArgs;
vector<string> mkdirArgs;
vector<string> catArgs;
vector<string> pwd;
int wrongLsArg = 0;
int wrongCdArg = 0;
int wrongMkdirArg = 0;
int wrongCatArg = 0;
uint32_t fileSizeForCat;

int main(int argc, char **argv)
{

    openImage(argv);
    // currentDirectory = 237;
    while (1)
    {

        if (pwd.empty())
        {
            cout << "/>";
        }

        else
        {
            for (int i = 0; i < pwd.size(); i++)
            {
                cout << "/" << pwd[i];
            }

            cout << ">";
        }

        char *line;
        line = new char[256];
        parsed_input *parsedInput = new parsed_input();
        fgets(line, 256, stdin);
        parse(parsedInput, line);

        if (parsedInput->type == QUIT)
        {
            delete[] line;
            clean_input(parsedInput);
            delete parsedInput;
            break;
        }

        if (parsedInput->type == CD)
        {

            if (parsedInput->arg1 == NULL)
            {
                pwd.clear();
                currentDirectory = root;
                continue;
            }

            if (parsedInput->arg1[0] == '/')
            {
                currentDirectory = root;
                pwd.clear();
            }

            char *token = strtok(parsedInput->arg1, "/");

            while (token != NULL)
            {

                cdArgs.push_back(token);
                token = strtok(NULL, "/");
            }

            for (int i=0; i<cdArgs.size(); i++)
            {
                if (cdArgs[i] == ".")
                {
                    cdArgs.erase(cdArgs.begin()+i);
                }
            }

            for (int i = 0; i < cdArgs.size(); i++)
            {
                int getClusterResult = getCluster(cdArgs[i]);

                if (getClusterResult == -1)
                {
                    break;
                }

                else if (getClusterResult == -2)
                {
                    continue;
                }

                cdHandler(getClusterResult, cdArgs[i]);
            }

            cdArgs.clear();
        }

        if (parsedInput->type == LS)
        {
            // just ls
            if (parsedInput->arg1 == NULL && parsedInput->arg2 == NULL)
            {
                lsHandler(parsedInput->arg1, parsedInput->arg2);
            }

            else if (parsedInput->arg2 == NULL)
            {
                // ls -l
                if (strcmp(parsedInput->arg1, "-l") == 0)
                {
                    lsHandlerForL(parsedInput->arg1, parsedInput->arg2);
                }

                // ls <directory>
                else
                {
                    int control = 0;
                    
                    if (parsedInput->arg1[0] == '/')
                    {
                        control = 1;
                    }

                    char *token = strtok(parsedInput->arg1, "/");

                    while (token != NULL)
                    {
                    
                        lsArgs.push_back(token);
                        token = strtok(NULL, "/");
                    }

                    for (int i=0; i<lsArgs.size(); i++)
                    {
                        if (lsArgs[i] == ".")
                        {
                            lsArgs.erase(lsArgs.begin()+i);
                        }
                    }


                    uint32_t currDirBackup = currentDirectory;

                    if (control == 1)
                    {
                        currentDirectory = root;
                        control = 0;
                    }

                    for (int i = 0; i < lsArgs.size(); i++)
                    {
                        int getClusterResult = getClusterLs(lsArgs[i]);

                        if (getClusterResult == -1)
                        {
                            wrongLsArg = 1;
                            break;
                        }

                        else if (getClusterResult == -2)
                        {
                            continue;
                        }

                        cdForLs(getClusterResult, lsArgs[i]);
                    }

                    if (wrongLsArg == 1)
                    {
                        wrongLsArg = 0;
                        lsArgs.clear();
                        currentDirectory = currDirBackup;
                        continue;
                    }

                    lsHandler(parsedInput->arg1, parsedInput->arg2);
                    lsArgs.clear();
                    currentDirectory = currDirBackup;
                }
            }

            // ls -l <directory>
            else if (parsedInput->arg1 != NULL && parsedInput->arg2 != NULL && (strcmp(parsedInput->arg1, "-l") == 0))
            {
                int control = 0;
                    
                if (parsedInput->arg2[0] == '/')
                {
                    control = 1;
                }

                char *token = strtok(parsedInput->arg2, "/");

                while (token != NULL)
                {
                    
                    lsArgs.push_back(token);
                    token = strtok(NULL, "/");
                }

                for (int i=0; i<lsArgs.size(); i++)
                {
                    if (lsArgs[i] == ".")
                    {
                        lsArgs.erase(lsArgs.begin()+i);
                    }
                }

                uint32_t currDirBackup = currentDirectory;

                if (control == 1)
                {
                    cout<<"buraya girerler"<<endl;
                    currentDirectory = root;
                    control = 0;
                }

                for (int i = 0; i < lsArgs.size(); i++)
                {
                    int getClusterResult = getClusterLs(lsArgs[i]);

                    if (getClusterResult == -1)
                    {
                        wrongLsArg = 1;
                        break;
                    }

                    else if (getClusterResult == -2)
                    {
                        continue;
                    }

                    cdForLs(getClusterResult, lsArgs[i]);
                }

                if (wrongLsArg == 1)
                {
                    wrongLsArg = 0;
                    lsArgs.clear();
                    currentDirectory = currDirBackup;
                    continue;
                }

                lsHandlerForL(parsedInput->arg1, parsedInput->arg2);
                lsArgs.clear();
                currentDirectory = currDirBackup;
            }
        }

        if (parsedInput->type == MKDIR)
        {
            

        }

        if (parsedInput->type == TOUCH)
        {
        }

        if (parsedInput->type == MV)
        {
        }

        if (parsedInput->type == CAT)
        {
            if (parsedInput->arg1 == NULL)
            {
                continue;
            }

            int control = 0;
                    
            if (parsedInput->arg1[0] == '/')
            {
                control = 1;
            }

            char *token = strtok(parsedInput->arg1, "/");

            while (token != NULL)
            {
                
                catArgs.push_back(token);
                token = strtok(NULL, "/");
            }

            for (int i=0; i<catArgs.size(); i++)
            {
                if (catArgs[i] == ".")
                {
                    catArgs.erase(catArgs.begin()+i);
                }
            }

            if (catArgs.size() == 1)
            {
                uint32_t currDirBackup = currentDirectory;

                if (control == 1)
                {
                    currentDirectory = root;
                    control = 0;
                }

                if (getClusterForFile(catArgs[catArgs.size()-1])==-1)
                {
                    catArgs.clear();
                    currentDirectory = currDirBackup;
                    continue;
                }
                

                catHandler(getClusterForFile(catArgs[0]));
                catArgs.clear();
                currentDirectory = currDirBackup;
                continue;
                

            }

            uint32_t currDirBackup = currentDirectory;

            if (control == 1)
            {
                currentDirectory = root;
                control = 0;
            }

            for (int i=0; i < catArgs.size()-1; i++)
            {
                int getClusterResult = getClusterLs(catArgs[i]);

                if (getClusterResult == -1)
                {
                    wrongCatArg = 1;
                    break;
                }

                else if (getClusterResult == -2)
                {
                    continue;
                }

                cdForLs(getClusterResult, catArgs[i]);
            }

            if (wrongCatArg == 1)
            {
                wrongCatArg = 0;
                catArgs.clear();
                currentDirectory = currDirBackup;
                continue;
            }



            if (getClusterForFile(catArgs[catArgs.size()-1]) == -2 && getClusterForFile(catArgs[catArgs.size()-1]) == -1)
            {
                catArgs.clear();
                currentDirectory = currDirBackup;
                continue;
            }

           


            catHandler(getClusterForFile(catArgs[catArgs.size()-1]));
            catArgs.clear();
            currentDirectory = currDirBackup;
        }

        delete[] line;
        clean_input(parsedInput);
        delete parsedInput;
    }

    return 0;
}

void openImage(char **image)
{
    filePointer = fopen(image[1], "r+");

    if (filePointer == NULL)
    {
        return;
    }

    // fseek(filePointer, 3, SEEK_SET);
    // fread(&BPB.BS_OEMName, 8, 1, filePointer);

    // fseek(filePointer, 11, SEEK_SET);
    // fread(&BPB.BytesPerSector, 2, 1, filePointer);
    // fread(&BPB.SectorsPerCluster, 1, 1, filePointer);
    // fread(&BPB.ReservedSectorCount, 2, 1, filePointer);
    // fread(&BPB.NumFATs, 1, 1, filePointer);
    // fread(&BPB.RootEntryCount, 2, 1, filePointer);

    // fread(&BPB.extended, 4, 1, filePointer);

    // fseek(filePointer, 44, SEEK_SET);
    // fread(&BPB.extended.RootCluster, 4, 1,filePointer);
    // currentDirectory = BPB.extended.RootCluster;
    // root = currentDirectory;
    fread(&BPB, 36, 1, filePointer);
    fread(&BPB.extended, 54, 1, filePointer);

    currentDirectory = BPB.extended.RootCluster;
    root = currentDirectory;

    uint32_t offset = CalculateOffset(currentDirectory);
    fseek(filePointer, offset, SEEK_SET);
    fread(&directory.msdos, 32, 1, filePointer);
}

uint32_t CalculateOffset(uint32_t sector)
{
    uint32_t result;
    if (sector == 0)
    {
        sector = 2;
    }

    result = ((sector - 2) * BPB.SectorsPerCluster * BPB.BytesPerSector) + (BPB.BytesPerSector * BPB.ReservedSectorCount) + (BPB.NumFATs * BPB.extended.FATSize * BPB.BytesPerSector);
    return result;
}

void lsHandler(char *arg1, char *arg2)
{

    if (filePointer == NULL)
    {
        return;
    }

    uint32_t offset;

    offset = CalculateOffset(currentDirectory);

    fseek(filePointer, offset, SEEK_SET);

    while(1)
    {


        fread(&directory.lfn, 32, 1, filePointer);

        if (directory.lfn.sequence_number == 0xE5)
        {
            continue;
        }

        if (directory.lfn.sequence_number == 0x00)
        {
            break;
        }

        string nameOfFile = "";

        vector<string> names;

        if (directory.lfn.attributes == 0x0f)
        {
            int control = 0;

            while (directory.lfn.attributes == 0x0f)
            {
                fseek(filePointer, -32, SEEK_CUR);

                fread(&directory.lfn, 32, 1, filePointer);

                // if (control)
                //     continue;
                for (int j = 0; j < 5; j++)
                {

                    if (!(directory.lfn.name1[j] == 0xffff || directory.lfn.name1[j] == 0xff00 || directory.lfn.name1[j] == 0x00ff || directory.lfn.name1[j] == 0x0000))
                    {

                        nameOfFile += (char)directory.lfn.name1[j];
                    }
                }

                // if (control)
                //     continue;
                for (int j = 0; j < 6; j++)
                {

                    if (!(directory.lfn.name2[j] == 0xffff || directory.lfn.name2[j] == 0xff00 || directory.lfn.name2[j] == 0x00ff || directory.lfn.name2[j] == 0x0000))
                    {
                        nameOfFile += (char)directory.lfn.name2[j];
                    }
                }

                // if (control)
                //     continue;
                for (int j = 0; j < 2; j++)
                {
                    if (!(directory.lfn.name3[j] == 0xffff || directory.lfn.name3[j] == 0xff00 || directory.lfn.name3[j] == 0x00ff || directory.lfn.name3[j] == 0x0000))
                    {
                        nameOfFile += (char)directory.lfn.name3[j];
                    }
                }
                fread(&directory.lfn, 32, 1, filePointer);
                names.push_back(nameOfFile);
            }
            fseek(filePointer, -32, SEEK_CUR);
            fread(&directory.msdos, 32, 1, filePointer);
            nameOfFile = "";


            for (int i=names.size()-1; i>-1; i--)
            {

                nameOfFile += names[i];


            }

            if (directory.msdos.attributes != 0x10)
            {
                if (directory.msdos.extension[0] != ' ')
                {
                    nameOfFile += ".";

                    for (int j = 0; j < 3; j++)
                    {

                        if (directory.msdos.extension[j] != ' ')
                            nameOfFile += directory.msdos.extension[j];
                    }
                }
            }

            cout << nameOfFile << " ";
        }
    }
    cout << endl;
}

void lsHandlerForL(char *arg1, char *arg2)
{

    if (filePointer == NULL)
    {
        return;
    }

    uint32_t offset;

    offset = CalculateOffset(currentDirectory);

    fseek(filePointer, offset, SEEK_SET);

    for (int i = 0; i < 16; i++)
    {

        fread(&directory.lfn, 32, 1, filePointer);

        if (directory.lfn.sequence_number == 0xE5)
        {
            continue;
        }

        if (directory.lfn.sequence_number == 0x00)
        {
            break;
        }

        string nameOfFile = "";
        vector<string> names;

        if (directory.lfn.attributes == 0x0f)
        {
            int control = 0;

            while (directory.lfn.attributes == 0x0f)
            {
                fseek(filePointer, -32, SEEK_CUR);

                fread(&directory.lfn, 32, 1, filePointer);

                // if (control)
                //     continue;
                for (int j = 0; j < 5; j++)
                {

                    if (!(directory.lfn.name1[j] == 0xffff || directory.lfn.name1[j] == 0xff00 || directory.lfn.name1[j] == 0x00ff || directory.lfn.name1[j] == 0x0000))
                    {

                        nameOfFile += (char)directory.lfn.name1[j];
                    }
                }

                // if (control)
                //     continue;
                for (int j = 0; j < 6; j++)
                {

                    if (!(directory.lfn.name2[j] == 0xffff || directory.lfn.name2[j] == 0xff00 || directory.lfn.name2[j] == 0x00ff || directory.lfn.name2[j] == 0x0000))
                    {
                        nameOfFile += (char)directory.lfn.name2[j];
                    }
                }

                // if (control)
                //     continue;
                for (int j = 0; j < 2; j++)
                {
                    if (!(directory.lfn.name3[j] == 0xffff || directory.lfn.name3[j] == 0xff00 || directory.lfn.name3[j] == 0x00ff || directory.lfn.name3[j] == 0x0000))
                    {
                        nameOfFile += (char)directory.lfn.name3[j];
                    }
                }
                fread(&directory.lfn, 32, 1, filePointer);
                names.push_back(nameOfFile);
            }
            fseek(filePointer, -32, SEEK_CUR);
            fread(&directory.msdos, 32, 1, filePointer);
            nameOfFile = "";


            for (int i=names.size()-1; i>-1; i--)
            {

                nameOfFile += names[i];


            }

            if (directory.msdos.attributes != 0x10)
            {
                if (directory.msdos.extension[0] != ' ')
                {
                    nameOfFile += ".";

                    for (int j = 0; j < 3; j++)
                    {

                        if (directory.msdos.extension[j] != ' ')
                            nameOfFile += directory.msdos.extension[j];
                    }
                }
            }

            if (directory.lfn.attributes == 0x10)
            {
                string month = creationMonth(directory.msdos.creationDate);
                int day = creationDay(directory.msdos.creationDate);
                int hour = creationHour(directory.msdos.creationTime);
                int minute = creationMinute(directory.msdos.creationTime);

                if (minute > 9 && hour > 9)
                {
                    cout << "drwx------ 1 root root 0 " << month << " " << day << " " << hour << ":" << minute << " " << nameOfFile << endl;
                }

                else if (minute > 9 && hour <= 9)
                {
                    cout << "drwx------ 1 root root 0 " << month << " " << day << " "
                         << "0" << hour << ":" << minute << " " << nameOfFile << endl;
                }

                else if (minute <= 9 && hour > 9)
                {
                    cout << "drwx------ 1 root root 0 " << month << " " << day << " " << hour << ":"
                         << "0" << minute << " " << nameOfFile << endl;
                }

                else if (minute <= 9 && hour <= 9)
                {
                    cout << "drwx------ 1 root root 0 " << month << " " << day << " "
                         << "0" << hour << ":"
                         << "0" << minute << " " << nameOfFile << endl;
                }
            }

            else
            {
                string month = creationMonth(directory.msdos.creationDate);
                int day = creationDay(directory.msdos.creationDate);
                int hour = creationHour(directory.msdos.creationTime);
                int minute = creationMinute(directory.msdos.creationTime);

                if (minute > 9 && hour > 9)
                {
                    cout << "-rwx------ 1 root root " << directory.msdos.fileSize << " " << month << " " << day << " " << hour << ":" << minute << " " << nameOfFile << endl;
                }

                else if (minute > 9 && hour <= 9)
                {
                    cout << "-rwx------ 1 root root " << directory.msdos.fileSize << " " << month << " " << day << " "
                         << "0" << hour << ":" << minute << " " << nameOfFile << endl;
                }

                else if (minute <= 9 && hour > 9)
                {
                    cout << "-rwx------ 1 root root " << directory.msdos.fileSize << " " << month << " " << day << " " << hour << ":"
                         << "0" << minute << " " << nameOfFile << endl;
                }

                else if (minute <= 9 && hour <= 9)
                {
                    cout << "-rwx------ 1 root root " << directory.msdos.fileSize << " " << month << " " << day << " "
                         << "0" << hour << ":"
                         << "0" << minute << " " << nameOfFile << endl;
                }
            }
        }
    }
}

bool checkAttr(FatFileEntry entry)
{
    if (entry.msdos.attributes == 0x10 || entry.msdos.attributes == 0x01 || entry.msdos.attributes == 0x20)
    {
        return true;
    }

    return false;
}

uint32_t getCluster(string dirname)
{

    
    uint32_t offset;

    offset = CalculateOffset(currentDirectory);

    fseek(filePointer, offset, SEEK_SET);

    while (1)
    {

        string filename = ""; // free leyince  hata veriyor tekrar kontrol et!!!!!!!!!!!

        fread(&directory.lfn, 32, 1, filePointer);

        if (directory.lfn.sequence_number == 0xE5)
        {
            continue;
        }

        if (directory.lfn.sequence_number == 0x00)
        {
            break;
        }

        vector<string> names;


        if (dirname == "..")
        {
            if ((char)directory.lfn.sequence_number == '.' && (char)directory.lfn.name1[0] == '.')
            {
                currentDirectory = directory.lfn.firstCluster;
                if (!pwd.empty())
                {
                    pwd.pop_back();
                }
                return -2;
            }
        }

        if (directory.lfn.attributes == 0x0f)
        {
            int control = 0;

            while (directory.lfn.attributes == 0x0f)
            {
                fseek(filePointer, -32, SEEK_CUR);

                fread(&directory.lfn, 32, 1, filePointer);

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 5; j++)
                {

                    if (!(directory.lfn.name1[j] == 0xffff || directory.lfn.name1[j] == 0xff00 || directory.lfn.name1[j] == 0x00ff || directory.lfn.name1[j] == 0x0000))
                    {

                        filename += (char)directory.lfn.name1[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 6; j++)
                {

                    if (!(directory.lfn.name2[j] == 0xffff || directory.lfn.name2[j] == 0xff00 || directory.lfn.name2[j] == 0x00ff || directory.lfn.name2[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name2[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 2; j++)
                {
                    if (!(directory.lfn.name3[j] == 0xffff || directory.lfn.name3[j] == 0xff00 || directory.lfn.name3[j] == 0x00ff || directory.lfn.name3[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name3[j];
                    }
                }
                fread(&directory.lfn, 32, 1, filePointer);
                names.push_back(filename);
            }

            fseek(filePointer, -32, SEEK_CUR);
            fread(&directory.msdos, 32, 1, filePointer);
            filename = "";


            for (int i=names.size()-1; i>-1; i--)
            {

                filename += names[i];


            }

            if (directory.msdos.attributes != 0x10)
            {
                continue;
            }
        }

        if (dirname == filename)
        {
            int cluster = directory.msdos.firstCluster;
            return cluster;
        }
    }

    return -1;
}

uint32_t getClusterForFile(string dirname)
{

    
    uint32_t offset;

    offset = CalculateOffset(currentDirectory);

    fseek(filePointer, offset, SEEK_SET);

    while (1)
    {

        string filename = ""; // free leyince  hata veriyor tekrar kontrol et!!!!!!!!!!!

        fread(&directory.lfn, 32, 1, filePointer);

        if (directory.lfn.sequence_number == 0xE5)
        {
            continue;
        }

        if (directory.lfn.sequence_number == 0x00)
        {
            break;
        }

        vector<string> names;


        if (dirname == "..")
        {
            if ((char)directory.lfn.sequence_number == '.' && (char)directory.lfn.name1[0] == '.')
            {
                currentDirectory = directory.lfn.firstCluster;
                
                return -2;
            }
        }

        if (directory.lfn.attributes == 0x0f)
        {
            int control = 0;

            while (directory.lfn.attributes == 0x0f)
            {
                fseek(filePointer, -32, SEEK_CUR);

                fread(&directory.lfn, 32, 1, filePointer);

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 5; j++)
                {

                    if (!(directory.lfn.name1[j] == 0xffff || directory.lfn.name1[j] == 0xff00 || directory.lfn.name1[j] == 0x00ff || directory.lfn.name1[j] == 0x0000))
                    {

                        filename += (char)directory.lfn.name1[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 6; j++)
                {

                    if (!(directory.lfn.name2[j] == 0xffff || directory.lfn.name2[j] == 0xff00 || directory.lfn.name2[j] == 0x00ff || directory.lfn.name2[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name2[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 2; j++)
                {
                    if (!(directory.lfn.name3[j] == 0xffff || directory.lfn.name3[j] == 0xff00 || directory.lfn.name3[j] == 0x00ff || directory.lfn.name3[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name3[j];
                    }
                }
                fread(&directory.lfn, 32, 1, filePointer);
                names.push_back(filename);
            }

            fseek(filePointer, -32, SEEK_CUR);
            fread(&directory.msdos, 32, 1, filePointer);
            filename = "";


            for (int i=names.size()-1; i>-1; i--)
            {

                filename += names[i];


            }

            if (directory.msdos.attributes == 0x10)
            {
                continue;
            }
        }

        if (dirname == filename)
        {
            uint32_t cluster = ((directory.msdos.eaIndex << 2) | directory.msdos.firstCluster);
            fileSizeForCat = directory.msdos.fileSize;
            return cluster;
        }
    }

    return -1;
}
uint32_t getClusterLs(string dirname)
{

    
    uint32_t offset;

    offset = CalculateOffset(currentDirectory);

    fseek(filePointer, offset, SEEK_SET);

    while (1)
    {


        fread(&directory.lfn, 32, 1, filePointer);

        if (directory.lfn.sequence_number == 0xE5)
        {
            continue;
        }

        if (directory.lfn.sequence_number == 0x00)
        {
            break;
        }

        string filename = ""; // free leyince  hata veriyor tekrar kontrol et!!!!!!!!!!!
        vector<string> names;


        if (dirname == "..")
        {
            if ((char)directory.lfn.sequence_number == '.' && (char)directory.lfn.name1[0] == '.')
            {
                currentDirectory = directory.lfn.firstCluster;

                return -2;
            }
        }

        if (directory.lfn.attributes == 0x0f)
        {
            int control = 0;

            while (directory.lfn.attributes == 0x0f)
            {
                fseek(filePointer, -32, SEEK_CUR);

                fread(&directory.lfn, 32, 1, filePointer);

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 5; j++)
                {

                    if (!(directory.lfn.name1[j] == 0xffff || directory.lfn.name1[j] == 0xff00 || directory.lfn.name1[j] == 0x00ff || directory.lfn.name1[j] == 0x0000))
                    {

                        filename += (char)directory.lfn.name1[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 6; j++)
                {

                    if (!(directory.lfn.name2[j] == 0xffff || directory.lfn.name2[j] == 0xff00 || directory.lfn.name2[j] == 0x00ff || directory.lfn.name2[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name2[j];
                    }
                }

                // if (control)
                // {
                //     continue;
                // }

                for (int j = 0; j < 2; j++)
                {
                    if (!(directory.lfn.name3[j] == 0xffff || directory.lfn.name3[j] == 0xff00 || directory.lfn.name3[j] == 0x00ff || directory.lfn.name3[j] == 0x0000))
                    {
                        filename += (char)directory.lfn.name3[j];
                    }
                }
                fread(&directory.lfn, 32, 1, filePointer);
                names.push_back(filename);
            }

            fseek(filePointer, -32, SEEK_CUR);
            fread(&directory.msdos, 32, 1, filePointer);
            filename = "";


            for (int i=names.size()-1; i>-1; i--)
            {

                filename += names[i];


            }

            if (directory.msdos.attributes != 0x10)
            {
                continue;
            }
        }

        if (dirname == filename)
        {
            int cluster = directory.msdos.firstCluster;
            return cluster;
        }
    }

    return -1;
}

void cdHandler(uint32_t cluster, string name)
{

    pwd.push_back(name);

    uint32_t offset = CalculateOffset(cluster);
    currentDirectory = cluster;
    fseek(filePointer, offset, SEEK_SET);
    fread(&directory.lfn, 32, 1, filePointer);

    if (directory.lfn.attributes == 0x0f)
    {
        int control = 0;

        while (directory.lfn.attributes == 0x0f)
        {
            fseek(filePointer, -32, SEEK_CUR);

            fread(&directory.lfn, 32, 1, filePointer);
        }
        fseek(filePointer, -32, SEEK_CUR);
        fread(&directory.msdos, 32, 1, filePointer);
    }
}

void cdForLs(uint32_t cluster, string name)
{

    uint32_t offset = CalculateOffset(cluster);
    currentDirectory = cluster;
    fseek(filePointer, offset, SEEK_SET);
    fread(&directory.lfn, 32, 1, filePointer);

    if (directory.lfn.attributes == 0x0f)
    {
        int control = 0;

        while (directory.lfn.attributes == 0x0f)
        {
            fseek(filePointer, -32, SEEK_CUR);

            fread(&directory.lfn, 32, 1, filePointer);
        }
        fseek(filePointer, -32, SEEK_CUR);
        fread(&directory.msdos, 32, 1, filePointer);
    }
}

string creationMonth(uint16_t creationDate)
{
    string months[12] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"};

    uint16_t month = creationDate;
    month &= 0x01e0;
    month = month >> 5;

    return months[month - 1];
}

int creationDay(uint16_t creationDate)
{
    uint16_t day = creationDate;
    day &= 0x001f;
    return day;
}

int creationHour(uint16_t creationTime)
{
    uint16_t hour = creationTime;
    hour &= 0xf800;
    hour = hour >> 11;
    return (hour % 23);
}

int creationMinute(uint16_t creationTime)
{
    uint16_t minute = creationTime;
    minute &= 0x07e0;
    minute = minute >> 5;
    return (minute % 60);
}

unsigned char lfn_checksum(const unsigned char *pFCBName)
{
    int i;
    unsigned char sum = 0;

    for (i = 11; i; i--)
        sum = ((sum & 1) << 7) + (sum >> 1) + *pFCBName++;

    return sum;
}

// void mkdirHandler()
// {
//     FatFileEntry newDirectory;
//     vector<FatFileEntry> entries;

//     string folderName = mkdirArgs[mkdirArgs.size()-1];

//     int checksum = shortEntryCreator(newDirectory.msdos, 0, 0, 0);
//     entries = lfnEntryCreator(checksum, folderName);
//     entries.push_back(newDirectory);
//     int clusterTemp;


//     if (mkdirArgs.size() == 1)
//     {
//         clusterTemp = currentDirectory;
//     }

//     else if (mkdirArgs.size() > 1)
//     {
//         clusterTemp = getCluster(mkdirArgs[mkdirArgs.size()-1]);
//     }

//     //currentDirectory = 

    


// }

void writeToImage(int offsetToAdd, vector<FatFileEntry> &entries)
{
    FatFileEntry entry;
    int offset;



    offset = CalculateOffset(currentDirectory);
    fseek(filePointer, offset + offsetToAdd, SEEK_SET);

    for (int i=entries.size()-1; i>-1; i--)
    {
        
        entry = entries[i];

        fwrite(&entry, 32, 1, filePointer);
        

        
    }
}

// unsigned char shortEntryCreator(FatFile83 &msdos, int special_file, int fileOrDir, int cluster)
// {
//     for (int i=0; i<3; i++)
//     {
//         msdos.extension[i] = 0x20;
//     }

//     for (int i=0; i<8; i++)
//     {
//         msdos.filename[i] = 0x20;
//     }

//     if (special_file == 0)
//     {
//         // msdos.filename[0] = '~';

//         if (fileOrDir == 0) // touch
//         {
//             msdos.eaIndex = 0;
//             msdos.firstCluster = 0;
//             msdos.attributes = 0x20;
//         }

//         if (fileOrDir == 1) // mkdir
//         {
//             int emptyCluster = findEmptyCluster();
//             msdos.firstCluster = emptyCluster;
//             msdos.eaIndex = emptyCluster >> 16;
//             msdos.attributes = 0x10;
//         }

//     }

//     else if (special_file == 1) // dot case
//     {
//         msdos.filename[0] = '.';
//         msdos.attributes = 0x10;
//         msdos.firstCluster = cluster;
//         msdos.eaIndex = cluster >> 16;

//     }

//     else if (special_file == 2) //dotdot case
//     {
//         msdos.filename[0] = '.';
//         msdos.filename[1] = '.';
//         msdos.attributes = 0x10;
//         msdos.firstCluster = cluster;
//         msdos.eaIndex = cluster >> 16;

//     }

//     msdos.fileSize = 0x00000000;
//     msdos.creationTimeMs = 0x00;
//     msdos.reserved = 0x00;

//     time_t timeContainer = time(0);
//     tm * currentTime = localtime(&timeContainer);

//     uint16_t date = date_constructor(currentTime);
//     uint16_t time = time_constructor(currentTime);

//     msdos.creationTime = time;
//     msdos.modifiedTime = time;
//     msdos.creationDate = date;
//     msdos.modifiedDate = date;
//     msdos.lastAccessTime = date;

//     auto chrs = mkdirArgs[mkdirArgs.size()-1].c_str();

//     auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(chrs));

//     unsigned char checkSum = lfn_checksum(uchrs);
//     return checkSum; 

// }

// vector<FatFileEntry> lfnEntryCreator(unsigned char checksum, string filename)
// {
//     vector<FatFileEntry> entries;

//     int i = 0;

//     int length = filename.length();

//     int iteration = calculate_iteration(length);

//     int filenameIndex = 0;

//     for (int i=0; i<iteration; i++)
//     {
//         FatFileEntry entry;

//         for (int i=0; i<5; i++)
//         {
//             entry.lfn.name1[i] = 0xffff;
//         }

//         for (int i=0; i<6; i++)
//         {
//             entry.lfn.name2[i] = 0xffff;
//         }

//         for (int i=0; i<2; i++)
//         {
//             entry.lfn.name3[i] = 0xffff;
//         }

//         entry.lfn.checksum = checksum;
//         entry.lfn.sequence_number = 0x41;
//         entry.lfn.attributes = 0x0f;
//         entry.lfn.reserved = 0x00;
//         entry.lfn.firstCluster = 0x0000;


//         int x = 0;

//         while (x < 5 && filename[filenameIndex] != '\0' && filenameIndex < filename.length())
//         {
//             entry.lfn.name1[x] = filename[filenameIndex];
//             x++;
//             filenameIndex++;
//         }

//         int y = 0;

//         while (y < 6 && filename[filenameIndex] != '\0' && filenameIndex < filename.length())
//         {
//             entry.lfn.name2[y] = filename[filenameIndex];
//             y++;
//             filenameIndex++;
//         }

//         int z = 0;

//         while (z < 5 && filename[filenameIndex] != '\0' && filenameIndex < filename.length())
//         {
//             entry.lfn.name3[z] = filename[filenameIndex];
//             z++;
//             filenameIndex++;
//         }

//         entries.push_back(entry);


//     }

//     return entries;
// }

int calculate_iteration(int length)
{
    int iteration;

    if (length % 13 != 0)
    {
        iteration = (length / 13) + 1;
    }

    else if (iteration % 13 == 0)
    {
        iteration = length / 13;
    }

    return iteration;


}


int findEmptyCluster()
{
    uint32_t emptyCluster;
    int entryNumber = (BPB.extended.FATSize * BPB.BytesPerSector) / 4;

    for (int i=2; i<entryNumber; i++)
    {
        int offset = (BPB.BytesPerSector * BPB.ReservedSectorCount) + (i*4);

        fseek(filePointer, offset, SEEK_SET);
        fread(&emptyCluster, 4, 1, filePointer);

        emptyCluster &= 0x0FFFFFFF;

        if (emptyCluster == 0x00000000)
        {
            emptyCluster = 0x0FFFFFFF;
            fseek(filePointer, offset, SEEK_SET);
            fwrite(&emptyCluster, 4, 1, filePointer);
            return i;
        }

    }

    return -1;
}

uint16_t date_constructor(tm* now)
{
    uint16_t date = 0x00FFFF & (((now->tm_year - 80) << 9) | (now->tm_mon << 5) | now->tm_mday);

}

uint16_t time_constructor(tm* now)
{
    uint16_t time = 0x00FFFF & (now->tm_hour << 11) | (now->tm_min << 5) | (now->tm_sec) ;

}

void catHandler(uint32_t cluster)
{
    string fileContents="";
    uint32_t offset;
 
    while (cluster != -1)
    {
        string temp;
        temp.resize(bytesPerCluster());
        offset = CalculateOffset(cluster);
        
        fseek(filePointer, offset, SEEK_SET);

        if (fileSizeForCat < bytesPerCluster())
        {
            fread(&temp[0], 1, fileSizeForCat, filePointer);
            fileContents += temp;
            break;
        }

        else
        {
            fread(&temp[0], 1, bytesPerCluster(), filePointer);
            fileSizeForCat -= bytesPerCluster();
            fileContents += temp;
            cluster = getNextCluster(cluster);

            
        }

        
    }
    cout<<fileContents<<endl;
   
}

uint32_t getNextCluster(uint32_t cluster)
{
    uint32_t nextCluster;

    if (cluster == 0)
    {
        cluster = 2;
    }

    uint32_t cur = cluster;

    
    uint32_t offset = (BPB.BytesPerSector * BPB.ReservedSectorCount) + (cur * 4);
    
    fseek(filePointer, offset, SEEK_SET);
    fread(&nextCluster, 4, 1, filePointer);
    
    nextCluster &= 0x0fffffff;
    
    if (nextCluster < 0x0ffffff0)
    {
        return nextCluster;
    }
    return -1;
}

uint32_t bytesPerCluster()
{
    return (BPB.BytesPerSector)*(BPB.SectorsPerCluster);
}

