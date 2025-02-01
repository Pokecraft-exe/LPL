# LPL - The Ultimate Parsing Language

## Overview
LPL (Language Parsing Language) is an ultimate parser inspired by regular expressions (regex). It provides a powerful and flexible way to define structured parsing rules, enabling developers to extract and interpret data with high precision. LPL is designed to work with nested structures, scoped variables, and custom parsing rules.

## Features
- **Structured Parsing**: Define parsing rules using named schemes.
- **Scoping & Visibility**: Set variable visibility across different levels.
- **Reference Mechanism**: Use `ref()` to reference other schemes dynamically.
- **Mapping & Lists**: `map()` to separate schema and values, `list()` to numerate properties.
- **Parent Relations**: Use `parent()` to refer to the parent scheme.
- **Default Values**: Use `default()` to specify fallback values when a token is absent.
- **Data Type Support**: Handle integers, floats, strings, characters, and custom types.
- **Multiline Support**: Control whether a scheme spans multiple lines.
- **Predefined Functions**: Includes `numeric()`, `alpha()`, `alphanum()`, `character()`, etc.

## Syntax
LPL defines data structures and parsing rules using a declarative syntax. Below is an example of a typical LPL scheme:

```lpl
(variable
"visibility"
"type"
"name"
"isExtern"
ref("function", 'function')
map("value")

inside 'page' "visibility" = global
inside 'function' "visibility" = local
inside 'function' "function" = parent()
multiline = fals

==
default("type" = any) "name" = or('variable' 'function' 'int' 'float' 'string' 'char' 'operation')
)
```

### Explanation:
- `variable` scheme defines properties like visibility, type, name, and external reference.
- `inside` statements assign values based on context.
- `default()` sets fallback values when a token is absent, allowing implicit assumptions.
- `==` separates the schema definition from the parsing logic.
- `default()` sets fallback values.

### Example Data Types
LPL supports basic types such as integers, floats, strings, and characters:

```lpl
(int
==
numeric()
)

(float
==
numeric().numeric()
)

(string
==
\"...()\"
)

(char
==
\'character()\'
)
```

## Functions
LPL includes built-in functions to streamline parsing:
- `...()` - Matches until a condition.
- `map()` - Separates scheme and values.
- `list()` - Creates enumerated properties.
- `ref()` - References another scheme.
- `parent()` - Gets the parent scheme.
- `default()` - Specifies a default value when a token is absent.
- `numeric()` - Matches numbers.
- `alpha()` - Matches alphabets.
- `alphanum()` - Matches alphanumeric characters.
- `character()` - Ensures a single character match.

## Example Usage of `default()`
Using `default()`, we can assign fallback values when a token is missing:

```lpl
default("type" = any) "name"
```

This allows parsing inputs like:
- `int var` (explicit type)
- `var` (implicitly `any var` due to `default()`)

## Conclusion
LPL is a robust and extensible language for parsing structured data. Its combination of regex-like flexibility with structured definitions makes it an excellent tool for parsing complex text formats efficiently.

License

The project, past and future releases and code, is under the CreativeCommons License:
<img src="https://camo.githubusercontent.com/c6d555019c5655f6ad9c17674fc433d86b74df17cf6f8471d34a3c139e9b0580/68747470733a2f2f6d6972726f72732e6372656174697665636f6d6d6f6e732e6f72672f70726573736b69742f627574746f6e732f38387833312f706e672f62792d6e632d73612e706e67"/>
