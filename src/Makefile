# définition des cibles particulières
.PHONY: all mrproper build  clean

# désactivation des règles implicites
.SUFFIXES:
# nom de l'executable :
EXEC =Tclasse


# Adresse des sources, des objets et des includes :
SRCDIR = $(CURDIR)/test/
OBJDIR = $(CURDIR)/obj/
BINDIR = $(CURDIR)/bin/
OBJLIB = $(CURDIR)/lib/
LIBSRC = $(CURDIR)/src/


INCLUDES = \
-I/usr/local/pgsql/include/ \
-I/usr/include/postgresql/ \
-I/usr/include/postgresql/libpq/ \
-I/home/soleil/ZONED/src/ \
-I$(LIBSRC) \
-I$(OBJLIB)




OBJ = $(OBJDIR)$(EXEC).o  
 
 
# choix du compilateur :
CXX = g++
# options compilations :
CPPFLAGS=	-std=c++17	-Wall	-fexceptions	-pedantic-errors	-Wextra	-Wparentheses	-Waddress	\
			-ftree-coalesce-vars  -fstack-clash-protection -fstack-protector-all  \
			-Wsign-compare -fpermissive 	-fexpensive-optimizations
 

LDFLAGS=     -lpthread -lctemplate  -lX11   -lpq  -ldl  

# tclasse
ifeq ($(EXEC), Tclasse)
INCLIB= /home/soleil/ZONED/lib/libZONED.a 
endif

# tiostream
ifeq ($(EXEC), Tiostream)
INCLIB= $(OBJLIB)/libpqwrp.a \
/home/soleil/ZONED/lib/libZONED.a 
endif

#--------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"


# regle edition de liens  
all: $(OBJ)
ifdef TEST	
	$(CXX)  $(OBJ) -o $(BINDIR)$(EXEC)    $(LDFLAGS)      -s $(INCLIB) 
else
	$(CXX)  $(OBJ) -o $(BINDIR)$(EXEC)    $(LDFLAGS)   -no-pie  -Os -s $(INCLIB)
endif 
# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)$(EXEC).cpp
	$(CXX)  $(CPPFLAGS) $(INCLUDES) -o $@ -c $<   


msg: mrproper
	@echo "$(OBJ)"
	@echo "$(CURDIR)"
	@echo "$(SRCDIR)"
	@echo "$(BNDDIR)"
	@echo "$(OBJLIB)"
	@echo "$(LIBSRC)"

	
# pour effacer tous les objets :
clean: 
	rm -rf  $(OBJDIR)$(EXEC).o

	
clean_a: 
	rm -rf  $(OBJDIR)$(EXEC).o


clean_b:  
	rm -rf  $(OBJDIR)$(EXEC).o

# pour effacer tous les objet et les executables :
mrproper: clean_a   
	rm -rf $(BINDIR)$(EXEC) 

# efface objet(s) et affiche la taille de l'objet résultant
build: clean_b
	 du -sh $(BINDIR)$(EXEC) 

