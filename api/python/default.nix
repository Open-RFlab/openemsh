{ lib
, buildPythonPackage
, hatchling
, openemsh
, openems
, appcsxcad
}:

buildPythonPackage {
  pname = "python-${openemsh.pname}";
  inherit (openemsh) version;

  src = lib.nix-filter {
    root = ../..;
    include = [
      "api/python"
      "CHANGELOG"
      "README.md"
    ];
  };

  sourceRoot = "source/api/python";

  pyproject = true;

  build-system = [
    hatchling
  ];

  dependencies = [
    openemsh
    openems
    appcsxcad
  ];

  meta = openemsh.meta // {
    description = "Python API for OpenEMSH, OpenEMS mesher";
  };
}
