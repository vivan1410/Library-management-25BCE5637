@echo off
echo =======================================================
echo Starting Local Web Server
echo =======================================================
echo.
echo Starting Python HTTP Server on port 8000...
echo.
echo Open a web browser and navigate to: http://localhost:8000
echo.
python -m http.server 8000
pause
