#pragma once

#include <unordered_map>
#include <string>

namespace SKSE_HTTP_TypedDictionary
{
    // class TypedValueString;
    // class TypedValueInt;
    // class TypedValueFloat;
    // class TypedValueBool;
    // class TypedValueNestedDictionary;

    class TypedValue{
        public:
            virtual std::string getTypeName() = 0;
    };

    class TypedValueString : public TypedValue{
        private:
            std::string _value;    
        
        public:
            TypedValueString(std::string value);
            std::string getValue();
            std::string getTypeName() override;

    };

    class TypedValueInt : public TypedValue{
        private:
            int _value;
        
        public:
            TypedValueInt(int value);
            int getValue();
            std::string getTypeName() override;
    };

    class TypedValueFloat : public TypedValue{
        private:
            float _value;

        public:
            TypedValueFloat(float value);
            float getValue();
            std::string getTypeName() override;
    };

    class TypedValueBool : public TypedValue{
        private:
            bool _value;

        public:
            TypedValueBool(bool value);
            bool getValue();
            std::string getTypeName() override;
    };

    class TypedValueNestedDictionary : public TypedValue{
        private:
            int _value;

        public:
            TypedValueNestedDictionary(int value);
            int getValue();
            std::string getTypeName() override;
    };


    class TypedDictionary
    {
        public:
            std::unordered_map<std::string, std::shared_ptr<TypedValue>> _dicElements;

            TypedDictionary();
            void setString(std::string name, std::string value);
            void setInt(std::string name, int value);
            void setFloat(std::string name, float value);
            void setBool(std::string name, bool value);
            void setNestedDictionary(std::string name, int id);

            bool hasElement(std::string name);

            std::string getString(std::string name);
            int getInt(std::string name);
            float getFloat(std::string name);
            bool getBool(std::string name);
            int getNestedDictionary(std::string name);
    };
}