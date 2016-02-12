rw_mesh
=======
[![Build Status](https://travis-ci.org/BelokrysFedotov/rw_mesh.svg?branch=master)](https://travis-ci.org/BelokrysFedotov/rw_mesh)
[![build status](http://git.masoft.ru/ci/projects/2/status.png?ref=master)](http://git.masoft.ru/ci/projects/2?ref=master)

Lib for reading/writing 3d mesh in simple 3d formats.
Supports triangle, quad and mixed surface mesh, stuctured and unstructed 3d volume mesh

Supportes formats:
* Geomview OFF (*.off, *coff, *.noff and etc)
* Medit (*.mesh)
* VTK 4.2 (ascii and binary; only structed and unstructed mesh)
* Plot3D (only read)
* CGNS (only write, using cgnslib)

In future:
* Tecplot(*.dat/*.tec)
* Gambit Neutral File Format (*neu)
* Obj
* Polygon File Format/Stanford Triangle Format (*.ply)
* StarCD
