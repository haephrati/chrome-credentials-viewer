#ifndef _BASE_H_
#define _BASE_H_

#include <stddef.h>
int conv_letter(char letter, unsigned char *conv);

void encode_b64(const unsigned char *src, char *dest, size_t src_len);
int decode_b64(const char *src, unsigned char *dest, 
	size_t len, size_t *decoded);

size_t encoded_size(size_t src_len);
size_t decoded_size(const char *encoded_data);
#endif
