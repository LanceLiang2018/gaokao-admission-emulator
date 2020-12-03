@REM emsdk_env
@REM @call "%~dp0emsdk" construct_env

call D:\gits\emsdk\emsdk.bat construct_env
emcc -o hello.html hello.c

@REM start server 80
@REM start http://localhost/hello.html
@REM pause
