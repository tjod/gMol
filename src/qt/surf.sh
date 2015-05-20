#! /bin/bash
cat >/tmp/params.tmp <<EODAT
molecular|func|vdw
molecular|contour|1.0
molecular|radius|0.0
molecular|step|0.75
molecular|padding|4.0
molecular|gradient|0
accessible|func|vdw
accessible|contour|1.0
accessible|radius|1.2
accessible|step|0.75
accessible|padding|4.0
accessible|gradient|0
custom|func|vdw
custom|contour|1.0
custom|radius|1.2
custom|step|0.5
custom|padding|4.0
custom|gradient|1
EODAT
sqlite3 init.gdb3 <<EOSQL
Drop Table If Exists params;
Create Table params (ptype Text, pname Text, pvalue Text);
.import "/tmp/params.tmp" params
EOSQL
