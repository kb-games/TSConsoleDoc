// ============================================================
// Project            :  Full Game (afx mess)
// File               :  .\tools\docTests.cs
// Copyright          :  
// Author             :  kent
// Created on         :  Thursday, November 16, 2023 10:19 PM
//
// Editor             :  TorqueDev v. 1.3.8712.22056
//
// Description        :  
//                    :  
// ============================================================
new ScriptObject(JSON_Template){
	templateName = "TScript JSON Output";
	templateVersion = 1;
	
	classTemplate = "{\"class\":\"{{name}}\",\"parent\":\"{{parent}}\",\"file\":\"{{file}}\",\"lineno\":\"{{lineno}}\",\"fields\":\"{{fieldset}}\",\"methods\":\"{{methodset}}\" }";
	
	fieldTemplate = "{\"fieldName\":\"{{name}}\",\"fieldType\":\"{{type}}\",\"fieldGroup\":\"{{group}}\",\"isDynamic\":\"{{dynamic}}\",\"isArrayElement\":\"{{array}}\",\"index\":\"{{index}}\",\"isSubstitution\":\"{{issub}}\",\"fieldSubstitution\":\"{{sub}}\" }";
	fieldsetTemplate = "\"fieldset\":[\n{{fieldset}}] ";
	fieldsetEol = "\n";
	fieldsetSep = ",";
	fieldsetInd = "    ";

	methodTemplate = "{\"methodName\":\"{{name}}\",\"returnType\":\"{{type}}\",\"methodGroup\":\"{{group}}\",\"methodNamespace\":\"{{namespace}}\",\"methodPackage\":\"{{package}}\",\"minArg\":\"{{minarg}}\",\"maxArg\":\"{{maxArg}}\",\"scriptFile\":\"{{file}}\",\"scriptLine\":\"{{lineno}}\",\"isVirtual\":\"{{virtual}}\",\"isCallback\":\"{{callback}}\",\"args\":\"{{argset}}\"}";
	methodsetTemplate = "\"methodset\":[\n{{methodset}}] ";
	methodsetEol = "\n";
	methodsetSep = ",";
	methodsetInd = "    ";
	
	argTemplate = "{\"arg\":\"{{name}}\",\"type\":\"{{type}}\",\"default\":\"{{default}}\",\"required\":\"{{required}}\" }";
	argsetSep = ",";
};

function SysDoc::ftst(){
	%tst = SysDoc.getDocClass("MainMenuAppLogo");
	%grp = %tst.getFieldSet();
	foreach(%f in %grp){ echo("Field:" SPC %f.fieldText);}
	return %grp;
}

function SysDoc::mtst(){
	%tst = SysDoc.getDocClass("MainMenuAppLogo");
	%grp = %tst.getMethodSet();
	warn("method group ...");
	foreach(%m in %grp){ echo("Method:" SPC %m.methodName @ "::" @ %m.methodNamespace);}
	return %grp;
}

function SysDoc::ctst(){
	%tst = SysDoc.getDocClass("MainMenuAppLogo");
	%grp = %tst.getClassDoc();
	warn("method group ...");
	foreach(%m in %grp){ echo("Method:" SPC %m.methodName);}
	return %grp;
}


function docReport(%group){
	%st = %group.setType;
	if(%st $= "" ){
		warn("Not a valid ConDoc simgroup.");
		return;
	}
	echo("report type:" SPC %group.setType);
	echo("class:" SPC %group.docClass);
	if(%st $= "classdoc"){
		%cls = %group.getObject(0);
		%mset = %group.getObject(1);
		%fset = %group.getObject(2);
		%usage = %group.getObject(3);
		echo("Parent: " SPC %cls.docClassParent);
		echo("Methods: " SPC %mset.getCount());
		echo("Fields: " SPC %fset.getCount());
	}
}