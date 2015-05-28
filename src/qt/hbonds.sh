cat > /tmp/donors.csv <<EODAT
ARG|NE
ASN|ND2
HIS|NE2
SER|OG
TYR|OH
ARG|NH1
CYS|SG
HIS|ND1
THR|OG1
ARG|NH2
GLN|NE2
LYS|NZ
TRP|NE1
*|N
EODAT
cat > /tmp/acceptors.csv <<EODAT
ASN|OD1
GLN|OE1
MET|SD
ASP|OD1
GLU|OE1
SER|OG
ASP|OD2
GLU|OE2
THR|OG1
CYH|SG
HIS|ND1 
TYR|OH
*|O
EODAT
sqlite3 init.gdb3 <<EOSQL
Drop Table donor;
Create Table donor (resnam text, name text);
.import "/tmp/donors.csv" donor
Drop Table acceptor;
Create Table acceptor (resnam text, name text);
.import "/tmp/acceptors.csv" acceptor
EOSQL
