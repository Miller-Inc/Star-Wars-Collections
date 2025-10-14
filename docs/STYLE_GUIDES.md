# Miller Inc Style Guidelines 
This document outlines the coding style guidelines for Miller Inc. 
    Adhering to these guidelines will help maintain code consistency, 
    readability, and quality across our projects. Please follow these
    guidelines when writing code, working on pull requests, or reviewing code.

## General Principles
- **Readability**: Write code that is easy to read and understand. Use meaningful names
           for variables, functions, and classes.
- **Simplicity**: Keep your code as simple as possible. Avoid unnecessary complexity.
- **Documentation**: Document your code with comments where necessary, but avoid over-commenting.
- **Testing**: Write tests for your code to ensure its correctness and reliability. Build it and
                  test it locally before pushing. When pushing, write a clear and concise commit
                  message that describes the changes you made. Include the OS and version of the
                  software you are working on and any additional OSes that you have tested on.
- **File Structure**: Organize your files and directories in a logical manner. Follow the existing
                  structure of the project you are working on. Source files should be placed in the
                  `src` directory, header files in the `include` directory, and tests in the `tests`
                  directory. If you are adding new functionality, create a new directory under `src`
                  and `include` with a name that reflects the functionality.

## Language-Specific Guidelines
### C/C++
- Use `CapitalizedCamelCase` for class names, functions, types, namespaces, and file names.
- When naming files, use the same name as the class or main content of the file.
  - Make sure to include the appropriate file extension: `.h` or `.hpp` for header files and `.cpp` 
        for implementation files. Use `.h` for normal C++ headers and `.hpp` for platform-specific 
        headers (i.e. Vulkan interfacing headers). ***Hint: You will almost always use `.h`***

- Use `lowercase_with_underscores` for local variables
- Use `mClassMember` for non-boolean private/protected class member variables
- Use `ClassMember` for ALL public class member variables and static class member variables
    - Static class member variables should also be marked `inline` and have a default value
- Use `bClassBooleanMember` for class boolean member variables
- Use `CAPITALIZED_WITH_UNDERSCORES` for constants and macros
- Use 4 spaces for indentation (or a tab, but be consistent)
- Limit lines to 100 characters or fewer
- Use braces `{}` for all control structures, even if they are optional
- Place the opening brace `{` on the following line for functions and control structures
- Use `//` for single-line comments and `/* */` for multi-line comments
- Use `nullptr` instead of `NULL` or `0` for pointers
- Use `override` and `final` keywords for virtual functions where appropriate
- Document each header file with a brief description of its purpose and contents.
- Document each class with a brief description of its purpose and functionality.
- Document every function with a brief description of its purpose, parameters,
        return values, and any exceptions it may throw. This is not yet done with 
        the current codebase, but should be done moving forward and will be done shortly. 
- Comments are highly encouraged throughout the codebase, especially in complex or non-obvious
        sections. Comments should explain the "why" and "how" behind the code, not just the
        "what". Remember that this repository is meant to be a learning tool for new
        developers, so clear and informative comments are essential.
- `#define`s should be placed at the top of the file, after includes and before any code or 
        in a separate `<UseCase>Macros.h` file if they are shared across multiple files.
- Use `#pragma once` at the top of header files to prevent multiple inclusions.
- Include necessary headers at the top of each file. Avoid unnecessary includes to reduce
        compilation time and dependencies. We prefer using the types defined in the 
        `EngineTypes` directory, so include those headers when possible. (Note that the 
        `MArray.h` header is an exception to this rule, as it works with everything but 
        strings and should be used everywhere but when working with strings.)
- `typedef`s and `using` statements should be placed after includes and before any code after 
        any `#define`s.
  - They should have a name that reflects their purpose and usage. Avoid generic names like 
        `MyType` or `DataType`.
- When using namespaces, prefer `namespace MillerInc {}` over `using namespace MillerInc;` to
        avoid polluting the global namespace. 
    - Use `using MillerInc::SomeType;` for specific types if necessary.
    - **NEVER** use `using namespace std;` or any other standard library namespace.
    - *NOTE*: The `MillerInc` namespace is the default namespace for all code in this repository. 
            All code should be placed within this namespace unless there is a specific reason 
            not to do so (e.g., interfacing with third-party libraries) and should be documented if
            that is the case.
    - Place code in namespaces that reflect the directory structure. For example, code in 
            the `Engine/Rendering` directory should be placed in the `MillerInc::Engine::Rendering` 
            namespace. If it is a simple type or utility, it can be placed directly in the
            `MillerInc` namespace.


Example: 
~~~c++
    // Example.h(pp)
    /// Added by [Your Name] on [Date]
    /// This file is an example header file demonstrating the Miller Inc style guidelines.
    
    #pragma once
    #include <iostream> // [Optional] Describe why this include is necessary and what types are being used from it.
    #include "EngineTypes/MString.h" // Include the MString type from the EngineTypes directory.
    
    namespace MillerInc { // Place code in the MillerInc namespace.
    // If this were in a subdirectory, you would use nested namespaces like so:
    // namespace MillerInc::Engine::Rendering {
    
        typedef struct exampleStruct {
            int exampleMember; // Example member variable.
        } ExampleStruct; // Typedef for the struct.
        
        #define EXAMPLE_MACRO 100 // Example macro definition.
      
        /// This class demonstrates the Miller Inc style guidelines.
        class MyClass {
        public:
            /// Constructor for MyClass.
            MyClass(); 
            
            /// Destructor for MyClass.
            ~MyClass();
            
            /// Example function demonstrating the style guidelines.
            /// This function increments the input parameter by one.
            /// @param exampleParam An example parameter for the function.
            /// @return an integer result of the parameter incremented by one.
            int Increment(int exampleParam);
            
            
            
        private:
            /// Example private function.
            /// This is a private function that demonstrates the style guidelines.
            /// It does not take any parameters and does not return a value.
            void PrivateFunction();
        
            // Example non-boolean class member variable.
            int mExampleMember = 0; // Default value of 0.
            
            // Example boolean class member variable.
            bool bIsExample = false; // Default value of false.
            
            // Example constant definition.
            static const int EXAMPLE_CONSTANT = 10;
            
            static inline int StaticMember = 5; // Example static inline member variable with default value.
        };
    }
~~~

~~~c++
    // Example.cpp
    /// Added by [Your Name] on [Date]
    /// This file is an example implementation file demonstrating the Miller Inc style guidelines.
    
    #include "Example.h" // Include the corresponding header file.
    
    namespace MillerInc { // Place code in the MillerInc namespace.
    
        MyClass::MyClass() 
        {
            // Default no-argument constructor should use the default values that are already set in the header file.
            // Constructor implementation.
        }
        
        MyClass::~MyClass() {
            // Destructor implementation.
        }
        
        int MyClass::Increment(int exampleParam) {
            // Function implementation.
            return exampleParam + 1; // Increment the input parameter by one and return the result.
        }
        
        void MyClass::PrivateFunction() {
            // Private function implementation.
        }
    }
~~~

## Adding Dependencies
When adding dependencies to the project, please follow these guidelines:
- **Justification**: Ensure that the dependency is necessary and adds significant value to the project.
           Avoid adding dependencies for trivial tasks that can be accomplished with standard libraries.
- **Lightweight**: Prefer lightweight dependencies that do not bloat the project or introduce unnecessary
           complexity.
- **Well-Maintained**: Choose dependencies that are actively maintained and have a strong community
           support.
- **License Compatibility**: Ensure that the dependency's license is compatible with the project's license.
- **Documentation**: Document the purpose of the dependency and how to use it in the project's
           documentation.
- **Versioning**: Please try to use the most up-to-date stable version of the dependency to ensure
           compatibility and access to the latest features and bug fixes.
- **Security**: Evaluate the security implications of adding the dependency. Avoid dependencies with
           known vulnerabilities or security issues.
- **Testing**: Test the dependency thoroughly to ensure it works as expected and does not introduce
           any issues or conflicts with existing code.
- **Approval**: Get approval from a project maintainer before adding a new dependency to the project.
           This helps ensure that the dependency aligns with the project's goals and guidelines.
- **Integration**: Integrate the dependency into the project in a way that minimizes disruption to
           existing code and workflows. Follow best practices for integrating third-party libraries.
- **Updates**: Regularly check for updates to the dependency and evaluate whether to update to
           the latest version. Consider the impact of updates on the project and test thoroughly
           before updating.
- **Removal**: If a dependency is no longer needed or becomes problematic, consider removing it
           from the project. Evaluate the impact of removal on existing code and workflows, and
           test thoroughly after removal.
- **Valid Third-Party Dependencies**: Valid third-party dependencies include libraries and frameworks that
           provide essential functionality, improve performance, or enhance the user experience.
           Dependencies must be able to be built from source and should not rely on pre-compiled binaries.
           This is because we want this repository to be as much cross-platform as possible and pre-compiled
           binaries may not be available for all platforms. Their compilation should be handled in the 
           CMake build system to ensure compatibility across different environments. Examples of valid
           third-party dependencies include:
   - Windowing libraries (e.g., GLFW, SDL)
   - Graphics libraries (e.g., Vulkan SDK, OpenGL) (Note: The Vulkan SDK is not open-source, but it is)
       provided by the Khronos Group and is widely used in the industry. It is acceptable to use
       the Vulkan SDK as a dependency for this project.)
   - Math libraries (e.g., GLM, Eigen)
   - Image loading libraries (e.g., stb_image, FreeImage)
   - Audio libraries (e.g., FFMPEG)

