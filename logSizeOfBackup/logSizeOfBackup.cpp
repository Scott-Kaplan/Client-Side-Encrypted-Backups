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
#include <fstream>
#include <iostream>
#include <sstream>
#include "../staticLibrary__fileUtilities/fileUtilities.h"

/*******************************/
/***** From Static Library *****/
/*******************************/
extern "C" double getSizeOfFile(string &path);
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ifstream &readFileHandle);
extern "C" void getGlobalStrings(globalStringS &globalString, string &purpose);
extern "C" string getFormattedSizeOfBackup(double sizeOfBackup);

/*********************/
/***** Functions *****/
/*********************/
int main(int argc, char * const argv[])
{
    if (argc == 3)
    {
        string purpose="backup";  // for path $HOME/.cloudbuddy/backup/
        globalStringS globalString;
        getGlobalStrings(globalString,purpose);

        /* read 2 command line arguments */
        string nameOfTheBackup = argv[1];
        string destination = argv[2];

        /* get the size of the backup */
        double sizeOfBackup = getSizeOfFile(nameOfTheBackup);

        /* get a formatted readable size of the backup */
        string formattedSizeOfBackup = getFormattedSizeOfBackup(sizeOfBackup);

        if (destination == "terminal")
        {
            cout<<formattedSizeOfBackup<<endl;
        }
        else /* write to the completed_backups log file */
        {
            ofstream logSizeOfBackupHandle;
            openForWriting(destination,__FILE__,__LINE__,logSizeOfBackupHandle,
                           CONCATENATE_OTHERWISE_CREATE_NEW_FILE);

            // write the size of the backup
            logSizeOfBackupHandle<<"=> size: "<<formattedSizeOfBackup<<endl;

            // write the number of files contained in the backup
            ifstream numberOfFilesInBackupHandle;
            string numberOfFilesInTheBackup="";
            openForReading(globalString.totalFilesInBackupPath
                           ,__FILE__,__LINE__,
                           numberOfFilesInBackupHandle);
            getline(numberOfFilesInBackupHandle,numberOfFilesInTheBackup);
            logSizeOfBackupHandle<<"=> files: "<<numberOfFilesInTheBackup<<endl;
            numberOfFilesInBackupHandle.close();

            logSizeOfBackupHandle.close();
        }
    }
    return 0;
}
