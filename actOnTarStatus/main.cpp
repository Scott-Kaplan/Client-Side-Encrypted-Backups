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
#include "../staticLibrary__fileUtilities/fileUtilities.h"
#include <stdlib.h>
#include <fstream>
#include <sstream> // needed for ostringstream
#include <iomanip> // setw() - sets column width of a field
#include <sys/stat.h> // support for stat()
//#include <error.h>

/********************************/
/******* Static Libraries *******/
/********************************/
extern "C" void getGlobalStrings(globalStringS &globalString,string &purpose);
extern "C" void openForReading(string &path,
                    string fromFileName,
                    int lineNumber,
                    ifstream &readFileHandle);
extern "C" double getSizeOfFile(string &path);

/*******************************************/
/********** File Scope Variables ***********/
/*******************************************/
globalStringS globalString;
string theBackupPath="";
string estimatedSizeOfTheTarBall="";
string purpose="";

/***************************************/
/********* Function Prototypes *********/
/***************************************/
void checkTheCommandLineArguments(int argc, char * const argv[]);
void actOnTarCommand();
void display(double tarPercentageComplete);

/*****************************/
/********* Functions *********/
/*****************************/
int main(int argc, char * const argv[])
{
    checkTheCommandLineArguments(argc,argv);
    getGlobalStrings(globalString,purpose);
    actOnTarCommand();
    return 0;
}

void checkTheCommandLineArguments(int argc, char * const argv[])
{
    // no arguments are expected to be passed in
    //cout << "argc = " << argc << endl;
    //for(int i = 0; i < argc; i++) cout << "argv[" << i << "] = " << argv[i] << endl;
//    if (argc != 1)

    /* one parameter is expected to be passed in */
    if (argc != 2)
    {
        exit(EXIT_SUCCESS);
    }
    purpose = argv[1];
}

void actOnTarCommand()
{
    /* get the tar process id */
    string tarProcessId="";
    ifstream fileThatContainsTheTarProcessIdHandle;
    openForReading(globalString.fileThatContainsTheTarProcessId,
                   __FILE__,
                   __LINE__,
                   fileThatContainsTheTarProcessIdHandle);
    getline(fileThatContainsTheTarProcessIdHandle,tarProcessId);
    fileThatContainsTheTarProcessIdHandle.close();
    //cout<<endl<<"tarProcessId = "<<tarProcessId<<endl<<endl;

    /* determine if tar is still running */
    struct stat sts;
    string processPath="/proc/"+tarProcessId;
    if (stat(processPath.c_str(),&sts) == -1)
    //if (stat(processPath.c_str(),&sts) == -1 && errno == ENOENT)
    {
        /* tar finished */
        //cout<<"tar is done"<<endl;

        // Notes on all the possible scenarios

        // Case 1
        // The user closed the terminal window before the tar command could
        // finish. There is no further action in this binary since the terminal
        // window no longer exists.

        // Case 2
        // The tar command finished successfully.  This is known by checking the
        // output of tar in the 'backup' program and getting the expected
        // output.  It displays 100% complete in the 'backup' program once it
        // finishes successfully.  There is no further action in this binary.

        // Case 3
        // The tar command did not finish successfully.  This is known by
        // checking the output of tar in the 'backup' program and getting
        // unexpected output.  It does not display 100% complete even if the
        // user elects to continue.  If they want to cancel the backup, the
        // entire backup directory contents are deleted and the process id of
        // both tar and ccrypt are killed if either were active.  There is no
        // further action in this binary.

        // Case 4
        // The user pressed Ctrl-C while the tar command was in process
        // This does not close the terminal window.  Instead the behavior is
        // to cleanup and terminate the script.
        // There is no further action in this binary since this is caught in
        // the 'backup' program

        /* kill this tar status process */
        //cout<<endl<<globalString.killTheTarStatusProcess<<endl;
        if(system(globalString.killTheTarStatusProcess.c_str()));

        /* create an empty theTarCommandIsDone file, this acts as a marker */
        /* so that once the tar command finishes, the 'backup' and 'restore' */
        /* programs can analyze the output of the tar command */
        string createDoneWithTarCmd="touch "+globalString.theTarCommandIsDone;
        if(system(createDoneWithTarCmd.c_str()));

    }
    else if (purpose == "backup")
    //else - this worked before for backup but is not sufficient for restore.  that's why needed the else if above
    {
        /* tar is still running */
        //cout<<"tar is still running"<<endl;

        /* extract the name of the backup with path, this will always exist even */
        /* when its finished, so no need to worry about when tar finishes and  */
        /* there is no process id */
        string theBackupPath="";
        ifstream fileContainingTheNameOfTheBackupWithPathHandle;
        openForReading(
            globalString.fileContainingTheBackupTarBallPath
           ,__FILE__,__LINE__,fileContainingTheNameOfTheBackupWithPathHandle);
        getline(fileContainingTheNameOfTheBackupWithPathHandle,theBackupPath);
        fileContainingTheNameOfTheBackupWithPathHandle.close();

        /* get the current size of the Tar Ball */
        double currentSizeOfTheTarBall = getSizeOfFile(theBackupPath);
        //dcout<<endl<<"string 3 ="<<currentSizeOfTheTarBall<<endl;

        /* extract the projected size of the tar ball */
        string projectedSizeOfTheTarBall="";
        ifstream projectedSizeOfTheTarBallHandle;
        openForReading(
            globalString.projectedSizeOfTheTarBallPath
           ,__FILE__,__LINE__,projectedSizeOfTheTarBallHandle);
        getline(projectedSizeOfTheTarBallHandle,projectedSizeOfTheTarBall);
        projectedSizeOfTheTarBallHandle.close();
        //dcout<<projectedSizeOfTheTarBall<<endl;

        /* divide current size by projected size */
        double projectedSizeOfTheTarBallDouble =
                                        atof(projectedSizeOfTheTarBall.c_str());
        double tarPercentageComplete =
                (currentSizeOfTheTarBall/projectedSizeOfTheTarBallDouble)*100;

        /* display the tar percentage complete */
        display(tarPercentageComplete);
    }
    else // purpose == "restore"
    {
        // todo: if what is being restored is large and has lots of directories
        //       the below may not finish within 1 second.
        //       examples)
        //       [1]
        //       did 6gb directory ($HOME/temp/test/test).  it had very few
        //       directories so it easily finsihed within 1 second
        //       [2]
        //       did 64Gb directory ($HOME/temp) which had many subdirectories
        //       from the command line, this took many seconds.  by running the
        //       below on such a large directory the behavior of the below code
        //       is unknown.  At best it just might not print any % complete
        //       at each second, however than the user may deduce that it is
        //       hung when its not.  At worst, it would print something unknown
        //       consider testing this.
        //       Tested with 150MB backup.  it worked perfect.  Its possible
        //       that even with large backups with lots of directories that this
        //       would work fine since it would know what it just restored it
        //       wouldn't have to think about it - the same way as du -sb command
        //       twice on $HOME/temp

        /* get the total size of the backup that needs to be restored */
        /* this is to be used to compute the percentage untar completed */
        ifstream sizeOfDecryptedBackupHandle;
        string totalSizeToBeRestored="";
        string alreadyDecryptedBackupPath =
                    globalString.basePath+"sizeOfBackupThatIsADecryptedTarBall";
        openForReading(alreadyDecryptedBackupPath,__FILE__,__LINE__,
                       sizeOfDecryptedBackupHandle);
        getline(sizeOfDecryptedBackupHandle,totalSizeToBeRestored);
        //cout<<"totalSizeToBeRestored = "<<totalSizeToBeRestored<<endl;
        sizeOfDecryptedBackupHandle.close();

        /* get the restore path */
        ifstream restorePathHandle;
        string restorePath="";
        string fileThatHoldsTheRestorePath=globalString.basePath+"restorePath";
        openForReading(fileThatHoldsTheRestorePath,__FILE__,__LINE__,
                       restorePathHandle);
        getline(restorePathHandle,restorePath);
        //cout<<"restorePath = "<<restorePath<<endl;
        restorePathHandle.close();

        /* save the size of the backup that has been restored to this point */
        // example) //du -sb e6230-primary\*\*2018-10-02__05\:47am/ | awk '{ print $1 }' > sizeOfDirectory
        string sizeThatsBeenRestoredToThisPointPath=globalString.basePath+"sizeRestoredToThisPoint";
        string cmd="du -sb "+restorePath+" | awk '{ print $1 }' > "+sizeThatsBeenRestoredToThisPointPath;
        //cout<<cmd<<endl;
        if(system(cmd.c_str()));

        /* get the size of the backup that has been restored to this point */
        ifstream sizeThatsBeenRestoredHandle;
        string sizeThatsBeenRestored="";
        openForReading(sizeThatsBeenRestoredToThisPointPath,__FILE__,__LINE__,sizeThatsBeenRestoredHandle);
        getline(sizeThatsBeenRestoredHandle,sizeThatsBeenRestored);
        //cout<<"sizeThatsBeenRestored = "<<sizeThatsBeenRestored;
        sizeThatsBeenRestoredHandle.close();

        /* convert the numerical strings to doubles */
        double totalSizeToBeRestoredDouble = atof(totalSizeToBeRestored.c_str());
        double sizeThatsBeenRestoredDouble = atof(sizeThatsBeenRestored.c_str());

        /* display the untar percentage complete */
        display((sizeThatsBeenRestoredDouble/totalSizeToBeRestoredDouble)*100);
    }
}

void display(double tarPercentageComplete)
{
    ostringstream tarPercentageCompleteString;
    tarPercentageCompleteString <<fixed<<setprecision(4)
                                <<tarPercentageComplete<<"%";
    cout
        //                 "xx.xxxx%       " - length of 15
        <<left<<setw(15)<<tarPercentageCompleteString.str()
        <<"\r";//<<endl;
    cout.flush();
}
