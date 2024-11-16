#include <iostream>
#include "consoleDocObj.h"

#include "platform/platform.h"
#include "platform/platformMemory.h"
#include "console/console.h"
//#include "console/consoleInternal.h"
#include "console/consoleObject.h"
#include "console/engineAPI.h"
#include "console/compiler.h"
#include "console/codeBlock.h"
#include "console/consoleDocFunctions.cpp"
#include "consoleDocObj.h"

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

    string getLine(string text, size_t offset = 0) {
        if (offset > text.length() || text.length()==0) return "";

        int epos = text.find("\n", offset);
        if (epos == string::npos) {
            return text.substr(offset);            
        }
        int len = epos - offset;
        return text.substr(offset,len);
    }

    string getStringBetween(string text, string start, string end, size_t offset = 0) {
        if(text==start) return text;
        text = " " + text;
        int ini = text.find(start,offset);
        if (ini == string::npos) {
            return "";
        }

        int len = text.find(end, ini);
        if (len == string::npos) {
            return text.substr(ini+1);
        }
        len -= ini;
        return text.substr(ini+start.length(), len - start.length());
    }

    string deleteBetween(string text, string start, string end) {
        if (text == start) return text;
        string rv = " " + text;
        int ini = rv.find(start);
        if (ini == string::npos) {
            return text;
        }
        int epos = rv.find(end, ini);
        if (epos == string::npos) {
            return rv.substr(1,ini + 1);
        }
        int len = epos - ini;
        string l = rv.substr(1, ini-1);
        string r = rv.substr(epos+end.size());
        Con::printf("l:%s \nr:%s", l.c_str(), r.c_str());
        string out = l + r;

        return out;
        //return text.substr(ini + 1, len - 1);
    }
}


// --------------------------------------------------
TSDocUsage::TSDocUsage() {
    brief = "";
    description = "";
    full = "";
    formatted = "";
};
TSDocUsage::~TSDocUsage() {
    examples.clear();
};

void TSDocUsage::set(TSDocUsage* usage_in) {
    if (!usage_in) return;
    brief = usage_in->brief;
    description = usage_in->description;
    formatted = usage_in->formatted;
    full = usage_in->full;
    examples.clear();
    for (std::vector<string>::iterator i = usage_in->examples.begin(); i != usage_in->examples.end(); ++i)
    {
        examples.push_back(*i);
    }
}


void TSDocUsage::parseUsage(string usageString) {
    if (usageString.empty()) {        
        brief = "";
        full = "";
        return;
    }
    ReplaceString(usageString, "\"", "\\\"");
    full = usageString;
    int pos = usageString.find("@brief");
    if ( pos != string::npos) {
        brief = trim(getLine(usageString, pos + 6));
    }
    else {
        brief = trim(getLine(usageString));   // Just use the first line as the usage string.
    }
}

/*
string TSDocUsage::applyTemplate(TSDocTemplate* tpl)
{
    string mtpl = tpl->;
    string rv = applyTemplate(mtpl, tpl);
    return rv;
}
*/
string TSDocUsage::applyTemplate(string body, bool fmtEOL)
{
    templatedText = body;
    string t = "";
    // if (body.find("{{argset}}") != string::npos) { }
    t = brief;
    if (fmtEOL) ReplaceString(t, "\n", "\\n");
    ReplaceString(templatedText, "{{u_brief}}", t);

    t = description;
    if(fmtEOL) ReplaceString(t, "\n", "\\n");
    ReplaceString(templatedText, "{{u_desc}}", t);

    t = full;
    if (fmtEOL) ReplaceString(t, "\n", "\\n");
    ReplaceString(templatedText, "{{u_full}}", t);

    t = formatted;
    if (fmtEOL) ReplaceString(t, "\n", "\\n");
    ReplaceString(templatedText, "{{u_formatted}}", t);

    return templatedText;
}

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
TSDocArg::TSDocArg() {
    argName = "";
    argType = "";
    argDefault = "";
    argRequired = false;
    templatedText = "";
    usage = new TSDocUsage();
}

TSDocArg::~TSDocArg() {
    delete usage;
}

void TSDocArg::testUsage() {
    //cout << usage->brief << endl;
    //cout << usage->examples.size() << endl;
}

void TSDocArg::set(TSDocArg* arg) {
    argName = arg->argName;
    argType = arg->argType;
    argTypeNative = arg->argTypeNative;
    argDefault = arg->argDefault;
    argRequired = arg->argRequired;
    templatedText = arg->templatedText;

    usage->set(arg->usage);
}

string TSDocArg::applyTemplate(TSDocTemplate* tpl)
{
    string atpl = tpl->argTpl;
    string rv = applyTemplate(atpl, tpl);
    return rv;
}
string TSDocArg::applyTemplate(string body, TSDocTemplate* tpl)
{
    templatedText = body;
    ReplaceString(templatedText, "{{name}}", argName);
    ReplaceString(templatedText, "{{type}}", argType);
    ReplaceString(templatedText, "{{typenative}}", argTypeNative);
    ReplaceString(templatedText, "{{default}}", argDefault);
    ReplaceString(templatedText, "{{required}}", argRequired ? "1" : "0");
    //return usage->applyTemplate(tpl); // Fill in any documentation fields in the template.
    //Con::printf("%s", body.c_str());
    return templatedText;
}
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

TSDocMethod::TSDocMethod()
{
    methodName = "";
    methodGroup = "";
    returnType = "";
    methodNamespace = "";
    methodBaseClass = "";
    methodPackage = "";
    minArg = 0;
    maxArg = 0;
    scriptFile = "";
    scriptLinenumber = 0;
    isScript = false;
    isVirtual = false;
    isCallback = false;
    templatedText = "";

    usage = new TSDocUsage();
    args = new TSDocArgSet();

}

TSDocMethod::~TSDocMethod() {
    delete usage;
    delete args;
}


void TSDocMethod::set(TSDocMethod* method_in)
{

    methodName = method_in->methodName;
    returnType = method_in->returnType;
    methodGroup = method_in->methodGroup;
    methodNamespace = method_in->methodNamespace;
    methodBaseClass = method_in->methodBaseClass;
    methodPackage = method_in->methodPackage;
    methodPrototype = method_in->methodPrototype;
    minArg = method_in->minArg;
    maxArg = method_in->maxArg;
    scriptFile = method_in->scriptFile;
    scriptLinenumber = method_in->scriptLinenumber;
    isScript = method_in->isScript;
    isVirtual = method_in->isVirtual;
    isCallback = method_in->isCallback;
    templatedText = method_in->templatedText;

    usage->set(method_in->usage);

    args->clear();
    for (int i = 0; i < method_in->args->args.size(); i++) {
        args->setArg(method_in->args->args[i]);
    }
}

string TSDocMethod::applyTemplate(TSDocTemplate* tpl)
{
    string mtpl = tpl->methodTpl;
    string rv = applyTemplate(mtpl, tpl);
    return rv;
}
string TSDocMethod::applyTemplate(string body, TSDocTemplate* tpl)
{
    templatedText = body;
    if (body.find("{{argset}}") != string::npos) {
        string astr = args->applyTemplate(tpl);
        ReplaceString(templatedText, "{{argset}}", astr);
    }

    ReplaceString(templatedText, "{{name}}", methodName);
    ReplaceString(templatedText, "{{type}}", returnType);
    ReplaceString(templatedText, "{{group}}", methodGroup);
    ReplaceString(templatedText, "{{namespace}}", methodNamespace);
    ReplaceString(templatedText, "{{baseclass}}", methodBaseClass);
    ReplaceString(templatedText, "{{package}}", methodPackage);
    ReplaceString(templatedText, "{{prototype}}", methodPrototype);
    ReplaceString(templatedText, "{{minarg}}", to_string(minArg) );
    ReplaceString(templatedText, "{{maxarg}}", to_string(maxArg));
    ReplaceString(templatedText, "{{file}}", scriptFile);
    ReplaceString(templatedText, "{{lineno}}", to_string(scriptLinenumber));
    ReplaceString(templatedText, "{{isscript}}", isScript ? "1" : "0");
    ReplaceString(templatedText, "{{isvirtual}}", isVirtual ? "1" : "0");
    ReplaceString(templatedText, "{{iscallback}}", isCallback ? "1" : "0");

    //templatedText = usage->applyTemplate(tpl->methodUTpl);
    templatedText = usage->applyTemplate(templatedText,tpl->fmtEOL);
    // Process usage blocks based on defined usage template.
    if (templatedText.find("{{usage}}") != string::npos) {
        string ubod = usage->applyTemplate(tpl->methodUTpl, tpl->fmtEOL); // Apply usage template to catch any direct usage fields
        ReplaceString(templatedText, "{{usage}}", ubod);
    }

    //return usage->applyTemplate(tpl); // Fill in any documentation fields in the template.
    //Con::printf("%s", body.c_str());
    return templatedText;
}

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

TSDocField::TSDocField() {
    fieldName = "";
    fieldValue = "";
    fieldType = "";
    fieldGroup = "";
    fieldText = "";    // simple text representation of the field information
    isDynamic = false;
    templatedText = "";
    isArrayElement = false;
    index = 0;
    isSubstitution = false;
    fieldSubstitution = "";    
    isValueObject = false;  // if the value points to an object (???)
    fieldValueClass = "";// If we want to nest.
    templatedText = "";

    usage = new TSDocUsage();
}

TSDocField::~TSDocField() {
    // delete valObject;    // We shouldn't ever hold responsiblitiy for this here.
    delete usage;
}

void TSDocField::set(TSDocField* prop) {
    fieldName = prop->fieldName;
    fieldValue = prop->fieldValue;
    fieldType = prop->fieldType;
    fieldGroup = prop->fieldGroup;
    fieldText = prop->fieldText;    // simple text representation of the field information
    isDynamic = prop->isDynamic;
    isArrayElement = prop->isArrayElement;
    index = prop->index;
    isSubstitution = prop->isSubstitution;
    fieldSubstitution = prop->fieldSubstitution;
    isValueObject = prop->isValueObject;
    fieldValueClass = prop->fieldValueClass;
    templatedText = prop->templatedText;
    usage->set(prop->usage);
}

string TSDocField::applyTemplate(TSDocTemplate* tpl) 
{
    string ftpl = tpl->fieldTpl;
    string rv = applyTemplate(ftpl, tpl);
    return rv;
}

string TSDocField::applyTemplate(string body, TSDocTemplate* tpl)
{
    //string tpl = "{\"fieldName\":\"{{name}}\",\"fieldType\":\"{{type}}\",\"fieldGroup\":\"{{group}}\",\"fieldValue\":\"{{value}}\",\"isDynamic\":\"{{isDynamic}}\"}";
    templatedText = body;
    ReplaceString(templatedText,"{{name}}", fieldName);
    ReplaceString(templatedText, "{{value}}", fieldValue);
    ReplaceString(templatedText, "{{type}}", fieldType);
    ReplaceString(templatedText, "{{group}}", fieldGroup);
    ReplaceString(templatedText, "{{text}}", fieldText);
    ReplaceString(templatedText, "{{isarray}}", isArrayElement ? "1" : "0");
    ReplaceString(templatedText, "{{index}}", isArrayElement ? ("[" + to_string(index) + "]") : "");
    ReplaceString(templatedText, "{{isdynamic}}", isDynamic ? "1" : "0");
    ReplaceString(templatedText, "{{issub}}", isSubstitution ? "1" : "0");
    ReplaceString(templatedText, "{{sub}}", fieldSubstitution);
    //return usage->applyTemplate(tpl); // Fill in any documentation fields in the template.
    //Con::printf("%s", tpl.c_str());
    templatedText = usage->applyTemplate(templatedText, tpl->fmtEOL); // Apply usage template to catch any direct usage fields
    // Process usage blocks based on defined usage template.
    if (templatedText.find("{{usage}}") != string::npos) {
        string ubod = usage->applyTemplate(tpl->fieldUTpl, tpl->fmtEOL); // Apply usage template to catch any direct usage fields
        ReplaceString(templatedText, "{{usage}}", ubod);
    }
    return templatedText;
}

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
string TSDocClass::sFieldTemplate = "{{type}} {{name}}{{idx}} = \"{{value}}\"";
TSDocClass::TSDocClass()
{
    docClassName = "";
    docClassParent = "";
    inheritance = "";
    scriptFile = "";
    scriptLine = 0;
    group = "";

    usage = new TSDocUsage();
    methodset = new TSDocMethodSet();
    fieldset = new TSDocFieldSet();

    fieldTemplate = sFieldTemplate;
}

TSDocClass::~TSDocClass() {
    delete usage;
    delete methodset;
    delete fieldset;
}

void TSDocClass::set(TSDocClass* obj) {
    docClassName = obj->docClassName;
    docClassParent = obj->docClassParent;
    inheritance = obj->inheritance;
    scriptFile = obj->scriptFile;
    scriptLine = obj->scriptLine;
    group = obj->group;
    
    obj->usage->set(obj->usage);

    methodset->clear();
    for (int i = 0; i < obj->methodset->methods.size(); i++) {
        methodset->set(obj->methodset->methods[i]);
    }
    fieldset->clear();
    for (int i = 0; i < obj->fieldset->fields.size(); i++) {
        fieldset->set(obj->fieldset->fields[i]);
    }
}

void TSDocClass::setDefaultTemplate() {
    docTemplate.templateName = "";
    docTemplate.templateVersion = "";

    docTemplate.classTpl = "";

    docTemplate.fieldTpl = "{{type}} {{name}}{{idx}} = \"{{value}}\"";
    docTemplate.fieldsetTpl = "";
    docTemplate.fieldsetEol = "";
    docTemplate.fieldsetDelim = "";
    docTemplate.fieldsetPad = "";

    docTemplate.methodTpl = "";
    docTemplate.methodsetTpl = "";
    docTemplate.methodsetEol = "";
    docTemplate.methodsetDelim = "";
    docTemplate.methodsetPad = "";

    docTemplate.argsetTpl = "";
    docTemplate.argsetDelim = "";    
}

TSDocMethod* TSDocClass::getMethod() {
    TSDocMethod* nmethod = new TSDocMethod();
    return nmethod;
}


// --------------------------------------------------------------
//
void TSDocClass::initClass(Namespace* classNamespace, bool getScript, bool getEngine) {
    const char* className = classNamespace->mName;
    const char* superClassName = classNamespace->mParent ? classNamespace->mParent->mName : NULL;
    AbstractClassRep* pRep = classNamespace->mClassRep;
    if (!pRep) {
        return;
    }
    docClassNamespace = classNamespace->getName();
    docClassPackage = classNamespace->getPackage() ? classNamespace->getPackage() : "";    
    docClassName = pRep->getClassName();
    if (pRep->getParentClass()) {
        docClassParent = pRep->getParentClass()->getClassName() ? pRep->getParentClass()->getClassName() : "";
    }
    scriptFile = "";
    scriptLine = 0;

    AbstractClassRep* parentRep = classNamespace->mParent ? classNamespace->mParent->mClassRep : NULL;
    
    string tmp;
    tmp = pRep->getDocString() ? pRep->getDocString() : "";
    if(!tmp.empty()) usage->parseUsage(tmp);

    tmp = pRep->getDescription() ? (string)pRep->getDescription() : "";
    ReplaceString(tmp, "\"", "\\\"");  // Need to escape quotes in usage examples.
    usage->description = tmp;

    string inhtree = "";
    char* ch = "";
    AbstractClassRep* tRep = pRep;
    //tRep = pRep->getParentClass();  // skip our own class in the inheritance?
    while (tRep)
    {
        inhtree += (string)ch + tRep->getClassName();
        ch = " "; //ch = "->"
        tRep = tRep->getParentClass();
    }
    inheritance = inhtree;

    if (getEngine) {
        initStaticFields(pRep,parentRep);
    }
}

void TSDocClass::initObject(const char* name, bool filterParents) {
    SimObject* obj = Sim::findObject(name);
    if (obj)
    {
        initObject(obj,filterParents);
    }
}

void TSDocClass::initObject(SimObject* object, bool filterParents) {
    if (!object->getClassName()) {
        Con::errorf("TSDocClass::initObject: empty class name?");
        return;
    }
    AbstractClassRep* pRep = object->getClassRep();
    docClassName = pRep->getClassName();    
    if (pRep->getParentClass()) {
        docClassParent = pRep->getParentClass()->getClassName() ? pRep->getParentClass()->getClassName() : "";
    }
    docClassNamespace = object->getNamespace()->getName();
    docClassPackage = object->getNamespace()->getPackage() ? object->getNamespace()->getPackage() : "";
    scriptFile = object->getFilename() ? object->getFilename() : "";
    scriptLine = object->getDeclarationLine();
    group = object->getGroup()->getName() ? object->getGroup()->getName() : "";
    string tmp;
    tmp = pRep->getDocString();    
    usage->parseUsage(tmp);
    const char* tmp2 = pRep->getDescription();
    if(tmp2){
        tmp = (string)tmp2;
        ReplaceString( tmp, "\"", "\\\"");  // Need to escape quotes in usage examples.
    }
    else {
        tmp = "";
    }
    usage->description = tmp;

    string inhtree = "";
    char* ch = "";
    //pRep = pRep->getParentClass();  // skip our own class in the inheritance?
    while (pRep)
    {
        inhtree += (string)ch + pRep->getClassName();
        ch = " "; //ch = "->"
        pRep = pRep->getParentClass();
    }
    inheritance = inhtree;

    initStaticFields(object,filterParents);
    initDynamicFields(object);
    Con::printf("Inherit to2: %s", methodInheritTo);
    initMethods(object, filterParents);
}


void TSDocClass::initStaticFields(SimObject* object, bool filterParentFields) {
    // Init fields from classRep
    initStaticFields(object->getClassRep(), filterParentFields);

    // Add any instance-object based field info.
    for (int i = 0; i < fieldset->fields.size(); i++) {
        const AbstractClassRep::Field* f = object->findField(StringTable->insert(fieldset->fields[i]->fieldName.c_str()));
        // Set the value of fields from object.
        const char* val = (*f->getDataFn)(object, Con::getData(f->type, (void*)(((const char*)object) + f->offset), fieldset->fields[i]->index, f->table, f->flag));// + typeSizes[fld.type] * array1));
        if (val) {
            fieldset->fields[i]->fieldValue = val;
        }

#ifdef CONDOC_T3D_4
        // Datablock substitution fields.
        if (((SimDataBlock*)object)->fieldHasSubstitution(f->pFieldname))
        {
            fieldset->fields[i]->fieldSubstitution = ((SimDataBlock*)object)->getSubstitution(f->pFieldname, j);
            fieldset->fields[i]->isSubstitution = !fieldset->fields[i]->fieldSubstitution.empty();
        }
#endif // CONDOC_T3D_V4
    }
}


void TSDocClass::initStaticFields(AbstractClassRep* object, bool filterParentFields) {
//}
//void TSDocClass::initStaticFields(SimObject* obj, bool filterParentFields) {
    
    //bool filterParentFields = true;
    char expandedBuffer[4096];
    char valBuffer[4096];   // Can probably be smaller.

    //AbstractClassRep* object = obj->getClassRep();
    AbstractClassRep* parentRep = object->getParentClass();

    // Deal with the classRep (to get members)...    
    /*
    AbstractClassRep::FieldList emptyList;
    AbstractClassRep::FieldList* parentList = &emptyList;
    //AbstractClassRep::FieldList* fieldList = &emptyList;    
    if (parentRep)
        parentList = &(parentRep->mFieldList);
    */
    Con::printf("Static Fields (%s/%s):",object->getClassName(), parentRep->getClassName());

    const AbstractClassRep::FieldList& list = object->mFieldList;
    Vector<const AbstractClassRep::Field*> flist(__FILE__, __LINE__);    
    //Namespace * lns = object->getNamespace();
    //Con::printf("Namespace:%s",lns->getName() );
    Con::printf("Namespace:%s/%s", object->getNameSpace()->getName(), parentRep->getNameSpace()->getName());
    
    for (U32 i = 0; i < list.size(); i++) {
        if (filterParentFields && parentRep){            
            if (parentRep->findField((list)[i].pFieldname)) {
                continue;
            }
        }        
        flist.push_back(&list[i]);
    }

    // Todo: fix sort routine? Sorting by name may screw up grouping.
    //dQsort(flist.address(), flist.size(), sizeof(AbstractClassRep::Field*), compareFields);

    string fieldGroup = "";
    for (Vector<const AbstractClassRep::Field*>::iterator itr = flist.begin(); itr != flist.end(); itr++)
    {
        const AbstractClassRep::Field* f = *itr;
        bool isDeprecated = (f->type == AbstractClassRep::DeprecatedFieldType);

        // Handle special field types.
        if (f->type == AbstractClassRep::StartGroupFieldType) {
            fieldGroup = f->pGroupname && f->pGroupname[0] ? f->pGroupname : "";
            continue;
        }
        else if (f->type == AbstractClassRep::EndGroupFieldType) {
            fieldGroup = "";
            continue;
        }
        else if (f->type >= AbstractClassRep::ARCFirstCustomField && f->type != AbstractClassRep::DeprecatedFieldType)
            continue;

        for (U32 j = 0; S32(j) < f->elementCount; j++)
        {
            TSDocField* dfield = new TSDocField();

            // [neo, 07/05/2007 - #3000]
            // Some objects use dummy vars and projected fields so make sure we call the get functions
            //const char* val = (*f->getDataFn)(object, Con::getData(f->type, (void*)(((const char*)object) + f->offset), j, f->table, f->flag));// + typeSizes[fld.type] * array1));
            const char* val = "";
            ConsoleBaseType* conType = ConsoleBaseType::getType(f->type);
            const char* conTypeName = "<unknown>";
            if (conType)
                conTypeName = conType->getTypeClassName();

            if (!val /*|| !*val*/)
                continue;
            if (f->elementCount == 1)
            {
                //dSprintf(expandedBuffer, sizeof(expandedBuffer), "  %s %s = \"", conTypeName, f->pFieldname);
                dfield->isArrayElement = false;
                dfield->index = 0;
            }
            else {
                //dSprintf(expandedBuffer, sizeof(expandedBuffer), "  %s %s[ %d ] = \"", conTypeName, f->pFieldname, j);
                dfield->isArrayElement = true;
                dfield->index = j;
            }
            //expandEscape(expandedBuffer + dStrlen(expandedBuffer), val);
            //dSprintf(expandedBuffer, sizeof(expandedBuffer), "%s\"", expandedBuffer);
            //Con::printf("%s", expandedBuffer);

            expandEscape(valBuffer, val);
            dfield->fieldName = f->pFieldname ? f->pFieldname : "<<empty_field>>";
            dfield->fieldType = conTypeName;
            dfield->fieldValue = valBuffer;
            dfield->isDepreciated = isDeprecated;
            dfield->fieldGroup = f->pGroupname && f->pGroupname[0] ? f->pGroupname : "";
            dfield->fieldGroup = dfield->fieldGroup == "" ? fieldGroup : dfield->fieldGroup;

            dfield->usage->description = f->pFieldDocs && f->pFieldDocs[0] ? f->pFieldDocs : "";
            dfield->usage->parseUsage(dfield->usage->description);

            // See if the field is an object.
            SimObject* ckobj = Sim::findObject(dfield->fieldName.c_str());
            if (ckobj)
            {
                dfield->isValueObject = true;
                dfield->fieldValueClass = ckobj->getClassName();
            }

            //dfield->fieldText = dfield->applyTemplate(fieldTemplate);  // Apply the generic field template last ...template
            TSDocTemplate ttpl;
            ttpl.setDefault();
            dfield->fieldText = dfield->applyTemplate(&ttpl);  // Apply the generic field template last ...template 
            // Add this field to the fieldset
            fieldset->set(dfield);
        }
    }
}

void TSDocClass::initDynamicFields(SimObject* object) {
    char expandedBuffer[4096];
    char valBuffer[4096];   // Can probably be smaller.
    
    if (object->getFieldDictionary()) {
        SimFieldDictionary* dict = object->getFieldDictionary();
        //object->getFieldDictionary()->printFields(object);
        for (SimFieldDictionaryIterator itr(dict); *itr; ++itr)
        {
            // Fetch Field Entry.
            SimFieldDictionary::Entry* ent = NULL;
            ent = *itr;

            TSDocField* dfield = new TSDocField();
            dfield->isDynamic = true;
            dfield->fieldName = ent->slotName ? ent->slotName : "<empty_field>";

            string tyna = ent->type ? ent->type->getTypeName() : "";            
            dfield->fieldType = tyna.empty() ? "var" : tyna;

            if (ent->value) {
                expandEscape(valBuffer, ent->value);
                dfield->fieldValue = valBuffer;
            }
            else {
                dfield->fieldValue = "";
            }
            // Stick all the fields into a summary template ... and stick that into a buffer
            expandEscape(expandedBuffer, dfield->applyTemplate(&docTemplate).c_str());
            dfield->fieldText = expandedBuffer;
            // Add this field to the fieldset
            fieldset->set(dfield);
        }
    }
}

// ------------------------------ 

void TSDocClass::initMethods(SimObject* object, bool filterParents, bool getScript, bool getEngine)
{
    Namespace* ns = object->getNamespace();
    initMethods(ns, filterParents, getScript, getEngine);
}

void TSDocClass::initMethods(Namespace* ns, bool filterParents, bool getScript, bool getEngine)
{    
    char expandedBuffer[4096];
    char valBuffer[4096];   // Can probably be smaller.
    
    string inhstr = "";
    if (!methodInheritTo.empty()) {
        int pos = inheritance.find(methodInheritTo);
        if (pos != string::npos) {
            inhstr = inheritance.substr(0, pos + methodInheritTo.size());
            inhstr = (string)ns->mClassRep->getClassName() + " " + inhstr;
        }
    }
    Con::printf("inherit:%s",inhstr);
    VectorPtr<Namespace::Entry*> vec(__FILE__, __LINE__);
    if (ns)
        ns->getEntryList(&vec);

    bool sawCBs = false;
    for (Vector<Namespace::Entry*>::iterator j = vec.begin(); j != vec.end(); j++)
    {
        Namespace::Entry* e = *j;
        if (!empty(inhstr) && inhstr.find(e->mNamespace->getName()) == string::npos) {
            continue;
        }
        if (filterParents && docClassName != e->mNamespace->getName() ) {
            continue;
        }
        S32 eType = e->mType;
        const char* funcName = e->mFunctionName;
        if ((eType == Namespace::Entry::ConsoleFunctionType) && !getScript)
            continue;
        if ((eType != Namespace::Entry::ConsoleFunctionType) && !getEngine)
            continue;

        TSDocMethod* method = new TSDocMethod();
        method->methodBaseClass = docClassName;             // For local template use.
        method->usage->brief = e->getBriefDescription();
        method->usage->description = e->getDocString();
        method->usage->full = e->mUsage ? e->mUsage : "";
        method->minArg = e->mMinArgs;
        method->maxArg = e->mMaxArgs;
        method->methodPrototype = e->getPrototypeString();
        if (e->mCode) {
            method->isScript = true;
            method->isVirtual = false;
            method->scriptLinenumber = (int)e->mFunctionLineNumber;
            method->scriptFile = e->mCode->name ? e->mCode->name : "";            
        } else {
            method->isScript = false;
            method->isVirtual = true;
        }
        method->methodNamespace = e->mNamespace->getName();
        method->methodPackage = e->mPackage ? e->mPackage : "";
        //method->isVirtual = false;

        if (e->mHeader && e->mHeader->mReturnString)
        {
            method->returnType = sGetDocTypeString(e->mHeader->mReturnString);
        }
        else
        {
            string tt;
            switch (e->mType)
            {
            case Namespace::Entry::StringCallbackType:
                tt = "string";
                break;
            case Namespace::Entry::IntCallbackType:
                tt = "int";
                break;
            case Namespace::Entry::FloatCallbackType:
                tt = "float";
                break;
            case Namespace::Entry::VoidCallbackType:
                tt = "void";
                break;
            case Namespace::Entry::BoolCallbackType:
                tt = "bool";
                break;
            case Namespace::Entry::ScriptCallbackType:
                tt = "<callback>";
                break;
            }
            method->returnType = tt;
        }
        // Decide what name to use
        if (e->mType == Namespace::Entry::ScriptCallbackType)
        {
            method->methodName = e->cb.mCallbackName;
            method->isCallback = true;
        }
        else {
            method->methodName = e->mFunctionName;
            method->isCallback = false;
        }
        // Try to parse out the arguments
        if (e->mHeader)
        {
            // Parse out the argument list string supplied with the extended
            // function header and add default arguments as we go.            
            Vector< String > argList;
            Vector< String > defaultArgList;

            sParseList(e->mHeader->mArgString, argList);
            sParseList(e->mHeader->mDefaultArgString, defaultArgList);

            const U32 numArgs = argList.size();
            const U32 numDefaultArgs = defaultArgList.size();

            const U32 firstDefaultArgIndex = numArgs - numDefaultArgs;

            for (U32 i = 0; i < numArgs; ++i)
            {
                // Create a new argument
                TSDocArg* arg = new TSDocArg();
                // Add type and name.

                String name;
                String type;
                sGetArgNameAndType(argList[i], type, name);

                arg->argName = name;
                arg->argTypeNative = type;
                arg->argType = sGetDocTypeString(type);

                if (i >= firstDefaultArgIndex)
                {
                    arg->argDefault = defaultArgList[i - firstDefaultArgIndex];
                    arg->argRequired = false;
                }
                else
                    arg->argRequired = true;

                method->args->setArg(arg);
            }
        }
        else
        {
            // No extended function header.  Try to parse out the argument
            // list from the usage string.
            const char* argListStart;
            const char* argListEnd;
            string arglist = "";
            if (sFindArgumentListSubstring(e->mUsage, argListStart, argListEnd)) {


                arglist = argListStart, argListEnd - argListStart;
                if (!arglist.empty()) {
                    arglist = trim(getStringBetween(arglist, "(", ")"));
                    if (!arglist.empty()) {
                        if (arglist.find(",") != string::npos) {
                            size_t pos = 0;
                            string arg;
                            while ((pos = arglist.find(",")) != string::npos) {
                                arg = arglist.substr(0, pos);
                                arglist.erase(0, pos + 1);

                                string name = "<unnamed>";
                                string type = "";
                                string default = "";

                                arg = trim(arg);
                                size_t found = arg.find(" ");
                                if (found != string::npos) {
                                    name = arg.substr(found + 1);
                                    type = arg.substr(0, found);
                                }
                                found = name.find("=");
                                if (found != string::npos) {
                                    default = name.substr(found + 1);
                                    name = name.substr(0, found);
                                }
                                bool isRequired = default.empty();
                                TSDocArg* argo = new TSDocArg();
                                argo->argName = name;
                                argo->argType = type;
                                argo->argDefault = default;
                                argo->argRequired = default.empty();
                                method->args->setArg(argo);
                            }
                        }
                        else {
                            string name = "<unnamed>";
                            string type = "";
                            string default = "";

                            string arg = trim(arglist);
                            size_t found = arg.find(" ");
                            if (found != string::npos) {
                                name = arg.substr(found + 1);
                                type = arg.substr(0, found);
                            }
                            found = name.find("=");
                            if (found != string::npos) {
                                default = name.substr(found + 1);
                                name = name.substr(0, found);
                            }
                            bool isRequired = default.empty();
                            TSDocArg* argo = new TSDocArg();
                            argo->argName = name;
                            //argo->argType = type;
                            argo->argTypeNative = type;
                            argo->argType = sGetDocTypeString(type.c_str());
                            argo->argDefault = default;
                            argo->argRequired = default.empty();
                            method->args->setArg(argo);
                        }
                    }
                }
                // parse and add to method.
            }
            else if (e->mType == Namespace::Entry::ConsoleFunctionType && e->mCode)
            {
                // This isn't correct but the nonsense console stuff is set up such that all
                // functions that have no function bodies are keyed to offset 0 to indicate "no code."
                // This loses the association with the original function definition so we can't really
                // tell here what the actual prototype is except if we searched though the entire opcode
                // stream for the corresponding OP_FUNC_DECL (which would require dealing with the
                // variable-size instructions).

                if (e->mFunctionOffset)
                {
                    U32 fnArgc = e->mCode->code[e->mFunctionOffset + 5];
                    for (U32 i = 0; i < fnArgc; ++i)
                    {
                        StringTableEntry var = CodeToSTE(e->mCode->code, e->mFunctionOffset + (i * 2) + 6);
                        string type = "string";
                        string val = "";
                        // Try to capture junked parameters
                        if (var[0])
                            val = var + 1;
                        else
                            val = "JUNK";

                        TSDocArg* arg = new TSDocArg();
                        arg->argName = var;
                        arg->argType = type;
                        arg->argDefault = val;
                        arg->argRequired = arg->argDefault.empty();
                        method->args->setArg(arg);
                    }
                }
                else {
                    // last ditch effort to find args failed ... do something?
                }
            }
        }
        // TODO: args are parsed ... this would be where to back-update 
        // documentation for args from method usage.
        if (!method->scriptFile.empty()) {
            int i = 1;
        }
        methodset->set(method);
    }
    // Figure out class documentation location

}

// ------------------------------ 

void TSDocClass::testObject() {
    docClassName = "TestClass"; //object->getClassName();
    docClassParent = "TestParent"; //object->getSuperClassNamespace();

    TSDocUsage* usage2 = new TSDocUsage();
    usage2->brief = "Class Object Brief";
    usage2->description = "class description";
    usage2->formatted = "class description formatted";
    usage2->examples.clear();
    usage2->examples.push_back("Class Example 1\n");
    usage2->examples.push_back("Class Example 2\n");

    usage->set(usage2);

    for (int j = 0; j < 5; j++) {
        //TSDocMethod* method = new TSDocMethod();
        //method->methodName = "yes a method.";        
        TSDocMethod* m = getMethod();
        m->methodName = "MyMethod_" + to_string(j);
        Con::printf("method0: %s", m->methodName);
/*
        m->methodName = "MyMethod_" + to_string(j);
        m->returnType = "void";
        Con::printf("method0: %s", m->methodName);
        for (int k = 0; k < 3; k++) {
            Con::printf("method1: %s", method->methodName);
            TSDocArg* arg = new TSDocArg();
            arg->argName = "arg_" + to_string(j) + "_" + to_string(k);
            arg->argType = "argType_" + to_string(j) + "_" + to_string(k);
            arg->usage->brief = "Testing testing ....";
            arg->testUsage();
            TSDocUsage* usage = new TSDocUsage();
            usage->brief = "my brief";
            usage->description = "description";
            usage->formatted = "formatted";
            usage->examples.push_back("Example(" + to_string(j) + "_" + to_string(k) + ") 1\n");
            usage->examples.push_back("Example(" + to_string(j) + "_" + to_string(k) + ") 2\n");
            usage->examples.push_back("Example(" + to_string(j) + "_" + to_string(k) + ") 3\n");
            //processor.processDirectory();
            arg->usage->set(usage);
            arg->testUsage();
            method->args->setArg(arg);
            //cout << "arg:" << method->args->getArg("arg")->usage->examples[0] << endl;            
        }
        methods->set(method);
*/
    }

    /*
    for (int j = 0; j < 5; j++) {
        Con::printf("method: %s", methods->get(j)->methodName);
        Con::printf("method: %s", methods->methods[j]->methodName);        
    }
    */
}

// --------------------------------------------------

string TSDocClass::applyTemplate(TSDocTemplate* tpl)
{
    string ctpl = tpl->classTpl;
    string rv = applyTemplate(ctpl, tpl);
    return rv;
}
string TSDocClass::applyTemplate(string body, TSDocTemplate* tpl)
{
    templatedText = body;
    if (templatedText.find("{{fieldset}}") != string::npos) {
        string fldset = fieldset->applyTemplate(tpl);
        ReplaceString(templatedText, "{{fieldset}}", fldset);
    }
    if (templatedText.find("{{methodset}}") != string::npos) {
        string mthdset = methodset->applyTemplate(tpl);
        ReplaceString(templatedText, "{{methodset}}", mthdset);
    }
    // Do class-level macro replacements.
    ReplaceString(templatedText, "{{cls_name}}", docClassName);
    ReplaceString(templatedText, "{{cls_parent}}", docClassParent);
    ReplaceString(templatedText, "{{cls_group}}", group);
    ReplaceString(templatedText, "{{cls_namespace}}", docClassNamespace);
    ReplaceString(templatedText, "{{cls_package}}", docClassPackage);  
    ReplaceString(templatedText, "{{cls_file}}", scriptFile);
    ReplaceString(templatedText, "{{cls_lineno}}", to_string(scriptLine));
    ReplaceString(templatedText, "{{cls_isscript}}", isScript ? "1" : "0");
    ReplaceString(templatedText, "{{cls_inheritance}}", inheritance);

    templatedText = usage->applyTemplate(templatedText, tpl->fmtEOL); // Fill in any documentation fields in the template.
    // Process usage blocks based on defined usage template.
    if (templatedText.find("{{usage}}") != string::npos) {
        string ubod = usage->applyTemplate(tpl->classUTpl, tpl->fmtEOL); // Apply usage template to catch any direct usage fields
        ReplaceString(templatedText, "{{usage}}", ubod);
    }
    //Con::printf("%s", templatedText.c_str());
    return templatedText;
}




// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

TSDocArgSet::TSDocArgSet() {}
TSDocArgSet::~TSDocArgSet()
{
    clear();
}

void TSDocArgSet::clear() {
    for (std::vector<TSDocArg*>::iterator i = args.begin(); i != args.end(); ++i)
    {
        delete (*i);
        *i = NULL;
    }
    args.clear();
};

TSDocArg* TSDocArgSet::getArg(string name)
{
    return getArg(argIndex(name));
}

TSDocArg* TSDocArgSet::getArg(int index)
{
    if (index >= 0 && index < args.size())
        return args[index];
    return NULL;
}

bool TSDocArgSet::argExists(string name) {
    return (argIndex(name) >= 0);
}

int TSDocArgSet::argIndex(string name)
{
    int c = 0;
    for (int i = 0; i < args.size(); ++i)
    {
        if (args[i]->argName == name) {
            return i;
        }
    }
    return -1;
}

void TSDocArgSet::setArg(TSDocArg* argObj)
{
    if (argObj->argName.empty()) return;

    int aidx = argIndex(argObj->argName);
    if (aidx == -1) {
        TSDocArg* narg = new TSDocArg();
        narg->set(argObj);
        args.push_back(narg);
    }
    else {
        args[aidx]->set(argObj);
    }
}

void TSDocArgSet::setArg(string name, string type, TSDocUsage* usageObj) {
    if (name.empty()) return;
    TSDocArg* narg = new TSDocArg();
    narg->argName = name;
    narg->argType = type;
    narg->usage->set(usageObj);
    setArg(narg);
}

bool TSDocArgSet::removeArg(string name)
{
    return removeArg(argIndex(name));
}

bool TSDocArgSet::removeArg(int idx)
{
    if (idx < 0) return false;
    if (args.size() > idx)
    {
        delete args[idx];   // delete the object
        args.erase(args.begin() + idx); // clear the pointer
        return true;
    }
    return false;
};
// --------------------------------------------------
///
/// Apply the indicated template to all fields in the set.
/// 
/// param fldtpl - Text template defining output format and content.
/// param delim - text to insert between each field output.
/// param eol - text to insert at the end of each field output.
/// param padding - text string to insert before each field output.
/// 
//string TSDocFieldSet::fieldsetTemplate(string fldtpl, string delim, string eol, string pad)
string TSDocArgSet::argsetTemplate(TSDocTemplate* tpl)
{
    string rv = "";
    string c = " ";
    string pad = ""; // tpl->argsetPad
    string eol = ""; // tpl->argsetEol
    for (int i = 0; i < args.size(); i++) {
        rv += pad + c + args[i]->applyTemplate(tpl) + eol;
        c = tpl->argsetDelim;
    }
    return rv;
}

///
/// Apply a fieldset template to this object and return the resulting text.
/// 
/// Generates templated output for all args contained in this set as
/// defined in *tpl and integrates this output with the base text
/// passed in body.
/// 
string TSDocArgSet::applyTemplate(string body, TSDocTemplate* tpl)
{
    templatedText = body;
    string args = argsetTemplate(tpl); // Get the templated fieldset output

    if (body.find("{{this}}") != string::npos) {
        string c = "";
        if (args.size() > 0) {
            c = ",";
        }
        ReplaceString(templatedText, "{{this}}", ("%this"+c) );
    }

    ReplaceString(templatedText, "{{argset}}", args);
    return templatedText;
}
string TSDocArgSet::applyTemplate(TSDocTemplate* tpl)
{
    string stpl = tpl->argsetTpl;    // Default to the proper template string from templateset.
    return applyTemplate(stpl, tpl);
}



// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

TSDocMethodSet::TSDocMethodSet() {
    templatedText = "";
}
TSDocMethodSet::~TSDocMethodSet()
{
    clear();
}

void TSDocMethodSet::clear() {
    for (std::vector<TSDocMethod*>::iterator i = methods.begin(); i != methods.end(); ++i)
    {
        delete (*i);
        *i = NULL;
    }
    methods.clear();
};

TSDocMethod* TSDocMethodSet::get(string name)
{
    return get(getIndex(name));
}

TSDocMethod* TSDocMethodSet::get(int index)
{
    if (index >= 0 && index < methods.size())
        return methods[index];
    return NULL;
}

bool TSDocMethodSet::getExists(string name) {
    return (getIndex(name) >= 0);
}

int TSDocMethodSet::getIndex(string name)
{
    int c = 0;
    for (int i = 0; i < methods.size(); ++i)
    {
        if (methods[i]->methodName == name) {
            return i;
        }
    }
    return -1;
}

void TSDocMethodSet::set(TSDocMethod* obj)
{
    if (obj->methodName.empty()) return;

    int aidx = getIndex(obj->methodName);
    if (aidx == -1) {
        TSDocMethod* nobj = new TSDocMethod();
        nobj->set(obj);
        methods.push_back(nobj);
    }
    else {
        methods[aidx]->set(obj);
    }
}

bool TSDocMethodSet::remove(string name)
{
    return remove(getIndex(name));
}

bool TSDocMethodSet::remove(int idx)
{
    if (idx < 0) return false;
    if (methods.size() > idx)
    {
        delete methods[idx];   // delete the object
        methods.erase(methods.begin() + idx); // clear the pointer
        return true;
    }
    return false;
};
// --------------------------------------------------
///
/// Apply the indicated template to all fields in the set.
/// 
/// param fldtpl - Text template defining output format and content.
/// param delim - text to insert between each field output.
/// param eol - text to insert at the end of each field output.
/// param padding - text string to insert before each field output.
/// 
//string TSDocFieldSet::fieldsetTemplate(string fldtpl, string delim, string eol, string pad)
string TSDocMethodSet::methodsetTemplate(TSDocTemplate* tpl)
{
    string rv = "";
    string c = " ";
    for (int i = 0; i < methods.size(); i++) {
        rv += tpl->methodsetPad + c + methods[i]->applyTemplate(tpl) + tpl->methodsetEol;
        c = tpl->methodsetDelim;
    }
    return rv;
}

///
/// Apply a fieldset template to this object and return the resulting text.
/// 
/// Generates templated output for all feilds contained in this set using
/// the fieldtpl and integrates this output with the fieldset template
/// passed in tpl.
/// 
string TSDocMethodSet::applyTemplate(string body, TSDocTemplate* tpl)
{
    templatedText = body;
    string fields = methodsetTemplate(tpl); // Get the templated fieldset output
    ReplaceString(templatedText, "{{methodset}}", fields);   // Update
    return templatedText;
}
string TSDocMethodSet::applyTemplate(TSDocTemplate* tpl)
{
    string stpl = tpl->methodsetTpl;    // Default to the FS template string from templateset.
    return applyTemplate(stpl, tpl);
}


// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------


TSDocFieldSet::TSDocFieldSet() {
    templatedText = "";
}
TSDocFieldSet::~TSDocFieldSet()
{
    clear();
}

void TSDocFieldSet::clear() {
    for (std::vector<TSDocField*>::iterator i = fields.begin(); i != fields.end(); ++i)
    {
        delete (*i);
        *i = NULL;
    }
    fields.clear();
};

TSDocField* TSDocFieldSet::get(string name)
{
    return get(getIndex(name));
}
    
TSDocField* TSDocFieldSet::get(int index)
{
    if (index >= 0 && index < fields.size())
        return fields[index];
    return NULL;
}

bool TSDocFieldSet::getExists(string name) {
    return (getIndex(name) >= 0);
}

int TSDocFieldSet::getIndex(string name)
{
    int c = 0;
    for (int i = 0; i < fields.size(); ++i)
    {
        if (fields[i]->fieldName == name) {
            return i;
        }
    }
    return -1;
}

void TSDocFieldSet::set(TSDocField* obj)
{
    if (obj->fieldName.empty()) return;

    int aidx = getIndex(obj->fieldName);
    if (aidx == -1) {
        TSDocField* nobj = new TSDocField();
        nobj->set(obj);
        fields.push_back(nobj);
    }
    else {
        fields[aidx]->set(obj);
    }
}

bool TSDocFieldSet::remove(string name)
{
    return remove(getIndex(name));
}

bool TSDocFieldSet::remove(int idx)
{
    if (idx < 0) return false;
    if (fields.size() > idx)
    {
        delete fields[idx];   // delete the object
        fields.erase(fields.begin() + idx); // clear the pointer
        return true;
    }
    return false;
};

///
/// Apply the indicated template to all fields in the set.
/// 
/// param fldtpl - Text template defining output format and content.
/// param delim - text to insert between each field output.
/// param eol - text to insert at the end of each field output.
/// param padding - text string to insert before each field output.
/// 
//string TSDocFieldSet::fieldsetTemplate(string fldtpl, string delim, string eol, string pad)
string TSDocFieldSet::fieldsetTemplate(TSDocTemplate* tpl)
{
    string rv = "";
    string c = " ";
    for (int i = 0; i < fields.size(); i++) {
        rv += tpl->fieldsetPad + c + fields[i]->applyTemplate(tpl) + tpl->fieldsetEol;
        c = tpl->fieldsetDelim;
    }
    return rv;
}

///
/// Apply a fieldset template to this object and return the resulting text.
/// 
/// Generates templated output for all feilds contained in this set using
/// the fieldtpl and integrates this output with the fieldset template
/// passed in tpl.
/// 
string TSDocFieldSet::applyTemplate(string body, TSDocTemplate* tpl)
{    
    string t = body;    
    string flds = fieldsetTemplate(tpl); // Get the templated fieldset output

    ReplaceString(t, "{{fieldset}}", flds);   // Update 

    templatedText = t;
    return templatedText;
}
string TSDocFieldSet::applyTemplate(TSDocTemplate* tpl)
{
    string fstpl = tpl->fieldsetTpl;    // Default to the FS template string from templateset.
    return applyTemplate(fstpl, tpl);
}
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------


TSDocClassSet::TSDocClassSet() {
    templatedText = "";
}
TSDocClassSet::~TSDocClassSet()
{
    clear();
}

void TSDocClassSet::clear() {
    for (std::vector<TSDocClass*>::iterator i = classes.begin(); i != classes.end(); ++i)
    {
        delete (*i);
        *i = NULL;
    }
    classes.clear();
};

TSDocClass* TSDocClassSet::get(string name)
{
    return get(getIndex(name));
}

TSDocClass* TSDocClassSet::get(int index)
{
    if (index >= 0 && index < classes.size())
        return classes[index];
    return NULL;
}

bool TSDocClassSet::getExists(string name) {
    return (getIndex(name) >= 0);
}

int TSDocClassSet::getIndex(string name)
{
    int c = 0;
    for (int i = 0; i < classes.size(); ++i)
    {
        if (classes[i]->docClassName == name) {
            return i;
        }
    }
    return -1;
}

void TSDocClassSet::set(TSDocClass* obj)
{
    if (obj->docClassName.empty()) return;

    int aidx = getIndex(obj->docClassName);
    if (aidx == -1) {
        TSDocClass* nobj = new TSDocClass();
        nobj->set(obj);
        classes.push_back(nobj);
    }
    else {
        classes[aidx]->set(obj);
    }
}

bool TSDocClassSet::remove(string name)
{
    return remove(getIndex(name));
}

bool TSDocClassSet::remove(int idx)
{
    if (idx < 0) return false;
    if (classes.size() > idx)
    {
        delete classes[idx];   // delete the object
        classes.erase(classes.begin() + idx); // clear the pointer
        return true;
    }
    return false;
};

void TSDocClassSet::initSystemClasses(bool getScript, bool getEngine)
{
    VectorPtr<Namespace*> vec;
    Namespace::trashCache();
    vec.reserve(1024);

    // We use mHashSequence to mark if we have traversed...
    // so mark all as zero to start.
    for (Namespace* walk = Namespace::mNamespaceList; walk; walk = walk->mNext)
        walk->mHashSequence = 0;

    for (Namespace* walk = Namespace::mNamespaceList; walk; walk = walk->mNext)
    {
        VectorPtr<Namespace*> stack;
        stack.reserve(1024);

        // Get all the parents of this namespace... (and mark them as we go)
        Namespace* parentWalk = walk;
        while (parentWalk)
        {
            if (parentWalk->mHashSequence != 0)
                break;
            if (parentWalk->mPackage == 0)
            {
                parentWalk->mHashSequence = 1;   // Mark as traversed.
                stack.push_back(parentWalk);
            }
            parentWalk = parentWalk->mParent;
        }

        // Load stack into our results vector.
        while (stack.size())
        {
            vec.push_back(stack[stack.size() - 1]);
            stack.pop_back();
        }
    }

    // Go through previously discovered classes
    U32 i;
    for (i = 0; i < vec.size(); i++)
    {
        Namespace* ns;
        ns = vec[i];

        const char* className = vec[i]->mName;
        const char* superClassName = vec[i]->mParent ? vec[i]->mParent->mName : NULL;

        // Skip the global namespace, that gets dealt with in dumpFunctions
        if (!className) continue;

        // If we're just dumping script functions, then we don't want to dump
        // a class that only contains script functions. So, we iterate over all
        // the functions.
        if (!getScript)
        {
            bool found = false;
            for (Namespace::Entry* ewalk = vec[i]->mEntryList; ewalk; ewalk = ewalk->mNext)
            {
                if (ewalk->mType != Namespace::Entry::ConsoleFunctionType)
                {
                    found = true;
                    break;
                }
            }

            // If we don't have engine functions and the namespace name
            // doesn't match the class name... then its a script class.
            if (!found && !vec[i]->isClass())
                continue;
        }

        // And we do the same for engine functions.
        if (!getEngine)
        {
            bool found = false;
            for (Namespace::Entry* ewalk = vec[i]->mEntryList; ewalk; ewalk = ewalk->mNext)
            {
                if (ewalk->mType == Namespace::Entry::ConsoleFunctionType)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                continue;
        }

        // If we hit a class with no members and no classRep, do clever filtering.
        if (vec[i]->mEntryList == NULL && vec[i]->mClassRep == NULL)
        {
            // Print out a short stub so we get a proper class hierarchy.
            if (superClassName) { // Filter hack; we don't want non-inheriting classes...
                //printClassHeader(NULL, className, superClassName, true);
                //printClassFooter();
                // Create an empty TSDocClass object for this entry.

            }
            continue;
        }

        if (!ns->mClassRep && ns->mParent) {
            // Script-created objects?
//            Con::printf("Script Object (no rep): %s/%s", vec[i]->mName, ns->mParent->mName);
        } else  if (!ns->mClassRep && !ns->mParent) {
            // Script-created objects w/o superclass (actions?)
            Con::printf("Null Class (no parent): %s", vec[i]->mName);
        }

        // Create a TSDocClass object to manage class data.
        TSDocClass* dclass = new TSDocClass();
        dclass->initClass(vec[i],getScript,getEngine);

        if (dclass->docClassName.empty() || dclass->docClassName == "") {
            //Con::printf("Null Class?: %s", vec[i]->mName);
        }
        else {
            //Con::printf("Class: %s", dclass->docClassName.c_str());
        }

        this->classes.push_back(dclass);        
    }
}

string TSDocClassSet::classsetTemplate(TSDocTemplate* tpl)
{
    string rv = "";
    string c = " ";
    for (int i = 0; i < classes.size(); i++) {
        rv += tpl->classsetPad + c + classes[i]->applyTemplate(tpl) + tpl->classsetEol;
        c = tpl->classsetDelim;
    }
    return rv;
}

string TSDocClassSet::applyTemplate(string body, TSDocTemplate* tpl)
{
    string t = body;
    string clslst = classsetTemplate(tpl); // Get the templated fieldset output

    ReplaceString(t, "{{classset}}", clslst);   // Update 

    templatedText = t;
    return templatedText;
}

string TSDocClassSet::applyTemplate(TSDocTemplate* tpl)
{
    string body = tpl->classsetTpl;    // Default to the FS template string from templateset.
    return applyTemplate(body, tpl);
}

// ---- ********** ---------- ****** ---------
// ---- ********** ---------- ****** ---------
// ---- ********** ---------- ****** ---------


void TSDocTemplate::setDefault() {
    templateName = "default";
    templateVersion = "1";
    fmtEOL = false;
    classTpl = "{{name}}";
    classUTpl = "{{brief}}";
    fieldTpl = "{{type}} {{name}}{{idx}} = \"{{value}}\"";
    fieldUTpl = "{{brief}}";
    fieldsetTpl = "";
    fieldsetEol = "";
    fieldsetDelim = "";
    fieldsetPad = "";
    methodTpl = "{prototype}";
    methodUTpl = "{{brief}}";
    methodsetTpl = "";
    methodsetEol = "";
    methodsetDelim = "";
    methodsetPad = "";
    argTpl = "";
    argsetTpl = "";
    argsetEol = "";
    argsetDelim = "";
    argsetPad = "";
}


void TSDocTemplate::setTemplate(TSDocTemplate* tpl) {
    if (!tpl) return;
    templateName = tpl->templateName;
    templateVersion = tpl->templateVersion;
    fmtEOL = tpl->fmtEOL;
    classTpl = tpl->classTpl;
    classUTpl = tpl->classUTpl;
    fieldTpl = tpl->fieldTpl;
    fieldUTpl = tpl->fieldUTpl;
    fieldsetTpl = tpl->fieldsetTpl;
    fieldsetEol = tpl->fieldsetEol;
    fieldsetDelim = tpl->fieldsetDelim;
    fieldsetPad = tpl->fieldsetPad;
    methodTpl = tpl->methodTpl;
    methodUTpl = tpl->methodUTpl;
    methodsetTpl = tpl->methodsetTpl;
    methodsetEol = tpl->methodsetEol;
    methodsetDelim = tpl->methodsetDelim;
    methodsetPad = tpl->methodsetPad;
    argTpl = tpl->argTpl;
    argsetTpl = tpl->argsetTpl;
    argsetEol = tpl->argsetEol;
    argsetDelim = tpl->argsetDelim;
    argsetPad = tpl->argsetPad;
}


namespace {
    // For some reason obj->getDataField() is failing on lookups with
    // console-created ScriptObjects. This is a quick dirty dynamic
    // field lookup function.
    const char* tpl_lookup(const char* fieldname, ScriptObject* obj) {
        if (!obj)return "";
        SimFieldDictionary* dict = obj->getFieldDictionary();
        if (!dict)return "";
        for (SimFieldDictionaryIterator itr(dict); *itr; ++itr)
        {
            SimFieldDictionary::Entry* ent = NULL;
            ent = *itr;
            const char* sname = NULL;
            sname = ent->slotName;
            if (sname)
                if (stricmp(sname, fieldname) == 0) {
                    return ent->value;
                }
        }
        return "";
    }
}
bool TSDocTemplate::setTemplate(ScriptObject* tpl) {
    if (!tpl) {
        return false;
    }
    templateName = tpl_lookup("templateName", tpl);
    templateVersion = tpl_lookup("templateVersion", tpl);
    fmtEOL = strcasecmp(tpl_lookup("formatEOL", tpl),"1") == 0 ? true : false;

    classTpl = tpl_lookup("classTemplate", tpl);
    classUTpl = tpl_lookup("classUsageTemplate", tpl);
    classsetTpl = tpl_lookup("classsetTemplate", tpl);
    classsetEol = tpl_lookup("classsetEol", tpl);
    classsetDelim = tpl_lookup("classsetDelim", tpl);
    classsetPad = tpl_lookup("classsetPad", tpl);

    fieldTpl = tpl_lookup("fieldTemplate", tpl);
    fieldUTpl = tpl_lookup("fieldUsageTemplate", tpl);
    fieldsetTpl = tpl_lookup("fieldsetTemplate", tpl);
    fieldsetEol = tpl_lookup("fieldsetEol", tpl);
    fieldsetDelim = tpl_lookup("fieldsetDelim", tpl);
    fieldsetPad = tpl_lookup("fieldsetPad", tpl);

    methodTpl = tpl_lookup("methodTemplate", tpl);
    methodUTpl = tpl_lookup("methodUsageTemplate", tpl);
    methodsetTpl = tpl_lookup("methodsetTemplate", tpl);
    methodsetEol = tpl_lookup("methodsetEol", tpl);
    methodsetDelim = tpl_lookup("methodsetDelim", tpl);
    methodsetPad = tpl_lookup("methodsetPad", tpl);

    argTpl = tpl_lookup("argTemplate", tpl);
    argsetTpl = tpl_lookup("argsetTemplate", tpl);
    argsetEol = tpl_lookup("argsetEol", tpl);
    argsetDelim = tpl_lookup("argsetDelim", tpl);
    argsetPad = tpl_lookup("argsetPad", tpl);
/*
    string mystring = "bool myfunc(bool var1,string var2)";
    Con::printf("tpl:%s", mystring.c_str());
    string tst = "";    
    tst = getStringBetween(mystring, "(", ")");
    Con::printf("Tst1:%s", tst.c_str());
    tst = deleteBetween(mystring, "(", ")");
    Con::printf("Tst2:%s", tst.c_str());
*/
    //Con::printf("Template:%s\nf:%s", templateName.c_str(), fieldTpl.c_str());
    return true;
}
