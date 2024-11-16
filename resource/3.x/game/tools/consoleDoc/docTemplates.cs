// ============================================================
// Project            :  Full Game
// File               :  .\tools\docTemplates.cs
// Copyright          :  
// Author             :  Kent Butler
// Created on         :  Thursday, November 16, 2023 10:19 PM
//
// Editor             :  TorqueDev v. 1.3.8712.22056
//
// Description        :  
//                    :  
// Usage Macros:
//    {{u_brief}}
//    {{u_full}}
//    {{u_desc}}
//    {{u_formatted}}
//
// Classset Macros:
//    {{this}}
//    {{classset}}
//
// Class Macros:
//    {{cls_name}}
//    {{cls_parent}}
//    {{cls_group}}
//    {{cls_namespace}}
//    {{cls_package}}
//    {{cls_filename}}
//    {{cls_lineno}}
//    {{cls_isscript}}
//    {{fieldset}}
//    {{methodset}}
//
// Methodset Macros:
//    {{this}}
//    {{methodset}}
//
// Method Macros:
//    {{name}}
//    {{type}}
//    {{group}}
//    {{namespace}}
//    {{baseclass}}
//    {{package}}
//    {{prototype}}
//    {{minarg}}
//    {{maxarg}}
//    {{file}}
//    {{lineno}}
//    {{isscript}}
//    {{isvirtual}}
//    {{iscallback}}
//
// Argset Macros:
//    {{this}}
//    {{argset}}
//
// Arg Macros:
//    {{name}}
//    {{type}}
//    {{typenative}}
//    {{default}}
//    {{required}}
//
// Fieldset Macros:
//    {{this}}
//    {{fieldset}}
//
// Field Macros:
//    {{name}}
//    {{value}}
//    {{type}}
//    {{group}}
//    {{text}}
//    {{isarray}}
//    {{index}}
//    {{isdynamic}}
//    {{issub}}
//    {{sub}}
//
// ============================================================
new ScriptObject(JSON_Template){
	templateVersion = 1;
	templateName = "TScript_JSON_Output";
   formatEOL = true;
   
	classTemplate = "{\"class\":\"{{cls_name}}\",\"parent\":\"{{cls_parent}}\",\"group\":\"{{cls_group}}\",\"namespace\":\"{{cls_namespace}}\",\"package\":\"{{cls_package}}\",\"file\":\"{{cls_file}}\",\"lineno\":\"{{cls_lineno}}\",\"isscript\":\"{{cls_isscript}}\",\"usage\":{\"brief\":\"{{u_brief}}\",\"description\":\"{{u_desc}}\",\"full\":\"{{u_full}}\"},{{fieldset}},{{methodset}} }";
   classUsageTemplate = "";
   
	fieldTemplate = "{\"fieldName\":\"{{name}}\",\"fieldType\":\"{{type}}\",\"fieldGroup\":\"{{group}}\",\"isDynamic\":\"{{isdynamic}}\",\"isArrayElement\":\"{{isarray}}\",\"index\":\"{{index}}\",\"isSubstitution\":\"{{issub}}\",\"fieldSubstitution\":\"{{sub}}\",\"usage\":{\"brief\":\"{{u_brief}}\",\"full\":\"{{u_full}}\"} }";
	fieldUsageTemplate = "/*\n{{u_brief}}\n*/";
	fieldsetTemplate = "\"fieldset\":[\n{{fieldset}}] ";
	fieldsetEol = "\n";
	fieldsetDelim = ",";
	fieldsetPad = "    ";

	methodTemplate = "{\"methodName\":\"{{name}}\",\"returnType\":\"{{type}}\",\"methodGroup\":\"{{group}}\",\"methodNamespace\":\"{{namespace}}\",\"methodPackage\":\"{{package}}\",\"minArg\":\"{{minarg}}\",\"maxArg\":\"{{maxarg}}\",\"isScript\":\"{{isscript}}\",\"scriptFile\":\"{{file}}\",\"scriptLine\":\"{{lineno}}\",\"isVirtual\":\"{{isvirtual}}\",\"isCallback\":\"{{iscallback}}\",{{argset}},\"usage\":{\"brief\":\"{{u_brief}}\",\"full\":\"{{u_full}}\"} }";
	methodUsageTemplate = "/*\n{{u_brief}}\n*/";
	methodsetTemplate = "\"methodset\":[\n{{methodset}}] ";
	methodsetEol = "\n";
	methodsetDelim = ",";
	methodsetPad = "    ";
	
	argTemplate = "{\"arg\":\"{{name}}\",\"type\":\"{{type}}\",\"default\":\"{{default}}\",\"required\":\"{{required}}\" }";
	argsetTemplate =  "\"args\":[{{argset}}] ";
	argsetEol = "";
	argsetDelim = ",";
	argsetPad = "";
};

new ScriptObject(Method_Template){
	templateVersion = 1;
	templateName = "TScript_Method_Output";
	formatEOL = false;

	methodTemplate = "function {{ts_methodobj}}::{{name}}{{argset}} \n{\n\n}\n\n";
	methodUsageTemplate = "";
	argTemplate = "%{{name}}";
	argsetTemplate =  "({{this}}{{argset}})";
	argsetEol = "";
	argsetDelim = ", ";
	argsetPad = "";
};

new ScriptObject(Method_Template_brief){
	templateVersion = 1;
	templateName = "TScript_Method_Output";
	formatEOL = false;

	methodTemplate = "/*\n{{u_brief}}\n*/ \nfunction {{ts_methodobj}}::{{name}}{{argset}} \n{\n\n}\n\n";
	//methodUsageTemplate = "";
	argTemplate = "%{{name}}";
	argsetTemplate =  "({{this}}{{argset}})";
	argsetEol = "";
	argsetDelim = ", ";
	argsetPad = "";
};

new ScriptObject(Method_Template_full){
	templateVersion = 1;
	templateName = "TScript_Method_Output";
	formatEOL = false;

	methodTemplate = "/*\n{{u_full}}\n*/ \nfunction {{ts_methodobj}}::{{name}}{{argset}} \n{\n\n}\n\n";
	methodUsageTemplate = "";
	argTemplate = "%{{name}}";
	argsetTemplate =  "({{this}}{{argset}})";
	argsetEol = "";
	argsetDelim = ", ";
	argsetPad = "";
};

new ScriptObject(HTML_Template){
	templateVersion = 1;
	templateName = "TScript_HTML_Output";
	formatEOL = false;
	classTemplate = "<link rel='stylesheet' href='tsstyle.css'>\n"@
					"<div class='tsclass'>\n"@
					"	<div class='row1'>\n"@
					"		<div class='dcell' id='tsclass_classblock'>\n" @
					"			<div class='' id='tsclass_name'>{{cls_name}}</div>\n" @
					"			<div class='' id='tsclass_parent'>{{cls_parent}}</div>\n" @
					"			<div class='' id='tsclass_group'><span class='dlabel'>Group:</span><span class='ddata'>{{cls_group}}</div>\n" @
					"			<div class='' id='tsclass_namespace'><span class='dlabel'>Namespace:</span><span class='ddata'>{{cls_namespace}}</div>\n" @
					"			<div class='' id='tsclass_package'><span class='dlabel'>Package:</span><span class='ddata'>{{cls_namespace}}</div>\n" @
					"			<div class='' id='tsclass_file'><span class='dlabel'>File(line):</span><span class='ddata'>{{cls_file}}:{{cls_lineno}}</div>\n" @
					"			<div class='' id='tsclass_isscript'><span class='dlabel'>Is script:</span><span class='ddata'>{{cls_isscript}}</div>\n" @
					"		</div><div class='dcell' id='tsclass_usageblock'>\n" @
					"			{{usage}}" @
					"		</div>\n" @
					"	</div><div class='row2'>\n" @
					"		<div class='dcell' id='fielddset_block'>\n" @
					"		<div class='icell''>\n" @
					"			{{fieldset}}\n" @
					"		</div>\n" @
					"		</div><div class='dcell' id='methodset_block'>\n" @
					"		<div class='icell''>\n" @
					"			{{methodset}}\n" @
					"		</div>\n" @					
					"		</div>\n" @
					"	</div>\n" @
					"</div>\n";
	classUsageTemplate = "<div class='' id='tsclass_usageblock'>\n" @
					"			<div class='' id='tsclass_ubrief'><span class='dlabel'>Brief:</span><span class='ddata'>{{u_brief}}</div>\n" @
					"			<div class='' id='tsclass_udesc'><span class='dlabel'>Description:</span><span class='ddata'>{{u_desc}}</div>\n" @
					"			<div class='' id='tsclass_ufull'><span class='dlabel'>Full:</span><span class='ddata'>{{u_full}}</div>\n" @
					"		</div>\n";	
	classsetTemplate = "<div class='tsclassset'>{{classset}}</div>\n";
	classsetEol = "\n";
	classsetDelim = "";
	classsetPad = "    ";
	
	fieldTemplate = "<div class='tsfield'>\n"@
					"	<div class='' id='tsfield_head'><span class='' id='tsfield_name'>{{name}}</span> <span class='' id='tsfield_type'>({{type}})</span></div>\n"@
					"	<div class='' id='tsfield_body'>\n"@
					"		<div class='tsfield_fields'>\n"@
					"			<div class='' id='tsfield_group'><span class='dlabel'>Group:</span><span class='ddata'>{{group}}</span></div>\n" @
					"			<div class='' id='tsfield_isdynamic'><span class='dlabel'>Is Dynamic:</span><span class='ddata'>{{isdynamic}}</span></div>\n" @
					"			<div class='' id='tsfield_isarray'><span class='dlabel'>Is Array(idx):</span><span class='ddata'>{{isarray}}:{{index}}</span></div>\n" @
					"			<div class='' id='tsfield_issub'><span class='dlabel'>Is Sub(field):</span><span class='ddata'>{{issub}}:{{sub}}</span></div>\n" @
					"		</div>\n"@
					"		<div class='' id='tsfield_usage'>\n"@
					"			{{usage}}"@
					"		</div>\n"@
					"	</div>\n"@
					"</div>";
	fieldUsageTemplate = "<div class='' id='tsfield_usageblock'>\n" @
					"			<div class='' id='tsfield_ubrief'><span class='dlabel'>Brief:</span><span class='ddata'>{{u_brief}}</span></div>\n" @
					"			<div class='' id='tsfield_udesc'><span class='dlabel'>Description:</span><span class='ddata'>{{u_desc}}</span></div>\n" @
					"			<div class='' id='tsfield_ufull'><span class='dlabel'>Full:</span><span class='ddata'>{{u_full}}</span></div>\n" @
					"		</div>\n";					
	fieldsetTemplate = 	"<div class='blockhead' id='tsfieldset_hd'>Fields</div>\n"@
						"<div class='' id='tsfieldset'>\n{{fieldset}}</div>\n";
	fieldsetEol = "\n";
	fieldsetDelim = "";
	fieldsetPad = "    ";

	methodTemplate = "<div class='' id='tsmethod'>\n"@
					"	<div class='' id='tsmethod_head'><span class='' id='tsmethod_proto'>{{prototype}}</span></div>\n"@
					"	<div class='' id='tsmethod_body'>\n"@
					"		<div class='tsmethod_argset'>\n"@
					"			{{argset}}"@
					"		</div>\n"@					
					"		<div><div class='' id='tsmethod_fields'>\n"@
					"			<div class='' id='tsmethod_name'><span class='dlabel'>Name:</span><span class='ddata'>{{name}}</span></div>\n" @
					"			<div class='' id='tsmethod_group'><span class='dlabel'>Group:</span><span class='ddata'>{{group}}</span></div>\n" @
					"			<div class='' id='tsmethod_rettype'><span class='dlabel'>Return Type:</span><span class='ddata'>{{rettype}}</span></div>\n" @					
					"			<div class='' id='tsmethod_baseclass'><span class='dlabel'>Base Class:</span><span class='ddata'>{{baseclass}}</span></div>\n" @
					"			<div class='' id='tsmethod_namespace'><span class='dlabel'>Namespace:</span><span class='ddata'>{{namespace}}</span></div>\n" @
					"			<div class='' id='tsmethod_package'><span class='dlabel'>Package:</span><span class='ddata'>{{package}}</span></div>\n" @
					"			<div class='' id='tsmethod_argmin'><span class='dlabel'>Num Arg(min/max):</span><span class='ddata'>{{minarg}}/{{maxarg}}</span></div>\n" @
					"			<div class='' id='tsmethod_isscript'><span class='dlabel'>Is Script(file):</span><span class='ddata'>{{isscript}} / {{file}}:{{lineno}}</span></div>\n" @					
					"			<div class='' id='tsmethod_isvirtual'><span class='dlabel'>Is Virtual:</span><span class='ddata'>{{isvirtual}}</span></div>\n" @
					"			<div class='' id='tsmethod_iscallback'><span class='dlabel'>Is Callback:</span><span class='ddata'>{{iscallback}}</span></div>\n" @
					"		</div>\n"@
					"		<div class='tsmethod_usage'>\n"@
					"			{{usage}}"@
					"		</div></div>\n"@
					"	</div>\n"@
					"</div>";
	methodUsageTemplate = "<div class='tsfield_usageblock'>\n" @
					"			<div class='tsfield_ubrief'><span class='dlabel'>Brief:</span><span class='ddata'>{{u_brief}}</span></div>\n" @
					"			<div class='tsfield_udesc'><span class='dlabel'>Description:</span><span class='ddata'>{{u_desc}}</span></div>\n" @
					"			<div class='tsfield_ufull'><span class='dlabel'>Full:</span><span class='ddata'>{{u_full}}</span></div>\n" @
					"		</div>\n";
	methodsetTemplate = "<div class='blockhead' id='tsmethodset_hd'>Methods</div>\n"@
						"<div class='tsmethodset'>\n{{methodset}}</div>";
	methodsetEol = "\n";
	methodsetDelim = "";
	methodsetPad = "    ";
	
	argTemplate = 	"	<tr class='tsarg_row'>\n"@
					"		<td>{{name}}</td>\n"@
					"		<td>{{type}}</td>\n"@
					"		<td>{{default}}</td>\n"@
					"		<td>{{required}}</td>\n"@
					"	</tr>\n";
	argsetTemplate =  	"<div class='tsargset'>\n"@
						"<table class='tsargset_table'>\n"@
						"	<tr class='tsarg_hrow'>\n"@
						"		<td>Name</td>\n"@
						"		<td>Type</td>\n"@
						"		<td>Default</td>\n"@
						"		<td>Required</td>\n"@
						"	</tr>\n"@
						"	{{argset}}\n"@
						"</table>\n"@
						"</div>\n";
	argsetEol = "\n";
	argsetDelim = "";
	argsetPad = "";
};
