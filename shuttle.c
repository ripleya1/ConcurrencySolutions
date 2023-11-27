#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "shuttle.h"
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h" 

// wait for shuttle
// lock when shuttle arrives (bc cant leave when arriving during boarding)
// attendees per shuttle <= 30
