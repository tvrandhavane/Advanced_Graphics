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
#include <stdrsl/RadianceSample.h>
#include <stdrsl/ShadingContext.h>
#include <stdrsl/SampleMgrPathTrace.h>

class plausibleEnvlight(
          float intensity = 1;
          color lightcolor = 1;
          string mapname = "";
          float nsamples = 8;
          string coordSys = "shader";
          string distribution = "filteredimportance";
          uniform float radius = 1000;
          string shadowname = "";
          float bias = 0.007;
          string __category = "stdrsl_plausible";
          point photontargetcenter = point(0,0,0);
          float photontargetradius = 100 )
{
    varying color m_lightColor = 1;
    varying normal m_Ns = 0;
    stdrsl_SampleMgrPathTrace m_sampleMgr;

    public void light(output vector L; output color Cl;)
    {
        // dummy method signals that this class is intended for use as light
        L = 0; Cl = 0;
    }

    public void prelighting(output color Ci, Oi) {
        color lc = intensity * lightcolor;
        emit(lc,
                "raytraceshadows", 1, 
                "shadowbias", bias,
                "environmentmap",mapname
                );
        
    }

    public void construct()
    {
        m_sampleMgr->construct();
    }

    public void begin() 
    {
        m_sampleMgr->begin();

        uniform float sides, rayhit;
        string label;
        attribute ("Sides", sides);
        rayinfo("label", label);
        if(label == "__reyes")
            rayhit = 0;
        else
            rayhit = 1;
        stdrsl_ShadingUtils sutils;
        normal nn, nf;
        vector in;
        sutils->GetShadingVars(rayhit, sides, nn, in, nf, m_Ns);
        m_lightColor = intensity * lightcolor;
    }

    public void generateSamples(
            string integrationdomain;
            output __radiancesample rsamples[]) 
    {
        uniform float nnsamples;
        m_sampleMgr->computeLightSamples(4, nsamples, nnsamples);
 
        // Empty mapname string produces ambient occlusion 
        generateSamplesEnv (mapname, Ps, m_Ns, nnsamples, rsamples,
                            "environmentspace", coordSys, 
                            "distribution", distribution, 
                            "radius", radius, 
                            "scale", m_lightColor,
                            "integrationdomain", integrationdomain);
    }

    public void evaluateSamples (
            string integrationdomain;
            output __radiancesample samples[])
    {

        evaluateSamplesEnv(mapname, Ps, m_Ns, samples,
                        "environmentspace", coordSys, 
                        "distribution", distribution, 
                        "radius", radius, 
                        "scale", m_lightColor,
                        "integrationdomain", integrationdomain);
    } 

    public void shadowSamples(output __radiancesample samples[]) 
    {
        filterregion fr;
        fr->calculate3d(Ps); 

        if (shadowname == "") {
            areashadow("", fr, samples, "raytrace", 1, "bias", bias);
        } else {
            areashadow(shadowname, fr, samples, "bias", bias);
        }
    }

    public void generatePhoton(output point origin; output vector direction;
                               output color power; output float pdf)
    {

        // Generate directions with highest probability at bright directions.
        // Note we use a random "starting" point to ensure we sample the
        // map with reasonable randomness. With the spherical domain, the
        // normal is ignored, so we give it (0,0,1).
        __radiancesample rsamples[];
        generateSamplesEnv (mapname, point(random(),random(),random()), 
                            normal(0,0,1), 1, rsamples,
                            "environmentspace", coordSys, 
                            "distribution", "importance", 
                            "radius", photontargetradius, 
                            "scale", m_lightColor,
                            "integrationdomain", "spherical");

        if (arraylength(rsamples) != 0) {
            // Set direction, power, & pdf from our single sample.
            direction = normalize(-(rsamples[0]->direction));
            power = rsamples[0]->radiance;
            pdf = rsamples[0]->lightPdf;
        } else {
            // This sample was rejected; this should never happen.
            // Set the power to zero if it does.
            direction = vector (0,0,1);
            power = 0;
            pdf = 0;
        } 
          
        // Generate axes perpendicular to direction
        vector udir, vdir;
        if (abs(direction[0]) > 0.001 || abs(direction[1]) > 0.001) {
            // udir = n x Z
            udir = vector(direction[1], -direction[0], 0);
        } else {
            // udir = n x X
            udir = vector(0, direction[2], -direction[1]);
        }
        udir = normalize(udir);
        vdir = direction ^ udir;    // vdir = direction x udir

        // Generate origin on disk that is projection of bounding sphere 
        // in chosen direction. 
        // NOTE: photontargetcenter is the center of the scene and 
        //       photontargetradius should be large enough of a sphere
        //       bound to encompass all objects in the scene.
        float angle = M_TWOPI * random();
        float r = sqrt(random());
        origin = photontargetcenter
              + photontargetradius * r * cos(angle) * udir 
              + photontargetradius * r * sin(angle) * vdir;
        origin -= photontargetradius * direction; 
    }
}
