#ifndef JOBJECT_HPP
#define JOBJECT_HPP

#include <string>
#include <vector>
#include <map> //TODO unordered
#include <cassert>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "json/jerror.hpp"


namespace json {

using boost::lexical_cast;
using boost::format;

class Object
{
public:
   enum Type
   {
      Number,
      Bool,
      String,
      Array,
      Hash,
      Null
   };

   static const std::map<Type, std::string> TypeMap;

public:
   Object();
   Object(Type type);
   Object(Type type, const std::string & data);
   Object(std::string x);
   Object(int x);
   Object(float x);
   Object(bool x);
   Object(const Object & copy);
//   Object(Object && move);

public:
//   static std::string typeToString(Type type);

public:
   Type getType() const;
   const std::string & getRawData() const;
   std::string toString(bool newline = false, int level = 0) const;

   Object & add(const Object & val);
   Object & add(const std::string & key, const Object & val);
   bool exists(const std::string & key) const;
   const Object & get(unsigned index) const;
   const Object & get(const std::string & key) const;

   template<typename T>
   T get() const;

   template<typename T>
   T get(unsigned index) const;

   template<typename T>
   T get(const std::string & key) const;

public:
   const std::string & string() const;
   int number() const;
   bool boolean() const;
   std::vector<Object> & getArray();
   const std::vector<Object> & getArray() const;
   std::map<std::string, Object> & getHash();
   const std::map<std::string, Object> & getHash() const;
   bool isNull() const;

private:
   Type _type;
   std::string _rawData;
   std::vector<Object> _array;
   std::map<std::string, Object> _hash;
};

template<typename T>
T Object::get() const
{
   T val;
   std::istringstream(_rawData) >> std::boolalpha >> val;
   return val;
}

template<typename T>
T Object::get(unsigned index) const
{
   assert(index < getArray().size());

   T val;
   std::istringstream(getArray()[index].getRawData()) >> std::boolalpha >> val;
   return val;
}

template<typename T>
T Object::get(const std::string & key) const
{
   if(!exists(key))
      throw std::runtime_error((boost::format("JSON: Элемент `%1%' не найден в хеше") % key).str());

   T val;
   std::istringstream(getHash().at(key).getRawData()) >> std::boolalpha >> val;
   return val;
}


} // ns json


#endif // JOBJECT_HPP
