[Setup]
AppName=GravitySim
AppVersion=1.0
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

[Icons]
Name: "{commondesktop}\GravitySim"; Filename: "{app}\GravitySim.exe"; IconFilename: "{app}\icon.ico"
Name: "{group}\GravitySim"; Filename: "{app}\GravitySim.exe"

[Run]
Filename: "{app}\bin\GravitySim.exe"; Description: "Launch GravitySim"; Flags: nowait postinstall skipifsilent
