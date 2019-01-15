# définition des cibles particulières
.PHONY: all mrprope build  clean

# désactivation des règles implicites
.SUFFIXES:
# nom de l'executable :
EXEC = srcsql


# gestion de version
VERSION_MAJOR := 0
VERSION_MINOR := 1
VERSION_PATCH := 0
VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

FLAGXXX =\
	-DDDSREP_VERSION_MAJOR=$(VERSION_MAJOR) \
	-DDDSREP_VERSION_MINOR=$(VERSION_MINOR) \
	-DDDSREP_VERSION_PATCH=$(VERSION_PATCH) \
	-DDDSREP_VERSION=\"$(VERSION)\"
	
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




INCLIB= 

OBJ = $(OBJDIR)$(EXEC).o  
EXECUTABLE = $(BINDIR)$(EXEC)



# choix du compilateur :
CXX = g++

# options compilations :


CPPFLAGS=    -std=c++17 -Wall -pedantic-errors  -fexceptions -Wno-parentheses -Waddress -Wsign-compare -fpermissive \
			-fexpensive-optimizations -ftree-coalesce-vars -fstack-protector-all  -fstack-clash-protection \
			`pkg-config --cflags gtk+-3.0`   

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

