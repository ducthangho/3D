#pragma once
#include "xml_format.h"
#include "xnormal.pb.h"
#include <google/protobuf/descriptor.h>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
using namespace y3d;
using namespace xnormal;
#ifdef PUGIXML_WCHAR_MODE 
#define _ToStr(c) std::wstring(L##c)
#else 
#define _ToStr(c) c
#endif



static ReplacementMap rmap = { { _ToStr("eHM2NMMethod_3x3"),_ToStr("3x3") } ,
							{ _ToStr("eHM2NMMethod_4Samples"),_ToStr("4Samples") },
							{ _ToStr("eHM2NMMethod_5x5"), _ToStr("5x5") },
							{ _ToStr("eHM2NMMethod_PRE3x3"), _ToStr("PRE3x3") },
							{ _ToStr("eHM2NMMethod_PRE5x5"), _ToStr("PRE5x5") },
							{ _ToStr("eHM2NMMethod_SO3x3"), _ToStr("SO3x3") },
							{ _ToStr("eHM2NMMethod_SO5x5"), _ToStr("SO5x5") },
							{ _ToStr("eSmoothNormals_AverageNormals"), _ToStr("AverageNormals") },
							{ _ToStr("eSmoothNormals_HardenNormals"), _ToStr("HardenNormals") },
							{ _ToStr("eSmoothNormals_UseExportedNormals"), _ToStr("UseExportedNormals") },
							{ _ToStr("eSwizzleComponent_X0"), _ToStr("X+") },
							{ _ToStr("eSwizzleComponent_X1"), _ToStr("X-") },
							{ _ToStr("eSwizzleComponent_Y0"), _ToStr("Y+") },
							{ _ToStr("eSwizzleComponent_Y1"), _ToStr("Y-") },
							{ _ToStr("eSwizzleComponent_Z0"), _ToStr("Z+") },
							{ _ToStr("eSwizzleComponent_Z1"), _ToStr("Z-") },
							{ _ToStr("eTransparencyMode_1Bit"), _ToStr("1Bit") },
							{ _ToStr("eTransparencyMode_AdditiveGlow"), _ToStr("AdditiveGlow") },
							{ _ToStr("eTransparencyMode_Blend"), _ToStr("Blend") },
							{ _ToStr("eTransparencyMode_None"), _ToStr("None") },
							{ _ToStr("eTransparencyMode_ScreenGlass"), _ToStr("ScreenGlass") },
							{ _ToStr("eNormalMapType_Derivative"), _ToStr("Derivative") },
							{ _ToStr("eNormalMapType_Object_space"), "Object-space" },
							{ _ToStr("eNormalMapType_Tangent_space"), "Tangent-space" },
							{ _ToStr("eAODistribution_Cosine"), _ToStr("Cosine") },
							{ _ToStr("eAODistribution_CosineSq"), _ToStr("CosineSq") },
							{ _ToStr("eAODistribution_Uniform"), _ToStr("Uniform") },
							{ _ToStr("eHMNormalization_Interactive"), _ToStr("Interactive") },
							{ _ToStr("eHMNormalization_Manual"), _ToStr("Manual") },
							{ _ToStr("eHMNormalization_Raw"), _ToStr("Raw") },
							{ _ToStr("eRNMCoordSys_AliB"), _ToStr("AliB") },
							{ _ToStr("eRNMCoordSys_Direct3D"), _ToStr("Direct3D") },
							{ _ToStr("eRNMCoordSys_OpenGL"), _ToStr("OpenGL") },
							{ _ToStr("eAntialiasing_1"), _ToStr("1") },
							{ _ToStr("eAntialiasing_2"), _ToStr("2") },
							{ _ToStr("eAntialiasing_4"), _ToStr("4") },
							{ _ToStr("eCurvAlgorithm_Average"), _ToStr("Average") },
							{ _ToStr("eCurvAlgorithm_Gaussian"), _ToStr("Gaussian") } };

template <class Mesh>
inline void setDefaultLowPolyMesh(Mesh* m) {
	m->set_visible(true);
	m->set_averagenormals(true);
	m->set_maxraydistancefront(0.500000);
	m->set_maxraydistanceback(0.500000);
	m->set_usecage(false);
	m->set_normapmaptype(tLowPolyMesh_eNormalMapType_eNormalMapType_Tangent_space);
	m->set_usepervertexcolors(true);
	m->set_usefresnel(false);
	m->set_fresnelrefractiveindex(1.330000);
	m->set_reflecthdrmult(1.000000);
	m->set_vectordisplacementts(false);
	m->set_vvdmswizzlex(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_X0);
	m->set_vvdmswizzley(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_Y0);
	m->set_vvdmswizzlez(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_Z0);
	m->set_batchprotect(false);
	m->set_castshadows(true);
	m->set_receiveshadows(true);
	m->set_backfacecull(true);
	m->set_nmswizzlex(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_X0);
	m->set_nmswizzley(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_Y0);
	m->set_nmswizzlez(tLowPolyMesh_eSwizzleComponent_eSwizzleComponent_Z0);
	m->set_highpolynormalsoverridetangentspace(true);
	m->set_transparencymode(tLowPolyMesh_eTransparencyMode_eTransparencyMode_None);
	char t[1] = { 127 };
	m->set_alphatestvalue(t, 1);
	m->set_matte(false);
	m->set_scale(1.000000);
	m->set_matchuvs(false);
	m->set_uoffset(0.000000);
	m->set_voffset(0.000000);
	char tmp[3] = { 0,0,0 };
	m->set_positionoffset(tmp, 3);
	//m->set_positionoffset(0.0000; 0.0000; 0.0000);
}

template <class Mesh>
inline void setDefaultHipolyMesh(Mesh* mesh) {
	mesh->set_visible(true);
	mesh->set_scale(1.000000);
	mesh->set_ignorepervertexcolor(true);
	mesh->set_averagenormals(tHighPolyMesh_eSmoothNormals_eSmoothNormals_UseExportedNormals);
	mesh->set_basetexistsnm(false);
	char tmp[3] = { 0,0,0 };
	mesh->set_positionoffset(tmp, 3);
}


template <class Map>
inline void setDefaultGenerateMap(Map* output) {
	output->set_gennormals(true);
	output->set_width(512);
	output->set_height(512);
	output->set_edgepadding(4);
	output->set_bucketsize(32);
	output->set_tangentspace(false);
	output->set_closestiffails(true);
	output->set_discardraybackfaceshits(true);
	output->set_swizzlex(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_X0);
	output->set_swizzley(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Y0);
	output->set_swizzlez(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Z0);
	output->set_aa(tGenerateMaps_eAntialiasing_eAntialiasing_1);
	output->set_bakehighpolybasetexture(false);
	output->set_bakehighpolybasetexturedrawobjectidifnotexture(false);
	output->set_genheights(false);
	output->set_heighttonemap(tGenerateMaps_eHMNormalization_eHMNormalization_Interactive);
	output->set_heighttonemapmin(-100.000000);
	output->set_heighttonemapmax(100.000000);
	output->set_genao(false);
	output->set_aorayspersample(128);
	output->set_aodistribution(tGenerateMaps_eAODistribution_eAODistribution_Uniform);
	output->set_aoconeangle(162.000000);
	output->set_aobias(0.080000);
	output->set_aoallowpureoccluded(true);
	output->set_aolimitraydistance(false);
	output->set_aoattenconstant(1.000000);
	output->set_aoattenlinear(0.000000);
	output->set_aoattencuadratic(0.000000);
	output->set_aojitter(false);
	output->set_aoignorebackfacehits(false);
	output->set_genbent(false);
	output->set_bentrayspersample(128);
	output->set_bentconeangle(162.000000);
	output->set_bentbias(0.080000);
	output->set_benttangentspace(false);
	output->set_bentlimitraydistance(false);
	output->set_bentjitter(false);
	output->set_bentdistribution(tGenerateMaps_eAODistribution_eAODistribution_Uniform);
	output->set_bentswizzlex(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_X0);
	output->set_bentswizzley(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Y0);
	output->set_bentswizzlez(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Z0);
	output->set_genprt(false);
	output->set_prtrayspersample(128);
	output->set_prtconeangle(179.500000);
	output->set_prtbias(0.080000);
	output->set_prtlimitraydistance(false);
	output->set_prtjitter(false);
	output->set_prtnormalize(true);
	output->set_prtthreshold(0.005000);
	output->set_genproximity(false);
	output->set_proximityrayspersample(128);
	output->set_proximityconeangle(80.000000);
	output->set_proximitylimitraydistance(true);
	/*output->set_proximityflipnormals(false);
	output->set_proximityflipvalue(false)*/;
	output->set_genconvexity(false);
	output->set_convexityscale(1.000000);
	//output->set_genthickness(false);
	output->set_gencavity(false);
	output->set_cavityrayspersample(128);
	output->set_cavityjitter(false);
	output->set_cavitysearchradius(0.500000);
	output->set_cavitycontrast(1.250000);
	output->set_cavitysteps(4);
	output->set_genwirerays(false);
	output->set_renderrayfails(true);
	output->set_renderwireframe(true);
	output->set_gendirections(false);
	output->set_directionsts(false);
	output->set_directionsswizzlex(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_X0);
	output->set_directionsswizzley(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Y0);
	output->set_directionsswizzlez(tGenerateMaps_eSwizzleComponent_eSwizzleComponent_Z0);
	output->set_directionstonemap(tGenerateMaps_eHMNormalization_eHMNormalization_Interactive);
	output->set_directionstonemapmin(false);
	output->set_directionstonemapmax(false);
	output->set_genradiositynormals(false);
	output->set_radiositynormalsrayspersample(128);
	output->set_radiositynormalsdistribution(tGenerateMaps_eAODistribution_eAODistribution_Uniform);
	output->set_radiositynormalsconeangle(162.000000);
	output->set_radiositynormalsbias(0.080000);
	output->set_radiositynormalslimitraydistance(false);
	output->set_radiositynormalsattenconstant(1.000000);
	output->set_radiositynormalsattenlinear(0.000000);
	output->set_radiositynormalsattencuadratic(0.000000);
	output->set_radiositynormalsjitter(false);
	output->set_radiositynormalscontrast(1.000000);
	output->set_radiositynormalsencodeao(true);
	output->set_radiositynormalscoordsys(tGenerateMaps_eRNMCoordSys_eRNMCoordSys_AliB);
	output->set_radiositynormalsallowpureocclusion(false);
	output->set_bakehighpolyvcols(false);
	output->set_gencurv(false);
	output->set_curvrayspersample(128);
	output->set_curvbias(0.000100);
	output->set_curvconeangle(162.000000);
	output->set_curvjitter(false);
	output->set_curvsearchdistance(1.000000);
	//output->set_curvtonemap(3col);
	output->set_curvdistribution(tGenerateMaps_eAODistribution_eAODistribution_Cosine);
	output->set_curvalgorithm(tGenerateMaps_eCurvAlgorithm_eCurvAlgorithm_Average);
	//output->set_curvsmoothing(true);
	output->set_genderivnm(false);
	output->set_gentranslu(false);
	output->set_translurayspersample(128);
	output->set_transludistribution(tGenerateMaps_eAODistribution_eAODistribution_Uniform);
	output->set_transluconeangle(162.000000);
	output->set_translubias(0.000500);
	output->set_transludist(1.000000);
	output->set_translujitter(false);

	set_color(output->mutable_nmbackgroundcolor(), 128, 128, 255);
	set_color(output->mutable_bakehighpolybasetexturenotexcol(), 255, 0, 0);
	set_color(output->mutable_bakehighpolybasetexturebackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_hmbackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_aooccludedcolor(), 0, 0, 0);
	set_color(output->mutable_aounoccludedcolor(), 255, 255, 255);
	set_color(output->mutable_aobackgroundcolor(), 255, 255, 255);
	set_color(output->mutable_bentbackgroundcolor(), 127, 127, 255);
	set_color(output->mutable_prtbackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_proximitybackgroundcolor(), 255, 255, 255);
	set_color(output->mutable_convexitybackgroundcolor(), 255, 255, 255);
	set_color(output->mutable_cavitybackgroundcolor(), 255, 255, 255);
	set_color(output->mutable_renderwireframecol(), 255, 255, 255);
	set_color(output->mutable_rendercwcol(), 0, 0, 255);
	set_color(output->mutable_renderseamcol(), 0, 255, 0);
	set_color(output->mutable_renderrayfailscol(), 255, 0, 0);
	set_color(output->mutable_renderwireframebackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_vdmbackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_radnmbackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_bakehighpolyvcolsbackgroundcol(), 255, 255, 255);
	set_color(output->mutable_curvbackgroundcolor(), 0, 0, 0);
	set_color(output->mutable_derivnmbackgroundcolor(), 127, 127, 0);
	set_color(output->mutable_translubackgroundcolor(), 0, 0, 0);
}

template <class T>
inline void set_color(T* c,int r, int g, int b) {
	c->set_r(r);
	c->set_g(g);
	c->set_b(b);
}

inline void setSize(Settings& s, google::protobuf::uint32 width = 512, google::protobuf::uint32 height = 512) {
	auto* output = s.mutable_generatemaps();
	output->set_width(width);
	output->set_height(height);

}

inline void createDefaultSettings(Settings* s,std::string hiPoly,std::string lowPoly,std::string outPath) {
	auto* highPoly = s->mutable_highpolymodel();
	auto* mesh = highPoly->add_mesh();
	mesh->set_file(hiPoly);
	//Now set default value mesh
	setDefaultHipolyMesh(mesh);
	highPoly->set_defaultmeshscale(1.0f);


	auto* loPoly = s->mutable_lowpolymodel();
	loPoly->set_defaultmeshscale(1.0f);
	auto* m = loPoly->add_mesh();
	m->set_file(lowPoly);
	//Now set default value for mesh
	setDefaultLowPolyMesh(m);	


	auto* detail = s->mutable_detail();
	detail->set_scale(0.5f);
	detail->set_method(tDetail_eHM2NMMethod_eHM2NMMethod_4Samples);


	auto* viewer3D = s->mutable_viewer3d();
	viewer3D->set_showgrid(true);
	viewer3D->set_showwireframe(false);
	viewer3D->set_showtangents(false);
	viewer3D->set_shownormals(false);
	viewer3D->set_showblockers(false);
	viewer3D->set_maxtessellationlevel(0);
	viewer3D->set_lightintensity(1.000000);
	viewer3D->set_lightindiretintensity(0.0f);	
	viewer3D->set_exposure(0.180000);
	viewer3D->set_hdrthreshold(0.900000);
	viewer3D->set_useglow(true);
	viewer3D->set_glowintensity(1.000000);
	viewer3D->set_ssaoenabled(false);
	viewer3D->set_ssaobright(1.100000);
	viewer3D->set_ssaocontrast(1.000000);
	viewer3D->set_ssaoatten(1.000000);
	viewer3D->set_ssaoradius(0.250000);
	viewer3D->set_ssaoblurradius(2.000000);
	viewer3D->set_parallaxstrength(0.000000);
	viewer3D->set_showhighpolys(true);
	viewer3D->set_showao(false);
	viewer3D->set_cageopacity(0.700000);
	viewer3D->set_diffusegiintensity(1.000000);
	viewer3D->set_castshadows(false);
	viewer3D->set_shadowbias(0.100000);
	viewer3D->set_shadowarea(0.250000);
	viewer3D->set_axisscl(0.040000);
	viewer3D->set_cameraorbitdistance(0.500000);
	viewer3D->set_cameraorbitautocenter(true);
	viewer3D->set_showstarfield(false);
	set_color(viewer3D->mutable_lightambientcolor(), 33, 33, 33);
	set_color(viewer3D->mutable_lightdiffusecolor(), 229, 229, 229);
	set_color(viewer3D->mutable_lightspecularcolor(), 255, 255, 255);
	set_color(viewer3D->mutable_lightsecondarycolor(), 0, 0, 0);
	set_color(viewer3D->mutable_lighttertiarycolor(), 0, 0, 0);
	set_color(viewer3D->mutable_backgroundcolor(), 0, 0, 0);
	set_color(viewer3D->mutable_gridcolor(), 180, 180, 180);
	set_color(viewer3D->mutable_cagecolor(), 76, 76, 76);
	auto* rot = viewer3D->mutable_camerarotation();
	rot->set_e11(1.0f);
	rot->set_e12(0);
	rot->set_e13(0.0f);

	rot->set_e21(0.0f);
	rot->set_e22(1.0f);
	rot->set_e23(0.0f);

	rot->set_e31(0.0f);
	rot->set_e32(0.0f);
	rot->set_e33(1.0f);

	auto* pos = viewer3D->mutable_cameraposition();
	pos->set_x(0.0f);
	pos->set_y(1.0f);
	pos->set_z(0.0f);

	auto* lpos = viewer3D->mutable_lightposition();
	lpos->set_x(0.0f);
	lpos->set_y(2.0f);
	lpos->set_z(5.0f);
	
	auto* output = s->mutable_generatemaps();
	output->set_file(outPath);
	//Now set default values for output (GenerateMaps)
	setDefaultGenerateMap(output);

}


inline void bakeNormal(Settings& s,std::string tmpLocation = "D:/example.xml") {
	pugi::xml_document doc;
	google::protobuf::XmlFormat::MessageToDOM(s, &doc, &rmap);
	bool saveSucceeded = doc.save_file(tmpLocation.c_str());
	if (saveSucceeded) {
		std::string cmd = "xNormal ";
		cmd += tmpLocation;
		auto rs = system(cmd.c_str());
	}
}