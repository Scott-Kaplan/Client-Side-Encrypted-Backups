# Usage: sudo ./install.sh
#!/bin/bash

clear
echo

if [ "$(whoami)" != "root" ];
then
	echo "Because this script installs some dependencies and copies built binaries into \"/usr/local/bin\", you'll need to run as superuser."
  echo "Usage:"
  echo "  sudo ./install.sh"
  echo
  exit
fi

currentUser=$SUDO_USER

# If this installation has previously happened, then the "$HOME/.cloudbuddy/input" directory will already contain the user's settings.  In this case save a copy of these in case the user would like to restore them.
if [ -e "$HOME/.cloudbuddy/input/[1] search_this_list_for_changes" ]
then
	dirToCreate=$(date +"input__prior_to_%m.%d.%Y_%I:%M%p")
  mkdir -p $HOME/.cloudbuddy/$dirToCreate
  chown $currentUser:$currentUser $HOME/.cloudbuddy/$dirToCreate
  cp $HOME/.cloudbuddy/input/* $HOME/.cloudbuddy/$dirToCreate
fi

echo "Starting the Client-Side-Encrypted-Backups installation ..."
echo
echo "Creating the directories ..."
# if these directories already exist, -p suppreses the warning
mkdir -p $HOME/.cloudbuddy
chown $currentUser:$currentUser $HOME/.cloudbuddy
mkdir -p $HOME/.cloudbuddy/backup
chown $currentUser:$currentUser $HOME/.cloudbuddy/backup
mkdir -p $HOME/.cloudbuddy/input
chown $currentUser:$currentUser $HOME/.cloudbuddy/input
mkdir -p $HOME/.cloudbuddy/log
chown $currentUser:$currentUser $HOME/.cloudbuddy/log
mkdir -p $HOME/.cloudbuddy/restore
chown $currentUser:$currentUser $HOME/.cloudbuddy/restore
echo

echo "Creating the configuration files ..."
echo "Done.  These are located in $HOME/.cloudbuddy/input"

cat > "$HOME/.cloudbuddy/input/[1] search_this_list_for_changes" << EOF
# Place the full path of files or directories that you would like to have checked for changes, so that they can be backed up.
# If the path that you specify below is a directory, then all files at that level and below (i.e. recursive) will be checked.

# A change is considered to be a new or updated file that comes after the timestamp of the file 'dateMarker'.
# The file 'dateMarker' can be found in this directory.

# Any changed files found will be a part of the backup as long as they are not filtered out by one of the other config files.
# These 'filtering' config files are also in this directory.
  
# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# Below are some additional examples that are commented out.
# If you'd like to use them, just remove '#' from the start of the line.
# And you can also add as many additional ones as you need.

#$HOME/.bashrc
#/var/www
#/usr/lib/cgi-bin
#$HOME/Pictures
#$HOME/Videos
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[1] search_this_list_for_changes"

cat > "$HOME/.cloudbuddy/input/[2] username_and_domain" << EOF
# This file contains the username and domain of the server that you want your backups to be transferred to.
# You can use CloudBuddy (https:\\cloudbuddy.cloud) or any other ssh server.

# One line needs to be added to this file that meets this criteria-
# <user>@<domain-of-the-ssh-server>
#
# If you have a CloudBuddy account, <user> is the email address that you registered with.
# For example, if you registered with username@example.com, then the line that would need to be added to this file is -
#username@example.com@cloudbuddy.cloud

# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[2] username_and_domain"

cat > "$HOME/.cloudbuddy/input/[3] landing_directory" << EOF
# This file contains the directory that your backups will be sftp'd into on the server.
# One line identifying this needs to be added to this file.

# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# If you are using CloudBuddy as your backup server and you would like your backups to reside in your base directory,
# then just remove the '#' from the next line
#/uploads/

# If you are not using CloudBuddy as your backup server and you would like your backups to reside in your base directory,
# then just remove the '#' from the next line
#/
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[3] landing_directory"

cat > "$HOME/.cloudbuddy/input/[4] computer_name" << EOF
# This file contains the computer name that your doing the backup from.
# This will appear as part of the name of the backup.
# One line identifying this needs to be added to this file.

# Note:
# You can use anything for the computer name as long as it is made up from any of these:
# characters, numbers, hyphens, underscores, and has a length less than 32.

# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# As an example, lets say the computer that you are doing this backup from is a Dell Latitude 5530.
# If you wanted to use dell-latitude-5530 as your computer name then, just remove the '#' from the next line
#dell-latitude-5530
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[4] computer_name"

cat > "$HOME/.cloudbuddy/input/[5] don't_backup_files_that_start_with_this" << EOF
# This file contains the beginning of the path to files that you don't want backed up.
# Each should be placed on its own line.
 
# Here is an example-
# 
# In the config file '[1] search_this_list_for_changes', lets say that you have the following line:
# $HOME/temp
#
# and lets say that you have the following directories on your computer/device:
# $HOME/temp/keep_until_can_scrub
# $HOME/temp/throw_away
#
# and lets also say that you want to backup what's in 'keep_until_can_scrub' but not backup 'throw_away'.
# Then you would just remove '#' from the next line.
#$HOME/temp/throw_away

# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# Below are some additional examples that are commented out.
# If you'd like to use them, just remove '#' from the start of the line.
# And you are also encouraged to add as many additional ones as you need.

#.
#/lib
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[5] don't_backup_files_that_start_with_this"

cat > "$HOME/.cloudbuddy/input/[6] don't_backup_files_that_end_with_this" << EOF
# This file contains the ending of the path to files that you don't want backed up.
# Each should be placed on its own line.
 
# Here is an example-
# 
# In the config file '[1] search_this_list_for_changes', lets say that you have the following line:
# $HOME/eclipse
#
# and lets say that you have the following files on your computer/device:
# $HOME/eclipse/main.o
# $HOME/eclipse/fileIO.o
#
# and lets also say that you don't want any *.o files to be in the backup.
# Then you would just remove '#' from the next line.
#.o

# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# Below are some additional examples that are commented out.
# If you'd like to use them, just remove '#' from the start of the line.
# And you are also encouraged to add as many additional ones as you need.

#~
#.log
#.o
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[6] don't_backup_files_that_end_with_this"

cat > "$HOME/.cloudbuddy/input/[7] don't_backup_files_that_contain_this" << EOF
# This file contains any part of the path to files that you don't want backed up.
# Each string should be placed on its own line.

# Here is an example-
# 
# In the config file '[1] search_this_list_for_changes', lets say that you have the following line:
# $HOME/projects
#
# and lets say that you have the following files on your computer/device:
# $HOME/projects/cookiesOLD/user/jbk
# $HOME/projects/cookiesOLD/user/slk
#
# and lets also say that you don't want any files containing *cookiesOLD* in the path to be backed up.
# Then you would just remove '#' from the next line.
#cookiesOLD

# Note:
# Any line in this file that is blank or starts with a '#' is treated as a comment line and won't be processed.

# This is a temporary file that gets generated when you backup or restore, so there is no value in backing it up
fileContainingprocessIdOfThisTerminalSession

# Below are some additional examples that are commented out.
# If you'd like to use them, just remove '#' from the start of the line.
# And you are also encouraged to add as many additional ones as you need.

#/bin/
#.DS_Store
#.~lock.
#/.git/
EOF
chown $currentUser:$currentUser "$HOME/.cloudbuddy/input/[7] don't_backup_files_that_contain_this"

echo
echo "Creating the timeStampMarker file ..."
echo "Done.  This is located in $HOME/.cloudbuddy/input"
#touch $HOME/.cloudbuddy/input/timeStampMarker
touch --date=@0 $HOME/.cloudbuddy/input/timeStampMarker # set the timestamp to epoch
chown $currentUser:$currentUser $HOME/.cloudbuddy/input/timeStampMarker
echo

# The following software needs to be installed
# g++
# tree
# ccrypt
echo Installing the necessary software which is: g++, tree, ccrypt
echo
distribution=$(awk -F'=' '/^ID=/ {print tolower($2)}' /etc/*-release)
if [ $distribution = "arch" ]; then
	yes | pacman -S g++ tree ccrypt
elif [ $distribution = "centos" ]; then
	yum -y install g++ tree ccrypt
elif [ $distribution = "debian" ]; then
	apt-get install -y g++ tree ccrypt
elif [ $distribution = "fedora" ]; then
	apt-get install -y g++ tree ccrypt
elif [ $distribution = "ubuntu" ]; then
	apt-get install -y g++ tree ccrypt
else
  echo ERROR
	echo    Unable to detect your linux distribution.
	echo HOW TO FIX
	echo    You will need to install these: g++, tree, ccrypt
  echo    Once complete, there is no need to rerun this installation script since you will have everything installed.
  echo
  exit
fi
echo

# Verify that the necessary software was installed.  Othwise, exit this script
if (! which g++ | grep -q g++) ||
   (! which tree | grep -q tree) ||
   (! which ccrypt | grep -q ccrypt);
then
  echo ERROR: The necessary software was not installed.  Please fix the above.
  echo
fi

echo "Starting to build the Client-Side-Encrypted-Backups static libraries and binaries ..."
./build.sh

# exit if the following wasn't built
# ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a
# ./backup/bin/Release/backup
# ./actOnTarStatus/bin/Release/actOnTarStatus
# ./actOnCcryptStatus/bin/Release/actOnCcryptStatus
# ./restore/bin/Release/restore
# ./logSizeOfBackup/bin/Release/logSizeOfBackup
if [ ! -e ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a ] ||
   [ ! -e ./backup/bin/Release/backup ] ||
   [ ! -e ./actOnTarStatus/bin/Release/actOnTarStatus ] ||
   [ ! -e ./actOnCcryptStatus/bin/Release/actOnCcryptStatus ] ||
   [ ! -e ./restore/bin/Release/restore ] ||
   [ ! -e ./logSizeOfBackup/bin/Release/logSizeOfBackup ];
then
  # there is no need to display additional error messages here since build.sh displayed them
  exit
fi