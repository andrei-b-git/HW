CC = cl
C_FLAGS = /W3 /D_CRT_SECURE_NO_DEPRECATE
SOURCE1 = tema1.c
SOURCE2 = hash.c
EXE = tema1.exe

all: build

build: $(SOURCE1) $(SOURCE2)
	$(CC) $(C_FLAGS) /Fe$(EXE) $**

clean: $(EXE)
	del $(EXE) *.obj
