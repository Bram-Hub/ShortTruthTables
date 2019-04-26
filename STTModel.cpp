#include "STTModel.h"

/*
Overarching file for the ShortTruthTables model
*/

/*
Add a premise to the model
Takes in a string, then parses that into an expression
sets the value of the premise expression to true
updates the constants that are in the model and where they appear
*/
void ShortTruthTables::STTModel::addPremise(std::string preorder){
	ParsedExpression* p = new ParsedExpression(preorder);
	p->setTruthValue(true);
	this->premises.push_back(p);
	std::vector<char> temp_constants = p->constants();
	for(int i = 0; i < temp_constants.size(); i++){
		char temp = temp_constants[i];
		std::vector<Expression*> expvec = p->uses_of_char(temp);
		for(int j = 0; j < expvec.size(); j++){
			constant_uses[temp].push_back(expvec[j]);
		}
	}
}

/*
Adds conclusion to the model
Makes sure there is not already a conclusion
Takes in a string and parses it into an expression
Assigns this expression to be false
Adds any new constants to constant_uses and updates that any existing constants are being used here
*/
bool ShortTruthTables::STTModel::addConclusion(std::string preorder){
	bool retval;
	if(this->conclusion == NULL){
		retval = false;
	}else{
		retval = true;
	}
	ParsedExpression* p = new ParsedExpression(preorder);
	p->setTruthValue(false);
	this->conclusion = p;
	std::vector<char> temp_constants = p->constants();
	for(int i = 0; i < temp_constants.size(); i++){
		char temp = temp_constants[i];
		std::vector<Expression*> expvec = p->uses_of_char(temp);
		for(int j = 0; j < expvec.size(); j++){
			constant_uses[temp].push_back(expvec[j]);
		}
	}
	return retval;
}

//Assigns a truth value to an expression
bool ShortTruthTables::STTModel::assignTruthValue(int expNum, int pos_in_string, bool tval){
	if(expNum == -1){
		Expression* exp = this->conclusion->expressionAtPosition(pos_in_string);
		exp->setTruthValue(tval);
	}else{
		Expression* exp = this->getPremise(expNum)->expressionAtPosition(pos_in_string);
		exp->setTruthValue(tval);
	}
}

//check if the truth value can be assigned to an atomic value based on that values previous assignments
bool ShortTruthTables::STTModel::can_assign_TVal_var(int expNum, int pos_in_string, bool tval){
	Expression* exp;
	if(expNum == -1){
		exp = this->conclusion->expressionAtPosition(pos_in_string);
	}else{
		exp = this->getPremise(expNum)->expressionAtPosition(pos_in_string);
	}
	if(exp->getExpressionType() == "AtomicValue"){ //if trying to assign a truth value to a variable
		char c = exp->getExpChar()[0]; //access which variable it is
		std::vector<Expression*> uses = this->constant_uses[c]; //get a vector of all the times this constant is used
		for(int i = 0; i < uses.size(); i++){
			if((tval && uses[i]->isAssignedTrue()) || (!tval && uses[i]->isAssignedFalse())){
				//if what you are hoping to assign matches how that variable has been used somewhere already, it can be assigned that way
				return true;
			}
		}
	}
	return false;
}

//check if the truth value can be assigned based off of the rest of the expression it is in
bool ShortTruthTables::STTModel::can_assign_TVal_expr(int expNum, int pos_in_string, bool tval){
	Expression* exp;
	if(expNum == -1){
		exp = this->conclusion->expressionAtPosition(pos_in_string);
	}else{
		exp = this->getPremise(expNum)->expressionAtPosition(pos_in_string);
	}
	if(exp->getExpressionType() == "AtomicValue"){
		if(exp->parent != NULL && exp->parent->canAssignChild(exp, tval)){
			return true;
		}
		return false;
	}else{
		return (exp->parent != NULL && exp->parent->canAssignChild(exp, tval)) || exp->canAssignSelf(tval);
	}
}


ShortTruthTables::ParsedExpression* ShortTruthTables::STTModel::getPremise(int i){
	if(i >= premises.size()) return NULL;
	return this->premises[i];
}
ShortTruthTables::ParsedExpression* ShortTruthTables::STTModel::getConclusion(){
	return this->conclusion;
}

ShortTruthTables::TruthValue ShortTruthTables::STTModel::valueOfExp(int expNum, int pos_in_string){
	ParsedExpression* e = this->getPremise(expNum);
	return e->expressionAtPosition(pos_in_string)->getTVal();
}

ShortTruthTables::STTModel::STTModel(){
	this->conclusion = NULL;
}
