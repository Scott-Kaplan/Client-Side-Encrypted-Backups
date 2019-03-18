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

/**********************/
/***** Namespaces *****/
/**********************/
using namespace std;

/********************/
/***** Includes *****/
/********************/
#include "fileUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <iomanip>

/***********************/
/***** Share these *****/
/***********************/
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ifstream &readFileHandle);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
extern "C" void convert$HOME(string &path);
extern "C" void deleteFile(string &filename);
extern "C" void getGlobalStrings(globalStringS &globalString, string &purpose);
extern "C" bool fileIsEmpty(string &path);
extern "C" string getStringUnder(int columnNumberWanted,
                                 int totalNumberOfColumns,
                                 string &lineToBeParsed);
extern "C" double getTotalLinesInFile(string &pathToCountLines);
extern "C" double getSizeOfFile(string &path);
extern "C" void retrieveTheUsernameAndDomain
                    (string &resultsDirectory,string &username,string &domain);
extern "C" void retrieveTheLandingDirectory
                    (string &resultsDirectory, string &landingDirectory);
extern "C" void clearTheTerminalWindow();
extern "C" void extractPathAndFileName(string &stringToParse, string &path,
                                       string &fileName);
extern "C" void writeCleanUpAndExitFunction
                                 (string &purpose, ofstream &scriptHandle);
extern "C" void writeCleanUpFunction
                                 (string &purpose, ofstream &scriptHandle);
extern "C" void saveTheTerminalPid(string &purpose);
extern "C" bool fileExist(string &lookupFilePath,
                          string fromFileName,
                          int fromLineNumber,
                          string resultsDirectory);
extern "C" bool directoryExist(string &lookupPath,
                               string fromFileName,
                               int fromLineNumber,
                               string resultsDirectory);
extern "C" void displayError(string &problem, string &correctiveAction);
extern "C" void checkThatTheConfigurationFileHasBeenInstalled(string &path,
                                                              string &purpose);
extern "C" void checkThatThereAreNoWhiteSpaces(string &input,
                                               string lineTitle,
                                               string &configurationFilePath);
extern "C" void displayIncorrectCommandLineArguments(int argc,
                                                    char * const argv[],
                                                    string &purpose);
extern "C" void stripInvalidCharactersFromStartOfLine(string &line);
extern "C" string getFormattedSize(double size);

/*******************************/
/***** Function Prototypes *****/
/*******************************/
void stripLeadingWhiteSpaces(string &line);
void stripLeadingUnicodeReplacementCharacters(string &line);

/*********************/
/***** Functions *****/
/*********************/
void openForReading(string &path,
                    string fromFileName,
                    int fromLineNumber,
                    ifstream &readFileHandle)
{
    convert$HOME(path);
    readFileHandle.open(path.c_str());
    if (!(readFileHandle.is_open()))
    {
        /* can't open the file for reading */
        cout<<endl<<"ERROR: Could not open this file for reading: \""<<path
            <<"\"\nfrom file = "<<fromFileName
            <<"\nfrom line # "<<fromLineNumber
            <<endl<<endl;
        exit(EXIT_SUCCESS);
    }
}

void openForWriting(string &path,
                    string fromFileName,
                    int fromLineNumber,
                    ofstream &writeFileHandle,
                    FileWritingType fileWritingType)
{
    convert$HOME(path);
    if (fileWritingType == NEW_FILE)
    {
        writeFileHandle.open(path.c_str());
    }
    else if (fileWritingType == CONCATENATE_OTHERWISE_CREATE_NEW_FILE)
    {
        writeFileHandle.open(path.c_str(),ios::app);
    }
    if (!(writeFileHandle.is_open()))
    {
        /* can't open the file for writing */
        cout<<endl<<"ERROR: Could not open this file for writing: \""<<path
        <<"\"\nfrom file = "<<fromFileName
        <<"\nfrom line # "<<fromLineNumber
        <<endl<<endl;
        exit(EXIT_SUCCESS);
    }
}

void convert$HOME(string &path)
{
    if (path.substr(0,5)=="$HOME")
    {
        string remainingStringAfter$HOME=path.substr(5);
        path=getenv("HOME")+remainingStringAfter$HOME;
    }
}

string convertToString(int number)
{
    stringstream ss;
    ss<<number;
    return ss.str();
}

bool fileExist(string &lookupFilePath,
               string fromFileName,
               int fromLineNumber,
               string resultsDirectory)
{
    /* the examples below are to list just a specific file (no directories) */

    // example) search for a file while in its directory
    // ls -ap | grep -v / | grep "filename" > results 2>&1

    // ls -ap "PathUpToFileName"| grep -v / | grep "filename" > results 2>&1
    // input: filename exists
    // output: filename gets saved into the results file

    // ls -ap "PathUpToFileName" | grep -v / | grep "filename1" > results 2>&1
    // input: filename1 doesn't exist
    // output: nothing saved into the results file

    // ls -ap "PathUpToFileName" | grep -v / | grep "Movies" > results 2>&1
    // input: Movies is a directory
    // output: nothing saved into the results file

    convert$HOME(lookupFilePath);
    globalStringS globalString;
    getGlobalStrings(globalString,resultsDirectory);
    string fileExistResults =
                globalString.basePath+
                "fileExistLookupFrom_"+fromFileName+
                "_fromLineNumber__"+convertToString(fromLineNumber);
    string dirThatTheFileIsIn="";
    string fileName="";
    extractPathAndFileName(lookupFilePath,dirThatTheFileIsIn,fileName);
    string fileExistCmd =
    //example)
    //ls -ap "$HOME/.cloudbuddy/input/" | grep -v / | grep -F
    //"[1] search_this_list_for_changes" > results 2>&1;
        "ls -ap \""+dirThatTheFileIsIn+"\" | grep -v / | grep -F \""+fileName
                                            +"\" > "+fileExistResults+" 2>&1";
    if(system(fileExistCmd.c_str()));
    bool theFileExists = false;
    // The above command only searches for substrings rather than entire strings
    // The above command searching for say 'howdy', could yield hits like
    //
    // howdy1~
    // howdy1
    // howdyBk
    // howdy
    //
    // from these above 4 hits, we only want to return true if the whole line
    // is 'howdy'
    //
    ifstream exactMatchHandle;
    string line="";
    openForReading(fileExistResults,__FILE__,__LINE__,exactMatchHandle);
    while (getline(exactMatchHandle,line))
    {
        if (line == fileName)
        {
            theFileExists = true;
            break;
        }
    }
    exactMatchHandle.close();
    deleteFile(fileExistResults);
    return theFileExists;
}

bool directoryExist(string &lookupDirectoryPath,
               string fromFileName,
               int fromLineNumber,
               string resultsDirectory)
{
    // example) search for a directory while in its directory
    // ls -d Videos/ > results 2>&1

    // ls -d "$HOME/Videos/" > results 2>&1
    // input: the Videos directory exists
    // output: Videos/ gets saved in the results file

    // successful failure because filename is a file
    // ls -d "$HOME/filename/" > results 2>&1
    // input: filename is a file
    // output: This gets saved in the results file -
    //         ls: cannot access filename/: Not a directory

    // ls -d "$HOME/Videos1/" > results 2>&1
    // input: the Videos1 directory does not exist
    // output: This gets saved in the results file -
    //         ls: cannot access Videos1/: No such file or directory

    convert$HOME(lookupDirectoryPath);
    globalStringS globalString;
    getGlobalStrings(globalString,resultsDirectory);
    string directoryExistResults =
                globalString.basePath+
                "directoryExistLookupFrom_"+fromFileName+
                "_fromLineNumber__"+convertToString(fromLineNumber);
    bool theDirectoryExists = true;
    string directoryExistCmd =
        // example)
        // ls -d "$HOME/Videos/" > results 2>&1
        "ls -d \""+lookupDirectoryPath+"/\" > "+directoryExistResults+" 2>&1";
    if(system(directoryExistCmd.c_str()));
    ifstream directoryExistHandle;
    string line="";
    openForReading(directoryExistResults,__FILE__,__LINE__,
                                                        directoryExistHandle);
    getline(directoryExistHandle,line);
    directoryExistHandle.close();
    deleteFile(directoryExistResults);
    if (line.substr(0,17) == "ls: cannot access")
    {
        theDirectoryExists = false;
    }

    return theDirectoryExists;
}

void deleteFile(string &filename)
{
    string deleteFileCmd="rm -f "+filename;
    if(system(deleteFileCmd.c_str()));
}

void getGlobalStrings(globalStringS &globalString, string &purpose)
{
    /* examples for the next line equate to one of these - */
    /* "$HOME/.cloudbuddy/backup/"  - when the user is backing up */
    /* "$HOME/.cloudbuddy/restore/" - when the user is restoring */
    globalString.basePath = "$HOME/.cloudbuddy/"+purpose+"/";

    globalString.theTarCommandIsDone =
            globalString.basePath+ "theTarCommandIsDone";

    globalString.fileContainingTheBackupTarBallPath =
            globalString.basePath+"fileContainingTheBackupTarBallPath";

    globalString.fileContainingTheEncryptedBackupPath =
            globalString.basePath+"fileContainingTheEncryptedBackupPath";

    globalString.fileThatContainsTheTarProcessId =
            globalString.basePath+"fileThatContainsTheTarProcessId";

    globalString.fileThatContainsTheTarStatusProcessId =
            globalString.basePath+"fileThatContainsTheTarStatusProcessId";

    globalString.fileThatContainsTheCcryptStatusProcessId =
            globalString.basePath+"fileThatContainsTheCcryptStatusProcessId";

    globalString.killTheTarProcess =
            "kill \"$(head -n 1 "+
            globalString.fileThatContainsTheTarProcessId+
            // if the process doesn't exist don't display a message
            ")\" >/dev/null 2>&1";

    globalString.killTheTarStatusProcess =
            "kill \"$(head -n 1 "+
            globalString.fileThatContainsTheTarStatusProcessId+
            // if the process doesn't exist don't display a message
            ")\" >/dev/null 2>&1";

    globalString.killTheCcryptStatusProcess=
            "kill \"$(head -n 1 "+
            globalString.fileThatContainsTheCcryptStatusProcessId+
            // if the process doesn't exist don't display a message
            ")\" >/dev/null 2>&1";

    globalString.projectedSizeOfTheTarBallPath =
            globalString.basePath+"projectedSizeOfTheTarBall";

    globalString.usernameAndDomainPath =
            "$HOME/.cloudbuddy/input/[2] username_and_domain";

    globalString.landingDirectoryPath =
            "$HOME/.cloudbuddy/input/[3] landing_directory";

    globalString.ccryptTitlePrintedPath =
            globalString.basePath+"ccryptStatusTitleAlreadyPrinted";

    globalString.deleteAllFilesInTheBackupDirectory =
            "rm -f $HOME/.cloudbuddy/backup/*";

    globalString.deleteAllFilesInTheRestoreDirectory =
            "rm -f $HOME/.cloudbuddy/restore/*";

    globalString.ccryptFinishedGracefully =
            globalString.basePath+"ccryptFinished";

    globalString.binaryThatShowsTheTarStatus =
            "/usr/local/bin/actOnTarStatus";

    globalString.binaryThatShowsTheCcryptStatus =
            "/usr/local/bin/actOnCcryptStatus";

    globalString.processIdOfThisTerminalSessionPath =
            globalString.basePath+
            "fileContainingprocessIdOfThisTerminalSession";

    globalString.tarOutputPath =
            globalString.basePath+"tarOutput";

    globalString.sizeOfBackupThatIsADecryptedTarBallPath =
            globalString.basePath +"sizeOfBackupThatIsADecryptedTarBall";

    globalString.unexpectedTarOutputPath =
            globalString.basePath +"unexpectedTarOutput";

    globalString.totalFilesInBackupPath =
            globalString.basePath +"totalFilesInBackup";
}

bool fileIsEmpty(string &path)
{
    convert$HOME(path);
    bool fileIsEmptyFlag = true;
    FILE * pFile;
    long size = 1;
    pFile = fopen (path.c_str(),"rb");
    if (pFile != NULL)
    {
        fseek (pFile, 0, SEEK_END);
        size=ftell (pFile);
        fclose (pFile);
    }
    if (size)
    {
        fileIsEmptyFlag = false;
    }
    return fileIsEmptyFlag;
}

string getStringUnder(int columnNumberWanted,
                      int totalNumberOfColumns,
                      string &lineToBeParsed)
{
    string discardColumn;
    string parsedResult;

    std::istringstream is(lineToBeParsed);

    for (int i=1; i < totalNumberOfColumns; ++i)
    {
        is >> parsedResult;
        if (i == columnNumberWanted)
            break;
    }
    return parsedResult;
}

double getTotalLinesInFile(string &pathToCountLines)
{
    string line="";
    ifstream fileHandle;
    openForReading(pathToCountLines,__FILE__,__LINE__,fileHandle);
    double totalLines=0;
    while (getline(fileHandle,line))
    {
        ++totalLines;
    }
    fileHandle.close();
    return totalLines;
}

double getSizeOfFile(string &path)
{
    convert$HOME(path);
    FILE *p_file = NULL;
    double size = 0;
    p_file = fopen(path.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    size = ftell(p_file);
    fclose(p_file);
    return size;
}

void retrieveTheUsernameAndDomain(string &resultsDirectory,
                                  string &username,
                                  string &domain)
{
    globalStringS globalString;
    getGlobalStrings(globalString,resultsDirectory);
    bool usernameAndDomainFound = false;
    string line="";
    ifstream fillUsernameAndDomainHandle;
    openForReading(globalString.usernameAndDomainPath,
                   __FILE__,
                   __LINE__,
                   fillUsernameAndDomainHandle);
    while (getline(fillUsernameAndDomainHandle,line))
    {
       stripInvalidCharactersFromStartOfLine(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            checkThatThereAreNoWhiteSpaces
                                    (line,"username and domain line",
                                     globalString.usernameAndDomainPath);
            size_t positionOfLastAtSign = line.find_last_of("@");
            if (positionOfLastAtSign != string::npos)
            {
                usernameAndDomainFound = true;
                username = line.substr(0,positionOfLastAtSign);
                domain = line.substr(positionOfLastAtSign+1);
            }
        }
    }
    fillUsernameAndDomainHandle.close();
    if (!usernameAndDomainFound)
    {
        string problem=
            "The configuration file \""+globalString.usernameAndDomainPath
            +"\" needs to contain a valid entry.";
        string correctiveAction=
            "Please add an entry.";
        displayError(problem,correctiveAction);
    }
}

void retrieveTheLandingDirectory(string &resultsDirectory,
                                 string &landingDirectory)
{
    globalStringS globalString;
    getGlobalStrings(globalString,resultsDirectory);
    bool landingDirectoryFound = false;
    string line="";
    ifstream landingDirectoryHandle;
    openForReading(globalString.landingDirectoryPath,
                   __FILE__,
                   __LINE__,
                   landingDirectoryHandle);
    while (getline(landingDirectoryHandle,line))
    {
      stripLeadingUnicodeReplacementCharacters(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
            checkThatThereAreNoWhiteSpaces
                                    (line,"landing directory line",
                                     globalString.landingDirectoryPath);
            landingDirectoryFound = true;
            landingDirectory = line;
        }
    }
    landingDirectoryHandle.close();
    if (!landingDirectoryFound)
    {
        string problem=
            "The configuration file \""+globalString.landingDirectoryPath
            +"\" needs to contain an entry.";
        string correctiveAction=
            "Please add an entry.";
        displayError(problem,correctiveAction);
    }
}

void clearTheTerminalWindow()
{
    if(system("clear"));
}

void extractPathAndFileName
                        (string &stringToParse, string &path, string &fileName)
{
    /* there is no directory, it's just the filename */
    if (stringToParse.find('/') == string::npos)
    {
        fileName = stringToParse;
        path = ".";
    }
    /* there is at least one directory */
    else
    {
        fileName = stringToParse.substr(stringToParse.find_last_of('/')+1,
                                            stringToParse.length());
        path = stringToParse.substr(0, stringToParse.find_last_of('/'));
    }
}

void saveTheTerminalPid(string &purpose)
{
    /* The purpose of this function is to have the ability to check whether */
    /* the user closed the terminal window through the terminal process id */
    /* during an encryption/decryption.  When this occurs, the background */
    /* process for backup/restore status is killed (so it doesn't otherwise */
    /* run forever) */

    globalStringS globalString;
    getGlobalStrings(globalString,purpose);
    ostringstream terminalPidStream;
    terminalPidStream<<getppid()<<endl;
    ofstream terminalPidHandle;
    openForWriting(globalString.processIdOfThisTerminalSessionPath,
                   __FILE__,__LINE__,terminalPidHandle,NEW_FILE);
    terminalPidHandle<<terminalPidStream.str()<<endl;
    terminalPidHandle.close();
}

void writeCleanUpFunction(string &purpose, ofstream &scriptHandle)
{
    globalStringS globalString;
    getGlobalStrings(globalString,purpose);

    /* begin the cleanUpAndExit function */
    scriptHandle<<tab0<<"cleanUp()"<<endl
    <<tab0<<"{"<<endl

    // kill the tar process if it is active
    <<tab1<<"if [ -e \""<<globalString.fileThatContainsTheTarProcessId<<"\" ]; "
          <<"then"<<endl
    <<tab2<<globalString.killTheTarProcess<<endl
    <<tab1<<"fi"<<endl

    // kill the tar status process if it is active
    <<tab1<<"if [ -e \""<<globalString.fileThatContainsTheTarStatusProcessId
          <<"\" ]; "<<"then"<<endl
    <<tab2<<globalString.killTheTarStatusProcess<<endl
    <<tab1<<"fi"<<endl

    // kill the ccrypt status process if it is active
    <<tab1<<"if [ -e \""
          <<globalString.fileThatContainsTheCcryptStatusProcessId
          <<"\" ]; then"<<endl
    <<tab2<<globalString.killTheCcryptStatusProcess<<endl
    <<tab1<<"fi"<<endl;

    if (purpose == "backup")
    {
        // delete all files in the backup directory
        scriptHandle<<tab1<<globalString.deleteAllFilesInTheBackupDirectory
        <<endl;
    }
    else // purpose == "restore"
    {
        // delete all files in the restore directory
        scriptHandle<<tab1<<globalString.deleteAllFilesInTheRestoreDirectory
        <<endl;
    }

    scriptHandle
    /* If the user Ctrl-C'd while either tar or ccrypt was going the */
    /* terminal window won't show the cursor because it was explicity removed */
    /* so that the progress percentage wouldn't show it blinking. */
    /* This brings the cursor back. */
    <<tab1<<"tput cnorm"<<endl

    /* in case echo'ing of what is typed in ceases, this brings it back */
    <<tab1<<"tset"<<endl

    /* end the cleanUp function*/
    <<tab0<<"}"<<endl<<endl;
}

void writeCleanUpAndExitFunction(string &purpose, ofstream &scriptHandle)
{
    /* begin the cleanUpAndExit function */
    scriptHandle
    <<tab0<<"cleanUpAndExit()"<<endl

    <<tab0<<"{"<<endl

    /* call the cleanUp function */
    <<tab1<<"cleanUp"<<endl

    <<tab1<<"echo"<<endl
    <<tab1<<"echo Exiting ..."<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"exit 1"<<endl
    <<tab0<<"}"<<endl<<endl;
}

void displayError(string &problem, string &correctiveAction)
{
    cout<<endl<<"ERROR"<<endl<<"  "<<problem
        <<endl<<"HOW TO FIX"<<endl<<"  "<<correctiveAction<<endl<<endl;
    exit(EXIT_SUCCESS);
}

void checkThatTheConfigurationFileHasBeenInstalled(string &path,
                                                   string &purpose)
{
    convert$HOME(path);
    if (!fileExist(path,__FILE__,__LINE__,purpose))
    {
        string problem=
            "The startup configuration file \""+path+"\" does not exist.";
        string correctiveAction=
            "Reinstall by following these steps:\n  "
            "[1] cd to your Client-Side-Encrypted-Backups directory\n  "
            "[2] sudo ./install.sh";
        displayError(problem,correctiveAction);
    }
}

void checkThatThereAreNoWhiteSpaces(string &line, string lineTitle,
                                                string &configurationFilePath)
{
    int lengthOfString = strlen(line.c_str());
    for (int i=0; i<lengthOfString; ++i)
    {
        if (line[i] == ' ')
        {
            string problem=
                "The "+lineTitle+" in \""+configurationFilePath
                +"\" contains a space in it.";
            string correctiveAction=
                "Please remove the space.";
            displayError(problem,correctiveAction);
        }
    }
}

void displayIncorrectCommandLineArguments(int argc,
                                         char * const argv[],
                                         string &purpose)
{
    string commandLineArgs="";
    for (int i=1,remaingArgs=argc;remaingArgs>1;++i,--remaingArgs)
    {
        commandLineArgs+=" ";
        commandLineArgs+=argv[i];
    }

    // examples of wrong usage:
    // > backup howdy there
    // > restore howdy howdy
    cout<<"> "<<purpose<<commandLineArgs<<endl;

    cout<<endl<<"ERROR - The wrong number of parameters were entered."<<endl
    <<endl;
}

void stripInvalidCharactersFromStartOfLine(string &line)
{
   stripLeadingUnicodeReplacementCharacters(line);
   stripLeadingWhiteSpaces(line);
}

void stripLeadingUnicodeReplacementCharacters(string &line)
{
   // For some unknown reason Libre office writer likes to inject unicode
   // replacement characters at the beginning of each config file.  These are
   // unrecognized characters which are represented by black diamonds with
   // question marks in them.  This block of code removes them. */
   int invalidStartingCharacterCounter = 0;
   while (+line[invalidStartingCharacterCounter] < 0)
   {
      ++invalidStartingCharacterCounter;
   }
   if (invalidStartingCharacterCounter)
   {
      line.erase(0,invalidStartingCharacterCounter);
   }
}

void stripLeadingWhiteSpaces(string &line)
{
    const string delimiter = " ";
    line.erase(0,line.find_first_not_of(delimiter));
}

string getFormattedSize(double size)
{
    ostringstream formattedSize;
    double sizeInCategory = 0;

    if (size < 1000)
    {
        formattedSize<<size<<" bytes";
    }
    else if (size < 1000000)
    {
        sizeInCategory = size/1000;
        // In the next line, setprecision(6) equates to a maximum of 6 digits
        // xxx.xxx while the minimum can be as few as 1 digit (x)
        formattedSize<<setprecision(6)<<sizeInCategory<<" KB";
    }
    else if (size < 1000000000)
    {
        sizeInCategory = size/1000000;
        formattedSize<<setprecision(6)<<sizeInCategory<<" MB";
    }
    else if (size < 1000000000000)
    {
        sizeInCategory = size/1000000000;
        formattedSize<<setprecision(6)<<sizeInCategory<<" GB";
    }
    else if  (size < 100000000000000)
    {
        sizeInCategory = size/1000000000000;
        formattedSize<<setprecision(6)<<sizeInCategory<<" TB";
    }
    return formattedSize.str();
}
