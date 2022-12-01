# WARNING, this file was written in a hurry
# NONE of the information here is reliable
# It is nothing more than a draft

# Single line comment

##
Multiple line comments
-------
So ho're you doing
##

##
Hydrogen can import source files directly, or for libraries,
header files can be automatically created
##

import std::math

##
All symbols in an import can be placed in a virtual namespace
Virtual namespaces have no effect on symbol name mangling

Setting a virtual namespace only adds a prefix to the function qualified name
##


import std::io in io


##
now calling a math function(eg log) can be done like this:
std::log(x) -> y

and calling an io function(eg print) can be done like this:
io::std::print("Hello, World! %d", 10)
##

##
We can also *only* import certain symbols from an import

For example to only import the log and sin functions from std::math
##


import std::log, std::sin from std::math

## OR ##

import std::log, std::sin from std::math in math

##
Namespaces

Namespaces can be defined like
##

ns test::ns1:
    # Namespace content

##
A symbol can be put in a namespace also with the 'in' keyword
##

##
Using namesapces

Specifying namespaces can be avoided

Assumed test_num is a symbol inside test::ns
It's qualified name would be test::ns1::test_num

however using
##

using test::ns1

##
Adds an alias for test::ns1::test_num that is just test_num
UNLESS test_num is already in the used namespaces
##

##
General syntax

```
using <qualified_name>
```
<qualified_name> can be a namesapce name or a symbol name

If it is a namespace name, all symbols and namespaces are aliased
to not reuired <qualified_name> as a prefix, if it is a symbol
name, only that symbol is aliased to not require its namesapce
as a prefix
##

##
Aliases can be added for any symbol/typename/namespace
##

alias int as status

# status -> int

alias test::ns1 as ns1

# ns1 -> test::ns1

alias test::test_num as test

# test -> test::test_num

alias test::test_num as t1 in test::ns1

# test::ns1::t1 -> test::test_num

alias test::test_num as test::ns1::t2

# test::ns1::t2 -> test::test_num

##
General syntax

```
alias <qualifed_name> as <qualified_name> (in <qualified_name>)?
```
##

##
Other than imports, Hydrogen also supports symbol prototype
Assuming 'test_number' is a variable  of type int
in (this or) another translation unit in the namespace 'test::ns1',
then it can be declared here like
##

ns test::ns1:
    decl var test_number : int

## OR ##

decl var test::ns::test_number : int

## OR ##


decl var test_number in test::ns1 : int

##
Assuming 'test_const' is like test_number except it is a constant
##

ns test::ns1:
    decl const test_const : int

## OR ##

decl const test::ns::test_const

## OR ##

decl const test_const in test::ns : int

##
General declaration syntax

```
decl (const|var)? <qualified_name> (in <ns>)? (: <type_name>)?
```
##

##
Variable/constant definition

Assuming we want to define global variables
test_number and constant test_const both of
type int
##

def global test_number : int

## OR ##

def global test_number : int = 10

## AND ##

def global test_const : int const = 10

##
By default, in global context, all symbols are package local const : int

ie
##

def test_const : int const = 10

##
Is the same as
##

def test_const = 10

##
However leaveaing out the type is highly
discouraged as it can lead to ambiguities
##

##
In general, a variable/const is defined like

```
def (global|local)? <qualified_name> (: <type_spec>)? (= value)?
```
##


##
By default, in function context, variables/constant defintions are by default:
function local, var, type int

However not relying on the default type is always the convention

Typical function variable defintion is like
##

def func f():
    def a : int = 1
    def b : char* = "Hello, World!"
    def c : int const = 2

# Structs

struct custom_type:
    a : int
    b : byte
    c : char const*

def func f2(s : custom_type*) -> opt b : byte, c : char const*:
    b = s->b
    c = s->c


def func f3():
    def b : byte
    def c : char const*

    def s : custom_type

    s.a = 1
    s.b = 4
    s.c = "Hello"

    f2(&s) -> _, c
    f2(&s) -> b, c

# Loops
# There is only one type of loops in Hydrogen

def func f4():
    def a : int = 0
    loop:
        if a >= 10:
            break
        ++a
