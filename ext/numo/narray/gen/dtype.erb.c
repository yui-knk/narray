/*
  <%=type_name%>.c
  Numerical Array Extension for Ruby
    (C) Copyright 1999-2011,2013 by Masahiro TANAKA

  This program is free software.
  You can distribute/modify this program
  under the same terms as Ruby itself.
  NO WARRANTY.
*/

#include <ruby.h>
#include <math.h>
#include "numo/narray.h"
#include "numo/template.h"
#include "SFMT.h"

#define m_map(x) m_num_to_data(rb_yield(m_data_to_num(x)))

<%
def_id "cast"
def_id "eq"
def_id "ne"
def_id "pow"
def_id "mulsum"
if is_complex
  def_id "real"
  def_id "imag"
else
  def_id "divmod"
end
if is_float
  def_id "nearly_eq"
end
if is_comparable
  def_id "gt"
  def_id "ge"
  def_id "lt"
  def_id "le"
end
if is_object
  def_id "bit_and"
  def_id "bit_or"
  def_id "bit_xor"
  def_id "bit_not"
  def_id "abs"
  def_id "minus"
  def_id "inverse"
  def_id "square"
  def_id "floor"
  def_id "round"
  def_id "ceil"
  def_id "nan?"
  def_id "infinite?"
  def_id "finite?"
  def_id ">" ,"op_gt"
  def_id ">=","op_ge"
  def_id "<" ,"op_lt"
  def_id "<=","op_le"
end
%>

<% IdVar.declaration.each do |x| %><%=
 x %>
<% end %>

#include "numo/types/<%=type_name%>.h"

VALUE cT;
extern VALUE cRT;
#ifdef mTM
VALUE mTM;
#endif

<%
if is_object
  def_allocate "robj_allocate"
end
def_method "allocate", 0

def_method "extract", 0
store_numeric
cast_array
store_array
if is_complex
  store_from "DComplex","dcomplex","m_from_dcomplex"
  store_from "SComplex","scomplex","m_from_scomplex"
end
store_from "DFloat","double",   "m_from_real"
store_from "SFloat","float",    "m_from_real"
store_from "Int64", "int64_t",  "m_from_real"
store_from "Int32", "int32_t",  "m_from_real"
store_from "Int16", "int16_t",  "m_from_real"
store_from "Int8",  "int8_t",   "m_from_real"
store_from "UInt64","u_int64_t","m_from_real"
store_from "UInt32","u_int32_t","m_from_real"
store_from "UInt16","u_int16_t","m_from_real"
store_from "UInt8", "u_int8_t", "m_from_real"

def_method "store", 1
def_singleton "cast", 1

def_method "aref", -1, "aref", :op=>"[]"
def_method "aset", -1, "aset", :op=>"[]="

def_method "coerce_cast", 1
def_method "to_a", 0
def_method "fill", 1
def_method "format", -1
def_method "format_to_a", -1
def_method "inspect", 0

def_method "each", 0
unary "map"
def_method "each_with_index", 0
def_method "map_with_index", 0

# Arithmetic

unary2 "abs", "rtype", "cRT"

binary "add", "+"
binary "sub", "-"
binary "mul", "*"
binary "div", "/"

if !is_complex
  binary "mod", "%"
  binary2 "divmod"
end

pow

unary "minus", "-@"
unary "inverse"

# Complex

if is_complex
  unary "conj"
  unary "im"
  unary2 "real", "rtype", "cRT"
  unary2 "imag", "rtype", "cRT"
  unary2 "arg",  "rtype", "cRT"
  def_alias "angle","arg"
  set2 "set_imag", "rtype", "cRT"
  set2 "set_real", "rtype", "cRT"
  def_alias "imag=","set_imag"
  def_alias "real=","set_real"
else
  def_alias "conj", "copy"
  def_alias "im", "copy"
end

def_alias "conjugate","conj"

# base_cond

cond_binary "eq"
cond_binary "ne"

# nearly_eq  : x=~y is true if |x-y| <= (|x|+|y|)*epsilon
if is_float
  cond_binary "nearly_eq"
else
  def_alias "nearly_eq", "eq"
end
def_alias "close_to", "nearly_eq"

# Integer
if is_int
  binary "bit_and", "&"
  binary "bit_or" , "|"
  binary "bit_xor", "^"
  unary  "bit_not", "~"
  def_alias "floor", "copy"
  def_alias "round", "copy"
  def_alias "ceil",  "copy"
end

if is_float && is_real
  unary "floor"
  unary "round"
  unary "ceil"
end

if is_comparable
  cond_binary "gt"
  cond_binary "ge"
  cond_binary "lt"
  cond_binary "le"
  def_alias ">", "gt"
  def_alias ">=","ge"
  def_alias "<", "lt"
  def_alias "<=","le"
end

# Float

if is_float
  cond_unary "isnan"
  cond_unary "isinf"
  cond_unary "isfinite"
end

accum "sum","dtype","cT"
accum "prod","dtype","cT"

if is_float
  accum "mean","dtype","cT"
  accum "stddev","rtype","cRT"
  accum "var","rtype","cRT"
  accum "rms","rtype","cRT"
end

if is_comparable
  accum "min","dtype","cT"
  accum "max","dtype","cT"
  accum_index "max_index"
  accum_index "min_index"
  def_method "minmax",-1
end

cum "cumsum","add"
cum "cumprod","mul"

# dot
accum_binary "mulsum"

# rmsdev
# prod

# shuffle
# histogram

def_method "seq",-1
def_method "eye",-1
def_alias  "indgen", "seq"

if !is_object
  def_method "rand", 0
end

# y = a[0] + a[1]*x + a[2]*x^2 + a[3]*x^3 + ... + a[n]*x^n
def_method "poly",-2

if is_comparable && !is_object
  qsort type_name,"dtype","*(dtype*)"
  def_method "sort",-1
  qsort type_name+"_index","dtype*","**(dtype**)"
  def_method "sort_index",-1
  def_method "median",-1
end

# Math
# histogram

if has_math
  math "sqrt"
  math "cbrt"
  math "log"
  math "log2"
  math "log10"
  math "exp"
  math "exp2"
  math "exp10"
  math "sin"
  math "cos"
  math "tan"
  math "asin"
  math "acos"
  math "atan"
  math "sinh"
  math "cosh"
  math "tanh"
  math "asinh"
  math "acosh"
  math "atanh"
  if !is_complex
    math "atan2",2
    math "hypot",2
    math "erf"
    math "erfc"
    math "ldexp",2
  end
end
%>
static VALUE <%= find_tmpl('store').c_func %>(VALUE,VALUE);
<%
Function.codes.each do |x| %>
<%= x %>
<% end %>

void
Init_nary_<%=tp%>()
{
    volatile VALUE hCast;

    cT = rb_define_class_under(mNumo, "<%=class_name%>", cNArray);
    <% for x in class_alias %>
    // alias of <%=class_name%>
    rb_define_const(mNumo, "<%=x%>", <%=type_var%>);
    <% end %>
    <% if has_math %>
    mTM = rb_define_module_under(cT, "Math"); <% end %>

    rb_define_const(cT, ELEMENT_BIT_SIZE,  INT2FIX(sizeof(dtype)*8));
    rb_define_const(cT, ELEMENT_BYTE_SIZE, INT2FIX(sizeof(dtype)));
    rb_define_const(cT, CONTIGUOUS_STRIDE, INT2FIX(sizeof(dtype)));

    rb_define_singleton_method(cT, "[]", <%=cast_func%>, -2);

    <% if is_object %>
    rb_undef_method(rb_singleton_class(cT),"from_string");
    rb_undef_method(cT,"to_string");
    <% end %>

    <% Function.definitions.each do |x| %>
    <%= x %><% end %>
    <% IdVar.assignment.each do |x| %>
    <%= x %><% end %>

    hCast = rb_hash_new();
    rb_define_const(cT, "UPCAST", hCast);
    rb_hash_aset(hCast, rb_cArray,   cT);
    <% for x in upcast %>
    <%= x %><% end %>
}
