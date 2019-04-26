# ShortTruthTables
## Authors
2015:
Lee Saltzman  
2019:
Lindsay Zadunayski

## About
A program to interactively solve STT Problems for bram's Computability and logic course at RPI

UI requires these packages:  pkg-config, gtk+-3.0, gtkmm-3.0
Run UI with: "make ui && ./stt-gui"
Run CLI with "make run"
After making any changes to a file, run "make clean" and then run again as above.

Most of the actual logic of the program is in ExpressionParser/Parser && ExpressionParser/ExpressionClasses

Each file begins with a brief description of what it does and there are comments throughout

Use STTCommandLineInterface to test new features before adding them to the ui. This makes sure that you properly seperated the model and view.


Features:
	Parses expressions in slate format
	Add Premises and Conclusions
	User can add truth values after selecting the proper justification
	Checks for contradictions
	Checks if the argument is valid

To Do:
	Save/Load
	SubTables/Guessing support? <-- might be really hard
	Windows support <--- IDK if it's even possible


