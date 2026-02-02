{ lib
, stdenv
, cmake
, cmake-utils
, git
, texlive
, cli11
, indicators
, pugixml
, qtbase
, wrapQtAppsHook
, fetchzip
, runCommand
, withPortabilityTweaks ? false
}:

stdenv.mkDerivation {
  pname = "openemsh";
  version =
  let
    firstLine = builtins.elemAt (lib.strings.split "\n" (builtins.readFile ./CHANGELOG)) 0;
  in builtins.elemAt (builtins.match "^[^ ]+ \\((.*)\\)$" firstLine) 0;

  src = lib.nix-filter {
    root = ./.;
    include = [
      "cmake"
      "src"
      "debian"
      "doc"
      "icon"
      "pack"
      "test"
      "CMakeLists.txt"
      "CMakePresets.json"
      "CHANGELOG"
    ];
  };

  nativeBuildInputs = [
    cmake
    cmake-utils
    git
    wrapQtAppsHook
    (texlive.combine {
      inherit (texlive) scheme-small standalone pgfplots unicode-math;
      # https://ctan.org/tex-archive/fonts/lete-sans-math
      # TODO use buildTeXLivePackage
#      lete-sans-math = pkgs.texlivePackages.buildTeXLivePackage {
##        revision = 64540;
#        shortdesc = "Lato-based OpenType Math font for LuaTeX and XeTeX";
##        stripPrefix = 0;
##        fontMaps = [
##          "Map andika.map"
##        ];
##        sha512.run = "4da9904459345033aa87deeb0019c8c4a39fbafcd59d973717ed2c4a410ece528944c69669b6a5ecf6ef8bb790f60bba909468e001485c405e7cf8775b7533e5";
##        sha512.doc = "50684857dc25ad942aff18eedec04c9e27e4e408f748c208f8527c5096e600d26769ec6f82e2f02e72ab472dfb662f12bb009156293a12daa2dc0676d63ed446";
#        hasRunfiles = false;
#        license = [ "lppl13c" ];
#        version = "0.41";
#      };
      lete-sans-math = {
        pkgs = [
          (runCommand "lete-sans-math" rec {
            src = fetchzip {
              url = "https://github.com/abccsss/LeteSansMath/archive/refs/tags/v${passthru.version}.zip";
              hash = "sha256-q+WwdSUeHIffpgrJ6GKzyQz+iNxUTeHr4GQZsT0r1Kw=";
            };
            passthru = {
              pname = "lete-sans-math";
              version = "0.50";
              tlType = "run";
            };
          } ''
            mkdir -p \
              $out/tex/latex/lete-sans-math/ \
              $out/fonts/opentype/public/lete-sans-math/
            cp $src/*.sty $src/*.ltx $out/tex/latex/lete-sans-math/
            cp $src/*.otf $out/fonts/opentype/public/lete-sans-math/
          '')
        ];
      };
    })
  ];

  buildInputs = [
    cli11
    indicators
    (pugixml.override { shared = true; })
    qtbase
  ];

  cmakeFlags = [
    (lib.cmakeBool "CPM_DISABLE" true)
    (lib.cmakeBool "OEMSH_PORTABILITY_TWEAKS" withPortabilityTweaks)
  ];

  shellHook = ''
    unset NIX_HARDENING_ENABLE
    export CPM_DISABLE=ON
  '';

  postInstall = lib.optionals stdenv.hostPlatform.isWindows ''
    mkdir -p $out/bin/platforms
    ln -t $out/bin/platforms -s ${qtbase}/lib/qt-6/plugins/platforms/qwindows.dll
  '';

  dontWrapQtApps = stdenv.hostPlatform.isWindows;

  QT_XCB_GL_INTEGRATION = "none";

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
