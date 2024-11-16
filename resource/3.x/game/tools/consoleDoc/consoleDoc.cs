// ============================================================
// Project            :  Full Game
// File               :  .\tools\consoleDoc.cs
// Copyright          :  
// Author             :  Kent Butler
// Created on         :  Wednesday, November 15, 2023 3:21 PM
//
// Editor             :  TorqueDev v. 1.3.8712.22056
//
// Description        :  Example script-side implementation of
//                    :  console documentation classes.
// ============================================================

// create a global documentation object to reference and extend.
$SysDoc = new SysDocObject(SysDoc);
//datablock SysDocClassDB(DocClassDB){};

exec("./docTemplates.cs");

exec("./classMethodExport/classMethodExport.gui");
exec("./classMethodExport/classMethodExport.cs");

function SysDocObject::exportMethods(%this,%group){
	//# DECLARE %group as SimGroup
	%group = %group;
	%st = %group.setType;
	echo("this:" SPC %this);
	echo("group:" SPC %group @ "(" @ %st @ ")");
	if(%st $= "" ){
		warn("Not a valid ConDoc simgroup.");
		return;
	}	
	echo("report type:" SPC %group.setType);
	echo("class:" SPC %group.docClass);
	if(%st $= "methodset"){
		//%set = %group.getObject(0);
		echo("Methods: " SPC %group.getCount());
      foreach(%method in %group){
         if(%method.addme){
            //echo( "(" @ %method.methodNamespace @ ")" @ %method.methodPrototype @ " : " @ %method.addme);
            echo(%method.applyTemplate(Method_Template) );
         }
      }
	} else {
	   warn("Must be a methodset SimSet object. (" @ %st @ ")");
	}
}

/*
	Generate and output report for a ConDoc generated simgroup
*/
function SysDocObject::report(%this,%group){
	//# DECLARE %group as SimGroup
	%group = %group;	
	%st = %group.setType;
	echo("this:" SPC %this);
	echo("group:" SPC %group @ "(" @ %st @ ")");
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
	} else if(%st $= "fieldset"){
		%fset = %group.getObject(2);
		echo("Fields: " SPC %fset.getCount());		
	} else if(%st $= "argset"){
		%set = %group.getObject(0);
		echo("Args: " SPC %set.getCount());		
	} else if(%st $= "methodset"){
		//%set = %group.getObject(0);
		echo("Methods: " SPC %group.getCount());
      foreach(%method in %group){
         //if(%method.isCallback){
            echo( "(" @ %method.methodNamespace @ ")" @ %method.methodPrototype @ " : " @ %method.addme);
         //}
      }
	} else {
		echo( "Unknown ConDoc type:" @ %st);
	}
}

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
	foreach(%m in %grp){
		%u = %m.getUsage();
		if(%u){
			%tx = %u.brief;
		}else {
			%tx = "";
		}	
		echo("Method:" SPC %m.methodName @ "::" @ %m.methodNamespace SPC %tx);
	}
	return %grp;
}

function SysDoc::ctst(){
	%tst = SysDoc.getDocClass("MainMenuAppLogo");
	%grp = %tst.getClassDoc();
	warn("Doc class group ...");
	//foreach(%m in %grp){ if(%m) echo("Method:" SPC %m.getClassName());}
	return %grp;
}

// -----------------------------------------------------

function SysDocClass::test(%this){
   echo(%this);
}

function SysDocClass::getCallbacks(%this,%classname){
   //%mset = %cls.getMethodSet();
   %mset = %this.getMethodGroup();
   warn(%mset);   
   
   %rset = new SimSet();
   %rset.setType = "methodset";
   %rset.docClass = %this.docClassName;
   %t = %classname !$= "" ? "no" : "yes";
   warn(%t);
   %c = 0;
   foreach(%method in %mset){
      %addme = true;
      if( %classname !$= "" && %classname !$= %method.methodNamespace ){
         %addme = false;
      }
      if(%method.isCallback && %addme){
         //%c++;
         echo(%method.getid() @ "(" @ %method.methodNamespace @ ") "  @ %method.methodName);
         //if(%c==1) %method.addme = 1;
         //if(%c==4) %c = 0;
         %rset.add(%method);
      }
   }
   return %rset;
}

function SysDocClass::getClassMethods(%this,%classname,%group){
   if(!%group) %group = %this.getMethodGroup();   
   %rset = new SimSet();
   %rset.setType = "methodset";
   %rset.docClass = %cls.docClassName;
   %c = 0;
   foreach(%method in %group){
      if(%method.methodNamespace $= %classname){
         echo(%method.getid() SPC %method.methodPrototype);         
         %rset.add(%method);
      }
   }
   return %rset;
}

function SysDocClass::countClassMethods(%this,%classname,%group){
   if(!%group) %group = %this.getMethodGroup();
   %c = 0;
   foreach(%method in %group){
      if(%method.methodNamespace $= %classname){
         %c++;
         //echo(%method.getid() SPC %method.methodPrototype);         
      }
   }
   return %c;
}
