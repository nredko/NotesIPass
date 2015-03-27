# NotesIPass
IBM Lotus Notes add-in to get passwords from [KeePass](http://keepass.info).

Requires [KeePassHttp](http://github.com/pfn/keepasshttp).
Implemented using [KeePassHttpClient](http://github.com/nredko/keepasshttpclient).

## Installation

1. Download [NotesIPass.dll](https://github.com/nredko/NotesIPass/blob/master/nNotesIPass.dll?raw=true)
2. Put NotesIPass.dll to IBM Lotus Notes installation folder, usually `C:\Program Files\IBM\Lotus\Notes\` or `C:\Program Files (x86)\IBM\Lotus\Notes\`
3. Add the following line to [Notes] section of your notes.ini 
>`EXTMGR_ADDINS=NotesIPass`
4. Install [KeePassHttp](http://github.com/pfn/keepasshttp) according instructions.