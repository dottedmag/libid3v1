OUT=showtag libid3v1.a

all: $(OUT)

showtag: libid3v1.a showtag.o

libid3v1.a: libid3v1.o
	ar r $@ $^
	ranlib $@

clean:
	-rm -f $(OUT) *.o
