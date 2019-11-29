set -euo pipefail

mkdir -p build && cd build

df -Th

pwd

ls -l ..

cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake --build . --config Debug -- -j 2

ctest --output-on-failure
