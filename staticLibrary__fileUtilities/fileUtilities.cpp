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
#include "fileUtilities.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>

/****************************************/
/************** Share these *************/
/****************************************/
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
// works except does both directories and files extern "C" bool fileExists(string &lookupPath,string &lookupPathResults);
extern "C" void deleteFile(string &filename);
extern "C" void getGlobalStrings(globalStringS &globalString, string &purpose);
extern "C" bool fileIsEmpty(string &path);
extern "C" string getStringUnder(int columnNumberWanted,
                                 int totalNumberOfColumns,
                                 string &lineToBeParsed);
extern "C" double getTotalLinesInFile(string &pathToCountLines);
extern "C" double getSizeOfFile(string &path);
extern "C" string getFileName(string &path);
extern "C" string getTimestamp();
extern "C" void retrieveUsernameAndDomain(string &username, string &domain,
                                          string &retrieveUsernameAndDomain);
extern "C" void removeLeadingWhiteSpaces(string &line);
extern "C" void clearTheTerminalWindow();
extern "C" void extractPathAndFileName(string &stringToParse, string &path,
                                       string &fileName);
extern "C" void writeCleanUpAndExitFunction
                                 (string &purpose, ofstream &scriptHandle);
extern "C" void writeCleanUpFunction
                                 (string &purpose, ofstream &scriptHandle);
extern "C" void saveTheTerminalPid(string &purpose);
//extern "C" bool fileExist(string &path, string &purpose);
extern "C" bool fileExist(string &lookupFilePath,
                          string fromFileName,
                          int fromLineNumber,
                          string resultsDirectory);
//extern "C" bool directoryExist(string &path, string &purpose);
extern "C" bool directoryExist(string &lookupPath,
                               string fromFileName,
                               int fromLineNumber,
                               string resultsDirectory);

///*******************************/
///***** Function Prototypes *****/
///*******************************/
bool exist(string &path, string &purpose, string &lookupSpecifier);

/*****************************/
/********* Functions *********/
/*****************************/

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
    /* $HOME varies dependent on the directory that the binary is called from */
    /* need to always make that this is the user's home directory */
    if (path.substr(0,5)=="$HOME")
    {
        string remainingStringAfter$HOME=path.substr(5);
        path=getenv("HOME")+remainingStringAfter$HOME;
    }
}

/*
bool fileExist(string &path, string &purpose)
{
    string lookupSpecifier = "f";
    return exist(path,purpose,lookupSpecifier);
}

bool directoryExist(string &path, string &purpose)
{
    string lookupSpecifier = "d";
    return exist(path,purpose,lookupSpecifier);
}

bool exist(string &path, string &purpose, string &lookupSpecifier)
{
    globalStringS globalString;
    getGlobalStrings(globalString,purpose);
    string directoryLeadingUpToFileName="";
    string fileName="";
    extractPathAndFileName(path,directoryLeadingUpToFileName,fileName);
    string pathLookup = globalString.basePath+fileName+"_lookUp";
//    string fileOrDirExistsLookupPath = path+"_LookUp";
                        //globalString.basePath+"fileOrDirExistsLookup";
//    // To test a file:      [ -f dir ] && echo exists > results
//    // to test a directory: [ -d dir ] && echo exists > results
//    string cmd =
//                "[ -"+lookupSpecifier+" \""+path+"\" ] && echo exists > "
//                +fileOrDirExistsLookupPath;

    //examples)
//    // To test a file:      [ -f dir ] && echo exists || echo doesnt_exist > results
//    // To test a directory: [ -d dir ] && echo exists || echo doesnt_exist > results
    // To test a file:      [ -f dir ] && echo exists > results || echo doesnt_exist > results
    // To test a directory: [ -d dir ] && echo exists > results || echo doesnt_exist > results

    string cmd =
        "[ -"+lookupSpecifier+" \""+path+"\" ] "
        "&& echo exists > \""+pathLookup+"\" || echo doesnt_exist > \""+pathLookup+"\"";


//the problem with this is that fileOrDirExistsLookup is not created when the path is not found
//however the file is still read in its previous state, which was file found.  so exists lingers in the file
//when ccrypt didn't have the key entered yet, it still found the file which had exists.
// there is another problem in that if the path was not found, fileOrDirExistsLookup doesn't exist and the segmentation fault
//came from trying to read it below.  Change this to print exists or doesn't exist and also don't share the same filename for
//the resultant file

cout<<endl<<cmd<<endl<<endl;
    if(system(cmd.c_str()));
//exit(EXIT_SUCCESS);
    ifstream pathLookupHandle;
//    ifstream fileOrDirExistsHandle;
    string line="";
    openForReading(pathLookup,__FILE__,__LINE__,pathLookupHandle);
//    openForReading(fileOrDirExistsLookupPath,
//                   __FILE__,__LINE__,
//                   fileOrDirExistsHandle);
    getline(pathLookupHandle,line);
    pathLookupHandle.close();
//    getline(fileOrDirExistsHandle,line);
//    fileOrDirExistsHandle.close();
// for some reason the next line causes an error that the file cannot be opened for reading
//    deleteFile(fileOrDirExistsLookupPath);
    deleteFile(pathLookup);
    if (line == "exists")
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/

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
//bool fileExist(string &lookupPath,string &fileExistResults)
{
    /* list only specific file */
    //https://askubuntu.com/questions/811210/how-can-i-make-ls-only-display-files

    // example in current directory
    // ls -p | grep -v / | grep "filename" > results 2>&1

    // success
    // ls -p "PathUpToFileName"| grep -v / | grep "filename" > results 2>&1
    // saves
    // filename

    // successful failure because filename1 doesn't exist
    // ls -p "PathUpToFileName" | grep -v / | grep "filename1" > results 2>&1
    // saves
    // nothing as expected

    // successful failure because Movies is a directory
    // ls -p "PathUpToFileName" | grep -v / | grep "Movies" > results 2>&1
    // saves
    // nothing as expected

    globalStringS globalString;
    getGlobalStrings(globalString,resultsDirectory);
    string fileExistResults =
                globalString.basePath+
                "fileExistLookupFrom_"+fromFileName+
                "_fromLineNumber__"+convertToString(fromLineNumber);
    string dirThatTheFileIsIn="";
    string fileName="";
    extractPathAndFileName(lookupFilePath,dirThatTheFileIsIn,fileName);
    bool theFileExists = true;
    string fileExistCmd =
    //example)
    //ls -p "$HOME/.cloudbuddy/input/" | grep -v / | grep -F "[1] search_this_list_for_changes" > results 2>&1;
            "ls -p \""+dirThatTheFileIsIn+"\" | grep -v / | grep -F \""+fileName
                                              +"\" > "+fileExistResults+" 2>&1";
    if(system(fileExistCmd.c_str()));
    if (fileIsEmpty(fileExistResults))
    {
        theFileExists = false;
    }
    return theFileExists;
}

bool directoryExist(string &lookupDirectoryPath,
               string fromFileName,
               int fromLineNumber,
               string resultsDirectory)
//bool directoryExist(string &lookupDirectory,string &directoryExistResults)
{
    /* list only specific directory */

    // example in current directory
    // ls -d Videos/ > results 2>&1

    // success
    // ls -d "$HOME/Videos/" > results 2>&1
    // saves
    // Videos/

    // successful failure because filename is a file
    // ls -d "$HOME/filename/" > results 2>&1
    // saves
    // ls: cannot access filename/: Not a directory

    // successful failure because Videos1 is not a directory
    // ls -d "$HOME/Videos1/" > results 2>&1
    // saves
    // ls: cannot access Videos1/: No such file or directory

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
cout<<directoryExistCmd<<endl;
LEFT OFF HERE
uncomment this line and ensure that it passes
//    if(system(directoryExistCmd.c_str()));
    ifstream directoryExistHandle;
    string line="";
    openForReading(directoryExistResults,__FILE__,__LINE__,directoryExistHandle);
    getline(directoryExistHandle,line);
    directoryExistHandle.close();
    deleteFile(directoryExistResults);
    if (line.substr(0,17) == "ls: cannot access")
    {
        theDirectoryExists = false;
    }
    return theDirectoryExists;
}

/* works but doesn't differentiate between directories and files
bool fileExists(string &lookupPath,string &lookupPathResults)
{
    string lsCmd=
                "ls "
                "\""+
                lookupPath+
                "\""
                " > "+
                lookupPathResults+
                " 2>&1";
    //dcout<<lsCmd<<endl;
    if(system(lsCmd.c_str()));
    ifstream lsOutputHandle;
    string line="";
    openForReading(lookupPathResults,__FILE__,__LINE__,lsOutputHandle);
    getline(lsOutputHandle,line);
    lsOutputHandle.close();
    deleteFile(lookupPathResults);
    if (line.substr(0,17)!="ls: cannot access")
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/


void deleteFile(string &filename)
{
    string deleteFileCmd="rm -f "+filename;
    if(system(deleteFileCmd.c_str()));
}

void getGlobalScriptLineFormatter(scriptLineFormatterS &f)
{
    f.tab0="";
    f.tab1="  ";
    f.tab2="    ";
    f.tab3="      ";
}

void getGlobalStrings(globalStringS &globalString, string &purpose)
{
    /* examples for the next line */
    /* "$HOME/.cloudbuddy/backup/" - if it's backing up */
    /* "$HOME/.cloudbuddy/restore/" -  if it's restoring */
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
            // if the process doesn't exist don't output a message
            ")\" >/dev/null 2>&1";

    globalString.killTheTarStatusProcess =
            "kill \"$(head -n 1 "+
            globalString.fileThatContainsTheTarStatusProcessId+
            // if the process doesn't exist don't output a message
            ")\" >/dev/null 2>&1";

    globalString.killTheCcryptStatusProcess=
            "kill \"$(head -n 1 "+
            globalString.fileThatContainsTheCcryptStatusProcessId+
            // if the process doesn't exist don't output a message
            ")\" >/dev/null 2>&1";

    globalString.projectedSizeOfTheTarBallPath =
            globalString.basePath+"projectedSizeOfTheTarBall";

    globalString.usernameAndDomainPath =
            "$HOME/.cloudbuddy/input/[2] username_and_domain";

    globalString.ccryptTitlePrintedPath =
            globalString.basePath+"ccryptStatusTitleAlreadyPrinted";

    globalString.deleteAllFilesInTheBackupDirectory =
//            "/bin/bash rm -f $HOME/.cloudbuddy/backup/*";
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

    globalString.resultsOfTarCommand =
            globalString.basePath+"resultsOfTarCommand";

    globalString.sizeOfBackupThatIsADecryptedTarBallPath =
            globalString.basePath +"sizeOfBackupThatIsADecryptedTarBall";
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
    openForReading(pathToCountLines,
                   __FILE__,
                   __LINE__,
                   fileHandle);
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

string getFileName(string &path)
{
    //source https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch10s15.html
    string fileName = "";
    char sep = '/';
    size_t i = path.rfind(sep, path.length());
    if (i != string::npos)
    {
        fileName = (path.substr(i+1, path.length() - i));
    }
    return fileName;
}

string getTimestamp()
{
    struct tm *loctime; // the current time
    char month[4];
    char day[4];
    char year[6];
    char hour[4];
    char minute[4];
    char second[4];
    char middayMarking[4]; // AM or PM
    time_t secondsSince1970;

    // Get the current time
    secondsSince1970 = time (NULL);

    // Convert local time representation
    loctime = localtime (&secondsSince1970);

    strftime (month,4,"%m",loctime);            // get the month
    strftime (day, 4, "%d", loctime);           // get the day
    strftime(hour, 4, "%I", loctime);           // get the hour
    strftime(minute, 4, "%M", loctime);         // get the minute(s)
    strftime(second, 4, "%S", loctime);         // get the second(s)
    strftime(middayMarking, 4, "%P", loctime);  // get am or pm
    strftime (year, 6, "%Y", loctime);          // get the year

    ostringstream currentTime;
    currentTime<<   year<<"_"<<
                    month<<"_"<<
                    day<<"__"<<
                    hour<<"_"<<
                    minute<<"_"<<
                    second<<
                    middayMarking;
    return currentTime.str();
}

void retrieveUsernameAndDomain(string &username, string &domain, string &usernameAndDomainPath)
{
    bool usernameAndDomainFound = false;
    string line="";
    ifstream fillUsernameAndDomainHandle;

    openForReading(usernameAndDomainPath,
                   __FILE__,
                   __LINE__,
                   fillUsernameAndDomainHandle);
    while (getline(fillUsernameAndDomainHandle,line))
    {
        //dcout<<line<<endl;
        removeLeadingWhiteSpaces(line);
        if ((line[0] != '#') && // skip comment lines
            (!line.empty()))    // skip empty lines
        {
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
        cout<<
            "\nError: Unable to extract the username and domain of the sftp "
            "server that you want to send the backup to.\nTo fix, please see "
            "this file -\n"
            "$HOME/.cloudbuddy/input/[2] username_and_domain\n"
        <<endl;
        exit(EXIT_SUCCESS);
    }
}

void removeLeadingWhiteSpaces(string &line)
{
    const string delimiter = " ";
    line.erase(0,line.find_first_not_of(delimiter));
    // source: http://www.cplusplus.com/faq/sequences/strings/trim/
}

void clearTheTerminalWindow()
{
    if(system("clear"));
}

void extractPathAndFileName
                        (string &stringToParse, string &path, string &fileName)
{
    /* there is no directory, it's just the filename*/
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
    /* the purpose of this function is to have the ability to check whether */
    /* the user closed the terminal window during an encryption/decryption. */
    /* In this case, the background process showing the encryption/decryption */
    /* status will get canceled (killed) */

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
    scriptLineFormatterS f;
    getGlobalScriptLineFormatter(f);
    globalStringS globalString;
    getGlobalStrings(globalString,purpose);

    /* begin the cleanUpAndExit function */
    scriptHandle<<f.tab0<<"cleanUp()"<<endl
    <<f.tab0<<"{"<<endl

    // kill the tar process if it is active
    <<f.tab1<<"if [ -e \""<<globalString.fileThatContainsTheTarProcessId<<"\" ]; "
          <<"then"<<endl
    <<f.tab2<<globalString.killTheTarProcess<<endl
    <<f.tab1<<"fi"<<endl

    // kill the tar status process if it is active
    <<f.tab1<<"if [ -e \""<<globalString.fileThatContainsTheTarStatusProcessId
          <<"\" ]; "<<"then"<<endl
    <<f.tab2<<globalString.killTheTarStatusProcess<<endl
    <<f.tab1<<"fi"<<endl

    // kill the ccrypt status process if it is active
    <<f.tab1<<"if [ -e \""
          <<globalString.fileThatContainsTheCcryptStatusProcessId
          <<"\" ]; then"<<endl
    <<f.tab2<<globalString.killTheCcryptStatusProcess<<endl
    <<f.tab1<<"fi"<<endl;

    if (purpose == "backup")
    {
        // delete all files in the backup directory
        scriptHandle<<f.tab1<<globalString.deleteAllFilesInTheBackupDirectory<<endl;
    }
    else // purpose == "restore"
    {
        // delete all files in the restore directory
        scriptHandle<<f.tab1<<globalString.deleteAllFilesInTheRestoreDirectory<<endl;
    }

    scriptHandle
    /* if the user Ctrl-C'd while either tar or ccrypt was going the */
    /* terminal window won't show the cursor because it was explicity removed */
    /* so that the progress percentage wouldn't show the cursor blinking. */
    /* This brings it back. */
    <<f.tab1<<"tput cnorm"<<endl

    /* in case echo'ing of what's typed in ceases, this brings it back */
    <<f.tab1<<"tset"<<endl

    /* end the cleanUp function*/
    <<f.tab0<<"}"<<endl<<endl;
}

void writeCleanUpAndExitFunction(string &purpose, ofstream &scriptHandle)
{
    scriptLineFormatterS f;
    getGlobalScriptLineFormatter(f);

    /* begin the cleanUpAndExit function */
    scriptHandle
    <<f.tab0<<"cleanUpAndExit()"<<endl

    <<f.tab0<<"{"<<endl

    /* call the cleanUp function */
    <<f.tab1<<"cleanUp"<<endl

    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"echo Exiting ..."<<endl
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"exit 1"<<endl
    <<f.tab0<<"}"<<endl<<endl;
}
