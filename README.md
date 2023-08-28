# Hydrogen
Hydrogen interpreter.

This is mostly experiments, the language isn't serious, doesn't bring anything
new to the table, not meant for use in anything serious, lacks every feature,
always work in progress. The lexer, parser and interpreter are all hand written.


Example (as of August 28, 2023, this example shows all the features implemented)
```hy
def func getstring():
  "Hello"

def func main():
  print(getstring(), ", World")

```
(petition for github to add syntax highlighting for hydrogen)