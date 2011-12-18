#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blowfish.h"

#define		KEY		"164891"
#define		KEY2		"169841"
#define		SIGNATURE	"Joymax Pack File"

unsigned char magick[] = {0xD8, 0xDA, 0x30, 0x00};

int main(int argc, char **argv)
{
	unsigned char data[128];
	CBlowFish *bf = new CBlowFish();
	
	memset(data, '\0', 128);
	bf->Initialize((unsigned char *)KEY, strlen(KEY));

	bf->Encode((unsigned char *)SIGNATURE, data, 128);
	fprintf(stdout, "Enc 1: [%x %x %x] [%x %x %x]\n", magick[0], magick[1], magick[2], data[0], data[1], data[2]);

	bf->Decode((unsigned char *)SIGNATURE, data, 128);
	fprintf(stdout, "Enc 2: [%x %x %x] [%x %x %x]\n", magick[0], magick[1], magick[2], data[0], data[1], data[2]);

	delete bf;
	bf = new CBlowFish();

	bf->Initialize((unsigned char *)KEY2, strlen(KEY2));

	bf->Encode((unsigned char *)SIGNATURE, data, 128);
	fprintf(stdout, "Enc 3: [%x %x %x] [%x %x %x]\n", magick[0], magick[1], magick[2], data[0], data[1], data[2]);

	bf->Decode((unsigned char *)SIGNATURE, data, 128);
	fprintf(stdout, "Enc 4: [%x %x %x] [%x %x %x]\n", magick[0], magick[1], magick[2], data[0], data[1], data[2]);
	delete bf;

	return EXIT_SUCCESS;
}

