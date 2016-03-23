# OS

##Installera

###Assemblerkompilator

Installera nasm:

sudo apt-get install nasm

###Virtuell maskin

Installera bochs:

sudo apt-get install bochs

sudo apt-get install bochs-x

Alt. kan virtualbox köras.

##Kör

###Bochs

Gå till src och kör makefile
Gå tillbaka och kör mk_iso.sh och run_bochs.sh(i ordning)

bash mk_iso.sh

bash run_bochs.sh

skriv in 'c' och tryck enter i bochs terminalfönster

###Virtualbox

Gå till src och kör makefile
Gå tillbaka och kör mk_iso.sh

Sätt upp en maskin i virtualbox med os.iso som avbildningsfil.

Kör.
