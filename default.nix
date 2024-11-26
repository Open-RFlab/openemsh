{ lib
, pkgs ? import <nixpkgs> {}
, stdenv ? pkgs.stdenv
, cmake
, cmake-utils
, git
, texlive
, pugixml
}:

with pkgs;

stdenv.mkDerivation {
  name = "openemsh";
  version = "0.0.0";

  src = ./.;

  nativeBuildInputs = [
    cmake
    cmake-utils
    git
    (texlive.combine {
      inherit (texlive) scheme-small standalone pgfplots;
    })
  ];

  buildInputs = [
    cli11
    (pugixml.override { shared = true; })
  ];

  cmakeFlags = [
    "-DCPM_DISABLE=ON"
  ];

  shellHook = ''
    unset NIX_HARDENING_ENABLE
    export CPM_DISABLE=ON
  '';

  meta = {
    homepage = "https://github.com/Open-RFlab/openemsh";
    description = "OpenEMS mesher draft";
    license = lib.licenses.gpl3;
    maintainers = [
      {
        email = "thomas.lepoix@protonmail.ch";
        github = "thomaslepoix";
        githubId = 26417323;
        name = "Thomas Lepoix";
      }
    ];
  };
}
