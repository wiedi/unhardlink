SRCS=unhardlink.c
OBJS=$(SRCS:.c=.o)
CFLAGS?=-g -Wall -Wextra -O3 -std=c99 -D__EXTENSIONS__

all: $(OBJS)
	$(CC) -o unhardlink $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f unhardlink *.o
