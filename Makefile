CC=gcc
DEPS =

INC=`pkg-config --cflags --libs cairo x11` -lcrypt
CFLAGS=-I. $(INC) 

ODIR = obj
_OBJ = cairo_locker.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clocker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	sudo chown root: clocker
	sudo chmod u+s clocker


clean: 
	rm -f $(ODIR)/*.o clocker 
