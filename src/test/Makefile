# définition des cibles particulières
.PHONY: all mrpropre build  clean

# désactivation des règles implicites
.SUFFIXES:
# nom de l'executable :
EXEC = Tiostream



# Adresse des sources, des objets et des includes :
SRCDIR = $(CURDIR)/test/
OBJDIR = $(CURDIR)/obj/
BINDIR = $(CURDIR)/bin/
OBJLIB = $(CURDIR)/lib/
 

INCLUDES = \
-I/usr/include/postgresql/ \
-I/home/soleil/ZONED/src/ \
-I$(CURDIR)/src/ \



OBJ = $(OBJDIR)$(EXEC).o  
EXECUTABLE = $(BINDIR)$(EXEC)
 
# choix du compilateur :
CXX = g++
# options compilations :

CPPFLAGS=  -Wall -fexpensive-optimizations -O2  -pedantic-errors  -Wextra -std=c++17 -fPIC  -Wparentheses -ftree-coalesce-vars  -fstack-protector 
LDFLAGS=   -lpthread -lctemplate  -lX11 -lpq 

INCLIB=\
/home/soleil/ZONED/lib/libZONED.a \


# -------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"



# regle edition de liens  
all: $(OBJ)
	
ifdef TEST	
	$(CXX)  $(OBJ) -o $(EXECUTABLE)  $(LDFLAGS)   $(INCLIB) 
else
	$(CXX)   $(OBJ) -o $(EXECUTABLE) $(LDFLAGS)   -s $(INCLIB) 
endif 
# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX)  $(CPPFLAGS) $(INCLUDES)  -o $@ -c $< 


# pour effacer tous les objets :
clean: 
	rm -rf  $(OBJDIR)*.o

	
clean_a: 
	rm -rf  $(OBJDIR)*.o


clean_b:  
	rm -rf  $(OBJDIR)*.o

# pour effacer tous les objet et les executables :
mrpropre: clean_a   
	rm -rf $(EXECUTABLE)

# efface objet(s) et affiche la taille de l'objet résultant
build: clean_b
	 du -sh $(EXECUTABLE)

