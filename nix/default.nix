{
  stdenv,
  pkgs,
  ...
}:
stdenv.mkDerivation {
  pname = "pong";
  version = "git";
  src = ../.;
  dontConfigure = true;

  buildInputs = with pkgs; [
    gnumake
    gcc
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
  ];

  installPhase = ''
    mkdir -pv $out/bin
    install -Dvm755 ./bin/* $out/bin/
  '';
}
