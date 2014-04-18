light occlusionlight1
(
  float samples = 64;
  color filter = color(1);
  output float __nonspecular = 1;
)
{
  normal Nn = normalize(N);

  illuminate (Ps + Nn)
  {  /* force execution independent of light location */

    /* Compute occlusion */
    float hits = 0;
    gather("illuminance", Ps, Nn, PI/2, samples,
           "distribution", "cosine")
    {
      hits += 1;
    }
    float occlusion = hits / samples;

    /* Set Cl */
    Cl = filter * (1 - occlusion);
  }
}