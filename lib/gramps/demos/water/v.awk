NR==1 {n=$1;print n/2,$2,$3}
NR<=n+1 && NR%2==0 {print}
