#include "json/jobject.hpp"


namespace json {

const std::map<Object::Type, std::string> Object::TypeMap =
{{ Object::Number, "Number" },
 { Object::Bool,   "Bool" },
 { Object::String, "String" },
 { Object::Array,  "Array" },
 { Object::Hash,   "Hash" },
 { Object::Null,   "Null" } };

Object::Object()
   : Object(Null)
{
}

Object::Object(Type type)
   : _type(type)
{
}

Object::Object(Type type, const std::string & rawData)
   : _type(type), _rawData(rawData)
{
}

Object::Object(std::string x)
   : Object(String, x)
{
}

Object::Object(int x)
   : Object(Number, lexical_cast<std::string>(x))
{
}

Object::Object(float x)
   : Object(Number, lexical_cast<std::string>(x))
{
}

Object::Object(bool x)
   : Object(Bool, x ? "true":"false")
{
}

Object::Object(const Object & copy)
   : _type(copy._type),
     _rawData(copy._rawData),
     _array(copy._array),
     _hash(copy._hash)
{
}

//Object::Object(Object && move)
//   : _type(move._type),
//     _rawData(std::move(move._rawData)),
//     _array(std::move(move._array)),
//     _hash(std::move(move._hash))
//{
//}

Object::Type Object::getType() const
{
   return _type;
}

const std::string & Object::getRawData() const
{
   return _rawData;
}

std::string Object::toString(bool newline, int level) const
{
   std::stringstream ss;

   switch(getType())
   {
      case Null: ss << "null";
      case Number:
      case Bool: ss << getRawData();
         break;

      case String: ss << '"' << getRawData() << '"';
         break;

      case Array:
      {
         ss << "[";
         unsigned i = 0;
         for(const auto & x : getArray())
         {
            ss << x.toString();
            if(i++ != getArray().size() - 1) ss << ", ";

            // Добавлено
//            if(newline)
//               ss << '\n' << std::string((level - 1) * 3, ' ');
         }
         ss << "]";
         break;
      }

      case Hash:
      {
         level += 1;
         ss << "{";
         if(newline) ss << '\n';
         unsigned i = 0;
         for(const auto & x : getHash())
         {
            if(newline) ss << std::string(level * 3, ' ');
            ss << x.first
               << " : " << x.second.toString(newline, level);
            if(i++ != getHash().size() - 1)
            {
               ss << ", ";
               if(newline) ss << '\n';
            }
         }
         if(newline) ss << '\n' << std::string((level - 1) * 3, ' ');
         ss << "}";
         break;
      }
   }

   return ss.str();
}

Object & Object::add(const Object & val)
{
   getArray().push_back(val);
   return *this;
}

Object & Object::add(const std::string & key, const Object & val)
{
   getHash().insert(std::make_pair(key, val));
   return *this;
}

bool Object::exists(const std::string & key) const
{
   return getHash().find(key) != getHash().end();
}

const Object & Object::get(unsigned index) const
{
   return getArray().at(index);
}

const Object & Object::get(const std::string & key) const
{
   if(!exists(key))
      throw Error((boost::format("JSON: элементff %1% не найден в хеше.") % key).str());

   return getHash().at(key);
}

const std::string & Object::string() const
{
   if(_type != String)
      throw TypeError(String, _type);

   return _rawData;
}

int Object::number() const
{
   if(_type != Number)
      throw TypeError(Number, _type);

   int n;
   std::stringstream ss;
   ss << _rawData;
   ss >> n;
   return n;
}

bool Object::boolean() const
{
   if(_type != Bool)
      throw TypeError(Bool, _type);

   return _rawData == "true" ? true : false;
}

std::vector<Object> & Object::getArray()
{
   if(_type != Array)
      throw TypeError(Array, _type);

   return _array;
}

const std::vector<Object> & Object::getArray() const
{
   if(_type != Array)
      throw TypeError(Array, _type);

   return _array;
}

std::map<std::string, Object> & Object::getHash()
{
   if(_type != Hash)
      throw TypeError(Hash, _type);

   return _hash;
}

const std::map<std::string, Object> & Object::getHash() const
{
   if(_type != Hash)
      throw TypeError(Hash, _type);

   return _hash;
}

bool Object::isNull() const
{
   return _type == Null;
}




} // ns json
