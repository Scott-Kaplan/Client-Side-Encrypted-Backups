# Client Side Encrypted Backups
Easy to use and zero chance of corruption.
## Overview
There are a number of encrypted backup programs available, however most can't prevent corrupt backups.  Client-Side-Encrypted-Backups always produces reliable backups.  Since corruption is not possible with Client-Side-Encrypted-Backups, only one full backup is needed (the first time that you do a backup).
## Requirements
* A computer running Linux
* Access to a sftp server (in the cloud, at your office, in your house, anywhere).  This is were your backups will be stored.  If you don't have access to one, you can create an account and use [CloudBuddy](https://cloudbuddy.cloud).

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
1.  Enter your information in config files [1] - [4], which are located in- $HOME/.cloudbuddy/input
	* 	**Note: leave config files [5] - [7] blank for this step**
2.  Do a backup (see the **To backup** section above).  It will ask you if you approve of the files that will go into the backup.  If you see a file that you don't want to be in the backup, follow these steps:
	- 	select: `Do not continue with the backup`
	-   add it to one of these config files: [5], [6] or [7]
	-   run again

## Here To Help
I'm of the opinion that as much as possible of a person's private 'stuff'
should be encrypted so that it cannot be viewed by others.  I've been doing backups in this manner for a while
and decided to make it available so that others could use.

If you have questions, please contact me -

scott.kaplan@cloudbuddy.cloud
651-203-7092
