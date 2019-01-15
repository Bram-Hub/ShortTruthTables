#include "STTConcDialogView.h"
#include "ExpressionButton.h"
#include <stdexcept>
#include <gtkmm.h>

void ConcDialog::show(){
	this->conclusionText->set_text("");
    int retval = concDialog->run();
}	

void ConcDialog::cancel_clicked(){
	concDialog->hide();
}

void ConcDialog::accept_clicked(){
	std::string conc_Exp = this->conclusionText->get_text();
	try{
		this->model->addConclusion(conc_Exp);
		ShortTruthTables::ParsedExpression* new_exp = this->model->getConclusion();
		Gtk::Grid* conc_grid = Gtk::manage(new Gtk::Grid());
    	Gtk::Box* conclusion_box;

    	refBuilder->get_widget("conclusion_box", conclusion_box);
    	if(conclusion_box->get_children().size() != 0) conclusion_box->remove(*(conclusion_box->get_children()[0]));
    	conclusion_box->pack_start(*conc_grid, true, true);
    	conclusion_box->show();
    	std::string inorder = new_exp->getInOrderExp();
		for(int j = 0; j < inorder.size(); j++){
			if(new_exp->expressionAtPosition(j) == NULL){
				Gtk::Label* temp = Gtk::manage(new Gtk::Label(std::string(1,inorder[j])));
				temp->show();
				conc_grid->add(*temp);
			}else{
				ExpressionButton* temp = Gtk::manage(new ExpressionButton(new_exp->expressionAtPosition(j), conc_grid, this->model));
				temp->set_exp_num_and_pos(-1, j);
				temp->show();
				conc_grid->add(*temp);
				if(new_exp->isTopLevel(new_exp->expressionAtPosition(j))){
					Gtk::Label* true_label = Gtk::manage(new Gtk::Label("F"));
					true_label->show();
					conc_grid->attach_next_to(*true_label, *temp, Gtk::POS_BOTTOM ,1,1);

				}
				if(inorder[j] == '<'){
					j+=2;
				}
				else if(inorder[j] == '-'){
					j+=1;
				}

			}
		}


		conc_grid->show();
		conclusion_box->show();
		concDialog->hide();
	}catch(std::invalid_argument e){
		this->conclusionText->set_text("Poorly Formatted Expression");
	}
}