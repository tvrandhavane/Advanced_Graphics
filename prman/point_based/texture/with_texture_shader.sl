surface with_texture_shader
(
  string filename = "", sides = "front";
  string texturename = "";
  float clampbleeding = 1, sortbleeding = 1,
	maxdist = 1e15, falloff = 0, falloffmode = 0,
	samplebase = 0, bias = 0,
	maxsolidangle = 0.05, maxvariation = 0, intensity = 1.0, clampocclusion = 0;
  string causticmap = "";
)
{
  normal Nn = normalize(N);
  color irr = 0, tex = 1, diffcolor;
  float occ = 0;

  irr = intensity * indirectdiffuse(P, Nn, 0, "pointbased", 1, "filename", filename,
                        "hitsides", sides,
                        "clamp", clampbleeding,
                        "sortbleeding", sortbleeding,
                        "maxdist", maxdist, "falloff", falloff,
                        "falloffmode", falloffmode,
                        "samplebase", samplebase, "bias", bias,
                        "maxsolidangle", maxsolidangle,
                        "maxvariation", maxvariation);
  occ = 0.4 * occlusion(P, Nn, 0, "pointbased", 1, "filename", filename,
        "hitsides", sides,
        "maxdist", maxdist, "falloff", falloff,
        "falloffmode", falloffmode,
        "samplebase", samplebase, "clamp", clampocclusion,
        "maxsolidangle", maxsolidangle);
  // Lookup diffuse texture (if any)
  if (texturename != "")
    tex = texture(texturename);

  diffcolor = Cs * tex;
  Ci = Os * Cs * irr * diffcolor * (1 - occ);
  Ci += photonmap(causticmap, P, Nn, "estimator", 50);
  Oi = Os;
}