$scriptPath=($pwd).path
echo  "scriptPath" "$scriptPath"
$WorkSpacePath=(get-item $scriptPath ).parent.parent.FullName
echo  "WorkSpacePath" "$WorkSpacePath"
echo "clean root" "$PSScriptRoot"
powershell  $WorkSpacePath\clean_temp.ps1

#$scriptPath = (Get-ScriptDirectory);
#$parentPath = Split-Path -parent $scriptPath

make clean
dir
