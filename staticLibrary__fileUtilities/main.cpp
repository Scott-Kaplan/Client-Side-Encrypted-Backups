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

/****************************************/
/************** Share these *************/
/****************************************/
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int lineNumber,
                               ifstream &readFileHandle);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int lineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
//extern "C" string convertNumberToString(int number);
extern "C" void convert$HOME(string &path);
extern "C" bool fileExists(string &lookupFile,string &lookupFileResults);
extern "C" void deleteFile(string &filename);
//extern "C" void getGlobalStrings(globalStringS &globalString);
extern "C" void getGlobalStrings(globalStringS &globalString, string &reason);
extern "C" bool fileIsEmpty(string &path);
extern "C" string getStringUnder(int columnNumberWanted,
                                 int totalNumberOfColumns,
                                 string &lineToBeParsed);
extern "C" double getTotalLinesInFile(string &pathToCountLines);
//extern "C" long int getTotalLinesInFile(string &pathToCountLines);
extern "C" double getSizeOfFile(string &path);
//extern "C" long long int getSizeOfFile(string &path);
extern "C" string getFileName(string &path);
extern "C" string getTimestamp();
//extern "C" void retrieveUsernameAndDomain(string &username, string &domain);
extern "C" void retrieveUsernameAndDomain(string &username, string &domain,
                                          string &retrieveUsernameAndDomain);
extern "C" void removeLeadingWhiteSpaces(string &line);
extern "C" void clearTheTerminalWindow();
extern "C" void extractPathAndFileName(string &stringToParse, string &path,
                                       string &fileName);
extern "C" void writeCleanUpAndExitFunction
                                 (string &reason, ofstream &scriptHandle);

/*****************************/
/********* Functions *********/
/*****************************/

void openForReading(string &path,
                    string fromFileName,
                    int lineNumber,
                    ifstream &readFileHandle)
{
    convert$HOME(path);
    readFileHandle.open(path.c_str());
    if (!(readFileHandle.is_open()))
    {
        /* can't open the file for reading */
        cout<<"ERROR: Could not open this file for reading: "<<path
            <<"\nfrom file = "<<fromFileName
            <<"\nfrom line # "<<lineNumber
            //<<"\nThe error enumerated value = "
            //<<convertNumberToString(errorNumIfFails)<<endl;
            <<endl;
        exit(EXIT_SUCCESS);
    }
}

void openForWriting(string &path,
                    //OfstreamErrorType errorNumIfFails,
                    string fromFileName,
                    int lineNumber,
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
        cout<<"ERROR: Could not open this file for writing: "<<path
        <<"\nfrom file = "<<fromFileName
        <<"\nfrom line # "<<lineNumber
        //<<"\nThe error enumerated value = "
        //<<convertNumberToString(errorNumIfFails)<<endl;
        <<endl;
        exit(EXIT_SUCCESS);
    }
}
/*
string convertNumberToString(int number)
{
    stringstream converter;
    converter << number;
    return converter.str();
}
*/

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

bool fileExists(string &lookupFile,string &lookupFileResults)
{
    string lsCmd=
                "ls "
                "\""+
                lookupFile+
                "\""
                " > "+
                lookupFileResults+
                " 2>&1";
    //dcout<<lsCmd<<endl;
    if(system(lsCmd.c_str()));
    ifstream lsOutputHandle;
    string line="";
    openForReading(lookupFileResults,__FILE__,__LINE__,lsOutputHandle);
    //openForReading(lookupFileResults,errorIfFails,lsOutputHandle);
    getline(lsOutputHandle,line);
    deleteFile(lookupFileResults);
    if (line.substr(0,17)!="ls: cannot access")
    {
        return true;
    }
    else
    {
        return false;
    }
}

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

void getGlobalStrings(globalStringS &globalString, string &reason)
{
    /* examples for the next line */
    /* "$HOME/.cloudbuddy/backup/" - if it's backing up */
    /* "$HOME/.cloudbuddy/restore/" -  if it's restoring */
    globalString.basePath = "$HOME/.cloudbuddy/"+reason+"/";
    //globalString.outputDirectoryPath = "$HOME/.cloudbuddy/output/";

    globalString.theTarCommandIsDone =
                                globalString.basePath+ "theTarCommandIsDone";
                            //"$HOME/.cloudbuddy/output/theTarCommandIsDone";

    globalString.fileContainingTheBackupTarBallPath =
        globalString.basePath
        +"fileContainingTheBackupTarBallPath";

    globalString.fileContainingTheEncryptedBackupPath =
        globalString.basePath
        +"fileContainingTheEncryptedBackupPath";
//        +"fileContainingTheNameOfTheEncryptedBackup";

    globalString.fileThatContainsTheTarProcessId =
        globalString.basePath+"fileThatContainsTheTarProcessId";
        //"$HOME/.cloudbuddy/output/fileThatContainsTheTarProcessId";

    globalString.fileThatContainsTheTarStatusProcessId =
        globalString.basePath+"fileThatContainsTheTarStatusProcessId";
        //"$HOME/.cloudbuddy/output/fileThatContainsTheTarStatusProcessId";

    globalString.fileThatContainsTheCcryptStatusProcessId =
        globalString.basePath+"fileThatContainsTheCcryptStatusProcessId";
        //"$HOME/.cloudbuddy/output/fileThatContainsTheCcryptStatusProcessId";

    globalString.killTheTarProcess =
                "kill \"$(head -n 1 "+
                globalString.fileThatContainsTheTarProcessId+
                ")\" >/dev/null 2>&1"; // if the process doesn't exist don't output a message

    globalString.killTheTarStatusProcess =
                "kill \"$(head -n 1 "+
                globalString.fileThatContainsTheTarStatusProcessId+
                ")\" >/dev/null 2>&1"; // if the process doesn't exist don't output a message

    globalString.killTheCcryptStatusProcess=
                "kill \"$(head -n 1 "+
                globalString.fileThatContainsTheCcryptStatusProcessId+
                ")\" >/dev/null 2>&1"; // if the process doesn't exist don't output a message

    globalString.projectedSizeOfTheTarBallPath =
                globalString.basePath+"projectedSizeOfTheTarBall";
                //globalString.outputDirectoryPath+"projectedSizeOfTheTarBall";

    globalString.usernameAndDomainPath =
                            "$HOME/.cloudbuddy/input/[2] username_and_domain";

    globalString.ccryptTitlePrintedPath =
                        globalString.basePath+"ccryptStatusTitleAlreadyPrinted";

    globalString.deleteOnlyFilesInSubDirectory = "rm -f $HOME/.cloudbuddy/"+reason+"/*";

    globalString.ccryptFinishedGracefully = globalString.basePath+"ccryptFinished";

    globalString.binaryThatShowsTheTarStatus =
    "\"$HOME/codeBlockProjects/open source/actOnTarStatus/bin/Release/actOnTarStatus\"";
    globalString.binaryThatShowsTheCcryptStatus =
    "\"$HOME/codeBlockProjects/open source/actOnCcryptStatus/bin/Release/actOnCcryptStatus\"";

    globalString.processIdOfThisTerminalSessionPath = globalString.basePath+
    "fileContainingprocessIdOfThisTerminalSession";
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
//long int getTotalLinesInFile(string &pathToCountLines)
{
    string line="";
    ifstream fileHandle;
    openForReading(pathToCountLines,
                   __FILE__,
                   __LINE__,
                   fileHandle);
    double totalLines=0;
    //long int totalLines=0;
    while (getline(fileHandle,line))
    {
        ++totalLines;
    }
    fileHandle.close();
    return totalLines;
}

double getSizeOfFile(string &path)
//long long int getSizeOfFile(string &path)
{
    convert$HOME(path);
    FILE *p_file = NULL;
    double size = 0;
    //long long int size = 0;
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

    /* get the month.  It might have a leading zero.  Remove it if it does */
    strftime (month,4,"%m",loctime);
//    if (month[0] == '0')
//    {
//        sprintf(month,"%c",month[1]);
//    }

    /* get the day.  It might have a leading zero.  Remove it if it does */
    strftime (day, 4, "%d", loctime);
//    if (day[0] == '0')
//    {
//        sprintf(day,"%c",day[1]);
//    }

    /* get the hour.  It might have a leading zero.  Remove it if it does */
    strftime(hour, 4, "%I", loctime);
//    if (hour[0] == '0')
//    {
//        sprintf(hour,"%c",hour[1]);
//    }

    strftime(minute, 4, "%M", loctime); // get the minute(s)
    strftime(second, 4, "%S", loctime); // get the second(s)
    strftime(middayMarking, 4, "%P", loctime); // get am or pm
    strftime (year, 6, "%Y", loctime); // get the year

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
//void retrieveUsernameAndDomain(string &username, string &domain)
{
//    globalStringS globalString;
//    getGlobalStrings(globalString);

    bool usernameAndDomainFound = false;
    string line="";
    ifstream fillUsernameAndDomainHandle;

    openForReading(usernameAndDomainPath,
//    openForReading(globalString.usernameAndDomainPath,
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

void writeCleanUpAndExitFunction(string &reason, ofstream &scriptHandle)
{
    scriptLineFormatterS f;
    getGlobalScriptLineFormatter(f);
    globalStringS globalString;
    getGlobalStrings(globalString,reason);

    /* begin the cleanUpAndExit function */
    scriptHandle
    <<f.tab0<<"cleanUpAndExit()"<<endl
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
    <<f.tab1<<"fi"<<endl

    // delete all files (non recursive, non directories) in the backup or
    // restore directory
    // It will be one of these
    // "rm -f $HOME/.cloudbuddy/backup/*";
    // "rm -f $HOME/.cloudbuddy/restore/*";
    <<f.tab1<<globalString.deleteOnlyFilesInSubDirectory<<endl

    /* if the user Ctrl-C'd while either tar or ccrypt was going the */
    /* terminal window won't show the cursor because it was explicity removed */
    /* so that the progress percentage wouldn't show the cursor blinking. */
    /* This brings it back. */
    <<f.tab1<<"tput cnorm"<<endl

    // exit the script
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"echo Exiting ..."<<endl
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"echo"<<endl
    <<f.tab1<<"exit 1"<<endl

    /* end the cleanUpAndExit function*/
    <<f.tab0<<"}"<<endl<<endl;
}










































































