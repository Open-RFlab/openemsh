{
  description = "OpenEMS mesher draft";

  nixConfig.bash-prompt-suffix = "(openemsh) ";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/8cec3cc";

    utils.url = "github:numtide/flake-utils";
    utils.inputs.nixpkgs.follows = "nixpkgs";

    cmake-utils-src.url = "github:conformism/cmake-utils";
  };

  outputs = { self
    , nixpkgs
    , cmake-utils-src
    , ...
    }@inputs:
    inputs.utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };

      cmake-utils = import cmake-utils-src {
        inherit pkgs;
      };

      cmake-utils-dev = import cmake-utils-src {
        inherit pkgs;
        need-all = true;
      };

      this-package = pkgs.callPackage ./default.nix {
        inherit pkgs cmake-utils;
        stdenv = pkgs.llvmPackages_13.stdenv;
      };

    in {
      devShell = with pkgs; mkShell rec {
        inputsFrom = [
          this-package
          cmake-utils-dev
        ];
      };

      devShells = {
        doc = pkgs.mkShell {
          packages = [
            (pkgs.python3.withPackages (python-pkgs: [
              python-pkgs.numpy
              python-pkgs.matplotlib
            ]))
          ];
        };
      };

      defaultPackage = this-package;
    });
}
