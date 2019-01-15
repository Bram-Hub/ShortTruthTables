CC=g++
CFLAGS=-c
LDFLAGS=
SOURCES=STTModel.cpp ExpressionParser/ExpressionClasses.cpp ExpressionParser/Parser.cpp
CLI=STTCommandLineInterface.cpp
MODELOBJECTS=$(SOURCES:.cpp=.o)
CLOBJ=$(CLI:.cpp=.o)
EXECUTABLE=stt-cli
UICPP=STTUIMain.cpp STTPremiseDialogView.cpp STTConcDialogView.cpp ExpressionButton.cpp
UIOBJ=$(UICPP:.cpp=.o)
UIFLAGS=`pkg-config --cflags --libs gtkmm-3.0` -static-libgcc -static-libstdc++
UIEXE=stt-gui.out

all: $(EXECUTABLE)

run: all
	./$(EXECUTABLE)
    
$(EXECUTABLE): $(MODELOBJECTS) $(CLOBJ)
	$(CC) $(LDFLAGS) $(MODELOBJECTS) $(CLOBJ) -o $@

$(MODELOBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(MODELOBJECTS)
	rm -f $(CLOBJ)
	rm -f $(EXECUTABLE)
	rm -f $(UIOBJ)
	rm -f $(UIEXE)

ui: $(UIEXE)

$(UIOBJ): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@ $(UIFLAGS)

$(UIEXE): $(MODELOBJECTS) $(UIOBJ)
	$(CC) $(LDFLAGS) $(MODELOBJECTS) $(UIOBJ) -o $@ $(UIFLAGS)
