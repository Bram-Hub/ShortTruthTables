#ifndef _ExpressionClasses_h
#define _ExpressionClasses_h

#include "../TruthValues.h"
#include <string>
namespace ShortTruthTables{

class Expression{
protected:
	ShortTruthTables::TruthValue tval;
public:
	ShortTruthTables::Expression* parent;
	virtual std::string getExpChar() = 0;
	bool isUnassigned(){
		return tval == ShortTruthTables::UNASSIGNED;
	}
	bool isAssignedTrue(){
		return tval == ShortTruthTables::ASSIGNED_TRUE;
	}
	bool isAssignedFalse(){
		return tval == ShortTruthTables::ASSIGNED_FALSE;
	}
	TruthValue getTVal(){
		return tval;
	}
	//true if assignment succeeds (either prev unassigned or held same value)
	//false if there's a conflics
	bool setTruthValue(bool val);
	virtual std::string getExpressionType(){
		return "Expression";
	}
	virtual std::string print() = 0;
	virtual bool isValid() = 0;
	virtual Expression* getLeft(){return NULL;}
	virtual Expression* getRight(){return NULL;}
	virtual Expression* getChild(){return NULL;}
	virtual bool canAssignChild(Expression* child, bool tval) = 0;
	virtual bool canAssignSelf(bool tval) = 0;
};

class AtomicValue : public Expression{
private:
	char name;
public:
	AtomicValue(char in_name) : Expression() {
		this->name = in_name;
		tval = ShortTruthTables::UNASSIGNED;
	}
	char getName(){
		return this->name;
	}
	std::string print(){
		return std::string(1, this->name);
	}
	std::string getExpressionType(){
		return "AtomicValue";
	}
	bool isValid(){
		return true;
	}
	std::string getExpChar(){
		return print();
	}
	bool canAssignChild(Expression* child, bool tval){
		return false;
	}
	bool canAssignSelf(bool tval){
		return true;
	}

};

//class does nothing, should never be instantiated
class Operator : public Expression{
public:
	virtual bool isValid() = 0;

};

class UnaryOperator : public Operator{
protected:
	Expression* child;

public:
	Expression* getChild(){return child;}
};

class BinaryOperator : public Operator{
protected:
	Expression* left_child;
	Expression* right_child;
public:
	Expression* getLeft(){return left_child;}
	Expression* getRight(){return right_child;}
	virtual bool isValid() = 0;
};

class NotOperator : public UnaryOperator{
public:
	NotOperator(Expression* n_child) : UnaryOperator(){
		child = n_child;
	}
	bool isValid();
	std::string print(){
		return "~" + child->print();
	}
	std::string getExpressionType(){
		return "NotOperator";
	}
	std::string getExpChar(){
		return "¬";
	}
	bool canAssignChild(Expression* child, bool tval);
	bool canAssignSelf(bool tval);
};

class AndOperator : public BinaryOperator{
public:
	AndOperator(Expression* left_child_in, Expression* right_child_in) : BinaryOperator(){
		this->left_child = left_child_in;
		this->right_child = right_child_in;
	}
	bool isValid();
	std::string print(){
		return "(" + left_child->print() + " & " + right_child->print() + ")";
	}
	std::string getExpressionType(){
		return "AndOperator";
	}
	std::string getExpChar(){
		return "∧";
	}
	bool canAssignChild(Expression* child, bool tval);
	bool canAssignSelf(bool tval);

};

class OrOperator : public BinaryOperator{
public:
	OrOperator(Expression* left_child_in, Expression* right_child_in) : BinaryOperator(){
		this->left_child = left_child_in;
		this->right_child = right_child_in;
	}
	bool isValid();
	std::string print(){
		return "(" + left_child->print() + " | " + right_child->print() + ")";
	}
	std::string getExpressionType(){
		return "OrOperator";
	}
	std::string getExpChar(){
		return "∨";
	}
	bool canAssignChild(Expression* child, bool tval);
	bool canAssignSelf(bool tval);
};

class ConditionalOperator : public BinaryOperator{
public: 
	ConditionalOperator(Expression* left_child_in, Expression* right_child_in) : BinaryOperator(){
		this->left_child = left_child_in;
		this->right_child = right_child_in;
	}
	bool isValid();
	std::string print(){
		return "(" + left_child->print() + " -> " + right_child->print() + ")";
	}
	std::string getExpressionType(){
		return "ConditionalOperator";
	}
	std::string getExpChar(){
		return "→";
	}
	bool canAssignChild(Expression* child, bool tval);
	bool canAssignSelf(bool tval);

};

class BiConditionalOperator : public BinaryOperator{
public:
	BiConditionalOperator(Expression* left_child_in, Expression* right_child_in) : BinaryOperator(){
		this->left_child = left_child_in;
		this->right_child = right_child_in;
	}
	bool isValid();
	std::string print(){
		return "(" + left_child->print() + " <-> " + right_child->print() + ")";
	}
	std::string getExpressionType(){
		return "BiConditionalOperator";
	}
	std::string getExpChar(){
		return "↔";
	}
	bool canAssignChild(Expression* child, bool tval);
	bool canAssignSelf(bool tval);

};

}
#endif