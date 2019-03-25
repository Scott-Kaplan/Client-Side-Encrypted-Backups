# Client Side Encrypted Backups
Easy to use and zero chance of corruption.
## Overview
There are a number of encrypted backup programs available, however most can't prevent corrupt backups.  Client-Side-Encrypted-Backups always produces reliable backups.  Since corruption is not possible with Client-Side-Encrypted-Backups, only one full backup is needed (the first time that you do a backup).
## Requirements
* A computer running Linux
* Access to a SFTP server (in the cloud, at your office, in your house, anywhere).  This is were your backups will be stored.  If you don't have access to one, you can create an account and use [CloudBuddy](https://cloudbuddy.cloud).

## To Backup
```console
user:~$ backup label-name|no-label
```
**Examples**
```console
user:~$ backup no-label
user:~$ backup secrets-of-the-world
```
**Examples of what the backups will be named on the server**
```bash
dell-laptop**2019-03-21__12:28am
dell-laptop**secrets-of-the-world**2019-03-21__12:28am
```
## To Restore
```console
user:~$ restore name-of-backup
```
**Examples**
```console
user:~$ restore dell-laptop**2019-03-21__12:28am
user:~$ restore dell-laptop**secrets-of-the-world**2019-03-21__12:28am
```
## Installation
1.  Clone the repo
	* 	**git clone https://github.com/Scott-Kaplan/Client-Side-Encrypted-Backups.git**
2.  **cd Client-Side-Encrypted-Backups**
3.  **sudo ./install.sh** (Note: 'sudo' is required because all built binaries are copied to /usr/local/bin)

## Setup Instructions
1.  Enter your information in config files [1] - [4], which are located in- $HOME/.cloudbuddy/input<br>
	  **Note: leave config files [5] - [7] blank for this step**
2.  Do a backup (see the **To Backup** section above for usage).  It will ask you if you approve of the files that will go into the backup.  If you see a file that you don't want to be in the backup, follow these steps:
	- 	select: `Do not continue with the backup`
	-   add it to one of these config files: [5], [6] or [7]
	-   run again

## Here To Help
I'm of the opinion that as much as possible of a person's private 'stuff'
should be encrypted so that it cannot be viewed by others.  I've been doing backups in this manner for a while
and decided to make it available so that others could use.

If you have questions, please contact me -

scott.kaplan@cloudbuddy.cloud<br>
651-203-7092

## Notes
* SFTP password - If you don't want to be prompted every time you do a backup (or a restore),
	then you can setup SSH public-key authentication.  If you are using CloudBuddy to store your backups, those one time setup instructions are located [here](https://cloudbuddy.cloud/how-to-use.html).
* A log containing all the files that you have backed up will be located here- $HOME/.cloudbuddy/log/completed_backups
* By default, the first backup is a full backup and every future one thereafter is incremental.
* [ccrypt](http://ccrypt.sourceforge.net/) provides the encryption that Client-Side-Encrypted-Backups uses.
* Here are some examples if you would like to change the timestamp of the timeStampMarker file -<br>
	**Note**:  This will alter the default behavior of what files will be considered new or changed that will go into the backup.
```bash
	# If you want a file to be in your next backup that hasn't changed since the previous backup, you can include it by refreshing it's timestamp
	touch path/file-name
	
	# Set the timestamp to Epoch ()
	touch --date=@0 $HOME/.cloudbuddy/input/timeStampMarker

	# Set the timestamp to 1/1/2000, 10pm
	touch -d "01 Jan 2000 20:00:00" $HOME/.cloudbuddy/input/timeStampMarker

	# Set the timestamp to 3/27/2018, 12:30am
	touch -d "27 Mar 2018 00:30:00" $HOME/.cloudbuddy/input/timeStampMarker
```
