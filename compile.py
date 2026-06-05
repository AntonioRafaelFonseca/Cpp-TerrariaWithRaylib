import subprocess
import os
import pyautogui
import time

compile_cmd = [
    "g++", "-std=c++20",
    "main.cpp", "entity.cpp",
    "-o", "main.exe",
    "-IC:\\raylib\\raylib\\src",  # path to raylib.h
    "-LC:\\raylib\\raylib\\src",  # path to libraylib.a
    "-lraylib",                   # link the Raylib library
    "-lopengl32", "-lgdi32", "-lwinmm"  # Windows dependencies
]

result = subprocess.run(
    compile_cmd,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    encoding="utf-8",
    errors="replace"
)

if result.returncode == 0:
    print("Compilado com sucesso")
    print("-" * 34 + '\n' * 3)
    subprocess.run(['main.exe'])
else:
    print("ERRO DE COMPILACAO")
    print("-" * 34)

    if result.stdout:
        print("STDOUT:\n", result.stdout)
    if result.stderr:
        print("STDERR:\n", result.stderr)

if input('clean') != 'n' :
    os.system('cls')