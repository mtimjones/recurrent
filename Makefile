CC = gcc
CFLAGS = -I. -std=c99 -Wall

DEPS = nettypes.h ga.h rnn.h
OBJS = main.o ga.o rnn.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rnn: $(OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS) -lm

clean:
	rm -f rnn *.o
