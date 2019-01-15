#include "STTModel.h"
#include "ExpressionParser/ExpressionClasses.h"
#include "ExpressionParser/Parser.h"
#include "TruthValues.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

void optionsMenu(){
	std::cout << "Options Menu: " << std::endl;
	std::cout << "p : adds a premise" << std::endl;
	std::cout << "c : changes the conclusion" << std::endl;
	std::cout << "d : displays current state and expression assignments" << std::endl;
	std::cout << "a : assign a truth value" << std::endl;
	std::cout << "q : quits" << std::endl;
}

void printTruthVals(ShortTruthTables::ParsedExpression* exp){
	std::string result = "    ";
	int len = exp->getInOrderExp().length();
	for(int i = 0; i < len; i++){
		if(exp->expressionAtPosition(i) != NULL){
			if(exp->expressionAtPosition(i)->getExpressionType() == "BiConditionalOperator"){
				result.push_back(' ');
			}
			if(exp->expressionAtPosition(i)->isUnassigned()){
				result.push_back(' ');
			}
			else if(exp->expressionAtPosition(i)->isAssignedTrue()){
				result.push_back('T');
			}else{
				result.push_back('F');
			}
		}else{
			if(exp->expressionAtPosition(i-1) != NULL && exp->expressionAtPosition(i-1)->getExpressionType() == "BiConditionalOperator"){
				continue;
			}
			result.push_back(' ');
		}
	}
	std::cout << result << std::endl;
}
void displayCurrentState(ShortTruthTables::STTModel* model){
	std::cout << "Current State:" << std::endl;
	std::cout << "Premises:" << std::endl;
	int count = model->getNumPremises();
	for(int i = 0; i < count; i++){
		std::cout << "P"<< i << "  " << model->getPremise(i)->getInOrderExp() << std::endl;
		printTruthVals(model->getPremise(i));
	}
	std::cout << std::endl;
	std::cout << "Conclusion:" << std::endl;

	if(!model->has_conclusion()) return;
	std::cout << "    " << model->getConclusion()->getInOrderExp() << std::endl;
	printTruthVals(model->getConclusion());
}

void addPremise(ShortTruthTables::STTModel* model){
	std::string premise;
	while(true){
		std::cout << "Enter premise in slate style preorder format: " << std::endl;
		std::cout << ">>";
		std::getline(std::cin, premise);
		if(premise == "q") return;
		try{
			model->addPremise(premise);
			return;
		}catch(std::invalid_argument &e){
			std::cout << "Expression not formatted correctly." << std::endl;
		}
	}
}

void addConclusion(ShortTruthTables::STTModel* model){
	if(model->has_conclusion()){
		std::cout << "Conclusion already exists, would you like to replace it? (y/n):";
		char c;
		std::cin >> c;
		std::cin.ignore();
		if(c != 'y') return;
	}
	std::string conclusion;
	while(true){
		std::cout << "Enter conclusion in slate style preorder format: " << std::endl;
		std::cout << ">>";
		std::getline(std::cin, conclusion);
		if(conclusion == "q") return;
		try{
			model->addConclusion(conclusion);
			return;
		}catch(std::invalid_argument &e){
			std::cout << "Expression not formatted correctly." << std::endl;
		}
	}
}
void printSubExpressions(ShortTruthTables::ParsedExpression* expression){
	std::cout << "Subexpressions of: " << expression->getInOrderExp() << std::endl;
	for(int i = 0; i < expression->getNumExpressions(); i++){
		ShortTruthTables::TruthValue tval = expression->getNthExpression(i)->getTVal();
		std::string assignment;
		if(tval == ShortTruthTables::UNASSIGNED){
			assignment = "";
		}else if(tval == ShortTruthTables::ASSIGNED_TRUE){
			assignment = "   assigned T";
		}else{
			assignment = "   assigned F";
		}
		std::cout << i << ":  " << expression->getNthExpression(i)->print() << assignment << std::endl;
	}
}

void assignPremise(ShortTruthTables::STTModel* model, int i){
	ShortTruthTables::ParsedExpression* expression = model->getPremise(i);
	while(true){
		printSubExpressions(expression);
		std::cout << "Enter the subexpression you wish to assign a value to: ";
		int i;
		std::cin >> i;
		if(i >= expression->getNumExpressions()) continue;
		std::cout << "T or F: ";
		char c;
		std::cin >> c;
		if(c == 'T'){
			expression->getNthExpression(i)->setTruthValue(true);
		}else if(c == 'F'){
			expression->getNthExpression(i)->setTruthValue(false);
		}else if(c == 'q'){
			return;
		}else{
			continue;
		}
		return;
	}
}

void assignConclusion(ShortTruthTables::STTModel* model){
	ShortTruthTables::ParsedExpression* expression = model->getConclusion();
	while(true){
		printSubExpressions(expression);
		std::cout << "Enter the subexpression you wish to assign a value to: ";
		int i;
		std::cin >> i;
		if(i >= expression->getNumExpressions()) continue;
		std::cout << "T or F: ";
		char c;
		std::cin >> c;
		if(c == 'T'){
			expression->getNthExpression(i)->setTruthValue(true);
		}else if(c == 'F'){
			expression->getNthExpression(i)->setTruthValue(false);
		}else if(c == 'q'){
			return;
		}else{
			continue;
		}
		return;
	}
}

void makeAssignment(ShortTruthTables::STTModel* model){
	while(true){
		char command;
		displayCurrentState(model);
		std::cout << "Enter either the premise number or c for conclusion: ";
		std::cin >> command;
		std::cin.ignore();	
		if(command >= '0' && command <= '9'){
			if(command-'0' >= model->getNumPremises()) continue;
			assignPremise(model, command-'1'+1);
			return;
		}else if(command == 'c'){
			assignConclusion(model);
			return;
		}else if(command == 'q'){
			return;
		}
	}
}

void solve(ShortTruthTables::STTModel* model){
	std::cout << "Is argument valid or invalid? (v/i): ";
	char c;
	std::cin >> c;
	if(c == 'v'){
		for(int i = 0; i < model->getNumPremises(); i++){
			for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){
				bool flag = (model->getPremise(i)->getNthExpression(j)->isValid());
				if(!flag && !model->getPremise(i)->getNthExpression(j)->isUnassigned()){
					std::cout << "Argument is valid, cannot make all premises true with conclusion false" << std::endl;
					return;
				}
			}
		}

		for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
			bool flag = (model->getConclusion()->getNthExpression(i)->isValid());
				if(!flag && !model->getConclusion()->getNthExpression(i)->isUnassigned()){
					std::cout << "Argument is valid, cannot make all premises true with conclusion false" << std::endl;
					return;
				}
		}
		std::cout << "Argument is not currently valid" << std::endl;
		return;
	}else if( c == 'i'){
		for(int i = 0; i < model->getNumPremises(); i++){
			for(int j = 0; j < model->getPremise(i)->getNumExpressions(); j++){
				bool flag = (!model->getPremise(i)->getNthExpression(j)->isUnassigned() && model->getPremise(i)->getNthExpression(j)->isValid());
				if(!flag){
					std::cout << "argument is not currently invalid" << std::endl;
					return;
				}
			}
		}

		for(int i = 0; i < model->getConclusion()->getNumExpressions(); i++){
			bool flag = (!model->getConclusion()->getNthExpression(i)->isUnassigned() && model->getConclusion()->getNthExpression(i)->isValid());
				if(!flag){
					std::cout << "argument is not currently invalid" << std::endl;
					return;
				}
		}
		std::cout << "Argument is indeed invalid, all premises true with conclusion false" << std::endl;
		return;
	}else{
		return;
	}
}
void programLoop(ShortTruthTables::STTModel* model){
	std::string command;
	while(true){
		std::cout << "Enter a command: ";
		std::cin >> command;
		std::cin.ignore();
		if(command == "h"){
			optionsMenu();
		}else if(command == "p"){
			addPremise(model);
		}else if(command == "c"){
			addConclusion(model);
		}else if(command == "q"){
			return;
		}else if(command == "d"){
			displayCurrentState(model);
		}else if(command == "a"){
			makeAssignment(model);
		}else if(command == "s"){
			solve(model);
		}else{
			optionsMenu();
		}
	}
}


int main(int argc, char* argv[]){
	ShortTruthTables::STTModel* model = new ShortTruthTables::STTModel();
	programLoop(model);
}