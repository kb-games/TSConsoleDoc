// ---------------------------
function ClassMethodExport::showDlg(%this,%object){

   %this.object = %object;
   %classObj = SysDoc.getDocClass(%object);
   %classObj.objectName = %object.name;
   %this.classObj = %classObj;
   echo("ClassMethodExport:" TAB %this TAB %classObj);
   
   Canvas.pushDialog(%this);
   //if(%db){
   //   %this.dbObj = SysDoc.getDocClass(%db);
   //}
}
// ---------------------------
function ClassMethodExport::onWake(%this)
{
   if(!%this.classObj) return;   

   %this.exportClassMethods = false;
   %this.fileAppend = false;

   %this.curSubclass = "";
   %this.clearInheritance();
   CME_viewAll.setStateOn(true);
   CME_viewAll.profile = "CSM_GuiCheckBoxProfile_bold";
   CME_ToggleSelect.setStateOn(false);

   %pop = CME_ClassSelector;
   %pop.clear();
   %pop.add(%this.classObj.docClassName, %this.classObj);

   %this.classList[0] = %this.classObj;

   CME_DocSel.clear();
   CME_DocSel.add("None",0);
   CME_DocSel.add("Brief",1);
   CME_DocSel.add("Full",2);
   CME_DocSel.setSelected(1,false);

	//# DECLARE %o as SimObject
	%o = %this.object;
	%db = "";
	if(%o.isMethod("getDatablock"))	%db = %this.object.getDatablock();

   if(%db){      
      
      %this.dbObj = SysDoc.getDocClass(%db);
      
      %this.dbObj.objectName = %db.name;
      
      %pop.add(%this.dbObj.docClassName, %this.dbObj);
      
      %dbcb = %this.dbObj.getCallbacks();
      %this.callbacks = %dbcb;
      %this.refreshMethodList(%dbcb);
      %this.setInheritance(%this.dbObj);
      CME_ClassSelector.setText(%this.dbObj.docClassName);
      echo("datablock:"@%this.dbObj);
      %this.classList[1] = %this.dbObj;
      CME_ObjName.setText(%this.object.name @ "(" @ %db.name @ "}" );
   } else {
      %cb = %this.classObj.getCallbacks();
      %this.dbObj = 0;
      %this.callbacks = %cb;
      %this.refreshMethodList(%cb);
      %this.setInheritance(%this.classObj);
      CME_ClassSelector.setText(%this.classObj.docClassName);
      CME_ObjName.setText(%this.object.name);
   }
}
// ---------------------------
function ClassMethodExport::onSleep(%this)
{   
   if(%this.classObj) ClassMethodExport.classObj.delete();
   %this.classObj = 0;
   
   if(this.callbacks) ClassMethodExport.callbacks.delete();
   %this.callbacks = 0;
   
   MethodSelectList.clear();
   %this.clearInheritance();
}
// ---------------------------
function ClassMethodExport::refreshMethodList(%this,%methods)
{
   MethodSelectList.clear();
   CME_mInfo.setText("");
   CME_mInfo.visible = false;
   CSM_expButton.visible = false;
   CME_Prototype.visible = false;
   
   if(!%methods.getCount()){
      MethodSelectList.addRow(0,"No methods.");
      return;
   }
   
   foreach(%method in %methods){
      //echo(%method.methodPrototype);
      %add = %method.addme ? "*" : " ";
      //%txt = %add SPC %method.methodPrototype;
      %txt = %add SPC "[" @ %method.methodNamespace @ "]" SPC %method.methodName;
      MethodSelectList.addRow(%method.getId(),%txt);
   }   
}
// ---------------------------
function ClassMethodExport::classSelected(%this,%classname,%ctrl)
{
   CME_viewAll.setStateOn(false);
   %this.clearCheckboxes();
   
   warn("Class selected:"@%classname SPC "ctrl:"@%ctrl);
   %this.refreshSelectedList(%classname);
   %this.resetInheritance();
   %this.curSubclass = %classname;
   echo("Subclass:" @ %this.curSubclass);

   if(%ctrl){
       %ctrl.profile = CSM_GuiAutoSizeWidthProfile_bold;
   }
}
// ---------------------------
function ClassMethodExport::superclassSelectorClick(%this){
   %idx = CME_ClassSelector.getSelected();
   %txt = CME_ClassSelector.getTextById(%idx);
   
   warn("Superlass switch:"@%idx SPC %txt);
   
   %this.curSubclass = "";
   %this.clearInheritance();
   %this.clearCheckboxes();
   CME_viewAll.setStateOn(true);
   CME_viewAll.profile = "CSM_GuiCheckBoxProfile_bold";
   CME_ToggleSelect.setStateOn(false);

   %cb = %idx.getCallbacks();
   if(%this.callbacks) %this.callbacks.delete();
   %this.callbacks = %cb;

   %this.refreshMethodList(%cb);
   %this.setInheritance(%idx);
   CME_ObjName.setText(%idx.objectName);
   //CME_ClassSelector.setText(%this.dbObj.docClassName);
}

// ---------------------------
function ClassMethodExport::refreshSelectedList(%this,%classname)
{
   %cl = %this.classObj.getClassMethods(%classname,%this.callbacks);
   %this.refreshMethodList(%cl);
   %cl.delete();
}
// ---------------------------
function ClassMethodExport::selectAll(%this)
{
   %this.clearCheckboxes();
   CME_viewAll.setStateOn(true);
   CME_viewAll.profile = "CSM_GuiCheckBoxProfile_bold";   
   warn("Class selected: ALL");
   %this.refreshMethodList(%this.callbacks);
   %this.resetInheritance();
   //%this.curSubclassList = ClassMethodExport.callbacks;
   %this.curSubclass = "";
}
// ---------------------------
function ClassMethodExport::clearCheckboxes(%this){
   CME_viewAll.profile = "GuiCheckBoxProfile";
   CME_viewAll.setStateOn(false);   
   CME_ShowExports.profile = "GuiCheckBoxProfile";
   CME_ShowExports.setStateOn(false);
   
   CME_ToggleSelect.profile = "GuiCheckBoxProfile";
   CME_ToggleSelect.setStateOn(false);
   
   //ClassMethodExport_expButton.profile = "GuiCheckBoxProfile";
}

// ---------------------------
function ClassMethodExport::clearInheritance(%this){
   for (%i=CME_inheritance.getCount() - 1; %i >=0 ; %i--)
   {
      %obj = CME_inheritance.getObject(%i);
      if(%obj) %obj.delete();
      else warn("No Object?" SPC %obj);
   }
}
// ---------------------------

function ClassMethodExport::resetInheritance(%this){
   for (%i=0; %i < CME_inheritance.getCount(); %i++)
   {
      %obj = CME_inheritance.getObject(%i);
      if(%obj) %obj.profile = CSM_GuiAutoSizeWidthProfile;
   }
}
// ---------------------------
function ClassMethodExport::reflowInheritance(%this){
   %xpos = 2;
   for (%i=0; %i < CME_inheritance.getCount(); %i++)
   {      
      %obj = CME_inheritance.getObject(%i);
      %pos = %obj.position;
      %pos.x = %xpos;
      %obj.position = %pos;
      %xpos += %obj.extent.x + 5;
   }
}
// ---------------------------
function ClassMethodExport::setInheritance(%this,%classobj){
   echo("class:"@%classobj);
   if(!%classobj) return;   
   %this.clearInheritance();
   CME_viewAll.setStateOn(true);
   %inh = "";
   %idel = "";
   %idx = 0;
   foreach$ (%cname in %classObj.Inheritance)
   {
      %inh = %inh SPC %idel @ %cname;      
      %idel = "<- ";
      %tstr = %this.getClassTxt(%cname,%idx);
      CME_inheritance.add(%tstr);
      %idx++;
   }
   //CME_inhTxt.setText(%this.classObj.Inheritance);
   //CME_inhTxt.setText(%inh);
   %this.reflowInheritance();
   echo(%inh);   
}
// ---------------------------
function ClassMethodExport::toggleListSelect(%this){
   echo("Toggle List Export.");
   %add = CME_ToggleSelect.isStateOn();
   for(%i = 0; %i < MethodSelectList.rowCount(); %i++){
      %item = MethodSelectList.getRowId(%i);
      %item.addme = %add;
   }
   warn("Cur subclass:"@%this.curSubclass);
   if(%this.curSubclass $= ""){
      %this.refreshMethodList(%this.callbacks);
   } else {
      echo("refreshing?");
      %this.refreshSelectedList(%this.curSubclass);
   }
}
// ---------------------------
function ClassMethodExport::methodSelected(%this){
   echo("Selection made.");
   %id = MethodSelectList.getSelectedId();
   %u = %id.getUsage();   
   echo(%id.methodname);
   echo(%u.full);
   CME_expButton.setStateOn(%id.addme);
   CME_mInfo.setText(%u.full);
   CME_mInfo.visible = true;
   CME_expButton.visible = true;
   CME_Prototype.setText(%id.methodPrototype);
   CME_Prototype.visible = true;
}

// ---------------------------
function ClassMethodExport::toggleExport(%this){
   %add = CME_expButton.isStateOn();
   %id = MethodSelectList.getSelectedId();   
   //echo("Export set:"@%id);
   %id.addme = %add;   
   %add = %id.addme ? "*" : " ";
   //%txt = %add SPC %id.methodPrototype;
   %txt = %add SPC "[" @ %id.methodNamespace @ "]" SPC %id.methodName;
   MethodSelectList.setRowById(%id,%txt);
}
// ---------------------------
function ClassMethodExport::exportMethods(%this){
   echo( "Class Count:"@ CME_ClassSelector.size() );   

   %dlg = new SaveFileDialog()
   {
      Filters        = "Torque Script Files (*.tscript,*.cs)|*.tscript,*.cs|All Files (*.*)|*.*|";
      DefaultPath    = "/scripts";
      ChangePath     = true;
      OverwritePrompt   = true;
   };

   %ret = %dlg.Execute();
   if(%ret)
   {
      // Immediately override/set the levelsDirectory
      %filename = %dlg.FileName;
   }
   warn(%filename);
   %dlg.delete();
   if(! %ret){
      return;
   }

	%file = new FileObject();
	if(%this.fileAppend){
	   if(!%file.openForAppend(%filename)){
	      MessageBoxOk( "Error",
	      "Could not open file for append (not saved): "@%filename);
         return;
	   }
	} else {
      if(!%file.openForWrite(%filename)){
         MessageBoxOk( "Error",
         "Could not open file for write (not saved): "@%filename);
         return;
      }
	}
	
   // Get the documentation type and select appropriate template.
   %doctype = CME_DocSel.getSelected();
   %tpl = Method_Template;
   if(%doctype == 1){
      %tpl = Method_Template_brief;
   } else if(%doctype == 2){
      %tpl = Method_Template_full;
   }

   for(%i=0;%i < CME_ClassSelector.size();%i++){      
      %id = %this.classList[%i];
      echo("export dbclass index:"@%i SPC %id);
      %cb = %id.getCallbacks();
      echo("Export Classes? "@%this.exportClassMethods);
      if(%this.exportClassMethods){
         %n = %id.docClassName;
      }else{
         %n = %id.objectName;
      }
      // Only make a heading and loop if there is an export
      if(%this.addcount(%cb) > 0){
         echo("Name:" @ %n);      
         %hd="/*\n    T3D: Auto-generated method template stubs for:" @ %n @ ".\n*/\n\n";
         %file.writeLine(%hd);
         foreach(%method in %cb){         
            if(%method.addme){         
               //%txt = %method.applyTemplate(Method_Template);            
               %txt = %method.applyTemplate(%tpl);
               
               // Handle local template macro replacements            
               %txt = strReplace(%txt,"{{ts_methodobj}}",%n);

               %file.writeLine(%txt);
               //echo(%txt);
               //echo(%method.methodPrototype);
            }
         }
      }
      %cb.delete();
   }
   %file.close();
   MessageBoxOk( "File Saved","File save successful: " @ %filename);
}

// ---------------------------
function ClassMethodExport::showExports(%this){
   %this.clearCheckboxes();
   CME_ShowExports.setStateOn(true);
   CME_ShowExports.profile = "CSM_GuiCheckBoxProfile_bold";

   %rset = new SimSet();
   %rset.setType = "methodset";
   %rset.docClass = %this.classObj.docClassName;

   for(%i=0;%i < CME_ClassSelector.size();%i++){      
      %id = %this.classList[%i];
      %cb = %id.getCallbacks();
      foreach(%method in %cb){         
         if(%method.addme){
            %rset.add(%method);
         }
      }
      %cb.delete();
   }
   %this.refreshMethodList(%rset);
   %rset.delete();
}

function ClassMethodExport::addCount(%this,%group){
   %rv = 0;
   foreach(%method in %group){         
      if(%method.addme){
         %rv++;
      }
   }
   return %rv;
}

function strReplace(%body,%search,%replace){
   %rv = %body;
   %start = strpos(%rv, %search);
   if(%start<0){
      return %body;
   }   
   %len = strlen(%search);
   while(%start>=0){
      %l = getSubStr(%rv,0,%start);
      %r = getSubStr(%rv,%start+%len);
      %rv = %l @ %replace @ %r;
      //echo(%rv);
      %start = strpos(%rv, %search);
   }
   return %rv;   
}
// ---------------------------

function ClassMethodExport::getClassTxt(%this,%classname,%idx)
{
   %idel = %idx > 0 ? "<- " : "   ";
   %sz = 75;   
   %pos = 1 + (%idx * %sz) + 5;
   
   %position = %pos @ " 0";
   %extent = %sz @ " 20";

   %ntxt = new GuiTextCtrl()
   {
            text = "className";
            maxLength = "1024";
            margin = "0 0 0 0";
            padding = "0 0 0 0";
            anchorTop = "1";
            anchorBottom = "0";
            anchorLeft = "1";
            anchorRight = "0";
            position = %pos;
            extent = %extent;
            minExtent = "8 2";
            horizSizing = "right";
            vertSizing = "bottom";
            //profile = "GuiTextProfile";
            profile = CSM_GuiAutoSizeWidthProfile;
            visible = "1";
            active = "1";
            tooltipProfile = "GuiToolTipProfile";
            hovertime = "1000";
            isContainer = "1";
            canSave = "1";
            canSaveDynamicFields = "0";
   };
   %command = "ClassMethodExport.classSelected(\"";
   %command = %command @ %classname;
   %command = %command @ "\"," ;
   %command = %command @ %ntxt;
   %command = %command @ ");";
   echo(%command);   
   %ntxt.command = %command;   
   %ntxt.setText(%idel @ %classname);

   return %ntxt;
}
