# TSConsoleDoc
Extended script object documentation engine for Torque3d

Updated classes for interacting with and documenting the fields and methods of SimObject derived object types defined in a game.

For more info see here: https://kb-games.github.io/TSConsoleDoc/

### Installation 3x: 
Drop the "game" and "engine" folders into the relevant locations within a local game setup. Add the .cpp/.h files in the "engine" subfolders to the project, and everything should work. <br>

The only modification to core game files is adding a line to execute the consoleDoc system in " ./tools/main.cs". So this should be a drop-in resource for any Torque implementation that has not modified this file.<br>

This was tested in T3D 3.10 with a clean install, but it should work as a drop-in resource with much earlier versions.<br>

### Installation 4x:
The 4x implementation still has issues. 

Engine-side works great. Add the files from the resource "engine" directory into the equivalent local project folders and add the .cpp/.h files to the project and it should build.<br>

Script-side currently does not work great (resources using long established Torque script-side structure simply refuse to execute in 4x).<br>

For down-and-dirty testing, I've been dropping the "consoleDoc/" folder directly into the root "game/" directory. The script-side system is then launced with:<br>
``` exec("./consoleDoc/consoleDoc.tscript"); ```

From there, all the SysDoc object manipulation and templating functions work well. The Method Export GUI has missing resources and is not currently fully functional.
