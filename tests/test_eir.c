#include "nd_iap_discovery.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
 uint8_t a[240]={5,9,'T','e','s','t',3,3,1,0x11},b[240],c[240];
 assert(!nd_iap_eir_build(b,a)); assert(!memcmp(a,b,10));
 assert(b[10]==17 && b[11]==6 && b[12]==0xff && b[27]==0 && b[28]==0);
 for(int i=0;i<16;i++) assert(b[12+i]==nd_iap_uuid_be[15-i]);
 assert(!nd_iap_eir_build(c,b) && !memcmp(b,c,240)); puts("PASS: existing name/SPP retained, iAP inserted and deduplicated");
 memset(a,0,240); a[0]=17;a[1]=7; memset(a+2,0x42,16); a[18]=3;a[19]=3;a[20]=1;a[21]=0x11;
 assert(!nd_iap_eir_build(b,a));assert(b[0]==33 && b[1]==7 && !memcmp(a+2,b+2,16) && !memcmp(a+18,b+34,4)); puts("PASS: merge existing UUID128 list and preserve subsequent field");
 memset(a,0,240);a[0]=239;a[1]=9;assert(nd_iap_eir_build(b,a)==-3);
 a[0]=240;assert(nd_iap_eir_build(b,a)==-2);
 a[0]=2;a[1]=7;assert(nd_iap_eir_build(b,a)==-2);
 assert(nd_iap_eir_build(a,a)==-1); puts("PASS: no space, malformed field, invalid UUID list, alias rejection");
 return 0;
}
