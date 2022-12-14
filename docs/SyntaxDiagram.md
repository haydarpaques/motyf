## Motyf Syntax Diagram

* Terminal tokens on the right-hand side of a rule are written as is: `token`.
* Non-terminal tokens on the right-hand side of a rule are bracketed: `[nonterminal]`.
* Empty (void) symbol: `#`.

Left-hand sides of rules are always non-terminal. 

```
function           : [function-prototype] [left-brace] [function-body] [right-brace]
function-prototype : func [identifier] [colon] [left-parenthesis] [function-param] [right-parenthesis] |
                     func [identifier] [colon] [left-parenthesis] [function-param] [right-parenthesis] [function-return]
function-body      :
identifier         :
function-param     : [statement-declaration] | [statement-declaration] [comma] [function-param] | #
function-return    : [minus] [right-chevron] [data-type-list]
data-type-list     : [data-type] | [data-type] [comma] [data-type-list]
```
