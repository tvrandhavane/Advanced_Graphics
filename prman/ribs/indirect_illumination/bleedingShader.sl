surface bleedingShader
(
	float Kd = 1;
	float samples = 512;
	string envmap = "";
)
{
	normal Nf = faceforward (N, I);

	color indirect = indirectdiffuse(P, Nf, samples, "environmentmap", envmap, "maxvariation" , 1);

    Ci = Kd * (diffuse(normalize(Nf)) + indirect) * Cs * Os;
    Oi = Os;
}