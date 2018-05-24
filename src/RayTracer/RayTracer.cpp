
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#include "RayTracer.hpp"

#include <Scene/Scene.hpp>
#include <Shading/BlinnPhongBRDF.hpp>
#include <Shading/CookTorranceBRDF.hpp>


RayTracer::RayTracer(const Scene * scene)
{
	this->scene = scene;
}

void RayTracer::SetParams(const Params & params)
{
	this->params = params;

	if (params.useCookTorrance)
	{
		brdf = new CookTorranceBRDF();
	}
	else
	{
		brdf = new BlinnPhongBRDF();
	}
}

const Params & RayTracer::GetParams() const
{
	return params;
}

Pixel RayTracer::CastRaysForPixel(const glm::ivec2 & pixel) const
{
	const Ray ray = scene->GetCamera().GetPixelRay(pixel, params.imageSize);
	return Pixel(CastRay(ray).ToColor());
}

RayTraceResults RayTracer::CastRay(const Ray & ray) const
{
	const float surfaceEpsilon = 0.001f;

	RayTraceResults results;

	const RayHitResults hitResults = scene->GetRayHitResults(ray);
	const Object * hitObject = hitResults.object;

	if (hitObject)
	{
		const Material & material = hitObject->GetMaterial();

		const glm::vec3 point = results.intersectionPoint = hitResults.point;
		const glm::vec3 view = -glm::normalize(ray.direction);
		const glm::vec3 normal = glm::normalize(hitResults.normal);

		if (params.useShading)
		{
			results.ambient = hitObject->GetMaterial().finish.ambient * hitObject->GetMaterial().color;
		}

		for (Light * light : scene->GetLights())
		{
			const bool inShadow = params.useShadows ?
				scene->IsLightOccluded(point + normal*surfaceEpsilon, light->position) :
				false;

			if (! inShadow)
			{
				const LightingResults lighting = GetLightingResults(light, hitObject->GetMaterial(), point, view, normal);

				results.diffuse += lighting.diffuse;
				results.specular += lighting.specular;
			}
		}

		if (params.debugNormals)
		{
			results.ambient = glm::vec3(0.f);
			results.specular = glm::vec3(0.f);

			results.diffuse = normal / 2.f + 0.5f;
		}
	}

	return results;
}

LightingResults RayTracer::GetLightingResults(const Light * const light, const Material & material, const glm::vec3 & point, const glm::vec3 & view, const glm::vec3 & normal) const
{
	LightingResults Results;

	if (params.useShading)
	{
		SurfaceVectors surface;
		surface.normal = normal;
		surface.view = view;
		surface.light = glm::normalize(light->position - point);

		if (brdf)
		{
			Results.diffuse  = light->color * material.finish.diffuse  * material.color * brdf->CalculateDiffuse(material, surface);
			Results.specular = light->color * material.finish.specular * material.color * brdf->CalculateSpecular(material, surface);
		}
	}
	else
	{
		Results.diffuse = material.color;
	}

	return Results;
}
