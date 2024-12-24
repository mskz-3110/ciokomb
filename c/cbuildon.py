import sys
import os
sys.path.append("""{}/build""".format(os.path.dirname(__file__)))
from cbuildon_scripts import *

def macos_version():
  return str(command(["sw_vers", "--productVersion"], stdout = subprocess.PIPE).stdout).rstrip()

def cmake_build_macos(libRootDir, isClean):
  for arch in ["x86_64"]:
    for configuration in ["Debug", "Release"]:
      buildDirectory = """build/{}_{}""".format(arch, configuration)
      libDir = os.path.abspath("""{}/{}""".format(libRootDir, configuration))
      buildArgs = [
          "cmake",
          "--build", buildDirectory,
          "--config", configuration,
        ]
      if os.path.isdir(buildDirectory) is False:
        mkdir(buildDirectory)
        command([
          "cmake",
          "-G", "Xcode",
          "-D", """CMAKE_OSX_ARCHITECTURES={}""".format(arch),
          "-D", """LIB_DIR={}""".format(libDir),
          "-B", buildDirectory,
        ])
      elif isClean:
        command(buildArgs + ["--target", "clean"])
      command(buildArgs, lambda _: None)
      print("Build completed")
      print(find("""{}/**/*.a""".format(buildDirectory)))
      for path in find("""{}/**/*.dylib""".format(buildDirectory)) + find("""{}/**/*.a""".format(buildDirectory)):
        print("""move: {} -> {}""".format(path, libDir))
        mkdir(libDir)
        move(path, libDir)

def macos_build(isClean):
  oldDir = getdir()
  libRootDir = """{}/lib/macos/{}""".format(oldDir, macos_version())
  for targetDirectory in ["build/macos/lib", "build/macos/tests"]:
    chdir(targetDirectory)
    cmake_build_macos(libRootDir, isClean)
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
