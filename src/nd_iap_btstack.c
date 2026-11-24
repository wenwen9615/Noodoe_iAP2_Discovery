#include "nd_iap_btstack.h"
static uint8_t eir[ND_EIR_SIZE]; /* BTstack keeps pointers; static lifetime */
static uint8_t sdp[256];
static uint8_t installed;
int nd_iap_register(const uint8_t existing_eir[ND_EIR_SIZE], uint8_t spp_channel,
                    uint8_t iap_channel, btstack_packet_handler_t handler){
    if(installed || !handler || !spp_channel || spp_channel>30 || !iap_channel || iap_channel>30 || iap_channel==spp_channel) return -10;
    int result=nd_iap_eir_build(eir,existing_eir); if(result) return result;
    uint32_t handle=sdp_create_service_record_handle();
    /* Public BTstack RFCOMM record helper: service UUID in canonical byte order.
       Candidate SDP record, not a certified iAP2 service profile. Backend owner must verify. */
    spp_create_custom_sdp_record(sdp,handle,nd_iap_uuid_be,iap_channel,"NewDoe iAP Lab");
    if(de_get_len(sdp)>sizeof sdp) return -11;
    uint8_t status=rfcomm_register_service(handler,iap_channel,512);
    if(status) return -(100+(int)status);
    status=sdp_register_service(sdp);
    if(status){rfcomm_unregister_service(iap_channel); return -(400+(int)status);}
    gap_set_extended_inquiry_response(eir);
    installed=1;
    return 0;
}
int nd_iap_lab_eir_only(const uint8_t existing_eir[ND_EIR_SIZE]){
#if defined(ND_IAP_ENABLE_DISCOVERY_ONLY) && ND_IAP_ENABLE_DISCOVERY_ONLY
    if(installed) return -10;
    int result=nd_iap_eir_build(eir,existing_eir); if(result) return result;
    gap_set_extended_inquiry_response(eir); installed=1; return 0;
#else
    (void)existing_eir; return -20;
#endif
}
