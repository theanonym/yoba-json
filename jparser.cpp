#include "json/jparser.hpp"
#include "json/jobject.hpp"
#include "json/jerror.hpp"


namespace json {

Parser::Parser()
{
}

void Parser::readString(std::string str)
{
   assert(!_root);
   prepare(str);
   _root.reset(new Object(parse(str)));
}

void Parser::readFile(const std::string & fname)
{
   std::ifstream file(fname);
   if(!file.good())
      throw Error((boost::format("Не удалось прочитать файл `%1%'") % fname).str());

   std::stringstream ss;
   ss << file.rdbuf();
   file.close();

   readString(ss.str());
}

const Object & Parser::getRoot() const
{
   return *_root.get();
}

void Parser::prepare(std::string & str)
{
   Context context;
   for(unsigned i = 0; i <= str.size(); i++)
   {
      if(str[i] == '"') context.string = !context.string;
      if((str[i] == '\n' || str[i] == '\t') && context.isNormal())
      {
         str.erase(i, 1);
         i--;
      }
   }
}

Object Parser::parse(const std::string & str)
{
   if(str == "null")
      return Object(Object::Null);

   if(str == "true" || str == "false")
      return Object(Object::Bool, str);

   if(str[0] == '"')
      return Object(Object::String, str.substr(1, str.length() - 2));

   if((str[0] >= 48 && str[0] <= 57) || str[0] == '-')
      return Object(Object::Number, str);

   if(str[0] == '[')
      return parseArray(str);

   if(str[0] == '{')
      return parseHash(str);

   throw Error((boost::format("Не удалось распознать тип: `%1%'") % str).str());
   return Object();
}

Object Parser::parseArray(const std::string & str)
{
   Object ret(Object::Array);

   std::string buf;
   bool is_string = false;
   int is_array = 0;
   int is_hash  = 0;
   for(unsigned i = 1; i < str.size(); i++)
   {
      if(str[i] == '"' && str[i - 1] != '\\')
         is_string = !is_string;

      if(!is_string)
      {
         if(str[i] == '[')
            is_array++;
         if(str[i] == ']')
            is_array--;

         if(str[i] == '{')
            is_hash++;
         if(str[i] == '}')
            is_hash--;
      }

      if(i == str.size() - 1)
      {
         ret.getArray().push_back(parse(buf));
         break;
      }

      if(str[i] == ',' && !is_string && !is_array && !is_hash)
      {
         ret.getArray().push_back(parse(buf));
         buf.clear();
      }
      else if(str[i] != ' ' || is_string || is_array || is_hash)
      {
         buf += str[i];
      }
   }

   return ret;
}

Object Parser::parseHash(const std::string & str)
{
   Object ret(Object::Hash);

   std::string keybuf, valbuf;
   Context context;
   context.key = true;

   for(unsigned i = 1; i <= str.size(); i++)
   {
      if(str[i] == '"') context.string = !context.string;
      if(!context.string)
      {
         if(str[i] == '[') context.array++;
         if(str[i] == ']') context.array--;
         if(str[i] == '{') context.hash++;
         if(str[i] == '}') context.hash--;
      }

      if(str[i] == ' ' && context.isNormal())
         continue;

      if(str[i] == ':' && context.isNormal())
      {
         context.key   = false;
         context.value = true;
         continue;
      }

      if(i == str.size() - 1)
      {
         ret.add(keybuf, parse(valbuf));
         break;
      }

      if(str[i] == ','  && context.isNormal())
      {
         ret.add(keybuf, parse(valbuf));
         context.key   = true;
         context.value = false;
         keybuf.clear();
         valbuf.clear();
         continue;
      }

      if(context.key)
      {
         keybuf += str[i];
      }
      if(!context.key)
      {
         valbuf += str[i];
      }
   }

   return ret;
}


} // ns json

