#include "Parser.h"
#include "ExpressionClasses.h"
#include <ctype.h>
#include <cassert>
#include <iostream>
#include <stdexcept>

class Expression;
//(OP (OP CONST) CONST)
//format:
//input (OP (OP CONST CONST) CONST) || CONST
ShortTruthTables::Expression* ShortTruthTables::ParsedExpression::recursiveParser(std::string::iterator &curr_pos, const std::string::iterator &end){
	if(*curr_pos == '('){
		std::string::iterator temp = curr_pos;
		temp++;
		std::string next_word(temp, temp+3);
		ShortTruthTables::Expression* resultOperation;
		if(next_word == "not"){
			curr_pos += 5;
			ShortTruthTables::Expression* child = recursiveParser(curr_pos, end);
			resultOperation = new ShortTruthTables::NotOperator(child);
			child->parent = resultOperation;
			if(*curr_pos != ')')  throw std::invalid_argument( "poorly formed expression" );
			curr_pos++;
			if(*curr_pos == ' '){
				curr_pos++;
			}
			return resultOperation;
		}else if(next_word == "or "){
			curr_pos += 4;
			ShortTruthTables::Expression* left = recursiveParser(curr_pos, end);
			ShortTruthTables::Expression* right = recursiveParser(curr_pos, end);
			resultOperation = new ShortTruthTables::OrOperator(left,right);
			left->parent = resultOperation;
			right->parent = resultOperation;
			if(*curr_pos != ')')  throw std::invalid_argument( "poorly formed expression" );
			curr_pos++;
			if(*curr_pos == ' '){
				curr_pos++;
			}
			return resultOperation;
		}else if(next_word == "and"){
			curr_pos += 5;
			ShortTruthTables::Expression* left = recursiveParser(curr_pos, end);
			ShortTruthTables::Expression* right = recursiveParser(curr_pos, end);
			resultOperation = new ShortTruthTables::AndOperator(left,right);
			left->parent = resultOperation;
			right->parent = resultOperation;
			if(*curr_pos != ')')  throw std::invalid_argument( "poorly formed expression" );
			curr_pos++;
			if(*curr_pos == ' '){
				curr_pos++;
			}
			return resultOperation;
		}else if(next_word == "if "){
			curr_pos += 4;
			ShortTruthTables::Expression* left = recursiveParser(curr_pos, end);
			ShortTruthTables::Expression* right = recursiveParser(curr_pos, end);
			resultOperation = new ShortTruthTables::ConditionalOperator(left,right);
			left->parent = resultOperation;
			right->parent = resultOperation;
			if(*curr_pos != ')')  throw std::invalid_argument( "poorly formed expression" );
			curr_pos++;
			if(*curr_pos == ' '){
				curr_pos++;
			}
			return resultOperation;
		}else if(next_word == "iff"){
			curr_pos += 5;
			ShortTruthTables::Expression* left = recursiveParser(curr_pos, end);
			ShortTruthTables::Expression* right = recursiveParser(curr_pos, end);
			resultOperation = new ShortTruthTables::BiConditionalOperator(left,right);
			left->parent = resultOperation;
			right->parent = resultOperation;
			if(*curr_pos != ')')  throw std::invalid_argument( "poorly formed expression" );
			curr_pos++;
			if(*curr_pos == ' '){
				curr_pos++;
			}
			return resultOperation;
		}else{
			throw std::invalid_argument( "poorly formed expression" );			
		}

	}else if(isalpha(*curr_pos) && isupper(*curr_pos)){
		ShortTruthTables::AtomicValue* constant = new ShortTruthTables::AtomicValue(*curr_pos);
		curr_pos++;
		constant_uses[constant->getName()].push_back(constant);
		if(curr_pos == end){
			return constant;
		}else if(*curr_pos == ')'){
			return constant;
		}else if(*curr_pos == ' '){
			curr_pos++;
			return constant;
		}else{
			throw std::invalid_argument( "poorly formed expression" );
		}
	}else{
		throw std::invalid_argument( "poorly formed expression" );
	}
}


ShortTruthTables::Expression* ShortTruthTables::ParsedExpression::parser(std::string& input){
	std::string::iterator temp = input.begin();
	recursiveParser(temp, input.end());
}

void getExpInOrder(ShortTruthTables::Expression* exp, std::vector<ShortTruthTables::Expression*>& inOrder){
	std::string type = exp->getExpressionType();
	if(type == "ConditionalOperator" || type == "BiConditionalOperator" || type == "OrOperator" || type == "AndOperator"){
		getExpInOrder(exp->getLeft(), inOrder);
		inOrder.push_back(exp);
		getExpInOrder(exp->getRight(), inOrder);
	}else if(type == "NotOperator"){
		inOrder.push_back(exp);
		getExpInOrder(exp->getChild(), inOrder);
	}else{
		inOrder.push_back(exp);
	}
}

void ShortTruthTables::ParsedExpression::left_to_right_map(){
	inOrder.clear();
	getExpInOrder(this->topLevelExpression, inOrder);
	int j = 0;
	for(int i = 0; i < this->inOrderExp.length(); i++){
		// std::cout << inOrder[j]->print() << " " << inOrderExp[i] << std::endl;
		// std::cout << i << " " << inOrderExp.length() << std::endl;
		if(j >= inOrder.size()) break;
		if(inOrderExp[i] == '(' || inOrderExp[i] == ')' ||inOrderExp[i] == ' '){
			continue;
		}
		if(isalpha(inOrderExp[i])){
			if(inOrder[j]->getExpressionType() == "AtomicValue"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}else if(inOrderExp[i] == '|'){
			if(inOrder[j]->getExpressionType() == "OrOperator"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}else if(inOrderExp[i] == '&'){
			if(inOrder[j]->getExpressionType() == "AndOperator"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}else if(inOrderExp[i] == '-' && inOrderExp[i-1] != '<'){
			if(inOrder[j]->getExpressionType() == "ConditionalOperator"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}else if(inOrderExp[i] == '<'){
			if(inOrder[j]->getExpressionType() == "BiConditionalOperator"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}else if(inOrderExp[i] == '~'){
			if(inOrder[j]->getExpressionType() == "NotOperator"){
				this->stringPlace[i] = inOrder[j];
				j++;
			}else{
				assert(false);
			}
		}
	}
}

ShortTruthTables::ParsedExpression::ParsedExpression(std::string exp){
	this->preOrder = exp;
	this->topLevelExpression = this->parser(exp);
	this->inOrderExp = topLevelExpression->print();
	this->left_to_right_map();
}

