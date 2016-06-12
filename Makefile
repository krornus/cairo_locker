CC=gcc
DEPS=render.c blur.c
FLAGS=-DHAVE_SHADOW_H

INC=`pkg-config --cflags --libs cairo x11` -lcrypt -lm
CFLAGS=-I. $(INC) $(FLAGS) 

ODIR=obj
_OBJ=cairo_locker.o render.o blur.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clocker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	sudo chown root: clocker
	sudo chmod u+s clocker


clean: 
	rm -f $(ODIR)/*.o clocker 
