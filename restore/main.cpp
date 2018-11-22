/*
MIT License

Copyright (c) 2018 Scott Kaplan

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

/**********************/
/***** Namespaces *****/
/**********************/
using namespace std;

/********************/
/***** Includes *****/
/********************/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "../staticLibrary__fileUtilities/fileUtilities.h"

/****************************/
/***** Static Libraries *****/
/****************************/
extern "C" string getTimestamp();
extern "C" void retrieveUsernameAndDomain
            (string &username, string &domain, string &usernameAndDomainPath);
extern "C" void getGlobalStrings(globalStringS &globalString,string &purpose);
extern "C" bool fileExists(string &lookupFile,string &lookupFileResults);
extern "C" void clearTheTerminalWindow();
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int lineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
void createRestoreDirectory();
extern "C" string getFileName(string &path);
extern "C" void extractPathAndFileName
                        (string &stringToParse, string &path, string &fileName);
extern "C" void writeCleanUpAndExitFunction
                (string directory, ofstream &scriptThatRestoresTheBackupHandle);
extern "C" void clearTheTerminalWindow();

/*******************************/
/***** Function Prototypes *****/
/*******************************/
void checkThatTheNeededConfigurationFilesExist();
void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[]);
void displayUsage();
void createAScriptTheWillRestoreTheBackup();
void runTheScriptThatRestoresTheBackup();
void createTemporaryPathToRestoreBackupIn
                                       (string &temporaryPathToRestoreBackupIn);
//bool backupIsAtBaseDirectory(string &path);
//string getdirectoryThatTheEncryptedBackupIsIn(string &nameOfEncryptedBackupWhichIncludesPath);
void clearTheRestoreDirectory();

/********************/
/***** Constants ****/
/********************/
string tab0="";
string tab1="  ";
string tab2="    ";
string tab3="      ";

/********************************/
/***** File Scope Variables *****/
/********************************/
string purpose="restore";  // for path $HOME/.cloudbuddy/restore
string username="";
string domain="";
string nameOfEncryptedBackupWhichIncludesPath="";
string nameOfEncryptedBackup="";
string directoryThatTheEncryptedBackupIsIn ="";
globalStringS globalString;
string restoreDirectory="$HOME/.cloudbuddy/restore/";
string scriptThatRestoresTheBackupPath=
                                restoreDirectory+"scriptThatRestoresTheBackup";
//void extractPathToBackupAndnameOfEncryptedBackup(string &nameOfEncryptedBackupWhichIncludesPath);

/*********************/
/***** Functions *****/
/*********************/
int main(int argc, char * const argv[])
{
    clearTheTerminalWindow();
    clearTheRestoreDirectory();
    getGlobalStrings(globalString,purpose);
    checkThatTheNeededConfigurationFilesExist();
    retrieveUsernameAndDomain(username,domain,globalString.usernameAndDomainPath);
    checkThatTheCommandLineArgumentsAreCorrect(argc,argv);
    createAScriptTheWillRestoreTheBackup();
    runTheScriptThatRestoresTheBackup();
    //clearTheRestoreDirectory();
    return 0;
}

void clearTheRestoreDirectory()
{
    string cmd="rm $HOME/.cloudbuddy/restore/*";
    //cout<<cmd<<endl;
    if(system(cmd.c_str()));
}

void checkThatTheNeededConfigurationFilesExist()
{
    string lookupFileResults=globalString.basePath+"FoundResults";
    //string lookupFileResults=globalString.outputDirectoryPath+"FoundResults";
//cout<<endl<<globalString.usernameAndDomainPath<<endl<<endl;

    if (!fileExists(globalString.usernameAndDomainPath,lookupFileResults))
    {
        cout<<"Error: \nThis configuration file could not be read - \n"
            <<globalString.usernameAndDomainPath<<endl;
        exit(EXIT_SUCCESS);
    }
}

void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[])
{
    if (argc != 2)
    {
        // the user didn't enter one parameter which equals
        // the filename with path that's to be restored.  Example
        // restore /uploads/e6230-pics-of-aunt-mary**2018-06-26__09:26pm
        displayUsage();
    }
    nameOfEncryptedBackupWhichIncludesPath = argv[1];
    /* both of these work
    ./codeBlockProjects/open\ source/restore/bin/Release/restore /uploads/l__12_16_2012__11_14pm
    ./codeBlockProjects/open\ source/restore/bin/Release/restore l__12_16_2012__11_14pm
    */
    extractPathAndFileName
        (nameOfEncryptedBackupWhichIncludesPath,
         directoryThatTheEncryptedBackupIsIn,
         nameOfEncryptedBackup);
}

void displayUsage()
{
    cout
    <<"\nUsage:\n\nrestore "<<startUnderline
    <<"<path>/<backup-to-restore>\n\n"<<endUnderline
    <<"Example:\n"
      "If your backup is in the '/uploads' directory on the server and is named"
      "\ne6230-pics-of-aunt-mary**2018-06-26__09:26pm\nthen you'd type-\n"
      "restore "<<startUnderline<<
      "/uploads/e6230-pics-of-aunt-mary**2018-06-26__09:26pm"<<endUnderline
    <<"\n\nHere is another example:\n"
      "If your backup is in the same directory of where you initially log in "
      "and is named\ne6230-pics-of-aunt-mary**2018-06-26__09:26pm\nthen you'd "
      "type-\nrestore "<<startUnderline<<
      "e6230-pics-of-aunt-mary**2018-06-26__09:26pm"<<endUnderline
    <<"\n\nNote: If the path that you have the backup in contains spaces, then"
      "\nthe entire first parameter needs to be quoted like so-\n"
      "restore "<<startUnderline<<
      "\"/directory name/e6230-pics-of-aunt-mary**2018-06-26__09:26pm\""
      <<endUnderline<<endl<<endl<<endl;
    exit(EXIT_FAILURE);
}

void createAScriptTheWillRestoreTheBackup()
{
    /* write the path to the encrypted backup so that the actOnCcrypt binary */
    /* can access it */
    ofstream fileContainingTheEncryptedBackupHandle;
    openForWriting(globalString.fileContainingTheEncryptedBackupPath
                   ,__FILE__,__LINE__
                   ,fileContainingTheEncryptedBackupHandle,NEW_FILE);
    fileContainingTheEncryptedBackupHandle
                <<globalString.basePath+nameOfEncryptedBackup;
    fileContainingTheEncryptedBackupHandle.close();
//    ofstream fileContainingTheNameOfTheEncryptedBackupHandle;
//    openForWriting(globalString.fileContainingTheNameOfTheEncryptedBackupPath
//                   ,__FILE__,__LINE__
//                   ,fileContainingTheNameOfTheEncryptedBackupHandle,NEW_FILE);
//    fileContainingTheNameOfTheEncryptedBackupHandle
//                <<globalString.basePath+nameOfEncryptedBackup;
//    fileContainingTheNameOfTheEncryptedBackupHandle.close();

    createRestoreDirectory();
    string restorePath="$HOME/restored-from-CloudBuddy/"+nameOfEncryptedBackup;
    string temporaryPathToRestoreBackupIn="";
    createTemporaryPathToRestoreBackupIn(temporaryPathToRestoreBackupIn);

    ofstream scriptThatRestoresTheBackupHandle;
    openForWriting(scriptThatRestoresTheBackupPath,__FILE__,__LINE__,
                   scriptThatRestoresTheBackupHandle,NEW_FILE);
    scriptThatRestoresTheBackupHandle
    <<tab0<<"#!/bin/bash"<<endl<<endl;

    writeCleanUpAndExitFunction("restore", scriptThatRestoresTheBackupHandle);
    scriptThatRestoresTheBackupHandle

    /* capture Ctrl-C if the user presses it */
    <<tab0<<"trap cleanUpAndExit SIGINT"<<endl<<endl

//test three sftp attempts again.  ensure ctrl-c brings back to cursor
//  one blapity, one legit backup with path, one legit backup no path.

    /* go to the restore directory */
    <<tab0<<"cd "<<restoreDirectory<<endl<<endl

    /* download the backup */
    /* Create a script to retrieve the encrypted backup */
    // Use -vv in the next line if have any issues, as this shows lots of
    // debug info.  However when doing this, need to also comment out the line
    // "-o LogLevel=error " below  as it suppresses all warnings
    <<tab0<<"sftp "
    // Need the next line, otherwise the sftp fails with sudo
    <<"-o StrictHostKeyChecking=no "
    // The next line is needed to suppress the one time message of: "Warning:
    // Permanently added '<domain,ip>' (ECDSA) to the list of known hosts."
    <<"-o LogLevel=error "
    <<username<<"@"<<domain
    <<"<<END_SCRIPT"<<endl
    <<tab0<<"cd "<<directoryThatTheEncryptedBackupIsIn<<endl
    <<tab0<<"get "<<nameOfEncryptedBackup<<endl
    // The next line
    // creates an empty file named "transfer-complete" once the 'get' command
    // finishes successfully. The file won't be created if any of the following
    // failures occurred -
    // 1) An incorrect sftp server, sftp username or sftp password is entered     // tested successfully
    // 2) A Ctrl-C is entered during the transfer                                 // tested successfully
    // 3) The session hangs.  In this case the user will likely close the window  // not tested but confident would work
    // 4) There is an unknown error either on the server or on the local          // not tested but confident would work
    //    machine. In this case, the file wouldn't be created. The behavior
    //    would be the same as entering Ctrl-C as 'get' would have to finish in
    //    order for execution to resume to this line and create the file.  The
    //    reason for this is the sftp is in a script itself and if an error was
    //    encountered, execution would exit the script and not get a chance to
    //    create this file.
    <<tab0<<"!command touch transfer-complete"<<endl
    // The next line ends the sftp script
    <<tab0<<"END_SCRIPT"<<endl<<endl
    // delete when above works
    // this works except if the user cancels the download during the transfer
    ///* example)  sftp <user>@cloudbuddy.cloud:/uploads/testFile . */
    //<<tab0<<"sftp "<<username<<"@"<<domain<<":"<<nameOfEncryptedBackupWhichIncludesPath
    //<<" ."<<endl

    <<tab0<<"echo"<<endl<<endl
    /* If the backup to be downloaded wasn't found, the transfer-complete */
    /* file still gets created since the 'get' command still finished */
    /* successfully even though nothing was downloaded.  The */
    /* 'transfer-complete' file is checked here in case the transfer was */
    /* interrupted. */
    <<tab0<<"if [ ! -e \"transfer-complete\" ] || [ ! -e \""<<restoreDirectory
          <<nameOfEncryptedBackup<<"\" ]; then "<<endl
    <<tab1<<"echo Error: Unable to locate the backup '\""
          <<nameOfEncryptedBackupWhichIncludesPath<<"\"'"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"echo Exiting ..."<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"exit 1"<<endl
    <<tab0<<"fi"<<endl<<endl

    /* Kick off a background process that runs every second that displays the */
    /* ccrypt command progress */
    <<tab0<<"(while sleep 1; "
    <<"do "<<globalString.binaryThatShowsTheCcryptStatus<<" "<<purpose<<"; "
    <<"done & echo $! > "<<globalString.fileThatContainsTheCcryptStatusProcessId
    <<")"<<endl<<endl

    /* decrypt the backup */
    // Note: With the below decryption command, the temporary file created
    // (before encryption finishes) has a different suffix each time
    // so there will be two files when not done and one once decryption finishes.
    <<tab0<<"ccrypt -d -f -T "<<nameOfEncryptedBackup<<endl<<endl
    <<tab0<<"echo"<<endl<<endl

    <<tab0<<"while [ ! -f \""<<globalString.ccryptFinishedGracefully<<"\" ]; do"
    <<endl
    <<tab1<<"sleep 1"<<endl
    <<tab0<<"done"<<endl<<endl

    <<tab0<<"echo"<<endl

<<"exit 1"<<endl

    /* create destination directory that the backup will be restored in */
    <<tab0<<"mkdir -p "<<restorePath<<endl<<endl

    /* restore the backup to a temporary directory */
    /* example) tar jxvf l__12_16_2012__11_14pm -C "$HOME/Temp/restore of l__12_16_2012__11_14pm"*/
    <<tab0<<"tar jxvf "<<nameOfEncryptedBackup<<" -C "<<restorePath<<endl<<endl

    <<tab0<<"echo"<<endl
    <<tab0<<"GREEN_CHECK_MARK=\"\033[0;32m\xE2\x9C\x94\033[0m\""<<endl
    <<tab0<<"echo Restore complete  ${GREEN_CHECK_MARK}"<<endl
    <<tab0<<"echo Your backup was restored to "<<restorePath<<endl
    <<tab0<<"echo"<<endl
    ;

    scriptThatRestoresTheBackupHandle.close();
}

void createTemporaryPathToRestoreBackupIn
                                        (string &temporaryPathToRestoreBackupIn)
{
    string createTemporaryPathToRestoreBackupInCmd=
        "mkdir -p \"$HOME/Temp/restore of "+nameOfEncryptedBackup+"\"";
    //cout<<endl<<createTemporaryPathToRestoreBackupInCmd<<endl<<endl;
    if(system(createTemporaryPathToRestoreBackupInCmd.c_str()));
}

void createRestoreDirectory()
{
    string createRestoreDirectoryCmd="mkdir -p "+restoreDirectory;
    if (system(createRestoreDirectoryCmd.c_str()));
}

void runTheScriptThatRestoresTheBackup()
{
    string restoreTheBackupScriptCommand=
                                "/bin/bash "+scriptThatRestoresTheBackupPath;
    cout<<
        "\nBeginning the process to restore your backup: "
        <<nameOfEncryptedBackupWhichIncludesPath<<endl<<endl<<
        "Reaching out to the server, please hang on a few seconds ... (up to a "
        "minute)"<<endl<<endl;
    if(system(restoreTheBackupScriptCommand.c_str()));
}

//void extractPathToBackupAndnameOfEncryptedBackup(string &nameOfEncryptedBackupWhichIncludesPath)  // both if/else tested
//{
//    //dcout<<"\nPassed in = \n"<<nameOfEncryptedBackupWhichIncludesPath<<endl;
//    // the backup is at the base directory (the directory when sftp'ing in)
//    if (nameOfEncryptedBackupWhichIncludesPath.find('/') == string::npos)
//    {
//        nameOfEncryptedBackup = nameOfEncryptedBackupWhichIncludesPath;
//        directoryThatTheEncryptedBackupIsIn = ".";
//    }
//    // the backup is in a subdirectory
//    else
//    {
//        nameOfEncryptedBackup = nameOfEncryptedBackupWhichIncludesPath.substr
//                            (nameOfEncryptedBackupWhichIncludesPath.find_last_of('/')+1,
//                             nameOfEncryptedBackupWhichIncludesPath.length());
//
//        directoryThatTheEncryptedBackupIsIn = nameOfEncryptedBackupWhichIncludesPath.substr
//                           (0, nameOfEncryptedBackupWhichIncludesPath.find_last_of('/'));
//    }
//    //dcout<<"\ndirectoryThatTheEncryptedBackupIsIn = "<<directoryThatTheEncryptedBackupIsIn<<endl;
//    //dcout<<"\nnameOfEncryptedBackup = "<<nameOfEncryptedBackup<<endl<<endl;
//    //exit(EXIT_SUCCESS);
//}
