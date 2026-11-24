#include "nd_iap_btstack.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static unsigned rf,sdp_count,unreg,adv;static uint8_t fail_rf,fail_sdp;static const uint8_t *live_eir;
uint32_t sdp_create_service_record_handle(void){return 0x10001;}
void spp_create_custom_sdp_record(uint8_t *r,uint32_t h,const uint8_t *uuid,int ch,const char *name){assert(h==0x10001 && ch==2 && !memcmp(uuid,nd_iap_uuid_be,16) && name);r[0]=1;}
uint32_t de_get_len(const uint8_t *r){assert(r[0]==1);return 80;}
uint8_t rfcomm_register_service(btstack_packet_handler_t h,uint8_t ch,uint16_t mtu){assert(h && ch==2 && mtu==512);rf++;return fail_rf;}
void rfcomm_unregister_service(uint8_t ch){assert(ch==2);unreg++;}
uint8_t sdp_register_service(const uint8_t *r){assert(r[0]==1);sdp_count++;return fail_sdp;}
void gap_set_extended_inquiry_response(const uint8_t *r){adv++;live_eir=r;}
static __attribute__((unused)) void handler(uint8_t a,uint16_t b,uint8_t *c,uint16_t d){(void)a;(void)b;(void)c;(void)d;}
int main(void){uint8_t e[240]={3,3,1,0x11};
#if defined(ND_IAP_ENABLE_DISCOVERY_ONLY) && ND_IAP_ENABLE_DISCOVERY_ONLY
assert(!nd_iap_lab_eir_only(e));assert(adv==1 && !rf && !sdp_count && live_eir[4]==17);assert(nd_iap_lab_eir_only(e)==-10);puts("PASS: explicit lab mode changes EIR only; no RFCOMM or SDP");
#else
assert(nd_iap_lab_eir_only(e)==-20);assert(nd_iap_register(e,1,1,handler)==-10);assert(nd_iap_register(e,1,2,0)==-10);assert(!rf&&!adv);
fail_rf=3;assert(nd_iap_register(e,1,2,handler)==-103 && rf==1 && !sdp_count && !adv);
fail_rf=0;fail_sdp=7;assert(nd_iap_register(e,1,2,handler)==-407 && rf==2 && sdp_count==1 && unreg==1 && !adv);
fail_sdp=0;assert(!nd_iap_register(e,1,2,handler) && rf==3 && sdp_count==2 && adv==1 && live_eir[4]==17);
assert(nd_iap_register(e,1,2,handler)==-10); puts("PASS: disabled lab default, channel collision, missing backend, failure rollback, successful call order (mock only)");
#endif
return 0;}
