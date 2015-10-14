/*
 * helper_allocation_test.cpp
 *
 *  Created on: 21 Oct 2015
 *      Author: tglman
 */
#include "../src/helpers.h"
#include <assert.h>

int main() {

	Orient::ContentBuffer buffer;

	buffer.prepare(100000);
	assert(buffer.size > 100000);
	return 0;
}

