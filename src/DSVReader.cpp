// DSVReader.cpp 
// Project 2
// Felipe Gutierrez
// ECS 034
//----------------------------------------------------------------------------

// C++ Headers
#include <fstream> 
#include <iostream>
#include <sstream>

// User Defined Headers
#include "StringUtils.h"
#include "DSVReader.h"

//----------------------------------------------------------------------------
// pimple
struct CDSVReader::SImplementation 
{
    std::shared_ptr<CDataSource> DataSource;
    char delimiter;
    // Constructor
    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DataSource(std::move(src)), delimiter(delimiter) {}
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(new SImplementation(std::move(src), delimiter)) {} 

//----------------------------------------------------------------------------
// Returns true if the row is successfully read, one string will be put in the row per column
// std::vector<std::string> &row: Vector of strings filled with the values of each column in the current row
CDSVReader::~CDSVReader() = default;
bool CDSVReader::ReadRow(std::vector< std::string > &row)
{
    char temp;
    std::string line;
    bool lineRead = false; 

    // Indicates if at least one character was read successfully
    while (DImplementation->DataSource->Get(temp)) 
    {
        lineRead = true; // Successfully read a character
        if (temp == '\n') { break; } 
        else { line += temp; }
    }

    // Case 1: Splits values by delimiter until newline is found
    if (lineRead) { row = StringUtils::Split(line, std::string(1, DImplementation->delimiter)); return true; } 

    // Case 2: No character found
    else { return false; }
}

//----------------------------------------------------------------------------
// Returns true if all rows have been read from the DSV
bool CDSVReader::End() const
{ return DImplementation->DataSource->End(); }