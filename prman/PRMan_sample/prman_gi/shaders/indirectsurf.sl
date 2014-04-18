surface indirectsurf
(
  float samples = 16, intensity = 1.0, maxvariation = 0.02;
  string envmap = ""
)
{
  normal Nn = normalize(N);
  Ci = diffuse(Nn)
     + intensity * indirectdiffuse(P, Nn, samples, "maxvariation", maxvariation,
                       "environmentmap", envmap);
  Ci *= Cs * Os;   /* for colors and transparency */
  Oi = Os;
}