#! /bin/bash
cat >/tmp/params.tmp <<EODAT
surface|molecular|func|vdw
surface|molecular|contour|1.0
surface|molecular|radius|0.0
surface|molecular|step|0.75
surface|molecular|padding|4.0
surface|molecular|gradient|0
surface|accessible|func|vdw
surface|accessible|contour|1.0
surface|accessible|radius|0.7
surface|accessible|step|0.75
surface|accessible|padding|4.0
surface|accessible|gradient|0
surface|custom|func|vdw
surface|custom|contour|1.0
surface|custom|radius|0.7
surface|custom|step|0.5
surface|custom|padding|4.0
surface|custom|gradient|1
EODAT
sqlite3 init.gdb3 <<EOSQL
Drop Table If Exists params;
Create Table params (pkind Text, pgroup Text, pname Text, pvalue Text);
.import "/tmp/params.tmp" params
EOSQL
