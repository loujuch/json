#include <iostream>
#include <limits>

#include "json.hpp"

#include <assert.h>

int main() {
	json::JBasePtr base;
	json::JParser parser;
	json::JFactory factory;

	std::string s("{\n\t\"name\": \"loujuch\",\n\t\"age\": {\n\t\t\"year\": 2002,\n\t\t\"month\": 6,\n\t\t\"day\": 22\n\t}\n}");

	base = parser.parse(s);
	std::cout << s << std::endl;

	assert(!parser.error());

	json::JObjectPtr o1, o2;
	o1 = factory.getObject(base);

	assert(factory.getString(o1->get("name"))->get() == "loujuch");
	o2 = factory.getObject(o1->get("age"));

	assert(factory.getUIntNumber(o2->get("year"))->get() == 2002);
	assert(factory.getUIntNumber(o2->get("month"))->get() == 6);
	assert(factory.getUIntNumber(o2->get("day"))->get() == 22);
	return 0;
}