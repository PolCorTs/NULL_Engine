#include "JSONParser.h"

#include "R_Animation.h"

R_Animation::R_Animation() : Resource(RESOURCE_TYPE::ANIMATION)
{

}

R_Animation::~R_Animation()
{

}

bool R_Animation::CleanUp()
{
	bool ret = true;



	return ret;
}

bool R_Animation::SaveMeta(ParsonNode& meta_root) const
{
	bool ret = true;

	ParsonArray contained_array = meta_root.SetArray("ContainedResources");

	ParsonNode settings = meta_root.SetNode("ImportSettings");
	animation_settings.Save(settings);

	return ret;
}

bool R_Animation::LoadMeta(const ParsonNode& meta_root)
{
	bool ret = true;



	return ret;
}