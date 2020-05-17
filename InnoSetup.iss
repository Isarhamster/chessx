
#define AppVersion '4.7.1'
#define AppName    'Scid'
#define TCLDIR     'C:\Tcl'

[Setup]
AppName={# AppName}
AppVerName={# AppName} {# AppVersion}
AppVersion={# AppVersion}
AppPublisher=The Scid project
AppPublisherURL=http://http://scid.sourceforge.net
UsePreviousAppDir=no
DefaultDirName={userdocs}\{# AppName}-{# AppVersion}
DefaultGroupName={# AppName}
DisableStartupPrompt=yes
WindowShowCaption=yes
WindowVisible=no
AllowNoIcons=yes
LicenseFile=COPYING
Compression=bzip/9
SourceDir=.
OutputDir=winpackage
OutputBaseFilename={# AppName}-{# AppVersion}
ChangesAssociations=yes
PrivilegesRequired=lowest

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: "associate_pgn"; Description: "&Associate PGN files"; GroupDescription: "Other tasks:"
Name: "associate_si4"; Description: "&Associate SI4 files"; GroupDescription: "Other tasks:"
Name: "copy_photos"; Description: "&Install players photo and spell checker"; GroupDescription: "Other tasks:"
Name: "install_stockfish"; Description: "&Stockfish ( http://stockfishchess.org )"; GroupDescription: "Chess engines:"

[Registry]
Root: "HKCU"; Subkey: "Software\Classes\.pgn"; Flags: uninsdeletevalue; Tasks: associate_pgn
Root: "HKCU"; Subkey: "Software\Classes\.pgn"; ValueType: string; ValueData: "scid"; Flags: uninsdeletevalue; Tasks: associate_pgn
Root: "HKCU"; Subkey: "Software\Classes\scid"; Flags: uninsdeletevalue; Tasks: associate_pgn
Root: "HKCU"; Subkey: "Software\Classes\scid\shell\open\command"; ValueType: string; ValueData: """{app}\bin\scid"" ""%1"""; Flags: uninsdeletevalue; Tasks: associate_pgn

Root: "HKCU"; Subkey: "Software\Classes\.si4"; Flags: uninsdeletevalue; Tasks: associate_si4
Root: "HKCU"; Subkey: "Software\Classes\.si4"; ValueType: string; ValueData: "scid"; Flags: uninsdeletevalue; Tasks: associate_si4
Root: "HKCU"; Subkey: "Software\Classes\scid"; Flags: uninsdeletevalue; Tasks: associate_si4
Root: "HKCU"; Subkey: "Software\Classes\scid\shell\open\command"; ValueType: string; ValueData: """{app}\bin\scid"" ""%1"""; Flags: uninsdeletevalue; Tasks: associate_si4

[Files]
Source: "Release\*"; DestDir: "{app}\bin"
Source: "scid.eco"; DestDir: "{app}\bin"
Source: "img\*"; DestDir: "{app}\bin\img"; Flags: recursesubdirs
Source: "tcl\*"; DestDir: "{app}\bin\tcl"; Flags: recursesubdirs
Source: "books\*"; DestDir: "{app}\bin\books"; Flags: recursesubdirs
Source: "html\*"; DestDir: "{app}\bin\html"; Flags: recursesubdirs
Source: "sounds\*"; DestDir: "{app}\bin\sounds"; Flags: recursesubdirs
Source: "bitmaps\*"; DestDir: "{app}\bitmaps"; Flags: recursesubdirs
Source: "bitmaps2\*"; DestDir: "{app}\bitmaps2"; Flags: recursesubdirs
Source: "help\*"; DestDir: "{app}\help"; Flags: recursesubdirs
Source: "COPYING"; DestDir: "{app}"
Source: "winpackage\bases\*"; DestDir: "{app}\bin\bases"; Flags: recursesubdirs
Source: "winpackage\Example.*"; DestDir: "{app}"
Source: "winpackage\scidsnd.exe"; DestDir: "{app}\bin"
Source: "winpackage\phalanx-scid.exe"; DestDir: "{app}\bin\engines";
Source: "winpackage\stockfish.exe"; DestDir: "{app}\bin\engines"; Tasks: install_stockfish
Source: "winpackage\spelling.ssp"; DestDir: "{app}\bin";  Tasks: copy_photos
Source: "winpackage\photos\*"; DestDir: "{app}\bin\photos"; Flags: recursesubdirs; Tasks: copy_photos
Source: "{# TCLDIR}\lib\tcl8\*"; DestDir: "{app}\lib\tcl8"; Flags: recursesubdirs
Source: "{# TCLDIR}\lib\tcl8.6\*"; DestDir: "{app}\lib\tcl8.6"; Flags: recursesubdirs
Source: "{# TCLDIR}\lib\tk8.6\*"; DestDir: "{app}\lib\tk8.6"; Flags: recursesubdirs
Source: "{# TCLDIR}\lib\teapot\package\win32-ix86\lib\*"; DestDir: "{app}\lib"; Flags: recursesubdirs skipifsourcedoesntexist
Source: "{# TCLDIR}\lib\teapot\package\win32-x86_64\lib\*"; DestDir: "{app}\lib"; Flags: recursesubdirs skipifsourcedoesntexist
Source: "{# TCLDIR}\bin\tcl86t.dll"; DestDir: "{app}\bin"; AfterInstall: CreateEngineDat
Source: "{# TCLDIR}\bin\tk86t.dll"; DestDir: "{app}\bin"; AfterInstall: CreateRecentDat

[Dirs]
Name: "{app}\bin\config"

[Icons]
Name: "{group}\{# AppName}"; Filename: "{app}\bin\scid.exe"; Comment: "{# AppName}!"; WorkingDir: {app}\bin
Name: "{group}\Uninstall {# AppName}"; Filename: "{uninstallexe}";
Name: "{userdesktop}\{# AppName}"; Filename: "{app}\bin\scid.exe"; Tasks: desktopicon; Comment: "Desktop {# AppName}!"; WorkingDir: {app}\bin

[Run]
Filename: "{app}\bin\scid.exe"; Flags: nowait postinstall skipifsilent; Description: "{cm:LaunchProgram,{#StringChange(AppName, '&', '&&')}}"

[Messages]
FinishedLabelNoIcons=Setup has finished installing [name] on your computer. Visit our wiki pages ( http://sourceforge.net/p/scid/wiki ) to get additional resources.

[Code]

procedure CreateRecentDat();
var
  fileName : string;
  lines : TArrayOfString;
begin
  fileName := ExpandConstant('{app}\bin\config\recent.dat');

  SetArrayLength(lines, 1);
  lines[0]  := 'set ::autoLoadBases {{' + ExpandConstant('{app}\Example') + '}}'
  SaveStringsToFile(filename,lines,true);
end;

procedure CreateEngineDat();
var
  fileName : string;
  phalanx : TArrayOfString;
  lines : TArrayOfString;
  Index: Integer;
begin
  fileName := ExpandConstant('{app}\bin\config\engines.dat');

  Index := 7
  if WizardForm.TasksList.Checked[Index] then
  begin
      SetArrayLength(lines, 37);
      lines[0]  := '# Analysis engines list file for Scid'
      lines[1]  := ''
      lines[2]  := 'engine {'
      lines[3]  := '  Name {Stockfish}'
      lines[4]  := '  Cmd  ../engines/stockfish.exe'
      lines[5]  := '  Args {}'
      lines[6]  := '  Dir  .'
      lines[7]  := '  Elo  0'
      lines[8]  := '  Time 0'
      lines[9]  := '  URL  {}'
      lines[10] := '  UCI 1'
      lines[11] := '  UCIoptions {{{Analysis Contempt} Off}}'
      lines[12] := '}'
      lines[13]  := ''
      lines[14]  := 'engine {'
      lines[15]  := '  Name {Stockfish - two threads}'
      lines[16]  := '  Cmd  ../engines/stockfish.exe'
      lines[17]  := '  Args {}'
      lines[18]  := '  Dir  .'
      lines[19]  := '  Elo  0'
      lines[20]  := '  Time 0'
      lines[21]  := '  URL  {}'
      lines[22] := '  UCI 1'
      lines[23] := '  UCIoptions {{{Analysis Contempt} Off} {Threads 2}}'
      lines[24] := '}'
      lines[25]  := ''
      lines[26]  := 'engine {'
      lines[27]  := '  Name {Stockfish - four threads}'
      lines[28]  := '  Cmd  ../engines/stockfish.exe'
      lines[29]  := '  Args {}'
      lines[30]  := '  Dir  .'
      lines[31]  := '  Elo  0'
      lines[32]  := '  Time 0'
      lines[33]  := '  URL  {}'
      lines[34] := '  UCI 1'
      lines[35] := '  UCIoptions {{{Analysis Contempt} Off} {Threads 4}}'
      lines[36] := '}'
      SaveStringsToFile(filename,lines,true);
  end

  SetArrayLength(phalanx, 14);
  phalanx[0]  := '# Analysis engines list file for Scid'
  phalanx[1]  := ''
  phalanx[2]  := 'engine {'
  phalanx[3]  := '  Name {Phalanx (easy)}'
  phalanx[4]  := '  Cmd  ../engines/phalanx-scid.exe'
  phalanx[5]  := '  Args {}'
  phalanx[6]  := '  Dir  .'
  phalanx[7]  := '  Elo  0'
  phalanx[8]  := '  Time 0'
  phalanx[9]  := '  URL  {}'
  phalanx[10] := '  UCI 0'
  phalanx[11] := '  UCIoptions {}'
  phalanx[12] := '}'
  phalanx[13] := ''
  SaveStringsToFile(filename,phalanx,true);
end;

