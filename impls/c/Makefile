SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
BINS = $(OBJS:%.o=%)
FLAGS = -Wall -I /usr/include -L /usr/lib -lreadline
LIBS = -lreadline
# OTHER_OBJS = 
# OTHER_HDRS = 

#####################

all: clean step4_if_lambda

step0_repl: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step0_repl $(LIBS)

step1_repl: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step1_read_print $(LIBS)

step2_eval: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step2_eval $(LIBS)

step3_env: $(OBJS)
	gcc $(FLAGS) $(OBJS) -o step3_env $(LIBS)

step4_if_lambda: $(OBJS)
	gcc -g $(FLAGS) $(OBJS) -o step4_if_lambda $(LIBS)

%.o: %.c
	gcc -g $(FLAGS) -c $< -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(BINS) $(OTHER_OBJS) mal