#include "SourceID.h"
int SourceID::counter = 1;
SourceID::SourceID() { counter++; }