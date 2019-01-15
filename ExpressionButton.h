#ifndef ExpressionButton_h_
#define ExpressionButton_h_ 

#include <gtkmm.h>
#include "STTModel.h"
#include "ExpressionParser/ExpressionClasses.h"


class ExpressionButton : public Gtk::Button{
private:
	ShortTruthTables::Expression* exp;
	Gtk::Grid* m_parent;
	ShortTruthTables::STTModel *m_model;
	int num, pos;
public:
	ExpressionButton(ShortTruthTables::Expression* e, Gtk::Grid* parent, ShortTruthTables::STTModel *model){
		this->exp = e;
		this->set_label(e->getExpChar());
		this->m_parent = parent;
		this->m_model = model;
	}
	void set_exp_num_and_pos(int num, int pos){
		this->num = num;
		this->pos = pos;
	}
	ShortTruthTables::Expression* exp_for_button(){
		return this->exp;
	}
	void on_clicked();
};

#endif
