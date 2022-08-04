# This draft is obsolete, check draft2.hy
## 
This file is a draft for syntaxes I would like to implement in the languages
Things here aren't necessarily accurate and can be either outdated,
never included in the final language, or modified before their inclusion

And btw, this is how you do multi line comments

a common syntax used in this file is the following:
[A|B] (C|D) E <F>

that means specifying A/B is optional
C or D is required
E is required
F is required, but not the literal string F, but some identifier

##

# Hydrogen is a statically type, compiled, object oriented,python like syntax language

# Importing a header file is usually done this way

import package.header

# But the full syntax is

import package::header in vnamespace

# The header file is looked for in IMPORT_PATH/package/header.{hy, hyh,}
# All symbols declared in that header are put in a virtual namespace 'vnamespace'
# Invoking a symbol X from that header would be done like: vnamespace::X, but it's mangled 
# name wouldn't be dependent on that virtual namespace

# To only import a set of symbols in a header:

import X, Y, Z, from package::header in vnamespace

# Again the 'in vnamespace' part is optional,
# any other symbol declared in that header is simply ignored

# Assuming 'a' is an int type global variable defined in another source file
# It can be accessed from here by declaring it like so

decl var int a

# Or if it is a constant

decl const int a

# Multiple variables/constants can be declared in one line:
# decl (var|const) <type> <name_1>, <name_2>, ..., <name_n>

# Variables can be defined like the following:
# [global|local] (var|const) <type> <name> [ = <value>] # If a const, then [ = <value>] becomes required
# Examples:
global var int a
global var int a = 12
local var int a
local var int a = 45
global const int a = 12
local const int a = 45
# If compiler optimizations are on, and no pointer access to a local const is invoked,
# then the compiler will consider that constant a constexpr

# Multiple variables can be defined in 1 line
# [global|local] (var|const) <type> <name_1> [ = <value_1>], <name_2>..., <name_n> [ = <value_n>]
# And same here, [ = <value>] is required when const

# Pointers and references types:
# Pointers: ptr<type>, example ptr<int>
# References: ref<type>, example ref<int>
# Address of an identifier: ptrof(identifier), example ptrof(a) where a is a identifier
# The type of ptrof(a) is ptr<typeof(a)>
# data contained at a pointer: deref(ptr), example deref(p) where p is a pointer type identifier
# The type of deref(p) where p is of type ptr<t> is t, which is also unptr<typeof(p)>
# a ptr type (without <>) is a void* equivalent, to get the data at a ptr type, you would need to
# specify the type of that data, deref<type>(p), where typeof(p) = ptr
# The type, if convertible can also be specified on a typed ptr
# Examples
global var ptr<int> a
global var ptr<int> a = ptrof(b)
global var ref<int> a = b # References have to be initialized in this context
global var ptr<const int> a # A pointer to a constant data int unptr<typeof(a)> = const int

# At this point I'm stopping to consider
# In pretty much all other symbols, the keyword decl is used to declare
# And def is used to define, except for variables/constants, where they are declared with decl
# but defined without a keyword. The solution is to change variable definition to
# def [global|local] (var|const) <type> <name> [ = name], but it is very long for a statement
# that is frequently used, but removing it makes the language kind of incoherent
# Idk
# I also love how I always finish long paragraphs with Idk

# Forward struct declaration
# The struct should be later defined in this same translation unit

decl struct <name>

# Defining a struct
##
Before we talk about struct definitions, let me define how they are supposed to work in Hydrogen

Structs in Hydrogen only declare the variables each object instance should hold. PERIOD

The functions acting on that object are defined outside the Structs
Those functions are distinguished from normal functions, but they work the same
##

##
def struct <name> : [(public|protected|private) <parent_struct>, ...]
    (public|protected|private) (var|const) <type> <name> [ = <default_value>]
##

# example

def struct a: public b, private c, protected d
    public var int f = 10
    private var byte g
    protected const int h = 2 
# Note: consts don't have to be initialized here, they can be initialized in the constructor or an init array

# Declaring an instance of a class:
# [global|local] (const|var) <struct_name> <instance_name> [ = <struct_val> | <struct_val>]
# <struc_val> can be either an array initializer, or a constructor call,
# or an expression that can be converted to a <struct_name>
# Ugh, I don't feel that the wording up is right
# The <struct_val> rule follows a similar pattern to C++
# That is we can have things like:
# <instance_name>(constructor params...)
# <instance_name>(expression convertible to <struct_name>)
# <instance_name>{array initializer}

# Example:
global var a b
global var a c = { f = 10, g = 1, h = 10 }
# Assume a has a constructor(int, char)
# and a: implicit constructor(int)
global var a(10, 'c)
global var a = 12 # Uses the implicit constructor


# Function declaration
decl func func_name([var|const] type1 param1, ... [var|const] typen paramn) [returns type]
# If returns type is ommited, then the funtion returns void
# returns void can also be specified too

# Function definition
def func func_name([var|const] type1 param1, ... [var|const] typen paramn) [returns type]:
    <statements>+

# To call a function:
func_name(param1, ... paramn)

# Declare a member function
decl func func_name([var|const] type1 param1...) [returns type] for struct_name

# And definition:
def func func_name([var|const] type1 param1...) [returns type] for struct_name:
    <statements>+
    # the identifier 'this' can be used to get a pointer to the calling instance

# The compiler puts those functions in a namespace struct_name and thus
# calling one of those functions for 'instance'

struct_name::func_name(instance, params...)

# minimal main function declaration:
decl func main()
# full main function declaration
decl func main(uint argc, ptr<ptr<char>> argv) return int

## Summary

Full import statement: `import <imported> [from <header>] [in <vnamespace>]`
    If the `from <header>` part is ommited then <imported> is a header
    otherwise imported is a comma separated list of identifiers
    
Full variable/constant declaration: `decl [global|local] [var|const] <type> <name>`
    [global|local] is defaulted to global
    [var|const] is defaulted to const

Full variable/constant definition: `def [global|local] [var|const] <type> <name> [ = <value>]`
    [global|local] is defaulted to local
    [var|const] is defaulted to const
    [value] is defaulted to <type>::__default_value__()

Full function declaration: `decl [global|local] func <name>([[var|const] <param_type> [<param_name>]*]) [returns <type>]`
    [global|local] defaults to global
    [var|const] defaults to var
    [returns <type>] defaults to `return void`

Full function definition: `def [global|local] func <name>([[var|const] <param_type> [<param_name>]*]) [returns <type>]: <statements>`
    [global|local] defaults to local
    [var|const] defaults to var
    [returns <type>] defaults to `return void`

Full struct declaration: `decl struct <name>`
Full struct definition: `def struct <name> [extends [public|protected|private] <parent_type>,*]: <data definition>`
    [public|protected|private] defaults to public

Full member variable/constant definition: `[public|protected|private] [var|const] <type> <name> [ = <value>]`
    [public|protected|private] defaults to private
    [var|const] defaults to var
    <value> defaults to <type>::__default_value__()

Alias types: `alias <alias_name> = <original_type>`



Built-in Hydrogen types:
    Format: <type_name>: <size_in_bytes>
    boolean: 1
    byte: 1
    ubyte: 1
    char: 1
    word: 2
    uword: 2
    dword: 4
    int: 4
    uint: 4
    lword: 8
    ulword: 8
    float: 4
    double: 8
    adr: depend on architecture[x86:4, x86-64:6?8]
    template <type t> ptr<t>: sizeof(adr)
    template <type t> ref<t>: sizeof(adr)

Built-in Hydrogen constexpr functions:
    template <type t> decl constexpr func ptrof(const ref<t>) returns ptr<t>
    template <type t> decl constexpr func deref(const ptr<t>) return ref<t>

##

## Literals
    Lexer num literals: 
        Start with 0-9 then optionally repeat 0-9 or _ then optionnaly end with H, D, O or B
        R [0-9] [0-9_]* [HDOB]?
    Lexer string literals: 
        Start with " then optionally repeat either any character other than " or \ followed by any character, then end with "
        R '"' ( \\. | [^"] )* '"'
    Lexer char literals:
        Start with ' then followed by any character except ' or by \ followed by any character, then end with '
        '\'' ( \\. | [^\'] ) '\''

##
