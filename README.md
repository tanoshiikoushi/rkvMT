# rkvMT
This repository hosts an "RKV Mega Tool" for Krome Merkury Engine v1 Archives (.rkv)

Info
===
Merkury Engine v1 is used for

- All versions of Ty the Tasmanian Tiger 1
- Other things that I'm sure other people will add

This project is in a fully working state - only extra features are added from here on out.

Details
===
As of the moment, this tool will extract all RKVs in a "semi-proprietary" format using the following prefixes:

- '\~' are used to denote symlinks
- '\!' are used to denote statically aligned files
- '\~\!' means both are true

This tool can extract all versions of Ty the Tasmanian Tiger 1 and should work for all other Krome Merkury Engine v1 Archives.

The versions that RKVs can currently be built for are:
- PC (tested)
- Gamecube (tested)
- Switch (in theory)
- PS4 (in theory)
- XBOX One (in theory)

Effectively, all of the remasters should work.

Versions that probably can be built (need extra testing):
- Original XBOX
- PS2 v1 and v2

Commands and Usage
===

`quit` or `exit`
---
Exits the program, unloading both slots.

`load`
---
```
load "C:/path/to/file.rkv" <in|out>
```
Loads the specified rkv into the 'in' or 'out' slot respectively.

`unload`
---
```
unload <in|out>
```
Unloads the specified rkv.

`poke`
---
```
poke <in|out> <file|dir> <id>
```
Displays information for the id-th file or directory of the chosen slot.

`extract`
---
```
extract <in|out> <*|id> "C:/dir/to/extract/to" (true|false)
```
If given an id, it extracts the file with the specified id from the chosen slot to the given directory.

If '\*' is given instead of an id, it fully extracts the RKV, properly setting up symlinks, static files, and directory structures.

If true or nothing is provided, the extract will preserve the original casing. If false is provided, it will automatically convert all paths to lowercase.

`load_dir`
---
```
load_dir "C:/path/to/main/dir" "..\relative_specific_path" <in|out> (true|false)
```
Loads the files provided in the first directory and the second directory, where the second directory works as the version specific path. (This will make more sense when you extract it and see that the version specific path is located one directory above the regular extract)

For all intents and purposes, if you're a regular user building for the PC version, just leave the directory hierarchy of the extract untouched and use the string: `"..\PC_Specific\"` as the relative specific path
---

The files and directories are loaded into the specified slot.

If true or nothing is provided, the directory structure will be recursed. If false is provided, it will only read the first directory and the second directory.

`generate`
---
```
generate <in|out>
```
Generates an RKV file for the specified slot.

`save`
---
```
save <in|out> "C:/path/to/save/file/to.rkv"
```
Saves a generated RKV file from the specified slot to the path given.
