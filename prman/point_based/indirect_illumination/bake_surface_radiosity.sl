surface bake_surface_radiosity
(
  string bakefile = "" , displaychannels = "", texturename = "";
  float Ka = 0;
  float Kd = 1;
  float Ks =.5;
  float Kr = .5;
  float Kt = .5;
  float maxdepth = 3;
  float eta = 1 ;
  float isSurfaceSpecular = 1 ;
  float roughness = 1/21 ;
)
{

  color irrad, tex = 1, diffcolor;
  normal Nn = normalize(N);
  float a = area(P, "dicing"); // micropolygon area (is 0 on e.g. curves)
    // Compute direct illumination (ambient and diffuse)
    irrad = Ka*ambient() + Kd*diffuse(Nn);

    // Lookup diffuse texture (if any)
    if (texturename != "")
      tex = texture(texturename);

    diffcolor = Cs * tex;

    // Compute Ci and Oi
    Ci = irrad * diffcolor * Os;
    Oi = Os;

    if (a > 0) {
    // Store area and Ci in point cloud file
    bake3d(bakefile, displaychannels, P, Nn, "interpolate", 1,
           "_area", a, "_radiosity", Ci, "Cs", diffcolor);
    }
  

}