CFLAGS+= -Wall -I$(INCDIR_APP_LOADER) -std=c99 -D_XOPEN_SOURCE=500 -O2 $(ARM_OPTIONS)
LDFLAGS+=-lpthread -lm -lwiringPi

dw1000-objs := platform.o deca_device.o deca_params_init.o

all: clean dw1000_tx dw1000_rx_cir
clean:
	rm -f clean dw1000_tx dw1000_rx_cir *.o

dw1000_tx: dw1000_tx.o $(dw1000-objs)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

dw1000_rx_cir: dw1000_rx_cir.o $(dw1000-objs)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)