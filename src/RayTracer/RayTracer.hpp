
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#pragma once

#include <glm/glm.hpp>

#include <Shading/BRDF.hpp>

#include <Scene/Object.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Light.hpp>
#include <Scene/Scene.hpp>

#include "Params.hpp"
#include "Pixel.hpp"
#include "RayTraceResults.hpp"



struct LightingResults
{
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class RayTracer
{

public:

	RayTracer(const Scene * scene);

	void SetParams(const Params & params);
	const Params & GetParams() const;

	Pixel CastRaysForPixel(const glm::ivec2 & Pixel) const;
	RayTraceResults CastRay(const Ray & ray) const;
	LightingResults GetLightingResults(const Light * const light, const Material & Material, const glm::vec3 & point, const glm::vec3 & view, const glm::vec3 & normal) const;

protected:

	Params params;
	const Scene * scene = nullptr;
	const BRDF * brdf = nullptr;

};
