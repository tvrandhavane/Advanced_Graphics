surface specularSurface
(
	float Ka = 0;
	float Kd = 1;
	float Ks =.5;
	float Kr = .5;
	float Kt = .5;
	float maxdepth = 3;
	float eta = 1 ;
	float roughness = 1/21 ;
	string causticmap = "";
)
{

	normal Nf = faceforward (N, I);
	vector V = -normalize (I);
	uniform float curDepth = 0;
	uniform float success = rayinfo("depth" ,curDepth);

	if(curDepth < maxdepth){
		vector reflectedRay = reflect (I, normalize (Nf));
		color reflect_color = trace (P , normalize (reflectedRay));
		color refract_color;
		if(N.Nf < 0){

			vector refractedRay = refract(I, normalize(Nf), 1/eta);
			refract_color = trace (P , normalize (refractedRay ));
		}
		else{
			vector refractedRay = refract(I, normalize(Nf), eta);
			refract_color = trace (P , normalize (refractedRay ));
			}

		Oi = Os ;
		Ci = Cs * ( Kd* diffuse(normalize (Nf)) + Ks * specular(normalize (Nf) , V , roughness  ) ) + Kr * reflect_color + Kt * refract_color ;
		Ci += 0.3*photonmap(causticmap, P, Nf, "estimator", 50);

	}
	else {
		Oi = Os ;
		Ci = Cs * ( Kd* diffuse(normalize (Nf)) + Ks * specular(normalize (Nf) , V , roughness  ) ) ;
		Ci += 0.3*photonmap(causticmap, P, Nf, "estimator", 50);
	}

}