$CONFIG = "Debug"
if ($args.Count -gt 0){
 $CONFIG = $args[0]
 Write-Host $CONFIG
}
cmake -S . -B ../winbuild -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
cmake --build ../winbuild --config $CONFIG 
