# ad-block-filter-serializer

Serializes ad block filters into a .dat file format. This project used the following GitHub project for serialization: https://github.com/brave/ad-block

To compile the project, use Visual Studio Build tools with the following command: cl /Fo.\obj\ /Fefserialize /EHsc .\cpp\*.cc .\cpp\*.cpp

To run the program, place all your filter lists in the 'filters' folder and run the following command: ./fserialize ./filters

