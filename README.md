<img src="./icon/openemsh.svg" align="right" style="float:right;" width="15%" title="OpenEMSH"/>

[![Liberapay patrons](https://img.shields.io/liberapay/patrons/thomaslepoix.svg?logo=liberapay)](https://liberapay.com/thomaslepoix/donate)
[![Discord](https://img.shields.io/discord/616889479298547722?logo=discord)](https://discord.gg/P82fEmE)
[![License](https://img.shields.io/github/license/Open-RFlab/openemsh)](LICENSE)
[![Version](https://img.shields.io/github/v/release/Open-RFlab/openemsh)](https://github.com/Open-RFlab/openemsh/releases/latest)
[![Nix](https://img.shields.io/static/v1?logo=nixos&logoColor=white&label=&message=Built%20with%20Nix&color=41439a)](https://builtwithnix.org/)

[![CodeQL](https://img.shields.io/github/actions/workflow/status/Open-RFlab/openemsh/test_codeql.yml?label=CodeQL&logo=github)](https://github.com/Open-RFlab/openemsh/security/code-scanning?query=is%3Aopen+branch%3Amain+tool%3ACodeQL)
[![SonarCloud quality gate](https://sonarcloud.io/api/project_badges/measure?project=Open-RFlab_openemsh&metric=alert_status)](https://sonarcloud.io/dashboard?id=Open-RFlab_openemsh)
[![SonarCloud coverage](https://sonarcloud.io/api/project_badges/measure?project=Open-RFlab_openemsh&metric=coverage)](https://sonarcloud.io/dashboard?id=Open-RFlab_openemsh)

# OpenEMSH : OpenEMS Mesher [WIP]

_This project is under heavy development and not yet usable, for now the best way to contribute is to give a symbolic tip. :)_

## Abstract

This is an [OpenEMS](https://openems.de) mesher draft (with a H at the end, it looks like mesh poorly written). Meshing an OpenEMS structure is a difficult task often done manually. Some automatic meshers exist, shipped with either an Octave/Python API or a converter tool from a PCB design software.

By creating myself one of those converter ([Qucs-RFlayout](https://github.com/thomaslepoix/Qucs-RFlayout)), discussing with other meshers authors, observing meshers behavior and looking into their code, I identified some pitfalls that end up to usability or mesh correctness issues :

- All polygon edges should be detected and not just the edges of the polygon minimal bounding boxes.
- The collinear edges' detection is not related to the mesh and thus should be treated _independently_, before any thirds rule application or smoothmesh generation.
- The user should be able to give directives to customize the mesh / solve meshlines conflicts through a decent interface.
- The [thirds rule](https://wiki.openems.de/index.php/FDTD_Mesh.html) should not be applied statically at the chosen mesh resolution before smoothmeshing, but should be applied _while_ smoothmeshing. The reason is fixed meshlines make the structure really difficult to smoothmesh correctly. Also, this way is not possible to correctly handle structure parts that are equivalent or thinner than the mesh resolution.

Those considerations led me to think the key to create a correct mesher is about conflict management system :
- Identifying and classifying conflict kinds.
- Registering conflicts.
- Providing automatic resolution.
- Providing access to resolve manually.

Also, I'd like this mesher to be a standalone tool, independent of any converter or scripting API.

The `doc/` directory contains some experimentations about meshing algorithms, such as [this Python script](doc/abstract/conflict_too_close_meshline_policies/autosolve.py).

<!--
## Build & usage
### Dependencies
-->

## Developing

The development environment is based on [Nix](https://nixos.org/) (which handles dependencies) and the tools provided by [cmake-utils](https://github.com/conformism/cmake-utils). Install Nix and take a look to cmake-utils for usage information.

- `nix flake update` For updating the dev env
- `nix develop` For entering the dev env

And then proceed as usual with CMake.

## Acknowledgments

<a href="https://nlnet.nl/project/OpenEMSH/"><img src="https://nlnet.nl/logo/banner.svg" height="50" title="NLnet"/></a>
&nbsp;&nbsp;&nbsp;
<a href="https://nlnet.nl/project/OpenEMSH/"><img src="https://nlnet.nl/image/logos/NGI0Core_tag.svg" height="50" title="NGI0 Core"/></a>
