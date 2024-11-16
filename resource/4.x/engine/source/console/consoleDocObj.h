#ifndef _CONSOLEDOC_H_
#define _CONSOLEDOC_H_

#ifndef _CONSOLEINTERNAL_H_
#include "console/consoleInternal.h"
#endif
#ifndef _SIMBASE_H_
#include "console/simBase.h"
#endif

#include "console/simSet.h"
#include "console/arrayObject.h"
#include "console/scriptObjects.h"

#include <string>
#include <vector>

using std::string;
using std::vector;


#pragma once
#include <string>
#include <vector>

//Uncomment for Afx/T3D_4 datablock field substitution
#define CONDOC_T3D_4 1

class TSDocClass;
using std::string;
using std::vector;

struct TSDocTemplate {
    string templateName = "";
    string templateVersion = "";
    bool fmtEOL = false;

    string classTpl = "";
    string classUTpl = "";
    string classsetTpl = "";
    string classsetEol = "";
    string classsetDelim = "";
    string classsetPad = "";

    string fieldTpl = "";
    string fieldUTpl = "";
    string fieldsetTpl = "";
    string fieldsetEol = "";
    string fieldsetDelim = "";
    string fieldsetPad = "";

    string methodTpl = "";
    string methodUTpl = "";
    string methodsetTpl = "";
    string methodsetEol = "";
    string methodsetDelim = "";
    string methodsetPad = "";
    
    string argTpl = "";
    string argsetTpl = "";
    string argsetEol = "";
    string argsetDelim = "";
    string argsetPad = "";

    void setTemplate(TSDocTemplate* tpl);
    bool setTemplate(ScriptObject* tpl);
    void setDefault();
};

// --------------------------------------------
struct argBrief {
    string arg;
    string brief;
};

// --------------------------------------------
class TSDocUsage {

public:
    string brief;
    string description;
    string full;
    string formatted;
    string templatedText;

    vector <string> examples;

    vector <argBrief*> params;

    TSDocUsage();
    ~TSDocUsage();

    void parseUsage(string usageString);
    void set(TSDocUsage* usage_in);

    //string applyTemplate(string body, TSDocTemplate* tpl);
    //string applyTemplate(TSDocTemplate* tpl);
    //string applyTemplate(string body);
    string applyTemplate(string body, bool fmtEOL = false);
};

class TSDocArg {

public:
    string argName;
    string argType;
    string argTypeNative;
    string argDefault;
    bool argRequired;
    string templatedText;

    TSDocUsage* usage;

    TSDocArg();
    ~TSDocArg();
    void set(TSDocArg* arg);
    
    string applyTemplate(TSDocTemplate* tpl);
    string TSDocArg::applyTemplate(string body, TSDocTemplate* tpl);
    void testUsage();
};

class TSDocArgSet {
public:
    TSDocArgSet();
    ~TSDocArgSet();

    vector <TSDocArg*> args;
    string templatedText;


    TSDocArg* getArg(string name);
    TSDocArg* getArg(int index);
    bool argExists(string name);
    int argIndex(string name);

    void setArg(TSDocArg* argObj);
    void setArg(string name, string type, TSDocUsage* usageObj = NULL);

    bool removeArg(string name);
    bool removeArg(int idx);

    void clear();

    string applyTemplate(TSDocTemplate* tpl);
    string applyTemplate(string body, TSDocTemplate* tpl);
    string argsetTemplate(TSDocTemplate* tpl);
};

class TSDocMethod
{

public:
    // (non-callback) Method types used in consoleDocInternal
    static const int MAX_METHOD_TYPES = 6;
    static const string METHOD_TYPES[MAX_METHOD_TYPES];// = { "string","int","float","void","bool","unknown" };    

    string returnType; 
    string methodName;
    string methodGroup;    
    string methodBaseClass;
    string methodNamespace;
    string methodPackage;
    string methodPrototype;

    int minArg;
    int maxArg;
    string scriptFile;
    int scriptLinenumber;
    bool isScript;
    bool isVirtual;
    bool isCallback;
    
    string templatedText;

    TSDocUsage* usage;
    TSDocArgSet* args;
    TSDocMethod();
    ~TSDocMethod();

    void set(TSDocMethod* method_in);
    string applyTemplate(TSDocTemplate* tpl);
    string applyTemplate(string body, TSDocTemplate* tpl);

};

class TSDocMethodSet {
public:
    TSDocMethodSet();
    ~TSDocMethodSet();

    vector <TSDocMethod*> methods;
    string templatedText;


    TSDocMethod* get(string name);
    TSDocMethod* get(int index);
    bool getExists(string name);
    int getIndex(string name);

    void set(TSDocMethod* argObj);

    bool remove(string name);
    bool remove(int idx);

    void clear();

    string applyTemplate(TSDocTemplate* tpl);
    string applyTemplate(string body, TSDocTemplate* tpl = NULL);

    string methodsetTemplate(TSDocTemplate* tpl);    
};

class TSDocField
{

public:
    TSDocField();
    ~TSDocField();
    
    string fieldName;
    string fieldValue;
    string fieldType;
    string fieldGroup;
    string fieldText;    // simple text representation of the field information
    string templatedText;
    bool isDepreciated;
    bool isDynamic;
    bool isArrayElement;
    int index;
    bool isSubstitution;
    string fieldSubstitution;
    //string fieldDoc;
    TSDocUsage* usage;
    bool isValueObject;  // if the value points to an object (???)
    string fieldValueClass;// If we want to nest.

    void set(TSDocField* prop);

    string applyTemplate(string body, TSDocTemplate* tpl = NULL);
    string applyTemplate(TSDocTemplate* tpl);

};

class TSDocFieldSet {
public:
    TSDocFieldSet();
    ~TSDocFieldSet();

    vector <TSDocField*> fields;
    string templatedText;

    TSDocField* get(string name);
    TSDocField* get(int index);
    bool getExists(string name);
    int getIndex(string name);

    void set(TSDocField* argObj);

    bool remove(string name);
    bool remove(int idx);

    void clear();

    string applyTemplate(TSDocTemplate* tpl);
    string applyTemplate(string body, TSDocTemplate* tpl=NULL);

    //string applyTemplate(string tpl = "\"fieldset\":[\n{{fieldset}}] ", string fieldtpl = "{\"fieldName\":\"{{name}}\",\"fieldType\":\"{{type}}\"}");
    string fieldsetTemplate(TSDocTemplate* tpl);
    //string fieldsetTemplate(string fldtpl, string delim = ",", string eol = "\n", string pad = "");
};

class TSDocClass
{
public:
    TSDocClass();
    ~TSDocClass();

    string docClassName;
    string docClassParent;
    string docClassNamespace;
    string docClassPackage;
    string group;
    string scriptFile;    
    int scriptLine;
    string inheritance;
    bool isScript;

    string templatedText;
    string methodInheritTo;
    TSDocUsage* usage;

    string fieldTemplate;
    static string sFieldTemplate;   // static default field template string.
    TSDocTemplate docTemplate;


    TSDocMethodSet* methodset;
    TSDocFieldSet* fieldset;


    void set(TSDocClass* obj);

    void setDefaultTemplate();

    void initObject(SimObject* object, bool filterParents=false);
    void initObject(const char* name, bool filterParents = false);
    void initObject(string name, bool filterParents = false) { return initObject(name.c_str(), filterParents); };
    
    void initClass(Namespace* classNamespace, bool getScript = true, bool getEngine = true);
    
    void initStaticFields(AbstractClassRep* object, bool filterParentFields = false);
    void initStaticFields(SimObject* object, bool filterParentFields = false);
    void initDynamicFields(SimObject* object);

    void initMethods(Namespace* ns, bool filterParents = false, bool getScript = true, bool getEngine = true);
    void initMethods(SimObject* object, bool filterParents=false, bool getScript = true, bool getEngine = true);

    TSDocMethod* getMethod();

    string applyTemplate(TSDocTemplate * tpl);
    string applyTemplate(string body, TSDocTemplate* tpl = NULL);
    void testObject();
};

class TSDocClassSet {
public:
    TSDocClassSet();
    ~TSDocClassSet();

    vector <TSDocClass *> classes;
    string templatedText;

    TSDocClass* get(string name);
    TSDocClass* get(int index);
    bool getExists(string name);
    int getIndex(string name);

    void set(TSDocClass* argObj);

    bool remove(string name);
    bool remove(int idx);

    void clear();

    string applyTemplate(TSDocTemplate* tpl);
    string applyTemplate(string body, TSDocTemplate* tpl = NULL);
    string classsetTemplate(TSDocTemplate* tpl);

    void initSystemClasses(bool getScript, bool getEngine);

};

#endif