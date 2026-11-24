CC ?= gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Isrc
.PHONY: test clean
test:
	$(CC) $(CFLAGS) src/nd_iap_discovery.c tests/test_eir.c -o test_eir
	./test_eir
	$(CC) $(CFLAGS) -Itests/mock src/nd_iap_discovery.c src/nd_iap_btstack.c tests/test_registration.c -o test_reg
	./test_reg
	$(CC) $(CFLAGS) -Itests/mock -DND_IAP_ENABLE_DISCOVERY_ONLY=1 src/nd_iap_discovery.c src/nd_iap_btstack.c tests/test_registration.c -o test_lab
	./test_lab
clean:
	rm -f test_eir test_reg test_lab test_eir.exe test_reg.exe test_lab.exe
