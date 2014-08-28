#include <stdio.h>
#include <fcntl.h>
main (int argc, char **argv) {
#define NBITS 1024
  // unsigned int is 32 bits
  unsigned int buff[32]; // 32x32 is 1024 bits
  unsigned char *cbuff;
  int i,j,p,e,k,b,w,bits;
  int fd = open("/dev/urandom", O_RDONLY); 
  float density;
  for (density = 0.1; density < 1.0; density += 0.1) {
   printf("%f\n", density);
   int bits = 0;
   for (i=0;i < 32; ++i) { buff[i] = 0; }
   while (bits < 1024*density) {
    j = 0;
    read(fd, &j, 2);
    b = j>>6; // which bit to set, 0 to 1023;
    i = b / 32; // which array element
    k = b % 32; // which bit in the element
    w = 1 << k; // the int with that bit set
    if (buff[i] & w) {
      //printf("skip %d,%d,%08x ", i,k,w);
    } else {
      buff[i] |= w;
      //printf("%d,%d,%08x ", i,k,w);
      ++bits;
    }
   }

   cbuff = (unsigned char *) buff;
// 128 bytes is 1024 as well, and 8*16 is 128
   k = 0;
   for (i=0;i < 8; ++i) {
     for (j=0;j < 16; ++j) {
       printf ("0x%02x, ", cbuff[k++]);
     }
     printf ("\n");
   }
   printf ("\n");
 }
}

