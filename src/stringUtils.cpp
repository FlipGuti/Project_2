#include "StringUtils.h"

// -------------------------------------------------------------------------------------------------
// stringUtils.cpp
// Project 1 ( Source File )
// Felipe Gutierrez
// ECS 034
// -------------------------------------------------------------------------------------------------

namespace StringUtils
{

// ( 1 ) Slice
std::string Slice( const std::string &str, ssize_t start, ssize_t end )noexcept
{   
    int stringsize = str.length();
    if( end != 0 ){ stringsize -= abs(end); }
    std::string cut = str.substr( start, stringsize ); 
    return cut;
}

// ( 2 ) Capitalize
std::string Capitalize(const std::string &str)noexcept
{ 
    std::string ALLCAPS;
    for (size_t i = 0; i < str.length(); i++) 
    {
        char letter = toupper(str[i]);
        ALLCAPS.push_back(letter); 
    }
    return ALLCAPS; 
}

// ( 3a ) Upper
std::string Upper(const std::string &str)noexcept
{ 
    int size = str.length();
    int start = 0;
    int end = size;

    for (size_t i = 0; i < size; i++) // Finds start
    {
        char letter = str[i];
        if( isupper(letter) ) { start = i; break; }
    }

    for (size_t j = start+1; j < size; j++)
    {
        char letter = str[j];
        if( isupper(letter) ){ end = j; break; } // Finds end
    }

    std::string upperstring = str.substr( start, end );
    return upperstring;
}

// ( 3b ) Lower
std::string Lower(const std::string &str)noexcept
{ 
    int size = str.length();
    int start = 0;
    int end = size;

    for (size_t i = 0; i < size; i++) // Finds start
    {
        char letter = str[i];
        if( islower(letter) ) { start = i; break; }
    }

    for (size_t j = start; j < size; j++)
    {
        char letter = str[j];
        if( isupper(letter) ) { end = j; break; } // Finds end of lower case string
    }

    std::string lowerstring = str.substr( start, end );
    return lowerstring;
}

// ( 4a ) Left
std::string LStrip(const std::string &str)noexcept
{ 
    std::string leftstrippedstring;
    int size = str.length();
    int middle = size / 2;

    // Appends non-space characters from left
    for (size_t i = 0; i < middle; i++) 
    {
        char letter = str[i];
        if( !isspace(letter) ) { leftstrippedstring.push_back( letter ); }
    }

    // Appends all characters from right
    for (size_t j = middle; j < size; j++)
    {
        char letter = str[j];
        leftstrippedstring.push_back( letter );
    }

    return leftstrippedstring;
}

// ( 4b ) Right
std::string RStrip(const std::string &str)noexcept
{ 
    std::string rightstrippedstring;
    int size = str.length();
    int middle = size / 2;

    for (size_t i = 0; i < middle; i++) // Appends non-space characters from left
    {
        char letter = str[i];
        rightstrippedstring.push_back( letter );
    }

    for (size_t j = middle; j < size; j++) // Appends all characters from right
    {
        char letter = str[j];
        if( !isspace(letter) ) { rightstrippedstring.push_back( letter ); }
    }

    return rightstrippedstring;
}

// ( 4c ) Both
std::string Strip(const std::string &str)noexcept
{ 
    std::string strippedstring;
    int size = str.length();

    for (size_t i = 0; i < size; i++) // Appends all non-space characters
    {
        char letter = str[i];
        if( !isspace(letter) ) { strippedstring.push_back( letter ); }
    }

    return strippedstring;
}

// ( 5a ) Center Justified
std::string Center(const std::string &str, int width, char fill)noexcept
{ 
    int padding = std::max(0, width - static_cast<int>(str.length()));
    
    // Odd Case
    if ( padding % 2 == 1) 
    { 
        padding = ( padding/2 ); 
        return std::string(padding + 1, fill) + str + std::string(padding, fill); 
    }

    // Even Case
    else 
    { 
        padding = ( padding / 2 ); 
        return std::string(padding, fill) + str + std::string(padding, fill);
    }
}

// ( 5b ) Left Justified
std::string LJust(const std::string &str, int width, char fill)noexcept
{ 
    // Finds number of characters needed to reach desired width of string
    // Max protects against invalid inputs less than original str length
    int padding = std::max(0, width - static_cast<int>( str.length() ) );

    // Adds and returns string of length padding to str
    return std::string(padding, fill) + str;
}

// ( 5c ) Right Justified
std::string RJust(const std::string &str, int width, char fill)noexcept
{ 
    // Finds number of characters needed to reach desired width of string
    // Max protects against invalid inputs less than original str length
    int padding = std::max(0, width - static_cast<int>( str.length() ) );

    // Adds and returns string of length padding to str
    return str + std::string(padding, fill);
}

// ( 6 ) Replace
std::string Replace(const std::string &str, const std::string &old, const std::string &rep)noexcept
{ 
    int str_size = str.length();
    int old_size = old.length();
    int old_position = str.find( old );

    std::string new_string = str;
    new_string.replace( old_position, old_size, rep );
    return new_string;
}

// ( 7 ) Splits
std::vector< std::string > Split(const std::string &str, const std::string &splt )noexcept
{ 
    // Create variables that can be transformed
    std::vector<std::string> string_vector;
    std::string temp_str = str;
    std::string temp_splt;

    // If splt is empty, assume white space
    if( splt.empty() ){ temp_splt = " "; }
    else { temp_splt = splt; }

    // Declare first found splt position
    int pos = temp_str.find(temp_splt); 

    while( pos != std::string::npos )
    {
        // Add to vector
        string_vector.push_back(temp_str.substr(0,pos));
        // Shorten temp_string
        temp_str = temp_str.substr( pos+1);
        // Find next_str
        pos = temp_str.find(temp_splt); 
    }

    // Push the remainding string onto the vector
    string_vector.push_back( temp_str ); 
    return string_vector;
}

// ( 8 ) Join
std::string Join(const std::string &str, const std::vector< std::string >&vect)noexcept
{ 
    std::string joined_string;
    for (int i = 0; i < vect.size(); ++i) 
    { joined_string.append( vect[i] ); }
    return joined_string;
}

// ( 9 ) Expand Tabs
std::string ExpandTabs(const std::string &str, int tabsize )noexcept
{ 
    // Initialize variables
    std::string temp_str = str;
    std::string old_tab = "\t";
    int tab_pos = temp_str.find(old_tab);

    // Initialize replacement tab
    std::string new_tab;
    for (int i = 0; i < tabsize; i++)
    { new_tab.append(" "); }

    // Find and replace all tabs
    while (tab_pos != std::string::npos)
    {
        temp_str = temp_str.replace(tab_pos, old_tab.length(), new_tab);
        tab_pos = temp_str.find(old_tab);
    }

    return temp_str;
}

// ( 10 ) Levenshtein Distance
int EditDistance(const std::string &left, const std::string &right, bool ignorecase)noexcept
{ 
    std::string temp_left = left;
    std::string temp_right = right;

    int total_edits = 0;
    int left_size = temp_left.length();
    int right_size = temp_right.length();
    int smallest_size; 
    
    // Initilizes total_edits with length difference between left and right
    if( std::abs( left_size - right_size ) != 0 ) { total_edits = std::abs( left_size - right_size ); }

    // Checks which string to iterate ( avoids out of bounds error ) 
    if( left_size == right_size ) { smallest_size = left_size; }
    else if( left_size < right_size ) { smallest_size = left_size; } 
    else if( left_size > right_size ) { smallest_size = right_size; }

    // Loop to change left_string -> right_string ( i = length of smallest string )
    for( int i = 0; i < smallest_size; i++ ) 
    {
        if( temp_left[i] != temp_right[i] ) 
        { total_edits = total_edits + 1; }
    }

    return total_edits;
}

};