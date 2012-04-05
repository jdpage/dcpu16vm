SRCFILES := $(shell find . -name "*.c")
HDRFILES := $(shell find . -name "*.h")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))

.PHONY: all clean rebuild

CFLAGS += -Wall -Wextra -Werror

all: dcpu16vm

dcpu16vm: $(OBJFILES)
	$(CC) $(CFLAGS) -o dcpu16vm $(OBJFILES)

clean:
	-rm dcpu16vm *.o *.d

rebuild: clean all

-include $(DEPFILES)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

