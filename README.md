# Client Side Encrypted Backups
Easy to use and zero chance of corruption.
## Overview
There are a number of encrypted backup programs available, however most can't prevent the occasional creation of a corrupt backup.  Client-Side-Encrypted-Backups always produces reliable backups.  Since corruption is not possible with Client-Side-Encrypted-Backups, only one full backup is needed (the first time that you do a backup).
## Requirements
* A computer running Linux
* Access to a SFTP server (in the cloud, at your office, at your place, anywhere).  This is where your backups will be stored.

## To Backup
```console
user:~$ backup no-label|label-name
```
**Examples**
```console
user:~$ backup no-label
user:~$ backup secrets-of-the-world
```
**Examples of what the backup will be named on the server**
```bash
hp-desktop**2025-03-13__09:22pm
hp-desktop**secrets-of-the-world**2025-03-13__09:22pm
```
## To Restore
```console
user:~$ restore name-of-backup
```
**Examples**
```console
user:~$ restore hp-desktop**2025-03-13__09:22pm
user:~$ restore hp-desktop**secrets-of-the-world**2025-03-13__09:22pm
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

## Notes
* If you don't want to be prompted for your SFTP password every time that you do a backup (or a restore),
	then you can setup SSH public-key authentication.
* A log containing all files that you have backed up is located here: $HOME/.cloudbuddy/log/completed_backups
* By default, the first backup is a full backup and every future one thereafter is incremental.
* [ccrypt](http://ccrypt.sourceforge.net/) provides the encryption for Client-Side-Encrypted-Backups.
* (Ignore this section unless you want to alter the default behavior of Client-Side-Encrypted-Backups for some specific reason).  Files that are created or modified after the timestamp on the timeStampMarker file will go into your backup as per your config file 1 minus config files 5-7 settings.  Some examples below change the timestamp on the timeStampMarker file.
```bash
	# If you'd like a file to be in your next backup that hasn't changed since the previous backup
	# you can include it by refreshing it's timestamp
	touch path/file-name
	
	# set the timestamp of the timeStampMarker file to Epoch ()
	touch --date=@0 $HOME/.cloudbuddy/input/timeStampMarker

	# set the timestamp of the timeStampMarker file to 10pm on 1/1/2000
	touch -d "01 Jan 2000 20:00:00" $HOME/.cloudbuddy/input/timeStampMarker

	# set the timestamp of the timeStampMarker file to 12:30am on 3/27/2018
	touch -d "27 Mar 2018 00:30:00" $HOME/.cloudbuddy/input/timeStampMarker
```
