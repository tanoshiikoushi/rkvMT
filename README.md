# rkvMT
This repository hosts an "RKV Mega Tool" for Krome Merkury Engine v1 Archives (.rkv)

Info
===
Merkury Engine v1 is used for

- All versions of Ty the Tasmanian Tiger 1
- Other things that I'm sure other people will add

This project is currently alpha so any support is largely limited, but feel free to reach out to me through Twitter or go join the Ty Speedrunning discord and you can find me there.

Details
===
As of the moment, this tool will extract all RKVs in a "semi-proprietary" format using the following prefixes:

- '\~' are used to denote symlinks
- '\!' are used to denote statically aligned files
- '\~\!' means both are true

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
extract <in|out> <*|id> "C:/dir/to/extract/to"
```
If given an id, it extracts the file with the specified id from the chosen slot to the given directory.

If '\*' is given instead of an id, it fully extracts the RKV, properly setting up symlinks, static files, and directory structures.
