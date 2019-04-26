#include <gtkmm.h>
#include <iostream>
#include <cstdlib>
#include "STTPremiseDialogView.h"
#include "STTModel.h"
#include "STTConcDialogView.h"

#define VAR_RESPONSE 14
#define EXPR_RESPONSE 15

/*
This is the main file for the logic behind the UI.
It has the functionality for the 'Check' button, the Find Contradiction Button, Add Premise and Add Conclusion
It has the high level code to tell when a button has been clicked, and when it has to call the approriate function
*/

//introducing variables for the UI
Glib::RefPtr<Gtk::Builder> refBuilder;
Glib::RefPtr<Gtk::Application> app;
Gtk::Window* mainWindow = 0;
Gtk::Button* premiseButton;
Gtk::Button* conclusionButton;
ShortTruthTables::STTModel* model;


/*
Functionality for the 'Check' button
*/
void solve(){
//See if 'Valid Assignment' has been ticked or not
  Gtk::CheckButton* checkButton;
  refBuilder->get_widget("checkbutton1", checkButton);
  char c;
  if(checkButton->get_active()){
//check if argument is valid
    c = 'v';
  }else{
//check if argument is invalid
    c = 'i';
  }

  if(c == 'v'){ //the user think the argument is valid, so check to find a contradiction
//for each premise
    for(int i = 0; i < model->getNumPremises(); i++){
//for each expression in that premise
      for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){
//find if that expression is valid: decided by
        bool flag = (model->getPremise(i)->getNthExpression(j)->isValid());
//if the expression is not valid the expression has been assigned
        if(!flag && !model->getPremise(i)->getNthExpression(j)->isUnassigned()){
//has found an expression that was assigned and not valid
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is valid, cannot make all premises true with conclusion false. The contradicting premise is "+model->getPremise(i)->getNthExpression(j)->print());
            int response = result->run();
            result->hide();
            return;
        }
      }
    }
//no contradictions in the premises, so check the conclusion
//loop through each nested expression in the conclusion
    for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
      bool flag = (model->getConclusion()->getNthExpression(i)->isValid());
//if the expression has been assigned and is not valid, there is a contradiction, thus the argument is valid
        if(!flag && !model->getConclusion()->getNthExpression(i)->isUnassigned()){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is valid, cannot make all premises true with conclusion false.  The contradiction is in the conclusion: "+model->getConclusion()->getNthExpression(i)->print());
            int response = result->run();
            result->hide();
          return;
        }
    }
//no contradictions were found in the premises or the conclusion

//check variables for contradictions
	std::map<char, std::vector<ShortTruthTables::Expression*> >::iterator it;
	std::map<char, std::vector<ShortTruthTables::Expression*> > variables = model->get_constants();
	for(it = variables.begin(); it != variables.end(); it++){
		std::vector<ShortTruthTables::Expression*> uses = it->second;
		char var = it->first;
		bool beenAssignedTrue= false;
		bool beenAssignedFalse = false;
		for(int i= 0; i< uses.size(); i++){
			if(uses[i]->isAssignedTrue()){
				beenAssignedTrue=true;
			}else if (uses[i]->isAssignedFalse()){
				beenAssignedFalse = true;
			}
		}if (beenAssignedTrue && beenAssignedFalse){
			std::string printmessage = "Argument is valid, there is a contradiction with the variable ";
			Gtk::MessageDialog *result = new Gtk::MessageDialog(printmessage+var);
			int response = result->run();
			result->hide();
			return;	
		}
	}

     Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is not currently valid; no contradictions were found");
     int response = result->run();
     result->hide();
    return;
  }else if( c == 'i'){ //check to see if the argument is invalid; i.e. it is possible for all premises to be true and the conclusion to be false
    for(int i = 0; i < model->getNumPremises(); i++){ //for each premise
      for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){ //for each nested expression within the premise
        bool flag = (!model->getPremise(i)->getNthExpression(j)->isUnassigned() && model->getPremise(i)->getNthExpression(j)->isValid());
        if(!flag){ //if the expression has been assigned true or fase that than assignment is not valid (there is a contradition)
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is not currently invalid.  A contradiction was found in the premise expression " + model->getPremise(i)->getNthExpression(j)->print());
            int response = result->run();
            result->hide();
          return;
        }
      }
    }
//also check the conclusion for any contradictions
    for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
      bool flag = (!model->getConclusion()->getNthExpression(i)->isUnassigned() && model->getConclusion()->getNthExpression(i)->isValid());
        if(!flag){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is not currently invalid.  A contradiction was found in the conclution " + model->getConclusion()->getNthExpression(i)->print());
            int response = result->run();
            result->hide();
          return;
        }
    }
	//check variables for contradictions
	std::map<char, std::vector<ShortTruthTables::Expression*> >::iterator it;
	std::map<char, std::vector<ShortTruthTables::Expression*> > variables = model->get_constants();
	for(it = variables.begin(); it != variables.end(); it++){
		std::vector<ShortTruthTables::Expression*> uses = it->second;
		char var = it->first;
		bool beenAssignedTrue= false;
		bool beenAssignedFalse = false;
		for(int i= 0; i< uses.size(); i++){
			if(uses[i]->isAssignedTrue()){
				beenAssignedTrue=true;
			}else if (uses[i]->isAssignedFalse()){
				beenAssignedFalse = true;
			}
		}if (beenAssignedTrue && beenAssignedFalse){
			std::string printmessage = "Argument is not currently invalid. There is a contradiction with the variable ";
			Gtk::MessageDialog *result = new Gtk::MessageDialog(printmessage+var);
			int response = result->run();
			result->hide();
			return;	
		}
	}

//no contradictions were found, so the argument is invalid
     Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is indeed invalid, all premises true with conclusion false and consistent variable assignments");
      int response = result->run();
     result->hide();
    return;
  }else{
    return;
  }
}


/*
This function is called when the 'Find Contradiction' button is pushed
Asks if it should look in the variable assignments or the expresssions, then does that and outputs it
*/
void findc(){
	// Open a dialog box to determine what kind of contradiction should be looked for
	Gtk::Dialog *tf_dialog = new Gtk::Dialog("Contradiction in Variable Assignment or within an Expression?");
	tf_dialog->add_button("Variable", VAR_RESPONSE);
	tf_dialog->add_button("Expression", EXPR_RESPONSE);
	int response = tf_dialog->run();
	tf_dialog->hide();

	if(response==EXPR_RESPONSE){ //go through all expressions and find one that is not valid (has a contradiction)
    		for(int i = 0; i < model->getNumPremises(); i++){ //go through each premise
      			for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){ //access an expression within a premise
				//if that expression is assigned and not valid
				if (!model->getPremise(i)->getNthExpression(j)->isValid() && !model->getPremise(i)->getNthExpression(j)->isUnassigned()){
				    Gtk::MessageDialog *result = new Gtk::MessageDialog("The contradicting premise expression is "+model->getPremise(i)->getNthExpression(j)->print());
				    int response = result->run();
				    result->hide();
				    return;
				}
      			}
    		}
		//no contradictions in the premises, so check the conclusion
		//loop through each nested expression in the conclusion
		for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
			//if there is a contradiction
		      	if(!model->getConclusion()->getNthExpression(i)->isValid() && !model->getConclusion()->getNthExpression(i)->isUnassigned()){
			    	Gtk::MessageDialog *result = new Gtk::MessageDialog("The contradiction is in the conclusion expression "+model->getConclusion()->getNthExpression(i)->print());
			    	int response = result->run();
			    	result->hide();
			  	return;
			}
		}
		//no contradictions were found in the premises or the conclusion
     		Gtk::MessageDialog *result = new Gtk::MessageDialog("No expression contradictions were found");
     		int response = result->run();
     		result->hide();
    		return;
	}else if(response==VAR_RESPONSE){ //go through all variables and find one that is not valid (is assigned different values)
		std::map<char, std::vector<ShortTruthTables::Expression*> >::iterator it;
		std::map<char, std::vector<ShortTruthTables::Expression*> > variables = model->get_constants();
		for(it = variables.begin(); it != variables.end(); it++){
			std::vector<ShortTruthTables::Expression*> uses = it->second;
			char var = it->first;
			bool beenAssignedTrue= false;
			bool beenAssignedFalse = false;
			for(int i= 0; i< uses.size(); i++){
				if(uses[i]->isAssignedTrue()){
					beenAssignedTrue=true;
				}else if (uses[i]->isAssignedFalse()){
					beenAssignedFalse = true;
				}
			}if (beenAssignedTrue && beenAssignedFalse){  //if the variable has been assigned to be both true and false within the expressions
				std::string printmessage = "There is a contradiction with the variable ";
				Gtk::MessageDialog *result = new Gtk::MessageDialog(printmessage+var);
				int response = result->run();
				result->hide();
				return;	
			}
		}
		Gtk::MessageDialog *result = new Gtk::MessageDialog("No variable contradictions were found");
		int response = result->run();
		result->hide();
		return;
	}
	return;
}


/*
Overarching function that is run when the UI launches
*/
int main (int argc, char **argv)
{
//Setup GUI 
  model = new ShortTruthTables::STTModel();
  app = Gtk::Application::create(argc, argv, "com.rpi.saltzl.stt");

  //Load the GtkBuilder file and instantiate its widgets:  Details of frontend are in interface_prototype.glade file
  refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("interface_prototype.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated Dialog:
  refBuilder->get_widget("mainWindow", mainWindow);
  PremDialog premView(refBuilder, model, mainWindow);
  ConcDialog concView(refBuilder,model,mainWindow);
  if(mainWindow)
  {
    // mainWindow->set_default_size(800,600);
    refBuilder->get_widget("premButton", premiseButton); //add a Premise
    premiseButton->signal_clicked().connect( sigc::mem_fun(premView, &PremDialog::show) );

    refBuilder->get_widget("concButton", conclusionButton); //add Conclusion
    conclusionButton->signal_clicked().connect( sigc::mem_fun(concView, &ConcDialog::show) );

    Gtk::Button* but5;
    refBuilder->get_widget("button5", but5); //Check if argument is valid
    but5->signal_clicked().connect( sigc::ptr_fun(&solve) );

Gtk::Button* but7;
    refBuilder->get_widget("button7", but7); //Try to find contraditions
    but7->signal_clicked().connect( sigc::ptr_fun(&findc) );


    app->run(*mainWindow);
  }

  delete mainWindow;

  return 0;
}
