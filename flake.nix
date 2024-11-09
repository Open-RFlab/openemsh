{
  description = "OpenEMS mesher draft";

  nixConfig.bash-prompt-suffix = "(openemsh) ";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/22.11";

    flake-utils.url = "github:numtide/flake-utils";

    cmake-utils = {
      url = "github:conformism/cmake-utils";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs = { self
  , nixpkgs
  , cmake-utils
  , flake-utils
  , ...
  }@inputs:
  flake-utils.lib.eachDefaultSystem (system:
  let
    pkgs = nixpkgs.legacyPackages.${system}.appendOverlays [
      cmake-utils.overlays.pkgs
      self.overlays.pkgs
    ];

  in {
    devShells = {
      default = pkgs.mkShell {
        inputsFrom = [
          pkgs.openemsh
          pkgs.cmake-utils-full
        ];
      };

      doc = pkgs.mkShell {
        packages = [
          (pkgs.python3.withPackages (python-pkgs: [
            python-pkgs.numpy
            python-pkgs.matplotlib
          ]))
        ];
      };
    };

    packages = {
      default = pkgs.openemsh;
      openemsh = pkgs.openemsh;
    };
  }) // {
    overlays = {
      pkgs = final: prev: {
        openemsh = prev.callPackage ./default.nix {
          stdenv = prev.llvmPackages_13.stdenv;
        };
      };
    };
  };
}
