CC=clang


default:
	$(CC) gpio.c gpio_test.c -o a.out
	sudo setcap cap_dac_override=pe a.out

debug:
	$(CC) $(SRC) -g -o test
	sudo setcap cap_dac_override=pe test