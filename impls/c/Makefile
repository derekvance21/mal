SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
BINS = $(OBJS:%.o=%)
FLAGS = -Wall -I /usr/include -L /usr/lib -lreadline
LIBS = -lreadline
# OTHER_OBJS = 
# OTHER_HDRS = 

#####################

all: clean step0_repl

step0_repl: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step0_repl $(LIBS)

step0_repl: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step1_read_print $(LIBS)

%.o: %.c
	gcc $(FLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(BINS) $(OTHER_OBJS) mal