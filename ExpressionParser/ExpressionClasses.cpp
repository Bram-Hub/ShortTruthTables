#include "ExpressionClasses.h"
#include <iostream>


/*
This file has the classes for all different types of Expressions
Class structure is in the .h file, and this file has all of the different functionality for the different types of expressions.
it has isValid(), canAssignChild(), and canAssignSelf() for each of the different expression types, as well as setTruthValue()
*/

//Assign the truth value to the expression.  Is only called after checking if this is valid
bool ShortTruthTables::Expression::setTruthValue(bool val){
	if(val){
		tval = ShortTruthTables::ASSIGNED_TRUE;
	}else if(!val){
		tval = ShortTruthTables::ASSIGNED_FALSE;
	}else{
		return false;
	}
	return true;
}


/*
The following section is isValid() functions for the many different operator expressions
These check if the assignments given to the expression are valid; 
i.e. it checks if there is a contradiction in its assigned values.
*/


bool ShortTruthTables::NotOperator::isValid(){
//if nothing is assigned it is valid
	if(this->isUnassigned() || child->isUnassigned()) return true;
//if the statement is assigned true then the child needs to be assigned false
	if(this->isAssignedTrue()) return child->isAssignedFalse();
//else: the statement is assigned false so the child needs to be assigned true
	else return child->isAssignedTrue();
}

//check for contradictions within an and expression
bool ShortTruthTables::AndOperator::isValid(){
//if neither child is assigned it will be valid
	if(left_child->isUnassigned() && right_child->isUnassigned()) return true;
//if the expression is not assigned it is valid
	if(this->isUnassigned()) return true;
//if the expression is assigned true, there must be either both children true, or one child true and the other unassigned
	if(this->isAssignedTrue() && (left_child->isAssignedTrue() && right_child->isAssignedTrue())) return true;
	if(this->isAssignedTrue() && left_child->isUnassigned() && right_child->isAssignedTrue()) return true;
	if(this->isAssignedTrue() && right_child->isUnassigned() && left_child->isAssignedTrue()) return true;
//if it is assigned False and at least one of the two children is false that is valid
	if(this->isAssignedFalse() && (left_child->isAssignedFalse() || right_child->isAssignedFalse())) return true;
//any arrangement that does not fit into one of the above buckets is not valid
	return false;
}

bool ShortTruthTables::OrOperator::isValid(){
//if the expression is not assigned, is valid
	if(this->isUnassigned()) return true;
//if the expression is assigned and at least one child is not assigned it is valid
	if(left_child->isUnassigned() || right_child->isUnassigned()) return true;
//at this point it is guaranteed that the expression and the two children all have assignments
//if the expression is true and at least one of the children is true that's valid
	if(this->isAssignedTrue() && (left_child->isAssignedTrue() || right_child->isAssignedTrue())) return true;
//if the expression is false and both children are false it is valid
	if(this->isAssignedFalse() && (left_child->isAssignedFalse() && right_child->isAssignedFalse())) return true;
//any other arrangement is a contradiction
	return false;
}


bool ShortTruthTables::ConditionalOperator::isValid(){
//if anything is left unassigned it is valid
	if(this->isUnassigned()|| left_child->isUnassigned() || right_child->isUnassigned()) return true;

	if(this->isAssignedTrue()){
	//if the expression is true
		if(left_child->isAssignedTrue()){
		//make sure T on the left is followed by T on the right, else there is a contradiction
			return right_child->isAssignedTrue();

		}else{ //has False as the first part, so will be valid no matter what
			return true;
		}
	}else{ // the conditional is assigned false, so the left must be true and the right must be false.
		return left_child->isAssignedTrue() && right_child->isAssignedFalse();
	}
}

bool ShortTruthTables::BiConditionalOperator::isValid(){
//if anything is unassigned there is no contradiction
	if(this->isUnassigned()|| left_child->isUnassigned() || right_child->isUnassigned()) return true;

	if(this->isAssignedTrue()){
	//if the biconditional is assigned true, make sure the children match or else there is a contradiction
		if(left_child->isAssignedTrue()) return right_child->isAssignedTrue();
		if(left_child->isAssignedFalse()) return right_child->isAssignedFalse();
	}

	if(this->isAssignedFalse()){ 
	//if the biconditional is false, make sure the children do not match, or else there is a contradiction 
		if(left_child->isAssignedTrue()) return right_child->isAssignedFalse();
		if(left_child->isAssignedFalse()) return right_child->isAssignedTrue();
	}
}


/*
The following section checks whether the child in an expression can be assigned that value.
Makes sure that the value is forced by the rest of the expression
*/


bool ShortTruthTables::NotOperator::canAssignChild(Expression* child, bool tval){
//in order to assign a value to K in (not K), either the expression is false and K should be true or the expression is true and K should be false
	return (tval && this->isAssignedFalse()) || (!tval && this->isAssignedTrue());
}

bool ShortTruthTables::AndOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child) other_child = left_child;
	else other_child = right_child;
	//if the and operation is true, must be trying to assign true to the child as well
	if(this->isAssignedTrue()){
		return tval;
	}else if(this->isAssignedFalse()){
	//if the and is false, then only able to assign to child when other child is assigned True and you are trying to assign False.
		return !tval && other_child->isAssignedTrue();
	}else{
	//anything else cannot be infered and cannot be assigned
		return false;
	}
}

bool ShortTruthTables::OrOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child){
		other_child = left_child;
	}else{
		other_child = right_child;
	}
	if(this->isAssignedTrue()){
//if the expression is true, can only assign when other child is false and you are trying to make this child true
		return tval && other_child->isAssignedFalse();
	}else if(this->isAssignedFalse()){ //if the or is false, can assign false to the child
		return !tval;
	}else{
		return false;
	}

}



bool ShortTruthTables::ConditionalOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child){
		other_child = left_child;
		if(this->isAssignedTrue()){//if the conditional is true, then if the left is true the right must also be true
			return tval && other_child->isAssignedTrue();
		}else if(this->isAssignedFalse()){
//if the conditional is false, then the right child must be assigned to False
			return !tval; 
		}else{
			return false;
		}

	}else{
		other_child = right_child;
		if(this->isAssignedTrue()){
//conditional is true, then if the right is false the left must be false
			return (!tval && other_child->isAssignedFalse());
		}else if(this->isAssignedFalse()){
//conditional is false, so left value must be true
			return tval;
		}else{
			return false;
		}
	}
}

bool ShortTruthTables::BiConditionalOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child) other_child = left_child;
	else other_child = right_child;
	if(this->isAssignedTrue()){
	//if the biconditional is true, make sure the child matches the other child
		return (tval && other_child->isAssignedTrue()) || (!tval && other_child->isAssignedFalse());
	}else if(this->isAssignedFalse()){
	//if the biconditional is false, make sure the child opposes the other child
		return (!tval && other_child->isAssignedTrue()) || (tval && other_child->isAssignedFalse());
	}else{
		return false;
	}
}


/*
The following section looks at when the children have been assigned and one is hoping to assign the expression itself

*/

bool ShortTruthTables::NotOperator::canAssignSelf(bool tval){
	return (child->isAssignedFalse() && tval) || (child->isAssignedTrue() && !tval);
}

bool ShortTruthTables::OrOperator::canAssignSelf(bool tval){
	if(tval){
		return left_child->isAssignedTrue() || right_child->isAssignedTrue();
	}else{
		return left_child->isAssignedFalse() && right_child->isAssignedFalse();
	}
}

bool ShortTruthTables::AndOperator::canAssignSelf(bool tval){
	if(tval){
		return left_child->isAssignedTrue() && right_child->isAssignedTrue();
	}else{
		return left_child->isAssignedFalse() || right_child->isAssignedFalse();
	}
}

bool ShortTruthTables::ConditionalOperator::canAssignSelf(bool tval){
	if(tval){
		return left_child->isAssignedFalse() || right_child->isAssignedTrue();
	}else{
		return left_child->isAssignedTrue() && right_child->isAssignedFalse();
	}
}

bool ShortTruthTables::BiConditionalOperator::canAssignSelf(bool tval){
	if(tval){
		return (left_child->isAssignedFalse() && right_child->isAssignedFalse()) || (left_child-> isAssignedTrue() && right_child->isAssignedTrue());
	}else{
		return (left_child->isAssignedFalse() && right_child->isAssignedTrue()) || (left_child-> isAssignedTrue() && right_child->isAssignedFalse());
	}
}
