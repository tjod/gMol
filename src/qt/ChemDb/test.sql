-- sample sql to show how to use a chemdb
-- bonds between residue 2 and other residues in chain A
.header on
.mode column
select aid,a1.atnum,a1.resnum, bid,a2.atnum,a2.resnum from bond join atom a1 on a1.atid=aid join atom a2 on a2.atid=bid where (a1.resnum=2 or a2.resnum=2) and a1.chain='A' and a2.chain='A' and a2.resnum!=a1.resnum;
-- bonds within residue 5
select aid,a1.atnum,a1.resnum, bid,a2.atnum,a2.resnum from bond join atom a1 on a1.atid=aid join atom a2 on a2.atid=bid where (a1.resnum=5 and a2.resnum=5) and a1.chain='A' and a2.chain='A';
-- "new", non-standard residues
select distinct chain,residue.name,resnum from atom Join residue On residue.id=atom.resid where resid > (Select id from residue where name='UNK');
-- atoms from "new", non-standard residues
select chain,residue.name,resnum,atom.name from atom Join residue On residue.id=atom.resid where resid > (Select id from residue where name='UNK');
-- CA atoms from resuides LEU
select 'LEU' as residue,resnum,name,x,y,z from atom where resid=(Select id from residue where name='LEU') and chain='A' and name='CA';
-- bonds in a protein based on bonds from residues
attach database 'residue.db' as residue;
--create view residue.bondname as select a1.resnam as res, a1.name atom1,a2.name atom2, bo from (select atid,resnam,name from residue.atom) a1 join (select atid,name from residue.atom) a2 join residue.bond on bond.aid=a1.atid and bond.bid=a2.atid;
select a1.resnam||a1.resnum,a1.name,a1.atid ,a2.resnam||a2.resnum,a2.name,a2.atid,bo from atom a1 join atom a2 on a1.resnum=a2.resnum and a1.chain=a2.chain join residue.bondname on a1.name=atom1 and a2.name=atom2 and a1.resnam=res limit 10;
