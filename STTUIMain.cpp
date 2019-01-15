#include <gtkmm.h>
#include <iostream>
#include <cstdlib>
#include "STTPremiseDialogView.h"
#include "STTModel.h"
#include "STTConcDialogView.h"


Glib::RefPtr<Gtk::Builder> refBuilder;
Glib::RefPtr<Gtk::Application> app;
Gtk::Window* mainWindow = 0;
Gtk::Button* premiseButton;
Gtk::Button* conclusionButton;
ShortTruthTables::STTModel* model;

void solve(){
  Gtk::CheckButton* checkButton;
  refBuilder->get_widget("checkbutton1", checkButton);
  char c;
  if(checkButton->get_active()){
    c = 'v';
  }else{
    c = 'i';
  }


  if(c == 'v'){
    for(int i = 0; i < model->getNumPremises(); i++){
      for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){
        bool flag = (model->getPremise(i)->getNthExpression(j)->isValid());
        if(!flag && !model->getPremise(i)->getNthExpression(j)->isUnassigned()){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is valid, cannot make all premises true with conclusion false");
            int response = result->run();
            result->hide();
            return;
        }
      }
    }

    for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
      bool flag = (model->getConclusion()->getNthExpression(i)->isValid());
        if(!flag && !model->getConclusion()->getNthExpression(i)->isUnassigned()){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is valid, cannot make all premises true with conclusion false");
            int response = result->run();
            result->hide();
          return;
        }
    }
     Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is not currently valid");
     int response = result->run();
     result->hide();
    return;
  }else if( c == 'i'){
    for(int i = 0; i < model->getNumPremises(); i++){
      for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){
        bool flag = (!model->getPremise(i)->getNthExpression(j)->isUnassigned() && model->getPremise(i)->getNthExpression(j)->isValid());
        if(!flag){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("argument is not currently invalid");
            int response = result->run();
            result->hide();
          return;
        }
      }
    }

    for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
      bool flag = (!model->getConclusion()->getNthExpression(i)->isUnassigned() && model->getConclusion()->getNthExpression(i)->isValid());
        if(!flag){
            Gtk::MessageDialog *result = new Gtk::MessageDialog("argument is not currently invalid");
            int response = result->run();
            result->hide();
          return;
        }
    }
     Gtk::MessageDialog *result = new Gtk::MessageDialog("Argument is indeed invalid, all premises true with conclusion false");
      int response = result->run();
     result->hide();
    return;
  }else{
    return;
  }
}

int main (int argc, char **argv)
{
  model = new ShortTruthTables::STTModel();
  app = Gtk::Application::create(argc, argv, "com.rpi.saltzl.stt");

  //Load the GtkBuilder file and instantiate its widgets:
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
    refBuilder->get_widget("premButton", premiseButton);
    premiseButton->signal_clicked().connect( sigc::mem_fun(premView, &PremDialog::show) );

    refBuilder->get_widget("concButton", conclusionButton);
    conclusionButton->signal_clicked().connect( sigc::mem_fun(concView, &ConcDialog::show) );

    Gtk::Button* but5;
    refBuilder->get_widget("button5", but5);
    but5->signal_clicked().connect( sigc::ptr_fun(&solve) );

    app->run(*mainWindow);
  }

  delete mainWindow;

  return 0;
}