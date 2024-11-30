{
  description = "OpenEMS mesher draft";

  nixConfig.bash-prompt-suffix = "(openemsh) ";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/22.11";

    nix-filter.url = "github:numtide/nix-filter";

    flake-utils.url = "github:numtide/flake-utils";

    cmake-utils = {
      url = "github:conformism/cmake-utils";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs = { self
  , nixpkgs
  , nix-filter
  , cmake-utils
  , flake-utils
  , ...
  }@inputs:
  let
    lib = nixpkgs.lib.extend nix-filter.overlays.default;

  in flake-utils.lib.eachDefaultSystem (system:
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
        packages = [
          pkgs.openems
          pkgs.appcsxcad
          (pkgs.octave.withPackages (p: [
            pkgs.octave-openems
            pkgs.octave-csxcad
            pkgs.octave-openems-hll
          ]))
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
        openemsh = prev.qt6.callPackage ./default.nix {
          inherit lib;
          stdenv = prev.llvmPackages_13.stdenv;
        };

        inherit (prev.python310Packages) cairosvg;

        csxcad = (prev.csxcad.overrideAttrs (new: old: {
          version = "0.6.3";
          src = prev.fetchFromGitHub {
            owner = "thliebig";
            repo = "CSXCAD";
            rev = "v${new.version}";
            hash = "sha256-SSV5ulx3rCJg99I/oOQbqe+gOSs+BfcCo6UkWHVhnSs=";
          };
          prePatch = ''
            substituteInPlace CMakeLists.txt --replace-fail 'INSTALL(DIRECTORY matlab DESTINATION share/CSXCAD)' ""
          '';
          outputs = [ "out" "dev" ];
        })).override {
          stdenv = prev.fastStdenv;
        };

        qcsxcad = (prev.qcsxcad.overrideAttrs (new: old: {
          nativeBuildInputs = old.nativeBuildInputs ++ [
            prev.libsForQt5.wrapQtAppsHook
          ];
        })).override {
          mkDerivation = prev.fastStdenv.mkDerivation;
          inherit (final) csxcad;
        };

        appcsxcad = (prev.appcsxcad.overrideAttrs (new: old: {
          nativeBuildInputs = old.nativeBuildInputs ++ [
            prev.libsForQt5.wrapQtAppsHook
          ];
        })).override {
          mkDerivation = prev.fastStdenv.mkDerivation;
          inherit (final) csxcad qcsxcad;
        };

        openems = (prev.openems.overrideAttrs (new: old: {
          version = "0.0.36";
          src = prev.fetchFromGitHub {
            owner = "thliebig";
            repo = "openEMS";
            rev = "v${new.version}";
            hash = "sha256-wdH+Zw7G2ZigzBMX8p3GKdFVx/AhbTNL+P3w+YjI/dc=";
          };
          postFixup = ""; # Avoid oct file building / setup.m patching.
          prePatch = ''
            substituteInPlace CMakeLists.txt --replace-fail 'INSTALL( DIRECTORY matlab DESTINATION share/openEMS )' ""
          '';
          outputs = [ "out" "dev" ];
        })).override {
          stdenv = prev.fastStdenv;
          inherit (final) csxcad qcsxcad;
        };

        octave-csxcad = prev.octavePackages.buildOctavePackage rec {
          inherit (final.csxcad) pname version meta src;
          patchPhase =
          let
            description = ''
              Name: csxcad
              Version: ${version}
              Date: 2016-07-06
              Author: Thorsten Liebig
              Maintainer: Thomas Lepoix <thomas.lepoix@protonmail.ch>
              Title: C++ library to describe geometrical objects
              Description: Signal processing tools, including filtering, windowing and display functions.
              Categories: Electromagnetics
              License: GPLv3
              Url: https://github.com/thliebig/CSXCAD
            '';
          in ''
            sed -i matlab/CSXGeomPlot.m \
              -e '30,36d'
            substituteInPlace matlab/CSXGeomPlot.m --replace-fail \
              "command = [AppCSXCAD_bin" \
              "command = ['env AppCSXCAD'"

            echo "TMPDIR: $TMPDIR"

            mkdir -p $TMPDIR/out
            mv ./NEWS $TMPDIR/out/
            mv ./COPYING $TMPDIR/out/
            mv ./matlab $TMPDIR/out/inst
            cat > $TMPDIR/out/DESCRIPTION <<EOF
            ${description}
            EOF
            rm -rf *
            mv $TMPDIR/out/* .

          '';
          propagatedBuildInputs = [
            final.appcsxcad
          ];
        };

        octave-openems = prev.octavePackages.buildOctavePackage rec {
          inherit (final.openems) pname version meta src;
          requiredOctavePackages = [
            final.octave-csxcad
          ];
          buildInputs = [
            prev.hdf5-cpp
          ];
          propagatedBuildInputs = [
            final.openems
            final.appcsxcad
          ];
          patchPhase =
          let
            description = ''
              Name: openems
              Version: ${version}
              Date: 2016-07-06
              Author: Thorsten Liebig
              Maintainer: Thomas Lepoix <thomas.lepoix@protonmail.ch>
              Title: Electromagnetic field solver
              Description: Signal processing tools, including filtering, windowing and display functions.
              Categories: Electromagnetics
              License: GPLv3
              Url: https://github.com/thliebig/openEMS
            '';
          in ''
            sed -i matlab/private/invoke_openEMS.m \
              -e '29,35d'
            substituteInPlace matlab/private/invoke_openEMS.m --replace-fail \
              "command = [openEMS_bin" \
              "command = ['env openEMS'"
            sed -i matlab/CalcNF2FF.m \
              -e '101,107d' \
              -e '124,126d'
            substituteInPlace matlab/CalcNF2FF.m --replace-fail \
              "system(['export LD_LIBRARY_PATH=; ' nf2ff_bin" \
              "system(['env nf2ff'"

            mkdir -p $TMPDIR/out/
            mv ./NEWS $TMPDIR/out/
            mv ./COPYING $TMPDIR/out/
            rm ./matlab/setup.m
            mv ./matlab $TMPDIR/out/inst
            cat > $TMPDIR/out/DESCRIPTION <<EOF
            ${description}
            EOF
            rm -rf *
            mv $TMPDIR/out/* .

            cd inst
            mkoctfile \
              -I${prev.hdf5-cpp}/include \
              -L${prev.hdf5-cpp}/lib \
              -lhdf5_cpp \
              --verbose \
              h5readatt_octave.cc
            rm h5readatt_octave.cc
            cd ..
          '';
        };

        octave-openems-hll = prev.octavePackages.buildOctavePackage rec {
          pname = "openems-hll";
          version = "ec5e1c1";
          src = prev.fetchFromGitHub {
            owner = "Open-RFlab";
            repo = "octave-openems-hll";
            rev = version;
            hash = "sha256-/RITOhGOwVjUuttDdiYQ7IaDhD7j9NB24PRI1tZRFJY=";
          };
          requiredOctavePackages = [
            final.octave-openems
            final.octave-csxcad
          ];
          propagatedBuildInputs = [
            prev.imagemagick
            prev.graphicsmagick
            prev.ghostscript
          ];
          meta = {
            homepage = "https://github.com/Open-RFlab/octave-openems-hll";
            description = "OpenEMS High-level layer";
            license = prev.lib.licenses.gpl3;
            maintainers = [
              {
                email = "thomas.lepoix@protonmail.ch";
                github = "thomaslepoix";
                githubId = 26417323;
                name = "Thomas Lepoix";
              }
            ];
          };
        };
      };
    };
  };
}
