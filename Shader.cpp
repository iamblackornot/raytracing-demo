#include "Shader.h"
#include "SingleDispatcher.h"

#include <cmath>
#include <functional>

Shader::Shader() : m_ambientLightLevel(0) {}

void Shader::SetAmbientLightLevel(double value)
{
    m_ambientLightLevel = value;
}

void Shader::AddDirectionalLight(double level, geometry::Vec3 direction)
{
    m_directionalLights.push_back({ level, direction });
}

void Shader::AddPointLight(double level, geometry::Vec3 position)
{
    m_pointLights.push_back({ level, position });
}

PixelARGB Shader::Apply(ShaderOptions options) const
{
    using namespace geometry;

    Vec3 normalVector = options.hitObject->GetNormalVector(options.hitPoint);
    Vec3 reflectionVector = options.observerPosition - options.hitPoint;

    double cumulativeLightLevel = m_ambientLightLevel;

    for (auto const& dirLight : m_directionalLights)
    {
        if (TraceAny(
            options.hitPoint,
            dirLight.positionOrDirection,
            MAX_TRACE_SHADOW_FROM_DIR_LIGHT_VALUE,
            options.objects))
        {
            continue;
        }

        cumulativeLightLevel += 
            ComputeLightLevel(
                dirLight.level,
                dirLight.positionOrDirection,
                normalVector,
                reflectionVector,
                options.hitObject->specularLevel);
    }

    for (auto const& pointLight : m_pointLights)
    {
        if (TraceAny(
            options.hitPoint,
            pointLight.positionOrDirection - options.hitPoint,
            MAX_TRACE_SHADOW_FROM_POINT_LIGHT_VALUE,
            options.objects))
        {
            continue;
        }

        cumulativeLightLevel +=
            ComputeLightLevel(
                pointLight.level,
                pointLight.positionOrDirection - options.hitPoint,
                normalVector,
                reflectionVector,
                options.hitObject->specularLevel);
    }

    return options.hitObject->color * cumulativeLightLevel;
}

double Shader::ComputeLightLevel(
    double lightLevel,
    geometry::Vec3 const& lightDirection,
    geometry::Vec3 const& normalVector,
    geometry::Vec3 const& reflectionVector,
    std::optional<double> specularLevel) const
{
    using namespace geometry;

    double dotLN = DotProduct(lightDirection, normalVector);

    if (dotLN <= 0)
    {
        return 0;
    }

    double diffuse = dotLN /
        (lightDirection.Length() * normalVector.Length());

    double specular = 0;

    if (specularLevel)
    {
        Vec3 specularReflectionVector =
            2 * dotLN * normalVector - lightDirection;

        double dotRV = DotProduct(specularReflectionVector, reflectionVector);

        if (dotRV > 0)
        {
            specular = dotRV / (specularReflectionVector.Length() * reflectionVector.Length());
            specular = pow(specular, specularLevel.value());
        }
    }

    return lightLevel * (diffuse + specular);
}

bool Shader::TraceAny(
    geometry::Vec3 const& startPosition,
    geometry::Vec3 const& direction,
    double maxTraceValue,
    SceneObjects objects) const
{
    using namespace geometry;

    auto rayTracerCallable = std::bind(
        &RayTracer::Trace,
        &m_rayTracer,
        startPosition,
        direction,
        MIN_TRACE_SHADOW_VALUE,
        maxTraceValue,
        std::placeholders::_1);

    SingleDispatcher<
        RayTracer::TraceResult,
        decltype(rayTracerCallable),
        Geometry,
        GEOMETRY_SET> dispatcher(rayTracerCallable);


    for (std::shared_ptr<Geometry> const& obj : objects)
    {
        RayTracer::TraceResult result = dispatcher.Dispatch(obj.get());

        if (result)
        {
            return true;
        }
    }

    return false;
}

ShaderOptions::ShaderOptions(
    geometry::Vec3 const& hitPoint,
    geometry::Vec3 const& observerPosition,
    std::shared_ptr<geometry::Geometry> hitObject,
    SceneObjects objects)
    :
    hitPoint(hitPoint),
    observerPosition(observerPosition),
    hitObject(hitObject),
    objects(objects) {}