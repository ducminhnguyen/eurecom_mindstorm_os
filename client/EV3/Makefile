
all:
	gcc -I./ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o tester.o
	gcc tester.o -Wall -lm -lev3dev-c -o tester
	gcc -std=gnu99 -W -Wall -c btclient.c -o btclient.o
	gcc btclient.o -Wall -lm -lev3dev-c -lbluetooth -o btclient
client:
	./btclient
run:
	./tester
