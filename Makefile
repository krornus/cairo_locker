CC=gcc
DEPS=render.c

INC=`pkg-config --cflags --libs cairo x11` -lcrypt 
CFLAGS=-I. $(INC) 

ODIR=obj
_OBJ=cairo_locker.o render.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clocker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	sudo chown root: clocker
	sudo chmod u+s clocker


clean: 
	rm -f $(ODIR)/*.o clocker 
