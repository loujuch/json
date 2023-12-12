#include <iostream>

#include "json.hpp"

#include <assert.h>

int main() {
	json::JBasePtr base;
	json::JParser parser;
	json::JFactory factory;

	// Array
	json::JArrayPtr arr;
	json::JArrayPtr arr1;

	base = parser.parse("[\"one\", 2, 3.0, [\"4\", true, null], -6]");

	std::cout << "start" << std::endl;
	assert(!parser.error());
	arr = factory.getArray(base);
	assert(factory.getString(arr->get(0))->get() == "one");
	assert(factory.getUIntNumber(arr->get(1))->get() == 2);
	assert(factory.getFloatNumber(arr->get(2))->get() == 3.0);
	arr1 = factory.getArray(arr->get(3));
	assert(factory.getString(arr1->get(0))->get() == "4");
	assert(factory.getBool(arr1->get(1))->get());
	assert(factory.getNull(arr1->get(2))->isNull());
	assert(factory.getIntNumber(arr->get(4))->get() == -6);
	std::cout << "end" << std::endl;
	return 0;
}