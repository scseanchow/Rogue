# Rogue
CIS * 2500 - Rogue Game

COMPILATION:
1) gcc src/main.c src/a2functions.c -Iinclude -lncurses 
2) make a.out

USAGE:
1)./a.out assets/"textfilename"
2)make run 

TESTING:
To test the program, I've included all sample data.txt and template.txt in the folder titled assets
- During my testing I tried to test edge cases susch as:
- small rooms, big rooms
- 4 doors
- using all symbols in a room

KNOWN LIMITATIONS:
- the ncurses screen cannot exceed terminal size
- room size cannot be too large
- will not accept incorrect data files, no exception handling, must follow guideline specifications 

REFERENCES:
- Sean Chow’s A2 (my previous assignment)
