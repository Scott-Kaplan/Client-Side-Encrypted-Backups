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

#ifndef FILEUTILITIES_H_INCLUDED
#define FILEUTILITIES_H_INCLUDED

/**********************/
/***** Namespaces *****/
/**********************/
using namespace std;

/********************/
/***** Includes *****/
/********************/
#include <string>

/*********************/
/***** Constants *****/
/*********************/
#define startUnderline  "\e[4m"
#define endUnderline    "\e[0m"
#define tab0            ""
#define tab1            "  "
#define tab2            "    "
#define tab3            "      "

/************************/
/***** Enumerations *****/
/************************/
typedef enum
{
    NEW_FILE = 2000,
    CONCATENATE_OTHERWISE_CREATE_NEW_FILE = 2001,
}FileWritingType;

/**********************/
/***** Structures *****/
/**********************/
typedef struct
{
    string basePath;
    string deleteAllFilesInTheBackupDirectory;
    string deleteAllFilesInTheRestoreDirectory;
    string theTarCommandIsDone;
    string killTheTarProcess;
    string fileThatContainsTheTarProcessId;
    string killTheTarStatusProcess;
    string fileThatContainsTheTarStatusProcessId;
    string killTheCcryptStatusProcess;
    string fileThatContainsTheCcryptStatusProcessId;
    string projectedSizeOfTheTarBallPath;
    string fileContainingTheBackupTarBallPath;
    string fileContainingTheEncryptedBackupPath;
    string usernameAndDomainPath;
    string landingDirectoryPath;
    string ccryptTitlePrintedPath;
    string ccryptFinishedGracefully;
    string binaryThatShowsTheTarStatus;
    string binaryThatShowsTheCcryptStatus;
    string processIdOfThisTerminalSessionPath;
    string resultsOfTarCommand;
    string sizeOfBackupThatIsADecryptedTarBallPath;
    string landingDirectory;
    string username;
    string domain;
    string unexpectedTarOutputPath;
}globalStringS;

#endif // FILEUTILITIES_H_INCLUDED
