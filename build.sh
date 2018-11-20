clear
echo

echo building the fileUtilities static library
cd ./staticLibrary__fileUtilities
g++ -Wall -O2  -c main.cpp -o obj/Release/main.o
rm -f bin/Release/libstaticLibrary__fileUtilities.a
ar -r -s bin/Release/libstaticLibrary__fileUtilities.a obj/Release/main.o

echo
echo building the backup binary
cd ../backup
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++ -o bin/Release/backup obj/Release/main.o -s  ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo building the actOnTarStatus binary
cd ../actOnTarStatus
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/actOnTarStatus obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo building the actOnCcryptStatus binary
cd ../actOnCcryptStatus
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/actOnCcryptStatus obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
echo building the restore binary
cd ../restore
g++ -Wall -fexceptions -O2  -c main.cpp -o obj/Release/main.o
g++  -o bin/Release/restore obj/Release/main.o -s ../staticLibrary__fileUtilities/bin/Release/libstaticLibrary__fileUtilities.a

echo
