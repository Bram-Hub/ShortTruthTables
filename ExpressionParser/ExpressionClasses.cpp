#include "ExpressionClasses.h"
#include <iostream>
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

bool ShortTruthTables::NotOperator::isValid(){
	if(this->isUnassigned() || child->isUnassigned()) return true;
	if(this->isAssignedTrue()) return child->isAssignedFalse();
	else return child->isAssignedTrue();
}

bool ShortTruthTables::AndOperator::isValid(){
	if(left_child->isUnassigned() && right_child->isUnassigned()) return true;
	if(this->isUnassigned()) return true;
	if(this->isAssignedTrue() && (left_child->isAssignedTrue() && right_child->isAssignedTrue())) return true;
	if(this->isAssignedTrue() && left_child->isUnassigned() && right_child->isAssignedTrue()) return true;
	if(this->isAssignedTrue() && right_child->isUnassigned() && left_child->isAssignedTrue()) return true;
	if(this->isAssignedFalse() && (left_child->isAssignedFalse() || right_child->isAssignedFalse())) return true;

	return false;
}

bool ShortTruthTables::OrOperator::isValid(){
	if(this->isUnassigned()) return true;

	if(left_child->isUnassigned() || right_child->isUnassigned()) return true;

	if(this->isAssignedTrue() && (left_child->isAssignedTrue() || right_child->isAssignedTrue())) return true;
	if(this->isAssignedFalse() && (left_child->isAssignedFalse() && right_child->isAssignedFalse())) return true;

	return false;
}


bool ShortTruthTables::ConditionalOperator::isValid(){
	if(this->isUnassigned()|| left_child->isUnassigned() || right_child->isUnassigned()) return true;

	if(this->isAssignedTrue()){
		if(left_child->isAssignedTrue()){
			return right_child->isAssignedTrue();

		}else{ //assigned false
			return true;
		}
	}else{ // assigned false
		return left_child->isAssignedTrue() && right_child->isAssignedFalse();
	}
}

bool ShortTruthTables::BiConditionalOperator::isValid(){
	if(this->isUnassigned()|| left_child->isUnassigned() || right_child->isUnassigned()) return true;

	if(this->isAssignedTrue()){
		if(left_child->isAssignedTrue()) return right_child->isAssignedTrue();
		if(left_child->isAssignedFalse()) return right_child->isAssignedFalse();
	}

	if(this->isAssignedFalse()){
		if(left_child->isAssignedTrue()) return right_child->isAssignedFalse();
		if(left_child->isAssignedFalse()) return right_child->isAssignedTrue();
	}
}

bool ShortTruthTables::NotOperator::canAssignChild(Expression* child, bool tval){
	return (tval && this->isAssignedFalse()) || (!tval && this->isAssignedTrue());
}

bool ShortTruthTables::AndOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child) other_child = left_child;
	else other_child = right_child;
	if(this->isAssignedTrue()){
		return tval;
	}else if(this->isAssignedFalse()){
		return !tval && other_child->isAssignedFalse();
	}else{
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
		return tval || other_child->isAssignedTrue();
	}else if(this->isAssignedFalse()){
		return !tval;
	}else{
		return false;
	}

}


bool ShortTruthTables::ConditionalOperator::canAssignChild(Expression* child, bool tval){
	Expression* other_child;
	if(child == right_child){
		other_child = left_child;
		if(this->isAssignedTrue()){
			return tval && other_child->isAssignedTrue();
		}else if(this->isAssignedFalse()){
			return !tval && other_child->isAssignedTrue();
		}else{
			return false;
		}

	}else{
		other_child = right_child;
		if(this->isAssignedTrue()){
			return (!tval && other_child->isAssignedFalse());
		}else if(this->isAssignedFalse()){
			return false;
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
		return (tval && other_child->isAssignedTrue()) || (!tval && other_child->isAssignedFalse());
	}else if(this->isAssignedFalse()){
		return (!tval && other_child->isAssignedTrue()) || (tval && other_child->isAssignedFalse());
	}else{
		return false;
	}
}

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
		return left_child->isAssignedFalse() || (left_child-> isAssignedTrue() && right_child->isAssignedTrue());
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