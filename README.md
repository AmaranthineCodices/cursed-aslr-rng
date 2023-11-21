# cursed-aslr-rng

some time ago I had a ~~terrible~~ brilliant idea: what if you used ASLR as a PRNG?

this is the result

please do not use this or anything like it for anything ever

## how

aslr randomizes the starting point of, among other things, your heap space. the premise is simple: get a new heap space, get a new random number. it's not a very good random number, but it's a number.

to do this, the executable must be linked as a position-independent executable (because otherwise you don't get aslr). from there you have to export a function (like a shared library) (yes, it's an executable). you then load the executable as a shared library, pull the function out, and have it give you a `uint64_t` pointer from that randomized address space.

however, there is a problem: `dlclose` is not guaranteed to actually release the address space, so you can end up with the same number if you try this multiple times in a row. to bust the cache, we copy the executable to a new file and load the copy. once we're done, we delete the copy. this is not a good idea, but then again, nothing here is.

## usage

- step 1: don't
- step 2: ???
- step 3: revel in your blissful existence

## building, in case you too are a fool
```
mkdir build
cd build
cmake ..
cmake --build .
./cursed-aslr
```
