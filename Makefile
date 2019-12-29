CC=cl
CFLAGS=/FC /LD
INCLUDES=/Iinclude^\
SRC=src\*.c src\*.cpp
OBJ=/Fobuild^\
OUT=build\nostale_packet_sender.dll

!IF "$(DEBUG)"=="yes"
NT_SND_DEBUG=/DNT_SND_DEBUG
!ENDIF

!IF "$(LOG)"=="yes"
NT_SND_LOG=/DNT_SND_LOG
!ENDIF

build: {\build}nostale_packet_sender.dll

clean:
  del build\*.dll 2>NUL
  del build\*.obj 2>NUL

nostale_packet_sender.dll: src\*.c src\*.h src\*.cpp
  -@ if NOT EXIST "build" mkdir "build"
  CL $(CFLAGS) $(NT_SND_DEBUG) $(NT_SND_LOG) $(INCLUDES) $(OBJ) $(SRC) /link /OUT:$(OUT)