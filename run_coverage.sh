bash ./build.sh

lcov --directory . --capture --output-file coverage.info

lcov --remove coverage.info '/usr/*' /home/parallels'/.cache/*' --output-file coverage.info

lcov --list coverage.info

rm -rf html

genhtml -o html coverage.info
