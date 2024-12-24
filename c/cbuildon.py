import sys
import os
sys.path.append("""{}/build""".format(os.path.dirname(__file__)))
from cbuildon_scripts import *

def move_lib(libDir, buildDir, exts):
  for ext in exts:
    for path in find("""{}/**/*.{}""".format(buildDir, ext)):
      mkdir(libDir)
      move(path, libDir)

def cmake_build(osName, libRootDir, buildConfig, isClean):
  for generator in buildConfig.keys():
    match osName:
      case "ios":
        for combination in buildConfig[generator]:
          macosTarget, sdk, archs, configuration = combination.split(" ")
          buildDir = """{}""".format(configuration)
          libDir = os.path.abspath("""{}_{}_{}""".format(libRootDir, sdk, buildDir))
          buildDir = """build/{}""".format(buildDir)
          if isClean:
            rm(buildDir)
          if os.path.isdir(buildDir) is False:
            mkdir(buildDir)
            command([
              "cmake",
              "-G", "Xcode",
              "-D", "CMAKE_MACOSX_BUNDLE=YES",
              "-D", "CMAKE_SYSTEM_NAME=iOS",
              "-D", "CMAKE_OSX_SYSROOT=iphoneos",
              "-D", """CMAKE_OSX_DEPLOYMENT_TARGET={}""".format(macosTarget),
              "-D", """LIB_DIR={}""".format(libDir),
              "-B", buildDir,
            ])
          options = []
          for arch in archs.split(";"):
            options.append("-arch")
            options.append(arch)
          command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
              "--",
              "CODE_SIGNING_REQUIRED=NO",
              "CODE_SIGNING_ALLOWED=NO",
              "-sdk", sdk,
            ] + options)
          for path in find("""{}/**/*.a""".format(buildDir)):
            command(["xcrun", "lipo", "-info", path])
      case "macos":
        for combination in buildConfig[generator]:
          arch, configuration = combination.split(" ")
          buildDir = """{}_{}""".format(arch, configuration)
          libDir = os.path.abspath("""{}_{}""".format(libRootDir, buildDir))
          buildDir = """build/{}""".format(buildDir)
          if isClean:
            rm(buildDir)
          if os.path.isdir(buildDir) is False:
            mkdir(buildDir)
            command([
              "cmake",
              "-G", "Xcode",
              "-D", """CMAKE_OSX_ARCHITECTURES={}""".format(arch),
              "-D", """LIB_DIR={}""".format(libDir),
              "-B", buildDir,
            ])
          command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
            ])
          move_lib(libDir, buildDir, ["dylib", "a"])

def version(osName):
  stdout = ""
  match osName:
    case "macos" | "ios":
      stdout = command(["sw_vers", "--productVersion"], stdout = subprocess.PIPE).stdout
    case _:
      return "?"
  return stdout.decode("utf-8").rstrip()

def build_config_paths(osName, argv):
  if len(argv) == 0:
    return find("""build/{}/build*.yaml""".format(osName))
  cwd = getdir()
  paths = []
  for arg in argv:
    path = os.path.relpath("""{}/{}""".format(cwd, arg))
    exists_assert(path)
    paths.append(path)
  return paths

def build(osName, isClean):
  oldDir = getdir()
  libRootDir = """{}/lib/{}/{}""".format(oldDir, osName, version(osName))
  for path in build_config_paths(osName, argv):
    with open(path, "r", encoding = "utf-8") as file:
      buildConfig = yaml.safe_load(file)
      for dirName in ["lib", "tests"]:
        buildDir = """build/{}/{}""".format(osName, dirName)
        if os.path.isdir(buildDir):
          chdir(buildDir)
          cmake_build(osName, libRootDir, buildConfig, isClean)
          chdir(oldDir)

def test_names(testNames = []):
  if len(testNames) == 0:
    return list(map(lambda filePath: os.path.splitext(os.path.basename(filePath))[0], find("tests/*.c") + find("tests/*.cpp")))
  else:
    return testNames

def test(osName, argv):
  for testName in test_names(argv):
    for path in find("""build/{}/tests/build/**/{}""".format(osName, testName)):
      print(path)
      command([path])

chdir(os.path.dirname(__file__))
argv = sys.argv[1:]
taskName = shift(argv, "")
match taskName:
  case "docker.build":
    command(["docker", "compose", "build"])
  case "docker.run":
    command(["docker", "compose", "run", "--rm", "debian", "bash"])
  case "linux.build":
    linux_build(False)
  case "linux.rebuild":
    linux_build(True)
  case "linux.test":
    test("linux", argv)
  case "windows.build":
    windows_build(argv, False)
  case "windows.rebuild":
    windows_build(argv, True)
  case "windows.test":
    test("windows", argv)
  case "android.build":
    android_build(argv, False)
  case "android.rebuild":
    android_build(argv, True)
  case "macos.build":
    build("macos", False)
  case "macos.rebuild":
    build("macos", True)
  case "macos.test":
    test("macos", argv)
  case "ios.build":
    build("ios", False)
  case "ios.rebuild":
    build("ios", True)
  case _:
    strings = []
    if 0 < len(taskName):
      strings.append("""\033[40m\033[31m{}\033[0m is undefined.""".format(taskName))
    strings.append("""<Tasks>
  docker.build

  docker.run

  linux.build

  linux.rebuild

  linux.test <test names>

  windows.build <yaml file paths>

  windows.rebuild <yaml file paths>

  windows.test <test names>

  android.build <yaml file paths>

  android.rebuild <yaml file paths>

  macos.build <yaml file paths>

  macos.rebuild <yaml file paths>

  macos.test <test names>

  ios.build <yaml file paths>

  ios.rebuild <yaml file paths>
""")
    sys.exit("\n".join(strings))
