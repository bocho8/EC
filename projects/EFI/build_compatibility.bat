"E:\Program Files\VisualStudio\MSBuild\Current\Bin\MSBuild.exe" "bootx64/bootx64_compatibility.sln" /t:Clean,Build /property:Configuration=Release /property:Platform=x64
copy "bootx64\x64\Release\bootx64.efi" .
