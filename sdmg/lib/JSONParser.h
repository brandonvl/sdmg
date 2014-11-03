#pragma once
#include <string>
#include <map>
#include <vector>

namespace JSON {
	class JSONArray;
	class JSONDocument;
	class JSONElement;
	class JSONException;
	class JSONObject;
	class JSONParser;
	class JSONVariable;

	class JSONException
	{
	public:
		JSONException(std::string description);
		virtual ~JSONException();
		std::string getDescription();

	private:
		std::string _description;
	};

	class JSONElement
	{
	public:
		JSONElement(JSONElement *parent);
		virtual ~JSONElement();
		void setKey(const std::string key);
		const std::string getKey();
		const JSONElement &getParent();
		virtual std::string str() = 0;

		enum class Type { Object, Array, Variable };

		Type getType();

	protected:
		void setType(Type type);
	private:
		std::string _key;
		JSONElement *_parent;
		Type _type;
	};

	class JSONObject :
		public JSONElement
	{
	public:
		JSONObject(JSONElement *parent);
		virtual ~JSONObject();

		void add(std::string key, JSONElement &element);
		void set(std::string key, JSONElement &element);
		JSONElement &get(std::string key);
		std::string getString(std::string key);
		float getFloat(std::string key);
		int getInt(std::string key);
		bool getBoolean(std::string key);
		JSONArray &getArray(std::string key);
		JSONObject &getObject(std::string key);
		JSONVariable &getVariable(std::string key);
		bool exists(std::string key);

		std::string str() override;
	private:
		std::map<std::string, JSONElement*> _elements;
	};

	class JSONVariable :
		public JSONElement
	{
	public:
		JSONVariable(JSONElement *parent);
		virtual ~JSONVariable();

		enum class ValueType { String, Number, Boolean };

		std::string getValue();
		std::string getString();
		float getFloat();
		int getInt();
		bool getBoolean();
		void setValue(std::string value);
		std::string str() override;
		ValueType getValueType();
		void setValueType(ValueType type);
	private:
		std::string _value;
		ValueType _type;
	};

	class JSONArray :
		public JSONElement
	{
	public:
		JSONArray(JSONElement *parent);
		virtual ~JSONArray();

		JSONElement &get(int i);
		JSONObject &getObject(int i);
		JSONArray &getArray(int i);
		JSONVariable &getVariable(int i);
		float getFloat(int i);
		int getInt(int i);
		std::string getString(int i);
		bool getBoolean(int i);
		bool exists(int i);
		
		int size();
		void push(JSONElement &element);
		std::string str() override;
	private:
		std::vector<JSONElement*> _elements;
	};

	class JSONDocument
	{
	public:
		static JSONDocument *fromFile(std::string path);
		virtual ~JSONDocument();
		std::string str();
		JSONElement &getRoot();
		JSONObject &getRootObject();
		JSONArray &getRootArray();
		void saveFile(std::string path);

	private:
		JSONDocument();
		JSONElement *_rootElement;
	};

	class JSONParser
	{
	public:
		JSONParser();
		~JSONParser();
		JSONElement *load(std::string path);
	private:
		JSONElement *parse();
		JSONObject &parseObject(JSONElement *parent = nullptr);
		JSONArray &parseArray(JSONElement *parent = nullptr);
		JSONVariable &parseVariable(JSONElement *parent = nullptr);
		bool isNumber(std::string str);
		bool isBool(std::string str);

		char next();
		char curChar();
		char previous();

		std::string _fileContents;
		int _curReadPos;
	};
}