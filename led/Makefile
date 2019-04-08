CC=clang
SRC=serial.c led_test.c

default:
	$(CC) serial.c led_test.c -o a.out
	

debug:
	$(CC) $(SRC) -g -o test
	sudo setcap cap_dac_override=pe test