// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// Python is used under the terms of the Python License 
// Copyright (C) 2001, 2002, 2003, 2004 Python Software Foundation; All Rights Reserved
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/Python/exPython.cc"
//! lib=RavlPython
//! author = "Warren Moore"
//! docentry = "Ravl.API.Python"

#include "Ravl/Python.hh"
#include "Ravl/PythonObject.hh"
#include "Ravl/PythonException.hh"
#include "Ravl/String.hh"

using namespace RavlN;

void displayError(const char *errorMessage)
{
  std::cerr << "#### Error" << std::endl;
  std::cerr << "## " << errorMessage << std::endl;
}

void displayException(PythonExceptionC &e)
{
  std::cerr << "#### Error" << std::endl;
  std::cerr << "##  Type:  " << e.Type() << std::endl;
  std::cerr << "##  Value: " << e.Value() << std::endl;
  std::cerr << "##  Trace: " << endl << e.Trace() << std::endl;
}

int main(int argc, char **argv)
{
  try 
  {
    // Initialise the python module
    PythonC python(true);

    std::cerr << "#### Initialising interpreter" << std::endl;
    if (!python.Initialised())
    {
      displayError("Failed to initialise interpreter");
      return -1;
    }

    std::cerr << "#### Appending '.' to system path" << std::endl;
    if (!python.AppendSystemPath("."))
    {
      displayError("Failed to append system path");
      return -1;
    }
  
  	cerr << "#### Importing 'ravlexample'" <<endl;
  	if (!python.Import("ravlexample"))
    {
      displayError("Failed to import 'ravlexample'");
      return -1;
    }
  
    // Build the arguments list
    PythonObjectC name = python.NewObject();
    name.BuildString("your-name-here");

    DListC<PythonObjectC> argList;
    argList.InsLast(name);
    PythonObjectC args = python.NewObject();
    args.BuildTuple(argList);

    // Call the example function and display the results
    std::cerr << "#### Calling 'ravlexample.myprint(" << name.String() << ")'" << std::endl;
    PythonObjectC res = python.Call("ravlexample", "myprint", args);
    if (res.IsValid())
    {
      if (res.IsString())
      {
	      std::cerr << "## Result(string): " << res.String() << std::endl;
      }
      else
      {
        displayError("'ravlexample.myprint' did not return a string");
        return -1;
      }
	  }
    else
    {
      displayError("Failed to call 'ravlexample.myprint'");
      return -1;
    }
     
	  // Run a script from a string
	  std::cerr << "#### Calling script from string" << std::endl;
    if (python.Run("x = 5"))
    {
      // Read the globals from the script
      PythonObjectC x = python.GetValue("x");
      std::cerr << "## Looking for global 'x': " << (x.IsValid() ? "Found" : "Not found") << std::endl;
      if (x.IsValid())
      {  
        if (x.IsInt())
        {
          std::cerr << "## 'x'= " << x.Int() << std::endl;
        }
        else
        {
          std::cerr << "## 'x' is not an int" << std::endl;
        }
      }
    }
    else
    {
      displayError("Failed to run script from string");
      return -1;
    }
  }
  catch (PythonExceptionC &e)
  {
    displayException(e);
  	return -1;
  }

  return 0;
}
