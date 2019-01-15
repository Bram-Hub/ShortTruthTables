#ifndef _STTPremiseDialogView_h_
#define _STTPremiseDialogView_h_ 
#include <gtkmm.h>
#include "STTModel.h"

class PremDialog{
private:
	Gtk::Dialog* premDialog;
	Gtk::Button* cancel;
	Gtk::Button* accept;
	Gtk::Entry* premiseText;
	Gtk::Window* mainWindow;
	ShortTruthTables::STTModel* model;
	Glib::RefPtr<Gtk::Builder> refBuilder;
public:
	PremDialog(Glib::RefPtr<Gtk::Builder> refBuilder, ShortTruthTables::STTModel* model, Gtk::Window* inWindow){
		this->model = model;
		this->mainWindow = inWindow;
		this->refBuilder = refBuilder;
		refBuilder->get_widget("dialog1", premDialog);
		refBuilder->get_widget("button1", cancel);
		cancel->signal_clicked().connect( sigc::mem_fun(*this, &PremDialog::cancel_clicked) );

		refBuilder->get_widget("button2", accept);
		accept->signal_clicked().connect( sigc::mem_fun(*this, &PremDialog::accept_clicked) );

		refBuilder->get_widget("entry1", premiseText);
	}
	void show();
	void accept_clicked();
	void cancel_clicked();
};

#endif
