#
# CMakeList.txt : CMake project for Compilers, include source and define project specific logic here.
#
#echo -e "\t==================================================="
#echo -e "\t|                 NIVLang Logo                    |"
#echo -e "\t==================================================="
#echo -e "\tNNNNNNNN        NNNNNNNNVVVVVVVV           VVVVVVVV"
#echo -e "\tN:::::::N       N::::::NV::::::V           V::::::V"
#echo -e "\tN::::::::N      N::::::NV::::::V           V::::::V"
#echo -e "\tN:::::::::N     N::::::NV::::::V           V::::::V"
#echo -e "\tN::::::::::N    N::::::N V:::::V           V:::::V "
#echo -e "\tN:::::::::::N   N::::::N  V:::::V         V:::::V  "
#echo -e "\tN:::::::N::::N  N::::::N   V:::::V       V:::::V   "
#echo -e "\tN::::::N N::::N N::::::N    V:::::V     V:::::V	   "
#echo -e "\tN::::::N  N::::N:::::::N     V:::::V   V:::::V	   "
#echo -e "\tN::::::N   N:::::::::::N      V:::::V V:::::V	   "
#echo -e "\tN::::::N    N::::::::::N       V:::::V:::::V	   "
#echo -e "\tN::::::N     N:::::::::N        V:::::::::V		   "
#echo -e "\tN::::::N      N::::::::N         V:::::::V		   "
#echo -e "\tN::::::N       N:::::::N          V:::::V		   "
#echo -e "\tN::::::N        N::::::N           V:::V		   "
#echo -e "\tNNNNNNNN         NNNNNNN            VVV			   "
#echo -e "\t==================================================="
#e                                      "

cmake_minimum_required (VERSION 3.8)

project ("Compilers")

# Add source to this project's executable: check name and dependences
add_executable (Compilers
	"Compilers.h"	"Compilers.c"
	"Reader.h"		"Reader.c"		"MainReader.c"
	"Scanner.c"		"MainScanner.c" "Scanner.h"
	#"Parser.h"		"Parser.c"		"MainParser.c"
 "MainScanner.c" "Scanner.c" "Scanner.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Compilers PROPERTY CXX_STANDARD 20)
endif()

# TO_DO: Add tests and install targets if needed.
