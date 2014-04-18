/* $Revision: #19 $ $Date: 2011/04/22 $
# ------------------------------------------------------------------------------
#
# Copyright (c) 2013 Pixar Animation Studios. All rights reserved.
#
# The information in this file is provided for the exclusive use of the
# software licensees of Pixar.  It is UNPUBLISHED PROPRIETARY SOURCE CODE
# of Pixar Animation Studios; the contents of this file may not be disclosed
# to third parties, copied or duplicated in any form, in whole or in part,
# without the prior written permission of Pixar Animation Studios.
# Use of copyright notice is precautionary and does not imply publication.
#
# PIXAR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
# ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL PIXAR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
# ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
# SOFTWARE.
#
# Pixar
# 1200 Park Ave
# Emeryville CA 94608
#
# ------------------------------------------------------------------------------
*/

#include <stdrsl/ShadingContext.h>
#include <stdrsl/RadianceSample.h>
#include <stdrsl/SpecularAS.h>
#include <stdrsl/Lambert.h>
#include <stdrsl/Colors.h>

// plausibleSubSurface:
// A plausible material with subsurface scattering.
// Implements a basic amount of functionality needed to shade materials
// with subsurface scattering and specular reflection.
//
// Indirect diffuse illum (at subsurface ray hits) is off by default for
// efficiency; turn it on by setting "indirectAtSss" to 1.
//
// Subsurface scattering at diffuse ray hits is off by default for
// efficiency; turn in on by setting sssAtIndirect to 1.

// In this shader, the surface albedo is ignored where the illumination
// enters the surface; it is only taken into account where the subsurface
// illum leaves the surface (the shading point).  An alternative is to
// use the albedo where the illumination enters the surface but ignore it
// where it leaves, or use the square root of the albedo both places.
// It is wrong to use the full albedo both where the light enters the surface
// and where it leaves it; this leads to too dark and saturated subsurface
// scattering results.

class plausibleSubSurface(
    uniform float roughness = 0.001;
    uniform float anisotropy = 0;
    uniform float specularGain = 1;
    uniform float diffuseGain = 1;
    uniform color specularColor = color(1);
    uniform color surfaceColor = color(0.830, 0.791, 0.753); // default: marble
    uniform float ior = 1.5; // default: marble
    uniform float mediaIor = 1;
    uniform float displacementAmount = 0;
    uniform string specularMap = "";
    uniform string surfaceMap = "";
    uniform string roughnessMap = "";
    uniform string displacementMap = "";
    uniform float minSpecularSamples = 1;
    uniform float maxSpecularSamples = 16;
    uniform float indirectAtSss = 0; // indir diffuse at sss hits off by default
    uniform float indirectDiffuseSamples = 64;
    uniform float indirectDiffuseMaxVar = .02;
    uniform float indirectDiffuseMaxDistance = 1e10;
    uniform float specularMaxDistance = 1e10;
    uniform float specularBroadening = 1;
    uniform string indirectSpecularSubset = "";
    uniform string lightCategory = "";
    uniform float applySRGB = 1;
    uniform float sssAtIndirect = 0; // sss at indir diffuse hits off by default
    uniform string sssType = ""; // dipole diffusion is default
    uniform float sssSamples = 16;
    uniform color sssDMFP = color(8.51, 5.57, 3.95); // default: marble
    uniform float sssUnitLength = 0.1;
    uniform float sssMaxDistance = 1e10;
    uniform float sssSmooth = 0;
    uniform float sssSwitchToBRDF = 0.0;
    uniform float __computesOpacity = 0;
    )
{
    // Member variables
    shader m_lights[];
    stdrsl_ShadingContext m_shadingCtx;
    stdrsl_Fresnel m_fres;

    stdrsl_Lambert m_diffuse;
    stdrsl_SpecularAS m_specular;
    uniform float m_indirectSamples;
    uniform float sssOffset = -1;

    public void construct() {
        m_shadingCtx->construct();
    }

    public void begin() 
    {
    }

    public void displacement(output point P; output normal N) 
    {
        if (displacementMap != "" && displacementAmount != 0) {
            float displacementValue = texture(displacementMap[0]);
            m_shadingCtx->displace(normalize(N),displacementAmount,"displace");
        }
        // Initialize the shading context, using 's' as the input for
        // computing the tangents needed for anisotropic specular
        m_shadingCtx->initWithTangentField(s);
    }
  
    public void prelighting(output color Ci,Oi) 
    {
        m_lights = getlights("category", lightCategory);
    }

    // Shared initialization function for diffuse.
    public void initDiff() 
    {
        color surfColor = surfaceColor * Cs;
        if (surfaceMap!= "") {
            color c = color texture(surfaceMap);
            if (applySRGB != 0)
                c = srgbToLinear(c);
            surfColor *= c; 
        }
        m_diffuse->init(surfColor*diffuseGain);
    }

    // Shared initialization function for specular.
    public void initSpec() 
    {
        m_fres->init(m_shadingCtx, mediaIor, ior);

        color specColor = specularColor; 
        if (specularMap != "") {
            color c = color texture(specularMap);
            if (applySRGB != 0)
                c = srgbToLinear(c);
            specColor *= c;
        }

        varying float roughscale = 1;
        if (roughnessMap!= "") { 
            roughscale = texture(roughnessMap[0]);
        }

        m_specular->init(m_shadingCtx, specularGain*specColor*m_fres->m_Kr,
                         roughness, anisotropy, roughscale,
                         minSpecularSamples, maxSpecularSamples);
    }

    public void lighting(output color Ci, Oi) 
    {
        initDiff();
        initSpec();

        color sss, spec;

        // Compute direct and indirect specular

        directlighting(this, m_lights, "specularresult", spec);

        spec += indirectspecular(this, "samplebase", 1,
                                 "subset", indirectSpecularSubset,
                                 "maxdist", specularMaxDistance);

        // Do *not* compute direct and indirect diffuse -- subsurface
        // handles those !!

        sss = subsurface("raytraced", P, m_shadingCtx->m_Nn,
                         "type", sssType,
                         "albedo", m_diffuse->m_diffColor,
                         "diffusemeanfreepath", sssDMFP,
                         "ior", ior, "unitlength", sssUnitLength,
                         "samples", sssSamples, "maxdist", sssMaxDistance,
                         "smooth", sssSmooth, "switchtobrdf", sssSwitchToBRDF,
                         "offset", sssOffset);

        Ci = sss * m_fres->m_Kt + spec; // fresnel already in spec
    }

    // Compute view-independent diffuse response (no specular, no fresnel).
    // diffuselighting() is called at diffuse and subsurface ray hit points.
    public void diffuselighting(output color Ci, Oi, Ii) 
    {
        initDiff();

        string raylabel, raytype;
        rayinfo("label", raylabel);
        rayinfo("type", raytype);

        if (raylabel == "subsurface") {

            // Compute direct diffuse and irradiance.
            directlighting(this, m_lights,
                           "diffuseresult", Ci,
                           "irradianceresult", Ii);
            // Divide directlighting() irradianceresult Ii by pi (similar to
            // what directlighting() does internally for diffuse: mult by
            // albedo / pi).
            Ii *= 1/PI;

            if (indirectAtSss != 0 && indirectDiffuseSamples > 0) {

                // Reduce number of indirect diffuse samples
                uniform float idiffSamps, idiffMaxVar;
                color indirect;
                m_shadingCtx->m_SampleMgr->computeIndirectDiffuseSamples(
                                                   indirectDiffuseSamples, 
                                                   indirectDiffuseMaxVar,
                                                   idiffSamps, idiffMaxVar);

                // Compute indirect diffuse and irradiance at subsurface ray hit
                indirect = indirectdiffuse(P, m_shadingCtx->m_Ns,
                                           idiffSamps,
                                           "maxvariation", idiffMaxVar,
                                           "maxdist", indirectDiffuseMaxDistance);
                Ci += m_diffuse->m_diffColor * indirect;
                Ii += indirect;
            }

        } else if (raytype != "diffuse" ||
                   (raytype == "diffuse" && sssAtIndirect != 0)) {

            // Reduce number of subsurface samples
            uniform float sssSamps, dummyMaxVar;
            m_shadingCtx->m_SampleMgr->computeIndirectDiffuseSamples(
                                                       sssSamples, 0.0,
                                                       sssSamps, dummyMaxVar);

            // Compute subsurface scattering at ray hit
            Ci = subsurface("raytraced", P, m_shadingCtx->m_Nn,
                            "type", sssType,
                            "albedo", m_diffuse->m_diffColor,
                            "diffusemeanfreepath", sssDMFP,
                            "ior", ior, "unitlength", sssUnitLength,
                            "samples", sssSamps, "maxdist", sssMaxDistance,
                            "smooth", sssSmooth, "switchtobrdf",sssSwitchToBRDF,
                            "offset", sssOffset);
        }
    }

    public void specularlighting(output color Ci,Oi) 
    {
        initSpec();
        color diff = Ci;
        color spec = 0;

        // On ray hits, we want no sample-base jittering
        uniform float sampleBase = (m_shadingCtx->m_RayDepth > 0) ? 0 : 1;

        // Compute direct and indirect specular

        directlighting(this, m_lights, "specularresult", spec);

        spec += indirectspecular(this, "samplebase", sampleBase,
                                 "subset", indirectSpecularSubset,
                                 "maxdist", specularMaxDistance);

        Ci = diff * m_fres->m_Kt + spec; // fresnel already in spec
    }

    public void evaluateSamples(string distribution;
                                output __radiancesample samples[]) 
    {
        if (distribution == "diffuse")
            m_diffuse->evalDiffuseSamps(m_shadingCtx, m_fres, samples);
        else if (distribution == "specular")
            m_specular->evalSpecularSamps(m_shadingCtx, m_fres, samples);
        else {
            uniform float i, n=arraylength(samples);
            for (i=0; i < n; i += 1) {
                samples[i]->materialResponse = 0;
            }
        }
    }

    public void generateSamples(string type; output __radiancesample samples[])
    {
        m_specular->genSpecularSamps(m_shadingCtx, m_fres, type, samples);
    }
}
