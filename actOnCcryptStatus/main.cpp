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
                               int lineNumber,
                               ifstream &readFileHandle);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int lineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
extern "C" bool fileExists(string &lookupFile,string &lookupFileResults);
extern "C" bool fileIsEmpty(string &path);
extern "C" double getSizeOfFile(string &path);
//extern "C" long long int getSizeOfFile(string &path);
extern "C" string getFileName(string &path);

/*******************************************/
/********** File Scope Variables ***********/
/*******************************************/
string backupTarBall="";
//string encryptedFilePath="";
globalStringS globalString;
string backupTarBallPath="";
string purpose="";
string encryptedBackupPath="";
string encryptedBackupPathWithCptExtension="";
string decryptionStartedPath="";
string encryptionStartedPath="";
//string nameOfTheEncryptedBackup="";

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
void print(int lineNumber); // todo - delete after done debugging
bool decryptionInProgress();
bool encryptionInProgress();
bool decryptHadStarted();
bool encryptHadStarted();

/*****************************/
/********* Functions *********/
/*****************************/
int main(int argc, char * const argv[])
{
//cout<<"howdy"<<endl;
    checkTheCommandLineArguments(argc,argv);
    getGlobalStrings(globalString,purpose);
print(__LINE__);
    extractPaths();
print(__LINE__);
    checkThatTheUserFinishedEnteringTheirCcryptPassword();
print(__LINE__);
    checkThatTheTerminalWindowIsStillOpen();
print(__LINE__);
    displayCcryptStatus();
print(__LINE__);
    return 0;
}

void checkTheCommandLineArguments(int argc, char * const argv[])
{
//cout<<__LINE__<<endl;
    //cout << "argc = " << argc << endl;
    //for(int i = 0; i < argc; i++) cout << "argv[" << i << "] = " << argv[i] << endl;
    /* one parameter is expected to be passed in */
//cout<<endl<<endl<<"start"<<endl<<endl;
    if (argc != 2)
    {
        exit(EXIT_SUCCESS);
    }
    purpose = argv[1];
//cout<<endl<<endl<<"purpose = "<<purpose<<endl<<endl;

//    if (argc != 1)
//    {
//        exit(EXIT_SUCCESS);
//    }
}

void checkThatTheUserFinishedEnteringTheirCcryptPassword()
{
//cout<<__LINE__<<endl;
    /* Test to see if the user has finished entering their encryption password*/
    /* If they haven't then these will be true - */
    /* 1) This means that encrypting the backup tar ball has not started yet, */
    /*    so there won't be a <backup tarball>.cpt file */
    /* 2) and ccrypt is listed in the process table as it is is still running */
    /*    waiting for the user to enter or re-enter their encryption password */
    /* When 1) and 2) are both true, do not display a ccrypt status, just exit*/
    /* as this will give the user as much time as he/she needs. */
//cout<<"ccryptHasStarted = "<<ccryptHasStarted()<<endl;
//cout<<"ccryptIsActive = "<<ccryptIsInProcessTable()<<endl;
    if (!ccryptHasStarted() && ccryptIsInProcessTable())
    {
        exit(EXIT_SUCCESS);
    }
    //dcout<<endl<<"encryption key entered"<<endl<<endl;
}

bool ccryptHasStarted()
{
print(__LINE__);
    bool ccryptHasStartedFlag = false;

    if ((purpose == "backup") && encryptionInProgress())
    {
        ccryptHasStartedFlag = true;
        string cmd = "touch "+encryptionStartedPath;
        if(system(cmd.c_str()));
    }
    // purpose = "restore"
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
    string lookupFileResults=
            globalString.basePath+
            "checkThatTheUserFinishedEnteringTheirEncryptionPasswordResults";
    //cout<<encryptedBackupPath<<endl;
    /* As soon as encryption begins, a *.cpt file is created */
    /* if not started a *.cpt file will be absent */
    if (fileExists(encryptedBackupPath,lookupFileResults))
    {
      encryptionIsInProgress = true;
    }
    return encryptionIsInProgress;
}

bool decryptionInProgress()
{
    bool decryptionIsInProgress = false;
    string lookupFileResults=
            globalString.basePath+
            "checkThatTheUserFinishedEnteringTheirDecryptionPasswordResults";
    //cout<<encryptedBackupPath<<endl;
    /* As soon as decryption begins, a backup without the .cpt file is created */
    /* if not started a backup without the .cpt file will be absent */
    if (fileExists(encryptedBackupPath,lookupFileResults))
    {
      decryptionIsInProgress = true;
    }
    return decryptionIsInProgress;
}

//    print(__LINE__);
//    // example) ls encryptedBackupPath* -1 | wc -l
//    // this command yields 1 or 2
//    // 1 means that decrypting has finished
//    // 2 means that decrypting has not finished as a temporary file also
//    //   exists along with the original encrypted backup
//    //   the temporary file always has a different suffix everytime this runs
//    //   ccrypt -d -f -T <encrypted backup>
//    //   once decrypting has finished, the temporary file is deleted
//    bool decryptionInProgressFlag = true;
//    string qtyOfFilesPath=globalString.basePath+"qtyOfFiles";
//    string qtyOfFiles="";
//    string cmd="ls "+encryptedBackupPath+"* -1 | wc -l > "+qtyOfFilesPath;
//    if(system(cmd.c_str()));
//    ifstream qtyOfFilesHandle;
//    openForReading(qtyOfFilesPath,__FILE__,__LINE__,qtyOfFilesHandle);
//    getline(qtyOfFilesHandle,qtyOfFiles);
//    qtyOfFilesHandle.close();
//    if (qtyOfFiles == "1")
//    {
//        //decryption is done
//        decryptionInProgressFlag = false;
//    }
//    return decryptionInProgressFlag;
//}

bool ccryptIsInProcessTable()
{
print(__LINE__);
    string isCcryptGoingResultsPath = globalString.basePath
    //string isCcryptGoingResultsPath = globalString.outputDirectoryPath
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
//cout<<__LINE__<<endl;
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
        //cout<<"encryptedBackupPath = "<<encryptedBackupPath<<endl;
//        ifstream fileContainingTheNameOfTheEncryptedBackupHandle;
//        openForReading
//                   (globalString.fileContainingTheNameOfTheEncryptedBackupPath,
//                    __FILE__,__LINE__,
//                    fileContainingTheNameOfTheEncryptedBackupHandle);
//        getline(fileContainingTheNameOfTheEncryptedBackupHandle,
//                nameOfTheEncryptedBackup);
//        fileContainingTheNameOfTheEncryptedBackupHandle.close();
//        encryptedFilePath = globalString.basePath+nameOfTheEncryptedBackup;
//        cout<<"encryptedFilePath = "<<encryptedFilePath<<endl;

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

bool decryptHadStarted()
{
    bool decryptionHadStarted = false;
    string lookupFileResults = globalString.basePath+
                                 "testIfDecryptionStartedResults";
    if (fileExists(decryptionStartedPath,lookupFileResults))
    {
        decryptionHadStarted = true;
    }
    return decryptionHadStarted;
}

bool encryptHadStarted()
{
    bool encryptionHadStarted = false;
    string lookupFileResults = globalString.basePath+
                                 "testIfEncryptionStartedResults";
    if (fileExists(encryptionStartedPath,lookupFileResults))
    {
        encryptionHadStarted = true;
    }
    return encryptionHadStarted;
}


bool ccryptFinished()
{
print(__LINE__);
    bool ccryptIsDone=false;
    if (purpose == "backup")
    {
        string lookupFileResults=globalString.basePath+
                                 "testIfTheBackupHasBeenFullyEncryptedResults";
        /* the absence of the backup tar ball means that encryption has completed */
        /* since the finished encrypted backup replaced the backup tar ball */
        if (!fileExists(backupTarBallPath,lookupFileResults))
        {
            ccryptIsDone=true;
        }
    }
    else // purpose = restore
    {
        //if (!decryptionInProgress())
        string lookupFileResults=globalString.basePath+
                                 "testIfTheBackupHasBeenFullyDecryptedResults";
        /* the absence of *.cpt means that decryption has finished */
        //cout<<encryptedBackupPathWithCptExtension<<endl;
        if (!fileExists(encryptedBackupPathWithCptExtension,lookupFileResults))
        {
            ccryptIsDone=true;
        }
    }
    return ccryptIsDone;
}

void displayCcryptStatus()
{
//    left off here - if enter the wrong decryption password, ccryptFinished() is true but need someway to confirm that the
//    decryption password was entered correctly.  Maybe check extension of what was decrypted
//    then check that the backup with a failed re-entry of the encrypt password exists gracefully

print(__LINE__);
    if (ccryptFinished())
    {
//prints 3 spaces prior
//cout<<endl<<endl<<__LINE__<<endl<<endl;
        display(100);
    }
    else
    {
        double ccryptPercentageComplete=0.0;
        if  (purpose == "backup")
        {
            // encryption hasn't finished yet, so calculate % completed so far
            double sizeOfTheBackupTarBallToBeEncrypted =
                                            getSizeOfFile(backupTarBallPath);
            ccryptPercentageComplete =
                                    (getSizeOfFile(encryptedBackupPath)/
                                     sizeOfTheBackupTarBallToBeEncrypted)*100;
            //cout<<endl<<"size so far = "<<getSizeOfFile(nameOfTheEncryptedFile)<<endl;
            //cout<<endl<<"total size  = "<<sizeOfTheBackupTarBallToBeEncrypted<<endl;
            //cout<<endl<<"percentage complete = "<<ccryptPercentageComplete<<endl;
        }
        else // purpose = restore
        {
            // decryption hasn't finished yet, so calculate % completed so far
            double sizeOfBackupThatsToBeDecrypted =
                            getSizeOfFile(encryptedBackupPathWithCptExtension);

            ccryptPercentageComplete =
                                    (getSizeOfFile(encryptedBackupPath)/
                                     sizeOfBackupThatsToBeDecrypted)*100;


//            print(__LINE__);
//            // encryption hasn't finished yet, so calculate % completed so far
//
//            /* save the temporary decrypted backup name */
//            string fileContainingNameOfTempDecryptedBackup =
//                globalString.basePath+"fileContainingNameOfTempDecryptedBackup";
//            string cmd="ls "+encryptedBackupPath+".* > "+
//                                        fileContainingNameOfTempDecryptedBackup;
//            if(system(cmd.c_str()));
//            print(__LINE__);
//            /* extract the name of the temporary decrypted backup */
//            string tempDecryptedBackupPath="";
//            ifstream fileContainingNameOfTempDecryptedBackupHandle;
//            openForReading(fileContainingNameOfTempDecryptedBackup,
//               __FILE__,__LINE__,fileContainingNameOfTempDecryptedBackupHandle);
//            getline(fileContainingNameOfTempDecryptedBackupHandle,tempDecryptedBackupPath);
//            fileContainingNameOfTempDecryptedBackupHandle.close();
//            print(__LINE__);
//            /* build the temporary decrypted backup path */
//            //string tempDecryptedBackupPath=globalString.basePath+fileName;
//            //cout<<"tempDecryptedBackupPath = "<<tempDecryptedBackupPath<<endl;
//            /* calculate the percentage complete for decryption */
//            double sizeofTheTemporaryDecryptedBackup =
//                                        getSizeOfFile(tempDecryptedBackupPath);
//            print(__LINE__);
//            ccryptPercentageComplete =
//                            (sizeofTheTemporaryDecryptedBackup/(getSizeOfFile(encryptedBackupPath)))*100;
//            print(__LINE__);
        }


        display(ccryptPercentageComplete);
    }
}

void display(double ccryptPercentageComplete)
{
print(__LINE__);
//cout<<endl<<endl<<ccryptPercentageComplete<<endl<<endl;
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

        // still produces on the next line
        //ccryptPercentageCompleteString  <<fixed<<setprecision(4)
        //                                <<ccryptPercentageComplete<<"%";


        killThisProcess=true;
    }
    else
    {
        // encryption hasn't finished yet so display a percentage between 0-100%
        ccryptPercentageCompleteString  <<fixed<<setprecision(4)
                                        <<ccryptPercentageComplete<<"%";
    }

    cout
        //                       "xx.xxxx%       " - length of 15  or
        //                       " Done (green check mark)
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
print(__LINE__);
    string lookupFileResults=
                globalString.basePath+
                "testIfTheccryptStatusTitleAlreadyPrintedResults";
    if (!fileExists(globalString.ccryptTitlePrintedPath,lookupFileResults))
    {
        if (purpose=="backup")
        {
            cout<<endl<<startUnderline<<"Progress of encrypting your backup"
                                                        <<endUnderline<<endl;
        }
        else // purpose = "restore"
        {
            //cout<<startUnderline<<"Progress of decrypting your backup"
            cout<<endl<<startUnderline<<"Progress of decrypting your backup"
                                                    <<endUnderline<<endl;
        }
        string touchCmd="touch "+globalString.ccryptTitlePrintedPath;
        //cout<<endl<<endl<<touchCmd<<endl<<endl;
        if(system(touchCmd.c_str()));
    }
}

void print(int lineNumber)
{
//    cout<<lineNumber<<endl;
}
