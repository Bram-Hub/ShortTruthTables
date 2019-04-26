#include "ExpressionButton.h"
#include <gtkmm.h>
#include <iostream>

//randomly chosen numbers, no special meanings
#define TRUE_RESPONSE 12
#define FALSE_RESPONSE 13
#define VAR_RESPONSE 14
#define EXPR_RESPONSE 15

/*
This function is called when you click on a variable or expresion and try to assign a truth value to it
It is the only function in the file
*/
void ExpressionButton::on_clicked(){
	// Make sure the variable is unassigned
	if(!this->exp->isUnassigned()){
		Gtk::MessageDialog invalid("Expression already has a value");
		invalid.run();
		invalid.hide();
		Gtk::Button::on_clicked();
		return;
	}
	//Ask the user whether they hope to assign True or False
	Gtk::Dialog *tf_dialog = new Gtk::Dialog("True or False");
	tf_dialog->add_button("True", TRUE_RESPONSE);
	tf_dialog->add_button("False", FALSE_RESPONSE);
	int response = tf_dialog->run();
	tf_dialog->hide();
	
	// Ask the user what their justification for this is 
	Gtk::Dialog *tf_dialog1 = new Gtk::Dialog("Justification? (Variable is previously assigned this value or expression mandates it)");
	tf_dialog1->add_button("Variable", VAR_RESPONSE);
	tf_dialog1->add_button("Expression",EXPR_RESPONSE);
	int response1 = tf_dialog1->run();

	if(response1 == VAR_RESPONSE){	//If they are assigning it based on variable, they must be looking at an atomic value that has been assigned this truth value in the past
		if(response == TRUE_RESPONSE){
			if(m_model->can_assign_TVal_var(num, pos, true)){ // check if this is possible, and if so update the UI
				this->exp->setTruthValue(true);
				Gtk::Label* true_label = Gtk::manage(new Gtk::Label("T"));
				true_label->show();
				m_parent->attach_next_to(*true_label, *this, Gtk::POS_BOTTOM ,1,1);
			}else{
				Gtk::MessageDialog invalid("Not a valid assignment");
				invalid.run();
				invalid.hide();
			}
		}else if(response == FALSE_RESPONSE){
			if(m_model->can_assign_TVal_var(num, pos, false)){ //Check if this is possible, and if so update the UI
				this->exp->setTruthValue(false);
				Gtk::Label* true_label = Gtk::manage(new Gtk::Label("F"));
				true_label->show();
				m_parent->attach_next_to(*true_label, *this, Gtk::POS_BOTTOM ,1,1);
			}else{
				Gtk::MessageDialog invalid("Not a valid assignment");
				invalid.run();
				invalid.hide();
			}
			
		}
	}else if(response1 == EXPR_RESPONSE){ // The user is hoping to assign a truth value based on the rest of the expression it is in
		if(response == TRUE_RESPONSE){
			if(m_model->can_assign_TVal_expr(num, pos, true)){ //check if this is valid, and if so update the UI
				this->exp->setTruthValue(true);
				Gtk::Label* true_label = Gtk::manage(new Gtk::Label("T"));
				true_label->show();
				m_parent->attach_next_to(*true_label, *this, Gtk::POS_BOTTOM ,1,1);
			}else{
				Gtk::MessageDialog invalid("Not a valid assignment");
				invalid.run();
				invalid.hide();
			}
		}else if(response == FALSE_RESPONSE){
			if(m_model->can_assign_TVal_expr(num, pos, false)){ //check if this is valid and if so update the UI
				this->exp->setTruthValue(false);
				Gtk::Label* true_label = Gtk::manage(new Gtk::Label("F"));
				true_label->show();
				m_parent->attach_next_to(*true_label, *this, Gtk::POS_BOTTOM ,1,1);
			}else{
				Gtk::MessageDialog invalid("Not a valid assignment");
				invalid.run();
				invalid.hide();
			}
			
		}
	}
	//end of function; clean up dialog boxes and then start up waiting again
	tf_dialog1->hide();
	delete tf_dialog1;
	delete tf_dialog;
	Gtk::Button::on_clicked();
}

