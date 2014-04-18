light indirectlight
(
  float samples = 64, maxvariation = 0.02;
  string envmap = "";
  color filter = color(1);
  output float __nonspecular = 1;
)
{
  normal Nn = normalize(N);

  /* Compute indirect diffuse illumination */
  illuminate (Ps + Nn) {  /* force execution independent of light location */
    Cl = filter * indirectdiffuse(Ps, Nn, samples, "maxvariation", maxvariation,
                                  "environmentmap", envmap);
  }
}