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
LIBSRC = $(CURDIR)/src/
 

INCLUDES = \
-I/usr/include/postgresql/ \
-I/usr/include/postgresql/libpq/ \
-I/home/soleil/ZONED/src/ \
-I$(LIBSRC) \
-I$(OBJLIB)
 

OBJ = $(OBJDIR)$(EXEC).o  
 
 
# choix du compilateur :
CXX = g++
# options compilations :
CPPFLAGS=  -Wall -fexpensive-optimizations -O2 -Os -pedantic-errors  -Wextra -std=c++17 -fPIC  -Wparentheses -ftree-coalesce-vars  -fstack-protector -Wabi-tag -D_GLIBCXX_USE_CXX11_ABI=0

LDFLAGS=     -lpthread -lctemplate  -lX11   -lpq  -ldl 

INCLIB= $(OBJLIB)libpqwrp.a \
/home/soleil/ZONED/lib/libZONED.a \


#--------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"


# regle edition de liens  
all: $(OBJ)
ifdef TEST	
	$(CXX)  $(OBJ) -o $(BINDIR)*.$(EXEC)    $(LDFLAGS)        $(INCLIB) 
else
	$(CXX)  $(OBJ) -o $(BINDIR)*.$(EXEC)    $(LDFLAGS)     -s $(INCLIB)
endif 
# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX)  $(CPPFLAGS) $(INCLUDES)  -o $@ -c $<   


msg: mrpropre
	@echo "$(OBJ)"
	@echo "$(CURDIR)"
	@echo "$(SRCDIR)"
	@echo "$(BNDDIR)"
	@echo "$(OBJLIB)"
	@echo "$(LIBSRC)"

	
# pour effacer tous les objets :
clean: 
	rm -rf  $(OBJDIR)*.o

	
clean_a: 
	rm -rf  $(OBJDIR)*.o


clean_b:  
	rm -rf  $(OBJDIR)*.o

# pour effacer tous les objet et les executables :
mrpropre: clean_a   
	rm -rf $(BINDIR)*.$(EXEC) 

# efface objet(s) et affiche la taille de l'objet résultant
build: clean_b
	 du -sh $(BINDIR)*.$(EXEC) 

