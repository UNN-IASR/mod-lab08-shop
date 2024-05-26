#pragma once

class Customer
{
private:
	int _id;
	unsigned int _itemCount;

public:
	Customer(int id, unsigned int itemCount);

	[[nodiscard]] int getId() const;
	[[nodiscard]] unsigned int getItemCount() const;
};
