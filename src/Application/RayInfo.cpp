
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#include "RayInfo.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <string>


void RayInfo::FirstHit(RayTracer * rayTracer, Scene * scene, const int x, const int y)
{
	const Ray ray = scene->GetCamera().GetPixelRay(glm::ivec2(x, y), rayTracer->GetParams().imageSize);
	const RayHitResults results = scene->GetRayHitResults(scene->GetCamera().GetPixelRay(glm::ivec2(x, y), rayTracer->GetParams().imageSize));
	const float intersection = results.t;
	const Object * const object = results.object;

	std::cout << "Pixel: [" << x << ", " << y << "] Ray: " << ray  << std::endl;

	if (object)
	{
		std::cout << "T = " << intersection << std::endl;
		std::cout << "Object Type: " << object->GetObjectType() << std::endl;
		std::cout << "Color: " << object->GetMaterial().color << std::endl;
	}
	else
	{
		std::cout << "No Hit" << std::endl;
	}
}

void RayInfo::PixelColor(RayTracer * rayTracer, Scene * scene, const int x, const int y)
{
	const Pixel pixel = rayTracer->CastRaysForPixel(glm::ivec2(x, y));
	const Ray ray = scene->GetCamera().GetPixelRay(glm::ivec2(x, y), rayTracer->GetParams().imageSize);
	const RayHitResults results = scene->GetRayHitResults(scene->GetCamera().GetPixelRay(glm::ivec2(x, y), rayTracer->GetParams().imageSize));
	const float intersection = results.t;
	const Object * const object = results.object;

	std::cout << "Pixel: [" << x << ", " << y << "] Ray: " << ray << std::endl;

	if (object)
	{
		std::cout << "T = " << intersection << std::endl;
		std::cout << "Object Type: " << object->GetObjectType() << std::endl;
		std::cout << "BRDF: " << "Blinn-Phong" << std::endl;
		std::cout << "Color: " << pixel << std::endl;
	}
	else
	{
		std::cout << "No Hit" << std::endl;
	}
}
