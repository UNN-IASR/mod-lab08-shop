#include "Customer.h"

Customer::Customer(int id, unsigned int itemCount)
	: _id(id), _itemCount(itemCount) {}

int Customer::getId() const
{
	return _id;
}

unsigned int Customer::getItemCount() const
{
	return _itemCount;
}
