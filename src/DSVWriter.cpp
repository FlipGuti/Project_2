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
#include <algorithm> // Include this for std::all_of
// User Defined Headers
#include "DSVWriter.h"
#include "StringUtils.h"
#include "StringDataSink.h"
//----------------------------------------------------------------------------
// Constructor for DSV writer, sink specifies the data destination, delimiter specifies the delimiting character, and quoteall specifies if all values
// should be quoted or only those that contain the delimiter, a double quote, or a newline

struct CDSVWriter::SImplementation 
{
    std::shared_ptr<CDataSink> DataSink;
    char delimiter;
    bool quoteall;
    // Constructor
    SImplementation(std::shared_ptr<CDataSink> DataSink, char delimiter, bool quoteall = false )
        : DataSink(std::move(DataSink)), delimiter(delimiter), quoteall(quoteall) {}
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(new SImplementation(std::move(sink), delimiter, quoteall)) {}

// Destructor
CDSVWriter::~CDSVWriter() = default;

//----------------------------------------------------------------------------
// Returns true if the row is successfully written, one string per column should be put in the row vector
bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {
    std::ostringstream finalRowStream;
    char quoteChar = '"';

    auto isNumeric = [](const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return std::isdigit(c) || c == '.'; });
    };

    for (size_t i = 0; i < row.size(); ++i) {
        std::string current = row[i];
        // Determine if quoting is needed based on the content of the current field
        bool needQuotes = DImplementation->quoteall || current.find(DImplementation->delimiter) != std::string::npos ||
                          current.find('"') != std::string::npos || current.find('\n') != std::string::npos;

        // If quoting is forced, check if it's numeric and adjust `needQuotes` accordingly
        if (DImplementation->quoteall && isNumeric(current)) {
            needQuotes = false;
        }

        // Escape double quotes
        current = StringUtils::Replace(current, "\"", "\"\"");

        // Add quotes if needed
        if (needQuotes) {
            finalRowStream << quoteChar << current << quoteChar;
        } else {
            finalRowStream << current;
        }

        // Add delimiter except for the last field
        if (i < row.size() - 1) {
            finalRowStream << DImplementation->delimiter;
        }
    }

    // End the row with a newline character
    finalRowStream << '\n';
    std::string finalRow = finalRowStream.str();

    // Convert finalRow to vector<char> and write to the data sink
    std::vector<char> vec(finalRow.begin(), finalRow.end());
    return DImplementation->DataSink->Write(vec);
}