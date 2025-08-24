{
  description = "OpenEMS mesher draft";

  nixConfig.bash-prompt-suffix = "(openemsh) ";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";

    # Known to work version, nixpkgs is often broken for cross compilation.
    nixpkgs-mingw.url = "github:NixOS/nixpkgs/58a1abdbae3217ca6b702f03d3b35125d88a2994";

    nix-filter.url = "github:numtide/nix-filter";

    flake-utils.url = "github:numtide/flake-utils";

    cmake-utils = {
      url = "github:conformism/cmake-utils";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };

    nixGL = {
      url = "github:nix-community/nixGL";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs = { self
  , nixpkgs
  , nixpkgs-mingw
  , nix-filter
  , cmake-utils
  , flake-utils
  , nixGL
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

    pkgs-mingw = nixpkgs-mingw.legacyPackages.${system}.appendOverlays [
      (final: prev: { inherit (pkgs) cli11; })
      cmake-utils.overlays.pkgs
      self.overlays.pkgs
    ];

    wineWrapper = package: pkgs.stdenvNoCC.mkDerivation {
      inherit (package) pname version meta;
      doCheck = false;
      dontUnpack = true;
      dontBuild = true;
      installPhase =
      let
        script = ''
          #!/bin/sh
          exec ${pkgs.wine64}/bin/wine64 ${package}/bin/${package.pname}.exe "$@"
        '';
      in ''
        mkdir -p $out/bin
        printf '${script}' > $out/bin/${package.pname}
        chmod +x $out/bin/${package.pname}
        mkdir -p $out/share
        ${pkgs.xorg.lndir}/bin/lndir -silent ${package}/share $out/share
      '';
    };

    zipWrapper = package: pkgs.stdenvNoCC.mkDerivation {
      inherit (package) pname version meta;
      doCheck = false;
      dontUnpack = true;
      nativeBuildInputs = [ pkgs.zip ];
      buildPhase = ''
        cp -rL ${package} ${package.name}
        chmod -R ug+w ${package.name}
        zip -r ${package.name}.zip ${package.name}/*
      '';
      installPhase = ''
        mkdir -p $out
        mv ${package.name}.zip $out/
      '';
    };

    debianSource = package: { distributions ? [ "unstable" ], metadata ? [ "urgency=medium" ] }:
    pkgs.stdenvNoCC.mkDerivation {
      inherit (package) version meta src;
      pname = "${package.pname}-deb-source";
      doCheck = false;
      nativeBuildInputs = [ pkgs.dpkg ];
      unpackPhase = ''
        cp -r ${package.src} ${package.name}
        chmod -R u+w ${package.name}
      '';
      patchPhase = ''
        mv ${package.name}/pack/debian ${package.name}/debian
        # Add APT stuff and reformat timestamps
        # https://manpages.debian.org/testing/dpkg-dev/deb-changelog.5.en.html
        sed < ${package.name}/CHANGELOG -E \
          -e 's/^(\S+ \(\S+\))$/\1 ${toString distributions}; ${lib.strings.concatStringsSep "," metadata}/g' \
          -e 's/^( -- .+ <\S+@\S+>  )([0-9]{1,2} [a-zA-Z]{3} [0-9]{4})$/echo "\1$(date -R --date "\2")"/e' \
        > ${package.name}/debian/changelog
        cat ${package.name}/debian/changelog
      '';
      buildPhase = ''
        dpkg-source -b ${package.name}
      '';
      installPhase = ''
        mkdir -p $out
        mv ${package.pname}_${package.version}.* $out/
      '';
    };

  in {
    devShells = {
      default = pkgs.mkShell {
        inputsFrom = [
          pkgs.openemsh
          pkgs.cmake-utils-full
        ];
      };

      openems = pkgs.mkShell {
        packages = [
          nixGL.packages.${system}.nixGLDefault
          pkgs.openems
          pkgs.appcsxcad
          (pkgs.octaveFull.withPackages (p: [
            pkgs.octave-openems
            pkgs.octave-csxcad
            pkgs.octave-openems-hll
          ]))
        ];
      };

      doc = pkgs.mkShell {
        packages = [
          pkgs.d2
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
      openemshMingw64 = pkgs-mingw.openemshMingw64;
      openemshMingw64Zip = zipWrapper pkgs-mingw.openemshMingw64;
      openemshWine64 = wineWrapper (pkgs-mingw.openemshMingw64.override { withPortabilityTweaks = true; });
      openemshDebSource = debianSource pkgs.openemsh {};
    };
  }) // {
    overlays = {
      pkgs = final: prev: {
        openemsh = prev.qt6.callPackage ./default.nix {
          inherit lib;
        };

        openemshMingw64 = prev.pkgsCross.mingwW64.qt6.callPackage ./default.nix {
          inherit lib;
          inherit (final) texlive;
        };

        inherit (prev.python3Packages) cairosvg;

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
          version = "5783fd4";
          src = prev.fetchFromGitHub {
            owner = "Open-RFlab";
            repo = "octave-openems-hll";
            rev = version;
            hash = "sha256-CIcWWAQ/bxo7DAZj1m5ekkUweY0gdebzbGvChVJgx4Y=";
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
