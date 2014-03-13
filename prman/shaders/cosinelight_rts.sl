light cosinelight_rts
(
    float intensity = 1;
    color lightcolor = 1;
    float falloff = 2;   /* default: inverse square fall-off */
    point from = point "shader" (0,0,0);   /* light position */
    vector dir = (0, -1, 0);
)
{
    illuminate(from) {
        float dist = length(L);
	Cl = intensity * lightcolor * pow(dist, -falloff);   /* fall-off */
	Cl *= transmission(Ps, from);   /* ray traced shadow */
    }
}
