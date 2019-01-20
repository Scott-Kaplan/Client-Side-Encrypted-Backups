/*
MIT License

Copyright (c) 2018-2019 Scott Kaplan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*******************************/
/********* Namespaces **********/
/*******************************/
using namespace std;

/*****************************/
/********* Includes **********/
/*****************************/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "../staticLibrary__fileUtilities/fileUtilities.h"
#include <sstream> // needed for ostringstream
#include <iomanip> // setw() - sets column width of a field
#include <sys/types.h>
#include <signal.h>

/********************************/
/******* Static Libraries *******/
/********************************/
extern "C" void getGlobalStrings(globalStringS &globalString,string &purpose);
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ifstream &readFileHandle);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
extern "C" bool fileExist(string &lookupFilePath,
                          string fromFileName,
                          int fromLineNumber,
                          string resultsDirectory);
extern "C" bool fileIsEmpty(string &path);
extern "C" double getSizeOfFile(string &path);
extern "C" string getFileName(string &path);

/*******************************************/
/********** File Scope Variables ***********/
/*******************************************/
string backupTarBall="";
globalStringS globalString;
string backupTarBallPath="";
string purpose="";
string encryptedBackupPath="";
string encryptedBackupPathWithCptExtension="";
string decryptionStartedPath="";
string encryptionStartedPath="";

/***************************************/
/********* Function Prototypes *********/
/***************************************/
void checkTheCommandLineArguments(int argc, char * const argv[]);
void checkThatTheUserFinishedEnteringTheirCcryptPassword();
void testIfTheBackupHasBeenFullyEncrypted();
void checkThatTheTerminalWindowIsStillOpen();
bool ccryptHasStarted();
bool ccryptIsInProcessTable();
void calculateAndShowTheEncryptionPercentageComplete();
void display(double ccryptPercentageComplete);
void displayTitleOfCcryptProgress();
void extractPaths();
void displayCcryptStatus();
bool ccryptFinished();
bool decryptionInProgress();
bool encryptionInProgress();
bool decryptHadStarted();
bool encryptHadStarted();

/*****************************/
/********* Functions *********/
/*****************************/
int main(int argc, char * const argv[])
{
    checkTheCommandLineArguments(argc,argv);
    getGlobalStrings(globalString,purpose);
    extractPaths();
    checkThatTheUserFinishedEnteringTheirCcryptPassword();
    checkThatTheTerminalWindowIsStillOpen();
    displayCcryptStatus();
    return 0;
}

void checkTheCommandLineArguments(int argc, char * const argv[])
{
    /* one parameter is expected to be passed in */
    if (argc != 2)
    {
        exit(EXIT_SUCCESS);
    }
    purpose = argv[1];
}

void checkThatTheUserFinishedEnteringTheirCcryptPassword()
{
    /* Test to see if the user has finished entering their encryption password*/
    /* If they haven't then these will be true - */
    /* 1) This means that encrypting the backup tar ball has not started yet, */
    /*    so there won't be a <backup tarball>.cpt file */
    /* 2) and ccrypt is listed in the process table as it is is still running */
    /*    waiting for the user to enter or re-enter their encryption password */
    /* When 1) and 2) are both true, do not display a ccrypt status, just exit*/
    /* as this will give the user as much time as he/she needs. */
    if (!ccryptHasStarted() && ccryptIsInProcessTable())
    {
        exit(EXIT_SUCCESS);
    }
}

bool ccryptHasStarted()
{
    bool ccryptHasStartedFlag = false;
    if ((purpose == "backup") && encryptionInProgress())
    {
        ccryptHasStartedFlag = true;
        string cmd = "touch "+encryptionStartedPath;
        if(system(cmd.c_str()));
    }
    else if ((purpose == "restore") && decryptionInProgress())
    {
        ccryptHasStartedFlag = true;
        string cmd = "touch "+decryptionStartedPath;
        if(system(cmd.c_str()));
    }
    return ccryptHasStartedFlag;
}

bool encryptionInProgress()
{
    bool encryptionIsInProgress = false;
    /* As soon as encryption begins, a *.cpt file is created */
    /* if not started a *.cpt file will be absent */
    if (fileExist(encryptedBackupPath,__FILE__,__LINE__,purpose))
    {
      encryptionIsInProgress = true;
    }
    return encryptionIsInProgress;
}

bool decryptionInProgress()
{
    bool decryptionIsInProgress = false;
    /* As soon as decryption begins, a backup without the .cpt file is created */
    /* if not started a backup without the .cpt file will be absent */
    if (fileExist(encryptedBackupPath,__FILE__,__LINE__,purpose))
    {
      decryptionIsInProgress = true;
    }
    return decryptionIsInProgress;
}

bool ccryptIsInProcessTable()
{
    string isCcryptGoingResultsPath = globalString.basePath
                                      +"isCcryptGoingResults";
    string isCcryptGoingCmd ="ps cax | grep ccrypt > "+isCcryptGoingResultsPath;
    if (system(isCcryptGoingCmd.c_str()));
    if (fileIsEmpty(isCcryptGoingResultsPath))
    {
        /* ccrypt is not in the process table, so ccrypt is no longer active */
        return false;
    }
    else
    {
        /* ccrypt is in the process table, so ccrypt is still active */
        return true;
    }
}

void extractPaths()
{
    if (purpose == "backup")
    {
        ifstream fileContainingTheBackupTarBallPathHandle;
        openForReading(globalString.fileContainingTheBackupTarBallPath,
                       __FILE__,__LINE__,
                       fileContainingTheBackupTarBallPathHandle);
        getline(fileContainingTheBackupTarBallPathHandle,backupTarBallPath);
        fileContainingTheBackupTarBallPathHandle.close();
        encryptedBackupPath = backupTarBallPath+".cpt";
        encryptionStartedPath = globalString.basePath+"encryptionStarted";
    }
    else if (purpose == "restore")
    {
        ifstream fileContainingTheEncryptedBackupHandle;
        openForReading
                   (globalString.fileContainingTheEncryptedBackupPath,
                    __FILE__,__LINE__,
                    fileContainingTheEncryptedBackupHandle);
        getline(fileContainingTheEncryptedBackupHandle,
                encryptedBackupPath);
        fileContainingTheEncryptedBackupHandle.close();
        decryptionStartedPath = globalString.basePath+"decryptionStarted";
        encryptedBackupPathWithCptExtension = encryptedBackupPath+".cpt";
    }
}

void checkThatTheTerminalWindowIsStillOpen()
{
    string pidString="";
    ifstream terminalPidHandle;
    openForReading(globalString.processIdOfThisTerminalSessionPath,
                   __FILE__,__LINE__,terminalPidHandle);
    getline(terminalPidHandle,pidString);
    terminalPidHandle.close();
    // source https://stackoverflow.com/questions/12601759/determine-if-a-process-exists-from-its-process-id
    if (kill(atoi(pidString.c_str()), 0) != 0)
    {
        /* the user closed the terminal window, */
        /* so kill this ccrypt status process   */
        if(system(globalString.killTheCcryptStatusProcess.c_str()));
        exit(EXIT_SUCCESS);
    }
}

bool ccryptFinished()
{
    bool ccryptIsDone=false;
    if (purpose == "backup")
    {
        /* the absence of the backup tar ball means that encryption has completed */
        /* since the finished encrypted backup replaced the backup tar ball */
        if (!fileExist(backupTarBallPath,__FILE__,__LINE__,purpose))
        {
            ccryptIsDone=true;
        }
    }
    else // purpose = restore
    {
        /* the absence of *.cpt means that decryption has finished */
        if (!fileExist(encryptedBackupPathWithCptExtension,__FILE__,__LINE__,purpose))
        {
            ccryptIsDone=true;
        }
    }
    return ccryptIsDone;
}

void displayCcryptStatus()
{
    if (ccryptFinished())
    {
        display(100);
    }
    else
    {
        double ccryptPercentageComplete=0.0;
        if  (purpose == "backup")
        {
            // encryption hasn't finished yet, so calculate the % complete so far
            double sizeOfTheBackupTarBallToBeEncrypted =
                                            getSizeOfFile(backupTarBallPath);
            ccryptPercentageComplete =
                                    (getSizeOfFile(encryptedBackupPath)/
                                     sizeOfTheBackupTarBallToBeEncrypted)*100;
        }
        else // purpose = restore
        {
            // decryption hasn't finished yet, so calculate % completed so far
            double sizeOfBackupThatsToBeDecrypted =
                            getSizeOfFile(encryptedBackupPathWithCptExtension);

            ccryptPercentageComplete =
                                    (getSizeOfFile(encryptedBackupPath)/
                                     sizeOfBackupThatsToBeDecrypted)*100;
        }
        display(ccryptPercentageComplete);
    }
}

void display(double ccryptPercentageComplete)
{
    bool killThisProcess=false;
    // display title if haven't already
    displayTitleOfCcryptProgress();

    ostringstream ccryptPercentageCompleteString;
    if (ccryptPercentageComplete == 100)
    {
        // since encryption is done, display green checkmark and set to kill
        // this background process
        ccryptPercentageCompleteString<<
            //Note:  15 extra trailing spaces here to clobber any prior xx.xxxxxx%
            "Done \033[0;32m\xE2\x9C\x94\033[0m               "<<endl;
        killThisProcess=true;
    }
    else
    {
        // encryption hasn't finished yet so display a percentage between 0-100%
        ccryptPercentageCompleteString  <<fixed<<setprecision(6)
                                        <<ccryptPercentageComplete<<"%";
    }
    cout
        //                       "xx.xxxxxx%     " - length of 15  or
        //                       " Done (with green check mark)
        << left <<  setw(15) <<  ccryptPercentageCompleteString.str()
        << " \r";
    cout.flush();

    if (killThisProcess)
    {
        // create handle to let foreground script know that ccrypt finished
        // gracefully so that it can continue processing (there is a sleep 1
        // waiting for this file to exist before it can continue processing)
        string ccryptFinishedCmd="touch "+globalString.ccryptFinishedGracefully;
        if(system(ccryptFinishedCmd.c_str()));

        // kill this background process now that encrypting/decrypting finished
        if(system(globalString.killTheCcryptStatusProcess.c_str()));
    }
}

void displayTitleOfCcryptProgress()
{
    if (!fileExist(globalString.ccryptTitlePrintedPath,__FILE__,__LINE__,purpose))
    {
        if (purpose=="backup")
        {
            cout<<endl<<startUnderline<<"Progress of encrypting your backup"
                                                        <<endUnderline<<endl;
        }
        else // purpose = "restore"
        {
            cout<<endl<<startUnderline<<"Progress of decrypting your backup"
                                                    <<endUnderline<<endl;
        }
        string touchCmd="touch "+globalString.ccryptTitlePrintedPath;
        if(system(touchCmd.c_str()));
    }
}
