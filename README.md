# Nabu

## Installation & configuration

Configure Nabu through the _config.hpp_ file.

In order to compile the program:

```sh
make
```

## Usage

Nabu commands:

-   `new` (`n`): Create a new note.
    Format:
    ```sh
    nabu n <new note> [-t <tag> ...] [-c <category>]
    ```
    Arguments:
    -   `-t`: Following arguments (separated by spaces) will be used as tags for the new note.
    -   `-c`: Following argument will set the category for the new note. The category designates the folder the note will be located in.
        Can be separated by spaces (e.g. `-c programming python`) or given as a single folder path (e.g. `-c programming/python`), which would both result in the note being placed under _/programming/python/_ in the Nabu root folder.
-   `read` (`r`): Read note using preferred program for the note type.
    Format:
    ```sh
    nabu r <note id>
    ```
-   `list` (`l`): Show list of notes.
    Format:
    ```sh
    nabu l [<specifier> [-and/-or <specifier> ...]]
    ```
    where `<specifier>` can be `-t <tag> ...` or `-c <category>`.
    Arguments:
    -   `-and`: Show notes matching both the specifier before and after `-and`.
    -   `-or`: Show notes matching either the specifier before or after `-or`.
-   `edit` (`e`): Edit a note.
    Format:
    ```sh
    nabu e <note id> [-t <tag> ...] [-c <category>]
    ```
    Arguments:
    -   `-t`: Change note tags to given tags.
    -   `-d`: Change note category to given category.
-   `delete` (`d`): Delete certain note.
    Format:
    ```sh
    nabu d <note id>
    ```
