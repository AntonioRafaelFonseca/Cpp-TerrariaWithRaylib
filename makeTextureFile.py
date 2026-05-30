import os
print("start make file")
arr = []

arr.append('#pragma once\n#include "raylib.h"\n')
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

with open("textures.hpp", "w") as f:
  for i in arr:
    f.write(i)

print("start compile")