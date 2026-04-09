@echo off
echo =======================================================
echo  Activating Emscripten...
echo =======================================================
call "C:\Users\vivan\OneDrive\Desktop\emsdk\emsdk_env.bat"

echo =======================================================
echo  Compiling Library Management System to WebAssembly...
echo =======================================================

em++ main.cpp -o index.html --shell-file shell.html -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY=1 -s EXIT_RUNTIME=0 -s FORCE_FILESYSTEM=1 -s EXPORTED_FUNCTIONS="['_main','_malloc','_free']" -s EXPORTED_RUNTIME_METHODS="['stringToUTF8','lengthBytesUTF8']" -O2

echo.
if %errorlevel%==0 (
  echo  BUILD SUCCESS
  echo  Emscripten has generated index.html, index.js, index.wasm
) else (
  echo  BUILD FAILED - Check errors above
)
echo.
echo Remember to run 'run_local.bat' to view the site on localhost!
pause