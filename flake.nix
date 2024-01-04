{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    package = pkgs.callPackage ./nix {};
  in {
    packages."${system}".default = package;
    devShells."${system}".default = with pkgs;
      mkShell {
        buildInputs = [
          package
          gcc
          make
          SDL2
          SDL2_image
          SDL2_ttf
          SDL2_mixer
          cjson
        ];
      };
  };
}
