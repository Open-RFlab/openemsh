{ lib
, stdenv
, cmake
, cmake-utils
, git
, texlive
, cli11
, pugixml
, qtbase
, wrapQtAppsHook
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
      inherit (texlive) scheme-small standalone pgfplots;
    })
  ];

  buildInputs = [
    cli11
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
