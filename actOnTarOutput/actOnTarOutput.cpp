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
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../staticLibrary__fileUtilities/fileUtilities.h"

/*******************************/
/***** From Static Library *****/
/*******************************/
extern "C" void openForReading(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ifstream &readFileHandle);
extern "C" void getGlobalStrings(globalStringS &globalString, string &purpose);

// On Fedora when doing a backup and tar'ing both .bashrc and .bashrc1 it
// returns this -
//tar: Removing leading `/' from member names
//tar: Removing leading `/' from hard link targets
//
// but when just .bashrc is in the backup, it returns just this one line -
//tar: Removing leading `/' from member names
//
// As a result cannot check the size of the tar output because it's variable
// and cannot say what is nominal output because it varies.
// So check every line of the tar output.  If a line has
// "tar: Removing leading" it is ok.  Otherwise create a file indicator so that
// the backup binary can inform the user that the tar output is unexpected

int main(int argc, char * const argv[])
{
    if (argc == 1)
    {
        string purpose="backup";  // for path $HOME/.cloudbuddy/backup/
        globalStringS globalString;
        getGlobalStrings(globalString,purpose);
        string line="";
        ifstream tarResultsHandle;
        openForReading(globalString.resultsOfTarCommand
                       ,__FILE__
                       ,__LINE__,
                       tarResultsHandle);
        while(getline(tarResultsHandle,line))
        {
            if (line.find("tar: Removing leading") == string::npos)
            {
                string createFileCmd =
                                "touch "+globalString.unexpectedTarOutputPath;
                if(system(createFileCmd.c_str()));
                exit(EXIT_SUCCESS);
            }
        }
        tarResultsHandle.close();
    }
    return 0;
}
