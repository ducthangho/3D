#pragma once
#include "xnormal.pb.h"

inline void createDefaultSettings(SettingsType* s,std::string hiPoly,std::string lowPoly,std::string outPath) {
	auto* highPoly = s->mutable_highpolymodel();
	auto* mesh = highPoly->add_mesh();
	mesh->set_file(hiPoly);

	auto* loPoly = s->mutable_lowpolymodel();
	auto* m = loPoly->add_mesh();
	m->set_file(lowPoly);

	auto* output = s->mutable_generatemaps();
	output->set_file(outPath);
	
	//Now set default values
	output->set_gennormals(true);
	output->set_width(512);
	output->set_height(512);
	output->set_edgepadding(16);
	output->set_bucketsize(32);
	output->set_tangentspace(true);
	output->set_closestiffails(true);
	output->set_discardraybackfaceshits(true);
	output->set_swizzlex(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_X);
	output->set_swizzley(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Y);
	output->set_swizzlez(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Z);
}
