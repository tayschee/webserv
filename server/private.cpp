/*this file regroup all private functions from server.hpp*/

#include "server.hpp"

template <typename Int_type>
Int_type server::reverse_bytes_order(Int_type x) const
{
	int i = 0;
	int size = sizeof(x);
	unsigned char octet[size];
	Int_type rev_x = 0;

	while (i < size)
	{
		octet[i] = ((x >> 8 * i) << 8 * size) >> 8 * size;
		++i;
	}
	for (i = 0; i < size; i++)
	{
		rev_x += octet[i];
		if (i + 1 < size)
			rev_x = rev_x << 8;
	}
	return rev_x;
}

unsigned short server::ft_htons(unsigned short x) const
{
	if (BYTE_ORDER == LITTLE_ENDIAN)
		x = reverse_bytes_order(x);
	return (x);
}
unsigned int server::ft_htonl(unsigned int x) const
{
	if (BYTE_ORDER == LITTLE_ENDIAN)
		x = reverse_bytes_order(x);
	return (x);
}