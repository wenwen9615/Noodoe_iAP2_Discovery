/* Host test double only, NOT BTstack headers or an implementation. */
#ifndef MOCK_BTSTACK_H
#define MOCK_BTSTACK_H
#include <stdint.h>
typedef void (*btstack_packet_handler_t)(uint8_t,uint16_t,uint8_t *,uint16_t);
uint32_t sdp_create_service_record_handle(void);
void spp_create_custom_sdp_record(uint8_t *,uint32_t,const uint8_t *,int,const char *);
uint32_t de_get_len(const uint8_t *);
uint8_t rfcomm_register_service(btstack_packet_handler_t,uint8_t,uint16_t);
void rfcomm_unregister_service(uint8_t);
uint8_t sdp_register_service(const uint8_t *);
void gap_set_extended_inquiry_response(const uint8_t *);
#endif
