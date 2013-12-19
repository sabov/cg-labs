/*
 * Basics of Computer Graphics Exercise
 *
 * DO NOT EDIT THIS FILE!
 */

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <gl_core_32.hh>
#include "main.h"
#include <iostream>

void drawScene(bool environmentOnly, int meshNumber, bool cubeMapping, bool debugTexture );
void initCustomResources();
void deleteCustomResources();
void printStudents();
void resizeCallback(int newWidth, int newHeight);

#endif