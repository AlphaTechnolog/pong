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
    mkdir -pv $out/bin $out/share
    cp -rvf ./bin/* $out/share
    for i in sprites fonts music; do cp -rvf $i $out/share; done
    cat << EOF >> $out/bin/pong
    #!${pkgs.bash}/bin/bash
    cd $out/share
    ./pong ''${@}
    EOF
    chmod +x $out/bin/pong
  '';
}
