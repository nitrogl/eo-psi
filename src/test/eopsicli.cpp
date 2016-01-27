/*
 * A client sample implementation
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>

#include <libhashbuckets.h>
#include "eopsicli.h"
//-----------------------------------------------------------------------------

static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -p <modulo> -n <number> -o <outfile>\n"
            << " -p : set p generating numbers modulo p (big integer)\n"
            << " -n : random numbers to generate\n"
            << " -o : file name to store generated numbers\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  EOPrivateSetIntersectionClient client;
  
  client.waitForServer();

  return 0;
}
//-----------------------------------------------------------------------------
