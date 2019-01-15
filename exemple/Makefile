# définition des cibles particulières
.PHONY: all mrprope build  clean

# désactivation des règles implicites
.SUFFIXES:
# nom de l'executable :
EXEC = srcsql

	
# Adresse des sources, des objets et des includes :
SRCDIR = $(CURDIR)/src/
OBJDIR = $(CURDIR)/obj/
BINDIR = $(CURDIR)/bin/
OBJLIB = $(CURDIR)/lib/

INCLUDES = \
-I$(SRCDIR) \
-I /usr/include/postgresql/ \
-I /usr/include/postgresql/libpq/ \
-I /home/soleil/ZLIBPQ/src/ \
-I/usr/include/gtk-3.0	\
-I/usr/lib64/gtk-3.0/include	\
-I/usr/include/atk-1.0	\
-I/usr/include/cairo	\
-I/usr/include/pango-1.0	\
-I/usr/include/glib-2.0	\
-I /usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
-I/usr/include/pixman-1	\
-I/usr/include/libpng12	\
-I//usr/include/gdk-pixbuf-2.0/ \



INCLIB= 

OBJ = $(OBJDIR)$(EXEC).o  
EXECUTABLE = $(BINDIR)$(EXEC)



# choix du compilateur :
CXX = g++

# options compilations :

CPPFLAGS=   -std=c++17 -Wall -pedantic-errors  -Wno-parentheses  -fexceptions  -Waddress \
			-ftree-coalesce-vars  -fstack-clash-protection -fstack-protector-all \
			-Wsign-compare -fpermissive 	-fexpensive-optimizations  \
			`pkg-config --cflags gtk+-3.0` `pkg-config --cflags gmodule-2.0`

LDFLAGS=   `pkg-config gtk+-3.0 --libs` `pkg-config gmodule-2.0 --libs`   -lpq  -lctemplate -no-pie 


ifdef PROD
   CDEBUG =  
else
   CDEBUG = -g 
endif


# -------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"
#
# debug -v   prod [ on enleve les symboles -s & compression ...]


# regle edition de liens  
all: $(OBJ)
ifdef PROD
	$(CXX)    -o $(EXECUTABLE)  $(OBJ)  $(LDFLAGS) -Os -s $(INCLIB)  
else
	$(CXX) -v -o $(EXECUTABLE)  $(OBJ)  $(LDFLAGS)   $(INCLIB) 
	du -sh $(EXECUTABLE)
endif	
 
 
# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CDEBUG) $(CPPFLAGS) $(FLAGXXX) $(INCLUDES)  -o $@ -c $< 


# pour effacer tous les objets :
clean: 
	rm -rf  $(OBJDIR)*.o

# pour effacer tous les objet et les executables :
mrprope: clean   
	rm -rf $(EXECUTABLE)
	
# efface objet(s) et affiche la taille de l'objet résultant
build: clean
	du -sh $(EXECUTABLE)

