#include "ExpressionButton.h"
#include <gtkmm.h>
#include <iostream>

//randomly chosen numbers, no special meanings
#define TRUE_RESPONSE 12
#define FALSE_RESPONSE 13

void ExpressionButton::on_clicked(){
	Gtk::Dialog *tf_dialog = new Gtk::Dialog("True or False");
	tf_dialog->add_button("True", TRUE_RESPONSE);
	tf_dialog->add_button("False", FALSE_RESPONSE);
	int response = tf_dialog->run();
	if(!this->exp->isUnassigned()){
		Gtk::MessageDialog invalid("Expression already has a value");
		invalid.run();
		invalid.hide();
		tf_dialog->hide();
		delete tf_dialog;
		Gtk::Button::on_clicked();
		return;
	}
	if(response == TRUE_RESPONSE){
		if(m_model->can_assign_TVal(num, pos, true)){
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
		if(m_model->can_assign_TVal(num, pos, false)){
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
	tf_dialog->hide();
	delete tf_dialog;
	Gtk::Button::on_clicked();
}