% NABU(1) nabu
% Logi Eyjólfsson
% April 2021

# NAME
nabu - a command line note manager

# SYNOPSIS
**nabu n** *FILENAME* [ **-t** *TAG* ... ]  [ **-c** *CATEGORY* ... ]

**nabu r** *ID*

**nabu l** [ *SPECIFIER* [ **-and/-or** *SPECIFIER* ... ] ]

**nabu e** *ID* [ **-t** *TAG* ... ] [ **-c** *CATEGORY* ... ]

**nabu d** *ID*

**nabu lc**

**nabu lt**

# DESCRIPTION
Starts in an interactive TUI mode when invoked with no arguments. Category corresponds to the notes location in the directory structure. Can be given as a folder path (e.g. my/folder) or separated by spaces (e.g. my folder).

**new**, **n**
: Create new note named *FILENAME*, optionally adding any tags or a category

**read**, **r**
: Open note with given id in preferred program.

**list**, **l**
: List notes, optionally specifying any specifiers where *SPECIFIER* can be either **-t** *TAG* ... or **-c** *CATEGORY* ...

**edit**, **e**
: Edit the note with given id, optionally changing its tags or category

**delete**, **d**
: Delete the note with given id

**list-categories**, **lc**
: List categories in a tree structure

**list-tags**, **lt**
: List all tags

# EXAMPLES
Create a new note called python.md with tags "python" and "programming" in the folder NABU_ROOT_PATH/programming/languages.

**nabu n python.md -t python programming -c programming languages**

List all notes which have tags "haskell" or are placed in the category NABU_ROOT_PATH/programming/functional:

**nabu l -t haskell -or -c programming functional**

Edit note with id 3 and move it to the category NABU_ROOT_PATH/mathematics/algebra.

**nabu e 3 -c mathematics/algebra**

# CUSTOMIZATION
nabu is customized through modifying variables in config.hpp and recompiling the source code.

# BUGS
This program is still a work in progress.

Github: https://github.com/sultur/nabu

