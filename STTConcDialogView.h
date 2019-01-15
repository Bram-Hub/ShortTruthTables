#ifndef _STTConcDialogView_h_
#define _STTConcDialogView_h_ 
#include <gtkmm.h>
#include "STTModel.h"

class ConcDialog{
private:
	Gtk::Dialog* concDialog;
	Gtk::Button* cancel;
	Gtk::Button* accept;
	Gtk::Entry* conclusionText;
	Gtk::Window* mainWindow;
	ShortTruthTables::STTModel* model;
	Glib::RefPtr<Gtk::Builder> refBuilder;
public:
	ConcDialog(Glib::RefPtr<Gtk::Builder> refBuilder, ShortTruthTables::STTModel* model, Gtk::Window* inWindow){
		this->model = model;
		this->mainWindow = inWindow;
		this->refBuilder = refBuilder;
		refBuilder->get_widget("dialog2", concDialog);
		refBuilder->get_widget("button4", cancel);
		cancel->signal_clicked().connect( sigc::mem_fun(*this, &ConcDialog::cancel_clicked) );

		refBuilder->get_widget("button6", accept);
		accept->signal_clicked().connect( sigc::mem_fun(*this, &ConcDialog::accept_clicked) );

		refBuilder->get_widget("entry2", conclusionText);
	}
	void show();
	void accept_clicked();
	void cancel_clicked();
};

#endif
