#ifndef ND_IAP_DISCOVERY_H
#define ND_IAP_DISCOVERY_H
#include <stdint.h>
#define ND_EIR_SIZE 240u
/* 0 = success; negative = invalid/insufficient capacity. out and in must not overlap. */
int nd_iap_eir_build(uint8_t out[ND_EIR_SIZE], const uint8_t in[ND_EIR_SIZE]);
extern const uint8_t nd_iap_uuid_be[16];
#endif
