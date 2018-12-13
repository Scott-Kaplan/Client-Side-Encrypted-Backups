# Usage: sudo ./install.sh

clear
echo

LOH
# If this installation has occurred before, the "$HOME/.cloudbuddy/input" directory will already contain the users settings.  Save these off into "./inputDir_as_of_mm.dd.yy_mm_ss otherwise the settings will be lost forever.

echo "Starting the Client-Side-Encrypted-Backups installation ..."
echo
echo "Creating the directories ..."
mkdir -p $HOME/.cloudbuddy
mkdir -p $HOME/.cloudbuddy/backup
mkdir -p $HOME/.cloudbuddy/input
mkdir -p $HOME/.cloudbuddy/log
mkdir -p $HOME/.cloudbuddy/restore
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

# Below are some additional examples that are commented out.
# If you'd like to use them, just remove '#' from the start of the line.
# And you are also encouraged to add as many additional ones as you need.

#/bin/
#.DS_Store
#.~lock.
EOF

echo
echo "Creating the timeStampMarker file ..."
echo "Done.  This is located in $HOME/.cloudbuddy/input"
touch $HOME/.cloudbuddy/input/timeStampMarker
echo

echo "Starting to build the Client-Side-Encrypted-Backups static libraries and binaries ..."
./build.sh

# copy the backup and restore binaries to /usr/local/bin/ if all of these build
# ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a
# ./backup/bin/Release/backup
# ./actOnTarStatus/bin/Release/actOnTarStatus
# ./actOnCcryptStatus/bin/Release/actOnCcryptStatus
# ./restore/bin/Release/restore
if [ ! -e ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a ] ||
   [ ! -e ./backup/bin/Release/backup ] ||
   [ ! -e ./actOnTarStatus/bin/Release/actOnTarStatus ] ||
   [ ! -e ./actOnCcryptStatus/bin/Release/actOnCcryptStatus ] ||
   [ ! -e ./restore/bin/Release/restore ];
then
  echo
  echo The build failed.  Once you have fixed the errors, rerun the install script again
  echo
  echo
  exit
else
  cp ./backup/bin/Release/backup /usr/local/bin
  cp ./actOnTarStatus/bin/Release/actOnTarStatus /usr/local/bin
  cp ./actOnCcryptStatus/bin/Release/actOnCcryptStatus /usr/local/bin
  cp ./restore/bin/Release/restore /usr/local/bin
fi

# The following software needs to be installed
# g++
# ccrypt
echo Installing the necessary software, which is: g++, ccrypt, ...
echo
# Note: This next line assumes that you are running on Ubuntu.  If not, adjust for your distribution
sudo apt-get install -y g++ ccrypt
echo

# Verify that the necessary software was installed.  Othwise, exit this script
if (! which g++ | grep -q g++) ||
   (! which ccrypt | grep -q ccrypt);
then
  echo The necessary software was not installed.  Once you have fixed the errors, rerun the install script again.
  echo
fi


















































