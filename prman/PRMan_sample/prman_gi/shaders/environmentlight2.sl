light environmentlight2
(
  float samples = 64, maxvariation = 0.02;
  string envmap = "";
  color filter = color(1);
  output float __nonspecular = 1;)
{
  normal Nn = normalize(N);

  illuminate (Ps + Nn) { /* force execution independent of light location */

    /* Compute occlusion and average unoccluded dir (environment dir) */
    vector envdir = 0;
    float occ = occlusion(Ps, Nn, samples, "maxvariation", maxvariation,
                          "environmentdir", envdir);

    /* Lookup in pre-blurred environment map */
    color envcolor = environment(envmap, envdir);

    /* Set Cl */
    Cl = filter * (1 - occ) * envcolor;
  }
}