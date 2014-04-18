surface occsurf1
(
  float samples = 64
)
{
  normal Nn = normalize(N);

  /* Compute occlusion */
  float hits = 0;
  gather("illuminance", P, Nn, PI/2, samples, "distribution", "cosine") {
    hits += 1;
  }
  float occlusion = hits / samples;

  /* Set Ci and Oi */
  Ci = (1.0 - occlusion) * Cs;
  Oi = 1;
}