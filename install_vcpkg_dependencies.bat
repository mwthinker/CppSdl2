@echo off
for /f "delims=" %%a in (vcpkg_dependencies) DO (
	vcpkg install %%a
)
