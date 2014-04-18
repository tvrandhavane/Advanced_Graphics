light environmentlight1
(
  float samples = 64;
  string envmap = "";
  color filter = color(1);
  output float __nonspecular = 1;
)
{
  normal Nn = normalize(N);

  illuminate (Ps + Nn) { /* force execution independent of light location */

    /* Compute occlusion and average unoccluded dir (environment dir) */
    vector dir = 0, envdir = 0;
    float hits = 0;
    gather("illuminance", Ps, Nn, PI/2, samples,
           "distribution", "cosine", "ray:direction", dir) {
      hits += 1;
    } else { /* ray miss */
      envdir += dir;
    }
    float occ = hits / samples;
    envdir = normalize(envdir);

    /* Lookup in pre-blurred environment map */
    color envcolor = environment(envmap, envdir);

    /* Set Cl */
    Cl = filter * (1 - occ) * envcolor;
  }
}