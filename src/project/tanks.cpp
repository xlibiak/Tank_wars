//
// Created by adria on 14. 11. 2019.
//
#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

const unsigned int SIZE = 512;

int main() {
    // Initialize our window
    ppgso::Window window("tanks", SIZE, SIZE);

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}