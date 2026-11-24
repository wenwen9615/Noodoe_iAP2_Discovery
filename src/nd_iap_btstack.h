#ifndef ND_IAP_BTSTACK_H
#define ND_IAP_BTSTACK_H
#include "btstack.h"
#include "nd_iap_discovery.h"
/* Call once during Product BT initialization, after RFCOMM/SDP init and before power on.
 * Only an authorized, initialized iAP backend may supply handler. Existing SPP channel must differ.
 * Caller owns pairing, security, backend readiness, restart and shutdown. Never call in Gate/Bootstrap.
 */
int nd_iap_register(const uint8_t existing_eir[ND_EIR_SIZE], uint8_t spp_channel,
                    uint8_t iap_channel, btstack_packet_handler_t handler);
/* Explicit discovery-only experiment. Does NOT register RFCOMM or perform authentication.
 * Compile with -DND_IAP_ENABLE_DISCOVERY_ONLY=1 on a bench build ONLY. */
int nd_iap_lab_eir_only(const uint8_t existing_eir[ND_EIR_SIZE]);
#endif
