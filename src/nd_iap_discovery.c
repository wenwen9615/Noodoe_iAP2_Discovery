#include "nd_iap_discovery.h"
const uint8_t nd_iap_uuid_be[16]={0,0,0,0,0xde,0xca,0xfa,0xde,0xde,0xca,0xde,0xaf,0xde,0xca,0xca,0xff};
static int is_iap(const uint8_t *p){for(unsigned i=0;i<16;i++) if(p[i]!=nd_iap_uuid_be[15-i]) return 0; return 1;}
int nd_iap_eir_build(uint8_t out[ND_EIR_SIZE],const uint8_t in[ND_EIR_SIZE]){
    if(!out || !in || out==in) return -1;
    unsigned end=0, list=ND_EIR_SIZE; int found=0;
    while(end<ND_EIR_SIZE && in[end]){
        unsigned n=in[end];
        if(n+1>ND_EIR_SIZE-end) return -2;
        if(in[end+1]==6 || in[end+1]==7){
            if((n-1)%16) return -2;
            if(list!=ND_EIR_SIZE) return -2; /* ambiguous multiple lists */
            list=end;
            for(unsigned j=0;j<n-1;j+=16) if(is_iap(in+end+2+j)) found=1;
        }
        end+=n+1;
    }
    unsigned add=found?0:(list==ND_EIR_SIZE?18:16);
    if(add>ND_EIR_SIZE-end) return -3;
    for(unsigned i=0;i<ND_EIR_SIZE;i++) out[i]=0;
    if(found){for(unsigned i=0;i<end;i++) out[i]=in[i]; return 0;}
    unsigned at=(list==ND_EIR_SIZE)?end:list+1+in[list];
    for(unsigned i=0;i<at;i++) out[i]=in[i];
    if(list==ND_EIR_SIZE){out[at++]=17;out[at++]=6;} /* incomplete list; no false completeness claim */
    else out[list]=(uint8_t)(in[list]+16);
    for(unsigned i=0;i<16;i++) out[at+i]=nd_iap_uuid_be[15-i];
    if(list!=ND_EIR_SIZE){unsigned old_at=list+1+in[list];for(unsigned i=old_at;i<end;i++) out[i+16]=in[i];}
    return 0;
}
