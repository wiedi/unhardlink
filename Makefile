SRCS=unhardlink.c
OBJS=$(SRCS:.c=.o)
CFLAGS?=-g -Wall -Wextra -O3 -std=c99 -D__EXTENSIONS__ -D_GNU_SOURCE

all: $(OBJS)
	$(CC) -o unhardlink $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f unhardlink *.o
