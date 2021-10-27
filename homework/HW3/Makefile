CC = clang
CFLAGS = -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla
EX3_SRC = *.c
EX3 = ex3
EX3MC = ex3_memory_check
EX3MC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
.PHONY: clean

all: $(EX3) $(EX3MC)
	@echo ex3 successfully constructed

$(EX3): $(EX3_SRC)
	$(CC) $(CFLAGS) -o $(EX3) $(EX3_SRC)

$(EX3MC): $(EX3_SRC)
	$(CC) $(CFLAGS) $(EX3MC_FLAGS) -o $(EX3MC) $(EX3_SRC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *.a *~ $(EX3) $(EX3MC)

compress:
	tar -cf hw3.tar ex2.c ex2.h

test:
	./ex3 rand_char*.txt inc
	./ex3 rand_int.txt inc

# qh:
# 	clang -std=gnu11 -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla -o ex3 ex3.c

# ./ex3_memory_check samples/rand_char*.txt inc