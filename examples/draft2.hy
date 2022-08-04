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

##

Hydrogen is a statically typed programming language made for os developement, but can probably be used for other applications
it is designed with some priorities, in this order, safety, predictability, performance, convenience and simplicity in mind

# Functions
# a function declaration can be done like so:
decl func <func_name> (
    <arg0_name> : <arg0_type> [in <arg0_safe_set>],
    <argN_name> : <argN_type> [in <argN_safe_set>] [, ...]
) [returns ret0_type, retN_type]

# A function definition can be done like this:
(global|local) func <func_name> (
(
    <arg0_name> : <arg0_type> [in <arg0_safe_set>],
    <argN_name> : <argN_type> [in <argN_safe_set>] [, ...]
) [returns ret0_type, retN_type] :


##
