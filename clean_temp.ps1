echo "clean temp"
$scriptPath=($pwd).path
echo "CleanScriptPath" "$scriptPath"

$ScriptDir = Split-Path $script:MyInvocation.MyCommand.Path
Write-Host "Current CleanScriptPath directory is $ScriptDir"


echo "clean temp root" "$PSScriptRoot"
cd  $PSScriptRoot
dir

 
Get-ChildItem . -recurse -include *.o | remove-item
Get-ChildItem . -recurse -include *.d | remove-item
Get-ChildItem . -recurse -include *.pp | remove-item
Get-ChildItem . -recurse -include *.obj | remove-item
Get-ChildItem . -recurse -include *.bak | remove-item

echo "clean temp done"