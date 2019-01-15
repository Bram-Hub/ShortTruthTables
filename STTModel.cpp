#include "STTModel.h"

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

bool ShortTruthTables::STTModel::assignTruthValue(int expNum, int pos_in_string, bool tval){
	if(expNum == -1){
		Expression* exp = this->conclusion->expressionAtPosition(pos_in_string);
		exp->setTruthValue(tval);
	}else{
		Expression* exp = this->getPremise(expNum)->expressionAtPosition(pos_in_string);
		exp->setTruthValue(tval);
	}
}

bool ShortTruthTables::STTModel::can_assign_TVal(int expNum, int pos_in_string, bool tval){
	Expression* exp;
	if(expNum == -1){
		exp = this->conclusion->expressionAtPosition(pos_in_string);
	}else{
		exp = this->getPremise(expNum)->expressionAtPosition(pos_in_string);
	}
	if(exp->getExpressionType() == "AtomicValue"){
		char c = exp->getExpChar()[0];
		std::vector<Expression*> uses = this->constant_uses[c];
		for(int i = 0; i < uses.size(); i++){
			if((tval && uses[i]->isAssignedTrue()) || (!tval && uses[i]->isAssignedFalse())){
				return true;
			}
		}
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
