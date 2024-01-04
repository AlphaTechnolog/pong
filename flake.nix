{
	inputs = {
		nixpkgs.url = github:nixos/nixpkgs;
		flake-utils.url = github:numtide/flake-utils;
	};

	outputs = { self, nixpkgs, flake-utils }: let
		system = "x86_64-linux";
		pkgs = nixpkgs.legacyPackages.${system};
	in {
		devShells."${system}".default = with pkgs; mkShell {
			buildInputs = [ gnumake gcc SDL2 SDL2_image SDL2_ttf SDL2_mixer cjson ];
		};
	};
}
