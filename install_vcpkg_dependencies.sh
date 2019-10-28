#!/bin/sh -e

while read -r dependency; do
	vcpkg install $dependency
done < vcpkg_dependencies
