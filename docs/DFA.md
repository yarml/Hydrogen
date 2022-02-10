# What is a DFA?
A DFA(stands for deterministic finite automata) is a simple model of machines that is often used to represent programs that can check if a string is valid or not following the rules of a language

A DFA has a set of states, and transitions with conditions to move from a state to another.

States can be final or non final, if the input makes the DFA stop on a non final state, then the input is invalid to that language, otherwise, the final state is returned by the DFA machine

# DFA graph ascii syntax
Somewhere  in the Hydrogen lexer/parser source code, you mat come accross a block of code preceded by a comment showing the DFA graph describing what that block of code is supposed to match

## Basic DFA syntax

```
STATE --TRANSITION_CONDITION--> (FINAL_STATE)
```

Non final states are just written with their name, or . if they don't have one, and usually it's the latter case that is most found
TRANSITION_CONDITION is supposed to be a character for lexer or token for parsers
Final states are written like their name with parentheses, their name should be the same as the value the whole DFA is returning, that is, final states should be named as tokens for lexer DFAs and as parser rules for parser DFAs

### Example
The following is the DFA that matches PARENR(')') and only PARENR lexer token

```
. --')'--> (PARENR)
```

## Branching DFAs
STATE --|--CONDITION1--> (FSTATE1)
        |--CONDITION2--> (FSTATE2)
        |--CONDITION3--> (FSTATE3)
        |--...
### Example
The following DFA matches both PARENL('(') and PARENR(')') lexer tokens

```
. --|--'('--> (PARENL)
    |--')'--> (PARENR)
```

## More complex DFAs
DFAs can be chained, every state can be a DFA of its own, for example the following DFA matches
GREAT('<'), GREAT_EQ('<='), SHIFTL('<<'), and ASSIGN_SHIFTL('<<=') lexer tokens


```
. --'<'--> (GREAT) --|--'='--> (GREAT_EQ)
                     |--'<'--> (SHIFTL) --'='--> (ASSIGN_SHIFTL)
```

## State references

Because it is hard to draw lines with ascii charcters, states which point to themselves, or to states before them can use the following syntax:
```
STATE1 --CONDITION1--> STATE2 --CONDITION2--> *STATE1
```

## More complex conditions
Regular expressions can be used for more complex conditions

For example if a condition matches 'a', 'b', or 'c', it can be written this way

```
. --[abc]--> .
```

The condition * matches every symbol

...

## Default path
If a state doesn't handle a condition, it is assumed to point to a dead state, defined by:

```
DEAD_STATE --*--> *DEAD_STATE
```
