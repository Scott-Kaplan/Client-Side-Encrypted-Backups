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
#include <stdlib.h>
#include "../staticLibrary__fileUtilities/fileUtilities.h"

/****************************/
/***** Static Libraries *****/
/****************************/
extern "C" string getTimestamp();
extern "C" void retrieveUsernameAndDomain
            (string &username, string &domain, string &usernameAndDomainPath);
extern "C" void getGlobalStrings(globalStringS &globalString,string &purpose);
extern "C" bool fileExist(string &lookupFilePath,
                          string fromFileName,
                          int fromLineNumber,
                          string resultsDirectory);
extern "C" void clearTheTerminalWindow();
extern "C" void openForWriting(string &path,
                               string fromFileName,
                               int fromLineNumber,
                               ofstream &writeFileHandle,
                               FileWritingType FileWritingType);
void createRestoreDirectory();
extern "C" string getFileName(string &path);
extern "C" void extractPathAndFileName
                        (string &stringToParse, string &path, string &fileName);
extern "C" void writeCleanUpAndExitFunction
                (string directory, ofstream &scriptThatRestoresTheBackupHandle);
extern "C" void writeCleanUpFunction
                (string directory, ofstream &scriptThatRestoresTheBackupHandle);
extern "C" void clearTheTerminalWindow();
extern "C" void saveTheTerminalPid(string &purpose);
extern "C" void convert$HOME(string &path);
extern "C" void checkThatConfigurationFileHasBeenInstalled(string &path,
                                                           string &purpose);
extern "C" void displayCommandLineArgumentsAreWrong(int argc,
                                                    char * const argv[],
                                                    string &purpose);

/*******************************/
/***** Function Prototypes *****/
/*******************************/
void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[]);
void displayUsage();
void createAScriptTheWillRestoreTheBackup();
void runTheScriptThatRestoresTheBackup();
void deleteAllFilesIntheRestoreDirectory();
void checkTheConfigurationFileIntegrity();

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
string expectedSizeOfTheOutputFileFromRunningTheTarCommand = "0";
string expectedOutputFromRunningTheTarCommand = "";

/*********************/
/***** Functions *****/
/*********************/
int main(int argc, char * const argv[])
{
    clearTheTerminalWindow();
    checkThatTheCommandLineArgumentsAreCorrect(argc,argv);
    getGlobalStrings(globalString,purpose);
    deleteAllFilesIntheRestoreDirectory();
    checkTheConfigurationFileIntegrity();
    saveTheTerminalPid(purpose);
    createAScriptTheWillRestoreTheBackup();
    runTheScriptThatRestoresTheBackup();
    return 0;
}

void deleteAllFilesIntheRestoreDirectory()
{
    string filesToBeDeleted="$HOME/.cloudbuddy/restore/*";
    convert$HOME(filesToBeDeleted);
    string deleteFilesCommand="rm -f "+filesToBeDeleted;
    if(system(deleteFilesCommand.c_str()));
}

void checkTheConfigurationFileIntegrity()
{
    checkThatConfigurationFileHasBeenInstalled(globalString.usernameAndDomainPath,purpose);
    retrieveUsernameAndDomain(username,domain,globalString.usernameAndDomainPath);
}

void checkThatTheCommandLineArgumentsAreCorrect(int argc, char * const argv[])
{
    if (argc != 2)
    {
        // Examples of what is expected
        // > restore /uploads/e6230**2018-06-26__09:26pm
        // > restore /uploads/e6230**pics-of-aunt-mary**2018-06-26__09:26pm
        displayCommandLineArgumentsAreWrong(argc,argv,purpose);
        displayUsage();
    }

    /* the expected number of parameters (1) were passed in, so continue */
    /* with checking the validity of the parameter. */
    nameOfEncryptedBackupWhichIncludesPath = argv[1];

    /* display what the user typed in to launch*/
    cout<<"> restore "<<nameOfEncryptedBackupWhichIncludesPath<<endl;

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

    createRestoreDirectory();

    /***** IMPORTANT NOTE *****/
    /***** Later on in this program a "rm -rf restorePath" is performed.  *****/
    /***** So, if you need to change this be extremely careful.           *****/
    string restorePath="$HOME/restored-from-backup/"+nameOfEncryptedBackup;

    ofstream scriptThatRestoresTheBackupHandle;
    openForWriting(scriptThatRestoresTheBackupPath,__FILE__,__LINE__,
                   scriptThatRestoresTheBackupHandle,NEW_FILE);
    scriptThatRestoresTheBackupHandle
    <<tab0<<"#!/bin/bash"<<endl<<endl;

    writeCleanUpFunction(purpose,scriptThatRestoresTheBackupHandle);
    writeCleanUpAndExitFunction(purpose,scriptThatRestoresTheBackupHandle);
    scriptThatRestoresTheBackupHandle

    /* capture Ctrl-C if the user presses it */
    <<tab0<<"trap cleanUpAndExit SIGINT"<<endl<<endl

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
    <<tab1<<"cleanUpAndExit"<<endl
    <<tab0<<"fi"<<endl<<endl

    /* hide the cursor as the cursor is distracting while viewing the */
    /* progress meter */
    <<tab0<<"tput civis"<<endl<<endl

    /* Kick off a background process that runs every second that displays the */
    /* ccrypt command progress */
    <<tab0<<"(while sleep 1; "
    <<"do "<<globalString.binaryThatShowsTheCcryptStatus<<" "<<purpose<<"; "
    <<"done & echo $! > "<<globalString.fileThatContainsTheCcryptStatusProcessId
    <<")"<<endl<<endl

    /* rename the encrypted backup to have the natural .cpt extension on it */
    /* the reason for doing this is to have the ability to exit the script */
    /* if the user enters the wrong decryption password and is therefore */
    /* unable to decrypt their backup */
    <<"mv "<<nameOfEncryptedBackup<<" "<<nameOfEncryptedBackup<<".cpt"<<endl

    /* decrypt the backup */
    // Note: With the below decryption command, the temporary file created
    // (after it has started & before it finishes) has a different suffix each
    // time the decryption command is run. There will be two files when not done
    // and one once decryption finishes.
    <<tab0<<"echo \"About to start decrypting your backup ...\""<<endl
    <<tab0<<"echo"<<endl<<"ccrypt -d -f -T "<<nameOfEncryptedBackup<<".cpt"<<endl<<endl

    /* Ensure the user sucessfully decrypted the backup.  If not, mention it */
    /* and exit the script.  Note that the .cpt is dropped off the backup */
    /* only after it has been decrypted*/
    <<tab0<<"if [ ! -f \""<<nameOfEncryptedBackup<<"\" ]; then "<<endl
    <<tab1<<"echo"<<endl
    <<tab1<<"cleanUpAndExit"<<endl
    <<tab0<<"fi"<<endl<<endl
    <<tab0<<"while [ ! -f \""<<globalString.ccryptFinishedGracefully<<"\" ]; do"
    <<endl
    <<tab1<<"sleep 1"<<endl
    <<tab0<<"done"<<endl<<endl

    /* bring the cursor back now that ccrypt is done */
    <<tab0<<"tput cnorm"<<endl
    <<tab0<<"echo"<<endl

    <<tab0<<"echo \"About to start restoring your backup ...\""<<endl

    /* save the size of the tar ball.  This is to be used to calculate the */
    /* percentage complete while extracting the tar ball by the */
    /* actOnTarStatus binary */
    <<tab0<<"stat -c %s "<<restoreDirectory<<nameOfEncryptedBackup<<" > "
          <<globalString.sizeOfBackupThatIsADecryptedTarBallPath<<endl
    <<tab0<<"echo"<<endl

    /* save the name of the restore path to a file */
    <<tab0<<"echo "<<restorePath<< " > "<<globalString.basePath+"restorePath"<<endl

    /* create destination directory that the backup will be restored in */
    // It is necessary to delete and start from nothing when restoring a backup
    // because untar progress % complete will initially show greater than
    // 100% otherwise
    <<tab0<<"rm -rf "<<restorePath<<endl
    <<tab0<<"mkdir "<<restorePath<<endl<<endl

    /* untar the backup.  Do this in the background so the tar command */
    /* progress (percentage complete) can be given in the foreground */
    // example:
    // (tar jxf nameOfEncryptedBackup --force-local -C restorePath >
    // resultsPath 2>&1 & echo $! > pathThatContainsTheTarProcessId)
    <<tab0<<"(tar jxf "<<nameOfEncryptedBackup
    // the next line overides the limitation of not being allowed to include
    // one or more colons in the name of the tarball
    <<" --force-local"
    // the next line restores the backup to the specified location
    <<" -C "<<restorePath
    // the next line sends all output from executing the tar command to a file
    // so the output can be analyzed to see if tar returned something unexpected
    <<" > "<<globalString.resultsOfTarCommand<<" 2>&1"
    // This next line saves the tar Process Id from executing the tar command.
    // When untar'ing large tarballs the pecentage complete is very useful,
    // otherwise the user just sees their session hanging and doesn't really
    // know if or when untaring will finish.
    <<" & echo $! > "<<globalString.fileThatContainsTheTarProcessId<<")"
    <<endl<<endl

    <<tab0<<"echo \""<<startUnderline
    <<"Progress of restoring your backup"<<endUnderline<<"\""<<endl

    /* hide the cursor as the cursor is distracting while viewing the */
    /* progress meter */
    <<tab0<<"tput civis"<<endl<<endl

    <<tab0<<"(while sleep 1; "
    <<"do "<<globalString.binaryThatShowsTheTarStatus<<" "<<purpose<<"; "
    <<"done & echo $! > "<<globalString.fileThatContainsTheTarStatusProcessId
    <<")"<<endl<<endl

    /* get the total backup from running tar once it has finished */
    <<tab0<<"while sleep 1; do"<<endl

    /* check that the file theTarCommandIsDone exists.  If so, analyze the */
    /* output of the tar command */
    <<tab1<<"if [ -e \""<<globalString.theTarCommandIsDone<<"\" ]; then"<<endl

    /* bring the cursor back now that tar is done */
    <<tab2<<"tput cnorm"<<endl

    /* Get the size of the output from tar.  If it is different than expected */
    /* which is "Removing leading `/' from member names", as the user whether */
    /* he/she wants to proceed with the backup.  If it's the same display,    */
    /* 100% complete */
    <<tab2<<"sizeOfFile=$(stat -c%s "<<globalString.resultsOfTarCommand<<")"<<endl
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
    <<tab3<<"while read line; do echo \"$line\"; done < "<<globalString.resultsOfTarCommand<<endl
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
    <<tab1<<"fi"<<endl // done with analyzing the output of the tar command

    <<tab0<<"done"<<endl<<endl

    /* Let the user know that the backup restored successfully */
    <<tab0<<"echo \""<<startUnderline<<"Your backup was successfully restored"
    <<endUnderline
    <<"\""<<endl
    <<tab0<<"echo \""<<"It can be found in "<<restorePath<<"\""<<endl
    <<tab0<<"echo"<<endl

    <<tab0<<"cleanUp"<<endl;

    scriptThatRestoresTheBackupHandle.close();
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
