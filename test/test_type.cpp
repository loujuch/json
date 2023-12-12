#include <iostream>
#include <limits>

#include "json.hpp"

#include <assert.h>

int main() {
	json::JBasePtr base;
	json::JParser parser;
	json::JFactory factory;

	std::numeric_limits<double>().max;

	char buffer[129] = "\0";
	std::string s;
	// String
	json::JStringPtr str;
	// int
	json::JIntNumberPtr inum;
	// uint
	json::JUIntNumberPtr unum;
	// float
	json::JFloatNumberPtr fnum;
	// bool
	json::JBoolPtr b;
	// null
	json::JNullPtr n;

	// String
	base = parser.parse("\"0123456789\"");
	assert(!parser.error());
	assert(base->isString());
	str = factory.getString(base);
	assert(str->get() == "0123456789");
	assert(str->toString() == "\"0123456789\"");

	base = parser.parse("\"\"");
	assert(!parser.error());
	assert(base->isString());
	str = factory.getString(base);
	assert(str->get() == "");
	assert(str->toString() == "\"\"");

	// IntNumber
	base = parser.parse("-0");
	assert(!parser.error());
	assert(base->isIntNumber());
	inum = factory.getIntNumber(base);
	assert(inum->get() == 0);
	assert(inum->toString() == "0");

	base = parser.parse("-12345");
	assert(!parser.error());
	assert(base->isIntNumber());
	inum = factory.getIntNumber(base);
	assert(inum->get() == -12345);
	assert(inum->toString() == "-12345");

	int64_t int_min = std::numeric_limits<int64_t>().min();
	s = std::to_string(int_min);

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isIntNumber());
	inum = factory.getIntNumber(base);
	assert(inum->get() == int_min);
	assert(inum->toString() == s);

	s.back() -= 1;
	int_min += 1;

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isIntNumber());
	inum = factory.getIntNumber(base);
	assert(inum->get() == int_min);
	assert(inum->toString() == s);

	s.back() += 2;
	int_min -= 1;

	base = parser.parse(s);
	assert(!parser.error());
	assert(!base->isIntNumber());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == ((double)int_min) - 1);

	// UIntNumber
	base = parser.parse("0");
	assert(!parser.error());
	assert(base->isUIntNumber());
	unum = factory.getUIntNumber(base);
	assert(unum->get() == 0);
	assert(unum->toString() == "0");

	base = parser.parse("12345");
	assert(!parser.error());
	assert(base->isUIntNumber());
	unum = factory.getUIntNumber(base);
	assert(unum->get() == 12345);
	assert(unum->toString() == "12345");

	uint64_t uint_max = std::numeric_limits<uint64_t>().max();
	s = std::to_string(uint_max);

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isUIntNumber());
	unum = factory.getUIntNumber(base);
	assert(unum->get() == uint_max);
	assert(unum->toString() == s);

	s.back() -= 1;
	uint_max -= 1;

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isUIntNumber());
	unum = factory.getUIntNumber(base);
	assert(unum->get() == uint_max);
	assert(unum->toString() == s);

	s.back() += 2;
	uint_max += 1;

	base = parser.parse(s);
	assert(!parser.error());
	assert(!base->isUIntNumber());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == ((double)uint_max) + 1);

	// FloatNumber
	base = parser.parse("0.0");
	assert(!parser.error());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == 0.0);

	base = parser.parse("123.456");
	assert(!parser.error());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == 123.456);

	double double_max = std::numeric_limits<double>().max();
	double double_min = std::numeric_limits<double>().min();

	s = std::to_string(double_max);

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == double_max);

	snprintf(buffer, sizeof(buffer), "%g", double_min);
	s = std::string(buffer);

	base = parser.parse(s);
	assert(!parser.error());
	assert(base->isFloatNumber());
	fnum = factory.getFloatNumber(base);
	assert(fnum->get() == 2.22507e-308);

	// 布尔
	base = parser.parse("true");
	assert(!parser.error());
	assert(base->isBool());
	b = factory.getBool(base);
	assert(b->get());

	base = parser.parse("false");
	assert(!parser.error());
	assert(base->isBool());
	b = factory.getBool(base);
	assert(!b->get());

	// 空类型
	base = parser.parse("null");
	assert(!parser.error());
	assert(base->isNull());

	// 错误
	base = parser.parse("12nau");
	assert(parser.error());
	base = parser.parse("!aaa");
	assert(parser.error());
	base = parser.parse("\"aaa\\\"");
	assert(parser.error());
	return 0;
}