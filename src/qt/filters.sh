#! /bin/bash
# if you add rows to this table, be sure to:
# 1. keep id column unique
# 2. use a menu name that cannot conflict with other names generated by gmol.
#    this typically means to not use e.g. CYS that will conflict with (be seen as abbreviation for) CYS4.
#    this applies to any residue name, standard or otherwise, or nucleic acid base name, e.g. A,G,T,U,C.
#    see use of CYS*, GLY*, MET*, PRO*
cat >/tmp/filters.tmp <<EODAT
0|0|0|NONE|0|
1|0|0|Atoms|atom.atid > 0|
2|6|T|Water|atom.resnam = 'HOH' Or atom.resnam = 'WAT'|
3|3|T|Main|atom.resnam Not In ('WAT', 'HOH') And atom.name In ('N', 'CA', 'C', 'O', 'OXT', 'H', 'HA', 'OP1', 'P', 'OP2', 'OP3', 'O5''', 'C5''', 'H5''', 'H5''''', 'H3''', 'H4''', 'C4''', 'C3''', 'O3''')|
4|4|T|Side|atom.resnam Not In ('WAT', 'HOH', 'A', 'C', 'G', 'T', 'U') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|
5|0|0|Residue|atom.resnam In ('ASP', 'ASN', 'GLU', 'GLN', 'HIS', 'LYS', 'ARG', 'GLY', 'ALA', 'VAL', 'LEU', 'ILE', 'SER', 'THR', 'TRP', 'TYR', 'PHE', 'CYS', 'PRO', 'MET')|standard 20
6|2|T|Trace|atom.name In ('CA', 'P')|
7|1|T|Cartoon|atom.resnam Not In ('WAT', 'HOH') And atom.name In ('CA', 'O')|
8|0|0|ATOM|atom.hetatm=0|
9|5|T|HETATM|atom.hetatm=1 And atom.resnam Not In ('HOH', 'WAT')|.pdb HETATM's
10|0|0|Molecule|atom.resnam Not In ('HOH', 'WAT')|all but water
11|7|M|CYS*|atom.resnam = 'CYS' And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|cysteine residues
12|7|M|MET*|atom.resnam = 'MET' And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|methionine residues
13|8|M|PRO*|atom.resnam = 'PRO' And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|proline residues
14|11|M|acidic|atom.resnam In ('ASP', 'ASN', 'GLU', 'GLN') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|acidic residues
15|12|M|basic|atom.resnam In ('HIS', 'LYS', 'ARG') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|basic residues
16|10|M|aliphatic|atom.resnam In ('ALA', 'VAL', 'LEU', 'ILE') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|aliphatic residues
17|13|M|hydroxylic|atom.resnam In ('SER', 'THR') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|hydroxylic residues
18|14|M|aromatic|atom.resnam In ('TRP', 'TYR', 'PHE') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|aromatic residues
19|15|M|other|atom.resnam Not In ('ASP', 'ASN', 'GLU', 'GLN', 'HIS', 'LYS', 'ARG', 'GLY', 'ALA', 'VAL', 'LEU', 'ILE', 'SER', 'THR', 'TRP', 'TYR', 'PHE', 'CYS', 'PRO', 'MET', 'WAT', 'HOH', 'A', 'C', 'G', 'T', 'U') And atom.name Not In ('CA', 'N', 'C', 'O', 'OXT')|non-standard
20|4|T|Bases|atom.resnam In ('A', 'C', 'G', 'T', 'U') And atom.name Not In ('H', 'HA', 'OP1', 'P', 'OP2', 'OP3', 'O5''', 'C5''', 'H5''', 'H5''''', 'H3''', 'H4''', 'C4''', 'C3''', 'O3''')|nucleotide bases
21|17|M|ATU*|atom.resnam In ('A', 'T', 'U') And atom.name Not In ('H', 'HA', 'OP1', 'P', 'OP2', 'OP3', 'O5''', 'C5''', 'H5''', 'H5''''', 'H3''', 'H4''', 'C4''', 'C3''', 'O3''')|AT(U) base pairs
22|18|M|GC*|atom.resnam In ('G', 'C') And atom.name Not In ('H', 'HA', 'OP1', 'P', 'OP2', 'OP3', 'O5''', 'C5''', 'H5''', 'H5''''', 'H3''', 'H4''', 'C4''', 'C3''', 'O3''')|GC base pairs
23|9|M|GLY*|atom.resnam='GLY' And atom.name='CA'|GLY
EODAT
sqlite3 init.gdb3 <<EOSQL
Drop Table filters;
Create Table filters (id Int Primary Key, ordering Int, appearance Text, menu Text, sql Text, tip Text);
.import "/tmp/filters.tmp" filters
EOSQL
