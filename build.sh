#clear
echo

echo Building the fileUtilities static library
cd ./staticLibrary__fileUtilities
g++ -Wall -O2  -c main.cpp -o obj/Release/main.o
rm -f bin/Release/libstaticLibrary__fileUtilities.a
ar -r -s bin/Release/libstaticLibrary__fileUtilities.a obj/Release/main.o

echo
echo Building the backup binary
cd ../backup
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++ -o bin/Release/backup obj/Release/main.o -s  ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the actOnTarStatus binary
cd ../actOnTarStatus
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/actOnTarStatus obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the actOnCcryptStatus binary
cd ../actOnCcryptStatus
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/actOnCcryptStatus obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo Building the restore binary
cd ../restore
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/restore obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
