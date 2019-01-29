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
#include <iomanip>
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

/*******************************/
/***** Function Prototypes *****/
/*******************************/
string getFormattedSizeOfBackup(double sizeOfBackup);

/*********************/
/***** Functions *****/
/*********************/
int main(int argc, char * const argv[])
{
    if (argc == 3)
    {
        /* read 2 command line arguments */
        string nameOfTheBackup = argv[1];
        string logFilePath = argv[2];

        /* get the size of the backup */
        double sizeOfBackup = getSizeOfFile(nameOfTheBackup);

        /* get a formatted readable size of the backup */
        string formattedSizeOfBackup = getFormattedSizeOfBackup(sizeOfBackup);

        /* write the formatted size of the backup to the completed backups */
        /* log file */
        ofstream logSizeOfBackupHandle;
        openForWriting(logFilePath,__FILE__,__LINE__,logSizeOfBackupHandle,
                       CONCATENATE_OTHERWISE_CREATE_NEW_FILE);
        logSizeOfBackupHandle<<"=> size: "<<formattedSizeOfBackup<<endl;
        logSizeOfBackupHandle.close();
    }
    return 0;
}

string getFormattedSizeOfBackup(double sizeOfBackup)
{
    ostringstream formattedSizeOfBackup;
    double sizeOfBackupInCategory = 0;

    if (sizeOfBackup < 1000)
    {
        formattedSizeOfBackup<<sizeOfBackup<<" bytes";
    }
    else if (sizeOfBackup < 1000000)
    {
        sizeOfBackupInCategory = sizeOfBackup/1000;
        // In the next line, setprecision(6) equates to a maximum of 6 digits
        // xxx.xxx while the minimum can be as few as 1 digit (x)
        formattedSizeOfBackup<<setprecision(6)<<sizeOfBackupInCategory<<" KB";
    }
    else if (sizeOfBackup < 1000000000)
    {
        sizeOfBackupInCategory = sizeOfBackup/1000000;
        formattedSizeOfBackup<<setprecision(6)<<sizeOfBackupInCategory<<" MB";
    }
    else if (sizeOfBackup < 1000000000000)
    {
        sizeOfBackupInCategory = sizeOfBackup/1000000000;
        formattedSizeOfBackup<<setprecision(6)<<sizeOfBackupInCategory<<" GB";
    }
    else if  (sizeOfBackup < 100000000000000)
    {
        sizeOfBackupInCategory = sizeOfBackup/1000000000000;
        formattedSizeOfBackup<<setprecision(6)<<sizeOfBackupInCategory<<" TB";
    }
    return formattedSizeOfBackup.str();
}
