// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/Strings.cc"

#include "Ravl/CxxDoc/Strings.hh"

namespace RavlCxxDocN {

  // C++ key words.
  
  const StringC str_abstract("abstract");
  const StringC str_extern("extern");
  const StringC str_void("void");
  const StringC str_operator("operator");
  const StringC str_typename("typename");
  const StringC str_protected("protected");
  const StringC str_public("public");
  const StringC str_private("private");
  const StringC str_friend("friend");
  const StringC str_class("class");
  const StringC str_union("union");
  const StringC str_struct("struct");
  const StringC str_virtual("virtual");
  const StringC str_namespace("namespace");
  const StringC str_static("static");
  const StringC str_enum("enum");
  const StringC str_const("const");
  
  const StringC str_template("template");
  const StringC str_throw("throw");
  const StringC str_using("using");
  const StringC str_asm("asm");
  const StringC str_typedef("typedef");
  
  const StringC str_unsigned("unsigned");
  const StringC str_signed("signed");
  const StringC str_long("long");
  const StringC str_short("short");
  const StringC str_int("int");
  const StringC str_float("float");
  const StringC str_double("double");
  const StringC str_char("char");
  const StringC str_bool("bool");
  
  const StringC strp_Colon(":");
  const StringC strp_ColonColon("::");
  const StringC strp_OpenBracket("(");
  const StringC strp_CloseBracket(")");
  const StringC strp_Tilda("~");     
  const StringC strp_Comma(",");     
  const StringC strp_Semicolon(";");
  const StringC strp_asterisk("*");
  const StringC strp_ampersand("&");
  const StringC strp_equals("=");
  const StringC strp_plus("+");
  
  // Declaration types.
  
  const StringC str_storageType("storageType");
  const StringC str_extdefs("extdefs");
  const StringC str_dectype("dectype");
  const StringC str_definition("definition");
  const StringC str_array("array");
  const StringC str_forward("forward");
  const StringC str_prototype("prototype");

  // Internal markers
  
  const StringC str_x("x");
  const StringC str_1("1");
  const StringC str_varList("varList");
  const StringC str_PathList("PathList");
  const StringC str_EnumNameList("EnumNameList");
  const StringC str_VarNameList("VarNameList");
  const StringC str_TemplateInstArgs("TemplateInstArgs");
  const StringC str_TemplateDefArgs("TemplateDefArgs");
  const StringC str_classInherit("classInherit");
  const StringC str_ClassContents("ClassContents");
  const StringC str_EmptyArgList("EmptyArgList");
  const StringC str_FuncArgList("FuncArgList");
  const StringC str_ConstructorScopePath("ConstructorScopePath");
  
  const StringC str_detail("detail");
  const StringC str_brief("brief");
  
}
