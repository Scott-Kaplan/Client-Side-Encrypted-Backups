# Client Side Encrypted Backups

Easy to use and zero chance of corruption

## Overview

There are a number of encrypted backup programs available, however most can't prevent against the accidental creation of a bad (corrupt) backup.  Client-Side-Encrypted-Backups always produces reliable backups.  Since corruption is not possible when using Client-Side-Encrypted-Backups, only one full backup is needed (the first time you do a backup).

## Requirements

* A computer running Linux
* Access to a sftp server (in the cloud, at your office, in your house, anywhere).  This is were your backups will be stored.  If you don't have access to one, you can create an account on [CloudBuddy](https://cloudbuddy.cloud).

## To backup
```console
user:~$ backup label-name|no-label
```
**Examples**
```console
user:~$ backup no-label
user:~$ backup secrets-of-the-world
```
**Examples of what you'll see on the server**
```bash
dell-laptop**2019-03-21__12:28am
dell-laptop**secrets-of-the-world**2019-03-21__12:28am
```
## To restore
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
	* 	git clone https://github.com/Scott-Kaplan/Client-Side-Encrypted-Backups.git
2.  cd into the **Client-Side-Encrypted-Backups** directory
3.  sudo ./install.sh (Note: 'sudo' is required because all built binaries are copied to /usr/local/bin)
