PROGRAMS := ipconv lookup

all: $(PROGRAMS)

%: %.c
	gcc -Wall $< -o $@

clean:
	rm -f $(PROGRAMS) *~
