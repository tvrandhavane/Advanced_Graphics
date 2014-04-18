light occlusionlight2
(
  float samples = 64, maxvariation = 0.02;
  color filter = color(1);
  output float __nonspecular = 1;
)
{
  normal Nn = normalize(N);
  illuminate (Ps + Nn)
  { /* force execution independent of light location */
    float occ = occlusion(Ps, Nn, samples, "maxvariation", maxvariation);
    Cl = filter * (1 - occ);
  }
}