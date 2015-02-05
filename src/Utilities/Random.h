
#ifndef __RANDOM_H__
#define __RANDOM_H__

namespace Random
{
	void				init();
	uint32_t			generateUnsigned(uint32_t min, uint32_t max);
	vector<uint32_t>	generateUnsignedArray(unsigned num, uint32_t min, uint32_t max);
	int32_t				generateInt(int32_t min, int32_t max);
}

#endif//__RANDOM_H__
