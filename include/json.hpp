#ifndef _JSON_HPP__
#define _JSON_HPP__

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace json {

///////////////////////////////// 对象类型 /////////////////////////////////

// JSON 对象类型
enum class JType :unsigned char {
	JOBJECT = 0,
	JARRAY,
	JSTRING,
	JINTNUMBER,
	JUINTNUMBER,
	JFLOATNUMBER,
	JBOOL,
	JNULL
}; // class JsonType

class _Base_;
class _Object_;
class _Array_;
class _String_;
class _IntNumber_;
class _UIntNumber_;
class _FloatNumber_;
class _Bool_;
class _Null_;

///////////////////////////////// 暴露类型 /////////////////////////////////

// 基本类型
using JBasePtr = std::shared_ptr<_Base_>;
// 对象类型
using JObjectPtr = std::shared_ptr<_Object_>;
// 列表类型
using JArrayPtr = std::shared_ptr<_Array_>;
// 字符串类型
using JStringPtr = std::shared_ptr<_String_>;
// 数值类型，当数值为负数且能使用int64_t表示
using JIntNumberPtr = std::shared_ptr<_IntNumber_>;
// 数值类型，当数值为正数且能使用uint64_t表示
using JUIntNumberPtr = std::shared_ptr<_UIntNumber_>;
// 数值类型，小数或不能使用int64_t和uint64_t表示的整数，若过大为INF
using JFloatNumberPtr = std::shared_ptr<_FloatNumber_>;
// 布尔类型
using JBoolPtr = std::shared_ptr<_Bool_>;
// 空类型
using JNullPtr = std::shared_ptr<_Null_>;

class JFactory;

///////////////////////////////// 实际类型 /////////////////////////////////

// JSON 基类型
class _Base_ {
public:
	friend class JFactory;
private:
	const JType m_type_;
protected:
	inline _Base_(JType type) :m_type_(type) {
	}
public:
	inline virtual ~_Base_() = default;

	inline JType type() const {
		return m_type_;
	};

	inline bool isObject() const {
		return type() == JType::JOBJECT;
	}

	inline bool isArray() const {
		return type() == JType::JARRAY;
	}

	inline bool isString() const {
		return type() == JType::JSTRING;
	}

	inline bool isIntNumber() const {
		return type() == JType::JINTNUMBER;
	}

	inline bool isUIntNumber() const {
		return type() == JType::JUINTNUMBER;
	}

	inline bool isFloatNumber() const {
		return type() == JType::JFLOATNUMBER;
	}

	inline bool isBool() const {
		return type() == JType::JBOOL;
	}

	inline bool isNull() const {
		return type() == JType::JNULL;
	}

	inline virtual std::string toString() const = 0;
private:
	_Base_() = delete;
	_Base_(const _Base_ &) = delete;
	_Base_(_Base_ &&) = delete;
	_Base_ &operator=(const _Base_ &) = delete;
	_Base_ &operator=(_Base_ &&) = delete;
}; // class JsonBase

// JSON 对象
class _Object_ :public _Base_ {
public:
	friend class JFactory;
public:
	using key_type = std::string;
	using value_type = JBasePtr;
private:
	std::unordered_map<key_type, value_type> m_elem_;
private:
	inline _Object_() :_Base_(JType::JOBJECT) {
	}
public:
	inline bool set(const key_type &key, const value_type &value) {
		auto p = m_elem_.emplace(key, value);
		return p.second;
	}

	inline bool set(key_type &&key, const value_type &value) {
		auto p = m_elem_.emplace(std::move(key), value);
		return p.second;
	}

	inline value_type get(const key_type &s) const {
		auto p = m_elem_.find(s);
		if(p == m_elem_.end()) {
			return value_type();
		}
		return p->second;
	}

	inline bool contain(const key_type &s) const {
		return m_elem_.count(s) > 0;
	}

	inline size_t size() const {
		return m_elem_.size();
	}

	inline std::string toString() const {
		std::string res;
		res.push_back('{');
		for(auto &p : m_elem_) {
			res.push_back('"');
			res.append(p.first);
			res.push_back('"');
			res.push_back(':');
			res.append(p.second->toString());
			res.push_back(',');
		}
		if(res.back() == ',') {
			res.pop_back();
		}
		res.push_back('}');
		return res;
	}
}; // class _Object_

// JSON 数组
class _Array_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = JBasePtr;
private:
	std::vector<value_type> m_elem_;
private:
	inline _Array_() :_Base_(JType::JARRAY) {
		m_elem_.reserve(100);
	}
public:
	inline void set(const value_type &ptr) {
		m_elem_.emplace_back(ptr);
	}

	inline value_type get(size_t i) const {
		return m_elem_.at(i);
	}

	inline size_t size() const {
		return m_elem_.size();
	}

	inline std::string toString() const {
		std::string res;
		res.push_back('[');
		for(size_t i = 0;i < size();++i) {
			if(i) {
				res.push_back(',');
			}
			res.append(m_elem_.at(i)->toString());
		}
		res.push_back(']');
		return res;
	}
}; // class _Array_

// JSON 字符串
class _String_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = std::string;
private:
	value_type m_elem_;
private:
	inline _String_() :_Base_(JType::JSTRING) {
	}
public:
	inline void set(const value_type &s) {
		m_elem_ = s;
	}

	inline void set(value_type &&s) {
		m_elem_ = std::move(s);
	}

	inline const value_type &get() const {
		return m_elem_;
	}

	inline std::string toString() const {
		return '"' + m_elem_ + '"';
	}
}; // class _String_

// JSON 整数
class _IntNumber_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = int64_t;
private:
	value_type m_number_;
private:
	inline _IntNumber_() :_Base_(JType::JINTNUMBER) {
	}
public:
	inline void set(value_type fn) {
		m_number_ = fn;
	}

	inline value_type get() const {
		return m_number_;
	}

	inline std::string toString() const {
		return std::to_string(m_number_);
	}
}; // class _IntNumber_

// JSON 无符号整数
class _UIntNumber_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = uint64_t;
private:
	value_type m_number_;
private:
	inline _UIntNumber_() :_Base_(JType::JUINTNUMBER) {
	}
public:
	inline void set(value_type fn) {
		m_number_ = fn;
	}

	inline value_type get() const {
		return m_number_;
	}

	inline std::string toString() const {
		return std::to_string(m_number_);
	}
}; // class _UIntNumber_

// // JSON 浮点
class _FloatNumber_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = double;
private:
	value_type m_number_;
private:
	inline _FloatNumber_() :_Base_(JType::JFLOATNUMBER) {
	}
public:
	inline void set(value_type fn) {
		m_number_ = fn;
	}

	inline value_type get() const {
		return m_number_;
	}

	inline std::string toString() const {
		char buffer[129] = "\0";
		snprintf(buffer, sizeof(buffer), "%g", m_number_);
		return std::string(buffer);
	}
}; // class _FloatNumber_

// JSON 布尔
class _Bool_ :public _Base_ {
public:
	friend class JFactory;
public:
	using value_type = bool;
private:
	value_type m_value_;
private:
	inline _Bool_() :_Base_(JType::JBOOL) {
	}
public:
	inline void set(value_type b) {
		m_value_ = b;
	}

	inline value_type get() const {
		return m_value_;
	}

	inline std::string toString() const {
		return m_value_ ? "true" : "false";
	}
}; // class _Bool_

// JSON 空类型
class _Null_ :public _Base_ {
public:
	friend class JFactory;
private:
	inline _Null_() :_Base_(JType::JNULL) {
	}
public:
	inline std::string toString() const {
		return "null";
	}
}; // class _Null_

///////////////////////////////// 类型字符串 /////////////////////////////////

class _JsonTypeString_ {
public:
	inline static const std::string &getTypeString(const JBasePtr &ptr) {
		if(ptr.get() == nullptr) {
			return getNullptr();
		}
		switch(ptr->type()) {
		case JType::JOBJECT:return getJsonTypeObjectString();
		case JType::JARRAY:return getJsonTypeArrayString();
		case JType::JSTRING:return getJsonTypeStringString();
		case JType::JINTNUMBER:return getJsonTypeIntNumberString();
		case JType::JUINTNUMBER:return getJsonTypeUIntNumberString();
		case JType::JFLOATNUMBER:return getJsonTypeFloatNumberString();
		case JType::JBOOL:return getJsonTypeBoolString();
		case JType::JNULL:return getJsonTypeNullString();
		}
		return getNullptr();
	}

	inline static const std::string &getNullptr() {
		static const std::string str("JsonInvaild");
		return str;
	}

	inline static const std::string &getJsonTypeObjectString() {
		static const std::string str("JsonObject");
		return str;
	}

	inline static const std::string &getJsonTypeArrayString() {
		static const std::string str("JsonArray");
		return str;
	}

	inline static const std::string &getJsonTypeStringString() {
		static const std::string str("JsonString");
		return str;
	}

	inline static const std::string &getJsonTypeIntNumberString() {
		static const std::string str("JsonIntNumber");
		return str;
	}

	inline static const std::string &getJsonTypeUIntNumberString() {
		static const std::string str("JsonUIntNumber");
		return str;
	}

	inline static const std::string &getJsonTypeFloatNumberString() {
		static const std::string str("JsonFloatNumber");
		return str;
	}

	inline static const std::string &getJsonTypeBoolString() {
		static const std::string str("JsonBool");
		return str;
	}

	inline static const std::string &getJsonTypeNullString() {
		static const std::string str("JsonNull");
		return str;
	}
}; // class _JsonTypeString

///////////////////////////////// JSON 类型转换异常 /////////////////////////////////

class JsonException :public std::exception {
private:
	const std::string what_str;
public:
	JsonException(const std::string &str) :what_str(str) {
	}

	const char *what() const noexcept {
		return what_str.c_str();
	}

	JsonException(const JsonException &) = default;
	JsonException(JsonException &&) = default;
	JsonException &operator=(const JsonException &) = default;
	JsonException &operator=(JsonException &&) = default;
}; // class JsonException

///////////////////////////////// 工厂类型 /////////////////////////////////

class JFactory {
public:
	inline static JObjectPtr getNewObject() {
		return JObjectPtr(new _Object_);
	}

	inline static JArrayPtr getNewArray() {
		return JArrayPtr(new _Array_);
	}

	inline static JStringPtr getNewString() {
		return JStringPtr(new _String_);
	}

	inline static JIntNumberPtr getNewIntNumber() {
		return JIntNumberPtr(new _IntNumber_);
	}

	inline static JUIntNumberPtr getNewUIntNumber() {
		return JUIntNumberPtr(new _UIntNumber_);
	}

	inline static JFloatNumberPtr getNewFloatNumber() {
		return JFloatNumberPtr(new _FloatNumber_);
	}

	inline static JBoolPtr getNewBool() {
		return JBoolPtr(new _Bool_);
	}

	inline static JNullPtr getNewNull() {
		return JNullPtr(new _Null_);
	}

	inline static JObjectPtr getObject(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isObject()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeObjectString());
		}
		return std::dynamic_pointer_cast<_Object_>(ptr);
	}

	inline static JArrayPtr getArray(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isArray()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeArrayString());
		}
		return std::dynamic_pointer_cast<_Array_>(ptr);
	}

	inline static JStringPtr getString(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isString()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeStringString());
		}
		return std::dynamic_pointer_cast<_String_>(ptr);
	}

	inline static JIntNumberPtr getIntNumber(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isIntNumber()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeIntNumberString());
		}
		return std::dynamic_pointer_cast<_IntNumber_>(ptr);
	}

	inline static JUIntNumberPtr getUIntNumber(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isUIntNumber()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeUIntNumberString());
		}
		return std::dynamic_pointer_cast<_UIntNumber_>(ptr);
	}

	inline static JFloatNumberPtr getFloatNumber(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isFloatNumber()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeFloatNumberString());
		}
		return std::dynamic_pointer_cast<_FloatNumber_>(ptr);
	}

	inline static JBoolPtr getBool(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isBool()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeBoolString());
		}
		return std::dynamic_pointer_cast<_Bool_>(ptr);
	}

	inline static JNullPtr getNull(const JBasePtr &ptr) {
		if(ptr.get() == nullptr || !ptr->isNull()) {
			throw JsonException(_JsonTypeString_::getTypeString(ptr) +
				" cannot be converted to " +
				_JsonTypeString_::getJsonTypeNullString());
		}
		return std::dynamic_pointer_cast<_Null_>(ptr);
	}
}; // class JFactory

///////////////////////////////// 辅助类型 /////////////////////////////////

// JSON 文本解析器
class JParser {
private:
	// token 类型
	class Token {
		const std::string &s;
		size_t first, last;
		bool finish;
	public:
		Token(const std::string &ss) :s(ss), first(0), last(0), finish(false) {
		}

		inline bool isEnd() const {
			return finish;
		}

		inline void nextToken() {
			if(finish) {
				return;
			}
			// 跳过空白字符
			while(last < s.size() && isspace(s[last])) {
				++last;
			}
			// 以空白符结尾
			if(last == s.size()) {
				finish = true;
				return;
			}
			first = last;
			if(s[last] == '{' || s[last] == '}'
				|| s[last] == ':' || s[last] == ','
				|| s[last] == '[' || s[last] == ']') {
				// 为特殊字符
				++last;
			} else if(s[last] == '"') {
				// 为字符串
				while(last < s.size()) {
					++last;
					if(s[last] == '"' && s[last - 1] != '\\') {
						// 不是被转义后的字符
						++last;
						break;
					}
				}
			} else {
				// 为其他 token
				while(last < s.size()) {
					++last;
					if(isspace(s[last])
						|| s[last] == '{' || s[last] == '}'
						|| s[last] == ':' || s[last] == ','
						|| s[last] == '[' || s[last] == ']') {
						break;
					}
				}
			}
		}

		std::string tokenStr() const {
			return s.substr(first, last - first);
		}

		inline char at(size_t i) const {
			return s[first + i];
		}

		inline size_t size() const {
			return last - first;
		}
	}; // class Token
private:
	bool m_error_;
public:
	JBasePtr parse(const std::string &s) {
		m_error_ = false;
		Token tokens(s);
		tokens.nextToken();
		return JBasePtr(parseValue(tokens));
	}

	bool error() const {
		return m_error_;
	}
private:

	// 匹配值
	JBasePtr parseValue(Token &token) {
		if(error()) {
			return JNullPtr();
		}
		char c = token.at(0);
		switch(c) {
		case '{':
			// 匹配对象
			return parseObject(token);
		case '[':
			// 匹配列表
			return parseArray(token);
		case '"':
			// 匹配字符串
			return parseString(token);
		case '-':
			return parseInt(token);
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			return parseUInt(token);
		case 'f': case 't':
			// 匹配布尔
			return parseBool(token);
		case 'n':
			return parseNull(token);
		}
		m_error_ = true;
		return JBasePtr();
	}


	////////////////////////// 匹配函数，进入前不推进 token 出来后推进 token //////////////////////////

	JObjectPtr parseObject(Token &token) {
		// 判断是否正确
		if(!matchChar(token, '{')) {
			m_error_ = true;
			return JObjectPtr();
		}

		// 定义状态
		enum ObjectType {
			WAIT_KEY_OR_END,
			WAIT_KEY,
			WAIT_COLON,
			WAIT_VALUE,
			WAIT_COMMA_OR_END,
			END
		} status;
		status = ObjectType::WAIT_KEY_OR_END;

		// 创建返回值
		JObjectPtr res = JFactory::getNewObject();
		JStringPtr key;
		JBasePtr value;

		while(!token.isEnd() && !error() && status != ObjectType::END) {
			switch(status) {
			case WAIT_KEY_OR_END:
				// 为结尾
				if(matchChar(token, '}')) {
					status = ObjectType::END;
					break;
				}
			case WAIT_KEY:
				// 为键
				key = parseString(token);
				status = WAIT_COLON;
				break;
			case WAIT_COLON:
				if(matchChar(token, ':')) {
					status = ObjectType::WAIT_VALUE;
				} else {
					m_error_ = true;
				}
				status = WAIT_VALUE;
				break;
			case WAIT_VALUE:
				value = parseValue(token);
				status = WAIT_COMMA_OR_END;
				break;
			case WAIT_COMMA_OR_END:
				if(matchChar(token, '}')) {
					status = ObjectType::END;
				} else if(matchChar(token, ',')) {
					status = ObjectType::WAIT_KEY;
				} else {
					m_error_ = true;
				}
				res->set(key->get(), value);
				break;
			}
		}

		return res;
	}

	JArrayPtr parseArray(Token &token) {
		if(!matchChar(token, '[')) {
			m_error_ = true;
			return JArrayPtr();
		}

		enum ArrayType {
			WAIT_VALUE_OR_END,
			WAIT_VALUE,
			WAIT_COMMA_OR_END,
			END
		} status;
		status = ArrayType::WAIT_VALUE_OR_END;

		JArrayPtr res = JFactory::getNewArray();
		JBasePtr base;

		while(!token.isEnd() && !error() && status != ArrayType::END) {
			switch(status) {
			case WAIT_VALUE_OR_END:
				if(matchChar(token, ']')) {
					status = ArrayType::END;
				}
			case WAIT_VALUE:
				res->set(parseValue(token));
				status = ArrayType::WAIT_COMMA_OR_END;
				break;
			case WAIT_COMMA_OR_END:
				if(matchChar(token, ',')) {
					status = ArrayType::WAIT_VALUE;
				} else if(matchChar(token, ']')) {
					status = ArrayType::END;
				} else {
					m_error_ = true;
				}
				break;
			}
		}

		return res;
	}

	JStringPtr parseString(Token &token) {
		size_t t = token.size();
		// 进行匹配
		if(t < 2 || token.at(0) != '"' || token.at(t - 1) != '"' || error()) {
			m_error_ = true;
			return JStringPtr();
		}
		std::string s;
		for(size_t i = 1;i + 1 < t;++i) {
			if(token.at(i) == '\\') {
				// 需要转义
				++i;
				if(i + 1 == t) {
					m_error_ = true;
					return JStringPtr();
				}
				char c = parseEscape(token.at(i));
				if(c == '\0') {
					// 出现错误转义符
					m_error_ = true;
					return JStringPtr();
				}
				s.push_back(c);
			} else {
				// 不需要转义
				s.push_back(token.at(i));
			}
		}
		// 推进 token
		token.nextToken();
		// 创建返回值
		JStringPtr res = JFactory::getNewString();
		res->set(std::move(s));
		return res;
	}

	JBasePtr parseInt(Token &token) {
		int status = matchNumber(token);
		if(status == 0) {
			m_error_ = true;
			return JFloatNumberPtr();
		}
		std::string tmp(token.tokenStr());
		if(status == 1 || lessIntNumberMin(tmp)) {
			JFloatNumberPtr fn = JFactory::getNewFloatNumber();
			fn->set(strtod(tmp.c_str(), nullptr));
			token.nextToken();
			return fn;
		} else {
			JIntNumberPtr in = JFactory::getNewIntNumber();
			in->set(strtoll(tmp.c_str(), nullptr, 10));
			token.nextToken();
			return in;
		}
	}

	JBasePtr parseUInt(Token &token) {
		int status = matchNumber(token);
		if(status == 0) {
			m_error_ = true;
			return JFloatNumberPtr();
		}
		std::string tmp(token.tokenStr());
		if(status == 1 || greaterUIntNumberMax(tmp)) {
			JFloatNumberPtr fn = JFactory::getNewFloatNumber();
			fn->set(strtod(tmp.c_str(), nullptr));
			token.nextToken();
			return fn;
		} else {
			JUIntNumberPtr in = JFactory::getNewUIntNumber();
			in->set(strtoull(tmp.c_str(), nullptr, 10));
			token.nextToken();
			return in;
		}
	}

	JBoolPtr parseBool(Token &token) {
		if(token.size() == 4 && token.at(0) == 't' && token.at(1) == 'r'
			&& token.at(2) == 'u' && token.at(3) == 'e') {
			token.nextToken();
			JBoolPtr res = JFactory::getNewBool();
			res->set(true);
			return res;
		} else if(token.size() == 5 && token.at(0) == 'f' && token.at(1) == 'a'
			&& token.at(2) == 'l' && token.at(3) == 's' && token.at(4) == 'e') {
			token.nextToken();
			JBoolPtr res = JFactory::getNewBool();
			res->set(false);
			return res;
		} else {
			m_error_ = true;
			return JBoolPtr();
		}
	}

	JNullPtr parseNull(Token &token) {
		if(token.size() == 4 && token.at(0) == 'n' && token.at(1) == 'u'
			&& token.at(2) == 'l' && token.at(3) == 'l') {
			token.nextToken();
			return  JFactory::getNewNull();
		} else {
			m_error_ = true;
			return JNullPtr();
		}
	}

	char parseEscape(char c) {
		switch(c) {
		case '"':return '"';
		case '\\':return '\\';
		case '/':return '/';
		case 'b':return '\b';
		case 'f':return '\f';
		case 'n':return '\n';
		case 'r':return '\r';
		case 't':return '\t';
		}
		return '\0';
	}

	bool lessIntNumberMin(const std::string &s) {
		if(s.size() > 20) {
			return true;
		}
		if(s.size() < 20) {
			return false;
		}
		static constexpr char int_number_min_str[21] = "-9223372036854775808";
		for(int i = 1;i < 20;++i) {
			if(s[i] > int_number_min_str[i]) {
				return true;
			}
			if(s[i] < int_number_min_str[i]) {
				return false;
			}
		}
		return false;
	}

	bool greaterUIntNumberMax(const std::string &s) {
		if(s.size() > 20) {
			return true;
		}
		if(s.size() < 20) {
			return false;
		}
		static constexpr char uint_number_min_str[21] = "18446744073709551615";
		for(int i = 0;i < 20;++i) {
			if(s[i] > uint_number_min_str[i]) {
				return true;
			}
			if(s[i] < uint_number_min_str[i]) {
				return false;
			}
		}
		return false;
	}

	// 返回0：不是数值
	// 返回1：是浮点数
	// 返回2：可能能使用整数表示
	int matchNumber(Token &token) {
		enum NumberStatus {
			START = 0,
			NEG,
			ZERO,
			NZERO,
			DOT,
			DECIMAL,
			SNOT,
			SIGNAL,
			FRAC
		} status;
		status = NumberStatus::START;

		for(size_t i = 0;i < token.size();++i) {
			char c = token.at(i);
			switch(status) {
			case START:
				if(c == '-') {
					status = NEG;
				} else if(c == '0') {
					status = ZERO;
				} else if(c >= '1' && c <= '9') {
					status = NZERO;
				} else {
					return 0;
				}
				break;
			case NEG:
				if(c == '0') {
					status = ZERO;
				} else if(c >= '1' && c <= '9') {
					status = NZERO;
				} else {
					return 0;
				}
				break;
			case ZERO:
				if(c == '.') {
					status = DOT;
				} else {
					return 0;
				}
				break;
			case NZERO:
				if(c == '.') {
					status = DOT;
				} else if(c >= '0' && c <= '9') {
					status = NZERO;
				} else if(c == 'E' || c == 'e') {
					status = SNOT;
				} else {
					return 0;
				}
				break;
			case DOT:
				if(c <= '9' && c >= '0') {
					status = DECIMAL;
				} else {
					return 0;
				}
				break;
			case DECIMAL:
				if(c <= '9' && c >= '0') {
					status = DECIMAL;
				} else if(c == 'E' || c == 'e') {
					status = SNOT;
				} else {
					return 0;
				}
				break;
			case SNOT:
				if(c == '+' || c == '-') {
					status = SIGNAL;
				} else if(c >= '0' && c <= '9') {
					status = FRAC;
				} else {
					return 0;
				}
				break;
			case SIGNAL:
				if(c >= '0' && c <= '9') {
					status = FRAC;
				} else {
					return 0;
				}
				break;
			case FRAC:
				if(c >= '0' && c <= '9') {
					status = FRAC;
				} else {
					return 0;
				}
				break;
			}
		}
		switch(status) {
		case START: case NEG: case DOT: case SNOT: case SIGNAL:
			return 0;
		case DECIMAL: case FRAC:
			return 1;
		case ZERO: case NZERO:
			return 2;
		}
		return 0;
	}

	bool matchChar(Token &token, char c) {
		bool res = token.size() == 1 && token.at(0) == c;
		if(res) {
			token.nextToken();
		}
		return res;
	}
}; // class JParser

} // namespace json


#endif // _JSON_HPP__