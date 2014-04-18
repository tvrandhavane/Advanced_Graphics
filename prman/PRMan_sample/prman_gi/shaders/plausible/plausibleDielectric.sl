/* $Revision: #1 $ $Date: 2013/07/30 $
# ------------------------------------------------------------------------------
#
# Copyright (c) 2011 Pixar Animation Studios. All rights reserved.
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
#include <stdrsl/SampleMgr.h>
#include <stdrsl/Lambert.h>
#include <stdrsl/Colors.h>

//  plausibleDielectric:
//  A plausible anisotropic dielectric material. Implements a basic amount 
//  of functionality needed to shade plausible dielectric materials.
//  Uses a stdrsl specular component (stdrsl_SpecularAS) with
//  a simple Lambertian diffuse term, weighted by the transmission coefficient.

class plausibleDielectric(
    uniform float roughness = 0.001;
    uniform float anisotropy = 0;
    uniform float specularGain = 1;
    uniform float diffuseGain = .5;
    uniform color specularColor = color(1);
    uniform color surfaceColor = color(1,1,1);
    uniform float ior = 1.33;
    uniform float mediaIor = 1;
    uniform float displacementAmount = 0;
    uniform string specularMap = "";
    uniform string surfaceMap = "";
    uniform string roughnessMap = "";
    uniform string displacementMap = "";
    uniform float minSpecularSamples = 1;
    uniform float maxSpecularSamples = 16;
    uniform float minDiffuseSamples = 0;
    uniform float maxDiffuseSamples = 0;
    uniform float indirectDiffuseSamples = 64;
    uniform float indirectDiffuseMaxVar = .02;
    uniform float indirectDiffuseMaxDistance = 1e10;
    uniform float specularMaxDistance = 1e10;
    uniform float specularBroadening = 1;
    uniform string indirectSpecularSubset = "";
    uniform string lightCategory = "";
    uniform float applySRGB = 1;
    uniform string causticMap = "";
    uniform float photonEstimator = 100;    
    uniform float __computesOpacity = 0;
    varying float _roughess = -1; // optional input from gather or indspec
    )
{
    // Member variables
    stdrsl_ShadingContext m_shadingCtx;
    stdrsl_Fresnel m_fres;
    stdrsl_Lambert m_diffuse;
    stdrsl_SpecularAS m_specular;
    varying float m_roughnessToSend;

    public void construct() {
        m_shadingCtx->construct();
    }

    public void begin() 
    {
    }

    public void displacement(output point P;output normal N) 
    {
        if(displacementMap != "" && displacementAmount != 0) {
            float displacementValue = 
                displacementAmount * texture(displacementMap[0]);
            m_shadingCtx->displace(vector(normalize(N)), displacementValue,
                                   "displace");
        }
        //Initialize the shading context, using 's' as the input for
        //computing the tangents needed for anisotropic specular
        m_shadingCtx->initWithTangentField(s);
    }
    
    public void prelighting(output color Ci,Oi) 
    {
    }

    //Shared initialization function for diffuse.
    public void initDiff() 
    {
        color surfColor = surfaceColor * Cs;
        if(surfaceMap!= "")
        {
            color c = color texture(surfaceMap);
            if(applySRGB != 0)
                c = srgbToLinear(c);
            surfColor *= c; 
        }
        m_diffuse->init(m_shadingCtx,surfColor*diffuseGain,minDiffuseSamples,
            maxDiffuseSamples);
    }

    // Shared initialization function for specular.
    public void initSpec() 
    {
        m_fres->init(m_shadingCtx, mediaIor, ior);

        color specColor = specularColor; 
        if(specularMap != "")
        {
            color c = color texture(specularMap);
            if(applySRGB != 0) c = srgbToLinear(c);
            specColor *= c;
        }

        varying float roughscale = 1;
        if(roughnessMap!= "")
        { 
            roughscale = texture(roughnessMap[0]);
        }
        if(specularBroadening == 0)
            m_specular->initWithDownstreamRoughness(-1,
                         m_shadingCtx, 
                         specularGain*specColor*m_fres->m_Kr,
                         roughness*roughscale, anisotropy,
                         minSpecularSamples, maxSpecularSamples,
                         m_roughnessToSend);
        else
            m_specular->initWithDownstreamRoughness(_roughess,
                         m_shadingCtx, 
                         specularGain*specColor*m_fres->m_Kr,
                         roughness*roughscale, anisotropy,
                         minSpecularSamples, maxSpecularSamples,
                         m_roughnessToSend);
    }

    public void lighting(output color Ci, Oi) 
    {
        initDiff();
        initSpec();

        __radiancesample samps[]; // filled by indirectspecular

        uniform float sampleBase = (m_shadingCtx->m_RayDepth > 0) ? 0 : 1;
            // On Ray hits, we want no sample-base jittering.

        color indspec = indirectspecular(this, "samplebase", sampleBase,
                                 "subset", indirectSpecularSubset,
                                 "maxdist", specularMaxDistance,
                                 "materialsamples", samps,
                                 "send:surface:_roughness", m_roughnessToSend);

        color diff, spec;
        
        if (m_diffuse->m_sampleCount > 0) {
            directlighting(this, 
                          getlights("category",lightCategory),
                          "specularresult", spec, 
                          "diffuseresult", diff,
                          "mis", 1, /* light plus material samples */
                          "diffusemis",1);
            // can't reuse indirectspec samples
        } else {
            directlighting(this, 
                          getlights("category",lightCategory),
                          "specularresult", spec, 
                          "diffuseresult", diff,
                          "mis", 1, /* light plus material samples */
                          "diffusemis",0,
                          "materialsamples", samps);

        }

        if(indirectDiffuseSamples > 0) 
        {
            uniform float idiffSamps, idiffMaxVar;
            m_shadingCtx->m_SampleMgr->computeIndirectDiffuseSamples(
                                                  indirectDiffuseSamples, 
                                                  indirectDiffuseMaxVar,
                                                  idiffSamps, idiffMaxVar);
            diff += m_diffuse->m_diffColor * 
                      indirectdiffuse(P, m_shadingCtx->m_Ns, idiffSamps, 
                                    "adaptive", 1,
                                    "maxvariation", idiffMaxVar,
                                    "maxdist", indirectDiffuseMaxDistance);
        }

        if(causticMap != "") 
        {
            diff += m_diffuse->m_diffColor * 
                        photonmap(causticMap, P, m_shadingCtx->m_Ns,
                                  "estimator", photonEstimator);
        }

        Ci = diff * m_fres->m_Kt + spec + indspec; // fresnel already in spec
    }

    public void diffuselighting(output color Ci, Oi) 
    {
        // produce view-independent diffuse response (no fresnel)
        initDiff();

        uniform float diffuseMIS = 0;
        if (m_diffuse->m_sampleCount > 0) diffuseMIS = 1;

        Ci += directlighting(this, getlights("category",lightCategory),
                                            "diffusemis",diffuseMIS);

        // Reduce the number of indirect samples based on diffuse ray depth
        if(indirectDiffuseSamples > 0) 
        {
            uniform float idiffSamps, idiffMaxVar;
            m_shadingCtx->m_SampleMgr->computeIndirectDiffuseSamples(
                                                  indirectDiffuseSamples, 
                                                  indirectDiffuseMaxVar,
                                                  idiffSamps, idiffMaxVar);
            if(idiffSamps > 0) 
            {
                Ci += m_diffuse->m_diffColor * 
                        indirectdiffuse(P, m_shadingCtx->m_Ns, idiffSamps,
                                        "adaptive", 1,
                                        "maxvariation", idiffMaxVar,
                                        "maxdist", indirectDiffuseMaxDistance);
            }
        }
        Ci += caustic(P,N);
    }

    public void specularlighting(output color Ci,Oi) 
    {
        initSpec();
        color diff = Ci;
        color spec;
        __radiancesample samps[];

        uniform float sampleBase = (m_shadingCtx->m_RayDepth > 0) ? 0 : 1;
            // On Ray hits, we want no sample-base jittering.

        color indspec = indirectspecular(this, "samplebase", sampleBase,
                                 "subset", indirectSpecularSubset,
                                 "maxdist", specularMaxDistance,
                                 "materialsamples", samps,
                                 "send:surface:_roughness", m_roughnessToSend);

        // specular only do not do diffuse MIS
        directlighting(this, 
                      getlights("category",lightCategory),
                      "specularresult", spec, 
                      "mis", 1, /* light plus material samples */
                      "diffusemis",0,
                      "materialsamples", samps);
        
        Ci = diff * m_fres->m_Kt + spec + indspec;
    }

    public void evaluateSamples(string distribution;
                                output __radiancesample samples[] ) 
    {
        if (distribution == "diffuse")
            m_diffuse->evalDiffuseSamps(m_shadingCtx, m_fres, samples);
        else if (distribution == "specular")
            m_specular->evalSpecularSamps(m_shadingCtx, m_fres, samples);
    }

    public void generateSamples(string type; output __radiancesample samples[])
    {
        uniform float specSampleCount = 0;
        uniform float diffSampleCount = 0;

        // Note: diffuse _must_ come first

        if (m_diffuse->m_sampleCount > 0) {
            m_diffuse->genDiffuseSamps(m_shadingCtx, m_fres, samples);
            diffSampleCount = arraylength(samples);
        }

        if (type != "diffuse") {
            m_specular->genSpecularSamps(m_shadingCtx, m_fres, type, samples);
            specSampleCount = arraylength(samples) - diffSampleCount;
        }
         

        if (m_diffuse->m_sampleCount > 0) {
            if (type != "diffuse") {
                uniform float sampleCounts[2] = {diffSampleCount,specSampleCount};
                varying color lobeWeights[2] = {color(1),color(1)};
                normalizeMaterialResponse(samples,sampleCounts);//,lobeWeights);
            }
        }
    }
}
