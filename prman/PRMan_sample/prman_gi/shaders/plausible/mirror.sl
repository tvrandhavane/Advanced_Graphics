
//
// This shader uses the environment shadeop to show the lookups
// in the given coordSys frame
//

surface
mirror(string mapname = "", coordSys = "";)
{
    color irrad = 0;

    irrad = environment (mapname, vtransform (coordSys, normalize (N)));

    /* Set Ci and Oi */
    Ci = irrad * Cs;
    Oi = Os;
}
