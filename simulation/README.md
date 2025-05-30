
## urdf to proto    https://github.com/cyberbotics/urdf2webots 
python3 -m urdf2webots.importer \
  --input="/Users/jerrywu/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Roboland/Multi-Agent/BECOv2/simulation/Webots/urdf/BECOV2.urdf" \
  --output="/Users/jerrywu/Library/Mobile Documents/com~apple~CloudDocs/Desktop/Roboland/Multi-Agent/BECOv2/simulation/Webots/protos/BECOV2BOX.proto" \
  --normal


# Solid format in world

Solid {
  translation 0 0 0 
  rotation 1 0 0 1.5708
  children [
    Shape {
      appearance PBRAppearance {
        baseColor 0.2 0.2 1.0
        roughness 0.5
        metalness 0.0
      }
      geometry Mesh {
        url "../../Blender/Terrain/terrain7/terrain7.obj"
      }
    }
  ]
  boundingObject Mesh {
    url "../../Blender/Terrain/terrain7/terrain7.obj"
  }
  physics Physics {
    mass 10
  }
  name "Terrain7"
}