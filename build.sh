mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j24
cd ..
./build/bin/SimpleStringTest
./build/bin/SimpleString ./data/text.txt
