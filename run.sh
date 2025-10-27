#!/bin/bash
set -e

rm -rf build
mkdir -p build
cd build

cmake ..

make run_tests && echo "Success" || echo "Fail"

./tests/run_tests --success

valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--error-exitcode=1 \
		./tests/run_tests --success

echo "
Success: All tests passed and no memory leaks detected."
