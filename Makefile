TARGET = tsSplit.exe

objects = lib_tssplit.o

EXTRA_CFLAGS +=	-Wall

release: $(objects)
	gcc $(EXTRA_CFLAGS) -O2 -o  $(TARGET)  $(objects)

debug: $(objects)
	gcc -g -o $(TARGET) $(objects)

lib_tssplit.o: tsSplit.c
	gcc -c tsSplit.c -o lib_tssplit.o


.PHONY:install
install:
	cp $(TARGET) ../bins/

.PHONY:clean
clean:
	rm -f $(objects) *.exe


