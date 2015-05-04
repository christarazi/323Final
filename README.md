# 323Final

~~Each part will be separated into folders.~~

~~Compile each `.cpp` file with `g++ -std=c++0x <file-name>`. Executable will be named `a.out`.~~

`run.cpp` is the main file. It includes all of part two's function and consolidates part one and three together.

Compile each `partone.cpp` and `partthree.cpp` with `g++ -c -std=c++0x <file-name>` to get the `.o` files.
Then `g++ -std=c++0x -c run.cpp`
Finally `g++ *.o -o a.out` and `./a.out` to run the executable.