#ifndef VOXELS_RETROWAVEWORLDGENERATOR_H_
#define VOXELS_RETROWAVEWORLDGENERATOR_H_

#include "../typedefs.h"
#include "../voxels/WorldGenerator.h"

struct voxel;
class Content;

class RetrowaveWorldGenerator : WorldGenerator {
public:

	RetrowaveWorldGenerator(const Content* content) : WorldGenerator(content) {}

	void generate(voxel* voxels, int x, int z, int seed);
};

#endif /* VOXELS_RETROWAVEWORLDGENERATOR_H_ */