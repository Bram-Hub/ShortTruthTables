# ShortTruthTables
## Authors
2015:
Lee Saltzman

## About
A program to interactively solve STT Problems for bram's Computability and logic course at RPI

UI requires these packages:  pkg-config, gtk+-3.0, gtkmm-3.0
Run UI with: "make ui && ./stt-gui"
Run CLI with "make run"

Most of the actual logic of the program is in ExpressionParser/Parser && ExpressionParser/ExpressionClasses

Use STTCommandLineInterface to test new features before adding them to the ui. This makes sure that you properly seperated the model and view.


Features:
	Parses expressions in slate format
	Add Premises and Conclusions
	Change value of expressions based off of what can be derived at the time

To Do:
	Save/Load
	SubTables/Guessing support? <-- might be really hard
	Windows support <--- IDK if it's even possible


