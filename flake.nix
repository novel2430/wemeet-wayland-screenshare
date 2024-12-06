{
  description = "Wemeet Wayland Screenshare";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
  };

  outputs = { self, nixpkgs }: 
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    
    libs = with pkgs; [
      # Build Tools
      gcc
      pkg-config
      git
      cmake
      ninja
      # libs
      libportal
      xorg.libXrandr
      xorg.libXdamage
      opencv
      libsForQt5.qt5.qtwayland
      libsForQt5.xwaylandvideobridge
      xdg-desktop-portal
      xdg-desktop-portal-wlr
      pipewire
      libsysprof-capture
      util-linux
      libselinux
      libsepol
    ];
  in
  {
    devShell.${system} = pkgs.mkShell {
      packages = [];
      buildInputs = libs;
      LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath libs;
    };
  };
}
