CC=clang


default:
	$(CC) serial.c led_serial_test.c -o a.out
	

debug:
	$(CC) $(SRC) -g -o test
	sudo setcap cap_dac_override=pe test