# Variables
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic -O2
LDFLAGS=-lm -lgmp -lcrypto -lssl
OFLAGS=-I inc
EXEC=main.exe
SRC=main.c square_multiply.c chiffrement.c dechiffrement.c creation_des_cles.c miller_rabin.c rsa.c temps.c timing_attack.c
OBJ=$(SRC:.c=.o)
DIR_EXEC=bin
DIR_OBJ=obj
DIR_SRC=src
DIR_HEADERS=inc
BUILD_DIR=$(DIR_EXEC) $(DIR_OBJ)

# Spécifie le chemin où trouver les fichiers .exe,.o,.c,.h
vpath %.exe bin
vpath %.o obj
vpath %.c src
vpath %.h inc

# Lance toutes les étapes
all: dirs
	@./bin/main.exe
	
# Lance la création des dossiers bin et obj et l'édition de liens
dirs: checkdirs $(EXEC)

# Créer les répertoires s'ils ne sont pas déjà créés.
checkdirs: $(BUILD_DIR)

# Construit les dossiers bin et obj
$(BUILD_DIR):
	@mkdir -pv $@

# Edition de liens
main.exe: $(OBJ)
	@$(CC) $(CFLAGS) -o $(DIR_EXEC)/$@  $(addprefix $(DIR_OBJ)/,$^) $(LDFLAGS)

# Dépendance des fichiers .o
main.o: rsa.h

rsa.o: chiffrement.h dechiffrement.h creation_des_cles.h temps.h rsa.h timing_attack.h

square_multiply.o: square_multiply.h

miller_rabin.o: miller_rabin.h square_multiply.h creation_des_cles.h

chiffrement.o: chiffrement.h square_multiply.h miller_rabin.h

dechiffrement.o: dechiffrement.h chiffrement.h square_multiply.h

creation_des_cles.o: creation_des_cles.h miller_rabin.h

temps.o: temps.h

montgomery.o: montgomery.h

timing_attack.o: timing_attack.h temps.h creation_des_cles.h

# Compilation
%.o: %.c
	@$(CC) $(OFLAGS) $(CFLAGS) -c $< -o $(DIR_OBJ)/$@

.PHONY: clean

# Supprime les dossiers bin et obj et tout leurs contenus
clean:
	@clear
	@echo " "
	@rm -rfv $(DIR_EXEC)/*
	@rm -dfv $(DIR_EXEC)
	@rm -rfv $(DIR_OBJ)/*
	@rm -dfv $(DIR_OBJ)
	@echo "\r\n####################\r\n"
	@ls -G
	@echo "\r\n####################\r\n"
	@ls -lhFUGS $(DIR_SRC) $(DIR_HEADERS)
	
