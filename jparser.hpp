#ifndef JPARSER_HPP
#define JPARSER_HPP

#include <fstream>
#include <memory>


namespace json {

class Object;

class Parser
{
public:
   Parser();

public:
   void readString(std::string str);
   void readFile(const std::string & fname);

   const Object & getRoot() const;

private:
   struct Context
   {
      bool string, key, value;
      int array, hash;

      Context() { reset(); }
      void reset() { string = key = value = false; array = hash = 0; }
      bool isNormal() { return !string && !array && !hash; }
   };

private:
   static void prepare(std::string & str);

   static Object parse(const std::string & str);
   static Object parseArray(const std::string & str);
   static Object parseHash(const std::string & str);

private:
   std::unique_ptr<Object> _root;
};


} // ns json

#endif // JPARSER_HPP
