; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

#ifndef AppVersion
#  define AppVersion  "1.0.0.0"
#endif 

#define AppId "{88CCA305-DF18-41B8-AEAE-BE3CA77843C8}"
#define AppName "iHateScreenSaver"

[Setup]
AppId={{#AppId}
AppName=My Program
AppVersion={#AppVersion}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
Compression=lzma2
SolidCompression=yes
OutputDir=.
OutputBaseFilename={#AppName}_v{#AppVersion}
CloseApplications=force

[CustomMessages]
AppName={#AppName}
LaunchProgram=설치 완료 후 프로그램 실행

[Run]
Filename: "taskkill.exe"; Parameters: "/f /im {cm:AppName}.exe"; Flags: waituntilterminated runhidden;
Filename: {app}\{cm:AppName}.exe; Description: {cm:LaunchProgram,{cm:AppName}}; Flags: nowait postinstall skipifsilent

[Files]
Source: "..\Release\ihatescreensaver.exe"; DestDir: "{app}"

[Registry]
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: iHateScreenSaver; ValueData: "{app}\ihatescreensaver.exe"; Flags: deletevalue uninsdeletevalue;

