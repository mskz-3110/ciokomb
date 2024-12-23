import sys
import os
sys.path.append("""{}/build""".format(os.path.dirname(__file__)))
from cbuildon_scripts import *

def cmake_build_macos(isClean):
  configurations = ["Debug", "Release"]
  for configuration in configurations:
    buildDirectory = "build"
    if os.path.isdir(buildDirectory) is False:
      mkdir(buildDirectory)
      command([
        "cmake",
        "-G", "Xcode",
        "-D", "CMAKE_OSX_ARCHITECTURES=\"x86_64;arm64\"",
        "-B", buildDirectory,
      ])
    elif isClean:
      command([
        "cmake",
        "--build", buildDirectory,
        "--config", configuration,
        "--target", "clean",
      ])
    command([
      "cmake",
      "--build", buildDirectory,
      "--config", configuration,
    ])

def macos_build(isClean):
  oldDir = getdir()
  chdir("build/macos")
  cmake_build_macos(isClean)
  print(find("build/**/*ciokomb.*"))
  chdir(oldDir)

def macos_test(argv):
  pass

chdir(os.path.dirname(__file__))
argv = sys.argv[1:]
taskName = shift(argv, "")
match taskName:
  case "windows.build":
    windows_build(argv, False)
  case "windows.rebuild":
    windows_build(argv, True)
  case "windows.test":
    windows_test(argv)
  case "android.build":
    android_build(argv, False)
  case "android.rebuild":
    android_build(argv, True)
  case "docker.build":
    command(["docker", "compose", "build"])
  case "docker.run":
    command(["docker", "compose", "run", "--rm", "debian", "bash"])
  case "linux.build":
    linux_build(False)
  case "linux.rebuild":
    linux_build(True)
  case "linux.test":
    linux_test(argv)
  case "macos.build":
    macos_build(False)
  case "macos.rebuild":
    macos_build(False)
  case "macos.test":
    macos_test(argv)
  case _:
    strings = []
    if 0 < len(taskName):
      strings.append("""\033[40m\033[31m{}\033[0m is undefined.""".format(taskName))
    strings.append("""<Tasks>
  windows.build <yaml file paths>

  windows.rebuild <yaml file paths>

  windows.test <test names>

  android.build <yaml file paths>

  android.rebuild <yaml file paths>

  docker.build

  docker.run

  linux.build

  linux.rebuild

  linux.test <test names>

  macos.build

  macos.rebuild

  macos.test <test names>
""")
    sys.exit("\n".join(strings))
