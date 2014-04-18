/* $Id: //depot/branches/rmanprod/rman-18.0/shaders/matte.sl#1 $  (Pixar - RenderMan Division)  $Date: 2013/07/30 $ */
/*
** Copyright (c) 1999 PIXAR.  All rights reserved.  This program or
** documentation contains proprietary confidential information and trade
** secrets of PIXAR.  Reverse engineering of object code is prohibited.
** Use of copyright notice is precautionary and does not imply
** publication.
**
**                      RESTRICTED RIGHTS NOTICE
**
** Use, duplication, or disclosure by the Government is subject to the
** following restrictions:  For civilian agencies, subparagraphs (a) through
** (d) of the Commercial Computer Software--Restricted Rights clause at
** 52.227-19 of the FAR; and, for units of the Department of Defense, DoD
** Supplement to the FAR, clause 52.227-7013 (c)(1)(ii), Rights in
** Technical Data and Computer Software.
**
** Pixar
** 1001 West Cutting Blvd.
** Richmond, CA  94804
*/
surface
matte( float Ka=1, Kd=1 )
{
    normal Nf;

    Nf = faceforward(normalize(N),I);

    Oi = Os;
    Ci = Os * Cs * ( Ka*ambient() + Kd*diffuse(Nf) ) ;
}
