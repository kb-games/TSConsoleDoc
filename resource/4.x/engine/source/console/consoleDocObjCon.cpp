#include <iostream>
#include "platform/platform.h"
#include "platform/platformMemory.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"
#include "console/compiler.h"
#include "console/codeBlock.h"
#include "console/simSet.h"
#include "console/arrayObject.h"
#include "console/scriptObjects.h"

#include "consoleDocObj.h"
#include "consoleDocObjCon.h"
#include <core/stream/fileStream.h>
#include "console/consoleDocFunctions.cpp"  // Namespaced helper functions.

using namespace Compiler;
using namespace std;


namespace {

    const char* ws = " \t\n\r\f\v";

    // trim from end of string (right)
    std::string& rtrim(std::string& s, const char* t = ws)
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning of string (left)
    std::string& ltrim(std::string& s, const char* t = ws)
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from both ends of string (right then left)
    std::string& trim(std::string& s, const char* t = ws)
    {
        return ltrim(rtrim(s, t), t);
    }

    void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }

    string getStringBetween(string text, string start, string end) {
        if(text==start) return text;
        text = " " + text;
        int ini = text.find(start);
        if (ini == string::npos) {
            return "";
        }

        int len = text.find(end, ini);
        if (len == string::npos) {
            return text.substr(ini+1);
        }
        len -= ini;
        return text.substr(ini+1, len-1);
    }
}


// --------------------------------------------------
SysDocUsage::SysDocUsage() {
    mBrief = "";
    mDescription = "";
    mFull = "";
    mFormatted = "";
};
SysDocUsage::~SysDocUsage() {
    mTSDocUsage = NULL;
};

void SysDocUsage::set(TSDocUsage* usage_in) {
    if (!usage_in) return;
    mBrief = usage_in->brief.c_str();
    mDescription = usage_in->description.c_str();
    mFormatted = usage_in->formatted.c_str();
    mFull = usage_in->full.c_str();
}


IMPLEMENT_CONOBJECT(SysDocUsage);
ConsoleDocClass(SysDocUsage,
    "@brief An object storing a method information structure for a defined console class.\n\n"
    "Internal Documentation class typically created by the consoleDocObject in response to"
    "a documentation request."
    "@ingroup Documentation"
);

void SysDocUsage::initPersistFields()
{
    Parent::initPersistFields();
    addField("brief", TypeString, Offset(mBrief, SysDocUsage),
        ".");
    addField("desc", TypeString, Offset(mDescription, SysDocUsage),
        ".");
    addField("full", TypeString, Offset(mFull, SysDocUsage),
        ".");
    addField("formatted", TypeString, Offset(mFormatted, SysDocUsage),
        ".");
}
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
SysDocMethod::SysDocMethod()
{
    mMethodName = "";
    mMethodGroup = "";
    mReturnType = "";
    mMethodNamespace = "";
    mMethodBaseClass = "";
    mMethodPackage = "";
    mMethodPrototype = "";
    mMinArg = 0;
    mMaxArg = 0;
    mScriptFile = "";
    mScriptLinenumber = 0;
    mIsScript = false;
    mIsVirtual = false;
    mIsCallback = false;

    //mTSDocMethod = new TSDocMethod();
    mTSDocMethod = NULL;
}

SysDocMethod::~SysDocMethod() {
    mTSDocMethod = NULL;   // We don't want to delete the underlying object, just clear the pointer.
}


void SysDocMethod::set(TSDocMethod* method_in)
{
    mMethodName = method_in->methodName.c_str();
    mReturnType = method_in->returnType.c_str();
    mMethodGroup = method_in->methodGroup.c_str();
    mMethodNamespace = method_in->methodNamespace.c_str();
    mMethodBaseClass = method_in->methodBaseClass.c_str();
    mMethodPackage = method_in->methodPackage.c_str();
    mMethodPrototype = method_in->methodPrototype.c_str();
    mMinArg = method_in->minArg;
    mMaxArg = method_in->maxArg;
    mScriptFile = method_in->scriptFile.c_str();
    mScriptLinenumber = method_in->scriptLinenumber;
    mIsScript = method_in->isScript;
    mIsVirtual = method_in->isVirtual;
    mIsCallback = method_in->isCallback;

    //mTSDocMethod->set(method_in);
    mTSDocMethod = method_in;
}

TSDocUsage* SysDocMethod::getUsage()
{
    if (!mTSDocMethod) return NULL;
    if (!mTSDocMethod->usage) return NULL;
    return mTSDocMethod->usage;
}


IMPLEMENT_CONOBJECT(SysDocMethod);
ConsoleDocClass(SysDocMethod,
    "@brief An object storing a method information structure for a defined console class.\n\n"
    "Internal Documentation class typically created by the consoleDocObject in response to"
    "a documentation request."
    "@ingroup Documentation"
);

void SysDocMethod::initPersistFields()
{
    Parent::initPersistFields();
    addGroup("Method");
    addField("methodName", TypeString, Offset(mMethodName, SysDocMethod),".");
    addField("methodGroup", TypeString, Offset(mMethodGroup, SysDocMethod),".");   
    addField("methodNamespace", TypeString, Offset(mMethodNamespace, SysDocMethod),".");
    addField("methodBaseClass", TypeString, Offset(mMethodBaseClass, SysDocMethod), ".");
    addField("methodPackage", TypeString, Offset(mMethodPackage, SysDocMethod),".");
    addField("methodPrototype", TypeString, Offset(mMethodPrototype, SysDocMethod), ".");
    addField("returnType", TypeString, Offset(mReturnType, SysDocMethod),".");
    addField("minArg", TypeS32, Offset(mMinArg, SysDocMethod),".");
    addField("maxArg", TypeS32, Offset(mMinArg, SysDocMethod),".");
    addField("scriptFile", TypeString, Offset(mScriptFile, SysDocMethod), ".");
    addField("scriptLinenumber", TypeS32, Offset(mScriptLinenumber, SysDocMethod),".");
    addField("isScript", TypeBool, Offset(mIsScript, SysDocMethod), ".");
    addField("isVirtual", TypeBool, Offset(mIsVirtual, SysDocMethod),".");
    addField("isCallback", TypeBool, Offset(mIsCallback, SysDocMethod),".");
    endGroup("Method");
}

DefineEngineMethod(SysDocMethod, getUsage, SimObject*, (), ,
    "Returns a usage object from the initialized method if set.\n")
{
    SysDocUsage* usage = new SysDocUsage;
    usage->registerObject();
    usage->set(object->getUsage());
    return (SimObject*)usage;
}


DefineEngineMethod(SysDocMethod, applyTemplate, const char*, (ScriptObject* tpl), ,
    "Returns a templated string representation of this method.\n"
    "@param tpl Script object defining the template.")
{
    TSDocTemplate * mtpl = new TSDocTemplate();
    mtpl->setTemplate(tpl);
    string rv = object->getDocMethod()->applyTemplate(mtpl);
    return object->getDocMethod()->templatedText.c_str();
    //return object->mDocClassObject->fieldset->templatedText.c_str();
}

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

SysDocField::SysDocField() {
    mFieldName = "";
    mFieldValue = "";
    mFieldType = "";
    mFieldGroup = "";
    mFieldText = "";    // simple text representation of the field information
    mIsDynamic = false;

    mIsArrayElement = false;
    mIndex = 0;
    mIsSubstitution = false;
    mFieldSubstitution = "";    
    mIsValueObject = false;  // if the value points to an object (???)
    mFieldValueClass = "";// If we want to nest.

    mTSDocField = NULL;
}

SysDocField::~SysDocField() {
    mTSDocField = NULL;    
}

void SysDocField::set(TSDocField* prop) {
    mFieldName = prop->fieldName.c_str();
    mFieldValue = prop->fieldValue.c_str();
    mFieldType = prop->fieldType.c_str();
    mFieldGroup = prop->fieldGroup.c_str();
    mFieldText = prop->fieldText.c_str();    // simple text representation of the field information
    mIsDynamic = prop->isDynamic;
    mIsArrayElement = prop->isArrayElement;
    mIndex = prop->index;
    mIsSubstitution = prop->isSubstitution;
    mFieldSubstitution = prop->fieldSubstitution.c_str();
    mIsValueObject = prop->isValueObject;
    mFieldValueClass = prop->fieldValueClass.c_str();
    mTSDocField = prop;
}

TSDocUsage* SysDocField::getUsage() {
    return mTSDocField->usage;
}



IMPLEMENT_CONOBJECT(SysDocField);
ConsoleDocClass(SysDocField,
    "@brief An object storing a method information structure for a defined console class.\n\n"
    "Internal Documentation class typically created by the consoleDocObject in response to"
    "a documentation request."
    "@ingroup Documentation"
);

void SysDocField::initPersistFields()
{
    Parent::initPersistFields();
    addGroup("Field");
    addField("fieldName", TypeString, Offset(mFieldName, SysDocField),
        "fieldname");
    addField("fieldValue", TypeString, Offset(mFieldValue, SysDocField),
        ".");
    addField("fieldType", TypeString, Offset(mFieldType, SysDocField),
        ".");
    addField("fieldGroup", TypeString, Offset(mFieldGroup, SysDocField),
        ".");
    addField("fieldText", TypeString, Offset(mFieldText, SysDocField),
        ".");
    addField("isDynamic", TypeBool, Offset(mIsDynamic, SysDocField),
        ".");
    addField("isArrayElement", TypeBool, Offset(mIsArrayElement, SysDocField),
        ".");
    addField("index", TypeS32, Offset(mIndex, SysDocField),
        ".");
    addField("isSubstitution", TypeBool, Offset(mIsSubstitution, SysDocField),
        ".");
    addField("fieldSubstitution", TypeString, Offset(mFieldSubstitution, SysDocField),
        ".");
    addField("isValueObject", TypeBool, Offset(mIsValueObject, SysDocField),
        "Determines if the set value points to a registered SimObject.");
    addField("fieldValueClass", TypeString, Offset(mFieldValueClass, SysDocField),
        ".");
    endGroup("Field");
}

DefineEngineMethod(SysDocField, getUsage, SimObject*, (), ,
    "Returns a usage object from the initialized field if set.\n")
{
    SysDocUsage* usage = new SysDocUsage;
    usage->registerObject();
    usage->set( object->getUsage() );
    return (SimObject *)usage;
}


DefineEngineMethod(SysDocField, applyTemplate, const char*, (const char* fieldtpl), ,
    "Returns a script object representing the listed field, if found.\n"
    "@param fieldname The new target object name.")
{    
    string str = fieldtpl;
    object->mTSDocField->applyTemplate(str); 
    return object->mTSDocField->templatedText.c_str();
}


// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

SysDocClassDB::SysDocClassDB() {}
SysDocClassDB::~SysDocClassDB() {}


IMPLEMENT_CO_DATABLOCK_V1(SysDocClassDB);

ConsoleDocClass(SysDocClassDB,
    "@brief Datablock implementation to allow script-level callbacks.\n\n"
    "Datablock object to allow global scripting for SysDocClass instances."
    "a documentation request."
    "@ingroup Documentation"
);

void SysDocClassDB::initPersistFields()
{
    Parent::initPersistFields();
}

// --------------------------------------------------
SysDocClass::SysDocClass() {
    mDocClassName = "";
    mDocClassParent = "";            
    mInheritance = "";
    //mFieldTemplate = "";
    mDocClassObject = NULL;
    mDatablock = NULL;

    mMethodGroup = NULL;
    mFieldGroup = NULL;
};

SysDocClass::~SysDocClass() {
    mDocClassObject = NULL;
    mDatablock = NULL;
};

void SysDocClass::set(TSDocClass* class_in) {
    if (!class_in) return;
    mDocClassName = class_in->docClassName.c_str();
    mDocClassParent = class_in->docClassParent.c_str();
    //mFieldTemplate = class_in->fieldTemplate.c_str();
    mInheritance = class_in->inheritance.c_str();;
    mDocClassObject = class_in;
    if (mFieldGroup) {
        mFieldGroup->deleteAllObjects();
        mFieldGroup->safeDeleteObject();
    }
    if (mMethodGroup) {
        mMethodGroup->deleteAllObjects();
        mMethodGroup->safeDeleteObject();
    }

    mFieldGroup = getFieldSet();
    mMethodGroup = getMethodSet();
}

SimSet* SysDocClass::getMethodGroup() {
    return mMethodGroup;
}

SimGroup* SysDocClass::getFieldGroup() {
    return mFieldGroup;
}

SysDocMethod* SysDocClass::getMethod(const char* methodname) {
    if (!mMethodGroup) return NULL;    
    for (SimSet::iterator i = mMethodGroup->begin(); i != mMethodGroup->end(); i++)
    {
        SysDocMethod * rmethod = dynamic_cast<SysDocMethod*>(*i);
        if (rmethod == NULL) continue;
        if (!strcasecmp(methodname, rmethod->mMethodName)) {
            return rmethod;
        }
    }
    return NULL;
}

SysDocClassDB * SysDocClass::getDatablock() {
    return mDatablock;
}

void SysDocClass::setDatablock(SysDocClassDB* datablock) {
    mDatablock = datablock;
}

SysDocUsage* SysDocClass::getUsage() {
    if (mDocClassObject->usage) 
    {
        SysDocUsage* u = new SysDocUsage();
        u->unregisterObject();
        u->set(mDocClassObject->usage);
        return u;
    }
    return NULL;
}

SimGroup* SysDocClass::getFieldSet() {
    SimGroup* fset = new SimGroup();
    fset->registerObject();

    fset->setDataField(StringTable->insert("groupType"), 0, "fieldset");
    fset->setDataField(StringTable->insert("docClass"), 0, mDocClassName);
   
    for (int i = 0; i < mDocClassObject->fieldset->fields.size(); i++)
    {
        TSDocField* fld = mDocClassObject->fieldset->get(i);
        if (fld) {
            SysDocField* cfld = new SysDocField();
            cfld->registerObject();
            cfld->set(fld);
            fset->addObject(cfld);
        }
    }
    return fset;
}

SimSet* SysDocClass::getMethodSet() {
    SimSet* mset = new SimGroup();
    mset->registerObject();

    mset->setDataField(StringTable->insert("setType"), 0, "methodset");
    mset->setDataField(StringTable->insert("docClass"), 0, mDocClassName);
    
    for (int i = 0; i < mDocClassObject->methodset->methods.size(); i++)
    {
        TSDocMethod* method = mDocClassObject->methodset->get(i);
        if (method) {
            SysDocMethod* cmethod = new SysDocMethod();
            cmethod->registerObject();
            cmethod->set(method);
            mset->addObject(cmethod);
        }
    }
    return mset;
}

SimGroup* SysDocClass::getClassDoc() {
    SimGroup* mset = new SimGroup();
    mset->registerObject();
    mset->setDataField(StringTable->insert("setType"), 0, "classdoc");
    mset->setDataField(StringTable->insert("docClass"), 0, mDocClassName);
    mset->setDataField(StringTable->insert("parent"), 0, mDocClassParent);
    mset->addObject(this, "classObject");
    mset->addObject(getMethodSet(), "methodset");
    mset->addObject(getFieldSet(), "fieldset");
    mset->addObject(getUsage(), "usage");
    return mset;
}

IMPLEMENT_CONOBJECT(SysDocClass);
ConsoleDocClass(SysDocClass,
    "@brief An object storing a method information structure for a defined console class.\n\n"
    "Internal Documentation class typically created by the consoleDocObject in response to"
    "a documentation request."
    "@ingroup Documentation"
);

void SysDocClass::initPersistFields()
{
    Parent::initPersistFields();
    addField("docClassName", TypeString, Offset(mDocClassName, SysDocClass),
        "Class name of the object being documented.");
    addField("docClassParent", TypeString, Offset(mDocClassParent, SysDocClass),
        "Parent class name of the object being documented.");
    addField("Inheritance", TypeString, Offset(mInheritance, SysDocClass),
        "Space separated class inheritance heiarchy.");
    addField("fieldTemplate", TypeString, Offset(mFieldTemplate, SysDocObject),
        "Replacement template used for generating field text.");
}

DefineEngineMethod(SysDocClass, getMethodGroup, SimSet*, (), ,
    "Returns a SimSet of all methods in the currently initialized TSDocClass.\n")
{
    return object->getMethodGroup();
}

DefineEngineMethod(SysDocClass, getFieldGroup, SimGroup*, (), ,
    "Returns a SimSet of all methods in the currently initialized TSDocClass.\n")
{
    return object->getFieldGroup();
}

DefineEngineMethod(SysDocClass, getMethod, SysDocMethod*, (const char* methodname), ,
    "Returns a script object representing the requested method from DocClassObject, if found.\n"
    "@param methodname The new target object name.")
{
    return object->getMethod(methodname);
}

DefineEngineMethod(SysDocClass, getFieldObj, SysDocField*, (const char* fieldname), ,
    "Returns a script object representing the listed field from DocClassObject, if found.\n"
    "@param fieldname The new target object name.")
{
    TSDocField* field = object->mDocClassObject->fieldset->get(fieldname);
    if (field) {
        SysDocField * sfield = new SysDocField();
        sfield->registerObject();
        sfield->set(field);
        return sfield;
    }
    return NULL;
}


DefineEngineMethod(SysDocClass, getFieldSet, SimGroup*, (), ,
    "Returns a SimSet of all fields defined in the currently initialized TSDocClass.\n")
{
    SimGroup* fset = new SimGroup();
    fset->registerObject();
    
    fset->setDataField(StringTable->insert("docClass"), 0, object->mDocClassName);
    fset->setDataField(StringTable->insert("setType"), 0, "fieldset");

    vector<TSDocField*> flds = object->mDocClassObject->fieldset->fields;
    for (int i = 0; i < flds.size(); i++)
    {
        TSDocField* fld = object->mDocClassObject->fieldset->get(i);
        if (fld) {
            SysDocField* cfld = new SysDocField();
            cfld->registerObject();
            cfld->set(fld);
            fset->addObject(cfld);
        }
    }
    return fset;
}

DefineEngineMethod(SysDocClass, getMethodObj, SysDocMethod*, (const char* methodname), ,
    "Returns a script object representing the requested method from DocClassObject, if found.\n"
    "@param methodname The new target object name.")
{
    TSDocMethod* method = object->mDocClassObject->methodset->get(methodname);
    if (method) {
        SysDocMethod* smethod = new SysDocMethod();
        smethod->registerObject();
        smethod->set(method);
        return smethod;
    }
    return NULL;
}

//DefineEngineMethod(SysDocClass, getMethodSet, SimGroup*, (), ,
DefineEngineMethod(SysDocClass, getMethodSet, SimSet*, (), ,
    "Returns a SimSet of all methods in the currently initialized TSDocClass.\n")
{
    return object->getMethodSet();
}

DefineEngineMethod(SysDocClass, getClassDoc, SimGroup*, (), ,
    "Returns a SimSet representing this class along with fields and methods.\n")
{
    return object->getClassDoc();
}

DefineEngineMethod(SysDocClass, applyTemplate, const char*, (const char* tpl), ,
    "Returns a script object representing the requested method, if found.\n"
    "@param methodname The new target object name.")
{
    object->mDocClassObject->fieldset->applyTemplate("");
    return object->mDocClassObject->fieldset->templatedText.c_str();
}

/*
DefineEngineMethod(SysDocClass, setDatablock, bool, (SysDocClassDB* datablock), ,
    "@brief Assign a new SysDocClassDB datablock for this docClass.\n"
    "@return void\n\n")
{
    if (datablock || datablock == NULL) {
        object->setDatablock(datablock);
        return true;
    }
    else {
        Con::warnf("setDatablock: datablock does not exist (or is not a SysDocClassDB)");
    }
    return false;
}

DefineEngineMethod(SysDocClass, getDatablock, SysDocClassDB*, (), ,
    "@brief Return the SysDocClassDB assigned to this SysDocClass.\n"
    "@return SysDocClassDB for this docClass.\n\n")
{
    return object->getDatablock();
}

*/

// ---- ********** ---------- ****** ---------
// ---- ********** ---------- ****** ---------
// ---- ********** ---------- ****** ---------

// default template to generate output for the "fieldText" of fields.
//string SysDocObject::sFieldTemplate = "{{type}} {{name}}{{idx}} = \"{{value}}\"";

SysDocObject::SysDocObject()
{
    mDocClass = "";
    mDocParent = "";    
    mMethodInheritTo = "";
    mFieldTemplate = sFieldTemplate;
    mMethodTemplate = sMethodTemplate;    
    mFieldsetTemplate = sFieldsetTemplate;
    mArgTemplate = sArgTemplate;
    mMethodsetTemplate = sMethodsetTemplate;
    mUsageTemplate = sUsageTemplate;

    mDocClassObject = new TSDocClass();
}

SysDocObject::~SysDocObject()
{
    delete mDocClassObject;
}

SysDocClass* SysDocObject::getDocClass(const char* name, bool filterParents) {
    SimObject* obj = Sim::findObject(name);
    if (obj) return getDocClass(obj, filterParents);
    return NULL;
}

SysDocClass* SysDocObject::getDocClass(SimObject* object, bool filterParents) {
    SysDocClass* sobj = new SysDocClass();
    sobj->registerObject();
    Con::printf("Inherit to: %s", mMethodInheritTo);
    TSDocClass* cobj = new TSDocClass();
    cobj->methodInheritTo = mMethodInheritTo;
    cobj->initObject(object,filterParents);
    sobj->set(cobj);
    return sobj;
}

IMPLEMENT_CONOBJECT(SysDocObject);
ConsoleDocClass(SysDocObject,
    "@brief An object storing the documentation structure for a defined console class.\n\n"
    "@ingroup Documentation"
);

void SysDocObject::initPersistFields()
{
	Parent::initPersistFields();
    addField("fieldTemplate", TypeString, Offset(mFieldTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("methodTemplate", TypeString, Offset(mMethodTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("methodsetTemplate", TypeString, Offset(mMethodsetTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("fieldsetTemplate", TypeString, Offset(mFieldsetTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("argTemplate", TypeString, Offset(mArgTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("usageTemplate", TypeString, Offset(mUsageTemplate, SysDocObject),
        "Replacement template used for generating field text.");
    addField("methodInheritTo", TypeString, Offset(mMethodInheritTo, SysDocObject),
        "Limit the return of inherited methods.");
}

DefineEngineMethod(SysDocObject, getDocClass, SimObject*, (const char* objName, bool filterParents /*, SysDocClassDB* datablock */), ,
    "Return a SysDocClass documenter initialized with the indicated object (if it exists).\n"
    "@param objName The object name/id.")
{
    //object->mMethodInheritTo = filterParents ? objName : "";

    SysDocClass* dcls = object->getDocClass(objName,1);
    /*
    if (dcls && datablock) {
        dcls->setDatablock(datablock);
    }
    */
    return dcls;
}

DefineEngineMethod(SysDocObject, exportClasses, void, (const char* fileName),("export.txt"),
    "Export system classes.\n"
    "@param fileName The file name for export.")
{
    TSDocClassSet* clsset = new TSDocClassSet();
    clsset->initSystemClasses(true, true);
    Con::printf("Class count: %d", clsset->classes.size() );
}


DefineEngineMethod(SysDocObject, setTemplate, bool, (ScriptObject* tpl), ,
    "Set the SysDoc template fields as defined in the provided script object.\n"
    "@param tpl ScriptObject holding a SysDoc template definition structure.")
{
    return object->mDocTemplate.setTemplate(tpl);
}

DefineEngineMethod(SysDocObject, applyTemplate, void, (SysDocClass* obj, const char* filename),("dump.txt"),
    "Return a class documenter for the reuquested object (if it exists).\n"
    "@param objName The new target object name.")
{
    string body = obj->mDocClassObject->applyTemplate(&object->mDocTemplate);

    FileStream* stream;
    // check for flags <selected, ...> 
    if ((stream = FileStream::createAndOpen(filename, Torque::FS::File::Write)) != NULL)
    {
        stream->write(dStrlen(body.c_str()), body.c_str());
        stream->close();
        Con::warnf("SysDocObject::applyTemplate - output to file successful (%s).", filename);
    }
    else {
        Con::errorf("SysDocObject::applyTemplate - could not open file:%s", filename);
    }
}
