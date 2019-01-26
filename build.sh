#clear
# Usage: sudo ./build.sh
#!/bin/bash
echo

if [ "$(whoami)" != "root" ];
then
	echo "Because this script copies built binaries into \"/usr/local/bin\", you'll need to run as superuser."
  echo "Usage:"
  echo "  sudo ./build.sh"
  echo
  exit
fi
  
echo Building the fileUtilities static library ...
cd ./staticLibrary__fileUtilities
rm -f bin/Release/libstaticLibrary__fileUtilities.a
g++ -Wall -O2  -c fileUtilities.cpp -o obj/Release/fileUtilities.o
ar -r -s bin/Release/libstaticLibrary__fileUtilities.a obj/Release/fileUtilities.o

echo
echo Building the backup binary ...
cd ../backup
rm -f bin/Release/backup
g++ -Wall -fexceptions -O2  -c backup.cpp -o obj/Release/backup.o
g++ -o bin/Release/backup obj/Release/backup.o -s  ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the actOnTarStatus binary ...
cd ../actOnTarStatus
rm -f bin/Release/actOnTarStatus
g++ -Wall -fexceptions -O2  -c actOnTarStatus.cpp -o obj/Release/actOnTarStatus.o
g++  -o bin/Release/actOnTarStatus obj/Release/actOnTarStatus.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the actOnCcryptStatus binary ...
cd ../actOnCcryptStatus
rm -f bin/Release/actOnCcryptStatus
g++ -Wall -fexceptions -O2  -c actOnCcryptStatus.cpp -o obj/Release/actOnCcryptStatus.o
g++  -o bin/Release/actOnCcryptStatus obj/Release/actOnCcryptStatus.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the restore binary ...
cd ../restore
rm -f bin/Release/restore
g++ -Wall -fexceptions -O2  -c restore.cpp -o obj/Release/restore.o
g++  -o bin/Release/restore obj/Release/restore.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the logSizeOfBackup binary ...
cd ../logSizeOfBackup
rm -f bin/Release/logSizeOfBackup
g++ -Wall -fexceptions -O2  -c logSizeOfBackup.cpp -o obj/Release/logSizeOfBackup.o
g++  -o bin/Release/logSizeOfBackup obj/Release/logSizeOfBackup.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a


# copy the binaries to /usr/local/bin/ if all of them build
# ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a
# ./backup/bin/Release/backup
# ./actOnTarStatus/bin/Release/actOnTarStatus
# ./actOnCcryptStatus/bin/Release/actOnCcryptStatus
# ./restore/bin/Release/restore
# ./logSizeOfBackup/bin/Release/logSizeOfBackup
echo
cd ..
if [ ! -e ./staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a ] ||
   [ ! -e ./backup/bin/Release/backup ] ||
   [ ! -e ./actOnTarStatus/bin/Release/actOnTarStatus ] ||
   [ ! -e ./actOnCcryptStatus/bin/Release/actOnCcryptStatus ] ||
   [ ! -e ./restore/bin/Release/restore ] ||
   [ ! -e ./logSizeOfBackup/bin/Release/logSizeOfBackup ];
then
  echo ERROR: The build failed.  Please fix above.
  echo
  echo
  exit
else
  cp ./backup/bin/Release/backup /usr/local/bin
  cp ./actOnTarStatus/bin/Release/actOnTarStatus /usr/local/bin
  cp ./actOnCcryptStatus/bin/Release/actOnCcryptStatus /usr/local/bin
  cp ./restore/bin/Release/restore /usr/local/bin
  cp ./logSizeOfBackup/bin/Release/logSizeOfBackup /usr/local/bin
fi