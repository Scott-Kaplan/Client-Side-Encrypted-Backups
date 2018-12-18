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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iomanip> // setw() - sets column width of a field
#include "../staticLibrary__fileUtilities/fileUtilities.h"
#include <unistd.h>


/****************************/
/***** Static Libraries *****/
/****************************/
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int lineNumber,
                               ifstream &readFileHandle);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int lineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
extern "C" string convertNumberToString(int number);
extern "C" void convert$HOME(string &path);
extern "C" bool fileExists(string &lookupFile,string &lookupFileResults);
extern "C" void deleteFile(string &filename);
extern "C" void getGlobalStrings(globalStringS &globalString, string &purpose);
extern "C" bool fileIsEmpty(string &path);
extern "C" string getStringUnder(int columnNumberWanted,
                                 int totalNumberOfColumns,
                                 string &lineToBeParsed);
extern "C" double getTotalLinesInFile(string &pathToCountLines);
//extern "C" long int getTotalLinesInFile(string &pathToCountLines);
extern "C" void retrieveUsernameAndDomain
            (string &username, string &domain, string &usernameAndDomainPath);
extern "C" void removeLeadingWhiteSpaces(string &line);
extern "C" void clearTheTerminalWindow();
extern "C" void writeCleanUpAndExitFunction
                                 (string &purpose, ofstream &scriptHandle);

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
string purpose="backup";  // for path $HOME/.cloudbuddy/backup
double storageSpaceOfFilesThatWillBeInTheBackup=0;
//long long int storageSpaceOfFilesThatWillBeInTheBackup=0;
globalStringS globalString;
string searchThisListForChangesPath =           "$HOME/.cloudbuddy/input/[1] search_this_list_for_changes";
//string usernameAndDomainPath =                  "$HOME/.cloudbuddy/input/[2] username_and_domain";
string landingDirectoryPath =                   "$HOME/.cloudbuddy/input/[3] landing_directory";
string computerNamePath =                       "$HOME/.cloudbuddy/input/[4] computer_name";
string dontBackupFilesThatStartWithThisPath =   "$HOME/.cloudbuddy/input/[5] don't_backup_files_that_start_with_this";
string dontBackupFilesThatEndWithThisPath =     "$HOME/.cloudbuddy/input/[6] don't_backup_files_that_end_with_this";
string dontBackupFilesThatContainThisPath =     "$HOME/.cloudbuddy/input/[7] don't_backup_files_that_contain_this";
string timeStampMarkerPath =                    "$HOME/.cloudbuddy/input/timeStampMarker";
string logOfCompletedBackupsPath =              "$HOME/.cloudbuddy/log/completed_backups";
string currentTimeStampPath =                   "$HOME/.cloudbuddy/backup/currentTimeStamp";
string changedAndNewFilesPath =                 "$HOME/.cloudbuddy/backup/changedAndNewFiles";
string filteredChangedAndNewFilesPath =         "$HOME/.cloudbuddy/backup/filteredChangedAndNewFiles";
string backupScriptPath =                       "$HOME/.cloudbuddy/backup/backupScript";
string remainingSizeInHomeDirectoryPath =       "$HOME/.cloudbuddy/backup/remainingSizeOnTheDrive";
string capturedTerminalOutput =                 "$HOME/.cloudbuddy/backup/capturedTerminalOutput";
string resultsOfTarCommand =                    "$HOME/.cloudbuddy/backup/resultsOfTarCommand";
string tarPercentageCompletePath =              "$HOME/.cloudbuddy/backup/tarPercentageCompletePath";
//string binaryThatShowsTheTarStatus =
//    "\"$HOME/codeBlockProjects/open source/actOnTarStatus/bin/Release/actOnTarStatus\"";
//string binaryThatShowsTheCcryptStatus =
//    "\"$HOME/codeBlockProjects/open source/actOnCcryptStatus/bin/Release/actOnCcryptStatus\"";


//string deleteTheOutputDirectoryContents =       "rm -f $HOME/.cloudbuddy/backup/*";
string computerName="";
string backupLabelName="";
//string expectedSizeOfTheOutputFileFromRunningTheTarCommand = "45";              // tested when 45 for all cases - including yes/no to continuing with the backup
string expectedSizeOfTheOutputFileFromRunningTheTarCommand = "44";              // this is the nominal case

// Note: In the next line, \\x60 escapes the back tick(`)
string expectedOutputFromRunningTheTarCommand =
                            "tar: Removing leading \\x60/' from member names";

/*******************************/
/***** Function Prototypes *****/
/*******************************/
//void retrieveUsernameAndDomain(string &username, string &domain);
void createEmptyFileIfItDoesntExist(string &path);
void displayUsage();
void clearTheOutputDirectory();
//void removeLeadingWhiteSpaces(string &line);
void filterUnwantedFilesSoThatTheyWontBeInTheBackup();
bool fileIsWantedBecauseBeginningOfPathIsOk(string &lineToKeepOrPitch);
bool fileIsWantedBecauseEntirePathIsOk(string &lineToKeepOrPitch);
bool fileIsWantedBecauseEndingPathIsOk(string &lineToKeepOrPitch);
void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[]);
void createTheConfigurationFilesIfTheyDontExist();
void retrieveTheLandingDirectory(string &landingDir);
void retrieveTheComputerName();
void checkThatTheComputerNameIsOk(string &computerName);
string createTheNameOfTheBackup(string &backupLabelName);
double getApproxSizeOfBackupBeforeTaringIt();
//long long int getApproxSizeOfBackupBeforeTaringIt();
void checkThatThereIsEnoughDiskSpaceToPerformTheBackup();
void createAListOfFilesThatHaveChangedOrAreNew();
void createAScriptThatWillPerformTheBackup();
void runTheScriptThatPeformsTheBackup();
double getRemainingSizeInHomeDir();
//long long int getRemainingSizeInHomeDir();
void saveTheTerminalPid();

/*********************/
/***** Functions *****/
/*********************/
int main(int argc, char * const argv[])
{
    //clearTheOutputDirectory();  // uncomment this
    getGlobalStrings(globalString,purpose);
    createTheConfigurationFilesIfTheyDontExist();
    clearTheOutputDirectory();
    saveTheTerminalPid();
    //deleteFile(logOfCompletedBackupsPath);  // todo: remove this line as it is only used for debugging
    clearTheTerminalWindow();
    retrieveTheComputerName();
    checkThatTheCommandLineArgumentsAreCorrect(argc,argv);
    createAListOfFilesThatHaveChangedOrAreNew();
    filterUnwantedFilesSoThatTheyWontBeInTheBackup();
    checkThatThereIsEnoughDiskSpaceToPerformTheBackup();
    createAScriptThatWillPerformTheBackup();
    runTheScriptThatPeformsTheBackup();
    //clearTheOutputDirectory();  // uncomment this
    return 0;
}

void clearTheOutputDirectory()
{
    //cout<<endl<<globalString.deleteOnlyFilesInSubDirectory.c_str()<<endl<<endl;
    if(system(globalString.deleteOnlyFilesInSubDirectory.c_str()));
    //if(system(deleteTheOutputDirectoryContents.c_str()));
}

void saveTheTerminalPid()
{
    /* the purpose of this function is to have the ability to check whether */
    /* the user closed the terminal window during an encryption/decryption. */
    /* In that case, the background process showing the encryption/decryption */
    /* status will get canceled (killed) */
    ostringstream terminalPidStream;
    terminalPidStream<<getppid()<<endl;
    ofstream terminalPidHandle;
    openForWriting(globalString.processIdOfThisTerminalSessionPath,
                   __FILE__,__LINE__,terminalPidHandle,NEW_FILE);
    terminalPidHandle<<terminalPidStream.str()<<endl;
    terminalPidHandle.close();
}

void filterUnwantedFilesSoThatTheyWontBeInTheBackup()
{
    string lineToKeepOrPitch;

    /* This contains all files that are new or have been modified since the */
    /* last backup.  Some of these we don't want to backup.  The filtering */
    /* code that occurs below will toss out the files that you don't want */
    /* backed up */
    ifstream analyzeFilesToKeepHandle;
    openForReading(changedAndNewFilesPath,
                   __FILE__,
                   __LINE__,
                   analyzeFilesToKeepHandle);

    /* this will contain only the files that we want to backup*/
    ofstream filteredChangedAndNewFilesHandle;
    openForWriting(filteredChangedAndNewFilesPath,
                   __FILE__,
                   __LINE__,
                   filteredChangedAndNewFilesHandle,
                   NEW_FILE);

    /* This is the loop that filters out all of the files that you don't want */
    /* in the backup and retains only the ones that you do want to backup */
    while (getline(analyzeFilesToKeepHandle,lineToKeepOrPitch))
    {
        //dcout<<"lineToKeepOrPitch = \n"<<lineToKeepOrPitch<<endl;
        if (fileIsWantedBecauseBeginningOfPathIsOk(lineToKeepOrPitch) &&
            fileIsWantedBecauseEndingPathIsOk(lineToKeepOrPitch) &&
            fileIsWantedBecauseEntirePathIsOk(lineToKeepOrPitch))
        {
            /* the file made the cut, so we want it in the backup. */
            //dcout<<"retain"<<endl;
            filteredChangedAndNewFilesHandle<<lineToKeepOrPitch<<endl;
        }
        else
        {
            /* discard the file because we do not want it in the backup*/
            //dcout<<"Skip this file so it won't be a part of the backup\n"
            //     <<lineToKeepOrPitch<<endl;
            //dcout<<"discard"<<endl;
        }
    }
    analyzeFilesToKeepHandle.close();
    filteredChangedAndNewFilesHandle.close();

    /* since we already have the filtered list of files that we want to backup*/
    /* delete the unfiltered list of new and changed files that we started with*/
    deleteFile(changedAndNewFilesPath);

    if (fileIsEmpty(filteredChangedAndNewFilesPath))
    {
        cout<<"\nAfter filtering, there were no files found that "
              "needed to be backed up.  Exiting ...\n"<<endl;
        exit(EXIT_SUCCESS);
    }

    cout<<endl<<startUnderline<<"Files found (after filtering)"<<endUnderline
    <<endl

//    cout<<endl
//    <<"Files Found (after filtering)"<<endl
//    <<"-----------------------------"<<endl
    <<getTotalLinesInFile(filteredChangedAndNewFilesPath)<<endl;
}

bool fileIsWantedBecauseBeginningOfPathIsOk(string &lineToKeepOrPitch)
{
    //dcout<<"\nBeginning string filter check-\nThis file\n"<<lineToKeepOrPitch<<endl;

    createEmptyFileIfItDoesntExist(dontBackupFilesThatStartWithThisPath);

    bool lineWanted = true;
    int lengthOfString = 0;
    string line="";
    ifstream dontBackupFilesThatStartWithThisHandle;
    openForReading(dontBackupFilesThatStartWithThisPath,
                   __FILE__,
                   __LINE__,
                   dontBackupFilesThatStartWithThisHandle);
    while(getline(dontBackupFilesThatStartWithThisHandle,line))
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            lengthOfString = strlen(line.c_str());
            if (!(strncmp(line.c_str(),
                          lineToKeepOrPitch.c_str(),
                          lengthOfString)))
            {
                //dcout<<"will not be included in the backup\nbecause \""
                //d    <<line<<"\" was contained at the beginning of the string"<<endl;
                /* This file won't be backed up because it starts with a string */
                /* contained in [5] don't_backup_files_that_start_with_this */
                lineWanted = false;
                break;
            }
        }
    }
    //dif (lineWanted) cout<<"so far will be included in the backup"<<endl;
    dontBackupFilesThatStartWithThisHandle.close();
    return lineWanted;
}

bool fileIsWantedBecauseEndingPathIsOk(string &lineToKeepOrPitch)
{
    //dcout<<"\nEnding string filter check-\nThis file\n"<<lineToKeepOrPitch<<endl;
    bool lineWanted = true;
    int lengthOfString = 0;
    int lengthOfLineToKeepOrPitch = 0;
    int offset = 0;
    string line;
    ifstream dontBackupFilesThatEndWithThisHandle;

    openForReading(dontBackupFilesThatEndWithThisPath,
                   __FILE__,
                   __LINE__,
                   dontBackupFilesThatEndWithThisHandle);

    while(getline(dontBackupFilesThatEndWithThisHandle,line))
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            lengthOfString = strlen(line.c_str());
            lengthOfLineToKeepOrPitch = strlen(lineToKeepOrPitch.c_str());
            offset = lengthOfLineToKeepOrPitch - lengthOfString;
            if (!(strcmp(line.c_str(),&lineToKeepOrPitch.c_str()[offset])))
            {
                //dcout<<"will not be included in the backup\nbecause \""
                //d    <<line<<"\" was contained at the end of the string"<<endl;
                /* This file won't be backed up because it ends with a string */
                /* contained in [6] don't_backup_files_that_end_with_this */
                lineWanted = false;
                break;
            }
        }
    }
    //dif (lineWanted) cout<<"so far will be included in the backup"<<endl;
    dontBackupFilesThatEndWithThisHandle.close();
    return lineWanted;
}

bool fileIsWantedBecauseEntirePathIsOk(string &lineToKeepOrPitch)
{
    //dcout<<"\nEntire string filter check-\nThis file\n"<<lineToKeepOrPitch<<endl;
    bool lineWanted = true;
    const char * pch;
    string line="";
    ifstream dontBackupFilesThatContainThisHandle;

    openForReading(dontBackupFilesThatContainThisPath,
                   __FILE__,
                   __LINE__,
                   dontBackupFilesThatContainThisHandle);
    while(getline(dontBackupFilesThatContainThisHandle,line))
    {
        //cout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            pch = strstr(lineToKeepOrPitch.c_str(),line.c_str());
            if (pch != NULL)
            {
                //dcout<<"will not be included in the backup\nbecause \""
                //d    <<line<<"\" was contained in the string"<<endl;
                /* This file won't be backed up because it contains a string */
                /* found in [7] don't_backup_files_that_contain_this */
                lineWanted = false;
                break;
            }
        }
    }
    //dif (lineWanted) cout<<"will be included in the backup"<<endl;
    dontBackupFilesThatContainThisHandle.close();
    return lineWanted;
}

void createEmptyFileIfItDoesntExist(string &path)
{
    convert$HOME(path);
    //getGlobalStrings(globalString,purpose);
    string lookupFileResults=globalString.basePath+"FoundResults";
    //string lookupFileResults=globalString.outputDirectoryPath+"FoundResults";
    if (!fileExists(path,lookupFileResults))
    {
        string createFileCmd="touch \""+path+"\"";
        //dcout<<"createFileCmd = "<<createFileCmd<<endl;
        if(system(createFileCmd.c_str()));
    }
}

void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[])
{
    if (argc != 2)
    {
        /* display usage message because the user didn't enter the correct */
        /* amount of parameters - which  was supposed to be (1) */
        displayUsage();
    }
    else
    {
        /* the expected number of parameters (1) was passed in, so continue */
        /* with checking the validity of those parameters. */
        backupLabelName = argv[1];

        /* display what the user typed in to launch*/
        cout<<"> backup "<<backupLabelName<<endl;

        if (backupLabelName != "no-label")
        {
            /* check to make sure that the backup label name isn't too long */
            if (backupLabelName.length() >= 128)
            {
                cout<<"Error: the length of the second parameter can't exceed "
                      "128"<<endl;
                displayUsage();
            }

            /* check that the backup label name doesn't contain illegal */
            /* symbols */
            for (unsigned int i=0;i<backupLabelName.length();++i)
            {
                if (!((isalnum(backupLabelName[i])) ||
                      (backupLabelName[i]=='-') ||
                      (backupLabelName[i]=='_') ||
                      (backupLabelName[i]=='.')))
                {
                    //cout<<"Error: Only letters, numbers, hyphens and underscores "
                    cout<<"Error: Only letters, numbers, periods, hyphens and "
                          "underscores can be used in label-name|no-label"
                          <<endl;
                    displayUsage();
                }
            }
        }
    }
}

void displayUsage()
{
    //todo: retest this as moved around the words to fit withing column of 80
    cout
    <<"\nUsage:\n\nbackup "<<startUnderline
    <<"label-name|no-label\n\n"<<endUnderline
    <<startUnderline<<"label-name|no-label"<<endUnderline<<" --> This is a"
    " parameter that you need to provide which makes\nup part of your backup"
    " name. If you don't want a label in your backup name,\nuse: no-label. If"
    " you are using a label, you can use anything as long as it\nis made up"
    " from one or more of these:\ncharacters, numbers, periods, hyphens,"
    " underscores and has a length less than 128.\n\nExamples\n  >"
    " backup no-label\n  This is how it will look on the server"
    " (Note: The timestamp will be different)\n  "<<computerName<<
    "**2018-05-01__12:28am\n\n  > backup pics-of-aunt-mary\n "
    " This is how it will look on the server (Note: The timestamp will be"
    " different)\n  "<<computerName<<"**pics-of-aunt-mary**2018-05-01__12:28am"
    "\n"<<endl;
    exit(EXIT_SUCCESS);
}

void createTheConfigurationFilesIfTheyDontExist()
{
    //cout<<endl<<"0"<<endl;
    /* This program relies on there at least being an empty file for each of */
    /* these.  Unique error messages on what is necessary out of each will be */
    /* displayed to the user if they occur. */
    createEmptyFileIfItDoesntExist(searchThisListForChangesPath);
    createEmptyFileIfItDoesntExist(globalString.usernameAndDomainPath);
    createEmptyFileIfItDoesntExist(landingDirectoryPath);
    createEmptyFileIfItDoesntExist(computerNamePath);
    createEmptyFileIfItDoesntExist(dontBackupFilesThatStartWithThisPath);
    createEmptyFileIfItDoesntExist(dontBackupFilesThatEndWithThisPath);
    createEmptyFileIfItDoesntExist(dontBackupFilesThatContainThisPath);
    createEmptyFileIfItDoesntExist(timeStampMarkerPath);
}

void retrieveTheLandingDirectory(string &landingDirectory)
{
    bool landingDirectoryFound = false;
    string line="";
    ifstream landingDirectoryHandle;
    openForReading(landingDirectoryPath,
                   __FILE__,
                   __LINE__,
                   landingDirectoryHandle);
    while (getline(landingDirectoryHandle,line))
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            landingDirectoryFound = true;
            landingDirectory = line;
        }
    }
    landingDirectoryHandle.close();
    if (!landingDirectoryFound)
    {
        cout<<
            "\nError: Unable to extract the landing directory on the sftp  "
            "server that you want to send the backup to.\nTo fix, please see "
            "this file -\n"
            "$HOME/.cloudbuddy/input/[3] landing_directory\n"
        <<endl;
        exit(EXIT_SUCCESS);
    }
}

void retrieveTheComputerName()
{
    bool computerNameFound = false;
    string line="";
    ifstream computerNameHandle;
    openForReading(computerNamePath,
                   __FILE__,
                   __LINE__,
                   computerNameHandle);
    while (getline(computerNameHandle,line))
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            computerNameFound = true;
            computerName = line;
        }
    }
    computerNameHandle.close();
    if (!computerNameFound)
    {
        cout<<
            "\nError: Unable to extract the computer name.  This is needed "
            "as it will be used to name your backup.\nTo fix, please see "
            "this file -\n"
            "$HOME/.cloudbuddy/input/[4] computer_name\n"
        <<endl;
        exit(EXIT_SUCCESS);
    }
    checkThatTheComputerNameIsOk(computerName);
}

void checkThatTheComputerNameIsOk(string &computerName)
{
    /* check to make sure that the computer name length isn't too long */
    if (computerName.length() >= 32)
    {
        cout<<"Error: the length of the computer name in the file \"[4] "
            <<"computer_name\" can't exceed 32"<<endl;
        displayUsage();
    }

    /* check that the computer name doesn't contain illegal symbols */
    for (unsigned int i=0;i<computerName.length();++i)
    {
        //dcout<<endl<<computerName<<endl<<endl;
        // source: http://www.cplusplus.com/reference/cctype/isalnum/
        if (!((isalnum(computerName[i])) ||
              (computerName[i]=='-') ||
              (computerName[i]=='_') ||
              (computerName[i]=='.')))
        {
            cout<<"Error: Only letters, numbers, periods, hyphens and "
                  "underscores can be used in from-computer-name"<<endl;
            displayUsage();
        }
    }
}

string createTheNameOfTheBackup(string &backupLabelName)
{
    /* format the current timestamp */
    string timestampCmd="date +%Y-%m-%d__%I:%M%P > "+currentTimeStampPath;
    if (system(timestampCmd.c_str()));

    /* extract the formatted timestamp*/
    ifstream timeStampHandle;
    openForReading(currentTimeStampPath,
                   __FILE__,
                   __LINE__,
                   timeStampHandle);
    string timeStamp="";
    getline(timeStampHandle,timeStamp);
    timeStampHandle.close();
    deleteFile(currentTimeStampPath);

    /* set the name of the backup as it will appear on the server after the */
    /* sftp transfer */
    string theBackup="";
    if (backupLabelName == "no-label")
    {
        theBackup = computerName+"**"+timeStamp;
    }
    else
    {
        theBackup = computerName+"**"+backupLabelName+"**"+timeStamp;
    }

    return theBackup;
}

double getApproxSizeOfBackupBeforeTaringIt()
//long long int getApproxSizeOfBackupBeforeTaringIt()
// source: https://stackoverflow.com/questions/5840148/how-can-i-get-a-files-size-in-c
{
    ifstream listOfFilesToBeTardHandle;
    openForReading(filteredChangedAndNewFilesPath,
                   __FILE__,
                   __LINE__,
                   listOfFilesToBeTardHandle);
    string line="";
    FILE *p_file = NULL;
    double size = 0;
//    long long int size = 0;
    while (getline(listOfFilesToBeTardHandle,line))
    {
        p_file = fopen(line.c_str(),"rb");
        fseek(p_file,0,SEEK_END);
        size += ftell(p_file);
        fclose(p_file);
    }
    listOfFilesToBeTardHandle.close();
    //std::cout.imbue(std::locale(""));
    //cout<<endl<<"The approximate size of your backup prior to tar'ing it is "<<size<<" bytes."<<endl;

    return size;
}

void createAListOfFilesThatHaveChangedOrAreNew()
{
    cout<<"\nCalculating the files that have changed or that are new, please "
          "hang on a few seconds ... (up to a minute)"<<endl;

    string line="";
    string findChangedAndNewFilesCmd="";
    string issueFTPCommand="";
    string analyzeFileToKeepline;
    ifstream searchThisListForChangesHandle;
    deleteFile(changedAndNewFilesPath); // Create this file from scratch below

    /* check that the list of directories/files to monitor is not empty */
    if (fileIsEmpty(searchThisListForChangesPath))
    {
        cout<<"\nError: The file "<<searchThisListForChangesPath<<" can't be "
              "blank.\n"<<endl;
        exit(EXIT_SUCCESS);
    }

    /* this creates a file with a list of all files that have changed since   */
    /* the last backup was performed.  The filtering of what needs to be      */
    /* actually backed up occurs below this.                                  */
    openForReading(searchThisListForChangesPath,
                   __FILE__,
                   __LINE__,
                   searchThisListForChangesHandle);

    double totalLinesInConfigFile1 =
                            getTotalLinesInFile(searchThisListForChangesPath);

    cout<<endl<<startUnderline<<"Files found (before filtering)"<<endUnderline
    <<"    "<<startUnderline<<"Files searched"<<endUnderline<<endl;
    for (int i=0;getline(searchThisListForChangesHandle,line);++i)
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            findChangedAndNewFilesCmd=
                "find "+
                line+
                " -path '../.*' -prune -o -type f -newer "+
                timeStampMarkerPath+
                " -print >> "+
                changedAndNewFilesPath;
            //dcout << findChangedAndNewFilesCmd << endl;
            if(system(findChangedAndNewFilesCmd.c_str()));
            line.clear(); // erases contents of line

            double linesInConfigFile1PercentageSearched =
                                    ((double)(i+1)/totalLinesInConfigFile1)*100;
            ostringstream percentageSearched;
            percentageSearched<<fixed<<setprecision(4)
                              <<linesInConfigFile1PercentageSearched<<"%";
            cout
            <<" "
            <<left<<setw(34)<<getTotalLinesInFile(changedAndNewFilesPath)
            <<left<<setw(18)<<percentageSearched.str()
            <<"     \r";
            cout.flush();
            //do not delete this next line.  When uncommented it shows every
            //percentage line one after another
            //cout<<endl;
        }
    }

    cout
    << left << setw(34)<<getTotalLinesInFile(changedAndNewFilesPath)
    << left << setw(18)<<"100%"
    << "     \r";
    cout.flush();
    cout<<endl;

    searchThisListForChangesHandle.close();
    if (fileIsEmpty(changedAndNewFilesPath))
    {
        cout<<"\nNo files were found that needed to be backed up.  Exiting.\n"
            <<endl;
    }
}

void checkThatThereIsEnoughDiskSpaceToPerformTheBackup()
{
    // In order to be able to perform the backup, a temporary storage space of
    // 2x the size of all of the files combined to be backed up is needed.
    // The first factor is the size of the tar ball representing all the files in the backup
    // The second factor is the size of that tar ball being encrypted
    // example)
    // Lets say all of the files combined that will go in this backup is 100MB.
    // So, you will additionally need 2x this, which is 200MB.  This extra space
    // needed is only temporary.  As soon as the backup is transfered, the 2x
    // extra storage space is immediately freed.

    int storageSpaceFactorNeeded = 2;

    storageSpaceOfFilesThatWillBeInTheBackup =
                                        getApproxSizeOfBackupBeforeTaringIt();
    ofstream projectedSizeOfTheTarBallHandle;
    openForWriting(globalString.projectedSizeOfTheTarBallPath,__FILE__,__LINE__,
                   projectedSizeOfTheTarBallHandle,NEW_FILE);
    projectedSizeOfTheTarBallHandle<<storageSpaceOfFilesThatWillBeInTheBackup
                                                                        <<endl;
    projectedSizeOfTheTarBallHandle.close();
    double sizeRemainingInHomeDir = getRemainingSizeInHomeDir();
    //long long int sizeRemainingInHomeDir = getRemainingSizeInHomeDir();
    double spaceNeededToPerformTheBackup =
    //long long int spaceNeededToPerformTheBackup =
            (storageSpaceOfFilesThatWillBeInTheBackup*storageSpaceFactorNeeded);

    std::cout.imbue(std::locale(""));
    //dcout<<"The space needed to perform the backup = "<<spaceNeededToPerformTheBackup<<endl;
    //dcout<<"The space remaining in you home directory = "<<sizeRemainingInHomeDir<<endl;
    if (spaceNeededToPerformTheBackup > sizeRemainingInHomeDir)
    {
        cout<<endl<<"Sorry but there isn't enough disc space in your Home"
              " directory to perform a backup."<<endl<<"Your Home directory"
              " needs to at least have "
              <<spaceNeededToPerformTheBackup<<" bytes available."<<endl<<endl;
        exit(EXIT_SUCCESS);
    }
    /* for debug.  Use for testing, then delete
    else
    {
        cout<<endl<<"total size in bytes left in home directory = "<<sizeRemainingInHomeDir<<endl;
    }
    */
}

void createAScriptThatWillPerformTheBackup()
{
    //getGlobalStrings(globalString,purpose);

    /* extract the ssh server information so a backup can be sent to it  */
    string username="";
    string domain="";
    retrieveUsernameAndDomain(username,domain,globalString.usernameAndDomainPath);
    //dcout<<username<<endl<<domain<<endl<<endl;
//cout<<endl<<"here"<<endl<<endl;
    string landingDirectory="";
    retrieveTheLandingDirectory(landingDirectory);
    //dcout<<landingDirectory<<endl;

    /* create input to the tar command with filtered changed and new files */
    string lineRead="";
    ifstream filteredChangedAndNewFilesHandle;
    openForReading(filteredChangedAndNewFilesPath,
                   __FILE__,
                   __LINE__,
                   filteredChangedAndNewFilesHandle);
    string filteredChangedAndNewFilesIntoTarCmd="";
    while (getline(filteredChangedAndNewFilesHandle,lineRead))
    {
        filteredChangedAndNewFilesIntoTarCmd += (" \"" + lineRead + "\"");
    }
    filteredChangedAndNewFilesHandle.close();

    string theBackup = createTheNameOfTheBackup(backupLabelName);

    /* write the backup path to a file so that the actOnTarStatus binary has */
    /* access to it */
    ofstream fileContainingTheNameOfTheBackupWithPathHandle;
    openForWriting(globalString.fileContainingTheBackupTarBallPath
                   ,__FILE__,__LINE__
                   ,fileContainingTheNameOfTheBackupWithPathHandle,NEW_FILE);
    fileContainingTheNameOfTheBackupWithPathHandle
                                <<globalString.basePath+theBackup;
//                                <<globalString.outputDirectoryPath+theBackup;
    fileContainingTheNameOfTheBackupWithPathHandle.close();

    /* Create a bash script that transfers the encrypted backup to a ssh */
    /* via sftp*/
    ofstream backupScriptHandle;
    openForWriting(backupScriptPath,
                   __FILE__,
                   __LINE__,
                   backupScriptHandle,
                   NEW_FILE);
    backupScriptHandle
    <<tab0<<"#!/bin/bash"<<endl<<endl;

    /* save the process Id of this terminal session */
    //<<tab0<<"echo $$ > "<<globalString.processIdOfThisTerminalSessionPath<<endl;

//cout<<endl<<"here"<<endl<<endl;
    writeCleanUpAndExitFunction(purpose, backupScriptHandle);

    /* Create a function that gives the user an option to quit */
    backupScriptHandle
    <<tab0<<"giveContinuationOptionToTheUser()"<<endl
    <<tab0<<"{"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"nontruncatedMessage=$@"<<endl
    <<tab1<<"read -p \"$nontruncatedMessage [y/n] \" -n 1 -r"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"if [[ ! $REPLY =~ ^[Yy]$ ]]; then"<<endl
    <<tab2<<"cleanUpAndExit"<<endl
    <<tab1<<"fi"<<endl
    <<tab0<<"}"<<endl<<endl

    /* capture Ctrl-C if the user presses it */
    <<tab0<<"trap cleanUpAndExit SIGINT"<<endl<<endl

    /* Go to the output directory */
    <<tab0<<"cd "<<globalString.basePath<<endl

    /* Display the list of files that will be backed up */
    <<tab0<<"echo"<<endl
    <<tab0<<"echo \""<<startUnderline
    <<"Here is the list of files that will be backed up:"<<endUnderline<<"\""
    <<endl
    <<tab0<<"while read line; do echo \"$line\"; done < "
    <<filteredChangedAndNewFilesPath<<endl<<endl

    /* Ask the user if the list of files that appear look to be correct as */
    /* these will be in the backup. Abort if the user answers no */
    <<tab0<<"message=\"Are you good with the above list of files being backed up?\""
    <<endl
    <<tab0<<"giveContinuationOptionToTheUser $message"<<endl<<endl

    /* tar the files that will be in the backup.  Do this in the background */
    /* so the tar command progress (percentage complete) can be given in the */
    /* foreground */
    // example
    // (tar jcf e6230-primary**2018-07-16__06:37pm --force-local
    // "/home/scott/temp/test/test/testTarBall" >
    // $HOME/.cloudbuddy/backup/resultsOfTarCommand 2>&1 &
    // echo $! > $HOME/.cloudbuddy/backup/fileThatContainsTheTarProcessId)
    <<tab0<<"(tar jcf "<<theBackup
    // This next line overides the limitation of not being allowed to include
    // one or more colons in the name of the tarball
    <<" --force-local"
    <<filteredChangedAndNewFilesIntoTarCmd<<" > "<<resultsOfTarCommand<<" 2>&1"
    // This next line saves the tar Process Id from executing the tar command.
    // This is needed by the 'progress' program in order to report the tar
    // percentage complete as the tar command is being executed.  When creating
    // large tarballs the pecentage complete is very useful, otherwise the user
    // just sees the command hanging and doesn't really know when it will finish
    <<" & echo $! > "<<globalString.fileThatContainsTheTarProcessId<<")"
    <<endl<<endl

    <<tab0<<"echo"<<endl
    <<tab0<<"echo \""<<startUnderline
    <<"Progress of creating a tarball of your backup"<<endUnderline<<"\""<<endl
    //<<tab0<<"echo"<<endl
    //<<tab0<<"echo \"Progress of creating a Tarball of your backup\""<<endl
    //<<tab0<<"echo \"---------------------------------------------\""<<endl

    /* hide the cursor as the cursor is distracting while viewing the */
    /* progress meter */
    <<tab0<<"tput civis"<<endl<<endl

    <<tab0<<"(while sleep 1; "
    <<"do "<<globalString.binaryThatShowsTheTarStatus<<" "<<purpose<<"; "
//    <<"do "<<binaryThatShowsTheTarStatus<<"; "
    <<"done & echo $! > "<<globalString.fileThatContainsTheTarStatusProcessId
    <<")"<<endl<<endl

    /* get the total backup from running tar once it has finished */
    <<tab0<<"while sleep 1; do"<<endl

    /* check that the file theTarCommandIsDone exists.  If so, anaylze the */
    /* output of the tar command */
    <<tab1<<"if [ -e \""<<globalString.theTarCommandIsDone<<"\" ]; then"<<endl

    /* bring the cursor back now that tar is done */
    <<tab2<<"tput cnorm"<<endl

    /* Get the size of the output from tar.  If it is different than expected */
    /* which is "Removing leading `/' from member names", as the user whether */
    /* he/she wants to proceed with the backup.  If it's the same display,    */
    /* 100% complete */
    <<tab2<<"sizeOfFile=$(stat -c%s "<<resultsOfTarCommand<<")"<<endl
    //<<"echo $sizeOfFile"<<endl
    //<<"echo \"Size = "<<"$sizeOfFile bytes.\""

    <<tab2<<"if [ $sizeOfFile -ne "
          <<expectedSizeOfTheOutputFileFromRunningTheTarCommand<<" ]; then"
          <<endl
    <<tab3<<"echo"<<endl
    <<tab3<<"echo $sizeOfFile"<<endl
    <<tab3<<"echo"<<endl
    <<tab3<<"echo"<<endl
    <<tab3<<"echo Warning: Normally, the output from running the tar command "
          <<"would have been -"<<endl
    <<tab3<<"echo "<<startUnderline<<endl
    <<tab3<<"printf \""<<expectedOutputFromRunningTheTarCommand<<"\""<<endl
    <<tab3<<"echo "<<endUnderline<<endl
    <<tab3<<"echo "<<endl
    <<tab3<<"echo "<<endl
    <<tab3<<"echo But, instead it was - "<<endl
    <<tab3<<"echo "<<startUnderline<<endl
    <<tab3<<"while read line; do echo \"$line\"; done < "<<resultsOfTarCommand<<endl
    <<tab3<<"echo "<<endUnderline<<endl
    <<tab3<<"message2=\"Do you still want to continue with the backup?\""<<endl
    <<tab3<<"giveContinuationOptionToTheUser $message2"<<endl
    <<tab2<<"else"<<endl
    // this next line is the setting for a green checkmark
    <<tab3<<"GREEN_CHECK_MARK=\"\033[0;32m\xE2\x9C\x94\033[0m\""<<endl
    // Note: the next line with the \\r brings cursor back to the beginning
    // and the 2 extra spaces overwrites the % in case the last progress output
    // was 100.00%, otherwise would have Done checkmark%
    <<tab3<<"echo -e \"\\rDone ${GREEN_CHECK_MARK}     \""<<endl
    <<tab3<<"echo"<<endl
    <<tab2<<"fi"<<endl

    <<tab2<<"break"<<endl
    <<tab1<<"fi"<<endl // done if the file "theTarCommandIsDone"

    //<<"echo done with tar command"<<endl
    <<tab0<<"done"<<endl<<endl

    /* Save the total size of all of the files to be encrypted.  This is */
    /* needed as an input to being able to calculate the percentage complete */
    /* of any ongoing encryption or decryption of a backup */
    // get the size of a file in bash - source https://unix.stackexchange.com/questions/16640/how-can-i-get-the-size-of-a-file-in-a-bash-script
    <<tab0<<"stat --printf=\"%s\" "<<theBackup<<" > sizeOfTheBackupThatsToBeEncrypted"<<endl<<endl

    /* hide the cursor as the cursor is distracting while viewing the */
    /* progress meter */
    <<tab0<<"tput civis"<<endl<<endl

    /* Kick off a background process that runs every second that displays the */
    /* ccrypt command progress */
//<<"echo "<<globalString.binaryThatShowsTheCcryptStatus<<endl
    <<tab0<<"(while sleep 1; "
    <<"do "<<globalString.binaryThatShowsTheCcryptStatus<<" "<<purpose<<"; "
    <<"done & echo $! > "<<globalString.fileThatContainsTheCcryptStatusProcessId
    <<")"<<endl<<endl

    /* Encrypt the backup */
    <<tab0<<"echo \"About to start encrypting your backup ...\""<<endl
    <<tab0<<"echo"<<endl<<"ccrypt -f -T "<<theBackup<<endl<<endl

    /* Ensure encryption password that the user entered twice matches.  If it */
    /* does not, mention it and exit the script */
    <<tab0<<"if [ ! -f \""<<theBackup<<".cpt\" ]; then "<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"echo \"Since the passwords don't match, can't proceed - so exiting ...\""<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"exit 1"<<endl
    <<tab0<<"fi"<<endl<<endl
    <<tab0<<"while [ ! -f \""<<globalString.ccryptFinishedGracefully<<"\" ]; do"
    <<endl
    <<tab1<<"sleep 1"<<endl
    <<tab0<<"done"<<endl<<endl

    /* bring the cursor back now that ccrypt is done */
    <<tab2<<"tput cnorm"<<endl

/*
    // since ccrypt is run in the foreground, making it here means that
    // encrypting the background completed successfully, so print done
    <<tab0<<"GREEN_CHECK_MARK=\"\033[0;32m\xE2\x9C\x94\033[0m\""<<endl
    // Note: the next line with the \\r brings cursor back to the beginning
    // and the 2 extra spaces overwrites the % in case the last progress output
    // was 100.00%, otherwise would have Done checkmark%
    <<tab0<<"echo -e \"\\rDone ${GREEN_CHECK_MARK}     \""<<endl
    <<tab0<<"echo"<<endl<<endl
*/
    // for debug only
    //backupScriptHandle<<"echo \""<<startUnderline<<"Size of the backup (after tar'ing and ecnrypting it)"<<endUnderline<<"\""<<endl;

    /* List the size of the backup */
    <<tab0<<"echo"<<endl
    <<tab0<<"echo \""<<startUnderline<<"Size of the backup"<<endUnderline<<"\""<<endl
    <<tab0<<"tree --du -ah "<<theBackup<<".cpt | grep \" used in \" | colrm 6 "
    // This next line trims the leading whitespaces off of the output
    <<"| sed -e 's/^[ \t]*//'"<<endl<<endl

    /* Remove the natural ccrypt extension off of the backup */
    <<tab0<<"mv "<<theBackup<<".cpt "<<theBackup<<endl

    /* Let the user know that the script is about to start running.  Let the */
    /* user know it could take a short while to start for the ssh server to */
    /* respond to the request */
    <<tab0<<"echo"<<endl<<endl
    <<tab0<<"echo \""<<startUnderline<<"About to start the transfer"<<endUnderline
    <<"\""<<endl
    <<tab0<<"echo \"Please hang on a few seconds (up to a minute) ... while the sftp server is contacted\""<<endl
    //<<tab0<<"echo \"Please hang on a few seconds ... (up to a minute)\""<<endl
    <<tab0<<"echo"<<endl

    /* Create a script to transfer the encrypted backup to a destination */
    /* machine */
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
    <<tab0<<"cd "<<landingDirectory<<endl
    <<tab0<<"put "<<theBackup<<endl
    // The next line
    // creates an empty file named "transfer-complete" once the 'put' command
    // finishes successfully. The file won't be created if any of the following
    // failures occurred -
    // 1) An incorrect sftp server, sftp username or sftp password is entered     // tested successfully
    // 2) A Ctrl-C is entered during the transfer                                 // tested successfully
    // 3) The session hangs.  In this case the user will likely close the window  // not tested but confident would work
    // 4) There is an unknown error either on the server or on the local          // not tested but confident would work
    //    machine. In this case, the file wouldn't be created. The behavior
    //    would be the same as entering Ctrl-C as 'put' would have to finish in
    //    order for execution to resume to this line and create the file.  The
    //    reason for this is the sftp is in a script itself and if an error was
    //    encountered, execution would exit the script and not get a chance to
    //    create this file.
    <<tab0<<"!command touch transfer-complete"<<endl
    // The next line ends the sftp script
    <<tab0<<"END_SCRIPT"<<endl<<endl

    /* If the transfer didn't finish, display an error message and exit */
    <<tab0<<"if [ ! -f \"transfer-complete\" ]; then "<<endl

    <<tab1<<"echo"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"echo \"Error: The transfer didn't finish, so exiting ...\""<<endl
//    <<tab1<<"echo \""<<startUnderline
//    <<tab1<<"Error: The transfer didn't finish - so exiting ..."<<endUnderline
//    <<tab1<<"\""<<endl

    <<tab1<<"echo"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"exit 1"<<endl
    <<tab0<<"fi"<<endl<<endl

    /* Refresh the timestamp of the timeStampMarker file, so the next backup */
    /* can only possibly contain files since then. */
    <<tab0<<"touch "<<timeStampMarkerPath<<endl

    /* Save a list of all files that made it into this backup to a log file */
    /* so the user can know anytime in the future what was backed up */
    <<tab0<<"echo \"=> Contents of "<<theBackup<<" - \" >> "
    <<logOfCompletedBackupsPath<<endl<<endl
    <<tab0<<"more "<<filteredChangedAndNewFilesPath<<" >> "
    <<logOfCompletedBackupsPath<<endl
    <<tab0<<"echo >> "<<logOfCompletedBackupsPath<<endl
    <<tab0<<"echo"<<endl

    /* Let the user know that the backup completed successfully with a note */
    /* of where they can find the log listing the files that were backed up */
    <<tab0<<"echo \""<<startUnderline<<"The backup is complete"<<endUnderline
    <<"\""<<endl
    <<tab0<<"echo \""<<"The log file ("<<logOfCompletedBackupsPath
    <<") shows a list of all files that were backed up."<<"\""<<endl<<endl
    <<tab0<<"cleanUpAndExit"<<endl;

    /* end the backup script */
    backupScriptHandle.close();

    //debug only - can delete when don't need it anymore
    //string displayScriptCmd="echo\nmore "+backupScriptPath;
    //if (system(displayScriptCmd.c_str()));
}

void runTheScriptThatPeformsTheBackup()
{
    string runTheBackupScriptCommand="/bin/bash "+backupScriptPath;
    if(system(runTheBackupScriptCommand.c_str()));
}

double getRemainingSizeInHomeDir()
//long long int getRemainingSizeInHomeDir()
{
    //source : https://stackoverflow.com/questions/33249591/how-can-i-only-get-the-number-of-bytes-available-on-a-disk-in-bash
    string getRemainingSizeInHomeDirCmd=
    "df ~ > "+remainingSizeInHomeDirectoryPath;
    //31151732 This is 31 Mega Bytes// NOTE:  This is multiplied by 1000 Below to make the correct units which is Gigabytes

    //the below line does not work on 18.04
    //"df | grep $HOME | awk '{print $4}' > "+remainingSizeInHomeDirectoryPath;

    if (system(getRemainingSizeInHomeDirCmd.c_str()));

    /* extract string equivalent of remaining size in Home directory */
    string line="";
    ifstream remainingSizeInHomeDirectoryHandle;
    openForReading(remainingSizeInHomeDirectoryPath,
                   __FILE__,
                   __LINE__,
                   remainingSizeInHomeDirectoryHandle);
    /* This is an example of what the file contains

    Filesystem           1K-blocks      Used Available Use% Mounted on
    /home/user/.Private  201453536 162199868  28997348  85% /home/user

    */
    // discard the first line, as this just contains column titles
    getline(remainingSizeInHomeDirectoryHandle,line);
    // this is the second line which contains the actual values.  This is the
    // line that we want to parse.
    getline(remainingSizeInHomeDirectoryHandle,line);

    remainingSizeInHomeDirectoryHandle.close();

    // We want what's under the "Available" column (Which is column #4 out of 6)
    string sizeRemainingInHomeDirectoryIn1KByteBlocks = getStringUnder(4,6,line);

    /* return the remaining size in the home directory by converting the string to a double */
    ///* return a converted string to long long int of the remaining size in the home directory */
    // source: https://stackoverflow.com/questions/21887274/convert-string-to-long-long-int
    char* endptr = NULL;
    //long long int remainingSizeInHomeDirectory =
        //strtoll(sizeRemainingInHomeDirectoryIn1KByteBlocks.c_str(), &endptr, 10);
    double remainingSizeInHomeDirectory =
        strtod(sizeRemainingInHomeDirectoryIn1KByteBlocks.c_str(), &endptr);
    int conversionFrom1KByteBlocksToBytes = 1000;
    remainingSizeInHomeDirectory *= conversionFrom1KByteBlocksToBytes;
    return remainingSizeInHomeDirectory;
}
