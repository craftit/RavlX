// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_STRINGS_HEADER
#define RAVLCXXDOC_STRINGS_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/Strings.hh"

#include "Ravl/String.hh"

#define STR(x) RavlCxxDocN::str_ ## x

namespace RavlCxxDocN {
  using namespace RavlN;

  // C++ key words.
  
  extern const StringC str_abstract;
  extern const StringC str_void;
  extern const StringC str_operator;
  extern const StringC str_typename;
  extern const StringC str_protected;
  extern const StringC str_public;
  extern const StringC str_private;
  extern const StringC str_friend;
  extern const StringC str_class;
  extern const StringC str_struct;
  extern const StringC str_union;
  extern const StringC str_virtual;
  extern const StringC str_namespace;
  extern const StringC str_static;
  extern const StringC str_enum;
  extern const StringC str_extern;
  extern const StringC str_const;

  extern const StringC str_template;
  extern const StringC str_throw;
  extern const StringC str_using;
  extern const StringC str_asm;
  extern const StringC str_typedef;

  extern const StringC str_unsigned;
  extern const StringC str_signed;
  extern const StringC str_long;
  extern const StringC str_short;
  extern const StringC str_int;
  extern const StringC str_float;
  extern const StringC str_double;
  extern const StringC str_char;
  extern const StringC str_bool;

  // Punctuation.
  
  extern const StringC strp_Colon;        // :
  extern const StringC strp_ColonColon;   // ::
  extern const StringC strp_OpenBracket;  // (
  extern const StringC strp_CloseBracket; // )
  extern const StringC strp_Tilda;        // ~
  extern const StringC strp_Comma;        // ,
  extern const StringC strp_Semicolon;    // ;
  extern const StringC strp_asterisk;     // *
  extern const StringC strp_ampersand;    // &
  extern const StringC strp_equals;       // =
  extern const StringC strp_plus;         // +
  
  // Declaration types.
  
  extern const StringC str_storageType;
  extern const StringC str_extdefs;
  extern const StringC str_dectype;
  extern const StringC str_definition;
  extern const StringC str_array;
  extern const StringC str_forward;
  extern const StringC str_prototype;
  
  // Internal markers
  
  extern const StringC str_x;
  extern const StringC str_1;
  extern const StringC str_varList;
  extern const StringC str_EnumNameList;
  extern const StringC str_VarNameList;
  extern const StringC str_TemplateInstArgs;
  extern const StringC str_TemplateDefArgs;
  extern const StringC str_classInherit;
  extern const StringC str_ClassContents;
  extern const StringC str_EmptyArgList;
  extern const StringC str_FuncArgList;
  extern const StringC str_ConstructorScopePath;
  extern const StringC str_PathList;

  extern const StringC str_detail;
  extern const StringC str_brief;
  
}


#endif
