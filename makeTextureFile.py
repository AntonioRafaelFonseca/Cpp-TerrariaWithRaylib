import os
print("start make file")
arr = []

types = [
  'NONE',
  'AIR',
  'GRASS',
  'DIRT',
  'STONE',
  'LEAF',
  'BARK',
  'TORCH',
  'CRAFTER',
  'COAL',
  'WoodenPlank'.upper()
  ]

arr.append('#pragma once\n#include "raylib.h"\n#include "block_type.hpp"\n')
arr.append('struct Textures\n{')
for i in os.listdir("imgs/"):
  fileName = "Texture" + i.capitalize().replace("Texture", "").replace("texture", "").replace(".png", "")
  arr.append(f"\nstatic inline Texture2D {fileName};")
arr.append("\n\nstatic void LoadAll() {\n")
for i in os.listdir("imgs/"):
  fileName = "Texture" + i.capitalize().replace("Texture", "").replace("texture", "").replace(".png", "")
  arr.append(f"{fileName} = LoadTexture(\"imgs/{i}\");\n")
for i in os.listdir("imgs/"):
  fileName = "Texture" + i.capitalize().replace("Texture", "").replace("texture", "").replace(".png", "")
arr.append("\n}")
arr.append("\nstatic void UnloadAll() {")
for i in os.listdir("imgs/"):
  fileName = "Texture" + i.capitalize().replace("Texture", "").replace("texture", "").replace(".png", "")
  arr.append(f"\nUnloadTexture({fileName});")
arr.append("\n}\n};\n")

arr.append("inline Texture2D getTextureByType(BlockType type){\n")
first = True
for i in os.listdir("imgs/"):
    if not i.endswith(".png"): continue
    
    # Nome do Enum (ex: dirt.png -> DIRT, grass.png -> GRASS)
    enumName = i.upper().replace(".PNG", "").replace("TEXTURE", "")
    
    isType = False
    for n in types:
      if n == enumName:
        isType = True

    if not isType: continue
    # Nome da Variável da Textura (ex: TextureDirt)
    fileName = "Texture" + i.capitalize().replace("Texture", "").replace("texture", "").replace(".png", "")
    
    # O primeiro bloco usa 'if', os seguintes usam 'else if'
    if first:
        arr.append(f"    if (type == BlockType::{enumName})\n    {{\n        return Textures::{fileName};\n    }}\n")
        first = False
    else:
        arr.append(f"    else if (type == BlockType::{enumName})\n    {{\n        return Textures::{fileName};\n    }}\n")

arr.append("return Textures::TextureNo;}")

with open("textures.hpp", "w") as f:
  for i in arr:
    f.write(i)

print("start compile")