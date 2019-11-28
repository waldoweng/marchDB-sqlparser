set -euo pipefail

mkdir -p build && cd build

cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake --build . --config Debug -- -j 2

ctest -j 2 --output-on-failure