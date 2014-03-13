light 
causticlight(
	color filter = color(1);
	output float __nonspecular = 1;)
{
        normal Ns = normalize(N);
        /* Compute "caustic illumination" */
        illuminate (Ps + Ns) {  /* force execution independent of light location */
                Cl = filter * caustic(Ps, Ns);
        }
}