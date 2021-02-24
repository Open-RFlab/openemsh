[![Liberapay patrons](https://img.shields.io/liberapay/patrons/thomaslepoix.svg?logo=liberapay)](https://liberapay.com/thomaslepoix/donate)
[![Discord](https://img.shields.io/discord/616889479298547722?logo=discord)](https://discord.gg/P82fEmE)
[![License](https://img.shields.io/github/license/Open-RFlab/openemsh)](LICENSE)
[![Version](https://img.shields.io/github/v/release/Open-RFlab/openemsh)](https://github.com/Open-RFlab/openemsh/releases/latest)

# OpenEMSH : OpenEMS Mesher [WIP]

## Abstract

This is an OpenEMS mesher draft (with a H at the end, it looks like mesh poorly written). Meshing an OpenEMS structure is a difficult task often done manually. Some automatic meshers exist, shipped with either an Octave/Python API or a converter tool from a PCB design software.

By creating myself one of those converter ([Qucs-RFlayout](https://github.com/thomaslepoix/Qucs-RFlayout)), discussing with other meshers authors, observing meshers behavior and looking into their code, I identified some pitfalls that end up to usability or mesh correctness issues :

- All polygon edges should be detected and not just the edges of the polygon minimal bounding boxes.
- The colinear edges detection is not related to the mesh and thus should be treated _independantly_, before any thirds rule application or smoothmesh generation.
- The user should be able to give directives to customize the mesh / solve meshlines conflicts through a decent interface.
- The [thirds rule](https://openems.de/index.php/FDTD_Mesh.html) should not be applied statically at the chosen mesh resolution before smoothmeshing, but should be applied _while_ smoothmeshing. The reason is fixed meshlines make the structure really difficult to smoothmesh correctly. Also this way is not possible to correctly handle structure parts that are equivalent or thinner than the mesh resolution.

Those considerations led me to think the key to create a correct mesher is about conflict management system :
- Identifying and classifying conflict kinds.
- Registering conflicts.
- Providing automatic resolution.
- Providing access to resolve manually.

Also I'd like this mesher to be a standalone tool, independant from any converter or scripting API.

<!--
## Build & usage
### Dependencies
-->

## Developping

### Additional dependencies

- [Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository)
- `gcov`
- `lcov`

### Build system configuration

- `-DCMAKE_BUILD_TYPE=<Release|Debug|Coverage>`
- `make [check|checkall|coverage]`