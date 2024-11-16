///
///
/// Console proxy objects for the conDoc structure.
/// 
/// When working with large object sets and/or producing raw
/// output, we don't want the overhead of creating/managing
/// console objects for an entire documentation object set.
/// These proxy classes can be used to expose members of the
/// full object set to the console as needed.

#ifndef _CONSOLEDOCCON_H_
#define _CONSOLEDOCCON_H_

#include "console/consoleDocObj.h"

#include <string>
using std::string;


// --------------------------------------------

class SysDocUsage : public SimObject
{
    typedef SimObject Parent;

protected:
    TSDocUsage* mTSDocUsage;

public:
    SysDocUsage();
    ~SysDocUsage();

    const char* mBrief;
    const char* mDescription;
    const char* mFull;
    const char* mFormatted;

    void set(TSDocUsage* usage_in);

    // SimObject
    DECLARE_CONOBJECT(SysDocUsage);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");

    static void initPersistFields();
};

// -----------------------------------------------
// -----------------------------------------------

class SysDocMethod : public SimObject
{
    typedef SimObject Parent;

protected:
    TSDocMethod* mTSDocMethod;

public:
    SysDocMethod();
    ~SysDocMethod();

    static const int MAX_METHOD_TYPES = 6;
    static const string METHOD_TYPES[MAX_METHOD_TYPES];// = { "string","int","float","void","bool","unknown" };
    const char* mMethodName;
    const char* mMethodGroup;
    const char* mReturnType;
    const char* mMethodBaseClass;
    const char* mMethodNamespace;
    const char* mMethodPackage;
    const char* mMethodPrototype;

    int mMinArg;
    int mMaxArg;
    const char* mScriptFile;
    int mScriptLinenumber;
    bool mIsScript;
    bool mIsVirtual;
    bool mIsCallback;

    TSDocMethod* getDocMethod() { return mTSDocMethod; };
    
    void set(TSDocMethod* method_in);
    TSDocUsage* getUsage();




    // SimObject
    DECLARE_CONOBJECT(SysDocMethod);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");

    static void initPersistFields();

};
/*
class SysDocMethodSet : public SimGroup
{
    typedef SimGroup Parent;

public:
    SysDocMethodSet() {};
    ~SysDocMethodSet() {};

    // SimObject
    DECLARE_CONOBJECT(SysDocMethodSet);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("A simgroup subclass to group SysDocMethod objects for this console class.");
    static void initPersistFields();

};
*/
// -----------------------------------------------
// -----------------------------------------------

class SysDocField : public SimObject
{
    typedef SimObject Parent;

//protected:
public:
    TSDocField* mTSDocField;
public:
    SysDocField();
    ~SysDocField();

    const char* mFieldName;
    const char* mFieldValue;
    const char* mFieldType;
    const char* mFieldGroup;
    const char* mFieldText;    // simple text representation of the field information
    bool mIsDynamic;

    bool mIsArrayElement;
    int mIndex;
    bool mIsSubstitution;
    const char* mFieldSubstitution;
    bool mIsValueObject;  // if the value points to an object (???)
    const char* mFieldValueClass;// If we want to nest.

    void set(TSDocField* prop);
    string applyTemplate(string tpl);

    TSDocUsage* getUsage();

    // SimObject
    DECLARE_CONOBJECT(SysDocField);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");

    static void initPersistFields();
};

// -----------------------------------------------
// --------------------------------------------
class SysDocClassDB : public SimDataBlock
{
public:
    SysDocClassDB();
    ~SysDocClassDB();


    typedef SimDataBlock Parent;
    DECLARE_CONOBJECT(SysDocClassDB);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");
    static void initPersistFields();
};

// -----------------------------------------------
class SysDocClass : public SimObject
{
    typedef SimObject Parent;

    SysDocClassDB * mDatablock;
    SimSet * mMethodGroup;
    SimGroup * mFieldGroup;

public:
    SysDocClass();
    ~SysDocClass();

    TSDocClass* mDocClassObject;    // Underlying structure to hold class information

    const char* mDocClassName;
    const char* mDocClassParent;
    const char* mInheritance;


    //const char* mFieldTemplate;
    TSDocTemplate docTemplate;

    void set(TSDocClass* class_in);

    SimGroup* getFieldGroup();
    SimSet* getMethodGroup();

    SysDocMethod* getMethod(const char* methodname);

    SysDocClassDB* getDatablock();
    void setDatablock(SysDocClassDB* datablock);



    SysDocUsage* getUsage();

    // Generate script objects
    SimSet* getMethodSet();
    SimGroup* getFieldSet();
    SimGroup* getClassDoc();

    DECLARE_CONOBJECT(SysDocClass);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");
    static void initPersistFields();
};

// -----------------------------------------------
// -----------------------------------------------

class SysDocObject : public SimObject
{
    typedef SimObject Parent;
public:
    SysDocObject();
    ~SysDocObject();

    const char * mDocClass;
    const char* mDocParent;

    const char* mFieldsetTemplate;
    const char* mFieldTemplate;
    const char* mMethodTemplate;
    const char* mArgTemplate;
    const char* mMethodsetTemplate;
    const char* mUsageTemplate;

    TSDocTemplate mDocTemplate;
    //const char* sFieldTemplate = "{{type}} {{name}}{{idx}} = \"{{value}}\"";
    const char* sFieldTemplate = "{\"fieldName\":\"{{name}}\",\"fieldType\":\"{{type}}\"}";
    const char* sFieldsetTemplate = "\"fieldset\":[\n{{fieldset}}] ";
    const char* sMethodTemplate = "{{prototype}}";
    const char* sArgTemplate = "";
    const char* sMethodsetTemplate = "";
    const char* sUsageTemplate = "";


    const char* mMethodInheritTo;
    TSDocClass* mDocClassObject;    // Underlying structure to hold class information

    static void initPersistFields();

    //void initDocObject(SimObject* object,bool filterParents=false);
    //void initDocObject(const char* name, bool filterParents = false);
    //void initDocObject(string name, bool filterParents = false) { return initDocObject(name.c_str(), filterParents); };

    SysDocClass* getDocClass(SimObject* obj, bool filterParents = false);
    SysDocClass* getDocClass(const char * name, bool filterParents = false);
    SysDocClass* getDocClass(string name, bool filterParents = false) { return getDocClass(name.c_str(), filterParents); };

    // SimObject
    DECLARE_CONOBJECT(SysDocObject);
    DECLARE_CATEGORY("Documentation");
    DECLARE_DESCRIPTION("An object storing the documentation structure for a defined console class.");
};

#endif