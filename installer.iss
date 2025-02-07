[Setup]
AppName=GravitySim
AppVersion=1.0.1
DefaultDirName={pf}\GravitySim
OutputDir=.
OutputBaseFilename=GravitySimInstaller
SetupIconFile=icon.ico
Compression=lzma
SolidCompression=yes
  
[Dirs]
Name: "{app}\bin"; Flags: uninsalwaysuninstall

[Files]
Source: "build\GravitySim.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "shaders\*"; DestDir: "{app}\shaders"; Flags: ignoreversion recursesubdirs
Source: "fonts\*"; DestDir: "{app}\fonts"; Flags: ignoreversion recursesubdirs
Source: "icon.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "icon.png"; DestDir: "{app}"; Flags: ignoreversion


[Icons]
; Desktop Shortcut
Name: "{commondesktop}\GravitySim"; Filename: "{app}\bin\GravitySim.exe"; IconFilename: "{app}\icon.ico"

; Start Menu Shortcut
Name: "{group}\GravitySim"; Filename: "{app}\bin\GravitySim.exe"; IconFilename: "{app}\icon.ico"

; Add shortcut for searching
Name: "{commonstartmenu}\GravitySim"; Filename: "{app}\bin\GravitySim.exe"; IconFilename: "{app}\icon.ico"; WorkingDir: "{app}\bin"

[Run]
Filename: "{app}\bin\GravitySim.exe"; Description: "Launch GravitySim"; Flags: nowait postinstall skipifsilent
