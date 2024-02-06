// DSVWriter.cpp
// Project 2
// ECS 034
//----------------------------------------------------------------------------
// C++ Headers
#include <expat.h>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstddef>
#include <vector>
// User Defined Headers
#include "DSVWriter.h"
#include "StringUtils.h"
#include "StringDataSink.h"
//----------------------------------------------------------------------------
// Constructor for DSV writer, sink specifies the data destination, delimiter specifies the delimiting character, and quoteall specifies if all values
// should be quoted or only those that contain the delimiter, a double quote,or a newline

struct CDSVWriter::SImplementation 
{
    std::shared_ptr<CDataSink> DataSink;
    char delimiter;
    bool quoteall;
    // Constructor
    SImplementation(std::shared_ptr<CDataSink> DataSink, char delimiter, bool quoteall = false )
        : DataSink(std::move(DataSink)), delimiter(delimiter), quoteall(quoteall) {}
};

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall = false)
    : DImplementation(new SImplementation(std::move(sink), delimiter, quoteall)) {} // Reassigns ptr ownership from sink -> DImple DataSink


// Destructor
CDSVWriter::~CDSVWriter() = default;


//----------------------------------------------------------------------------
// Returns true if the row is successfully written, one string per column should be put in the row vector
bool CDSVWriter::WriteRow(const std::vector< std::string > &row)
{
    std::vector<std::string> ProRow; // Proccessed Row
    std::string finalRow;
    char quoteChar = '"';
    bool needQuotes = false;

    for (const auto& field : row) 
    {
        std::string current = field;
        current = StringUtils::Replace(current, "\"", "\"\"");

        // Determined by CDSVWriter class
        if (DImplementation->quoteall) 
        { needQuotes = true; }

        else
        {   
            if (current.find(DImplementation->delimiter) != std::string::npos) { needQuotes = true; } // Check for delimiter in the string
            else if (current.find('"') != std::string::npos) { needQuotes = true; } // Check for double quote in the string
            else if (current.find('\n') != std::string::npos) { needQuotes = true; } // Check for newline in the string
        }

        if (needQuotes)
        { current = quoteChar + current + quoteChar; }

        ProRow.push_back(current);
    }

    // Join the processed fields using the delimiter
    for (size_t i = 0; i < ProRow.size(); ++i) 
    {
        finalRow += ProRow[i];
        if (i < ProRow.size() - 1) { finalRow += DImplementation->delimiter; }
    }

    // End the row with a newline character
    finalRow += '\n';

    // Convert finalRow to vector<char> and write to the data sink
    std::vector<char> vec(finalRow.begin(), finalRow.end());
    return DImplementation->DataSink->Write(vec);
}