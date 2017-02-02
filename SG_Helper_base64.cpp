// Chrome Credentials Viewer
// By Michael Haephrati
// ©2014-2017 Secured Globe, Inc.

#include "stdafx.h"
#include <stddef.h>
#include <string.h>
#include "SG_Helper_base64.h"

static const char *b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* returns 0 if it isn't in b64 table, 
 * 1 otherwise and returns the conversion in 
 * conv
 */
int conv_letter(char letter, unsigned char *conv)
{
	const char *pos;

	pos = strchr(b64_table, letter);
	if (!pos && letter != '=')
		return 0;
	else if (letter == '=')
		*conv = 0;
	else
		*conv = pos - b64_table;
	return 1;
}


static int decode_4chars(const char *src, unsigned char *dest)
{
	unsigned char index_array[4];
	int i, count = 0;
	
	for (i = 0; i < 4; ++i) {
		unsigned char conv;

		if (!conv_letter(src[i], &conv))
			return 0;
		//if (conv == 0)
		//	--count;
		index_array[i] = conv;
	}

	dest[0] = (index_array[0] << 2) | (index_array[1] >> 4);
	dest[1] = (index_array[1] << 4) | (index_array[2] >> 2);
	dest[2] = (index_array[2] << 6) | index_array[3];

	count += 3;
	return count;
}

/* returns 0 on failure (invalid b64 input characters)
 * 1 on success
 */
int decode_b64(const char *src, unsigned char *dest, 
	size_t len, size_t *decoded)
{
	unsigned char buf[3];

	if (len % 4)
		return 0;

	while (len) {
		int decode_count = decode_4chars(src, buf);
		if (decode_count < 1)
			return 0;
		SF_MEMCPY(dest, buf, decode_count);
		src += 4;
		dest += decode_count;
		len -= 4;
		*decoded += decode_count;
	}

	return 1;
}

static void encode_3bytes(const unsigned char *src, char *dest)
{
	dest[0] = b64_table[src[0] >> 2];
	dest[1] = b64_table[((src[0] & 0x03) << 4) | (src[1] >> 4)];
	dest[2] = b64_table[((src[1] & 0x0f) << 2) | ((src[2] & 0xc0) >> 6)];
	dest[3] = b64_table[src[2] & 0x3f];
}

void encode_b64(const unsigned char *src, char *dest, size_t src_len)
{
	char buf[4];
	while (src_len >= 3) {
		encode_3bytes(src, buf);
		SF_MEMCPY(dest, buf, 4);
		src += 3;
		dest += 4;
		src_len -= 3;
	}

	if (src_len) {
		unsigned char src_buf[3] = {0};
		int pad = 3 - src_len;
		char *pad_end = &buf[3];

		SF_MEMCPY(src_buf, src, src_len);
		encode_3bytes(src_buf, buf);
		while (pad--)
			*pad_end-- = '=';
		SF_MEMCPY(dest, buf, 4);
	}
}

static unsigned long allign(unsigned long num, unsigned long to)
{
	return ((num + to - 1) / to) * to;
}

size_t encoded_size(size_t src_len)
{
	return allign(src_len, 3)/3 * 4;
}

size_t decoded_size(const char *encoded_data)
{
	size_t str_len, size;
	str_len = strlen(encoded_data);
	size = str_len/4 * 3;
	if (encoded_data[str_len - 1] == '=') {
		if (encoded_data[str_len - 2] == '=')
			size -= 2;
		else
			size -= 1;
	}
	return size;
}
