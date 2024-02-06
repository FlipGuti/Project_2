# Project 2 README
Student 1: Felipe Gutierrez (919406096)
Student 2: Kavya Sasikumar

## Project Status
Submitted

## Known Issues
N/A

## Code References
* [Different ways to initialize variables](https://stackoverflow.com/questions/22543875/different-ways-to-initialize-variables)
* [XMLTips] (https://www.ibm.com/docs/en/streams/4.3.0?topic=splxml-xmlparse)
* [C++Functions] (https://www.geeksforgeeks.org/string-tokenization-in-c/?ref=ml_lbp)
* [ReadingDSVExamples] (https://stackoverflow.com/questions/19936483/c-reading-csv-file)
* [ReadingXMLExamples] (https://stackoverflow.com/questions/31687974/c-what-is-the-most-efficient-way-of-reading-xml-files)


## Generative AI Use
We used Chat GPT for Prompt 1, 2, 3

### Prompt 1
What is the proper Pimpl format for class types?

### Response 1
The Pimpl (Pointer to Implementation) idiom, also known as the Compiler Firewall or Cheshire Cat technique, is used in C++ programming to separate the implementation details of a class from its interface. This approach helps in reducing compile-time dependencies, thus speeding up the build process, and also in hiding implementation details from the user, making the library or application more flexible to changes without breaking binary compatibility.

#include "MyClass.h"

class MyClass::Impl {
public:
    void doSomething() {
        // Actual implementation
    }
};

MyClass::MyClass() : pimpl(new Impl) {}

MyClass::~MyClass() = default; // The default destructor is fine (C++11 and later)

MyClass::MyClass(const MyClass& other) : pimpl(new Impl(*other.pimpl)) {}

MyClass::MyClass(MyClass&& other) noexcept = default;

MyClass& MyClass::operator=(const MyClass& other) {
    if (this != &other) {
        *pimpl = *other.pimpl;
    }
    return *this;
}

MyClass& MyClass::operator=(MyClass&& other) noexcept = default;

void MyClass::doSomething() {
    pimpl->doSomething();
}

### Changes 1
We used this explanation to implement the pimple format in DSV:

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

CDSVReader::~CDSVReader() = default;


### Prompt 2
What do I do when a C++ error tells me that a variable cannot be a non static member?

### Response 2
When a C++ error message indicates that a variable cannot be a non-static member, it suggests that you're trying to use a variable in a way that requires it to have a single, shared value across all instances of a class, but it is currently defined as an instance member variable. This often occurs when trying to use a member variable in a static function or when trying to initialize a non-static member variable directly in its declaration inside the class, which is not allowed for non-const and non-inline static members.

Here's how you can resolve this issue, depending on the context:

1. If you're using the variable in a static function:
Make the variable static: Declare the variable as static within your class. This makes the variable shared across all instances of the class. Remember, static variables need to be defined outside the class definition in a .cpp file.
cpp
Copy code
class MyClass {
public:
    static int myStaticVar; // Declaration

    static void myStaticFunction() {
        myStaticVar = 5; // This is okay
    }
};

// Definition
int MyClass::myStaticVar = 0;
2. If you're trying to initialize a non-static member in its declaration:
Move initialization to the constructor: C++ allows non-static data members to be initialized where they are declared, but this does not work for all types of initialization, especially before C++11, and even in C++11 and later, there are restrictions. For non-static, non-constexpr members, you usually initialize them in the constructor.
cpp
Copy code
class MyClass {
public:
    int myVar;

    MyClass() : myVar(5) {} // Initialize myVar in the constructor
};
3. If you intended the variable to be shared across instances:
Declare and define the variable as static: This approach is suitable if you want a single shared value for all instances of the class. Remember to also provide a definition outside the class definition.
4. For constexpr static members:
If the variable is meant to be a compile-time constant and shared across instances, you can declare it as static constexpr and initialize it within the class definition.
cpp
Copy code
class MyClass {
public:
    static constexpr int myConstVar = 5; // Allowed and encouraged for compile-time constants
};
General Tip:
Review the usage context: Ensure that your use of static vs. non-static members aligns with your design goals. Static members are shared across all instances of a class, affecting all instances globally, while non-static members are unique to each instance.
If you encounter this error, review your code to determine whether the variable in question should indeed be shared across all instances (making it static) or whether you made a mistake in how you're attempting to use or initialize the variable.

### Changes 2
When referencing the Split() function in DSVReader.cpp under Readrow(), I orginally did not reference the StringUtils namespace, which caused a non member error


### Propmt 3
Why is my Makefile making an Obj directory and Bin directory but not running any of my test files? 

### Response 3
To address the issue you're facing with your Makefile not running any of your test files, despite successfully creating Obj and Bin directories, we'll need to consider several potential reasons and troubleshooting steps. Makefiles are used to automate the building and testing of projects by specifying rules and dependencies. The behavior you're experiencing can be caused by a variety of issues, such as missing or incorrect rules, missing dependencies, or incorrect path specifications.

Here are some common reasons and troubleshooting steps to consider:

Check for a Test Rule: Ensure that your Makefile contains a rule for running tests. This might look something like a target named test with commands to execute your test files.
Dependencies on Test Rule: If there is a test rule, make sure it is correctly specified and that its dependencies are correctly listed. If the dependencies are not up to date or are missing, the test rule might not execute.
Correct Paths: Verify that the paths to your test files are correctly specified in the Makefile. If the paths are incorrect, Make won't be able to find and execute your tests.
Explicitly Call Test Rule: If your test rule is not part of the default target (usually the first target in the Makefile), you need to explicitly call it by running make test (assuming test is the name of your test target).
Permissions: Ensure that your test files have the appropriate execute permissions. If they are supposed to be executable scripts, you might need to set their permissions with chmod +x.
Shebang Line for Scripts: If your test files are scripts, ensure they have the correct shebang line at the top (e.g., #!/bin/bash for Bash scripts) so that the system knows how to execute them.
Check for Conditional Execution: Look for any conditional statements in your Makefile that might prevent the test rule from executing. Conditional logic in Makefiles can be based on the existence of files, variables values, etc.
Error in Compilation or Previous Steps: If there are errors in earlier targets that your test target depends on, Make will not proceed to execute the test target. Make sure all dependencies build successfully.
Verbose Output: Run make with more verbose output to see more details about what it's doing. You can do this by adding @echo statements in your Makefile or running make with additional flags like make V=1 (depending on how your Makefile is set up for verbose output).
Check the Makefile's Default Goal: If your Makefile's default goal does not include running tests, they won't run unless you explicitly tell make to do so. The default goal is the first target in the Makefile unless explicitly set otherwise with the .DEFAULT_GOAL variable.
If you can share a snippet of your Makefile, especially the parts that define the Obj and Bin directories and the rule you expect to run your test files, I can provide more targeted advice.

### Changes 3
We incorrectly labled the "include" directory as "Include", it was very frustrating and yet the soltuion was so simple
